/* ====================================================
#   Copyright (C)2020 All rights reserved.
#
#   Author        : Xin-Xin MA
#   Email         : xxmawhu@163.com
#   File Name     : FillInfo.cpp
#   Create Time   : 2020-02-08 17:29
#   Last Modified : 2020-02-08 17:29
#   Describe      :
#
# ====================================================*/
#include <iostream>
#include "EventModel/EventModel.h"
#include "EventModel/EventHeader.h"
#include "EventModel/Event.h"
#include "EventNavigator/EventNavigator.h"
#include "AutoTagAlg/AutoTagAlg.hpp"
using namespace std;
void AutoTagAlg::FillInfo(const CDCandidate& tag) {
    // fill beam status info
    m_tupleSvc[0] << *gBeamInfoSvc;
    gPhotonConverSvc->Feed(tag);
    m_tupleSvc[0] << *gPhotonConverSvc;
    // fill the kininatic infomation
    ///   cout << "gDecayChainSvc.Express" << endl;
    gDecayChainSvc.SetDecayTree(DecayTree(m_fid));
    gDecayChainSvc.Express(tag, m_tupleSvc[0]);
    // fill the decay chain information

    SmartDataPtr<Event::EventHeader> eventHeader(eventSvc(),
                                                 "/Event/EventHeader");
    int run = eventHeader->runNumber();
    int events = eventHeader->eventNumber();
    /// cout << "Info in AutoTagAlg::FillInfo: "
    ///     << "#run = " << run << ", #id = "
    ///      << events << endl;
    m_tupleSvc[0] << *gMCTruthInfoSvc;
}
