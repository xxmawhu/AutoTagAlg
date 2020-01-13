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
#include "AutoTagAlg/util/Majorana.hpp"

AutoTagAlg::AutoTagAlg(const std::string& name, ISvcLocator* pSvcLocator)
    : Algorithm(name, pSvcLocator)
    , m_useMatch(false)
    , m_Mother(100443)
    , m_MothersN(1)
    , m_Ecm(3.686)
    , m_readBeamE(false)
    , m_fillMCInfo(true)
    , m_fillMCParA(true)
    , m_minTrks(0)
    , m_maxTrks(100)
    , m_minShowers(0)
    , m_maxShowers(100)
    , m_useKinematicFit(false)
    , m_debug(true)
    , m_InfoLvl(0)
    , m_tagCCID(3334)
    , m_inclCC(true)
{
    declareProperty("UseMatch", m_useMatch);
    declareProperty("Mother", m_Mother);
    declareProperty("MothersNumber", m_MothersN);
    declareProperty("Ecm", m_Ecm);
    declareProperty("ReadBeamE", m_readBeamE);
    declareProperty("FillMCInfo", m_fillMCInfo);
    declareProperty("FillMCParAll", m_fillMCParA);
    declareProperty("MinChargedTracks", m_minTrks);
    declareProperty("MaxChargedTracks", m_maxTrks);
    declareProperty("MinShowers", m_minShowers);
    declareProperty("MaxShowers", m_maxShowers);
    declareProperty("UseKinematicFit", m_useKinematicFit);
    declareProperty("Debug", m_debug);
    declareProperty("InfoLevel", m_InfoLvl);
    declareProperty("TagCCID", m_tagCCID);
    declareProperty("IncludeCC", m_inclCC);

    m_fid.clear();
    declareProperty("FID", m_fid);
}

