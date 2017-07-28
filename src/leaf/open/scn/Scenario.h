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

#include <leaf/open/scn/Common.h>
#include <leaf/open/scn/ScenarioPtr.h>

#include <leaf/util/DataHelper.h>

namespace leaf
{
namespace open
{
namespace scn
{

class LEAF_OPEN_EXPORTS Scenario : public Object
{
public:
    ///
    virtual ~Scenario();

    ///
    static ScenarioPtr Create(
        Poco::Dynamic::Var const& name,
        Poco::Dynamic::Var const& soil,
        Poco::Dynamic::Var const& rotation,
        Poco::Dynamic::Var const& tillage,
        Poco::Dynamic::Var const& removal,
        Poco::Dynamic::Var const& yield,
        Poco::Nullable< Poco::Dynamic::Var > const& spatial =
            Poco::Data::Keywords::null,
        Poco::Nullable< Poco::Dynamic::Var > const& climate =
            Poco::Data::Keywords::null,
        Poco::Nullable< Poco::Dynamic::Var > const& cover =
            Poco::Data::Keywords::null,
        Poco::Nullable< Poco::Dynamic::Var > const& barrier =
            Poco::Data::Keywords::null,
        Poco::Nullable< Poco::Dynamic::Var > const& id =
            Poco::Data::Keywords::null );

    ///
    static void DeleteFromDB(
        Poco::Dynamic::Var const& id );

    ///
    static bool Exists(
        Poco::Dynamic::Var const& id );

    ///
    static ScenarioPtr Extract(
        Poco::Dynamic::Var const& id );

    ///
    Poco::Dynamic::Var const& GetId() const;

    ///
    Poco::Dynamic::Var const& GetName() const;

    ///
    Poco::Nullable< Poco::Dynamic::Var > const& GetSpatial() const;

    ///
    Poco::Nullable< Poco::Dynamic::Var > const& GetClimate() const;

    ///
    Poco::Dynamic::Var const& GetSoil() const;

    ///
    Poco::Dynamic::Var const& GetRotation() const;

    ///
    Poco::Nullable< Poco::Dynamic::Var > const& GetCover() const;

    ///
    Poco::Dynamic::Var const& GetTillage() const;

    ///
    Poco::Dynamic::Var const& GetRemoval() const;

    ///
    Poco::Nullable< Poco::Dynamic::Var > const& GetBarrier() const;

    ///
    Poco::Dynamic::Var const& GetYield() const;

#ifndef SWIG
    ///
    LEAF_OPEN_EXPORTS
    friend std::ostream& operator <<(
        std::ostream& os,
        ScenarioPtr const& scenario );
#endif //SWIG

protected:
    ///
    Scenario();

    ///
    Scenario(
        Poco::Dynamic::Var const& name,
        Poco::Dynamic::Var const& soil,
        Poco::Dynamic::Var const& rotation,
        Poco::Dynamic::Var const& tillage,
        Poco::Dynamic::Var const& removal,
        Poco::Dynamic::Var const& yield,
        Poco::Nullable< Poco::Dynamic::Var > const& spatial =
            Poco::Data::Keywords::null,
        Poco::Nullable< Poco::Dynamic::Var > const& climate =
            Poco::Data::Keywords::null,
        Poco::Nullable< Poco::Dynamic::Var > const& cover =
            Poco::Data::Keywords::null,
        Poco::Nullable< Poco::Dynamic::Var > const& barrier =
            Poco::Data::Keywords::null,
        Poco::Nullable< Poco::Dynamic::Var > const& id =
            Poco::Data::Keywords::null );

    ///
    static ScenarioPtr Create();

private:
    ///
    friend class Poco::Data::TypeHandler< Scenario >;
    template< typename T > friend class Poco::Data::Extraction;
    friend void boost::detail::allocator::construct< Scenario >(
        void*, Scenario const& );
    friend ScenarioPtr boost::make_shared< Scenario >();

    ///
    void CheckIfExists(
        Poco::Nullable< Poco::Dynamic::Var > const& id );

    ///
    void WriteToDB();

    ///
    Poco::Dynamic::Var m_id;

    ///
    Poco::Dynamic::Var m_name;

    ///
    Poco::Nullable< Poco::Dynamic::Var > m_spatial;

    ///
    Poco::Nullable< Poco::Dynamic::Var > m_climate;

    ///
    Poco::Dynamic::Var m_soil;

    ///
    Poco::Dynamic::Var m_rotation;

    ///
    Poco::Nullable< Poco::Dynamic::Var > m_cover;

    ///
    Poco::Dynamic::Var m_tillage;

    ///
    Poco::Dynamic::Var m_removal;

    ///
    Poco::Nullable< Poco::Dynamic::Var > m_barrier;

    ///
    Poco::Dynamic::Var m_yield;

};

} //end scn
} //end open
} //end leaf

