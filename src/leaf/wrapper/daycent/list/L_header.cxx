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
#include <leaf/wrapper/daycent/list/L_header.h>

// --- Boost Includes --- //
#include <boost/assign.hpp>
#include <boost/filesystem.hpp>

#include <boost/algorithm/string.hpp>

namespace fs = boost::filesystem;

// --- STL Includes --- //
#include <iomanip>
#include <iostream>
#include <sstream>
#include <numeric>

unsigned int const NUM_TVALS( 3 ), TBL_HGT( 445 ), STR_LEN( 15 );
unsigned int const TBL_SZ( NUM_TVALS * TBL_HGT * STR_LEN );

extern "C"
{
extern struct
{
    int tvals[ NUM_TVALS ];
    char table[ TBL_SZ ];
} tables_;
}

namespace leaf
{
namespace wrapper
{
namespace daycent
{
namespace list
{

////////////////////////////////////////////////////////////////////////////////
L_100::L_100()
    :
    m_dirRoot(),
    m_ps(),
    m_found(),
    m_recordSet()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
L_100::~L_100()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
void L_100::Exit()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
void L_100::Init(
    std::string const& dirRoot )
{
    m_dirRoot = dirRoot;

    //Transpose extern char array
    int* tvs = tables_.tvals;
    char* tbl = tables_.table;
    std::string str; str.reserve( STR_LEN );
    m_ps.assign( tvs, tvs + NUM_TVALS );
    std::partial_sum( m_ps.begin(), m_ps.end(), m_ps.begin() );
    m_ps.push_front( 0 );
    m_found.insert( std::make_pair( "time", 0 ) );
    for( unsigned int i = 0; i < TBL_HGT; ++i )
    {
        for( unsigned int j = 0; j < NUM_TVALS; ++j )
        {
            for( unsigned int k = 0; k < STR_LEN; ++k )
            {
                char& c = tbl[ ( ( ( i * NUM_TVALS ) + j ) * STR_LEN ) + k ];
                if( c == ' ' || c == '\0' ) break;
                str.push_back( c );
            }
            if( str.empty() ) continue;
            m_found.insert( std::make_pair( str, m_ps.at( j ) + i + 1 ) );
            str.clear();
        }
    }
}
////////////////////////////////////////////////////////////////////////////////
L_100* L_100::instance(
    bool const& erase )
{
    static L_100* sL100 = new L_100();
    if( erase )
    {
        sL100->Exit();
        delete sL100;
        sL100 = NULL;
    }

    return sL100;
}
////////////////////////////////////////////////////////////////////////////////
void L_100::Read(
    std::string const& binFile,
    double const& stime,
    double const& etime )
{
    fs::path cp( fs::current_path() );
    fs::path temp( cp/"daycent" );
    fs::current_path( temp );

    //
    std::ifstream in( binFile.c_str(), std::ios::in | std::ios::binary );
    assert( in.is_open() );

    //Unformatted Sequential File
    short int bof( 0 ), l1( 0 ), l2( 0 ), gcnt( 0 );
    short int const rs( *( m_ps.rbegin() ) + 1 );
    short int const gc( rs * 4 ), eof( 130 ), bs( 128 ), of( bs + 1 );
    //Add very small value to et b/c of real numbers in binary
    float st( stime ), et( etime );//et += 0.000999;
    RecordPtr rec = boost::make_shared< Record >( rs );
    m_recordSet.clear();

    //Beginning-of-file byte (75 decimal)
    in.read( reinterpret_cast< char* >( &bof ), 1 ); assert( bof == 75 );
    while( in.good() )
    {
        //Where record begins
        in.read( reinterpret_cast< char* >( &l1 ), 1 ); assert( l1 <= eof );
        //End-of-file byte (130 decimal)
        if( l1 == eof ) break;

        //When you create a logical record of more than one physical block,
        //the compiler sets the length byte to 129 to indicate that the data in
        //the current physical block continues on into the next physical block.
        assert( gcnt % 4 == 0 );
        gcnt += in.read( reinterpret_cast< char* >(
            &(*rec)[ 0 ] + ( gcnt / 4 ) ), ( l1 == of ) ? bs : l1 ).gcount();
        if( gcnt == gc )
        {
            gcnt = 0;
            float const& time = (*rec).at( 0 );
            if( ( st == -1 && et == -1 ) ||
                ( st != -1 && time >= st && et == -1 ) ||
                ( st == -1 && et != -1 && time <= et ) ||
                ( st != -1 && time >= st && et != -1 && time <= et ) )
            {
                m_recordSet.insert( rec );
                rec = boost::make_shared< Record >( rs );
            }
        }

        //Where record ends
        in.read( reinterpret_cast< char* >( &l2 ), 1 ); assert( l2 == l1 );
    }
    in.close();

    fs::current_path( cp );
}
////////////////////////////////////////////////////////////////////////////////
Record const& L_100::GetRecord(
    float const& time ) const
{
    return *GetRecordPtr( time );
}
////////////////////////////////////////////////////////////////////////////////
RecordPtr const& L_100::GetRecordPtr(
    float const& time ) const
{
    RecordByTime::const_iterator itr =
        m_recordSet.get< TIME >().find( time );
    if( itr == m_recordSet.get< TIME >().end() )
    {
        std::stringstream err;
        err << "leaf::wrapper::daycent::list::L_100::GetRecord( \"" << time
            << "\" ) could not find specified time in RecordSet!";

        throw std::bad_exception( err.str().c_str() );
    }

    return *itr;
}
////////////////////////////////////////////////////////////////////////////////
RecordSet const& L_100::GetRecordSet() const
{
    return m_recordSet;
}
////////////////////////////////////////////////////////////////////////////////
float L_100::GetValue(
    Record const& record,
    std::string const& name )
{
    LU_MAP::const_iterator itr = m_found.find( name );
    if( itr == m_found.end() )
    {
        std::stringstream err;
        err << "leaf::wrapper::daycent::list::L_100::GetValue( \"" << name
            << "\" ) could not find specified value in Record!";

        throw std::bad_exception( err.str().c_str() );
    }
    float const& value = record.at( itr->second );

    //Zero the displayed output for very small values
    return ( fabs( value ) < 1.0e-9 ) ? 0.0 : value;
}
////////////////////////////////////////////////////////////////////////////////

} //end list
} //end daycent
} //end wrapper
} //end leaf
