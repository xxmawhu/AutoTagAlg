/* <===<===<===<===<===<===<===<===<===~===>===>===>===>===>===>===>===>===>
 * File Name:    Benchmark.hpp
 * Author:       Xin-Xin MA, Hao-Kai SUN
 * Created:      2019-10-25 Fri 17:10:44 CST
 * <<=====================================>>
 * Last Updated: 2019-12-08 Sun 20:19:34 CST
 *           By: Hao-Kai SUN
 *     Update #: 14
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
#ifndef OMEGAXIK_UTIL_BENCHMARK_HPP
#define OMEGAXIK_UTIL_BENCHMARK_HPP

#include <iostream>
#include <ostream>
#include <string>

// #include <time.h>
// the accurate of ctime is not enough
#include <sys/time.h>

class Benchmark
{
    // usage: create an object Benchmark
    //       Benchmark benchmark("test tracking");
    // member method:
    //   begin() begin record the time
    //   end() finish this record
    //   print() print the time using
    //   eg. running test tracking m_total_time s
  public:
    Benchmark(const std::string& term = "program");

    void begin();
    void end();
    double elapsed() { return m_total_time; }
    void reset();
    void print();

    friend std::ostream& operator<<(std::ostream& out, const Benchmark& aBm);

  private:
    // time_t m_beg;
    struct timeval tvBegin, tvEnd;
    std::string m_term;
    double m_total_time;
    long int m_calledTimes;
};

#endif // OMEGAXIK_UTIL_BENCHMARK_HPP
/* ===================================================================<<< */
/* ====================== Benchmark.hpp ends here ======================= */
