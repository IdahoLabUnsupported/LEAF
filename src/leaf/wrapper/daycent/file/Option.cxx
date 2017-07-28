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
#include <leaf/wrapper/daycent/file/Option.h>

using namespace leaf::util;

// --- Poco Includes --- //
using namespace Poco::Dynamic;

// --- STL Includes --- //
#include <cassert>
#include <iostream>

namespace leaf
{
namespace wrapper
{
namespace daycent
{
namespace file
{

////////////////////////////////////////////////////////////////////////////////
Option::Option(
    int const& numparms,
    int const& type,
    int const& head,
    int const* const headlines,
    char const* const abbrev,
    char const* const descript,
    char value[][ MWORD ],
    char name[][ MWORD ],
    char heading[][ BWORD ] )
    :
    m_numparms( numparms ),
    m_type( type ),
    m_head( head ),
    m_abbrev( abbrev ),
    m_descript( descript )
{
    std::memcpy( &m_headlines, headlines, sizeof( m_headlines ) );
    std::memcpy( &m_value, value, sizeof( m_value ) );
    std::memcpy( &m_name, name, sizeof( m_name ) );
    std::memcpy( &m_heading, heading, sizeof( m_heading ) );

    for( unsigned int i = 0; i < numparms; ++i )
    {
        m_valPosMap.insert( std::make_pair( name[ i ], i ) );
    }
}
////////////////////////////////////////////////////////////////////////////////
Option::~Option()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
/*Option::Option(
    const Option& input )
{
    m_numparms = input.m_numparms;
    m_type = input.m_type;
    m_head = input.m_head;
    m_headlines = input.m_headlines;
    m_abbrev = input.m_abbrev;
    m_descript = input.m_descript;
    std::memcpy( &m_value, input.m_value, sizeof( m_value ) );
    std::memcpy( &m_name, input.m_name, sizeof( m_name ) );
    std::memcpy( &m_heading, input.m_heading, sizeof( m_heading ) );
    m_valPosMap = input.m_valPosMap;
}
////////////////////////////////////////////////////////////////////////////////
Option& Option::operator =(
    const Option& input )
{
    if( this != &input )
    {
        m_numparms = input.m_numparms;
        m_type = input.m_type;
        m_head = input.m_head;
        m_headlines = input.m_headlines;
        m_abbrev = input.m_abbrev;
        m_descript = input.m_descript;
        std::memcpy( &m_value, input.m_value, sizeof( m_value ) );
        std::memcpy( &m_name, input.m_name, sizeof( m_name ) );
        std::memcpy( &m_heading, input.m_heading, sizeof( m_heading ) );
        m_valPosMap = input.m_valPosMap;
    }
    return *this;
}*/
////////////////////////////////////////////////////////////////////////////////
bool Option::DeleteValue(
    std::string const& name ) const
{
    unsigned int pos;
    bool success = GetPos( pos, name );
    if( success )
    {
        m_name[ pos ][ 0 ] = 0;
        m_value[ pos ][ 0 ] = 0;
        m_valPosMap.erase( name );
    }
    return success;
}
////////////////////////////////////////////////////////////////////////////////
std::string const& Option::GetAbbrev() const
{
    return m_abbrev;
}
////////////////////////////////////////////////////////////////////////////////
std::string const& Option::GetDescript() const
{
    return m_descript;
}
////////////////////////////////////////////////////////////////////////////////
bool Option::GetPos(
    unsigned int& pos,
    std::string const& name ) const
{
    std::string tmp = "\'" + name + "\'";
    VAL_POS_MAP::const_iterator itr = m_valPosMap.find( tmp );
    bool success = ( itr != m_valPosMap.end() );
    if( success ) pos = itr->second;
    return success;
}
////////////////////////////////////////////////////////////////////////////////
bool Option::GetValue(
    std::string const& name,
    Var& value ) const
{
    unsigned int pos;
    bool success = GetPos( pos, name );
    if( success ) value = m_value[ pos ];
    return success;
}
////////////////////////////////////////////////////////////////////////////////
void Option::SetAbbrev(
    std::string const& abbrev ) const
{
    m_abbrev = abbrev;
}
////////////////////////////////////////////////////////////////////////////////
void Option::SetDescript(
    std::string const& descript ) const
{
    m_descript = descript;
}
////////////////////////////////////////////////////////////////////////////////
bool Option::SetValue(
    std::string const& name,
    Var const& value ) const
{
    assert( Convert< double >( value, NaN ) != NaN );
    std::string val = Convert< std::string >( value );
    unsigned int pos;
    bool success = GetPos( pos, name );
    if( success )
    {
        unsigned int len = val.length();
        val.copy( m_value[ pos ], len );
        m_value[ pos ][ len ] = 0;
    }
    return success;
}
////////////////////////////////////////////////////////////////////////////////
void Option::Write() const
{
    writeoption( m_type, m_head, m_headlines,
        &m_abbrev[ 0 ], &m_descript[ 0 ], m_value, m_name, m_heading );
}
////////////////////////////////////////////////////////////////////////////////

} //end file
} //end daycent
} //end wrapper
} //end leaf
