/*************** <auto-copyright.rb BEGIN do not edit this line> **************
 *
 * Copyright 2012-2013 by Ames Laboratory
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License version 2.1 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.rb END do not edit this line> ***************/


// --- LEAF Includes --- //
#include <leaf/wrapper/daycent/cal/Common.h>

// --- POCO Includes --- //
#include <Poco/Data/SessionFactory.h>
#include <Poco/Data/Session.h>
#include <Poco/Data/SQLite/Connector.h>

using namespace Poco::Data;
using namespace Poco::Data::Keywords;

namespace leaf
{
namespace wrapper
{
namespace daycent
{
namespace cal
{

////////////////////////////////////////////////////////////////////////////////
Poco::Data::SessionPool* SessPool::instance(
    bool const& erase )
{
    static SessionPool* pool =
        new SessionPool( "SQLite", "db/calibration.db" );
    if( erase )
    {
        delete pool;
        pool = NULL;
    }

    return pool;
}
////////////////////////////////////////////////////////////////////////////////
Poco::Data::Statement CreateResultsTable(
    std::string const& tableName )
{
    Statement const createTable = ( SessPool::instance()->get()
        << "CREATE TABLE IF NOT EXISTS [" << tableName.c_str() << "](\n"
        << "  [id] INTEGER NOT NULL,\n"
        << "  [crop] VARCHAR( 50 ) NOT NULL,\n"
        << "  [mukey] INTEGER NOT NULL,\n"
        << "  [cokey] VARCHAR( 50 ) NOT NULL,\n"
        << "  [muacres] VARCHAR( 50 ) NOT NULL,\n"
        << "  [pctacres] VARCHAR( 50 ) NOT NULL,\n"
        << "  [yld] VARCHAR( 50 ) NOT NULL,\n"
        << "  [wtyld] VARCHAR( 50 ) NOT NULL,\n"
        << "  [tillreg] VARCHAR( 50 ) NOT NULL,\n"
        << "  [remreg] VARCHAR( 50 ) NOT NULL,\n"
        << "  [rotreg] VARCHAR( 50 ) NOT NULL,\n"
        << "  [yldreg] VARCHAR( 50 ) NOT NULL,\n"
        << "  [sci] VARCHAR( 50 ) NOT NULL,\n"
        << "  [sciOM] VARCHAR( 50 ) NOT NULL,\n"
        << "  [sciER] VARCHAR( 50 ) NOT NULL,\n"
        << "  [sciFO] VARCHAR( 50 ) NOT NULL,\n"
        << "  [windEros] VARCHAR( 50 ) NOT NULL,\n"
        << "  [waterEros] VARCHAR( 50 ) NOT NULL,\n"
        << "  [totBioRem] VARCHAR( 50 ) NOT NULL,\n"
        << "  PRIMARY KEY( [id] AUTOINCREMENT ),\n"
        << "  UNIQUE( [crop], [mukey], [cokey], [muacres], [pctacres],\n"
        << "          [yld], [wtyld], [tillreg], [remreg], [rotreg], [yldreg] )\n"
        << "  ON CONFLICT ABORT )", now );

    return createTable;
}
////////////////////////////////////////////////////////////////////////////////
bool CreateCalibrationTable()
{
    Poco::Data::SQLite::Connector::registerConnector();
    //m_session = new Poco::Data::Session("SQLite", "db/calibration" );

    std::ostringstream createCommand;
    createCommand << "CREATE TABLE IF NOT EXISTS [prdx](\n"
        << "  [id] INTEGER,\n"
        << "  [mukey] VARCHAR( 50 ) NOT NULL,\n"
        << "  [crop] VARCHAR( 50 ) NOT NULL,\n"
        << "  [yld] REAL NOT NULL,\n"
        << "  [prdx] REAL NOT NULL,\n"
        << "  PRIMARY KEY( [id] AUTOINCREMENT ),\n"
        << "  UNIQUE( [mukey], [crop], [yld] )\n"
        << "  ON CONFLICT IGNORE )";

    Poco::Data::Statement stmt = ( SessPool::instance()->get()
        << createCommand.str(), now );

    stmt.execute();

    return true;
}
////////////////////////////////////////////////////////////////////////////////

} //end cal
} //end daycent
} //end wrapper
} //end leaf
