/* <===<===<===<===<===<===<===<===<===~===>===>===>===>===>===>===>===>===>
 * File Name:    Log.cpp
 * Author:       Xin-Xin MA, Hao-Kai SUN
 * Created:      2019-11-11 Mon 17:00:21 CST
 * <<=====================================>>
 * Last Updated: 2019-11-11 Mon 17:04:45 CST
 *           By: Hao-Kai SUN
 *     Update #: 1
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
#include "AutoTagAlg/util/Log.hpp"
#include <iostream>

structlog LOGCFG = {true, Type::ERROR};

LOG::LOG()
{
    opened   = false;
    msglevel = Type::ERROR;
}

LOG::LOG(Type::typelog type, std::string func)
{
    this->opened   = false;
    this->msglevel = type;
    if (LOGCFG.headers && this->msglevel >= LOGCFG.level) {
        operator<<("[" + func + "]" + " [" + getLabel(type) + "] ");
    }
}

void LOG::test()
{
    std::cout << "LOG level = " << this->msglevel << std::endl;
    std::cout << "LOGCFG.level = " << LOGCFG.level << std::endl;
    std::cout << "opened = " << this->opened << std::endl;
    std::cout << "getLabel: " << getLabel(this->msglevel) << std::endl;
}
/* ===================================================================<<< */
/* ========================= Log.cpp ends here ========================== */
