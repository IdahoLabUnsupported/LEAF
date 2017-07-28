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
#include <leaf/open/soi/MapUnit.h>

#include <leaf/util/DataHelper.h>
#include <leaf/util/SoilHelper.h>

#include <leaf/wrapper/java/lang/Object.h>

namespace leaf
{
namespace wrapper
{
namespace weru
{
namespace soil
{

class NASIS;
typedef boost::shared_ptr< NASIS > NASISPtr;

class NASIS : public java::lang::Object
{
public:
    ///
    NASIS(
        jobject const& o );

    ///
    NASIS(
        std::size_t const& inpnumlay );

    ///
    virtual ~NASIS();

    ///
    util::SoilError LoadSQL(
        open::soi::ComponentPtr const& component );

    ///
    static jclass const& GetJclass();

protected:

private:
    ///
    Poco::Nullable< Poco::Dynamic::Var > CopyComponent(
        open::soi::ComponentPtr const& component );

    ///
    void CopyMapUnit(
        open::soi::MapUnit const& mapunit );

    ///
    void CopyMuAggatt(
        open::soi::MapUnit const& mapunit );

    ///
    void CopyCoRestrictions(
        open::soi::ComponentPtr const& component );

    ///
    void CopyLegend(
        open::soi::MapUnit const& mapunit );

    ///
    void CopyHorizon(
        open::soi::CHorizon const& horizon,
        unsigned int const& idx );

    ///
    void CopyFrags(
        open::soi::ChFrags const& frags,
        unsigned int const& idx );

    ///
    void CopyTexture(
        open::soi::ChTextureGrpPtr const& chtgrp,
        unsigned int const& idx );

    ///
    static jmethodID const& GetJmid(
        std::string const& name );

    ///
    static jfieldID const& GetJfid(
        std::string const& name );

    ///
    static std::set< std::string > const ORGANIC_TEXTURES;

    ///
    bool IsLayerOrganic(
        std::string const& hzname,
        std::string const& desgnmaster,
        std::string const& texture,
        double const& organicMatter );

};

} //end soil
} //end weru
} //end wrapper
} //end leaf
