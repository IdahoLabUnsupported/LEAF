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


// --- LEAF Includes --- //
#include <leaf/wrapper/rusle2/R2ImportNasis.h>
#include <leaf/wrapper/rusle2/RomeWrapper.h>
#include <leaf/wrapper/rusle2/GdbHelper.h>

#include <leaf/open/soi/MapUnit.h>

using namespace leaf::open;
using namespace leaf::util;

// --- Boost Includes --- //
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

// --- POCO Includes --- //
using namespace Poco;
using namespace Poco::Dynamic;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;

// --- STL Includes --- //
#include <sstream>

namespace leaf
{
namespace wrapper
{
namespace rusle2
{

////////////////////////////////////////////////////////////////////////////////
SoilError ReadNASISSoilData(
    soi::ComponentPtr const& component,
    std::string const& filePath )
{
    //
    bool testForMiscComps( true );

    //Get a reference to this component's parent
    soi::MapUnit const& mapunit = component->GetMapUnit();

    std::string sasym = Convert< std::string >( mapunit.GetAreasymbolLegend() );
    std::string saname = Convert< std::string >( mapunit.GetAreanameLegend(), "" );

    //Create the mapunit name
    std::string musym = Convert< std::string >( mapunit.GetMusym() );
    std::string muname = Convert< std::string >( mapunit.GetMuname(), "" );
    std::string mukey = Convert< std::string >( mapunit.GetMukey() );
    std::string muFolderName = musym + " " + muname;
    //Create the component name
    std::string compname = Convert< std::string >( component->GetCompname(), "" );
    std::string cokey = Convert< std::string >( component->GetCokey() );
    std::string localphase = Convert< std::string >( component->GetLocalphase(), "" );
    //Plus the dominance
    std::ostringstream dominanceStr;
    Nullable< Var > const& comppct_r = component->GetComppct();
    if( !( comppct_r.isNull() ) )
    {
        double dominance = Convert< double >( comppct_r, NaN );
        if( dominance != NaN ) dominanceStr << dominance << "%";
    }
    //Construct the full component name
    std::string fullComponentName =
        compname + " " + localphase + dominanceStr.str();

    //Start sorting through the components
    //1) In the component table, look for compkind = "miscellaneous area"
    //   This will be for water, pits, etc, that should be thrown out completely
    std::string compkind = Convert< std::string >( component->GetCompkind(), "" );
    std::transform( compkind.begin(), compkind.end(), compkind.begin(), tolower );
    //Is not a soil at all and should be thrown out
    if( compkind == "miscellaneous area" && testForMiscComps )
    {
        return SOIERR_MISCAREA;
    }

    //Should be looking for some valid horizon
    bool haveGoodHorizon( false );
    soi::CHorizons const& horizons = component->GetHorizons();
    soi::CHorizonsByHzdept::const_iterator chItr =
        horizons.get< HZDEPT >().begin();

    //2) If the soil is a Histosol (taxorder == "Histosol"),
    //   just grab the first horizon and get whatever we can out of that
    std::string taxorder = Convert< std::string >( component->GetTaxorder(), "" );
    std::transform( taxorder.begin(), taxorder.end(), taxorder.begin(), tolower );
    std::string taxsubgrp = Convert< std::string >( component->GetTaxsubgrp(), "" );
    std::transform( taxsubgrp.begin(), taxsubgrp.end(), taxsubgrp.begin(), tolower );
    //An organic soil, so just grab the first horizon,
    //since there will not be any valid one
    if( taxorder == "histosols" ||
        //Need to make sure these are not case insensitive comparisons
        taxsubgrp.find( "histic" ) != std::string::npos )
    {
        //Have a good horizon for this histosol
        if( component->HasHorizons() )
        {
            haveGoodHorizon = true;
        }
        //Didn't find a matching horizon
        else
        {
            return SOIERR_HORIZON;
        }
    }
    //Hopefully just a normal soil
    else
    {
        //Need to look for a good horizon
        //Unfortunately, this information is not available in chorizon
        //We need to use the horizon key to drill down into the chtexturegrp,
        //then the key from there to drill into the ctexture
        //Once there, we search for the top horizon with a layer that is not OM-based
        //For that, we find the chtexturegrp key,
        //use that to find the correct chtexturegrp,
        //from which we get the correct chorizon key,
        //then we use that horizon
        //First get all horizons that have the right component id
        //Didn't find a matching horizon
        if( !component->HasHorizons() )
        {
            return SOIERR_HORIZON;
        }
        else
        {
            //Now have the list of all horizons that match the component, ordered by depth
            //Search through these, looking for the first that has the right texture
            bool found( false );
            for( ; chItr != horizons.get< HZDEPT >().end(); ++chItr )
            {
                soi::CHorizon const& horizon = **chItr;
                if( !horizon.HasTextureGrps() )
                {
                    continue;
                }
                //Now get all the chtexturegrp records that have this chorizon ID; may only be one
                soi::ChTextureGrp const& chtg =
                    **( horizon.GetTextureGrps().get< CHTGKEY >().begin() );
                if( !chtg.HasTextures() )
                {
                    continue;
                }
                //Now get all the chtexture records that have this chtexturegrp ID; may only be one
                soi::ChTexture const& cht =
                    **( chtg.GetTextures().get< CHTKEY >().begin() );
                std::string lieutex = Convert< std::string >( cht.GetLieutex(), "" );
                if( lieutex == "Highly decomposed plant material" ||
                    lieutex == "Moderately decomposed plant material" ||
                    lieutex == "Mucky peat" ||
                    lieutex == "Muck" ||
                    lieutex == "Partially decomposed organic matter" ||
                    lieutex == "Peat" ||
                    lieutex == "Slightly decomposed plant material" ||
                    lieutex == "Undecomposed organic matter" )
                {
                    //Is an organic horizon
                    continue;
                }
                //Should be a horizon we can use
                else
                {
                    //Found a horizon with a mineral texture, so this is the correct mineral chorizon
                    found = true;
                    break;
                }
            }

            //Didn't find a matching horizon
            if( !found )
            {
                return SOIERR_MINERAL;
                //Like a histosol???
                //chItr = horizons.get< HZDEPT >().begin();
                //haveGoodHorizon = true;
            }
            else
            {
                haveGoodHorizon = true;
            }
        }
    }

    //Now can start getting information out of RS_chorizon
    assert( haveGoodHorizon );

    //The first horizon in a depth-ordered list of those with k values should be the one we want the information from
    soi::CHorizon const& horizon = **chItr;

    //Now open the chtexturegrp table to get texture which is needed for the file name
    std::string texture;
    if( horizon.HasTextureGrps() )
    {
        soi::ChTextureGrp const& chtg =
            **( horizon.GetTextureGrps().get< CHTGKEY >().begin() );
        texture = Convert< std::string >( chtg.GetTexdesc(), "" );
    }

    fullComponentName = compname + " " + texture + " " + localphase + dominanceStr.str();

    //The full name of the imported file
    std::string sFullname = "soils\\" + saname + "\\" + muFolderName + "\\" + fullComponentName;

    //Fill a string with the soil data in XML form.
    std::ostringstream sXML;

    //Output file header elements.
    sXML << "<?xml version='1.0'?>\n";
    //sXML << "<!DOCTYPE commands\n[\n";
    //sXML << " <!ENTITY und '&#x5F;'><!-- '_' -->\n";
    //sXML << " <!ENTITY per '&#x25;'><!-- '%' -->\n] >\n";
    sXML << MakeTag( "Obj" );
    sXML << MakeElement( "Type", "SOIL" );
    //This path must be soils\\#NAME when using file:// for db system
    sXML << MakeElement( "Filename", sFullname );
    //Note that should be linked to the SCIENCEVERSION date in rusle.cpp
    sXML << MakeElement( "Science", RomeWrapper::Instance().GetSciVerStr() );

    //Output values from the NASIS component table

    //Note: before 2005-02-24, TYPICAL_LENGTH was incorrectly imported as U_FOOT.
    double slopelenusle_r = Convert< double >( component->GetSlopelenusle(), NaN );
    sXML << AttrXML( "Flt", "TYPICAL_LENGTH", slopelenusle_r, "", "U_METER" );
    double slope_r = Convert< double >( component->GetSlope(), NaN );
    sXML << AttrXML( "Flt", "TYPICAL_STEEPNESS", slope_r, "", "U_PERCENT" );
    double tfact = Convert< double >( component->GetTfact(), NaN );
    sXML << AttrXML( "Flt", "SOIL_T_VALUE", tfact, "", "U_TON_P_AC_YR" );

    //Hydrological class
    Nullable< Var > const& val = component->GetHydgrp();
    std::string sTiledHydrologicClass;
    std::string sHydrologicClass;
    if( !( val.isNull() ) )
    {
        std::string hydgrp = Convert< std::string >( val, "" );

        //Extract undrained and drained hyd groups from possibly concatenated string "A/D"
        std::string undrained, drained;
        if( hydgrp.length() == 3 )
        {
            //If A/D, first category is drained (low runoff), latter category is undrained (high runoff)
            drained = hydgrp[ 0 ];
            undrained = hydgrp[ 2 ];
        }
        else
        {
            undrained = hydgrp[ 0 ];
            //Drained is undefined, so set same as undrained
            drained = undrained;
        }

        sTiledHydrologicClass = Rusle2HydrologicClass( drained );
        sHydrologicClass = Rusle2HydrologicClass( undrained );
    }
    //No NASIS data available
    else
    {
        sTiledHydrologicClass = "NaN";
        sHydrologicClass = "NaN";
    }
    sXML << AttrXML( "Lst", "TILED_HYDROLOGIC_CLASS", sTiledHydrologicClass, "", "" );
    sXML << AttrXML( "Lst", "HYDROLOGIC_CLASS", sHydrologicClass, "", "" );

    // Soil description
    std::string geomdesc = Convert< std::string >( component->GetGeomdesc(), "" );
    sXML << AttrXML( "Str", "SOIL_DESCRIP", geomdesc, "", "" );

    //Get more values from the chorizon table
    //Need to set the NASIS organic matter value into two places
    //The first is just a place-holder,
    //while the second is the value actually used in the calculations
    double om_r = Convert< double >( horizon.GetOm(), NaN );
    sXML << AttrXML( "Flt", "NASIS_OM_REP_HOR_1", om_r,  "", "U_PERCENT" );
    sXML << AttrXML( "Flt", "ORGANIC_MATTER", om_r, "", "U_PERCENT" );
    double ph1to1h2o_r = Convert< double >( horizon.GetPh1to1h2o(), NaN );
    sXML << AttrXML( "Flt", "NASIS_PH_1TO1_H20_REP_HOR_1", ph1to1h2o_r, "", "U_PH_UNITS" );

    //For Histosols, set the erodibility = 0.02
    if( taxorder == "histosols" ||
        taxsubgrp.find( "histic" ) != std::string::npos )
    {
        sXML << AttrXML( "Flt", "ERODIBILITY", "0.02", "", "U_ENGLISH_EROD" );
        sXML << AttrXML( "Flt", "ERODIBILITY_HAND", "0.02", "", "U_ENGLISH_EROD" );
        sXML << AttrXML( "SbR", "ERODIBILITY_OPTION_PTR", "ERODIBILITY_OPTION_SET_BY_USER", "", "" );
    }
    else
    {
        //User-entry ERODIBILITY values are now stored in ERODIBILITY_HAND instead
        //We also set ERODIBILITY_OPTION_PTR to "ERODIBILITY_OPTION_SET_BY_USER", although that is the default value
        //We still set ERODIBILITY directly, even though it is now calculated, because we are faking the science date
        double kffact = Convert< double >( horizon.GetKffact(), NaN );
        //Bad erod value for this selected horizon
        if( isnan( kffact ) )
        {

            sXML << AttrXML( "Flt", "ERODIBILITY", "0.0", "", "U_ENGLISH_EROD" );
            sXML << AttrXML( "Flt", "ERODIBILITY_HAND", "0.0", "", "U_ENGLISH_EROD" );
            sXML << AttrXML( "SbR", "ERODIBILITY_OPTION_PTR", "ERODIBILITY_OPTION_SET_BY_USER", "", "" );
        }
        //Good kffact value
        else
        {
            sXML << AttrXML( "Flt", "ERODIBILITY", kffact, "", "U_ENGLISH_EROD" );
            sXML << AttrXML( "Flt", "ERODIBILITY_HAND", kffact, "", "U_ENGLISH_EROD" );
            sXML << AttrXML( "SbR", "ERODIBILITY_OPTION_PTR", "ERODIBILITY_OPTION_SET_BY_USER", "", "" );
        }
    }

    //Histosols back into this at Lightle's request (051026) because wanted to be able to open these soils for SCI calculations
    if( taxorder == "Histosols" ||
        taxsubgrp.find( "Histic" ) != std::string::npos )
    {
        //Have already put in a bogus erodibility = 0.02 for these, so might as well give them bogus texture!
        sXML << AttrXML( "Flt", "SAND", "0.0", "", "U_PERCENT" );
        sXML << AttrXML( "Flt", "SILT", "0.0", "", "U_PERCENT" );
        sXML << AttrXML( "Flt", "CLAY", "100.0", "", "U_PERCENT" );
    }
    //Real soil
    else
    {
        //Need to check NaN (Not a Number)
        bool changedVals( false );
        double sandtotal_r= Convert< double >( horizon.GetSandtotal(), NaN );
        if( isnan( sandtotal_r ) ) changedVals = true;
        double silttotal_r = Convert< double >( horizon.GetSilttotal(), NaN );
        if( isnan( silttotal_r ) ) changedVals = true;
        double claytotal_r = Convert< double >( horizon.GetClaytotal(), NaN );
        if( isnan( claytotal_r ) ) changedVals = true;

        //Check now whether add up OK
        if( !changedVals )
        {
            //Something wrong
            if( fabs( sandtotal_r + silttotal_r + claytotal_r - 100.0 ) > 10.0/*0.1*/ )
            {
                changedVals = true;
            }
        }
        //Something changed, so warn of that and store nonsense values (unless is Histosol)
        if( changedVals )
        {
            sXML << AttrXML( "Flt", "SAND", "0.0", "", "U_PERCENT" );
            sXML << AttrXML( "Flt", "SILT", "0.0", "", "U_PERCENT" );
            sXML << AttrXML( "Flt", "CLAY", "0.0", "", "U_PERCENT" );

            return SOIERR_SANDSILTCLAY;
        }
        //Everything OK, so proceed normally
        else
        {
            sXML << AttrXML( "Flt", "SAND", sandtotal_r, "", "U_PERCENT" );
            sXML << AttrXML( "Flt", "SILT", silttotal_r, "", "U_PERCENT" );
            sXML << AttrXML( "Flt", "CLAY", claytotal_r, "", "U_PERCENT" );
        }
    }
    //Add NASIS parameters to identify this soil.
    sXML << AttrXML( "Str", "NASIS_MUSYM", musym, "1", "" );
    sXML << AttrXML( "Str", "NASIS_SASYM", sasym, "1", "" );

    //Add new NASIS parameters desired by Dabney 2010-04-17
    double cec7_r = Convert< double >( horizon.GetCec7(), NaN );
    sXML << AttrXML( "Flt", "NASIS_CEC_7", cec7_r, "", "U_MEQ_P_100_ML" );
    double wthirdbar_r = Convert< double >( horizon.GetWthirdbar(), NaN );
    sXML << AttrXML( "Flt", "NASIS_WATER_CONTENT_THIRD_BAR", wthirdbar_r, "", "U_PERCENT" );
    double wfifteenbar_r = Convert< double >( horizon.GetWfifteenbar(), NaN );
    sXML << AttrXML( "Flt", "NASIS_WATER_CONTENT_15_BAR", wfifteenbar_r, "", "U_PERCENT" );
    double dbthirdbar_r = Convert< double >( horizon.GetDbthirdbar(), NaN );
    sXML << AttrXML( "Flt", "NASIS_BULK_DENSITY_THIRD_BAR", dbthirdbar_r, "", "U_G_P_CM3" );

    sXML << MakeEnd( "Obj" );

    //Write the file from the XML string
    //std::ios::binary is important to keep file format correct
    std::ofstream ofile( filePath.c_str(),
        std::ios::out | std::ios::trunc | std::ios::binary );
    assert( ofile.is_open() );
    ofile << sXML.str();
    ofile.close();
    assert( fs::exists( filePath ) && fs::file_size( filePath ) );

    return SOIERR_NONE;
}
////////////////////////////////////////////////////////////////////////////////
std::string Rusle2HydrologicClass(
    std::string const& hydrologicClass )
{
    switch( *( hydrologicClass.c_str() ) )
    {
        case 'A':
            return "HYDROLOGIC_CLASS_LOW_RO";
        case 'B':
            return "HYDROLOGIC_CLASS_MOD_LOW_RO";
        case 'C':
            return "HYDROLOGIC_CLASS_MOD_HIGH_RO";
        case 'D':
            return "HYDROLOGIC_CLASS_HIGH_RO";
        default:
            return "HYDROLOGIC_CLASS_HIGH_RO";
    }
}
////////////////////////////////////////////////////////////////////////////////
std::string MakeElement(
    std::string const& pszName,
    std::string const& pszText )
{
    assert( !pszName.empty() && !pszText.empty() );
    assert( pszName == EncodeXML( pszName ) );

    std::ostringstream s;
    s << "<"  << pszName << ">"
      << ( IsEncodedXML( pszText ) ? pszText : EncodeXML( pszText ) )
      << "</" << pszName << ">\n";

    return s.str();
}
////////////////////////////////////////////////////////////////////////////////
std::string MakeTag(
    std::string const& pszName )
{
    std::ostringstream s;
    s << "<" << pszName << ">";

    return s.str();
}
////////////////////////////////////////////////////////////////////////////////
std::string MakeEnd(
    std::string const& pszName )
{
    std::ostringstream s;
    s << "</" << pszName << ">\n";

    return s.str();
}
////////////////////////////////////////////////////////////////////////////////
std::string AttrXML(
    std::string const& pszTag,
    std::string const& pszRusle2Name,
    Poco::Dynamic::Var const& pszValue,
    std::string const& pszDims,
    std::string const& pszUnit )
{
    assert( !( pszValue.isEmpty() ) );

    std::ostringstream sXML;

    sXML << MakeTag( pszTag );

    sXML << MakeElement( "Name", pszRusle2Name );

    if( !pszDims.empty() )
    {
        sXML << MakeElement( "Dims", pszDims );
    }

    //Output the data element for a single value
    if( ( pszValue.isNumeric() && isnan( pszValue ) ) || pszValue == "" )
    {
        sXML << MakeElement( "Data", "NaN" );
    }
    else
    {
        sXML << MakeElement( "Data", EncodeXML( pszValue ) );
    }

    if( !pszUnit.empty() )
    {
        sXML << MakeElement( "Unit", pszUnit );
    }

    sXML << MakeEnd( pszTag );

    return sXML.str();
}
////////////////////////////////////////////////////////////////////////////////

} //end rusle2
} //end wrapper
} //end leaf

