/* <===<===<===<===<===<===<===<===<===~===>===>===>===>===>===>===>===>===>
 * File Name:    AutoTagAlg.cpp
 * Author:       Xin-Xin MA, Hao-Kai SUN
 * Created:      2019-11-09 Sat 15:49:02 CST
 * <<=====================================>>
 * Last Updated: 2020-01-07 Tue 14:05:12 CST
 *           By: Hao-Kai SUN
 *     Update #: 209
 * <<======== COPYRIGHT && LICENSE =======>>
 *
 * Copyright © 2019 SUN Hao-Kai <spin.hk@outlook.com>. All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Emacs.  If not, see <https://www.gnu.org/licenses/>.
 *
 * ============================== CODES ==============================>>> */
#include "AutoTagAlg/AutoTagAlg.hpp"

#include "EmcRecEventModel/RecEmcShower.h"
#include "EvTimeEvent/RecEsTime.h"

#include "EventModel/Event.h"
#include "EventModel/EventHeader.h"
#include "EventModel/EventModel.h"
#include "EventNavigator/EventNavigator.h"
#include "EvtRecEvent/EvtRecEvent.h"

#include "GaudiKernel/IJobOptionsSvc.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/SmartDataPtr.h"

#include "AutoTagAlg/SignalCandidate.h"

#ifndef ENABLE_BACKWARDS_COMPATIBILITY
typedef HepGeom::Point3D<double> HepPoint3D;
#endif

StatusCode AutoTagAlg::execute()
{
    MsgStream log(msgSvc(), name());
    // log << MSG::INFO << "in execute()" << endreq;
    log << MSG::INFO << "In execute()" << endmsg;
    if (m_InfoLvl > 5) std::cout << "In execute()" << std::endl;
    ++m_TotEvtN;

    SmartDataPtr<Event::EventHeader> eventHeader(eventSvc(),
                                                 "/Event/EventHeader");
    if (!eventHeader) {
        log << MSG::FATAL << "Could not find EvtHeader" << endmsg;
        return StatusCode::FAILURE;
    }

    SmartDataPtr<EvtRecEvent> evtRecEvent(eventSvc(),
                                          "/Event/EvtRec/EvtRecEvent");
    if (!evtRecEvent) {
        log << MSG::FATAL << "Could not find EvtRecEvent" << endmsg;
        return StatusCode::FAILURE;
    }

    if (m_InfoLvl > 5) {
        std::cout << "FID: ";
        for (unsigned i = 0; i < m_fid.size(); ++i) {
            std::cout << m_fid[i] << ", ";
        }
        std::cout << std::endl;
    }
    // Get Event Info
    m_runID     = eventHeader->runNumber();
    int chrTrks = evtRecEvent->totalCharged();
    if (chrTrks < m_minTrks) return StatusCode::SUCCESS;
    if (chrTrks > m_maxTrks) return StatusCode::SUCCESS;

    int showers = evtRecEvent->totalNeutral();
    if (showers < m_minShowers) return StatusCode::SUCCESS;
    if (showers > m_maxShowers) return StatusCode::SUCCESS;
    ++m_TrkShwEvtN;

    bool findProton = false;
    if (m_runID < 0) {
        this->fillDecayChain();
        for(int i=0; i < tag_indexmc; ++i) {
            if (abs(tag_pdgid[i]) ==  2212) {
                findProton = true;
            }
            cout << "pdgid[" << i << "] = " << tag_pdgid[i] << endl;
            cout << "mothI[" << i << "] = " << tag_motheridx[i] << endl;
        }
    }
    if (!findProton) return StatusCode::SUCCESS;
    if (m_runID < 0 && m_fillMCInfo && findProton) {
        if (m_InfoLvl > 5) std::cout << "FillTruthInfo Begin!" << std::endl;
        FillTruthInfo(m_fid);
        mctp->write();
    }
    if (!GetParList()) return StatusCode::SUCCESS;

    SearchSignal();
    return StatusCode::SUCCESS;
}

