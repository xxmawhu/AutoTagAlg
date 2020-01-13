/* <===<===<===<===<===<===<===<===<===~===>===>===>===>===>===>===>===>===>
 * File Name:    Majorana.hpp
 * Author:       Hao-Kai SUN
 * Created:      2019-12-15 Sun 16:39:34 CST
 * <<=====================================>>
 * Last Updated: 2019-12-16 Mon 16:26:04 CST
 *           By: Hao-Kai SUN
 *     Update #: 31
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
#ifndef OMEGAXIK_MAJORANA_HPP
#define OMEGAXIK_MAJORANA_HPP

#include <map>
#include <string>

class Majorana
{
  public:
    Majorana() {}

    std::string operator[](const int& idx);
    static bool contained(const int& idx);

  private:
    static std::map<int, std::string> init_map();
    static std::map<int, std::string> m_majorana;
};

extern Majorana majorana;

#endif /* OMEGAXIK_MAJORANA_HPP */
/* ===================================================================<<< */
/* ====================== Majorana.hpp ends here ======================== */
