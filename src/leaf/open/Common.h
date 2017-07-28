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


#pragma once

// --- LEAF Includes --- //
#include <leaf/LEAFConfig.h>

#include <leaf/util/DataHelper.h>

namespace leaf
{
namespace open
{

#ifndef SWIG
///
enum ConnectorEnum
{
    CONN_NONE = 0x0,
#ifdef POCO_MYSQL_API
    CONN_MYSQL = 0x1,
#endif //POCO_MYSQL_API
#ifdef POCO_ODBC_API
    CONN_ODBC = 0x2,
#endif //POCO_ODBC_API
    CONN_SQLITE = 0x4,
#if defined( POCO_MYSQL_API ) && defined( POCO_ODBC_API )
    CONN_ALL = CONN_MYSQL | CONN_ODBC | CONN_SQLITE,
#elif defined( POCO_MYSQL_API ) && !defined( POCO_ODBC_API )
    CONN_ALL = CONN_MYSQL | CONN_SQLITE,
#elif !defined( POCO_MYSQL_API ) && defined( POCO_ODBC_API )
    CONN_ALL = CONN_ODBC | CONN_SQLITE,
#else
    CONN_ALL = CONN_SQLITE
#endif //POCO_MYSQL_API && POCO_ODBC_API
};

///
enum DatabaseEnum
{
    DB_NONE = 0x0,
    DB_LEAF,
    DB_SSURGO,
    DB_CUSTOM
};

///
typedef boost::tuple<
    DatabaseEnum, ConnectorEnum, std::string > ConnectionTuple;
typedef std::vector< ConnectionTuple > ConnectionVector;
#endif //SWIG

///
LEAF_OPEN_EXPORTS
void RegisterConnectors();

#ifndef SWIG
///
LEAF_OPEN_EXPORTS
void RegisterConnectors(
    ConnectionVector const& connections );
#endif //SWIG

///
LEAF_OPEN_EXPORTS
void UnregisterConnectors();

#ifndef SWIG
///
LEAF_OPEN_EXPORTS
void Drivers(
    unsigned int const& connectors = CONN_ALL );

///
LEAF_OPEN_EXPORTS
void DataSources(
    unsigned int const& connectors = CONN_ALL );

///
LEAF_OPEN_EXPORTS
Poco::Data::Session GetSession(
    DatabaseEnum const& dbEnum,
    unsigned int const& maxRetryAttempts = 100,
    unsigned int const& retrySleep = 100 );

///
LEAF_OPEN_EXPORTS
ConnectorEnum const& GetConnector(
    DatabaseEnum const& dbEnum );
#endif //SWIG

} //end open
} //end leaf
