/* <===<===<===<===<===<===<===<===<===~===>===>===>===>===>===>===>===>===>
 * File Name:    KinematicFit.cpp
 * Author:       Xin-Xin MA, Hao-Kai SUN
 * Created:      2019-12-03 Tue 17:36:40 CST
 * <<=====================================>>
 * Last Updated: 2019-12-14 Sat 16:48:52 CST
 *           By: Hao-Kai SUN
 *     Update #: 49
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

#include "HadronInfo/EtaInfo.h"
#include "HadronInfo/KsInfo.h"
#include "HadronInfo/LamInfo.h"
#include "HadronInfo/Pi0Info.h"
#include "HadronInfo/TrackInfo.h"

#include "EvtRecEvent/EvtRecTrack.h"

double AutoTagAlg::KinematicFit(const CDCandidate& sig, vecint fid)
{
    std::vector<WTrackParameter> wtrks;
    vecint childsize;

    for (unsigned i = 0; i < fid.size(); ++i) {
        const CDCandidate& aChild = sig.decay().child(i);
        int PID                   = abs(fid[i]);
        // AddWTrack(aChild, PID, wtrks, childsize, resmass, cList);
        AddWTrack(aChild, PID, wtrks, childsize);
    }

    vecint signaltrk;
    for (unsigned i = 0; i < wtrks.size(); ++i) {
        signaltrk.push_back(i);
    }

    KalmanKinematicFit* kmfit = KalmanKinematicFit::instance();
    kmfit->init(); // clear/reset all variables
    for (unsigned i = 0; i < wtrks.size(); ++i) {
        kmfit->AddTrack(i, wtrks[i]);
    }

    // 0.011 rad, BEPCII BESIII e+ e- horizontal crossing angle
    HLV p4tot(m_Ecm * 0.011, 0, 0, m_Ecm);
    // the 0 in the following three lines is the index/kmfit_ID.
    kmfit->AddFourMomentum(0, p4tot);
    kmfit->Fit(0);
    double m_sig_chisq = kmfit->chisq(0);
    // std::cout << "chisq: " << m_sig_chisq << std::endl;

    // fill four-momentum-after-kinematic-fit
    int index = 0;
    for (unsigned i = 0; i < fid.size(); ++i) {
        int size = childsize[i];
        HLV p4(0., 0., 0., 0.);
        for (int j = 0; j < size; ++j) {
            p4 += kmfit->pfit(index);
            // std::cout << "p4 in fit " << index << kmfit->pfit(index) <<
            // std::endl;
            index++;
        }
        fillP4(m_fitP4[i], p4);
    }

    return m_sig_chisq;
}

void AutoTagAlg::AddWTrack(const CDCandidate& aChild, const int& PID,
                            std::vector<WTrackParameter>& wtrks, vecint& child)
{
    if (PID == 310) {
        KsInfo ksInfo(aChild);
        wtrks.push_back(ksInfo.wtrk());
        child.push_back(1);
        return;
    }

    //         kaon          pion          proton         e            mu
    if (PID == 321 || PID == 211 || PID == 2212 || PID == 11 || PID == 13) {
        TrackInfo trackInfo(aChild);
        wtrks.push_back(trackInfo.wtrk(PID));
        child.push_back(1);
        // std::cout << "PID=" << PID << trackInfo.wtrk(PID) << std::endl;
        return;
    }

    //        gamma
    if (PID == 22) {
        TrackInfo trackInfo;
        const EvtRecTrack* shower = aChild.finalChildren().second[0];
        wtrks.push_back(trackInfo.wtrkc(shower, 22));
        child.push_back(1);
        return;
    }

    //         pi0
    if (PID == 111) {
        Pi0Info pi0Info(aChild);
        wtrks.push_back(pi0Info.wtrk());
        child.push_back(1);
        //  std::cout << "PID=" << PID << pi0Info.wtrk() << std::endl;
        return;
    }

    //         eta
    if (PID == 221) {
        EtaInfo etaInfo(aChild);
        wtrks.push_back(etaInfo.wtrk());
        child.push_back(1);
        return;
    }

    //         Lambda
    if (PID == 3122) {
        LamInfo lmdInfo(aChild);
        // wtrks.push_back( ksInfo.wtrk(0) );
        // wtrks.push_back( ksInfo.wtrk(1) );
        wtrks.push_back(lmdInfo.wtrk());
        child.push_back(1);
        return;
    }

    std::cout << "KinematicFit::AddWTrack Error:: the particle: " << PID
              << " should be defines fisrt" << std::endl;
}
/* ===================================================================<<< */
/* ===================== KinematicFit.cpp ends here ===================== */
