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

#include <leaf/wrapper/daycent/cal/Common.h>

// --- Boost Includes --- //
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/assign/list_of.hpp>

#include <Poco/Nullable.h>

// --- STL Includes --- //
#include <string>
#include <iostream>
#include <map>

namespace leaf
{
namespace wrapper
{
namespace daycent
{
namespace cal
{

class YieldCalibration
{
public:
    ///
    YieldCalibration(void);

    ///
    ~YieldCalibration(void);

    ///
    static YieldCalibration* instance(
        bool const& erase = false );

    ///
    bool CalibrateYield(
        std::string const& abbr,
        std::string const& cokey,
        std::string const& calPath,
        int const& simyr,
        bool dist = false);

    ///
    bool CalibrateBaseline(
        std::string const& abbr,
        std::string const& cokey,
        std::string const& calPath,
        unsigned int simyr=0 );

    ///
    bool RunBaseline(
        std::string const& abbr,
        std::string const& cokey,
        std::string const& path);

    ///
    bool YieldCalibration::BuildDistributedFiles(
        std::string const& abbr,
        std::string const& cokey,
        std::string const& calPath,
        int const& simyr );

    ///
    bool YieldCalibration::RunDistributedFiles(
        std::string const& abbr,
        std::string const& cokey,
        std::string const& calPath,
        int const& simyr );

    ///
    std::vector< double > RunYears(
        std::string const& abbr,
        std::string const& path,
        const int years );

    ///
    void SetYieldInput(
        std::vector< double > yield );

    ///
    void RemoveAllFilesWithExtension(
        std::string const& ext );

    ///
    std::vector< std::string > GetRotation();
    void SetRotation(
        std::vector< std::string > rot );

    ///
    static std::map< std::string, double > const& crop_conv;

    ///
    static std::vector< std::string > const& file_ext;

    ///
    Poco::Dynamic::Var const& GetSci() const;
    void SetSci(
        Poco::Dynamic::Var const& sci );

    ///
    Poco::Dynamic::Var const& GetSciOM() const;
    void SetSciOM(
        Poco::Dynamic::Var const& sciOM );

    ///
    Poco::Dynamic::Var const& GetSciER() const;
    void SetSciER(
        Poco::Dynamic::Var const& sciER );

    ///
    Poco::Dynamic::Var const& GetSciFO() const;
    void SetSciFO(
        Poco::Dynamic::Var const& sciFO );

    ///
    Poco::Dynamic::Var const& GetWindEros() const;
    void SetWindEros(
        Poco::Dynamic::Var const& windEros );

    ///
    Poco::Dynamic::Var const& GetWaterEros() const;
    void SetWaterEros(
        Poco::Dynamic::Var const& waterEros );

    ///
    Poco::Dynamic::Var const& GetTotBioRem() const;
    void SetTotBioRem(
        Poco::Dynamic::Var const& totBioRem );

    ///
    void SetAbbr(
        std::string const& abbr );

    ///
    void SetMukey(
        std::string const& mukey );

    ///
    void SetCokey(
        std::string const& cokey );

    ///
    void SetMuacres(
        std::string const& muacres );

    ///
    void SetPctAcres(
        std::string const& pctacres );

    ///
    void SetTillage(
        std::string const& tillreg );

    ///
    void SetRemoval(
        std::string const& remreg );

    ///
    void SetRotation(
    std::string const& rotreg );

    ///
    void SetYield(
        std::string const& yldreg );

    ///
    std::string const& GetCMZ();
    void SetCMZ(
        std::string const& cmz );

    ///
    void SetPRDX(
        std::vector< double > );

    ///
    void SetField(
        std::string name );

    ///
    std::string m_cmz;

    ///
    std::vector< double > m_yield;

    ///
    std::vector< std::string > m_rot;

    ///
    Poco::Nullable< Poco::Dynamic::Var > m_abbr;

    ///
    Poco::Nullable< Poco::Dynamic::Var > m_mukey;

    ///
    Poco::Nullable< Poco::Dynamic::Var > m_cokey;

    ///
    Poco::Nullable< Poco::Dynamic::Var > m_muacres;

    ///
    Poco::Nullable< Poco::Dynamic::Var > m_pctacres;

    ///
    Poco::Nullable< Poco::Dynamic::Var > m_yld;

    ///
    Poco::Nullable< Poco::Dynamic::Var > m_wtyld;

    ///
    Poco::Nullable< Poco::Dynamic::Var > m_tillreg;

    ///
    Poco::Nullable< Poco::Dynamic::Var > m_remreg;

    ///
    Poco::Nullable< Poco::Dynamic::Var > m_rotreg;

    ///
    Poco::Nullable< Poco::Dynamic::Var > m_yldreg;

    ///
    Poco::Nullable< Poco::Dynamic::Var > m_windEros;

    ///
    Poco::Nullable< Poco::Dynamic::Var > m_waterEros;

    ///
    Poco::Nullable< Poco::Dynamic::Var > m_sci;

    ///
    Poco::Nullable< Poco::Dynamic::Var > m_sciOM;

    ///
    Poco::Nullable< Poco::Dynamic::Var > m_sciER;

    ///
    Poco::Nullable< Poco::Dynamic::Var > m_sciFO;

    ///
    Poco::Nullable< Poco::Dynamic::Var > m_totBioRem;

    ///
    Poco::Dynamic::Var m_ftapp;

    ///
    Poco::Dynamic::Var m_ftside;

    ///
    std::vector< double > m_prdx;

    ///
    std::string name, county, soil, tillage, removal, rotation, yldreg;

};

} //end cal
} //end daycent
} //end wrapper
} //end leaf
