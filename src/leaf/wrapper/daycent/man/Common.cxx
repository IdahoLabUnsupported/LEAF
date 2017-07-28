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
#include <leaf/wrapper/daycent/man/Common.h>

using namespace Poco::Data;
using namespace Poco::Data::Keywords;

namespace leaf
{
namespace wrapper
{
namespace daycent
{
namespace man
{
////////////////////////////////////////////////////////////////////////////////
Poco::Data::SessionPool* SessPool::instance(
    bool const& erase )
{
    static SessionPool* pool =
        new SessionPool( "SQLite", "db/crop100.db" );
    if( erase )
    {
        delete pool;
        pool = NULL;
    }

    return pool;
}
////////////////////////////////////////////////////////////////////////////////
std::vector< std::pair< std::string, std::string > > 
    GetLatLong( std::string const& abbr )
{
    std::vector< std::pair< std::string, std::string > > latLong;

    Statement stmt = ( SessPool::instance()->get()
        << "select\n"
        << "    t1.lat, \n"
        << "    t1.long \n"
        << "from spinup as t1 \n"
        << "where t1.abbr = :abbr",
        into( latLong ),
        useRef( abbr ) );

    stmt.execute();

    return latLong;
}
////////////////////////////////////////////////////////////////////////////////
std::map< std::string, std::vector< std::string > > GetSpinups( std::string const& abbr )
{
    std::map< std::string, std::vector< std::string > > files;
    std::vector< std::string > cmz;
    std::vector< std::string > file;
    std::vector< std::string > veg;

    Statement stmt = ( SessPool::instance()->get()
        << "select\n"
        << "    t1.cmz, \n"
        << "    t1.file, \n"
        << "    t1.vegtype \n"
        << "from spinup as t1 \n"
        << "where t1.abbr = :abbr",
        into( cmz ),
        into( file ),
        into( veg ),
        useRef( abbr ) );

    stmt.execute();

    files.insert( std::make_pair( "CMZ", cmz ) );
    files.insert( std::make_pair( "FILE", file ) );
    files.insert( std::make_pair( "VEG", veg ) );

    return files;
}
////////////////////////////////////////////////////////////////////////////////
std::vector< std::string > GetCropParamNames()
{
    std::vector< std::string > params;
    std::vector< std::string > colNames;

    std::string table( "table" );
    std::string db( "crop100" );

    //std::string crop( "C6" );
    //Statement stmt = 
    RecordSet rs ( ( SessPool::instance()->get()
            << "select * \n"
            //<< "t1.DESCRIPTION\n"
            << "from crop100\n",
            //<< "where t1.CROP = :crop",
            into( params ), now ) );

    ( SessPool::instance()->get()
        << "select name from sqlite_master where type=:table and name=:crop100",
        into( colNames ),
        useRef( table ), 
        useRef( db ), now );

    //bool more = rs.moveFirst();
    //while( more )
    std::cout << rs.columnCount() << " " << rs.rowCount() << std::endl;
    {
        for( size_t i=0; i<rs.columnCount(); ++i )
        {
            for( size_t j=0; j<rs.rowCount(); ++j )
            {
                //params.push_back( rs.row( j ).namesToString() );
                //std::cout << rs[i,j] << std::endl;
            }
        }
    }

    //stmt.execute();

    std::cout << params.at(0) << " " << colNames.at(0) << "  " << colNames.size() << std::endl;
    
    return params;
}
////////////////////////////////////////////////////////////////////////////////
std::vector< std::string > GetCropParams( std::vector< std::string > crops )
{
    std::vector< std::string > vals;

    for( size_t i=0; i<crops.size(); ++i )
    {
        std::string crop( crops.at(i) );
        ( SessPool::instance()->get()
            << "select * \n"
            << "from crop100 as t1\n"
            << "where t1.CROP = :crop",
            into( vals ),
            useRef( crop ), now );
    }

    return vals;
}
////////////////////////////////////////////////////////////////////////////////
} //end man
} //end daycent
} //end wrapper
} //end leaf
