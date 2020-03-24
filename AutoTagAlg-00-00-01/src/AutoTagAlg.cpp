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

#include "AutoTagAlg/selector/SignalCandidate.h"

#ifndef ENABLE_BACKWARDS_COMPATIBILITY
typedef HepGeom::Point3D<double> HepPoint3D;
#endif

StatusCode AutoTagAlg::execute() {
    SmartDataPtr<EvtRecEvent> evtRecEvent(eventSvc(),
                                          "/Event/EvtRec/EvtRecEvent");
    int chrTrks = evtRecEvent->totalCharged();
    if (chrTrks < m_minTrks || chrTrks > m_maxTrks) {
        return StatusCode::SUCCESS;
    }

    int showers = evtRecEvent->totalNeutral();
    if (showers < m_minShowers || showers > m_maxShowers) {
        return StatusCode::SUCCESS;
    }
    if (m_fillMCInfo) {
        m_tupleSvc[1] << *gMCTruthInfoSvc;
        m_tupleSvc[1].Write();
    }
    SearchSignal();
    return StatusCode::SUCCESS;
}

/* ===================================================================<<< */
/* ===================== AutoTagAlg.cpp ends here ====================== */
