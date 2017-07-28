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
#include <leaf/wrapper/daycent/file/Option.h>

// --- Boost Includes --- //
#include <boost/noncopyable.hpp>

namespace leaf
{
namespace wrapper
{
namespace daycent
{
namespace file
{

class F_100 : public boost::noncopyable
{
public:
    ///
    void AddOption(
        Option const& option );

    ///
    void CloseFile();

    ///
    void DeleteOption(
        std::string const& abbrev = "" );

    ///
    void DeleteOption(
        Option const& option );

    ///
    void Exit();

    ///
    Option const& GetOption(
        std::string const& abbrev = "" ) const;

    ///
    Options const& GetOptions() const;

    ///
    void Init(
        std::string const& dirRoot );

    ///
    static F_100* instance(
        bool const& erase = false );

    ///
    bool OpenFile(
       unsigned int const& type,
       std::string const& path,
       std::string const& abbr );

    ///
    void Weather(
        std::string const& abbr,
        std::string const& path );

    ///
    void WriteOptions() const;

protected:
    ///
    F_100();

    ///
    ~F_100();

private:
    ///
    OptByAbb::const_iterator Find(
        std::string const& abbrev = "" ) const;

    ///
    void GenerateKey(
        int const& vk,
        bool const& extended = false );

    ///
    void GenerateKeys(
        std::string const& s );

    ///
    bool ReadOptions(
        unsigned int const& type );

    ///
    bool m_fileIsOpen;

    ///
    std::string m_dirRoot;

    ///
    Options m_options;

};

} //end file
} //end daycent
} //end wrapper
} //end leaf
