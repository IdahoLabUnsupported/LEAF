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
#include <leaf/wrapper/java/lang/Object.h>
#include <leaf/wrapper/java/lang/String.h>

namespace leaf
{
namespace wrapper
{
namespace weru
{
namespace mcrew
{

class SkelImportPanel;
typedef boost::shared_ptr< SkelImportPanel > SkelImportPanelPtr;

class SkelImportPanel : public java::lang::Object
{
public:
    ///
    SkelImportPanel();

    ///
    static SkelImportPanelPtr Create();

    ///
    virtual ~SkelImportPanel();

    ///
    bool SkelToMan(
        java::lang::String const& filename,
        java::lang::String const& manfile );

    ///
    static jclass const& GetJclass();

protected:

private:
    ///
    bool SetDatabases(
        java::lang::String const& opdb,
        java::lang::String const& vegdb );

    ///
    static jmethodID const& GetJmid(
        std::string const& name );

};

} //end mcrew
} //end weru
} //end wrapper
} //end leaf
