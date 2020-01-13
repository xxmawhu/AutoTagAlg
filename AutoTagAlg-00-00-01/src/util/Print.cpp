/* <===<===<===<===<===<===<===<===<===~===>===>===>===>===>===>===>===>===>
 * File Name:    Print.cpp
 * Author:       Xin-Xin MA, Hao-Kai SUN
 * Created:      2019-12-07 Sat 17:33:57 CST
 * <<=====================================>>
 * Last Updated: 2019-12-18 Wed 19:01:16 CST
 *           By: Hao-Kai SUN
 *     Update #: 12
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
#include "AutoTagAlg/AutoTagAlg.hpp"

// #include <iterator>

// using std::iterator;

// Template<typename T> void AutoTagAlg::Print(T ll) {
//   std::cout<<"========== this is all information about this decaylist
//   ============="<<std::endl; std::cout<<"######################  begind
//   ##########################"<<std::endl; typename T::iterator itr; for( itr
//   = ll.particle_begin() ; itr!=ll.particle_end() ; itr++){
//       const CDCandidate& aP = (*itr).particle() ;
//       std::cout<<"# "<<aP.mass()<<"\tP4:\t"<<aP.p4()<<std::endl;
//   }
//   std::cout<<"######################  E N D
//   ##########################"<<std::endl;
//
void AutoTagAlg::Print(CDDecayList ll)
{
    std::cout << "========== "
              << "All the information about this DecayList"
              << "==========" << std::endl;
    std::cout << "######################### BEGIN #############################"
              << std::endl;

    for (CDDecayList::iterator itr = ll.particle_begin();
         itr != ll.particle_end(); ++itr) {
        const CDCandidate& aP = (*itr).particle();
        std::cout << "# Mass: " << aP.mass() << ", P4: " << aP.p4()
                  << ", Chr: " << aP.charge() << std::endl;
    }

    std::cout << "######################### E N D #############################"
              << std::endl;
}
/* ===================================================================<<< */
/* ======================== Print.cpp ends here ========================= */
