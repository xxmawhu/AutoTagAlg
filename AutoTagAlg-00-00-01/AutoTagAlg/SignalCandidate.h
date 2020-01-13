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

   private:
    SignalCandidate(const SignalCandidate&);
    const SignalCandidate& operator=(const SignalCandidate&);

    double m_minMass;
    double m_maxMass;
    double m_chisq;
};
extern SignalCandidate localSignalCandidate;

#endif  // _COMBINATEPARTICLE_H
