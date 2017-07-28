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
#include <leaf/wrapper/java/io/InputStream.h>

#include <leaf/wrapper/java/lang/Object.h>
#include <leaf/wrapper/java/lang/String.h>

namespace leaf
{
namespace wrapper
{
namespace java
{
namespace util
{

class Properties;
typedef boost::shared_ptr< Properties > PropertiesPtr;

class LEAF_JAVA_EXPORTS Properties : public lang::Object
{
public:
    ///
    Properties();

    ///
    Properties(
        jobject const& o );

    ///
    virtual ~Properties();

    ///
    lang::StringPtr GetProperty(
        lang::String const& key ) const;

    ///
    void Load(
        io::InputStream const& inStream ) const;

    ///
    lang::StringPtr SetProperty(
        lang::String const& key,
        lang::String const& value ) const;

    ///
    static jclass const& GetJclass();

protected:

private:
    ///
    static jmethodID const& GetJmid(
        std::string const& name );

};

} //end util
} //end java
} //end wrapper
} //end leaf
