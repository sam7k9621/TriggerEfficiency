#include "TriggerEfficiency/EfficiencyPlot/interface/LepEfficiency.h"

using namespace std;


MuEfficiency::MuEfficiency( const edm::ParameterSet& iConfig ):
    _tagtri( iConfig.getParameter<vector<edm::ParameterSet>>( "tagtrigger" ) ),
    _protri( iConfig.getParameter<vector<edm::ParameterSet>>( "protrigger" ) ),
    _pro ( consumes<vector<pat::Muon>>( iConfig.getParameter<edm::InputTag> ( "probe" ) ) ),
    _tag ( consumes<vector<pat::Muon>>  ( iConfig.getParameter<edm::InputTag>( "tag" ) ) )

{
    usesResource( "TFileService" );

    /*****common setting*****/
    for( const auto& tagtri : _tagtri ) {
        string         triname = tagtri.getParameter<string>( "name" );
        vector<double> etabin  = tagtri.getParameter<vector<double>>( "etabin" );
        vector<double> ptbin   = tagtri.getParameter<vector<double>>( "ptbin" );
        AddHist( "total_pt_" + triname, ptbin );
        AddHist( "total_eta_" + triname, etabin );
        AddHist( "pass_pt_" + triname, ptbin );
        AddHist( "pass_eta_" + triname, etabin );
    }
}


MuEfficiency::~MuEfficiency() {
}


void
MuEfficiency::analyze( const edm::Event& iEvent, const edm::EventSetup& iSetup ) {
    edm::Handle<vector<pat::Muon> > prohandle;
    edm::Handle<vector<pat::Muon> > taghandle;
    iEvent.getByToken( _pro, prohandle );
    iEvent.getByToken( _tag, taghandle );
    pat::Muon tag = ( *taghandle )[0];
    pat::Muon pro = ( *prohandle )[0];

    for( int i = 0; i < ( int )_tagtri.size(); i++ ) {
        /*****common setting*****/
        string         triname = _tagtri[i].getParameter<string>( "name" );
        /*****setting for tag*****/
        vector<string> hltlist = _tagtri[i].getParameter<vector<string>>( "HLT" );
        double         ptcut   = _tagtri[i].getParameter<double>( "ptcut" );
        double         etacut  = _tagtri[i].getParameter<double>( "etacut" );
        bool passtag = false;

        for( const auto& hlt : hltlist ) {
            if( tag.hasUserInt( hlt ) && tag.pt() > ptcut && fabs( tag.eta() ) < etacut ) {
                passtag = true;
            }
        }

        if( !passtag ) {
            continue;
        }

        /*****another cut for isolation or tkiso considering HLT*********/
        /*****setting for probe*****/
        hltlist = _protri[i].getParameter<vector<string>>( "HLT" );
        ptcut   = _protri[i].getParameter<double>( "ptcut" );
        etacut  = _protri[i].getParameter<double>( "etacut" );

        if( fabs( pro.eta() ) < etacut ) {
            Hist( "total_pt_" + triname ) -> Fill( pro.pt() );
        }

        if( pro.pt() > ptcut ) {
            Hist( "total_eta_" + triname ) -> Fill( pro.eta() );
        }

        for( const auto& hlt : hltlist ) {
            if( pro.hasUserInt( hlt ) ) {
                if( fabs( pro.eta() ) < etacut ) {
                    Hist( "pass_pt_" + triname ) -> Fill( pro.pt() );
                }

                if( pro.pt() > ptcut ) {
                    Hist( "pass_eta_" + triname ) -> Fill( pro.eta() );
                }
            }
        }
    }
}




void
MuEfficiency::beginJob() {
}

void
MuEfficiency::endJob() {
}

void
MuEfficiency::fillDescriptions( edm::ConfigurationDescriptions& descriptions ) {
    //The following says we do not know what parameters are allowed so do no validation
    // Please change this to state exactly what you do use, even if it is no parameters
    edm::ParameterSetDescription desc;
    desc.setUnknown();
    descriptions.addDefault( desc );
}

//define this as a plug-in
DEFINE_FWK_MODULE( MuEfficiency );