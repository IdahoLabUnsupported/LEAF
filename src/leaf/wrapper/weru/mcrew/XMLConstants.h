// Copyright 2017 Battelle Energy Alliance, LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


#pragma once

// --- LEAF Includes --- //
#include <leaf/wrapper/java/lang/String.h>

namespace leaf
{
namespace wrapper
{
namespace weru
{
namespace mcrew
{

///STATIC INTERFACE
class XMLConstants : public boost::noncopyable
{
public:
    ///
    static void Static();

    ///
    static java::lang::String const& soperation();

    ///
    static java::lang::String const& scrop();

    ///
    static jclass const& GetJclass();

protected:
    ///
    XMLConstants();

    ///
    ~XMLConstants();

private:
    ///
    static XMLConstants& Instance(
        bool const& erase = false );

    ///
    static jmethodID const& GetJmid(
        std::string const& name );

    ///
    static jfieldID const& GetJfid(
        std::string const& name );

    ///
    java::lang::String const m_soperation;

    ///
    java::lang::String const m_scrop;


};

} //end mcrew
} //end weru
} //end wrapper
} //end leaf
