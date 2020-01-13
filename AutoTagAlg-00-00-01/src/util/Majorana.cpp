/* <===<===<===<===<===<===<===<===<===~===>===>===>===>===>===>===>===>===>
 * File Name:    Majorana.cpp
 * Author:       Hao-Kai SUN
 * Created:      2019-12-15 Sun 16:48:06 CST
 * <<=====================================>>
 * Last Updated: 2019-12-16 Mon 16:28:44 CST
 *           By: Hao-Kai SUN
 *     Update #: 35
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
#include "AutoTagAlg/util/Majorana.hpp"

#include <iostream>

std::map<int, std::string> Majorana::m_majorana = Majorana::init_map();

std::string Majorana::operator[](const int& idx)
{
    if (contained(idx)) {
        return m_majorana[idx];
    } else {
        std::cout << "*** ERROR: Invalid index! ***" << std::endl;
        return "ERROR";
    }
}

bool Majorana::contained(const int& idx)
{
    return m_majorana.find(idx) != m_majorana.end();
}

std::map<int, std::string> Majorana::init_map()
{
    std::map<int, std::string> _majorana;

    _majorana[21]       = "g";
    _majorana[10022]    = "vpho";
    _majorana[20022]    = "Cerenkov";
    _majorana[23]       = "Z0";
    _majorana[25]       = "Higgs0";
    _majorana[28]       = "reggeon";
    _majorana[29]       = "pomeron";
    _majorana[32]       = "Z'0";
    _majorana[33]       = "Z''0";
    _majorana[35]       = "Higgs'0";
    _majorana[36]       = "A0";
    _majorana[41]       = "Xu0";
    _majorana[81]       = "specflav";
    _majorana[83]       = "phasespa";
    _majorana[91]       = "cluster";
    _majorana[92]       = "string";
    _majorana[93]       = "indep";
    _majorana[94]       = "CMshower";
    _majorana[95]       = "SPHEaxis";
    _majorana[96]       = "THRUaxis";
    _majorana[97]       = "CLUSjet";
    _majorana[98]       = "CELLjet";
    _majorana[99]       = "table";
    _majorana[111]      = "pi0";
    _majorana[20111]    = "pi(2S)0";
    _majorana[221]      = "eta";
    _majorana[20221]    = "eta(2S)";
    _majorana[331]      = "eta'";
    _majorana[113]      = "rho0";
    _majorana[110]      = "rho_diff0";
    _majorana[30113]    = "rho(2S)0";
    _majorana[40113]    = "rho(3S)0";
    _majorana[-9040113] = "rho(2150)0";
    _majorana[223]      = "omega";
    _majorana[220]      = "omega_diff";
    _majorana[30223]    = "omega(2S)";
    _majorana[333]      = "phi";
    _majorana[330]      = "phi_diff";
    _majorana[10111]    = "a_00";
    _majorana[9000221]  = "f_0(600)";
    _majorana[9010221]  = "f_0";
    _majorana[10221]    = "f_0(1370)";
    _majorana[10113]    = "b_10";
    _majorana[10223]    = "h_1";
    _majorana[10333]    = "h'_1";
    _majorana[20113]    = "a_10";
    _majorana[20223]    = "f_1";
    _majorana[115]      = "a_20";
    _majorana[225]      = "f_2";
    _majorana[50221]    = "f_0(1500)";
    _majorana[335]      = "f'_2";
    _majorana[9020221]  = "eta(1405)";
    _majorana[100331]   = "eta(1475)";
    _majorana[10335]    = "eta2(1870)";
    _majorana[10331]    = "f_0(1710)";
    _majorana[229]      = "f_4(2050)";
    _majorana[20333]    = "f'_1";
    _majorana[8888888]  = "f_0(1790)";
    _majorana[9000223]  = "f_1(1510)";
    _majorana[9050225]  = "f_2(1950)";
    _majorana[9070225]  = "f_2(2150)";
    _majorana[9070221]  = "f_0(2200)";
    _majorana[9020225]  = "f_2(1640)";
    _majorana[9040225]  = "f_2(1910)";
    _majorana[9080221]  = "eta(2225)";
    _majorana[9040221]  = "eta(1760)";
    _majorana[9999999]  = "x(1835)";
    _majorana[310]      = "K_S0";
    _majorana[130]      = "K_L0";
    _majorana[9060221]  = "f_0(2100)";
    _majorana[150]      = "B0L";
    _majorana[510]      = "B0H";
    _majorana[350]      = "B_s0L";
    _majorana[530]      = "B_s0H";
    _majorana[441]      = "eta_c";
    _majorana[20441]    = "eta_c(2S)";
    _majorana[443]      = "J/psi";
    _majorana[440]      = "psi_diff";
    _majorana[100443]   = "psi(2S)";
    _majorana[30443]    = "psi(3770)";
    _majorana[9000443]  = "psi(4040)";
    _majorana[9010443]  = "psi(4160)";
    _majorana[9030443]  = "psi(4260)";
    _majorana[9040443]  = "psi(4360)";
    _majorana[9020443]  = "psi(4415)";
    _majorana[10443]    = "h_c";
    _majorana[10441]    = "chi_c0";
    _majorana[20443]    = "chi_c1";
    _majorana[445]      = "chi_c2";
    _majorana[551]      = "eta_b";
    _majorana[20551]    = "eta_b(2S)";
    _majorana[40551]    = "eta_b(3S)";
    _majorana[553]      = "Upsilon";
    _majorana[30553]    = "Upsilon(2S)";
    _majorana[60553]    = "Upsilon(3S)";
    _majorana[70553]    = "Upsilon(4S)";
    _majorana[80553]    = "Upsilon(5S)";
    _majorana[10553]    = "h_b";
    _majorana[40553]    = "h_b(2P)";
    _majorana[100553]   = "h_b(3P)";
    _majorana[10551]    = "chi_b0";
    _majorana[20553]    = "chi_b1";
    _majorana[555]      = "chi_b2";
    _majorana[30551]    = "chi_b0(2P)";
    _majorana[50553]    = "chi_b1(2P)";
    _majorana[10555]    = "chi_b2(2P)";
    _majorana[50551]    = "chi_b0(3P)";
    _majorana[110553]   = "chi_b1(3P)";
    _majorana[20555]    = "chi_b2(3P)";
    _majorana[40555]    = "eta_b2(1D)";
    _majorana[60555]    = "eta_b2(2D)";
    _majorana[120553]   = "Upsilon_1(1D)";
    _majorana[30555]    = "Upsilon_2(1D)";
    _majorana[557]      = "Upsilon_3(1D)";
    _majorana[130553]   = "Upsilon_1(2D)";
    _majorana[50555]    = "Upsilon_2(2D)";
    _majorana[10557]    = "Upsilon_3(2D)";
    _majorana[10222]    = "sigma_0";
    _majorana[100]      = "geantino";
    _majorana[101]      = "chargedgeantino";
    _majorana[61]       = "chi_c0p";
    _majorana[62]       = "chi_c1p";
    _majorana[63]       = "chi_c2p";
    _majorana[64]       = "X3940";
    _majorana[65]       = "Y3940";
    _majorana[66]       = "xvpho";
    _majorana[67]       = "eta_c2";

    return _majorana;
}

Majorana majorana;
/* ===================================================================<<< */
/* ======================= Majorana.cpp ends here ======================= */
