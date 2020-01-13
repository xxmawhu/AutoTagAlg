/* <===<===<===<===<===<===<===<===<===~===>===>===>===>===>===>===>===>===>
 * File Name:    ReadBeamInfoFromDB.cpp
 * Author:       Xin-Xin MA, Hao-Kai SUN
 * Created:      2019-11-11 Mon 16:44:59 CST
 * <<=====================================>>
 * Last Updated: 2019-12-07 Sat 17:39:23 CST
 *           By: Hao-Kai SUN
 *     Update #: 9
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
#include "GaudiKernel/Bootstrap.h"
#include "GaudiKernel/IJobOptionsSvc.h"
#include "GaudiKernel/ISvcLocator.h"
#include "GaudiKernel/PropertyMgr.h"

#include "AutoTagAlg/util/ReadBeamInfoFromDB.hpp"
#include <string.h>
#include <string>

ReadBeamInfoFromDB::ReadBeamInfoFromDB()
    : m_runID(-1)
    , m_isRunValid(false)
    , m_usecbE(false)
    , m_Ecm(0)
    , m_beta(0.011, 0, 0)
{}

MYSQL* ReadBeamInfoFromDB::OpenDb() const
{
    const char host[]     = "bes3db2.ihep.ac.cn";
    const char user[]     = "guest";
    const char passwd[]   = "guestpass";
    const char db[]       = "offlinedb"; // const char db[] = "run";
    unsigned int port_num = 3306;

    MYSQL* mysql = mysql_init(NULL);

    mysql = mysql_real_connect(mysql, host, user, passwd, db, port_num,
                               NULL, // socket
                               0);   // client_flag

    if (mysql == NULL) {
        fprintf(stderr, "can not open database: offlinedb\n");
    }

    return mysql;
}

void ReadBeamInfoFromDB::CloseDb(MYSQL* mysql) const
{
    mysql_close(mysql);
}

double ReadBeamInfoFromDB::ReadDb(int run)
{

    m_runID      = run;
    m_isRunValid = false;

    // read db directly
    // turned off now
    /*
    MYSQL* mysql=OpenDb();

    char stmt[1024];
    snprintf(stmt, 1024,
             "select BER_PRB, BPR_PRB "
            "from RunParams where run_number = %d", run);
    if (mysql_real_query(mysql, stmt, strlen(stmt))) {
      fprintf(stderr, "query error\n");
      CloseDb(mysql);
      return m_Ecm ;
    }


    MYSQL_RES* result_set = mysql_store_result(mysql);
    MYSQL_ROW row = mysql_fetch_row(result_set);
    if (!row) {
      fprintf(stderr, "cannot find data for RunNo %d\n", run);
      mysql_free_result(result_set);
      CloseDb(mysql);
      return m_Ecm;
    }

    double E_E=0, E_P=0;
    sscanf(row[0], "%lf", &E_E);
    sscanf(row[1], "%lf", &E_P);

    m_Ecm=(E_E+E_P)/2.0;

    // Free the memory to prevent memory leak!
    mysql_free_result(result_set);
    CloseDb(mysql);
    */

    // read db use service
    Gaudi::svcLocator()->service("DatabaseSvc", m_dbsvc, true);
    // calibrated beam Energy
    if (m_usecbE) {
        char stmt1[1024];
        snprintf(stmt1, 1024,
                 "select beam_energy, px, py, pz "
                 "from RunParams where run_number = %d",
                 run);
        DatabaseRecordVector res;
        int row_no = m_dbsvc->query("offlinedb", stmt1, res);
        if (row_no == 0) {
            return m_Ecm;
        }

        DatabaseRecord* records = res[0];

        double bE = records->GetDouble("beam_energy");
        m_Ecm     = bE;

        double px = records->GetDouble("px");
        double py = records->GetDouble("py");
        double pz = records->GetDouble("pz");

        m_beta.setX(px);
        m_beta.setY(py);
        m_beta.setZ(pz);

        // std::cout<<"beam e is:"<<bE<<",
        // px="<<px<<",py="<<py<<",pz="<<pz<<std::endl;
    }
    // use online beam Energy
    else {
        char stmt1[1024];
        snprintf(stmt1, 1024,
                 "select BER_PRB, BPR_PRB "
                 "from RunParams where run_number = %d",
                 run);
        DatabaseRecordVector res;
        int row_no = m_dbsvc->query("run", stmt1, res);
        if (row_no == 0) {
            return m_Ecm;
        }

        DatabaseRecord* records = res[0];
        double E_E = 0, E_P = 0;
        E_E   = records->GetDouble("BER_PRB");
        E_P   = records->GetDouble("BPR_PRB");
        m_Ecm = (E_E + E_P) / 2.0;
    }

    m_isRunValid = true;
    return m_Ecm;
}

bool ReadBeamInfoFromDB::isRunValid(int run)
{

    if (run == -1 || m_runID != run) {
        ReadDb(run);
        return m_isRunValid;
    }

    return false;
}

double ReadBeamInfoFromDB::getbeamE(int run, double defaultbeamE)
{
    int absrun = abs(run);

    if (!isRunValid(absrun)) {
        return defaultbeamE;
        fprintf(stderr, "ERROR in ReadBeamInfoFromDB: runNo is invalid!\n");
    }
    return m_Ecm;
}
/* ===================================================================<<< */
/* ================== ReadBeamInfoFromDB.cpp ends here ================== */