StatusCode AutoTagAlg::initialize()
{
    time_t now = time(0);
    char* dt   = ctime(&now);
    std::cout << "The date : " << dt << std::endl;

    MsgStream log(msgSvc(), name());
    log << MSG::INFO << "in initialize()" << endmsg;

    _parname = NamePar(m_fid);

    NTuplePtr tuple(ntupleSvc(), "FILE/sig");
    if (tuple) {
        tp = tuple;
    } else {
        tp = ntupleSvc()->book("FILE/sig", CLID_ColumnWiseTuple, "Psi2S");
        if (tp) {
            //--------tot-----------
            tp->addItem("runID", m_runID);
            tp->addItem("eventID", m_eventID);
            tp->addItem("goodTracksN", m_NGoodTrks);
            tp->addItem("goodShowersN", m_NGoodShwrs);
            tp->addItem("tagcharge", m_tagCharge);
            tp->addItem("totalcharge", m_totCharge);
            tp->addItem("totalchargeMdc", m_totChargeMdc);
            tp->addItem("totalchargeKal", m_totChargeKal);
            tp->addItem("Ecm", m_Ecmtp);

            ParticleInf pInfo;
            pInfo.SetChildren(m_fid);
            std::string tmpname = "";

            // || abs(m_fid[i]) == 310 || abs(m_fid[i]) == 3122 || abs(m_fid[i])
            // == 3334
            for (unsigned i = 0; i < m_fid.size(); ++i) {
                if (abs(m_fid[i]) == 111 || abs(m_fid[i]) == 11 ||
                    abs(m_fid[i]) == 211 || abs(m_fid[i]) == 22 ||
                    abs(m_fid[i]) == 321 || abs(m_fid[i]) == 2212 ||
                    abs(m_fid[i]) == 221 || abs(m_fid[i]) == 3122 ||
                    abs(m_fid[i]) == 310 || abs(m_fid[i]) == 3334)
                    continue;

                tmpname = "m" + _parname[i];
                std::cout << i << "\t" << tmpname << std::endl;
                tp->addItem(tmpname, m_mass[i]);
            }

            for (unsigned i = 0; i < m_fid.size(); ++i) {
                tmpname = "P4" + _parname[i];
                std::cout << i << "\t" << tmpname << std::endl;
                tp->addItem(tmpname, 4, m_p4[i]);
            }

            for (unsigned i = 0; i < m_fid.size(); i++) {
                // only pi0, eta decay to 2 gamma, has 1C cut, this chisq.
                if (m_fid[i] != 221) continue;
                tmpname = "chisq" + _parname[i];
                std::cout << i << "\t" << tmpname << std::endl;
                tp->addItem(tmpname, m_chisqs[i]);
            }

            // for (unsigned i = 0; i < m_fid.size(); i++) {
            //     if (!(m_fid[i] == 22)) continue;
            //     char name[40];
            //     sprintf(name, "showershape%s", this->_parname[i].c_str());
            //     // std::cout<<i<<"\t"<<name<<std::endl;
            //     // tp->addItem(name,      m_showershape[i]);
            // }

            // for (unsigned i = 0; i < m_fid.size(); i++) {
            //     if (!(m_fid[i] == 22)) continue;
            //     char name[40];
            //     sprintf(name, "showertime%s", this->_parname[i].c_str());
            //     // std::cout<<i<<"\t"<<name<<std::endl;
            //     // tp->addItem(name,      m_showertime[i]);
            // }

            vecint eleIds = pInfo.GammaCov();
            if (m_InfoLvl > 0) {
                std::cout << "#gamma conversion\t" << eleIds.size() / 2
                          << std::endl;
            }
            for (unsigned i = 0; i < eleIds.size(); i += 2) {
                char name1[100], name2[40], name3[40];
                sprintf(name1, "GammaConvRxy%d", i);
                sprintf(name2, "GammaConvRx%d", i);
                sprintf(name3, "GammaConvRy%d", i);
                tp->addItem(name1, m_GammaConv_Rxy[i / 2]);
                tp->addItem(name2, m_GammaConv_Rx[i / 2]);
                tp->addItem(name3, m_GammaConv_Ry[i / 2]);
                std::cout << "Gamma Gov\t" << name1 << std::endl;
            }
            tp->addItem("match", m_match);
            tp->addItem("chisqKFit", m_KFitChisq);
            tp->addItem("chisqTotal", m_totChisq);

            // information of Pi0
            int numPi0 = -1;
            for (unsigned i = 0; i < m_fid.size(); ++i) {
                if (abs(m_fid[i]) != 111) continue;
                ++numPi0;
                if (numPi0 > 16) {
                    std::cout << "Error: too much Pi0 -> " << numPi0
                              << std::endl;
                    return StatusCode::FAILURE;
                }

                tmpname = "IsGood" + _parname[i];
                tp->addItem(tmpname, m_isGoodPi0Col[numPi0]);
                std::cout << "addItem " << tmpname << std::endl;

                tmpname = "m" + _parname[i];
                tp->addItem(tmpname, m_mPi0Col[numPi0]);
                std::cout << "addItem " << tmpname << std::endl;

                tmpname = "m1C" + _parname[i];
                tp->addItem(tmpname, m_m1CPi0Col[numPi0]);
                std::cout << "addItem " << tmpname << std::endl;

                tmpname = "heliAngle" + _parname[i];
                tp->addItem(tmpname, m_heliAnglePi0Col[numPi0]);
                std::cout << "addItem " << tmpname << std::endl;

                tmpname = "openAngle" + _parname[i];
                tp->addItem(tmpname, m_openAnglePi0Col[numPi0]);
                std::cout << "addItem " << tmpname << std::endl;

                tmpname = "helicity" + _parname[i];
                tp->addItem(tmpname, m_helicityPi0Col[numPi0]);
                std::cout << "addItem " << tmpname << std::endl;

                tmpname = "Chisq1C" + _parname[i];
                tp->addItem(tmpname, m_chisq1CPi0Col[numPi0]);
                std::cout << "addItem " << tmpname << std::endl;

                tmpname = "P41C" + _parname[i];
                tp->addItem(tmpname, 4, m_p41CPi0Col[numPi0]);
                std::cout << "addArray " << tmpname << std::endl;
            }

            // information of K_S0
            int numKs = -1;
            for (unsigned i = 0; i < m_fid.size(); ++i) {
                if (abs(m_fid[i]) != 310) continue;
                numKs += 1;
                if (numKs > 8) {
                    std::cout << "Error: too much K_S0 -> " << numKs
                              << std::endl;
                    return StatusCode::FAILURE;
                }
                tmpname = "m" + _parname[i];
                tp->addItem(tmpname, m_mKsCol[numKs]);
                std::cout << "addItem " << tmpname << std::endl;

                tmpname = "R" + _parname[i];
                tp->addItem(tmpname, m_RKsCol[numKs]);
                std::cout << "addItem " << tmpname << std::endl;

                tmpname = "d" + _parname[i];
                tp->addItem(tmpname, m_dKsCol[numKs]);
                std::cout << "addItem " << tmpname << std::endl;

                tmpname = "chisq1" + _parname[i];
                tp->addItem(tmpname, m_chisq1KsCol[numKs]);
                std::cout << "addItem " << tmpname << std::endl;

                tmpname = "chisq2" + _parname[i];
                tp->addItem(tmpname, m_chisq2KsCol[numKs]);
                std::cout << "addItem " << tmpname << std::endl;

                tmpname = "p4Pip" + _parname[i];
                tp->addItem(tmpname, 4, m_p4PipKsCol[numKs]);
                std::cout << "addArray " << tmpname << std::endl;

                tmpname = "p4Pim" + _parname[i];
                tp->addItem(tmpname, 4, m_p4PimKsCol[numKs]);
                std::cout << "addArray " << tmpname << std::endl;
            }

            // information of Lambda
            int numLmd = -1;
            for (unsigned i = 0; i < m_fid.size(); ++i) {
                if (abs(m_fid[i]) != 3122 && abs(m_fid[i]) != 3334) continue;
                numLmd += 1;
                if (numLmd > 4) {
                    std::cout << "Error: too much Lambda -> " << numLmd
                              << std::endl;
                    return StatusCode::FAILURE;
                }
                // std::string lmdname =
                //     pInfo.Name(3122 / abs(m_fid[i]) * m_fid[i]);

                std::string lmdname = "Lambda";

                tmpname = "m" + lmdname; // _parname[i];
                tp->addItem(tmpname, m_mLmdCol[numLmd]);
                std::cout << "addItem " << tmpname << std::endl;

                tmpname = "R" + lmdname; // _parname[i];
                tp->addItem(tmpname, m_RLmdCol[numLmd]);
                std::cout << "addItem " << tmpname << std::endl;

                tmpname = "d" + lmdname; // _parname[i];
                tp->addItem(tmpname, m_dLmdCol[numLmd]);
                std::cout << "addItem " << tmpname << std::endl;

                tmpname = "chisq1" + lmdname; // _parname[i];
                tp->addItem(tmpname, m_chisq1LmdCol[numLmd]);
                std::cout << "addItem " << tmpname << std::endl;

                tmpname = "chisq2" + lmdname; // _parname[i];
                tp->addItem(tmpname, m_chisq2LmdCol[numLmd]);
                std::cout << "addItem " << tmpname << std::endl;

                tmpname = "p4Pr" + lmdname; // _parname[i];
                tp->addItem(tmpname, 4, m_p4PrLmdCol[numLmd]);
                std::cout << "addArray " << tmpname << std::endl;

                tmpname = "p4Pi" + lmdname; // _parname[i];
                tp->addItem(tmpname, 4, m_p4PiLmdCol[numLmd]);
                std::cout << "addArray " << tmpname << std::endl;
            }

            // information of Omega
            int numOmega = -1;
            for (unsigned i = 0; i < m_fid.size(); ++i) {
                if (abs(m_fid[i]) != 3334) continue;
                numOmega += 1;
                if (numOmega > 3) {
                    std::cout << "Error: too much Omega -> " << numOmega
                              << std::endl;
                    return StatusCode::FAILURE;
                }

                tmpname = "m" + _parname[i];
                tp->addItem(tmpname, m_mOmegaCol[numOmega]);
                std::cout << "addItem " << tmpname << std::endl;

                tmpname = "R" + _parname[i];
                tp->addItem(tmpname, m_ROmegaCol[numOmega]);
                std::cout << "addItem " << tmpname << std::endl;

                tmpname = "d" + _parname[i];
                tp->addItem(tmpname, m_dOmegaCol[numOmega]);
                std::cout << "addItem " << tmpname << std::endl;

                // First Vertex Fit
                tmpname = "chisq" + _parname[i];
                tp->addItem(tmpname, m_chisq1OmegaCol[numOmega]);
                std::cout << "addItem " << tmpname << std::endl;

                // Secondary Vertex Fit
                tmpname = "chisq2" + _parname[i];
                tp->addItem(tmpname, m_chisq2OmegaCol[numOmega]);
                std::cout << "addItem " << tmpname << std::endl;

                tmpname = "p4Lmd" + _parname[i];
                tp->addItem(tmpname, 4, m_p4LmdOmegaCol[numOmega]);
                std::cout << "addArray " << tmpname << std::endl;

                tmpname = "p4Ka" + _parname[i];
                tp->addItem(tmpname, 4, m_p4KaOmegaCol[numOmega]);
                std::cout << "addArray " << tmpname << std::endl;
            }

            // the kinematic result
            if (m_useKinematicFit) {
                for (unsigned i = 0; i < m_fid.size(); i++) {
                    tmpname = "p4Fit" + _parname[i];
                    std::cout << i << "\t" << tmpname << std::endl;
                    tp->addItem(tmpname, 4, m_fitP4[i]);
                }
            }

            tp->addItem("indexmc", tag_indexmc, 0, 100);
            tp->addIndexedItem("pdgid", tag_indexmc, tag_pdgid);
            tp->addIndexedItem("motheridx", tag_indexmc, tag_motheridx);

            tp->addItem("TagLabel", m_tagLabel);
            if (m_fillMCInfo) {
                tp->addItem("motherNumber", m_mothersN, 1, 6);
                for (unsigned i = 0; i < m_fid.size(); ++i) {
                    tmpname = "truthP4Init" + _parname[i];
                    std::cout << i << "\t" << tmpname << std::endl;
                    tp->addIndexedItem(tmpname, m_mothersN, 4, m_truthP4I[i]);

                    tmpname = "truthX4Init" + _parname[i];
                    std::cout << i << "\t" << tmpname << std::endl;
                    tp->addIndexedItem(tmpname, m_mothersN, 4, m_truthX4I[i]);

                    tmpname = "truthX4Final" + _parname[i];
                    std::cout << i << "\t" << tmpname << std::endl;
                    tp->addIndexedItem(tmpname, m_mothersN, 4, m_truthX4F[i]);

                    tmpname = "truthFGLP" + _parname[i];
                    std::cout << i << "\t" << tmpname << std::endl;
                    tp->addIndexedItem(tmpname, m_mothersN, 4, m_truthTag[i]);
                }
            }

            tp->addItem("TotExeEvtN", m_TotEvtN);
            tp->addItem("PassTrkShwCEvtN", m_TrkShwEvtN);
            tp->addItem("PassSelectionEvtN", m_SltEvtN);
        } else {
            log << MSG::ERROR << "    Cannot sig N-tuple:" << long(tp)
                << endmsg;
            return StatusCode::FAILURE;
        }
    }

    if (m_fillMCInfo) {
        NTuplePtr mctuple(ntupleSvc(), "FILE/mc");

        if (mctuple) {
            mctp = mctuple;
        } else {
            mctp = ntupleSvc()->book("FILE/mc", CLID_ColumnWiseTuple,
                                     "mc decay chain");
            if (mctp) {
                // ParticleInf pInfo;
                // pInfo.SetChildren(m_fid);

                if (m_fillMCParA) {
                    std::cout << "Add Item: "
                              << "allMCParNum" << std::endl;
                    mctp->addItem("allMCParNum", m_allParInMCN, 0, 100);
                    std::cout << "Add Array: "
                              << "allMCPar" << std::endl;
                    mctp->addIndexedItem("allMCPar", m_allParInMCN,
                                         m_allParInMC);
                }

                mctp->addItem("indexmc", m_indexmc, 0, 100);
                mctp->addIndexedItem("pdgid", m_indexmc, m_pdgid);
                mctp->addIndexedItem("motheridx", m_indexmc, m_motheridx);

            } else {
                log << MSG::ERROR << " cannot book mcds:" << long(mctp)
                    << endmsg;
                return StatusCode::FAILURE;
            }
        }

        IMcDecayModeSvc* i_McDecayModeSvc;
        StatusCode sc_McDecayModeSvc =
            service("McDecayModeSvc", i_McDecayModeSvc);

        if (sc_McDecayModeSvc.isFailure()) {
            log << MSG::FATAL << "could not load McDecayModeSvc" << endmsg;
            return sc_McDecayModeSvc;
        }
        m_svc = dynamic_cast<McDecayModeSvc*>(i_McDecayModeSvc);

        log << MSG::INFO << "successfully return from initialize()" << endmsg;
        // init match service
        IMCTruthMatchSvc* i_matchSvc;
        StatusCode sc_MC = service("MCTruthMatchSvc", i_matchSvc);
        if (sc_MC.isFailure()) {
            std::cout << "could not load MCTruthMatchSvc!" << std::endl;
            return 0;
        }
        matchSvc = dynamic_cast<MCTruthMatchSvc*>(i_matchSvc);
    }

    return StatusCode::SUCCESS;
}

