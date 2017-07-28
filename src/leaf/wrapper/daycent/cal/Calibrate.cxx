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
#include <leaf/wrapper/daycent/cal/Calibrate.h>
#include <leaf/wrapper/daycent/cal/Common.h>

// --- POCO Includes --- //
#include <Poco/Data/SQLite/Connector.h>
#include <Poco/Data/Session.h>

#include <math.h>

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
Calibrate::Calibrate(void)
{
    assert( CreateCalibrationTable() );
}
////////////////////////////////////////////////////////////////////////////////
Calibrate::~Calibrate(void)
{
}
////////////////////////////////////////////////////////////////////////////////
void Calibrate::SetPRDX(
    std::string const& mukey,
    std::string crop, 
    double yld,
    double prdx )
{
    Poco::Data::SQLite::Connector::registerConnector();
    Poco::Data::Session *session = 
    new Poco::Data::Session( "SQLite", "db/calibration.db" );

    int yield = ceil(yld);
    Poco::Data::Statement stmt = ( *session
        << "INSERT INTO prdx(\n"
        << " id, mukey, crop, yld, prdx)\n"
        << " VALUES(NULL, :mukey, :crop, :yld, :prdx)\n",
        useRef( mukey ),
        useRef( crop ),
        useRef( yield ),
        useRef( prdx ),
        now );
}
////////////////////////////////////////////////////////////////////////////////
double Calibrate::GetPRDX(
    std::string const& mukey,
    std::string const& crop, 
    double yld)
{
    if( !PRDXExist( mukey, crop, yld ) )
    {
        return 0.0;
    }

    int yield = ceil(yld);

    Poco::Data::SQLite::Connector::registerConnector();
    Poco::Data::Session *session = 
        new Poco::Data::Session( "SQLite", "db/calibration.db" );

    Poco::Data::Statement stmt = ( *session
        << "select\n"
        << "  ifnull(prdx,-1)\n"
        << "from prdx\n"
        << "where\n"
        << "  prdx.yld = :yield and\n"
        << "  prdx.mukey = :mukey and\n"
        << "  prdx.crop = :crop\n",
        into( prdx ),
        useRef( yield ),
        useRef( mukey ),
        useRef( crop ),
        now);

    std::cout << prdx << std::endl;

    return prdx;
}
////////////////////////////////////////////////////////////////////////////////
bool Calibrate::PRDXExist(              
    std::string const& mukey,
    std::string const& crop, 
    double yld)
{
     int yield = ceil(yld);

    Poco::Data::SQLite::Connector::registerConnector();
    Poco::Data::Session *session = 
        new Poco::Data::Session( "SQLite", "db/calibration.db" );

    int ex;
    Poco::Data::Statement stmt = ( *session
        << "select exists\n"
        << "  (select prdx \n"
        << "   from prdx\n"
        << "   where\n"
        << "     prdx.yld = :yield and\n"
        << "     prdx.mukey = :mukey and\n"
        << "     prdx.crop = :crop\n"
        << "   limit 1)\n",
        into( ex ),
        useRef( yield ),
        useRef( mukey ),
        useRef( crop ),
        now);

    return ex;
}
////////////////////////////////////////////////////////////////////////////////

} //end cal
} //end daycent
} //end wrapper
} //end leaf