#include <memory>
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"

#include "DataFormats/Common/interface/Ptr.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Math/interface/deltaR.h"

#include <iostream>
#include <string>
using namespace std;


class DataFltr : public edm::stream::EDFilter<> {
    public:
        explicit DataFltr(const edm::ParameterSet&);
        ~DataFltr();

        static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

    private:
        virtual void beginStream(edm::StreamID) override;
        virtual bool filter(edm::Event&, const edm::EventSetup&) override;
        virtual void endStream() override;
        // ----------member data ---------------------------

        const edm::EDGetToken _musrc;
        edm::Handle<vector<pat::Muon> > muonhandle;
        const edm::EDGetToken _elsrc;
        edm::Handle<vector<pat::Electron> > elehandle;
        const edm::EDGetTokenT<vector<reco::Vertex> > _vtxsrc;
        edm::Handle<reco::VertexCollection>  vtxhandle;

        const edm::EDGetToken _hltInputTag;
        const edm::EDGetToken _hltObjectsInputTag;
        edm::Handle < edm::TriggerResults > triggerResults;
        edm::Handle<pat::TriggerObjectStandAloneCollection> triggerObjects;

        double _zmin;
        double _zmax;
        bool   _useMC;
        string _lepton;
};

DataFltr::DataFltr(const edm::ParameterSet& iConfig):
    _musrc ( consumes<vector<pat::Muon> >(iConfig.getParameter<edm::InputTag>("musrc") ) ),
    _elsrc ( consumes<vector<pat::Electron> >(iConfig.getParameter<edm::InputTag>("elsrc") ) ),
    _vtxsrc  ( consumes<vector<reco::Vertex> > (iConfig.getParameter<edm::InputTag >("vtxsrc") ) ),
    _hltInputTag (consumes<edm::TriggerResults>(iConfig.getParameter<edm::InputTag>("HLTInputTag"))),
    _hltObjectsInputTag (consumes<vector<pat::TriggerObjectStandAlone> >(iConfig.getParameter<edm::InputTag>("HLTObjectsInputTag"))),
    _zmin (iConfig.getParameter   <double>("Zmassmin")),
    _zmax (iConfig.getParameter   <double>("Zmassmax")),
    _useMC (iConfig.getParameter<bool>("useMC")),
    _lepton (iConfig.getParameter<string>("lepton_type")) 
{
  /*  if(lepton_type ==  "electron") {*/
        //produces<vector<pat::Electron>>("Tag");
        //produces<vector<pat::Electron>>("Probe");
    //}
    //else if(lepton_type == "muon") {
        //produces<vector<pat::Muon>>("Tag");
        //produces<vector<pat::Muon>>("Probe");
    /*}*/

    cout<<"zmin   "<<_zmin<<endl;
    cout<<"zmax   "<<_zmax<<endl;
    cout<<"useMC  "<<_useMC<<endl;
    cout<<"lepton "<<_lepton<<endl;
}


DataFltr::~DataFltr() {
    

}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
DataFltr::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) {
    return true;
}

// ------------ method called once each stream before processing any runs, lumis or events  ------------
void
DataFltr::beginStream(edm::StreamID) {
}

// ------------ method called once each stream after processing all runs, lumis and events  ------------
void
DataFltr::endStream() {
}

// ------------ method called when starting to processes a run  ------------
/*
void
DataFltr::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a run  ------------
/*
void
DataFltr::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when starting to processes a luminosity block  ------------
/*
void
DataFltr::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a luminosity block  ------------
/*
void
DataFltr::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
DataFltr::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
    //The following says we do not know what parameters are allowed so do no validation
    // Please change this to state exactly what you do use, even if it is no parameters
    edm::ParameterSetDescription desc;
    desc.setUnknown();
    descriptions.addDefault(desc);
}
//define this as a plug-in
DEFINE_FWK_MODULE(DataFltr);
