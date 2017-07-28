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
#include <leaf/wrapper/daycent/out/Definitions.h>

// --- Boost Includes --- //
#include <boost/filesystem.hpp>

namespace leaf
{
namespace wrapper
{
namespace daycent
{
namespace out
{

///
struct TIME{};
typedef std::vector< Poco::Dynamic::Var > Record;
typedef boost::shared_ptr< Record > RecordPtr;

template< unsigned int T = TIME_DAILY >
struct RecordSet
{
    typedef boost::multi_index_container<
        RecordPtr,
        boost::multi_index::indexed_by<
            //Sort by time and then by dayofyr
            boost::multi_index::ordered_non_unique<
                boost::multi_index::tag< TIME >,
                boost::multi_index::composite_key<
                    Record,
                    BOOST_MULTI_INDEX_CONST_MEM_FUN(
                        Record, Poco::Dynamic::Var const&, front ),
                    BOOST_MULTI_INDEX_CONST_MEM_FUN(
                        Record, Poco::Dynamic::Var const&, front )
                >,
                boost::multi_index::composite_key_compare<
                    util::less_than< float >,
                    util::less_than< int >
                >
            >
        >
    > type;
};

template<>
struct RecordSet< TIME_MONTHLY >
{
    typedef boost::multi_index_container<
        RecordPtr,
        boost::multi_index::indexed_by<
            //Sort by time
            boost::multi_index::ordered_unique<
                boost::multi_index::tag< TIME >,
                BOOST_MULTI_INDEX_CONST_MEM_FUN(
                    Record, Poco::Dynamic::Var const&, front ),
                util::less_than< float >
            >
        >
    > type;
};

template<>
struct RecordSet< TIME_YEARLY >
{
    typedef RecordSet< TIME_MONTHLY >::type type;
};


//typedef RecordSet::index< TIME >::type RecordByTime;

///
class File
{
public:
    ///
    File();

    ///
    ~File();

    ///
    FILE_NAMES id;

    ///
    mutable bool output;

    ///
    std::string name;

    ///
    friend std::ostream& operator <<(
        std::ostream& os,
        File const& file );

protected:

private:

};

///
struct SEQ{};
typedef boost::multi_index_container<
    File,
    boost::multi_index::indexed_by<
        //Sort by insertion sequence
        boost::multi_index::sequenced<
            boost::multi_index::tag< SEQ >
        >,
        //Sort by FILE_NAMES
        boost::multi_index::ordered_unique<
            boost::multi_index::tag< ID >,
            BOOST_MULTI_INDEX_MEMBER(
                File, FILE_NAMES, id )
        >
    >
> Files;
typedef Files::index< SEQ >::type FilesBySeq;
typedef Files::index< ID >::type FilesById;

} //end out
} //end daycent
} //end wrapper
} //end leaf
