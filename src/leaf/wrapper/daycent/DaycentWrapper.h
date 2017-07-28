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

#include <leaf/wrapper/daycent/soil/Soils.h>

// --- Boost Includes --- //
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

// --- STL Includes --- //
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

namespace leaf
{
namespace wrapper
{
namespace daycent
{

class DaycentWrapper;
typedef boost::shared_ptr< DaycentWrapper > DaycentWrapperPtr;

typedef std::vector< std::vector< std::vector< std::string > > > SCHEDULE;

class DaycentWrapper : public boost::noncopyable
{
public:
    ///
    static DaycentWrapper* instance(
        bool const& erase = false );

    ///
    void Initialize();

    ///
    void Run(
        std::string const& dir,
        std::string const& abbr,
        std::string const& cokey );

    ///
    void RunSpinups( 
        std::string const& dir,
        std::string const& abbr,
        std::string const& cokey );

    ///
    void CreateWeatherFile(
        std::string const& abbr,
        std::string const& wthDC,
        std::string const& cliPath );

    ///
    void UpdateSiteFile(
        std::string const& abbr,
        std::string const& filePath,
        std::string const& orgsitefile);

    ///
    void CreateMgntFiles(
        std::string const& wepsdir,
        std::string const& fileDir,
        std::string const& abbr,
        const int simyears,
        bool const& sim = false );

    ///
    void CopyFileDir(
        std::string const& filePath,
        std::string const& fileDir,
        bool const& overwrite = false );

    ///
    void GetSpinupFiles(
        std::string const& filePath,
        std::string const& fileDir,
        std::string const& abbr );

    ///
    void GetSpinupFiles(
        std::string const& abbr );

    ///
    void SetWindEros( double windEros );

    ///
    void SetWaterEros( double waterEros );

    ///
    double GetWindEros();

    ///
    double GetWaterEros();

    ///
    void SetRemovalRate(
        std::string const& removalRate );

    ///
    void SetRotation(
        std::string const& rotation );

    ///
    void SetTillage(
        std::string const& tillage );

    ///
    void SetNLayer(
        int const& layer );

    ///
    void SetSilt(
        double const& silt );

    ///
    void SetSand(
        double const& sand );

    ///
    void SetClay(
        double const& clay );

    ///
    void SetBulkDensity(
        double const& bulkden );

    ///
    void SetInputYield(
        std::vector< double > yield );

    ///
    void SetCMZ(
        std::string  const& cmz );
    std::string const& GetCMZ();

    ///
    void SetField(
        std::string field );

    ///
    void SetFert(
              std::string const& abbr,
              std::string const& rot,
              std::string const& fert,
              std::string const& path );

    ///
    void RemoveAllFilesWithExtension(
        std::string const& ext);

    ///
    SCHEDULE GetSchedule();
    void SetSchedule(
        SCHEDULE schedule );

    ///
    void BuildSchFile(
        SCHEDULE schedule,
        std::string const& abbr,
        const int simyr,
        bool run = false );

    ///
    //std::vector< std::string > GetCropRotation();
    //void SetCropRotation(
    //    std::vector< std::string > rot );

    ///
    std::string m_curDir;

    ///
    std::string m_spinupFile;

    ///
    std::string m_killFile;

    ///
    std::string m_baseFile;

    ///
    std::string m_cmzFile;

    ///
    std::string m_projFile;

    ///
    double m_windEros;

    ///
    double m_waterEros;

    ///
    unsigned int m_nlayer;

    ///
    double m_sand;

    ///
    double m_silt;

    ///
    double m_clay;

    ///
    double m_bulkden;

    ///
    std::vector< double > m_yield;

    ///
    unsigned int m_simyr;

    ///
    std::string m_removalRate;

    ///
    std::string m_rotation;

    ///
    std::string m_tillage;

    ///
    //std::vector< std::string > m_rot;

    ///
    std::string m_field;

    ///
    std::string m_cmz;

    ///
    SCHEDULE m_schedule;

protected:
    ///
    DaycentWrapper();

    ///
    ~DaycentWrapper();

private:

    ///
    std::string m_latitude;

    ///
    std::string m_longitude;
};

} //end daycent
} //end wrapper
} //end leaf