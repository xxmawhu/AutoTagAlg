/* ====================================================
#   Copyright (C)2020 All rights reserved.
#
#   Author        : Xin-Xin MA
#   Email         : xxmawhu@163.com
#   File Name     : SignalCandidate.h
#   Create Time   : 2020-01-12 19:29
#   Last Modified : 2020-01-12 22:44
#   Describe      :
#
# ====================================================*/
#ifndef SignalCandidate__H
#define SignalCandidate__H
#include "BesDChain/CDDecay.h"
#include "DecayChain/Function/DCSelectionFunction.h"
#include <string>
class SignalCandidate : public DCSelectionFunction<CDDecay> {
   public:
    SignalCandidate();

    bool operator()(CDDecay& aComb);
    void SetEcm(double Ecm) {
        m_p4Beam = HepLorentzVector(0.011*Ecm, 0, 0, Ecm);
    }

   private:
    SignalCandidate(const SignalCandidate&);
    const SignalCandidate& operator=(const SignalCandidate&);
    HepLorentzVector m_p4Beam;

    double m_minMass;
    double m_maxMass;
    double m_chisq;

    double m_minRecMass;
    double m_maxRecMass;
};
extern SignalCandidate localSignalCandidate;

#endif  // _COMBINATEPARTICLE_H
