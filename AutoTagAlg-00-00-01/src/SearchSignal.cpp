#include "AutoTagAlg/AutoTagAlg.hpp"
// #include "EmcRecEventModel/RecEmcShower.h"
// #include "EvTimeEvent/RecEsTime.h"

// #include "EventModel/Event.h"
// #include "EventModel/EventHeader.h"
// #include "EventModel/EventModel.h"
#include "EventNavigator/EventNavigator.h"
#include "EvtRecEvent/EvtRecEvent.h"

// #include "GaudiKernel/IJobOptionsSvc.h"
// #include "GaudiKernel/MsgStream.h"
// #include "GaudiKernel/SmartDataPtr.h"
#include "AutoTagAlg/selector/SignalCandidate.h"

bool GetBestCandidate(CDDecayList& signalList, CDDecayList::iterator& best) {
    best = signalList.particle_end();
    if (signalList.empty()) {
        return false;
    }
    double minQ = 1E6, Q = 1E6;
    for (CDDecayList::iterator itr = signalList.particle_begin();
         itr != signalList.particle_end(); ++itr) {
        const CDCandidate& signal = (*itr).particle();
        if (signal.decay().child(0).charge() > 0) continue;
        Q = signal.p4().e();
        ///  cout << "Q = " << Q << endl;
        if (Q < minQ) {
            minQ = Q;
            best = itr;
        }
    }
    return true;
}

void AutoTagAlg::SearchSignal() {
    if (m_readBeamE) {
        gBeamInfoSvc->GetEcm(m_Ecm);
        localSignalCandidate.SetEcm(m_Ecm);
        gPhotonConverSvc->SetEcm(m_Ecm);
    } else {
        localSignalCandidate.SetEcm(m_Ecm);
        gPhotonConverSvc->SetEcm(m_Ecm);
    }
    CDDecayList signal(localSignalCandidate);
    signal = m_DCListSvc->DecayList(m_fid);
    if (signal.empty()) return;

    if (m_InfoLvl >= 2) {
        if (signal.size() > 1) {
            std::cout << "*** Multi Signal: " << signal.size() << " ***"
                      << std::endl;
        }
        // print(signal);
    }
    if (m_InfoLvl > 2) {
        cout << "Search for best candidate!" << endl;
    }
    CDDecayList::iterator best = signal.particle_end();
    bool status = GetBestCandidate(signal, best);
    if (best == signal.particle_end()) {
        std::cout << "Error: didnot find a best candidate" << std::endl;
        return;
    }
    if (m_InfoLvl > 2) {
        cout << "Fill Info!" << endl;
    }
    const CDCandidate& sig = (*best).particle();
    /// cout << "p4 = " << sig.p4() << endl;
    FillInfo(sig);
    if (m_InfoLvl > 2) {
        cout << "Write" << endl;
    }
    m_tupleSvc[0].Write();
}
