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

#include <leaf/open/ObjectPtr.h>

#include <leaf/util/DataHelper.h>

// --- Boost Includes --- //
#include <boost/noncopyable.hpp>

namespace leaf
{
namespace open
{

///
class LEAF_OPEN_EXPORTS Object
#ifndef SWIG
    : public boost::noncopyable
#endif //SWIG
{
public:
    ///
    Object(
        Poco::Dynamic::Struct< std::string >::Data const& val );

    ///
    virtual ~Object();

    ///
    void FromJson(
        std::string const& json );

    ///
    std::string ToJson();

protected:
    ///
    Object();

    ///
    Poco::Dynamic::Var m_struct;

private:

};

} //end open
} //end leaf
