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

#include <leaf/wrapper/java/lang/Object.h>

namespace leaf
{
namespace wrapper
{
namespace java
{
namespace lang
{

class String;
typedef boost::shared_ptr< String > StringPtr;

class LEAF_JAVA_EXPORTS String : public Object
{
public:
    ///
    String();

    ///
    String(
        jobject const& o );

    ///
    String(
        char const* const& cstr );

    ///
    String(
        std::string const& str );

    ///
    String(
        Poco::Dynamic::Var const& var );

    ///
    String(
        Poco::Nullable< Poco::Dynamic::Var > const& var );

    ///
    virtual ~String();

    ///
    static jclass const& GetJclass();

    ///
    virtual jstring Jobj() const;

    ///
    std::string StdString() const;

protected:

private:
    ///
    jstring NewString(
        std::string const& str );

    ///
    static jmethodID const& GetJmid(
        std::string const& name );

};

} //end lang
} //end java
} //end wrapper
} //end leaf
