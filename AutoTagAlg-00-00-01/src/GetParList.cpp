/* <===<===<===<===<===<===<===<===<===~===>===>===>===>===>===>===>===>===>
 * File Name:    GetParList.cpp
 * Author:       Xin-Xin MA, Hao-Kai SUN
 * Created:      2019-12-01 Sun 20:24:15 CST
 * <<=====================================>>
 * Last Updated: 2020-01-07 Tue 19:26:38 CST
 *           By: Hao-Kai SUN
 *     Update #: 166
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

#include "EvtRecEvent/EvtRecEtaToGG.h"
#include "EvtRecEvent/EvtRecEvent.h"
#include "EvtRecEvent/EvtRecPi0.h"

#include "HadronInfo/ParticleInf.h"
#include "BesStdSelector/Selector.h"
//#include "BesStdSelector/Proton.h"

#include "AutoTagAlg/AutoTagAlg.hpp"
#include "DTagAlg/LocalKaonSelector.h"

using BesStdSelector::primaryProtonSelector;
using BesStdSelector::secondaryProtonSelector;

static void printInfo(const std::string& name, const int& primary,
                      const int& all)
{
    std::cout << "Need to reconstruct the " << name << std::endl
              << "And find " << all << " candidates in total, "
              << "in which, the number of primary ones is " << primary
              << std::endl;
}

static void printInfo(const std::string& name, const int& size)
{
    std::cout << "Need to reconstruct the " << name << std::endl
              << "And find " << size << " candidates." << std::endl;
}

bool AutoTagAlg::GetParList()
{
    if (m_InfoLvl > 5) std::cout << "Get Particle List BEGIN!" << std::endl;
    SmartDataPtr<EvtRecEvent> evtRecEvent(eventSvc(),
                                          "/Event/EvtRec/EvtRecEvent");
    if (!evtRecEvent) return false;

    SmartDataPtr<EvtRecTrackCol> evtRecTrackCol(eventSvc(),
                                                "/Event/EvtRec/EvtRecTrackCol");
    if (!evtRecTrackCol) return false;

    EvtRecTrackIterator track_begin = evtRecTrackCol->begin();
    EvtRecTrackIterator track_end =
        evtRecTrackCol->begin() + evtRecEvent->totalCharged();

    if (m_InfoLvl > 5) std::cout << "Set HadronInfo Children!" << std::endl;
    ParticleInf particleInfo;
    particleInfo.SetChildren(m_fid);
    bool RecAll = false;


    if (particleInfo.NeedProton() || RecAll) {
        cout << "rec Proton" << endl;
        // cout << "total tracks " << track_end -  track_begin  << endl;
        m_ProtonPrimaryList =
            CDProtonList(track_begin, track_end, primaryProtonSelector);
        m_ProtonAllList =
            CDProtonList(track_begin, track_end, secondaryProtonSelector);

        if (particleInfo.NeedProton() && m_ProtonPrimaryList.empty() &&
            m_ProtonAllList.empty()) {
            return false;
        }
        
        if (m_InfoLvl > 6) {
            printInfo(particleInfo.Name(2212), m_ProtonPrimaryList.size(),
                      m_ProtonAllList.size());
        }
    }
    if (particleInfo.NeedPion() || RecAll) {
        m_PionPrimaryList = CDChargedPionList(track_begin, track_end,
                                              BesStdSelector::primaryPionSelector);
        m_PionAllList =
            CDChargedPionList(track_begin, track_end, BesStdSelector::secondaryPionSelector);

        if (particleInfo.NeedPion() && m_PionPrimaryList.empty() &&
            m_PionAllList.empty())
            return false;

        if (m_InfoLvl > 6) {
            printInfo(particleInfo.Name(211), m_PionPrimaryList.size(),
                      m_PionAllList.size());
            // Print(m_PionAllList);
        }
    }

    if (particleInfo.NeedKaon() || RecAll) {
        m_KaonPrimaryList = CDChargedKaonList(track_begin, track_end,
                                              BesStdSelector::primaryKaonSelector);
        m_KaonAllList =
            CDChargedKaonList(track_begin, track_end, BesStdSelector::secondaryKaonSelector);

        if (particleInfo.NeedKaon() && m_KaonPrimaryList.empty() &&
            m_KaonAllList.empty())
            return false;

        if (m_InfoLvl > 6) {
            printInfo(particleInfo.Name(321), m_KaonPrimaryList.size(),
                      m_KaonAllList.size());
            // Print(m_KaonAllList);
        }
    }

    if (particleInfo.NeedPi0() || RecAll) {
        if (m_InfoLvl > 5) std::cout << "Need Pi0 BEGIN!" << std::endl;

        SmartDataPtr<EvtRecPi0Col> recPi0Col(
            Algorithm::eventSvc(), "/Event/EvtRec/EvtRecPi0Col");

        if (!recPi0Col) return false;

        if (m_InfoLvl > 5) {
            std::cout << "Enter Pi0 selector!" << std::endl;
            std::cout << "EvtRecPi0Col Size: " << recPi0Col->size()
                      << std::endl;
            std::cout << "EvtRecPi0Col Path: " << recPi0Col.path()
                      << std::endl;
            std::cout << "EvtRecPi0Col Path: " << recPi0Col->capacity()
                      << std::endl;
        }

        SmartDataPtr<EvtRecEtaToGGCol> evtRecEtaToGGCol(
            Algorithm::eventSvc(), "/Event/EvtRec/EvtRecEtaToGGCol");
        if (!evtRecEtaToGGCol) return false;

        if (m_InfoLvl > 5) {
            std::cout << "Enter EtaToGG selector!" << std::endl;
            std::cout << "EvtRecEtaToGGCol Size: " << evtRecEtaToGGCol->size()
                      << std::endl;
            std::cout << "EvtRecEtaToGGCol Path: " << evtRecEtaToGGCol.path()
                      << std::endl;
            std::cout << "EvtRecEtaToGGCol Capacity: "
                      << evtRecEtaToGGCol->capacity() << std::endl;
        }

        CDPi0List Pi0List(BesStdSelector::pi0Selector);
        dc_fill(Pi0List, recPi0Col->begin(), recPi0Col->end());
        m_Pi0List = Pi0List;

        if (m_InfoLvl > 5) {
            std::cout << "Pass Pi0 selector!" << std::endl;
            std::cout << "Pi0List Size: " << Pi0List.size() << std::endl;
            std::cout << "m_Pi0List Size: " << m_Pi0List.size() << std::endl;
        }
        if (particleInfo.NeedPi0() && m_Pi0List.empty()) return false;

        if (m_InfoLvl > 6) {
            printInfo(particleInfo.Name(111), m_Pi0List.size());
            // Print(m_Pi0List);
        }
    }

    if (particleInfo.NeedPhoton() || RecAll) {
        if (BesStdSelector::soloPhotonSelector.vetoPi0()) {
            vector<const EvtRecPi0*> _pi0s;
            CDDecayList::iterator begin = m_Pi0List.particle_begin();
            CDDecayList::iterator end   = m_Pi0List.particle_end();

            for (CDDecayList::iterator itr = begin; itr != end; ++itr) {
                const CDCandidate& aPi0 = (*itr).particle();
                const EvtRecPi0* navPi0 = aPi0.decay().child(0).navPi0();
                _pi0s.push_back(navPi0);
            }

            BesStdSelector::soloPhotonSelector.setPi0s(_pi0s);
        }

        EvtRecTrackIterator neu_begin =
            evtRecTrackCol->begin() + evtRecEvent->totalCharged();
        EvtRecTrackIterator neu_end = evtRecTrackCol->end();

        m_PhotonList = CDPhotonList(neu_begin, neu_end, BesStdSelector::soloPhotonSelector);

        if (particleInfo.NeedPhoton() && m_PhotonList.empty()) return false;

        if (m_InfoLvl > 6) {
            printInfo(particleInfo.Name(22), m_PhotonList.size());
            // Print(m_PhotonList);
        }
    }

    if (particleInfo.NeedEta() || RecAll) {
        SmartDataPtr<EvtRecEtaToGGCol> evtRecEtaToGGCol(
            eventSvc(), "/Event/EvtRec/EvtRecEtaToGGCol");
        if (!evtRecEtaToGGCol) return false;

        CDEtaList EtaGGList(BesStdSelector::etatoGGSelector);
        dc_fill(EtaGGList, evtRecEtaToGGCol->begin(), evtRecEtaToGGCol->end());
        m_EtaList = EtaGGList;

        if (particleInfo.NeedEta() && m_EtaList.empty()) return false;

        if (m_InfoLvl > 6) {
            printInfo(particleInfo.Name(221), m_EtaList.size());
        }
    }

    if (particleInfo.NeedKs() || RecAll) {
        SmartDataPtr<EvtRecVeeVertexCol> recVeeVertexCol(
            eventSvc(), "/Event/EvtRec/EvtRecVeeVertexCol");
        if (!recVeeVertexCol) return false;

        CDKsList KsList(BesStdSelector::ksSelector);
        dc_fill(KsList, recVeeVertexCol->begin(), recVeeVertexCol->end());
        m_KsList = KsList;

        if (particleInfo.NeedKs() && m_KsList.empty()) return false;

        if (m_InfoLvl > 6) {
            printInfo(particleInfo.Name(310), m_KsList.size());
            // Print(m_KsList);
        }
    }

    if (particleInfo.NeedElectron() || RecAll) {
        m_ElectronList =
            CDElectronList(track_begin, track_end, BesStdSelector::primaryElectronSelector);
        if (m_ElectronList.empty() && particleInfo.NeedElectron()) return false;

        if (m_InfoLvl > 6) {
            printInfo(particleInfo.Name(11), m_ElectronList.size());
            // Print(m_ElectronList);
        }
    }
    return true;
}

CDDecayList AutoTagAlg::DecayList(int pid)
{
    // Ks 310 ; K 321 ; Pion 211 ; Pi0 111 ; Eta 221 ; eta' 331
    // Lambda: 3122, Omega: 3334, etaV3pi 2212211, electron 11
    CDDecayList decayList;
    switch (pid) {
        case 310:
            return m_KsList;

        case 321:
            decayList = m_KaonPrimaryList.plus();
            return decayList;

        case -321:
            decayList = m_KaonPrimaryList.minus();
            return decayList;

        case 211:
            decayList = m_PionPrimaryList.plus();
            return decayList;

        case -211:
            decayList = m_PionPrimaryList.minus();
            return decayList;

        case 11:
            decayList = m_ElectronList.plus();
            return decayList;

        case -11:
            decayList = m_ElectronList.minus();
            return decayList;

        case 111:
            decayList = m_Pi0List;
            return decayList;

        case 221:
            return m_EtaList;

        case 22:
            decayList = m_PhotonList;
            return decayList;

        case 331: { // Eta'  42.6(7)
            CDDecayList EtaP(BesStdSelector::EtaPtoPiPiEtaSelector);
            EtaP = m_PionPrimaryList.plus() * m_PionPrimaryList.minus() *
                   m_EtaList;
            return EtaP;
        }
        case 223: { // omega  89.2(7)
            CDDecayList omegaList(BesStdSelector::omegaSelector);
            omegaList =   m_PionPrimaryList.plus() * m_PionPrimaryList.minus() *
                   m_Pi0List;
            return omegaList;
        }

        case 333: { // phi 492(5)
            CDDecayList phiList(BesStdSelector::phiSelector);
            phiList = m_KaonPrimaryList.plus() * m_KaonPrimaryList.minus();
            return phiList;
        }

        case 2212:
            decayList = m_ProtonPrimaryList.plus();
            return decayList;

        case -2212:
            decayList = m_ProtonPrimaryList.minus();
            return decayList;

        case 3122: {
            CDDecayList Lambda(BesStdSelector::lambdaSelector);
            Lambda = m_ProtonAllList.plus() * m_PionAllList.minus();
            return Lambda;
        }

        case -3122: { 
            CDDecayList Lambdabar(BesStdSelector::lambdaSelector);
            Lambdabar = m_ProtonAllList.minus() * m_PionAllList.plus();
            return Lambdabar;
                    }
        case 2212211: { 
            CDDecayList Eta(BesStdSelector::etatoPiPiPi0Selector);
            Eta = m_PionPrimaryList.plus() * m_PionPrimaryList.minus() *
                  m_Pi0List;
            return Eta;
        }

        case 3212: { 
            CDDecayList Sigma0(BesStdSelector::Sigma0Selector);
            // Sigma0 = Lambda * m_PhotonList;
            Sigma0 = DecayList(3122) * m_PhotonList;
            return Sigma0;
                   }

        case -3212: {
            CDDecayList Sigma0bar(BesStdSelector::Sigma0Selector);
            // Sigma0bar = Lambdabar * m_PhotonList;
            Sigma0bar = DecayList(-3122) * m_PhotonList;
            return Sigma0bar;
                    }

        case 3334: {
            CDDecayList Omega(BesStdSelector::OmegaSelector);
            Omega = DecayList(3122) * m_KaonAllList.minus();
            return Omega;
                   }

        case -3334: {
            CDDecayList Omegabar(BesStdSelector::OmegaSelector);
            Omegabar = DecayList(-3122) * m_KaonAllList.plus();
            return Omegabar;
                    }

        default:
            return decayList;
    }
    return decayList;
}

CDDecayList AutoTagAlg::DecayList(vecint fid)
{
    switch (fid.size()) {
        case 1:
            return DecayList(fid[0]);

        case 2:
            return DecayList(fid[0]) * DecayList(fid[1]);

        case 3:
            return DecayList(fid[0]) * DecayList(fid[1]) * DecayList(fid[2]);

        case 4:
            return DecayList(fid[0]) * DecayList(fid[1]) * DecayList(fid[2]) *
                   DecayList(fid[3]);

        case 5:
            return DecayList(fid[0]) * DecayList(fid[1]) * DecayList(fid[2]) *
                   DecayList(fid[3]) * DecayList(fid[4]);

        case 6:
            return DecayList(fid[0]) * DecayList(fid[1]) * DecayList(fid[2]) *
                   DecayList(fid[3]) * DecayList(fid[4]) * DecayList(fid[5]);

        case 7:
            return DecayList(fid[0]) * DecayList(fid[1]) * DecayList(fid[2]) *
                   DecayList(fid[3]) * DecayList(fid[4]) * DecayList(fid[5]) *
                   DecayList(fid[6]);

        case 8:
            return DecayList(fid[0]) * DecayList(fid[1]) * DecayList(fid[2]) *
                   DecayList(fid[3]) * DecayList(fid[4]) * DecayList(fid[5]) *
                   DecayList(fid[6]) * DecayList(fid[7]);

        case 9:
            return DecayList(fid[0]) * DecayList(fid[1]) * DecayList(fid[2]) *
                   DecayList(fid[3]) * DecayList(fid[4]) * DecayList(fid[5]) *
                   DecayList(fid[6]) * DecayList(fid[7]) * DecayList(fid[8]);

        case 10:
            return DecayList(fid[0]) * DecayList(fid[1]) * DecayList(fid[2]) *
                   DecayList(fid[3]) * DecayList(fid[4]) * DecayList(fid[5]) *
                   DecayList(fid[6]) * DecayList(fid[7]) * DecayList(fid[8]) *
                   DecayList(fid[9]);

        case 11:
            return DecayList(fid[0]) * DecayList(fid[1]) * DecayList(fid[2]) *
                   DecayList(fid[3]) * DecayList(fid[4]) * DecayList(fid[5]) *
                   DecayList(fid[6]) * DecayList(fid[7]) * DecayList(fid[8]) *
                   DecayList(fid[9]) * DecayList(fid[10]);

        case 12:
            return DecayList(fid[0]) * DecayList(fid[1]) * DecayList(fid[2]) *
                   DecayList(fid[3]) * DecayList(fid[4]) * DecayList(fid[5]) *
                   DecayList(fid[6]) * DecayList(fid[7]) * DecayList(fid[8]) *
                   DecayList(fid[9]) * DecayList(fid[10]) * DecayList(fid[11]);

        case 13:
            return DecayList(fid[0]) * DecayList(fid[1]) * DecayList(fid[2]) *
                   DecayList(fid[3]) * DecayList(fid[4]) * DecayList(fid[5]) *
                   DecayList(fid[6]) * DecayList(fid[7]) * DecayList(fid[8]) *
                   DecayList(fid[9]) * DecayList(fid[10]) * DecayList(fid[11]) *
                   DecayList(fid[12]);

        case 14:
            return DecayList(fid[0]) * DecayList(fid[1]) * DecayList(fid[2]) *
                   DecayList(fid[3]) * DecayList(fid[4]) * DecayList(fid[5]) *
                   DecayList(fid[6]) * DecayList(fid[7]) * DecayList(fid[8]) *
                   DecayList(fid[9]) * DecayList(fid[10]) * DecayList(fid[11]) *
                   DecayList(fid[12]) * DecayList(fid[13]);

        case 15:
            return DecayList(fid[0]) * DecayList(fid[1]) * DecayList(fid[2]) *
                   DecayList(fid[3]) * DecayList(fid[4]) * DecayList(fid[5]) *
                   DecayList(fid[6]) * DecayList(fid[7]) * DecayList(fid[8]) *
                   DecayList(fid[9]) * DecayList(fid[10]) * DecayList(fid[11]) *
                   DecayList(fid[12]) * DecayList(fid[13]) * DecayList(fid[14]);

        case 16:
            return DecayList(fid[0]) * DecayList(fid[1]) * DecayList(fid[2]) *
                   DecayList(fid[3]) * DecayList(fid[4]) * DecayList(fid[5]) *
                   DecayList(fid[6]) * DecayList(fid[7]) * DecayList(fid[8]) *
                   DecayList(fid[9]) * DecayList(fid[10]) * DecayList(fid[11]) *
                   DecayList(fid[12]) * DecayList(fid[13]) *
                   DecayList(fid[14]) * DecayList(fid[15]);

        default: {
            CDDecayList decayList;
            decayList.erase();
            return decayList;
        }
    }
}
/* ===================================================================<<< */
/* ====================== GetParList.cpp ends here ====================== */