void AutoTagAlg::SearchSignal()
{
    // SmartDataPtr<EvtRecTrackCol> evtRecTrackCol(eventSvc(),
    //                                             "/Event/EvtRec/EvtRecTrackCol");
    // if (!evtRecTrackCol) return;

    // SmartDataPtr<EvtRecEvent> evtRecEvent(eventSvc(),
    //                                       "/Event/EvtRec/EvtRecEvent");
    // if (!evtRecEvent) return;

    // std::cout << "In Searching ..." << std::endl;
    SmartDataPtr<Event::EventHeader> eventHeader(eventSvc(),
                                                 "/Event/EventHeader");
    if (!eventHeader) return;

    m_runID   = eventHeader->runNumber();
    m_eventID = eventHeader->eventNumber();

    std::cout << "runID:  " << m_runID << std::endl;
    // omegaXiKSelectorSignal.setEcm(m_Ecm);
    // CDDecayList signal(omegaXiKSelectorSignal);

    //omegaXiKSelectorXi0Xi0bar.setEcm(m_Ecm);
    CDDecayList signal(localSignalCandidate);
    // if (m_inclCC) {
    //     signal = DecayList(m_fid) + DecayList(getChannelCC(m_fid));
    // } else {
    //     signal = DecayList(m_fid);
    // }
    // std::cout << "Load selector into CDDecayList signal!" << std::endl;
    signal = DecayList(m_fid);
    // std::cout << "After DecayList()!" << std::endl;
    if (signal.empty()) return;

    cout << " find signal" << endl;
    ++m_SltEvtN;

    // std::cout << "Not Empty!" << std::endl;
    if (m_InfoLvl >= 1) {
        std::cout << "The number of candidates:\t" << signal.size()
                  << std::endl;
        if (m_InfoLvl >= 2) {
            if (signal.size() > 1) {
                std::cout << "*** Multi Signal: " << signal.size() << " ***"
                          << std::endl;
            }
            Print(signal);
        }
    }

    CDDecayList::iterator begin = signal.particle_begin();
    CDDecayList::iterator best  = signal.particle_end();
    CDDecayList::iterator end   = signal.particle_end();
    double minQ = 1E12, Q = 1E12;

    for (CDDecayList::iterator itr = begin; itr != end; ++itr) {
        const CDCandidate& aCandidate = (*itr).particle();

        if (m_useKinematicFit) {
            Q = 1.0;
        } else {
            Q = 1.0;
        }

        if (m_InfoLvl >= 5) std::cout << "Chisq -> " << Q << std::endl;

        if (Q < minQ) {
            minQ = Q;
            best = itr;
        }
    }

    if (best != end) {
        const CDCandidate& theBest = (*best).particle();

        if (m_InfoLvl > 5)
            std::cout << "Best Label: " << (*best).label() << std::endl;

        // tag Omega- or Omegabar+ to tell c.c. channel
        m_tagLabel     = (*best).label();
        m_totChisq     = minQ;

        if (m_InfoLvl > 5) {
            for (unsigned i = 0; i < theBest.decay().numberChildren(); ++i) {
                std::cout << "C" << i
                          << " M: " << theBest.decay().child(i).mass()
                          << std::endl;
                std::cout << "C" << i
                          << " C: " << theBest.decay().child(i).charge()
                          << std::endl;
            }
        }
        if (m_InfoLvl > 5) std::cout << "FillInfo Begin!" << std::endl;
        FillInfo(theBest);
        if (m_InfoLvl > 5) std::cout << "FillInfo End!" << std::endl;
        m_Ecmtp = m_Ecm;
        if (m_runID < 0) {
            this->fillDecayChain();
            for(int i=0; i < tag_indexmc; ++i) {
                cout << "pdgid[" << i << "] = " << tag_pdgid[i] << endl;
                cout << "mothI[" << i << "] = " << tag_motheridx[i] << endl;
            }
        }

        tp->write();
    }
}
/* ===================================================================<<< */
/* ===================== AutoTagAlg.cpp ends here ====================== */
