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


#ifndef MAN_PARSER_H
#define MAN_PARSER_H

// --- LEAF Includes --- //

// --- STL Include --- //
#include <string>
#include <vector>
#include <map>
#include <set>

namespace leaf
{
namespace wrapper
{
namespace daycent
{
namespace man
{

class ManParser
{
public:
    ///
    ManParser(
        std::string const& filePath,
        std::string const& fips,
        const int rotYears );

    ///
    ~ManParser(void);

    ///
    static std::set< std::string > const crops;

    ///
    void BuildSchFile( 
        std::vector< std::vector< std::vector< std::string > > > schedule,
        const int simYears,
        bool const& sim = false );

    ///
    std::vector< std::vector< std::string > > MergeErosionOperations(
        std::vector< std::vector< std::string > > mngt,
        std::vector< std::vector< std::string > > ero );

    ///
    void SetFertParams( std::string const& filePath );

    ///
    std::vector< std::vector< std::pair< std::string, std::string > > > m_erosion;

    ///
    std::vector< std::vector< std::pair< std::string, std::string > > > GetAnnualErosion();

    ///
    std::vector< std::vector< std::vector< std::string > > > AssignOperationType(
        std::string const& tillreg,
        std::vector< std::vector< std::pair< std::string, std::string > > > mgnt );

    ///
    //bool GrowingDegreeDay( int gdd );

    ///
    bool GrowingDegreeDay( const std::string& crop );

    ///
    bool PerennialCrop( const std::string& crop );

    ///
    void SetRemovalRate( 
        std::string const& removalRate );

    ///
    std::string GetCrop( std::vector< std::vector< std::string > > op );
    void SetCrop( 
        std::string const& crop );

    ///
    std::vector< std::string > GetCropAbbrev();

    ///
    std::string m_removalRate;

    ///
    std::vector< std::string > m_rot;

private:
    ///

    std::map< std::string, int > m_crop;
    ///
    std::string m_path;

    ///
    std::string m_abbr;

    ///
    int m_rotYears;

    ///
    bool m_GDD;
};

} //end man
} //end daycent
} //end wrapper
} //end leaf

#endif