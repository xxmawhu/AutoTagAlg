/* <===<===<===<===<===<===<===<===<===~===>===>===>===>===>===>===>===>===>
 * File Name:    FillInfo.cpp
 * Author:       Xin-Xin MA, Hao-Kai SUN
 * Created:      2019-10-21 Mon 19:17:20 CST
 * <<=====================================>>
 * Last Updated: 2020-01-06 Mon 19:42:12 CST
 *           By: Hao-Kai SUN
 *     Update #: 550
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
#include "BesStdSelector/BesStdSelector.h"
// #include "AutoTagAlg/selector/GoodChrTrk.hpp"
// #include "AutoTagAlg/selector/Photon.hpp"

#include "HadronInfo/EtaInfo.h"
#include "HadronInfo/KsInfo.h"
#include "HadronInfo/LamInfo.h"
// #include "HadronInfo/OmegaInfo.h"
#include "HadronInfo/ParticleInf.h"
#include "HadronInfo/Pi0Info.h"

#include "EvtRecEvent/EvtRecEvent.h"
#include "GammaConv/GammaConv.h"
#include "McTruth/McParticle.h"

#ifndef ENABLE_BACKWARDS_COMPATIBILITY
typedef HepGeom::Point3D<double> HepPoint3D;
#endif

using Event::McParticleCol;

void AutoTagAlg::FillInfo(const CDCandidate& tag)
{
    m_KFitChisq  = m_useKinematicFit ? this->KinematicFit(tag, m_fid) : 1E4;
    m_NGoodTrks  = this->GoodTracks()[0];
    m_NGoodShwrs = this->GoodShowers();

    m_totCharge    = tag.charge();
    m_totChargeMdc = this->GoodTracks()[1];
    m_totChargeKal = this->GoodTracks()[2];

    this->FillFinalPar(tag); // fill final particles' masses and P4s.
    this->FillPi0Info(tag);
    this->FillEtaInfo(tag);
    this->FillKsInfo(tag);
    this->FillLmdInfo(tag);
    this->FillGammaConvInfo(tag);
}

void AutoTagAlg::FillFinalPar(const CDCandidate& tag)
{
    // 111: pi0    211: pi+-    11: e-+    22: mu-+
    // 321: K+-    221: eta     2212: proton+-
    // 3122: Lambda    3334: Omega    310: K_S0
    for (unsigned i = 0; i < m_fid.size(); ++i) {
        if (abs(m_fid[i]) == 111 || abs(m_fid[i]) == 11 ||
            abs(m_fid[i]) == 211 || abs(m_fid[i]) == 22 ||
            abs(m_fid[i]) == 321 || abs(m_fid[i]) == 2212 ||
            abs(m_fid[i]) == 221 || abs(m_fid[i]) == 3122 ||
            abs(m_fid[i]) == 310 || abs(m_fid[i]) == 3334) {
            continue;
        }
        m_mass[i] = tag.decay().child(i).mass();
    }

    for (unsigned i = 0; i < m_fid.size(); ++i) {
        fillP4(m_p4[i], tag.decay().child(i).p4());

        if (m_InfoLvl >= 4)
            std::cout << "child\t" << i << "\t" << tag.decay().child(i).p4()
                      << std::endl;
    }
}

void AutoTagAlg::FillGammaConvInfo(const CDCandidate& signal)
{
    ParticleInf particleInfo;
    particleInfo.SetChildren(m_fid);
    vecint eleIds = particleInfo.GammaCov();
    if (eleIds.empty()) return;

    // IVertexDbSvc* vtxsvc;
    // Gaudi::svcLocator()->service("VertexDbSvc", vtxsvc);

    // if (vtxsvc->isVertexValid()) {
    //     // double* dbv = vtxsvc->PrimaryVertex();
    //     // double* vv  = vtxsvc->SigmaPrimaryVertex();
    // }

    // To get the position where gamma conversion happens, one needs to
    // calculate it from detector's space origin (0, 0, 0), thus can determine
    // whether it locates in detector materials or the inner vacuum since gamma
    // conversion only occurs in detector materials i.e. gamma interacts with
    // those materials to generate electrons and positrons.
    HepPoint3D IP(0, 0, 0);
    // HepPoint3D IP(dbv[0], dbv[1], dbv[2]);

    for (unsigned i = 0; i < eleIds.size() / 2; ++i) {
        const EvtRecTrack* trk1 =
            signal.decay().child(eleIds[i]).finalChildren().first[0];
        const EvtRecTrack* trk2 =
            signal.decay().child(eleIds[i + 1]).finalChildren().first[0];

        HepVector helix1 =
            const_cast<EvtRecTrack*>(trk1)->mdcKalTrack()->getZHelixE();
        HepVector helix2 =
            const_cast<EvtRecTrack*>(trk2)->mdcKalTrack()->getZHelixE();

        GammaConv gammaConv(helix1, helix2, IP);
        m_GammaConv_Rxy[i] = gammaConv.getRxy();
        m_GammaConv_Rx[i]  = gammaConv.getRx();
        m_GammaConv_Ry[i]  = gammaConv.getRy();
    }
}

void AutoTagAlg::FillPi0Info(const CDCandidate& tag)
{

    int index = 0;
    for (unsigned i = 0; i < m_fid.size(); ++i) {
        if (abs(m_fid[i]) != 111) continue;

        const CDCandidate& aPi0 = tag.decay().child(i);
        Pi0Info aPi0Info(aPi0);

        m_isGoodPi0Col[index]    = aPi0Info.isGoodPi0();
        m_mPi0Col[index]         = aPi0Info.m();
        m_m1CPi0Col[index]       = aPi0Info.m1c();
        m_heliAnglePi0Col[index] = aPi0Info.angle();
        m_openAnglePi0Col[index] = aPi0Info.openAngle();
        m_helicityPi0Col[index]  = aPi0Info.helicity();
        m_chisq1CPi0Col[index]   = aPi0Info.chisq();

        fillP4(m_p41CPi0Col[index], aPi0Info.p41c());
        // m_p41CPi0Col[index]    = aPi0Info.p41c();
        ++index;
    }
}

void AutoTagAlg::FillEtaInfo(const CDCandidate& tag)
{
    for (unsigned i = 0; i < m_fid.size(); ++i) {
        if (m_fid[i] == 221) {
            const CDCandidate& aEta = tag.decay().child(i);
            // second represents showers, i.e. gammas
            // eta -> gamma gamma
            EtaInfo etaInfo(aEta.finalChildren().second);
            m_chisqs[i] = etaInfo.chisq();
            m_mass[i]   = etaInfo.m();
        }
    }
}

void AutoTagAlg::FillKsInfo(const CDCandidate& tag)
{
    int index = 0;
    for (unsigned i = 0; i < m_fid.size(); ++i) {
        if (abs(m_fid[i]) != 310) continue;

        const CDCandidate& aKs = tag.decay().child(i);
        KsInfo aKsInfo(aKs);
        m_mKsCol[index] = aKsInfo.m();
        m_dKsCol[index] = aKsInfo.decayLength();
        m_RKsCol[index] = aKsInfo.decayLengthRatio();

        m_chisq1KsCol[index] = aKsInfo.vtxChi2();
        m_chisq2KsCol[index] = aKsInfo.chi2();

        fillP4(m_p4PipKsCol[index], aKsInfo.p4child(0));
        fillP4(m_p4PimKsCol[index], aKsInfo.p4child(1));
        index += 1;
    }
}

void AutoTagAlg::FillLmdInfo(const CDCandidate& tag)
{
    // index does not always equal i, since the if..continue statement
    int index = 0;
    for (unsigned i = 0; i < m_fid.size(); ++i) {
        if (abs(m_fid[i]) != 3122 && abs(m_fid[i]) != 3334) continue;

        const CDCandidate& aLmd = tag.decay().child(i);
        LamInfo aLmdInfo(aLmd);
        m_mLmdCol[index] = aLmdInfo.m();
        m_dLmdCol[index] = aLmdInfo.decayLength();
        m_RLmdCol[index] = aLmdInfo.decayLengthRatio();

        m_chisq1LmdCol[index] = aLmdInfo.vtxChi2();
        m_chisq2LmdCol[index] = aLmdInfo.chisq();

        fillP4(m_p4PrLmdCol[index], aLmdInfo.p4child(0));
        fillP4(m_p4PiLmdCol[index], aLmdInfo.p4child(1));
        index += 1;
    }
}

void AutoTagAlg::fillP4(NTAdbl m_p4, const HLV& p4)
{
    for (unsigned i = 0; i < 4; ++i)
        m_p4[i] = p4[i];
}

void AutoTagAlg::fillMatrixP4(NTMdbl m_Mp4, const vecHLV& Mp4)
{
    for (unsigned i = 0; i < Mp4.size(); ++i) {
        for (unsigned j = 0; j < 4; ++j) {
            m_Mp4[i][j] = Mp4[i][j];
        }
    }
}

void AutoTagAlg::fillDecayChain()
{
    fillDecayChain(tag_indexmc, tag_pdgid, tag_motheridx, m_Mother);
}

void AutoTagAlg::fillDecayChain(NTIint& nt_idxmc, NTAint& nt_pdgid,
                                 NTAint& nt_motheridx, int thePdgId)
{
    SmartDataPtr<Event::McParticleCol> mcParticleCol(eventSvc(),
                                                     "/Event/MC/McParticleCol");
    int m_numParticle = 0;
    if (!mcParticleCol) {
        std::cout << "Could not retrieve McParticelCol" << std::endl;
        return;
    } else {
        vecint pdgid;
        vecint motherindex;
        pdgid.clear();
        motherindex.clear();

        Event::McParticleCol::iterator iter_mc = mcParticleCol->begin();
        for (; iter_mc != mcParticleCol->end(); ++iter_mc) {
            if ((*iter_mc)->particleProperty() == thePdgId) {
                m_svc->extract(*iter_mc, pdgid, motherindex);
                m_numParticle = pdgid.size();
                for (unsigned i = 0; i != pdgid.size(); i++) {
                    nt_pdgid[i]     = pdgid[i];
                    nt_motheridx[i] = motherindex[i];
                }
                break;
            }
        }
        nt_idxmc        = m_numParticle;
        nt_motheridx[0] = -1;

        return;
    }
}

void AutoTagAlg::getDecayChain(vecint& n_pdgid, vecint& n_motheridx,
                                int thePdgId)
{
    SmartDataPtr<McParticleCol> mcParticleCol(eventSvc(),
                                              "/Event/MC/McParticleCol");
    if (!mcParticleCol) {
        std::cout << "Could not retrieve McParticelCol" << std::endl;
        return;
    }
    n_pdgid.erase(n_pdgid.begin(), n_pdgid.end());
    n_motheridx.erase(n_motheridx.begin(), n_motheridx.end());

    for (McParticleCol::iterator itr = mcParticleCol->begin();
         itr != mcParticleCol->end(); itr++) {
        if ((*itr)->particleProperty() == thePdgId) {
            m_svc->extract(*itr, n_pdgid, n_motheridx);
            break;
        }
    }
    if (n_motheridx.empty()) return;
    n_motheridx[0] = -1;
}

int AutoTagAlg::match(const CDCandidate& aTag)
{
    vecint pdg, dechain;
    getDecayChain(pdg, dechain, m_Mother);
    vecint TrkIds;
    vecint ShowerIds;

    for (unsigned i = 0; i < aTag.finalChildren().second.size(); i++) {
        ShowerIds.push_back(22);
    }

    std::vector<const EvtRecTrack*> tracks;
    ParticleInf pInfo;
    for (unsigned i = 0; i < m_fid.size(); i++) {
        vecint pids = pInfo.ChargedChildren(m_fid[i]);
        std::vector<const EvtRecTrack*> chrTrks =
            aTag.decay().child(i).finalChildren().first;
        for (unsigned j = 0; j < pids.size(); j++) {
            tracks.push_back(chrTrks[j]);
            TrkIds.push_back(pids[j]);
        }
    }

    if (matchSvc->match(tracks, TrkIds, pdg, dechain)) {
        if (matchSvc->match(aTag.finalChildren().second, ShowerIds, pdg,
                            dechain)) {
            return 2;
        }
        return 1;
    }
    return 0;
}

void AutoTagAlg::getMothers(const int& MotherID,
                             SmartRefVector<McParticle>& mothers)
{
    mothers.clear();
    SmartDataPtr<McParticleCol> mcParticleCol(eventSvc(),
                                              "/Event/MC/McParticleCol");
    if (!mcParticleCol) {
        std::cout << "Could not retrieve McParticelCol" << std::endl;
        return;
    }

    if (m_fillMCParA) {
        unsigned i = -1;
        int aPID;
        m_allParInMCN = mcParticleCol->size();
        for (McParticleCol::iterator itr = mcParticleCol->begin();
             itr != mcParticleCol->end(); ++itr) {
            aPID = (*itr)->particleProperty();
            if (aPID == MotherID) mothers.push_back(*itr);
            m_allParInMC[++i] = aPID;
        }
    } else {
        for (McParticleCol::iterator itr = mcParticleCol->begin();
             itr != mcParticleCol->end(); ++itr) {
            if ((*itr)->particleProperty() == MotherID) mothers.push_back(*itr);
        }
    }

    // check if all required Mother particle are found in MC Truth.
    if (m_MothersN == mothers.size()) {
        m_mothersN = m_MothersN; // recorded in root tuple.
    } else {
        std::cout << "*** ERROR: The Number of Mothers NOT Match! ***"
                  << std::endl;
        std::cout << "***        User initialize with: " << m_MothersN
                  << std::endl;
        std::cout << "***        Found in MC Truth is: " << mothers.size()
                  << std::endl;
        m_mothersN = 0;
        return;
    }
}

static void emptyTensor(mtxbool& mtx, const unsigned& dim)
{
    vecbool tmp;
    tmp.reserve(4);
    for (unsigned i = 0; i < 4; ++i) {
        tmp.push_back(false);
    }

    for (unsigned i = 0; i < dim; ++i) {
        mtx.push_back(tmp);
    }
}

static void emptyTensor(vecint& mtx, const unsigned& dim)
{
    for (unsigned i = 0; i < dim; ++i) {
        mtx.push_back(-9123579);
    }
}

static void emptyTensor(vecHLV& mtx, const unsigned& dim)
{
    HLV lzero(0., 0., 0., 0.);
    for (unsigned i = 0; i < dim; ++i) {
        mtx.push_back(lzero);
    }
}

static void emptyTensor(SmartRefVector<McParticle>& mtx, const unsigned& dim)
{
    McParticle* tmp;
    for (unsigned i = 0; i < dim; ++i) {
        mtx.push_back(tmp);
    }
}

bool AutoTagAlg::FillTruthInfo(const vecint& mm_fid)
{
    // declare temporary variables
    if (m_InfoLvl > 5) {
        std::cout << "mm_fid: ";
        for (unsigned i = 0; i < mm_fid.size(); ++i) {
            std::cout << mm_fid[i] << " ";
        }
        std::cout << std::endl;
    }

    const unsigned fiddim = mm_fid.size();
    SmartRefVector<McParticle> mothers, tmp;

    vecint tmpvi;
    emptyTensor(tmpvi, fiddim);
    mtxint sigID;

    vecHLV tmpvh;
    emptyTensor(tmpvh, fiddim);
    mtxHLV sigP4I, sigX4I, sigX4F;

    mtxbool tmpvb;
    emptyTensor(tmpvb, fiddim);
    std::vector<mtxbool> sigTag;

    // get all Mother particles, usually there is only one.
    getMothers(m_Mother, mothers);
    if (mothers.empty()) return false;

    // for each target Mother, like Psi(2S) 100443
    // find m_fid in their daughters of all generations.
    // // initialize sigID, sigP4, sigX4.
    for (unsigned i = 0; i < mothers.size(); ++i) {
        sigID.push_back(tmpvi);
        sigP4I.push_back(tmpvh);
        sigX4I.push_back(tmpvh);
        sigX4F.push_back(tmpvh);
        sigTag.push_back(tmpvb);
    }

    vecint mm_fidcc;
    for (unsigned i = 0; i < mothers.size(); ++i) {
        tmp.clear();
        tmp = mothers[i]->daughterList();

        // recursion to collect all the MC-Truth info for particles in
        // 'm_fid'
        SmartRefVector<McParticle> tmpcc;
        emptyTensor(tmpcc, fiddim);
        collectDaughters(tmp, mm_fid, sigID[i], tmpcc);

        if (sigID[i] != mm_fid && m_inclCC) {
            sigID[i] = tmpvi;
            emptyTensor(tmpcc, fiddim);
            mm_fidcc = getChannelCC(mm_fid);
            collectDaughters(tmp, mm_fidcc, sigID[i], tmpcc);
        } else {
            mm_fidcc = mm_fid;
        }

        if (sigID[i] != mm_fidcc) {
            // searched all the particles in this MC simulation
            // and NOT found all particles in 'm_fid'
            MsgStream log(msgSvc(), name());
            log << "*** ERROR: Cannot Find all FID in MC Truth! ***" << endmsg;
            std::cout << "*** ERROR: Cannot Find all FID in MC Truth! ***"
                      << std::endl;
            if (m_InfoLvl > 5) {
                std::cout << "*** MC Truth: ";
                for (unsigned j = 0; j < sigID[i].size(); ++j) {
                    std::cout << sigID[i][j] << " ";
                }
                std::cout << " ***" << std::endl;

                std::cout << "***      FID: ";
                for (unsigned j = 0; j < mm_fidcc.size(); ++j) {
                    std::cout << mm_fidcc[j] << " ";
                }
                std::cout << " ***" << std::endl;
            }
            // return false;
            break;
        } else {
            if (m_InfoLvl > 5) std::cout << "Fill Tag: START" << std::endl;
            for (unsigned j = 0; j < fiddim; ++j) {
                sigP4I[i][j]    = tmpcc[j]->initialFourMomentum();
                sigX4I[i][j]    = tmpcc[j]->initialPosition();
                sigX4F[i][j]    = tmpcc[j]->finalPosition();
                sigTag[i][j][0] = tmpcc[j]->decayInFlight();
                sigTag[i][j][1] = tmpcc[j]->decayFromGenerator();
                sigTag[i][j][2] = tmpcc[j]->leafParticle();
                sigTag[i][j][3] = tmpcc[j]->primaryParticle();
            }
            if (m_InfoLvl > 5) std::cout << "Fill Tag: DONE" << std::endl;
        }
    }

    if (m_InfoLvl > 5) std::cout << "Fill ROOT: START" << std::endl;
    // fill into root tuple.
    for (unsigned i = 0; i < fiddim; ++i) {
        for (unsigned j = 0; j < m_MothersN; ++j) {
            for (unsigned k = 0; k < 4; ++k) {
                if (m_InfoLvl > 5) {
                    std::cout << "(" << i << ", " << j << "," << k << ","
                              << " )" << std::endl;
                    std::cout << sigP4I[j][i][k] << "; ";
                    std::cout << sigX4I[j][i][k] << "; ";
                    std::cout << sigX4F[j][i][k] << "; ";
                    std::cout << sigTag[j][i][k] << std::endl;
                }
                m_truthP4I[i][j][k] = sigP4I[j][i][k];
                m_truthX4I[i][j][k] = sigX4I[j][i][k];
                m_truthX4F[i][j][k] = sigX4F[j][i][k];
                m_truthTag[i][j][k] = sigTag[j][i][k];
            }
        }
    }
    if (m_InfoLvl > 5) std::cout << "Fill ROOT: DONE" << std::endl;

    return true;
}

void AutoTagAlg::collectDaughters(SmartRefVector<McParticle>& tmp,
                                   const vecint& _fid, vecint& found_fid,
                                   SmartRefVector<McParticle>& cc)
{
    int aPID;
    // 'collectd': used to collect all daughters.
    SmartRefVector<McParticle> collectd;
    collectd.clear();

    for (SmartRefVector<McParticle>::iterator itr = tmp.begin();
         itr != tmp.end(); ++itr) {
        // 'itr' is a pointer to a particle in 'tmp',
        // while 'aPID' is its particle ID, like pi+ is 211.
        aPID = (*itr)->particleProperty();
        if (m_InfoLvl > 5) std::cout << aPID << " ";

        // now, iterate all IDs in 'm_fid';
        unsigned idx = 0;
        for (; idx < _fid.size(); ++idx) {
            // it's possible that there are multi same ID in
            // 'm_fid', like {211, -211, 211, -211} i.e. pi+ pi- pi+
            // pi-
            if (aPID == _fid[idx] && aPID != found_fid[idx]) {
                // record which is newly found this time
                // and will be recorded in root tuple.
                found_fid[idx] = aPID;
                // record MC Particle in order, for latter info extraction.
                cc[idx] = *itr;
                break;
            }
        }

        if (idx == _fid.size()) {
            // meaning this particle in 'tmp' not found in 'm_fid'
            // also, we should collect its daughters!
            SmartRefVector<McParticle> tmpd = (*itr)->daughterList();
            for (SmartRefVector<McParticle>::iterator itr2 = tmpd.begin();
                 itr2 != tmpd.end(); ++itr2) {
                collectd.push_back(*itr2);
            }
        }
    }
    if (m_InfoLvl > 5) std::cout << std::endl;

    if (found_fid != _fid && !collectd.empty()) {
        collectDaughters(collectd, _fid, found_fid, cc);
    }
}

// use instance omegaXiKSelectorGoodChrTrk to sieve good tracks.
vecint AutoTagAlg::GoodTracks()
{
    SmartDataPtr<EvtRecTrackCol> evtRecTrackCol(eventSvc(),
                                                "/Event/EvtRec/EvtRecTrackCol");
    SmartDataPtr<EvtRecEvent> evtRecEvent(eventSvc(),
                                          "/Event/EvtRec/EvtRecEvent");

    EvtRecTrackIterator track_begin = evtRecTrackCol->begin();
    EvtRecTrackIterator track_end =
        evtRecTrackCol->begin() + evtRecEvent->totalCharged();
    int _chrMdc = 0;
    int _chrKal = 0;
    for (EvtRecTrackIterator itr = track_begin, itre = track_end; itr != itre;
         ++itr) {
        _chrMdc += (*itr)->mdcTrack()->charge();
        _chrKal += (*itr)->mdcKalTrack()->charge();
    }
    CDElectronList goodTrkList;//(track_begin, track_end,
                              // omegaXiKSelectorGoodChrTrk);

    vecint tmp;
    tmp.push_back(goodTrkList.size());
    tmp.push_back(_chrMdc);
    tmp.push_back(_chrKal);
    return tmp;
}

// use instance omegaXiKSelectorPhoton to sieve good showers.
int AutoTagAlg::GoodShowers()
{
    SmartDataPtr<EvtRecTrackCol> evtRecTrackCol(eventSvc(),
                                                "/Event/EvtRec/EvtRecTrackCol");
    SmartDataPtr<EvtRecEvent> evtRecEvent(eventSvc(),
                                          "/Event/EvtRec/EvtRecEvent");
    EvtRecTrackIterator neu_begin =
        evtRecTrackCol->begin() + evtRecEvent->totalCharged();
    EvtRecTrackIterator neu_end = evtRecTrackCol->end();
    CDPhotonList photonList;//(neu_begin, neu_end, omegaXiKSelectorPhoton);
    return photonList.size();
}
// template<typename T1, typename T2>
// void fillTensor(T1& lft, T2& rlt, const vecuint& maxdim, unsigned st = 0)
// {
//     for (unsigned i = 0; i < maxdim[st]; ++i) {
//         if (st + 1 < maxdim.size()) {
//             fillTensor<decltype(lft[i]), decltype(rlt[i])>(lft[i], rlt[i],
//                                                            maxdim, st + 1);
//         } else {
//             lft[i] = rlt[i];
//         }
//     }
// }
/* ===================================================================<<< */
/* ====================== FillInfo.cpp ends here ======================== */
