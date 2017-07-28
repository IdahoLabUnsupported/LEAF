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
#include <leaf/util/DataHelper.h>

extern "C"
{
#include <F_header.inc>
}

// --- STL Includes --- //
#include <string>
#include <vector>
#include <map>

namespace leaf
{
namespace wrapper
{
namespace daycent
{
namespace file
{

///
class Option
{
public:
    ///
    Option(
        int const& numparms,
        int const& type,
        int const& head,
        int const* const headlines,
        char const* const abbrev,
        char const* const descript,
        char value[][ MWORD ],
        char name[][ MWORD ],
        char heading[][ BWORD ] );

    ///
    ~Option();

    /*///Copy Constructor
    Option(
        const Option& input );

    ///Equal operator
    Option& operator =(
        const Option& input );*/

    ///
    bool DeleteValue(
        std::string const& name ) const;

    ///
    std::string const& GetAbbrev() const;

    ///
    std::string const& GetDescript() const;

    ///
    bool GetValue(
        std::string const& name,
        Poco::Dynamic::Var& value ) const;

    ///
    void SetAbbrev(
        std::string const& abbrev ) const;

    ///
    void SetDescript(
        std::string const& descript ) const;

    ///
    bool SetValue(
        std::string const& name,
        Poco::Dynamic::Var const& value ) const;

    ///
    void Write() const;

protected:

private:
    ///
    bool GetPos(
        unsigned int& pos,
        std::string const& name ) const;

    ///
    int m_numparms;

    ///
    mutable int m_type;

    ///
    mutable int m_head;

    ///
    mutable int m_headlines[ HEADERS ];

    ///
    mutable std::string m_abbrev;

    ///
    mutable std::string m_descript;

    ///
    mutable char m_value[ MAXVALS ][ MWORD ];

    ///
    mutable char m_name[ MAXVALS ][ MWORD ];

    ///
    mutable char m_heading[ HEADERS ][ BWORD ];

    ///
    typedef std::map< std::string, unsigned int > VAL_POS_MAP;
    mutable VAL_POS_MAP m_valPosMap;

};

///
struct SEQ{};
struct ABB{};
typedef boost::multi_index_container<
    Option,
    boost::multi_index::indexed_by<
        //Sort by insertion sequence
        boost::multi_index::sequenced<
            boost::multi_index::tag< SEQ >
        >,

        //Sort by abbrev
        boost::multi_index::hashed_unique<
            boost::multi_index::tag< ABB >,
            BOOST_MULTI_INDEX_CONST_MEM_FUN(
                Option, std::string const&, GetAbbrev )
        >
    >
> Options;

typedef Options::index< SEQ >::type OptBySeq;
typedef Options::index< ABB >::type OptByAbb;

} //end file
} //end daycent
} //end wrapper
} //end leaf
