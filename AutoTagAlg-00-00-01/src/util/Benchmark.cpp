/* <===<===<===<===<===<===<===<===<===~===>===>===>===>===>===>===>===>===>
 * File Name:    Benchmark.cpp
 * Author:       Xin-Xin MA, Hao-Kai SUN
 * Created:      2019-11-11 Mon 17:12:17 CST
 * <<=====================================>>
 * Last Updated: 2019-12-08 Sun 20:19:36 CST
 *           By: Hao-Kai SUN
 *     Update #: 13
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
#include "AutoTagAlg/util/Benchmark.hpp"

// constructor with default value, term = "program"
Benchmark::Benchmark(const std::string& term)
    : m_term(term), m_total_time(0.0), m_calledTimes(0)
{}

void Benchmark::begin()
{
    // get the current time
    gettimeofday(&tvBegin, NULL);
    m_calledTimes += 1;
    // m_beg = time(0);
}

void Benchmark::end()
{
    gettimeofday(&tvEnd, NULL);
    m_total_time += (tvEnd.tv_sec - tvBegin.tv_sec) +
                    ((tvEnd.tv_usec - tvBegin.tv_usec) / 1000.0) / 1000.0;
    // m_beg = time(0);
    gettimeofday(&tvBegin, NULL);
}

void Benchmark::reset()
{
    m_total_time  = 0.0;
    m_calledTimes = 0;
}

void Benchmark::print()
{
    std::cout << "running " << m_term << " " << m_total_time << " s"
              << " called " << m_calledTimes << " times" << std::endl;
}

std::ostream& operator<<(std::ostream& out, const Benchmark& aBm)
{
    out << "running" << aBm.m_term << "\t" << aBm.m_total_time << " s"
        << " called " << aBm.m_calledTimes << " times";
    return out;
}
/* ===================================================================<<< */
/* ====================== Benchmark.cpp ends here ======================= */
