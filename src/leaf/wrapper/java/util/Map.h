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
#include <leaf/wrapper/java/lang/Object.h>

#include <leaf/wrapper/java/util/Set.h>

namespace leaf
{
namespace wrapper
{
namespace java
{
namespace util
{

template< typename K, typename V,
    bool keyIsObject = boost::is_base_of< lang::Object, K >::value,
    bool valIsObject = boost::is_base_of< lang::Object, V >::value >
class Map : public lang::Object
{
public:

    ///
    typedef typename boost::shared_ptr< K > KeyPtr;
    typedef typename boost::shared_ptr< V > ValuePtr;
    typedef typename boost::shared_ptr< Map< K, V > > SPtr;
    typedef typename leaf::util::Select<
        keyIsObject, KeyPtr, K >::Result KeyType;
    typedef typename leaf::util::Select<
        valIsObject, ValuePtr, V >::Result ValueType;

    ///
    class Entry : public lang::Object
    {
    public:
        ///
        Entry(
            jobject const& o )
            :
            lang::Object( o )
        {
            ;
        }

        ///
        virtual ~Entry()
        {
            ;
        }

        ///
        KeyType GetKey() const
        {
            return GetKey( leaf::util::Int2Type< keyIsObject >() );
        }

        ///
        ValueType GetValue() const
        {
            return GetValue( leaf::util::Int2Type< valIsObject >() );
        }

        ///
        static jclass const& GetJclass()
        {
            static jclass const clazz =
                java::FindClass( "java/util/Map$Entry" );
            return clazz;
        }

    protected:

    private:
        ///
        KeyType GetKey(
            leaf::util::Int2Type< true > ) const
        {
            return Create< K >( java::CallMethod< jobject >(
                m_jobject, GetJmid( "getKey" ) ) );
        }

        ///
        KeyType GetKey(
            leaf::util::Int2Type< false > ) const
        {
            return java::CallMethod< K >( m_jobject, GetJmid( "getKey" ) );
        }

        ///
        ValueType GetValue(
            leaf::util::Int2Type< true > ) const
        {
            return Create< V >( java::CallMethod< jobject >(
                m_jobject, GetJmid( "getValue" ) ) );
        }

        ///
        ValueType GetValue(
            leaf::util::Int2Type< false > ) const
        {
            return java::CallMethod< V >( m_jobject, GetJmid( "getValue" ) );
        }

        ///
        static jmethodID const& GetJmid(
            std::string const& name )
        {
            static java::JMIDMAP const jmidMap = boost::assign::map_list_of
                ( "getKey",
                  java::GetMethodID(
                      GetJclass(), "getKey",
                      "()Ljava/lang/Object;" ) )
                ( "getValue",
                  java::GetMethodID(
                      GetJclass(), "getValue",
                      "()Ljava/lang/Object;" ) );

            java::JMIDMAP::const_iterator itr = jmidMap.find( name );
            return itr->second;
        }

    };

    ///
    Map(
        jobject const& o )
        :
        lang::Object( o )
    {
        ;
    }

    ///
    virtual ~Map()
    {
        ;
    }

    ///
    typename Set< Entry >::SPtr EntrySet() const
    {
        return Create< Set< typename Map< K, V >::Entry > >(
            java::CallMethod< jobject >( m_jobject, GetJmid( "entrySet" ) ) );
    }

    ///
    ValueType Get(
        K const& key ) const
    {
        return Get( key, leaf::util::Int2Type< valIsObject >() );
    }

    ///
    typename Set< K >::SPtr KeySet() const
    {
        return Create< Set< K > >(
            java::CallMethod< jobject >( m_jobject, GetJmid( "keySet" ) ) );
    }

    ///
    static jclass const& GetJclass()
    {
        static jclass const clazz = java::FindClass( "java/util/Map" );
        return clazz;
    }

protected:

private:
    ///
    ValueType Get(
        K const& key,
        leaf::util::Int2Type< true > ) const
    {
        return Create< V >( java::CallMethod< jobject >(
            m_jobject, GetJmid( "get" ), key.Jobj() ) );
    }

    ///
    ValueType Get(
        K const& key,
        leaf::util::Int2Type< false > ) const
    {
        return java::CallMethod< V >( m_jobject, GetJmid( "get" ), key );
    }

    ///
    static jmethodID const& GetJmid(
        std::string const& name )
    {
        static java::JMIDMAP const jmidMap = boost::assign::map_list_of
            ( "entrySet",
              java::GetMethodID(
                  GetJclass(), "entrySet",
                  "()Ljava/util/Set;" ) )
            ( "get",
              java::GetMethodID(
                  GetJclass(), "get",
                  "(Ljava/lang/Object;)Ljava/lang/Object;" ) )
            ( "keySet",
              java::GetMethodID(
                  GetJclass(), "keySet",
                  "()Ljava/util/Set;" ) );

        java::JMIDMAP::const_iterator itr = jmidMap.find( name );
        return itr->second;
    }

};

} //end util
} //end java
} //end wrapper
} //end leaf