namespace Poco
{
namespace Data
{

#ifndef SWIG
template<>
class TypeHandler< class leaf::open::scn::Scenario >
{
public:
    ///
    static std::size_t size()
    {
        //We handle eleven columns of the Table
        return 11;
    }

    ///
    static void bind(
        std::size_t pos,
        leaf::open::scn::Scenario const& obj,
        AbstractBinder* pB,
        AbstractBinder::Direction dir )
    {
        poco_assert_dbg( pB != NULL );

        //id
        TypeHandler< Dynamic::Var >::bind( pos++,
            obj.m_id, pB, dir );
        //name
        TypeHandler< Dynamic::Var >::bind( pos++,
            obj.m_name, pB, dir );
        //spatial
        TypeHandler< Nullable< Dynamic::Var > >::bind( pos++,
            obj.m_spatial, pB, dir );
        //climate
        TypeHandler< Nullable< Dynamic::Var > >::bind( pos++,
            obj.m_climate, pB, dir );
        //soil
        TypeHandler< Dynamic::Var >::bind( pos++,
            obj.m_soil, pB, dir );
        //rotation
        TypeHandler< Dynamic::Var >::bind( pos++,
            obj.m_rotation, pB, dir );
        //cover
        TypeHandler< Nullable< Dynamic::Var > >::bind( pos++,
            obj.m_cover, pB, dir );
        //tillage
        TypeHandler< Dynamic::Var >::bind( pos++,
            obj.m_tillage, pB, dir );
        //removal
        TypeHandler< Dynamic::Var >::bind( pos++,
            obj.m_removal, pB, dir );
        //barrier
        TypeHandler< Nullable< Dynamic::Var > >::bind( pos++,
            obj.m_barrier, pB, dir );
        //yield
        TypeHandler< Dynamic::Var >::bind( pos++,
            obj.m_yield, pB, dir );
    }

    ///
    static void prepare(
        std::size_t pos,
        leaf::open::scn::Scenario& obj,
        AbstractPreparator* pP )
    {
        poco_assert_dbg( pP != NULL );

        //id
        TypeHandler< Dynamic::Var >::prepare( pos++,
            obj.m_id, pP );
        //name
        TypeHandler< Dynamic::Var >::prepare( pos++,
            obj.m_name, pP );
        //spatial
        TypeHandler< Nullable< Dynamic::Var > >::prepare( pos++,
            obj.m_spatial, pP );
        //climate
        TypeHandler< Nullable< Dynamic::Var > >::prepare( pos++,
            obj.m_climate, pP );
        //soil
        TypeHandler< Dynamic::Var >::prepare( pos++,
            obj.m_soil, pP );
        //rotation
        TypeHandler< Dynamic::Var >::prepare( pos++,
            obj.m_rotation, pP );
        //cover
        TypeHandler< Nullable< Dynamic::Var > >::prepare( pos++,
            obj.m_cover, pP );
        //tillage
        TypeHandler< Dynamic::Var >::prepare( pos++,
            obj.m_tillage, pP );
        //removal
        TypeHandler< Dynamic::Var >::prepare( pos++,
            obj.m_removal, pP );
        //barrier
        TypeHandler< Nullable< Dynamic::Var > >::prepare( pos++,
            obj.m_barrier, pP );
        //yield
        TypeHandler< Dynamic::Var >::prepare( pos++,
            obj.m_yield, pP );
    }

    ///Note: defVal is never used for Nullable<> TypeHandler
    static void extract(
        std::size_t pos,
        leaf::open::scn::Scenario& obj,
        leaf::open::scn::Scenario const& def,
        AbstractExtractor* pE )
    {
        poco_assert_dbg( pE != NULL );

        //id
        TypeHandler< Dynamic::Var >::extract( pos++,
            obj.m_id, def.m_id, pE );
        //name
        TypeHandler< Dynamic::Var >::extract( pos++,
            obj.m_name, def.m_name, pE );
        //spatial
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_spatial, Keywords::null, pE );
        //climate
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_climate, Keywords::null, pE );
        //soil
        TypeHandler< Dynamic::Var >::extract( pos++,
            obj.m_soil, def.m_soil, pE );
        //rotation
        TypeHandler< Dynamic::Var >::extract( pos++,
            obj.m_rotation, def.m_rotation, pE );
        //cover
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_cover, Keywords::null, pE );
        //tillage
        TypeHandler< Dynamic::Var >::extract( pos++,
            obj.m_tillage, def.m_tillage, pE );
        //removal
        TypeHandler< Dynamic::Var >::extract( pos++,
            obj.m_removal, def.m_removal, pE );
        //barrier
        TypeHandler< Nullable< Dynamic::Var > >::extract( pos++,
            obj.m_barrier, Keywords::null, pE );
        //yield
        TypeHandler< Dynamic::Var >::extract( pos++,
            obj.m_yield, def.m_yield, pE );
    }

};
#endif //SWIG

} //end Data
} //end Poco
