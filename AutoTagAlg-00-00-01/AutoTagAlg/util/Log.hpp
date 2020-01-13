/* <===<===<===<===<===<===<===<===<===~===>===>===>===>===>===>===>===>===>
 * File Name:    Log.hpp
 * Author:       Alberto Lepe, Xin-Xin MA, Hao-Kai SUN
 * Created:      2019-11-11 Mon 16:52:28 CST
 * <<=====================================>>
 * Last Updated: 2019-11-11 Mon 16:56:59 CST
 *           By: Hao-Kai SUN
 *     Update #: 7
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
#ifndef OMEGAXIK_UTIL_LOG_HPP
#define OMEGAXIK_UTIL_LOG_HPP

#include <iostream>

namespace Type {
enum typelog
{
    DEBUG = 2,
    INFO,
    WARNING,
    ERROR,
    FATAL
};
} // namespace Type

struct structlog
{
    bool headers;
    Type::typelog level;
};

extern structlog LOGCFG;

class LOG
{
  public:
    LOG();
    LOG(Type::typelog type, std::string func = "");
    ~LOG()
    {
        if (this->opened) {
            std::cout << std::endl;
        }
        this->opened = false;
    }

    void test();

    inline std::string getLabel(Type::typelog type)
    {
        std::string label;
        switch (type) {
            case Type::DEBUG:
                label = "DEBUG";
                break;
            case Type::INFO:
                label = "INFO ";
                break;
            case Type::WARNING:
                label = "WARN ";
                break;
            case Type::ERROR:
                label = "ERROR";
                break;
            case Type::FATAL:
                label = "FATAL";
                break;
            default:
                label = "ERROR";
                break;
        }
        return label;
    }

    template<typename T>
    LOG& operator<<(const T& msg)
    {
        if (this->msglevel >= LOGCFG.level) {
            std::cout << msg;
            this->opened = true;
        }
        return *this;
    }

  private:
    bool opened;
    Type::typelog msglevel;
};

#endif /* OMEGAXIK_UTIL_LOG_HPP */
/* ===================================================================<<< */
/* ========================= Log.hpp ends here ========================== */