vecstr AutoTagAlg::NamePar(vecint _fid)
{
    vecstr _name;
    vecint index;

    for (unsigned i = 0; i < m_fid.size(); ++i) {
        index.push_back(0);
        for (unsigned j = 0; j <= i; ++j) {
            if (_fid[i] == _fid[j]) ++index[i];
        }
    }

    ParticleInf pInfo;
    pInfo.SetChildren(_fid);
    std::string tmpname = "";
    std::string dd      = "";

    for (unsigned i = 0; i < _fid.size(); ++i) {
        tmpname = pInfo.Name(_fid[i]);
        if (index[i] > 1) {
            stringstream ss;
            ss << index[i];
            ss >> dd;
            tmpname += dd;
        }
        _name.push_back(tmpname);
    }

    return _name;
}

bool AutoTagAlg::fromVee(const CDCandidate& sig, const int& pid)
{
    CDDecayList veeList = DecayList(pid);

    CDDecayList::iterator begin = veeList.particle_begin();
    CDDecayList::iterator end   = veeList.particle_end();

    for (CDDecayList::iterator itr = begin; itr != end; ++itr) {
        const CDCandidate& aVee = (*itr).particle();
        if (aVee.overlap(sig)) {
            return true;
        }
    }

    return false;
}

vecint AutoTagAlg::getChannelCC(const vecint& fid)
{
    vecint fidcc;
    fidcc.clear();
    for (vecint::const_iterator itr = fid.begin(); itr != fid.end(); ++itr) {
        if (majorana.contained(*itr)) {
            fidcc.push_back(*itr);
        } else {
            fidcc.push_back((*itr) * (-1));
        }
    }
    return fidcc;
}

StatusCode AutoTagAlg::finalize()
{
    MsgStream logmess(msgSvc(), name());
    return StatusCode::SUCCESS;
}
/* ===================================================================<<< */
/* ========================= Init.cpp ends here ========================= */
