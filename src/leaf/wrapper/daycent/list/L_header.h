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

// --- Boost Includes --- //
#include <boost/noncopyable.hpp>

// --- STL Includes --- //
#include <map>
#include <string>
#include <vector>
#include <deque>

namespace leaf
{
namespace wrapper
{
namespace daycent
{
namespace list
{

///
struct TIME{};
typedef std::vector< float > Record;
typedef boost::shared_ptr< Record > RecordPtr;
typedef boost::multi_index_container<
    RecordPtr,
    boost::multi_index::indexed_by<
        //Sort by time
        boost::multi_index::ordered_unique<
            boost::multi_index::tag< TIME >,
            BOOST_MULTI_INDEX_CONST_MEM_FUN(
                Record, float const&, front )
        >
    >
> RecordSet;
typedef RecordSet::index< TIME >::type RecordByTime;

///
class LEAF_DAYCENT_EXPORTS L_100 : public boost::noncopyable
{
public:
    ///
    void Exit();

    ///
    void Init(
        std::string const& dirRoot );

    ///
    static L_100* instance(
        bool const& erase = false );

    ///
    void Read(
        std::string const& binFile,
        double const& stime = -1,
        double const& etime = -1 );

    ///
    Record const& GetRecord(
        float const& time ) const;

    ///
    RecordPtr const& GetRecordPtr(
        float const& time ) const;

    ///
    RecordSet const& GetRecordSet() const;

    ///
    float GetValue(
        Record const& record,
        std::string const& name );

protected:
    ///
    L_100();

    ///
    ~L_100();

private:
    ///
    std::string m_dirRoot;

    ///
    std::deque< int > m_ps;

    ///
    typedef std::map< std::string, unsigned int > LU_MAP;
    LU_MAP m_found;

    ///
    RecordSet m_recordSet;

};

} //end list
} //end daycent
} //end wrapper
} //end leaf
