/* <===<===<===<===<===<===<===<===<===~===>===>===>===>===>===>===>===>===>
 * File Name:    ReadBeamInfoFromDB.hpp
 * Author:       Xin-Xin MA, Hao-Kai SUN
 * Created:      2019-09-06 Sat 14:47:41 CST
 * <<=====================================>>
 * Last Updated: 2019-12-05 Thu 12:57:40 CST
 *           By: Hao-Kai SUN
 *     Update #: 10
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
#ifndef OMEGAXIK_UTIL_READBEAMINFOFROMDB_HPP
#define OMEGAXIK_UTIL_READBEAMINFOFROMDB_HPP

#include "CLHEP/Vector/ThreeVector.h"
#include "DatabaseSvc/IDatabaseSvc.h"

#include <mysql.h>

class ReadBeamInfoFromDB
{
  public:
    ReadBeamInfoFromDB();
    ~ReadBeamInfoFromDB() {}

    bool isRunValid(int run);
    double getbeamE(int run, double defaultbeamE);
    inline CLHEP::Hep3Vector getbeta() { return m_beta; }
    // inline bool setcalib(bool calib) { m_usecbE = calib; }
    inline void setcalib(bool calib) { m_usecbE = calib; }

  private:
    MYSQL* OpenDb() const;
    double ReadDb(int run);
    void CloseDb(MYSQL* mysql) const;

    IDatabaseSvc* m_dbsvc;
    int m_runID;
    bool m_isRunValid;
    bool m_usecbE;
    double m_Ecm;
    CLHEP::Hep3Vector m_beta;
};

#endif /* OMEGAXIK_UTIL_READBEAMINFOFROMDB_HPP */
/* ===================================================================<<< */
/* ================== ReadBeamInfoFromDB.hpp ends here ================== */
