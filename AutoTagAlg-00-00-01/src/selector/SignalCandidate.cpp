/* ====================================================
#   Copyright (C)2020 All rights reserved.
#
#   Author        : Xin-Xin MA
#   Email         : xxmawhu@163.com
#   File Name     : SignalCandidate.cpp
#   Create Time   : 2020-01-12 22:41
#   Last Modified : 2020-01-12 22:41
#   Describe      :
#
# ====================================================*/
#include "GaudiKernel/Bootstrap.h"
#include "GaudiKernel/IJobOptionsSvc.h"
#include "GaudiKernel/ISvcLocator.h"
#include "GaudiKernel/PropertyMgr.h"

#include "AutoTagAlg/selector/SignalCandidate.h"

SignalCandidate::SignalCandidate(){
    IJobOptionsSvc* jobSvc;
    Gaudi::svcLocator()->service("JobOptionsSvc", jobSvc);

    PropertyMgr m_propMgr;

    m_propMgr.declareProperty("MinMass", m_minMass = -10);
    m_propMgr.declareProperty("MaxMass", m_maxMass = 10.0);

    m_propMgr.declareProperty("MinRecMass", m_minRecMass = -10.0);
    m_propMgr.declareProperty("MaxRecMass", m_maxRecMass = 10);

    jobSvc->setMyProperties("SignalCandidate", &m_propMgr);
}

bool SignalCandidate::operator()(CDDecay& aComb) {
    double mass = aComb.mass();
    if (mass < m_minMass || mass > m_maxMass) {
        return false;
    }
    double mrec = (m_p4Beam - aComb.p4()).m();
    if (mrec < m_minRecMass || mrec > m_maxRecMass) {
        return false;
    }
    return true;
}

SignalCandidate localSignalCandidate;
