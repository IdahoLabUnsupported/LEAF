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
#include <leaf/util/DataHelper.h>

// --- POCO Includes --- //
using namespace Poco::Data;
using namespace Keywords;

////////////////////////////////////////////////////////////////////////////////
bool isnan(
    double v )
{
    return v != v;
}
////////////////////////////////////////////////////////////////////////////////

namespace leaf
{
namespace util
{

////////////////////////////////////////////////////////////////////////////////
StmtObj::StmtObj(
    Session const& session )
    :
    m_session( session ),
    m_statementImpl( m_session.createStatementImpl() ),
    m_statement( m_session )
{
    Statement stmt( m_statementImpl );
    m_statement.swap( stmt );
}
////////////////////////////////////////////////////////////////////////////////
StmtObj::~StmtObj()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
void ExecuteRetry(
    StmtObj& stmtObj,
    bool const& reset,
    unsigned int const& maxRetryAttempts,
    unsigned int const& retrySleep )
{
    //
    Statement& stmt = stmtObj.m_statement;
    if( stmt.isAsync() )
    {
        throw std::runtime_error(
            "leaf::util::ExecuteRetry does not support asynchronous queries" );
    }

    StatementImpl& impl = *( stmtObj.m_statementImpl );
    unsigned int cnt( 0 );
    while( ++cnt <= maxRetryAttempts )
    {
        try
        {
            if( cnt == 1 )
            {
                stmt.execute();
            }
            else
            {
                impl.reset();
                impl.execute( true );
            }
            return;
        }
        catch( Poco::Data::SQLite::DBLockedException const& ){;}
        catch( Poco::Data::SQLite::TableLockedException const& ){;}
        catch( Poco::Exception const& ex ){ ex.rethrow(); }
        Poco::Thread::sleep( retrySleep );
    }

    throw std::runtime_error( "leaf::util::ExecuteRetry failed" );
}
////////////////////////////////////////////////////////////////////////////////

} //end util
} //end leaf
