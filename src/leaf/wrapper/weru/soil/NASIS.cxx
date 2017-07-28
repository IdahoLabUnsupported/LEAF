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


// --- LEAF Includes --- //
#include <leaf/wrapper/weru/soil/NASIS.h>

#include <leaf/wrapper/weru/util/ConfigData.h>

#include <leaf/wrapper/java/lang/Double.h>

#include <leaf/wrapper/java/util/Array.h>

using namespace leaf::open;
using namespace leaf::util;
using namespace leaf::wrapper::java::lang;
using namespace leaf::wrapper::java::util;

// --- POCO Includes --- //
using namespace Poco;
using namespace Poco::Dynamic;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;

// --- Boost Includes --- //
#include <boost/regex.hpp>

#include <boost/algorithm/string.hpp>

namespace leaf
{
namespace wrapper
{
namespace weru
{
namespace soil
{

////////////////////////////////////////////////////////////////////////////////
std::set< std::string > const NASIS::ORGANIC_TEXTURES = boost::assign::list_of
    ( "mpm" ) ( "mpt" ) ( "muck" ) ( "peat" )
    ( "spm" ) ( "udom" ) ( "pdom" ) ( "hpm" );
////////////////////////////////////////////////////////////////////////////////
NASIS::NASIS(
    jobject const& o )
    :
    java::lang::Object( o )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
NASIS::NASIS(
    std::size_t const& inpnumlay )
    :
    java::lang::Object(
        java::NewObject( GetJclass(), GetJmid( "<init>" ), inpnumlay ) )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
NASIS::~NASIS()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
SoilError NASIS::LoadSQL(
    soi::ComponentPtr const& component )
{
    std::string cokey = Convert< std::string >( component->GetCokey() );
    Nullable< Var > taxorder = CopyComponent( component );

    //Can test taxorder now to see if organic
    if( util::ConfigData::GetSoilTestOrganic() )
    {
        if( boost::algorithm::to_lower_copy(
                Convert< std::string >( taxorder ) ) == "histosols" )
        {
            return SOIERR_ORGANICTEST;
        }
    }

    //Get a reference to this component's parent
    soi::MapUnit const& mapunit = component->GetMapUnit();

    CopyMapUnit( mapunit );

    CopyMuAggatt( mapunit );

    CopyCoRestrictions( component );

    CopyLegend( mapunit );

    int layerNum( 0 );
    int i( 0 );

    bool foundAMineralLayer( false );

    //Up to this amount is allowed to be ignored
    double maxOrganicDepth( util::ConfigData::GetSoilMaxOrganicDepth() );

    //As we loop over the layers, add up how much organic layer has been ignored
    double usedOrganicDepth( 0 );
    bool isSkpOrgSurLays = util::ConfigData::IsSkipOrganicSoilSurfaceLayers();
    //Order by depth to make sure layers are in the correct order
    soi::CHorizons const& horizons = component->GetHorizons();
    soi::CHorizonsByHzdept::const_iterator chItr =
        horizons.get< HZDEPT >().begin();
    for( ; chItr != horizons.get< HZDEPT >().end(); ++chItr )
    {
        soi::CHorizon const& horizon = **chItr;
        soi::ChTextureGrps const& chtgrps = horizon.GetTextureGrps();
        soi::ChTextureGrpPtr chtgrp = horizon.HasTextureGrps() ?
            *( chtgrps.get< CHTGKEY >().begin() ) : soi::ChTextureGrpPtr();
        if( isSkpOrgSurLays && !foundAMineralLayer &&
            usedOrganicDepth <= maxOrganicDepth )
        {
            std::string hzname = Convert< std::string >( horizon.GetHzname() );
            std::string desgnmaster =
                Convert< std::string >( horizon.GetDesgnmaster() );
            double om = Convert< double >( horizon.GetOm(), Double::NaN() );
            //Convert from percent to fraction
            om /= 100.0;
            std::string text = ( chtgrp != soi::ChTextureGrpPtr() ) ?
                Convert< std::string >( chtgrp->GetTexture() ) : "";
            bool organic = IsLayerOrganic( hzname, desgnmaster, text, om );
            if( organic )
            {
                //Data mart is in cm not mm
                double thicknessMiliMeter = Convert< double >(
                    horizon.GetHzthk(),
                    Convert< double >(
                        horizon.GetHzdepb(), Double::NaN() ) ) * 10.0;
                usedOrganicDepth += thicknessMiliMeter;
                if( usedOrganicDepth <= maxOrganicDepth )
                {
                    //Still allowed to ignore
                    continue;
                }
                else
                {
                    if( i == 0 )
                    {
                        return SOIERR_ORGLAYEXCIGNDEP;
                    }
                    else
                    {
                        return SOIERR_SOILAYBELIGNORGLAYISORG;
                    }
                }
            }
            else
            {
                //Not organic, so we're good to go
                foundAMineralLayer = true;
            }
        }

        CopyHorizon( horizon, layerNum );

        soi::ChFrags const& frags = horizon.GetFrags();
        CopyFrags( frags, layerNum );

        CopyTexture( chtgrp, layerNum );

        ++layerNum;
        ++i;
    }

    if( layerNum <= 0 )
    {
        return SOIERR_HORIZON;
    }

    java::SetField< jint >( m_jobject, GetJfid( "numlay" ), layerNum );

    if( util::ConfigData::IsAverageStratifiedSoilLayers() )
    {
        java::CallMethod< void >(
            m_jobject, GetJmid( "averageStratifiedLayers" ) );
    }

    java::CallMethod< void >( m_jobject, GetJmid( "fixUpData" ) );

    try
    {
        java::CallMethod< void >( m_jobject, GetJmid( "testData" ) );
    }
    catch( std::exception const& e )
    {
        //This exception will occur when estimating a value for lep,
        //but either clay or sand values are NaN
        static boost::regex const rgx(
            "java\\.lang\\.ArrayIndexOutOfBoundsException: \\d+\n"
            "\\s+usda\\.weru\\.soil\\.SoilUtil\\.estimateMineralBulkDensity\\(SoilUtil\\.java:\\d+\\)\n"
            "\\s+usda\\.weru\\.soil\\.SoilUtil\\.estimateSettledBulkDensity\\(SoilUtil\\.java:\\d+\\)\n"
            "\\s+usda\\.weru\\.soil\\.NASIS\\.testData\\(NASIS\\.java:\\d+\\)",
            boost::regex::perl );
        if( boost::regex_search( e.what(), rgx ) ) return SOIERR_NAN;

        throw;
    }

    return SOIERR_NONE;
}
////////////////////////////////////////////////////////////////////////////////
Nullable< Var > NASIS::CopyComponent(
    soi::ComponentPtr const& component )
{
    //compname
    Nullable< Var > const& compname = component->GetCompname();
    java::SetField< jobject >(
        m_jobject, GetJfid( "compname" ), String( compname ).Jobj() );

    //comppct
    Nullable< Var > const& comppct = component->GetComppct_r();
    java::SetField< jobject >(
        m_jobject, GetJfid( "comppct" ), String( comppct ).Jobj() );

    //taxorder
    Nullable< Var > const& taxorder = component->GetTaxorder();
    java::SetField< jobject >(
        m_jobject, GetJfid( "taxorder" ), String( taxorder ).Jobj() );

    //localphase
    std::string localphase =
        Convert< std::string >( component->GetLocalphase(), "" );
    java::SetField< jobject >(
        m_jobject, GetJfid( "localphase" ), String( localphase ).Jobj() );

    //tfact - losstolerance
    std::string temp = Convert< std::string >( component->GetTfact(), "0" );
    boost::trim( temp );
    if( temp.length() == 0 ) temp = "0";
    double tfact = boost::lexical_cast< int >( temp );
    java::SetField< jdouble >( m_jobject, GetJfid( "tfact" ), tfact );

    //slope
    double slope = Convert< double >( component->GetSlope(), Double::NaN() );
    java::SetField< jdouble >( m_jobject, GetJfid( "slope" ), slope );

    //albedodry
    double albedodry =
        Convert< double >( component->GetAlbedodry(), Double::NaN() );
    java::SetField< jdouble >( m_jobject, GetJfid( "albedodry" ), albedodry );

    return taxorder;
}
////////////////////////////////////////////////////////////////////////////////
void NASIS::CopyMapUnit(
    soi::MapUnit const& mapunit )
{
    //musym
    Var const& musym = mapunit.GetMusym();
    java::SetField< jobject >( m_jobject, GetJfid( "musym" ),
        String( musym ).Jobj() );
}
////////////////////////////////////////////////////////////////////////////////
void NASIS::CopyMuAggatt(
    soi::MapUnit const& mapunit )
{
    //brockdepmin
    double brockdepmin =
        Convert< double >( mapunit.GetBrockdepmin(), Double::NaN() );
    java::SetField< jdouble >(
        m_jobject, GetJfid( "brockdepmin" ), brockdepmin );
}
////////////////////////////////////////////////////////////////////////////////
void NASIS::CopyCoRestrictions(
    soi::ComponentPtr const& component )
{
    //resdeptmin
    double resdeptmin =
        Convert< double >( component->GetMinResDept(), Double::NaN() );
    java::SetField< jdouble >( m_jobject, GetJfid( "resdeptmin" ), resdeptmin );
}
////////////////////////////////////////////////////////////////////////////////
void NASIS::CopyLegend(
    soi::MapUnit const& mapunit )
{
    //areaname - ssaname
    Nullable< Var > const& ssaname = mapunit.GetAreanameLegend();
    java::SetField< jobject >(
        m_jobject, GetJfid( "areaname" ), String( ssaname ).Jobj() );

    //areasymbol - ssaid
    Nullable< Var > const& ssaid = mapunit.GetAreasymbolLegend();
    java::SetField< jobject >(
        m_jobject, GetJfid( "areasymbol" ), String( ssaid ).Jobj() );

    // --- Skip cordate, ssurgoarchived, tabularversion logging --- //

    Nullable< Var > state, county;
    std::vector< std::string > parts;
    std::string str = Convert< std::string >( ssaname );
    boost::split( parts, str, boost::is_any_of( "," ) );
    if( parts.size() == 0 )
    {
        county = "";
    }
    else if( parts.size() == 1 )
    {
        boost::trim( parts[ 0 ] );
        state = parts[ 0 ];
        county = "";
    }
    else
    {
        boost::trim( parts[ 0 ] );
        county = parts[ 0 ];
        boost::trim( parts[ 1 ] );
        state = parts[ 1 ];
    }

    //state
    java::SetField< jobject >(
        m_jobject, GetJfid( "state" ), String( state ).Jobj() );

    //county
    java::SetField< jobject >(
        m_jobject, GetJfid( "county" ), String( county ).Jobj() );
}
////////////////////////////////////////////////////////////////////////////////
void NASIS::CopyHorizon(
    soi::CHorizon const& horizon,
    unsigned int const& idx )
{
    //hzdept
    double hzdept = Convert< double >( horizon.GetHzdept(), Double::NaN() );
    Array< double > hzdept_arr(
        java::GetField< jobject >( m_jobject, GetJfid( "hzdept" ) ) );
    hzdept_arr.SetElement( idx, hzdept );

    //hzdepb
    double hzdepb = Convert< double >( horizon.GetHzdepb(), Double::NaN() );
    Array< double > hzdepb_arr(
        java::GetField< jobject >( m_jobject, GetJfid( "hzdepb" ) ) );
    hzdepb_arr.SetElement( idx, hzdepb );

    //claytotal
    double claytotal =
        Convert< double >( horizon.GetClaytotal(), Double::NaN() );
    Array< double > claytotal_arr(
        java::GetField< jobject >( m_jobject, GetJfid( "claytotal" ) ) );
    claytotal_arr.SetElement( idx, claytotal );

    //sandtotal
    double sandtotal =
        Convert< double >( horizon.GetSandtotal(), Double::NaN() );
    Array< double > sandtotal_arr(
        java::GetField< jobject >( m_jobject, GetJfid( "sandtotal" ) ) );
    sandtotal_arr.SetElement( idx, sandtotal );

    //silttotal
    double silttotal =
        Convert< double >( horizon.GetSilttotal(), Double::NaN() );
    Array< double > silttotal_arr(
        java::GetField< jobject >( m_jobject, GetJfid( "silttotal" ) ) );
    silttotal_arr.SetElement( idx, silttotal );

    //sandvc
    double sandvc = Convert< double >( horizon.GetSandvc(), 0 );
    Array< double > sandvc_arr(
        java::GetField< jobject >( m_jobject, GetJfid( "sandvc" ) ) );
    sandvc_arr.SetElement( idx, sandvc );

    //sandco
    double sandco = Convert< double >( horizon.GetSandco(), 0 );
    Array< double > sandco_arr(
        java::GetField< jobject >( m_jobject, GetJfid( "sandco" ) ) );
    sandco_arr.SetElement( idx, sandco );

    //sandmed
    double sandmed = Convert< double >( horizon.GetSandmed(), 0 );
    Array< double > sandmed_arr(
        java::GetField< jobject >( m_jobject, GetJfid( "sandmed" ) ) );
    sandmed_arr.SetElement( idx, sandmed );

    //sandfine
    double sandfine = Convert< double >( horizon.GetSandfine(), 0 );
    Array< double > sandfine_arr(
        java::GetField< jobject >( m_jobject, GetJfid( "sandfine" ) ) );
    sandfine_arr.SetElement( idx, sandfine );

    //sandvf
    double sandvf = Convert< double >( horizon.GetSandvf(), Double::NaN() );
    Array< double > sandvf_arr(
        java::GetField< jobject >( m_jobject, GetJfid( "sandvf" ) ) );
    sandvf_arr.SetElement( idx, sandvf );

    //dbthirdbar
    double dbthirdbar =
        Convert< double >( horizon.GetDbthirdbar(), Double::NaN() );
    Array< double > dbthirdbar_arr(
        java::GetField< jobject >( m_jobject, GetJfid( "dbthirdbar" ) ) );
    dbthirdbar_arr.SetElement( idx, dbthirdbar );

    //dbovendry
    //double dbovendry =
        //Convert< double >( horizon.Dbovendry(), Double::NaN() );
    //Array< double > dbovendry_arr(
        //java::GetField< jobject >( m_jobject, GetJfid( "dbovendry" ) ) );
    //dbovendry_arr.SetElement( idx, dbovendry );

    //wtenthbar
    //double wtenthbar =
        //Convert< double >( horizon.GetWtenthbar(), Double::NaN() );
    double wtenthbar = -9.9;
    Array< double > wtenthbar_arr(
        java::GetField< jobject >( m_jobject, GetJfid( "wtenthbar" ) ) );
    wtenthbar_arr.SetElement( idx, wtenthbar );

    //wthirdbar
    double wthirdbar =
        Convert< double >( horizon.GetWthirdbar(), Double::NaN() );
    Array< double > wthirdbar_arr(
        java::GetField< jobject >( m_jobject, GetJfid( "wthirdbar" ) ) );
    wthirdbar_arr.SetElement( idx, wthirdbar );

    //wfifteenbar
    double wfifteenbar =
        Convert< double >( horizon.GetWfifteenbar(), Double::NaN() );
    Array< double > wfifteenbar_arr(
        java::GetField< jobject >( m_jobject, GetJfid( "wfifteenbar" ) ) );
    wfifteenbar_arr.SetElement( idx, wfifteenbar );

    //ksat
    double ksat = Convert< double >( horizon.GetKsat(), Double::NaN() );
    Array< double > ksat_arr(
        java::GetField< jobject >( m_jobject, GetJfid( "ksat" ) ) );
    ksat_arr.SetElement( idx, ksat );

    //cec7
    double cec7 = Convert< double >( horizon.GetCec7(), Double::NaN() );
    Array< double > cec7_arr(
        java::GetField< jobject >( m_jobject, GetJfid( "cec7" ) ) );
    cec7_arr.SetElement( idx, cec7 );

    //ecec
    double ecec = Convert< double >( horizon.GetEcec(), Double::NaN() );
    Array< double > ecec_arr(
        java::GetField< jobject >( m_jobject, GetJfid( "ecec" ) ) );
    ecec_arr.SetElement( idx, ecec );

    //I think my error checking will work - it compiles :-)
    //However, my test case error is not because both fields are null,
    //but that one is null for one layer and the other is null for another
    //and the NASIS(SSURGO) to IFC code can't handle that correctly (yet) - LEW
    //if( ( cec7[ idx ] < 0.0 ) && ( ecec[ idx ] < 0.0 ) )
        //System.err.println(
            //"NASIS_copyHorizon: cec7 or ecec fields not populated " );
    //if( ( cec7[ idx ] < 0.0 ) && ( ecec[ idx ] < 0.0 ) )
        //throw new FieldNotPopulated( "cec7 and ecec" );

    //om
    double om = Convert< double >( horizon.GetOm(), Double::NaN() );
    Array< double > om_arr(
        java::GetField< jobject >( m_jobject, GetJfid( "om" ) ) );
    om_arr.SetElement( idx, om );

    //caco3
    double caco3 = Convert< double >( horizon.GetCaco3(), Double::NaN() );
    Array< double > caco3_arr(
        java::GetField< jobject >( m_jobject, GetJfid( "caco3" ) ) );
    caco3_arr.SetElement( idx, caco3 );

    //ph1to1h2o
    double ph1to1h2o =
        Convert< double >( horizon.GetPh1to1h2o(), Double::NaN() );
    Array< double > ph1to1h2o_arr(
        java::GetField< jobject >( m_jobject, GetJfid( "ph1to1h2o" ) ) );
    ph1to1h2o_arr.SetElement( idx, ph1to1h2o );

    //ph01mcacl2
    double ph01mcacl2 =
        Convert< double >( horizon.GetPh01mcacl2(), Double::NaN() );
    Array< double > ph01mcacl2_arr(
        java::GetField< jobject >( m_jobject, GetJfid( "ph01mcacl2" ) ) );
    ph01mcacl2_arr.SetElement( idx, ph01mcacl2 );

    //lep
    double lep = Convert< double >( horizon.GetLep(), Double::NaN() );
    Array< double > lep_arr(
        java::GetField< jobject >( m_jobject, GetJfid( "lep" ) ) );
    lep_arr.SetElement( idx, lep );
}
////////////////////////////////////////////////////////////////////////////////
void NASIS::CopyFrags(
    soi::ChFrags const& frags,
    unsigned int const& idx )
{
    double fragvol( 0.0 );

    soi::ChFragsByChfragskey::const_iterator chfItr =
        frags.get< CHFRAGSKEY >().begin();
    for( ; chfItr != frags.get< CHFRAGSKEY >().end(); ++chfItr )
    {
        soi::ChFrag const& frag = **chfItr;
        double tempFragVol =
            Convert< double >( frag.GetFragvol(), Double::NaN() );
        if( tempFragVol != Double::NaN() ) fragvol += tempFragVol;
    }

    //fragvol
    Array< double > fragvol_arr(
        java::GetField< jobject >( m_jobject, GetJfid( "fragvol" ) ) );
    fragvol_arr.SetElement( idx, fragvol );
}
////////////////////////////////////////////////////////////////////////////////
void NASIS::CopyTexture(
    soi::ChTextureGrpPtr const& chtgrp,
    unsigned int const& idx )
{
    bool exists = ( chtgrp != soi::ChTextureGrpPtr() );

    //texture
    std::string temp = exists ?
        Convert< std::string >( chtgrp->GetTexture() ) : "";
    boost::algorithm::replace_all( temp, "-", "_" );
    Array< String > texture_arr(
        java::GetField< jobject >( m_jobject, GetJfid( "texture" ) ) );
    texture_arr.SetElement( idx, temp );

    bool bVal( false );

    std::string stratifiedString = exists ?
        Convert< std::string >( chtgrp->GetStratextsflag() ) : "";
    boost::trim( stratifiedString );
    boost::algorithm::to_lower( stratifiedString );
    if( stratifiedString == "yes" )
    {
        bVal = true;
    }
    else
    {
        bVal = false;
    }

    //stratextsflag
    Array< bool > stratextsflag_arr(
        java::GetField< jobject >( m_jobject, GetJfid( "stratextsflag" ) ) );
    stratextsflag_arr.SetElement( idx, bVal );

    std::string rvString = exists ?
        Convert< std::string >( chtgrp->GetRvindicator() ) : "";
    boost::trim( rvString );
    boost::algorithm::to_lower( rvString );
    if( rvString == "yes" )
    {
        bVal = true;
    }
    else
    {
        bVal = false;
    }

    //rvindicator
    Array< bool > rvindicator_arr(
        java::GetField< jobject >( m_jobject, GetJfid( "rvindicator" ) ) );
    rvindicator_arr.SetElement( idx, bVal );
}
////////////////////////////////////////////////////////////////////////////////
jclass const& NASIS::GetJclass()
{
    static jclass const clazz = java::FindClass( "usda/weru/soil/NASIS" );
    return clazz;
}
////////////////////////////////////////////////////////////////////////////////
jmethodID const& NASIS::GetJmid(
    std::string const& name )
{
    static java::JMIDMAP const jmidMap = boost::assign::map_list_of
        ( "<init>",
          java::GetMethodID(
              GetJclass(), "<init>",
              "(I)V" ) )
        ( "averageStratifiedLayers",
          java::GetMethodID(
              GetJclass(), "averageStratifiedLayers",
              "()V" ) )
        ( "fixUpData",
          java::GetMethodID(
              GetJclass(), "fixUpData",
              "()V" ) )
        ( "testData",
          java::GetMethodID(
              GetJclass(), "testData",
              "()V" ) );

    java::JMIDMAP::const_iterator itr = jmidMap.find( name );
    return itr->second;
}
////////////////////////////////////////////////////////////////////////////////
jfieldID const& NASIS::GetJfid(
    std::string const& name )
{
    static java::JFIDMAP const jfidMap = boost::assign::map_list_of
        //spatial
        ( "state",
          java::GetFieldID(
              GetJclass(), "state",
              "Ljava/lang/String;" ) )
        ( "county",
          java::GetFieldID(
              GetJclass(), "county",
              "Ljava/lang/String;" ) )
        //legend
        ( "areaname",
          java::GetFieldID(
              GetJclass(), "ssaname",
              "Ljava/lang/String;" ) )
        ( "areasymbol",
          java::GetFieldID(
              GetJclass(), "ssaid",
              "Ljava/lang/String;" ) )
        //mapunit
        ( "musym",
          java::GetFieldID(
              GetJclass(), "musym",
              "Ljava/lang/String;" ) )
        //muaggatt
        ( "brockdepmin",
          java::GetFieldID(
              GetJclass(), "bedrockDepth",
              "D" ) )
        //component
        ( "compname",
          java::GetFieldID(
              GetJclass(), "compname",
              "Ljava/lang/String;" ) )
        ( "comppct",
          java::GetFieldID(
              GetJclass(), "comppct",
              "Ljava/lang/String;" ) )
        ( "taxorder",
          java::GetFieldID(
              GetJclass(), "taxorder",
              "Ljava/lang/String;" ) )
        ( "localphase",
          java::GetFieldID(
              GetJclass(), "localphase",
              "Ljava/lang/String;" ) )
        ( "tfact",
          java::GetFieldID(
              GetJclass(), "losstolerance",
              "D" ) )
        ( "slope",
          java::GetFieldID(
              GetJclass(), "slope",
              "D" ) )
        ( "albedodry",
          java::GetFieldID(
              GetJclass(), "albedodry",
              "D" ) )
        //corestrictions
        ( "resdeptmin",
          java::GetFieldID(
              GetJclass(), "impermiableDepth",
              "D" ) )
        //horizon
        ( "hzdept",
          java::GetFieldID(
              GetJclass(), "hzdept",
              "[D" ) )
        ( "hzdepb",
          java::GetFieldID(
              GetJclass(), "hzdepb",
              "[D" ) )
        //( "hzthk",
          //java::GetFieldID(
              //GetJclass(), "hzthk",
              //"[D" ) )
        ( "claytotal",
          java::GetFieldID(
              GetJclass(), "claytotal",
              "[D" ) )
        ( "sandtotal",
          java::GetFieldID(
              GetJclass(), "sandtotal",
              "[D" ) )
        ( "silttotal",
          java::GetFieldID(
              GetJclass(), "silttotal",
              "[D" ) )
        ( "sandvc",
          java::GetFieldID(
              GetJclass(), "sandvco",
              "[D" ) )
        ( "sandco",
          java::GetFieldID(
              GetJclass(), "sandco",
              "[D" ) )
        ( "sandmed",
          java::GetFieldID(
              GetJclass(), "sandmed",
              "[D" ) )
        ( "sandfine",
          java::GetFieldID(
              GetJclass(), "sandfine",
              "[D" ) )
        ( "sandvf",
          java::GetFieldID(
              GetJclass(), "sandvf",
              "[D" ) )
        ( "dbthirdbar",
          java::GetFieldID(
              GetJclass(), "dbthirdbar",
              "[D" ) )
        ( "wtenthbar",
          java::GetFieldID(
              GetJclass(), "wtenthbar",
              "[D" ) )
        ( "wthirdbar",
          java::GetFieldID(
              GetJclass(), "wthirdbar",
              "[D" ) )
        ( "wfifteenbar",
          java::GetFieldID(
              GetJclass(), "w15thbar",
              "[D" ) )
        ( "ksat",
          java::GetFieldID(
              GetJclass(), "ksat",
              "[D" ) )
        ( "cec7",
          java::GetFieldID(
              GetJclass(), "cec7",
              "[D" ) )
        ( "ecec",
          java::GetFieldID(
              GetJclass(), "ecec",
              "[D" ) )
        ( "om",
          java::GetFieldID(
              GetJclass(), "om",
              "[D" ) )
        ( "caco3",
          java::GetFieldID(
              GetJclass(), "caco3",
              "[D" ) )
        ( "ph1to1h2o",
          java::GetFieldID(
              GetJclass(), "ph1to1h2o",
              "[D" ) )
        ( "ph01mcacl2",
          java::GetFieldID(
              GetJclass(), "ph01mcacl2",
              "[D" ) )
        ( "lep",
          java::GetFieldID(
              GetJclass(), "lep",
              "[D" ) )
        //chfrags
        ( "fragvol",
          java::GetFieldID(
              GetJclass(), "fragvol",
              "[D" ) )
        //chtexturegrp
        ( "texture",
          java::GetFieldID(
              GetJclass(), "texture",
              "[Ljava/lang/String;" ) )
        ( "stratextsflag",
          java::GetFieldID(
              GetJclass(), "stratified",
              "[Z" ) )
        ( "rvindicator",
          java::GetFieldID(
              GetJclass(), "rv",
              "[Z" ) )
        //misc
        ( "numlay",
          java::GetFieldID(
              GetJclass(), "numlay",
              "I" ) );

    java::JFIDMAP::const_iterator itr = jfidMap.find( name );
    return itr->second;
}
////////////////////////////////////////////////////////////////////////////////
bool NASIS::IsLayerOrganic(
    std::string const& hzname,
    std::string const& desgnmaster,
    std::string const& texture,
    double const& organicMatter )
{
    //Horizon name
    if( boost::algorithm::starts_with( hzname, "O" ) )
    {
        return true;
    }

    if( boost::algorithm::starts_with( desgnmaster, "O" ) )
    {
        return true;
    }

    if( ORGANIC_TEXTURES.find( boost::algorithm::to_lower_copy( texture ) ) !=
        ORGANIC_TEXTURES.end() )
    {
        return true;
    }

    if( organicMatter > util::ConfigData::GetOmFractionThreshold() )
    {
        return true;
    }

    //Aka, a mineral layer
    return false;
}
////////////////////////////////////////////////////////////////////////////////

} //end soil
} //end weru
} //end wrapper
} //end leaf
