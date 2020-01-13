/* <===<===<===<===<===<===<===<===<===~===>===>===>===>===>===>===>===>===>
 * File Name:    AutoTagAlg.hpp
 * Author:       Xin-Xin MA, Hao-Kai SUN
 * Created:      2019-09-25 Wed 19:32:12 CST
 * <<=====================================>>
 * Last Updated: 2020-01-06 Mon 19:41:23 CST
 *           By: Hao-Kai SUN
 *     Update #: 231
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
#ifndef AutoTagAlg_HPP
#define AutoTagAlg_HPP

#include "BesDChain/CDChargedKaonList.h"
#include "BesDChain/CDChargedPionList.h"
#include "BesDChain/CDDecayList.h"
#include "BesDChain/CDElectronList.h"
#include "BesDChain/CDEtaList.h"
#include "BesDChain/CDKsList.h"
#include "BesDChain/CDLambdaList.h"
#include "BesDChain/CDPhotonList.h"
#include "BesDChain/CDPi0List.h"
#include "BesDChain/CDProtonList.h"

#include "GaudiKernel/Algorithm.h"
#include "MCTruthMatchSvc/MCTruthMatchSvc.h"
#include "McDecayModeSvc/McDecayModeSvc.h"
#include "ParticleID/ParticleID.h"
#include "VertexFit/WTrackParameter.h"

// #include "AutoTagAlg/Namespace.hpp"
#include "AutoTagAlg/util/Custom.hpp"
#include "AutoTagAlg/util/ReadBeamInfoFromDB.hpp"

// already include in "GaudiKernel/Algorithm.h"
// #include <string>
// #include <vector>

class AutoTagAlg : public Algorithm {
   public:
    AutoTagAlg(const std::string &name, ISvcLocator *pSvcLocator);
    StatusCode initialize();
    StatusCode execute();
    StatusCode finalize();

   private:
    bool m_useMatch;
    int m_Mother;
    unsigned m_MothersN;
    double m_Ecm;
    bool m_readBeamE;
    bool m_fillMCInfo;
    bool m_fillMCParA;
    int m_minTrks, m_maxTrks, m_minShowers, m_maxShowers;
    bool m_useKinematicFit;
    bool m_debug;
    int m_InfoLvl;
    int m_tagCCID;
    bool m_inclCC;

    vecint m_fid;

    McDecayModeSvc *m_svc;
    MCTruthMatchSvc *matchSvc;

    void Print(CDDecayList);
    double KinematicFit(const CDCandidate &, vecint);
    // void AddWTrack(const CDCandidate& aChild, const int& pid,
    //                std::vector<WTrackParameter>& wtrks, vecint&
    //                child, std::vector<double>& resmass,
    //                std::vector<vecint >& cList);
    void AddWTrack(const CDCandidate &aChild, const int &pid,
                   std::vector<WTrackParameter> &wtrks, vecint &child);

    // fill final particles' masses and P4s.
    void FillFinalPar(const CDCandidate &);

    void FillPi0Info(const CDCandidate &);
    void FillEtaInfo(const CDCandidate &);
    void FillKsInfo(const CDCandidate &);
    void FillLmdInfo(const CDCandidate &);

    void FillGammaConvInfo(const CDCandidate &signal);

    void getMothers(const int &MotherID, SmartRefVector<McParticle> &mothers);
    void collectDaughters(SmartRefVector<McParticle> &tmp, const vecint &mm_fid,
                          vecint &found_fid, SmartRefVector<McParticle> &cc);
    vecint getChannelCC(const vecint &fid);
    bool FillTruthInfo(const vecint &fid);
    void FillInfo(const CDCandidate &tag);

    bool fromVee(const CDCandidate &sig, const int &pid);
    int GoodShowers();
    vecint GoodTracks();

    CDElectronList m_ElectronList;
    CDChargedPionList m_PionNoPID, m_PionPrimaryList, m_PionAllList;
    CDChargedKaonList m_KaonPrimaryList, m_KaonAllList;
    CDProtonList m_ProtonPrimaryList, m_ProtonAllList;

    CDLambdaList m_LambdaList;

    CDDecayList m_Pi0List;
    CDDecayList m_KsList;
    CDDecayList m_EtaList;
    CDDecayList m_PhotonList;

    CDDecayList m_OmegaList;

    vecstr _parname;

    CDDecayList DecayList(int);
    CDDecayList DecayList(vecint fid);

    int match(const CDCandidate &);
    bool GetParList();
    vecstr NamePar(vecint fid);
    double getChisq(const CDCandidate &sig, vecint fid);
    int getTagCharge(const CDCandidate &sig, vecint fid, const int &cctag);
    void SearchSignal();

    void fillP4(NTAdbl m_p4, const HLV &p4);
    void fillMatrixP4(NTMdbl m_Mp4, const vecHLV &Mp4);

    void fillDecayChain();
    void fillDecayChain(NTIint &, NTAint &, NTAint &, int);

    void getDecayChain(vecint &n_pdgid, vecint &n_motheridx, int thePdgId);

    // Information to store in ROOT file.
    NTuple::Tuple *tp, *mctp;

    NTIint m_runID, m_eventID, m_NGoodTrks, m_NGoodShwrs, m_tagCharge,
        m_tagLabel, m_totCharge, m_totChargeMdc, m_totChargeKal, m_match,
        tag_indexmc, m_indexmc;
    NTIint m_TotEvtN, m_TrkShwEvtN, m_SltEvtN;
    NTAint tag_pdgid, tag_motheridx, m_pdgid, m_motheridx;

    NTIdbl m_Ecmtp, m_KFitChisq, m_totChisq, m_mass[10], m_unconm[10],
        m_chisqs[10], m_GammaConv_Rxy[10], m_GammaConv_Rx[10],
        m_GammaConv_Ry[10];
    NTAdbl m_p4[20], m_fitP4[20];

    NTIuint m_mothersN;
    NTMdbl m_truthP4I[12], m_truthX4I[12], m_truthX4F[12];
    NTMint m_truthTag[12];

    NTIuint m_allParInMCN;
    NTAint m_allParInMC;

    // For Pi0 particle
    NTIuint m_isGoodPi0Col[16];
    NTIdbl m_mPi0Col[16], m_m1CPi0Col[16], m_heliAnglePi0Col[16],
        m_openAnglePi0Col[16], m_helicityPi0Col[16], m_chisq1CPi0Col[16];
    NTAdbl m_p41CPi0Col[16];

    // For K_S0 particle
    NTIdbl m_mKsCol[6], m_RKsCol[6], m_dKsCol[6], m_chisq1KsCol[6],
        m_chisq2KsCol[6];
    NTAdbl m_p4PipKsCol[6], m_p4PimKsCol[6];

    // For Lambda particle
    NTIdbl m_mLmdCol[4], m_RLmdCol[4], m_dLmdCol[4], m_chisq1LmdCol[4],
        m_chisq2LmdCol[4];
    NTAdbl m_p4PrLmdCol[4], m_p4PiLmdCol[4];

    // For Omega particle
    NTIdbl m_mOmegaCol[4], m_ROmegaCol[4], m_dOmegaCol[4], m_chisq1OmegaCol[4],
        m_chisq2OmegaCol[4];
    NTAdbl m_p4LmdOmegaCol[4], m_p4KaOmegaCol[4];
};

#endif /* PHYS_ANA_OMEGAXIKALG_HPP */
/* ===================================================================<<< */
/* ===================== AutoTagAlg.hpp ends here ====================== */
