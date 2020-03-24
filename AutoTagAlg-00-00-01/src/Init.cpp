/* <===<===<===<===<===<===<===<===<===~===>===>===>===>===>===>===>===>===>
 * File Name:    Init.cpp
 * Author:       Xin-Xin MA, Hao-Kai SUN
 * Created:      2019-10-19 Sat 17:49:42 CST
 * <<=====================================>>
 * Last Updated: 2020-01-06 Mon 19:41:46 CST
 *           By: Hao-Kai SUN
 *     Update #: 258
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
#include <ctime>

#include "EvtRecEvent/EvtRecVeeVertex.h"

#include "HadronInfo/ParticleInf.h"

#include "AutoTagAlg/AutoTagAlg.hpp"
#include "TupleSvc/DecayChainSvc.h"
#include "AutoTagAlg/selector/SignalCandidate.h"

AutoTagAlg::AutoTagAlg(const std::string& name, ISvcLocator* pSvcLocator)
    : Algorithm(name, pSvcLocator) {
    declareProperty("UseMatch", m_useMatch = false);
    declareProperty("Mother", m_Mother = 443);
    declareProperty("MothersNumber", m_MothersN = 1);
    declareProperty("Ecm", m_Ecm = 3.097);
    declareProperty("ReadBeamE", m_readBeamE = false);
    declareProperty("FillMCInfo", m_fillMCInfo = false);
    declareProperty("FillMCParAll", m_fillMCParA = true);
    declareProperty("MinChargedTracks", m_minTrks = 0);
    declareProperty("MaxChargedTracks", m_maxTrks = 100);
    declareProperty("MinShowers", m_minShowers = 0);
    declareProperty("MaxShowers", m_maxShowers = 100);
    declareProperty("UseKinematicFit", m_useKinematicFit = true);
    declareProperty("Debug", m_debug = false);
    declareProperty("InfoLevel", m_InfoLvl = 0);
    declareProperty("TagCCID", m_tagCCID = 3334);
    declareProperty("IncludeCC", m_inclCC = 0);
    declareProperty("ApplyTags", m_tagmodes = 1);
    declareProperty("BenchmarkLevel", m_benchMarkLevel = 0);

    m_fid.clear();
    declareProperty("FID", m_fid);
    declareProperty("TruthChannel", m_truthfid);
    const int m_nTuple = m_tagmodes + 1;
    m_tupleSvc = new TupleSvc[m_nTuple];
    // int i;
    // for(i=0; i< m_tagmodes; ++i) {
    //     m_tupleSvc[i] = TupleSvc(10, 50, 4, 50);
    // }
    // // for decay chain Info
    // m_tupleSvc[m_tagmodes] = TupleSvc(10, 10, 8, 10);
}
AutoTagAlg::~AutoTagAlg() { delete[] m_tupleSvc; }

StatusCode AutoTagAlg::initialize() {
    MsgStream log(msgSvc(), name());
    log << MSG::INFO << "in initialize()" << endmsg;
    time_t now = time(0);
    char* dt = ctime(&now);
    std::cout << "The date : " << dt << std::endl;
    cout << DecayTree(m_fid) << endl;
    cout << DecayTree(m_truthfid) << endl;
    this->LoadSvc(log);

    // book tuple
    tp = this->BookTuple("sig");
    if (tp) {
        cout << "Book tuple 'sig' successful!" << endl;
    }
    m_tupleSvc[0].BindTuple(tp);
    m_tupleSvc[0].Register(*gBeamInfoSvc);
    
    gDecayChainSvc.SetDecayTree(DecayTree(m_fid));
    gDecayChainSvc.BulkRegister(m_tupleSvc[0]);

    m_tupleSvc[0].Register(*gPhotonConverSvc);
    gMCTruthInfoSvc->SetName("");
    m_tupleSvc[0].Register(*gMCTruthInfoSvc);

    if (m_fillMCInfo) {
        mctp = this->BookTuple("mc");
        if (mctp) {
            cout << "Book Tuple 'mc' successful!" << endl;
        }
        m_tupleSvc[1].BindTuple(mctp);
        m_tupleSvc[1].Register(*gMCTruthInfoSvc);
    }
    log << MSG::INFO << "successfully return from initialize()" << endmsg;
    return StatusCode::SUCCESS;
}

vector<int> AutoTagAlg::getChannelCC(const vector<int>& fid) {
    vector<int> fidcc;
    return fidcc;
}

StatusCode AutoTagAlg::finalize() {
    MsgStream logmess(msgSvc(), name());
    return StatusCode::SUCCESS;
}
NTuple::Tuple* AutoTagAlg::BookTuple(const string& name,
                                     const string& comment) {
    MsgStream log(msgSvc(), Algorithm::name());
    NTuplePtr tmptuple(ntupleSvc(), string("FILE/") + name);
    if (tmptuple) {
        return tmptuple;
    }

    return ntupleSvc()->book(string("FILE/") + name, CLID_ColumnWiseTuple,
                             comment);
}
StatusCode AutoTTTSAlg::LoadSvc(MsgStream& log) {
    // load MCTruthInfo service
    IMCTruthInfo* i_matruthInfo;
    StatusCode sc_MCInfo = service("MCTruthInfo", i_matruthInfo);
    if (sc_MCInfo.isFailure()) {
        std::cout << "could not load MCTruthInfo!" << std::endl;
        return 0;
    }
    gMCTruthInfoSvc = dynamic_cast<MCTruthInfo*>(i_matruthInfo);
    gMCTruthInfoSvc->SetDecayTree(DecayTree(m_tagFid));
    
    // load BeamInfoSvc
    IBeamInfoSvc* i_BeamInfoSvc;
    StatusCode sc_BeamInfoSvc = service("BeamInfoSvc", i_BeamInfoSvc);
    if (sc_BeamInfoSvc.isFailure()) {
        std::cout << "could not load BeamInfoSvc!" << std::endl;
        return 0;
    }
    gBeamInfoSvc = dynamic_cast<BeamInfoSvc*>(i_BeamInfoSvc);
    if (!m_readBeamE) {
        gBeamInfoSvc->SetEcm(m_Ecm);
    }
    // load PhotonConverSvc
    IPhotonConverSvc* i_PhotonConverSvc;
    StatusCode sc_PhotonConverSvc = service("PhotonConverSvc", i_PhotonConverSvc);
    if (sc_PhotonConverSvc.isFailure()) {
        std::cout << "could not load PhotonConverSvc!" << std::endl;
        return 0;
    }
    gPhotonConverSvc = dynamic_cast<PhotonConverSvc*>(i_PhotonConverSvc);
    gPhotonConverSvc->SetDecayTree(DecayTree(m_tagFid));

    // load MCTruthMatchSvc
    IMCTruthMatchSvc* i_matchSvc;
    StatusCode sc_MC = service("MCTruthMatchSvc", i_matchSvc);
    if (sc_MC.isFailure()) {
        std::cout << "could not load MCTruthMatchSvc!" << std::endl;
        return 0;
    }
    m_matchSvc = dynamic_cast<MCTruthMatchSvc*>(i_matchSvc);

    // load McDecayModeSvc
    IMcDecayModeSvc* i_McDecayModeSvc;
    StatusCode sc_McDecayModeSvc = service("McDecayModeSvc", i_McDecayModeSvc);
    if (sc_McDecayModeSvc.isFailure()) {
        log << MSG::FATAL << "could not load McDecayModeSvc" << endmsg;
        return sc_McDecayModeSvc;
    }
    m_McDecayModeSvc = dynamic_cast<McDecayModeSvc*>(i_McDecayModeSvc);
    
    IDCListSvc* i_DCListSvc;
    StatusCode sc_DCListSvc = service("DCListSvc", i_DCListSvc);
    if (sc_DCListSvc.isFailure()) {
        log << MSG::FATAL << "could not load DCListSvc" << endmsg;
        return sc_DCListSvc;
    }
    m_DCListSvc = dynamic_cast<DCListSvc*>(i_DCListSvc);
    return StatusCode::SUCCESS;
}
/* ===================================================================<<< */
/* ========================= Init.cpp ends here ========================= */
