/* <===<===<===<===<===<===<===<===<===~===>===>===>===>===>===>===>===>===>
 * File Name:    Custom.hpp
 * Author:       Xin-Xin MA, Hao-Kai SUN
 * Created:      2019-09-06 Fri 19:39:21 CST
 * <<=====================================>>
 * Last Updated: 2019-12-15 Sun 16:14:34 CST
 *           By: Hao-Kai SUN
 *     Update #: 32
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
#ifndef OMEGAXIK_UTIL_CUSTOM_HPP
#define OMEGAXIK_UTIL_CUSTOM_HPP

#include "CLHEP/Vector/LorentzVector.h"
#include "CLHEP/Vector/ThreeVector.h"

#include "GaudiKernel/NTuple.h"
#include "TMath.h"

typedef NTuple::Item<bool> NTIbool;
typedef NTuple::Array<bool> NTAbool;
typedef NTuple::Matrix<bool> NTMbool;

typedef NTuple::Item<unsigned> NTIuint;
typedef NTuple::Array<unsigned> NTAuint;
typedef NTuple::Matrix<unsigned> NTMuint;

typedef NTuple::Item<int> NTIint;
typedef NTuple::Array<int> NTAint;
typedef NTuple::Matrix<int> NTMint;

typedef NTuple::Item<double> NTIdbl;
typedef NTuple::Array<double> NTAdbl;
typedef NTuple::Matrix<double> NTMdbl;

typedef std::vector<bool> vecbool;
typedef std::vector<vecbool> mtxbool;
typedef std::vector<unsigned> vecuint;
typedef std::vector<vecuint> mtxuint;
typedef std::vector<int> vecint;
typedef std::vector<vecint> mtxint;
typedef std::vector<double> vecdbl;
typedef std::vector<vecdbl> mtxdbl;

typedef CLHEP::HepLorentzVector HLV;
typedef std::vector<HLV> vecHLV;
typedef std::vector<vecHLV> mtxHLV;

typedef std::vector<std::string> vecstr;
typedef std::vector<vecstr> mtxstr;

#endif /* OMEGAXIK_UTIL_CUSTOM_HPP */
/* ===================================================================<<< */
/* ======================== Custom.hpp ends here ======================== */
