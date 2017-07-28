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

#include <leaf/open/Object.h>

#include <leaf/open/man/Common.h>
#include <leaf/open/man/CropPtr.h>

namespace leaf
{
namespace open
{
namespace man
{

class LEAF_OPEN_EXPORTS Crop
{
public:
    ///Default constructor
    Crop();

    ///
    static CropPtr Create();

    ///
    Crop(
        std::string const& name );

    ///Copy constructor
    Crop(
        Crop const& crop );

    ///
    static CropPtr Create(
        Crop const& crop );

#if _MSC_VER >= 1800
    ///Move constructor
    Crop(
        Crop&& crop );
#endif //_MSC_VER

    ///Destructor
    ~Crop();

    ///Assignment operator
    Crop& operator =(
        Crop crop );

#ifndef SWIG
    ///Swap function
    friend void swap(
        Crop& first,
        Crop& second );
#endif //SWIG

    ///
    std::string const& GetName() const;

    ///
    void SetName(
        std::string const& name );

#ifndef SWIG
    ///
    LEAF_OPEN_EXPORTS
    friend std::ostream& operator <<(
        std::ostream& os,
        Crop const& crop );
#endif //SWIG

protected:

private:
    ///
    friend class Operation;
    friend class Poco::Data::TypeHandler< Crop >;
    template< typename T > friend class Poco::Data::Extraction;
    friend void boost::detail::allocator::construct< Crop >(
        void*, Crop const& );
    friend CropPtr boost::make_shared< Crop >();

    ///varchar() not null
    std::string m_name;

};

#ifndef SWIG
///
struct CROP_SEQ{};

///
typedef boost::multi_index_container<
    Crop,
    boost::multi_index::indexed_by<
        //Sort by insertion sequence
        boost::multi_index::sequenced<
            boost::multi_index::tag< CROP_SEQ >
        >
    >
> Crops;

///
typedef Crops::index< CROP_SEQ >::type CropsBySeq;
#endif //SWIG

} //end man
} //end open
} //end leaf

namespace Poco
{
namespace Data
{

#ifndef SWIG
template<>
class TypeHandler< class leaf::open::man::Crop >
{
public:
    ///
    static std::size_t size()
    {
        //We handle zero columns of the Table
        return 0;
    }

    ///
    static void bind(
        std::size_t pos,
        leaf::open::man::Crop const& obj,
        AbstractBinder* pB,
        AbstractBinder::Direction dir )
    {
        poco_assert_dbg( pB != NULL );

        //Should never make it here
        poco_assert( false );
    }

    ///
    static void prepare(
        std::size_t pos,
        leaf::open::man::Crop& obj,
        AbstractPreparator* pP )
    {
        poco_assert_dbg( pP != NULL );

        //Should never make it here
        poco_assert( false );
    }

    ///Note: defVal is never used for Nullable<> TypeHandler
    static void extract(
        std::size_t pos,
        leaf::open::man::Crop& obj,
        leaf::open::man::Crop const& defVal,
        AbstractExtractor* pE )
    {
        poco_assert_dbg( pE != NULL );


    }

};
#endif //SWIG

} //end Data
} //end Poco
