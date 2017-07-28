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
#include <leaf/wrapper/daycent/soil/Soils.h>
#include <leaf/wrapper/daycent/file/F_header.h>

// --- Boost Includes --- //
#include <boost/assign.hpp>

// --- STL Includes --- //
#include <iostream>
#include <fstream>
#include <cmath>

using namespace leaf::open;
using namespace leaf::util;

namespace leaf
{
namespace wrapper
{
namespace daycent
{
namespace soil
{

////////////////////////////////////////////////////////////////////////////////
std::set< unsigned int > const Soils::ADEP = boost::assign::list_of
    ( 10 )                          //ADEP(1)
    ( 30 )                          //ADEP(2)
    ( 45 )                          //ADEP(3)
    ( 60 )                          //ADEP(4)
    ( 90 )                          //ADEP(5)
    ( 120 )                         //ADEP(6)
    ( 150 )                         //ADEP(7)
    ( 180 )                         //ADEP(8)
    ( 210 )                         //ADEP(9)
    ( 240 );                        //ADEP(10)
////////////////////////////////////////////////////////////////////////////////
std::set< unsigned int > const Soils::LAYER = boost::assign::list_of
    ( 2 ) ( 5 ) ( 10 )              //ADEP(1)
    ( 20 ) ( 30 )                   //ADEP(2)
    ( 45 )                          //ADEP(3)
    ( 60 )                          //ADEP(4)
    ( 75 ) ( 90 )                   //ADEP(5)
    ( 105 ) ( 120 )                 //ADEP(6)
    ( 150 )                         //ADEP(7)
    ( 180 )                         //ADEP(8)
    ( 210 )                         //ADEP(9)
    ( 240 );                        //ADEP(10)
////////////////////////////////////////////////////////////////////////////////
std::vector< double > const Soils::EVAP_COEFF = boost::assign::list_of
    ( 0.8 ) ( 0.2 ) ( 0.0 )         //ADEP(1)
    ( 0.0 ) ( 0.0 )                 //ADEP(2)
    ( 0.0 )                         //ADEP(3)
    ( 0.0 )                         //ADEP(4)
    ( 0.0 ) ( 0.0 )                 //ADEP(5)
    ( 0.0 ) ( 0.0 )                 //ADEP(6)
    ( 0.0 )                         //ADEP(7)
    ( 0.0 )                         //ADEP(8)
    ( 0.0 )                         //ADEP(9)
    ( 0.0 );                        //ADEP(10)
////////////////////////////////////////////////////////////////////////////////
std::vector< double > const Soils::TRANS_COEFF = boost::assign::list_of
    ( 0.01 ) ( 0.04 ) ( 0.25 )      //ADEP(1)
    ( 0.30 ) ( 0.10 )               //ADEP(2)
    ( 0.05 )                        //ADEP(3)
    ( 0.04 )                        //ADEP(4)
    ( 0.03 ) ( 0.02 )               //ADEP(5)
    ( 0.01 ) ( 0.0 )                //ADEP(6)
    ( 0.0 )                         //ADEP(7)
    ( 0.0 )                         //ADEP(8)
    ( 0.0 )                         //ADEP(9)
    ( 0.0 );                        //ADEP(10)
////////////////////////////////////////////////////////////////////////////////
std::vector< double > const Soils::MIN_VSWC = boost::assign::list_of
    ( 0.8 ) ( 0.6 ) ( 0.4 )         //ADEP(1)
    ( 0.1 ) ( 0.0 )                 //ADEP(2)
    ( 0.0 )                         //ADEP(3)
    ( 0.0 )                         //ADEP(4)
    ( 0.0 ) ( 0.0 )                 //ADEP(5)
    ( 0.0 ) ( 0.0 )                 //ADEP(6)
    ( 0.0 )                         //ADEP(7)
    ( 0.0 )                         //ADEP(8)
    ( 0.0 )                         //ADEP(9)
    ( 0.0 );                        //ADEP(10)
////////////////////////////////////////////////////////////////////////////////
Soils::Soils()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
Soils::~Soils()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
bool Soils::LoadSQL(
    soi::Component const& component,
    std::string const& dir,
    std::string const& abbr )
{
    std::string cokey = Convert< std::string >( component.GetCokey() );

    //Get the component horizons
    soi::CHorizons const& horizons = component.GetHorizons();
    soi::CHorizonsByHzdept::const_iterator chitr =
        horizons.get< HZDEPT >().begin();

    //This requires at least the first horizon to be valid
    if( chitr == horizons.get< HZDEPT >().end() ) return false;
    Layer layer( **chitr );
    if( !( layer.IsValid() ) ) return false;

    Layers layers;
    double totEvapCoeff( 0.0 ), totTransCoeff( 0.0 );
    std::set< unsigned int >::const_iterator itr = LAYER.begin();
    for( unsigned int lyr = 0; itr != LAYER.end(); ++itr, ++lyr )
    {
        LayerPtr nxtlay( NULL );
        unsigned int const& value = *itr;
        double const& hzdept = layer.GetMinDepth();
        double hzdepb = layer.GetMaxDepth();
        bool success( true );
        if( hzdepb < value )
        {
            Layer agglay = layer;
            agglay *= ( hzdepb - hzdept ) / ( value - hzdept );
            do
            {
                success = ( ++chitr != horizons.get< HZDEPT >().end() );
                if( !success ) break;
                if( Convert< double >((**chitr).GetSandtotal()) == 0 &&
                    Convert< double >((**chitr).GetSilttotal()) == 0 &&
                    Convert< double >((**chitr).GetClaytotal()) == 0 )
                {
                    success = false;
                }
                else
                {
                    nxtlay.reset( new Layer( **chitr ) );
                    success = layer.IsValid();
                }

            if( !success ) break;
                double const& maxdept = nxtlay->GetMaxDepth();
                agglay += *nxtlay * ( ( std::min< double >(
                    maxdept, value ) - hzdepb ) / ( value - hzdept ) );
                hzdepb = maxdept;
            }
            while( hzdepb < value );
            if( success ) layer = agglay;
        }

        layer.SetMaxDepth( value );
        layer.SetEvapCoeff( EVAP_COEFF.at( lyr ) );
        layer.SetTransCoeff( TRANS_COEFF.at( lyr ) );
        layer.SetMinVolH2O( MIN_VSWC.at( lyr ) * layer.GetWltPnt() );
        totEvapCoeff += layer.GetEvapCoeff();
        totTransCoeff += layer.GetTransCoeff();
        layers.push_back( layer );

        if( !success ) break;
        if( hzdepb > value )
        {
            if( nxtlay != NULL ) layer = *nxtlay;
            layer.SetMinDepth( value );
            layer.SetMaxDepth( hzdepb );
        }
        else
        {
            if( ++chitr == horizons.get< HZDEPT >().end() ) break;
            layer = Layer( **chitr );
            if( !( layer.IsValid() ) ) break;
        }
    }

    //file: DailyDayCent/Swconst.h - #define FNSOIL "soils.in"
    std::ofstream myfile;
    std::string fileDir( dir );
    myfile.open( ( fileDir.append( "/soils.in" ) ).c_str() );
    //Update site file
    unsigned int maxdepth = layer.GetMaxDepth();
    std::cout << "Max Depth: " << maxdepth << std::endl;
    std::set< unsigned int >::const_iterator adep = ADEP.begin();
    for( ; adep != ADEP.end(); ++adep )
    {
        std::cout << *adep << std::endl;
        if( maxdepth <= *adep )
        {
            maxdepth = *adep;
            std::cout << maxdepth << std::endl;
            break;
        }
    }
    std::cout << maxdepth << std::endl;

    SetNLayer( 
        std::distance( ADEP.begin(), ADEP.find( maxdepth ) ) + 1);
    SetSilt( Convert< double >( (**horizons.get< HZDEPT >().begin()).GetSilttotal() ) );
    SetSand( Convert< double >( (**horizons.get< HZDEPT >().begin()).GetSandtotal() ) );
    SetClay( Convert< double >( (**horizons.get< HZDEPT >().begin()).GetClaytotal() ) );
    SetBulkDensity( Convert< double >( (**horizons.get< HZDEPT >().begin()).GetDbthirdbar() ) );
    std::cout << Convert< double >( (**horizons.get< HZDEPT >().begin()).GetDbthirdbar() ) << std::endl;

    std::cout << "NALYER " << std::distance( ADEP.begin(), ADEP.find( maxdepth ) ) + 1 << std::endl;
    //opt.SetValue( "NLAYER", adep );
    //Use first layer to set these values (probably not needed at all)
    Layers::iterator litr = layers.begin();

    //Normalize evapCoeff and transCoeff to 1 and write layer to file
    for( ; litr != layers.end(); ++litr )
    {
        Layer& l = *litr;
        assert( totEvapCoeff != 0.0 && totTransCoeff != 0.0 );
        l.SetEvapCoeff( l.GetEvapCoeff() / totEvapCoeff );
        l.SetTransCoeff( l.GetTransCoeff() / totTransCoeff );
        myfile << l;
    }
    myfile.close();

    return true;
}
////////////////////////////////////////////////////////////////////////////////
Soils::Layer::Layer(
    open::soi::CHorizon const& horizon )
    :
    m_isValid( false ),
    m_minDepth( NaN ),
    m_maxDepth( NaN ),
    m_bulkDens( NaN ),
    m_fldCpcty( NaN ),
    m_wltPnt( NaN ),
    m_evapCoeff( NaN ),
    m_transCoeff( NaN ),
    m_sandFrac( NaN ),
    m_clayFrac( NaN ),
    m_siltFrac( NaN ),
    m_om( NaN ),
    m_minVolH2O( NaN ),
    m_satHydCond( NaN ),
    m_ph( NaN )
{
    m_sandFrac = Convert< double >( horizon.GetSandtotal(), NaN ) / 100.0;
    m_clayFrac = Convert< double >( horizon.GetClaytotal(), NaN ) / 100.0;
    m_siltFrac = Convert< double >( horizon.GetSilttotal(), NaN ) / 100.0;
    double acoef = (exp(-4.396 - 0.0715 * m_clayFrac*100 - 4.88e-4 *
              pow(m_sandFrac*100, 2) - 4.285e-5 * pow(m_sandFrac*100, 2) * m_clayFrac*100));
    double bcoef = (-3.14 - 0.00222 * pow(m_clayFrac*100, 2) - 3.484e-5 *
              pow(m_sandFrac*100, 2) * m_clayFrac*100);
    double sat = (0.332 - 7.251e-4 * m_sandFrac*100 + 0.1276 * log10(m_clayFrac*100));
    std::cout << acoef << " " << bcoef << " " << sat << std::endl;

    double wp_raw = (pow((15.0 / acoef), (1.0 / bcoef)));
    double fc_raw = (pow((0.333 / acoef), (1.0 / bcoef)));
    double ksat_raw = (exp((12.012 - 0.0755 * m_sandFrac*100) + (-3.895 + 0.03671 *
             m_sandFrac*100 - 0.1103 * m_clayFrac*100 + 8.7546e-4 * pow(m_clayFrac*100, 2)) /
             sat));
    double bd_raw = (1 - sat) * 2.65f;
    std::cout << wp_raw << " " << fc_raw << " " << ksat_raw << std::endl;

    /* Corrections from Steve Del Grosso */
    double wiltpt = wp_raw + (-0.15f * wp_raw);
    double fieldc = fc_raw + (0.07f * fc_raw);
    double ksat = ksat_raw / 3600;
    double bulkd = bd_raw + (-0.08f * bd_raw);

    m_minDepth = Convert< double >( horizon.GetHzdept(), NaN );
    m_maxDepth = Convert< double >( horizon.GetHzdepb(), NaN );
    m_bulkDens = bulkd;//bd_raw;//Convert< double >( horizon.GetDbthirdbar(), NaN );
    m_fldCpcty = fieldc;//fc_raw;//Convert< double >( horizon.GetWthirdbar(), NaN ) / 100.0;
    m_wltPnt = wiltpt;//wp_raw;//Convert< double >( horizon.GetWfifteenbar(), NaN ) / 100.0;
    
    m_om = Convert< double >( horizon.GetOm(), NaN ) / 100.0;
    m_minVolH2O = wiltpt;//wp_rawK;
    // micrometer/sec to cm/sec
    m_satHydCond = ksat;//ksat_raw;//Convert< double >( horizon.GetKsat(), NaN ) / 10000.0;
    m_ph = Convert< double >( horizon.GetPh01mcacl2(),
        Convert< double >( horizon.GetPh1to1h2o(), NaN ) );

    //Normalize to make sure that the SAND, SILT, and CLAY values sum to 1.0
    double sscsum = m_sandFrac + m_siltFrac + m_clayFrac;
    std::cout << m_sandFrac << " " << m_siltFrac << " " << m_clayFrac << " " <<sscsum << std::endl;
    if( sscsum != 1.0 )
    {
        m_sandFrac /= sscsum;
        m_siltFrac /= sscsum;
        m_clayFrac /= sscsum;
    }

    //
    m_isValid = !(
        *this == NaN ||
        m_maxDepth < m_minDepth ||
        fabs( sscsum - 1.0 ) > 0.1 ||
        sscsum == 0 );

    SetIsValid( m_isValid );
}
////////////////////////////////////////////////////////////////////////////////
Soils::Layer::~Layer()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
bool const& Soils::Layer::IsValid() const
{
    return m_isValid;
}
////////////////////////////////////////////////////////////////////////////////
void Soils::Layer::SetIsValid( bool valid )
{
    m_isValid = valid;
}
////////////////////////////////////////////////////////////////////////////////
double const& Soils::Layer::GetMinDepth() const
{
    return m_minDepth;
}
////////////////////////////////////////////////////////////////////////////////
void Soils::Layer::SetMinDepth(
    double const& minDepth )
{
    m_minDepth = minDepth;
}
////////////////////////////////////////////////////////////////////////////////
double const& Soils::Layer::GetMaxDepth() const
{
    return m_maxDepth;
}
////////////////////////////////////////////////////////////////////////////////
void Soils::Layer::SetMaxDepth(
    double const& maxDepth )
{
    m_maxDepth = maxDepth;
}
////////////////////////////////////////////////////////////////////////////////
double const& Soils::Layer::GetBulkDens() const
{
    return m_bulkDens;
}
////////////////////////////////////////////////////////////////////////////////
double const& Soils::Layer::GetFldCpcty() const
{
    return m_fldCpcty;
}
////////////////////////////////////////////////////////////////////////////////
double const& Soils::Layer::GetWltPnt() const
{
    return m_wltPnt;
}
////////////////////////////////////////////////////////////////////////////////
double const& Soils::Layer::GetEvapCoeff() const
{
    return m_evapCoeff;
}
////////////////////////////////////////////////////////////////////////////////
void Soils::Layer::SetEvapCoeff(
    double const& evapCoeff )
{
    m_evapCoeff = evapCoeff;
}
////////////////////////////////////////////////////////////////////////////////
double const& Soils::Layer::GetTransCoeff() const
{
    return m_transCoeff;
}
////////////////////////////////////////////////////////////////////////////////
void Soils::Layer::SetTransCoeff(
    double const& transCoeff )
{
    m_transCoeff = transCoeff;
}
////////////////////////////////////////////////////////////////////////////////
double const& Soils::Layer::GetMinVolH2O() const
{
    return m_minVolH2O;
}
////////////////////////////////////////////////////////////////////////////////
void Soils::Layer::SetMinVolH2O(
    double const& minVolH2O )
{
    m_minVolH2O = minVolH2O;
}
////////////////////////////////////////////////////////////////////////////////
double const& Soils::Layer::GetSandFrac() const
{
    return m_sandFrac;
}
////////////////////////////////////////////////////////////////////////////////
double const& Soils::Layer::GetClayFrac() const
{
    return m_clayFrac;
}
////////////////////////////////////////////////////////////////////////////////
double const& Soils::Layer::GetSiltFrac() const
{
    return m_siltFrac;
}
////////////////////////////////////////////////////////////////////////////////
double const& Soils::Layer::GetOm() const
{
    return m_om;
}
////////////////////////////////////////////////////////////////////////////////
double const& Soils::Layer::GetSatHydCond() const
{
    return m_satHydCond;
}
////////////////////////////////////////////////////////////////////////////////
double const& Soils::Layer::GetPh() const
{
    return m_ph;
}
////////////////////////////////////////////////////////////////////////////////
int Soils::GetNLayer()
{
    return m_nlayer;
}
////////////////////////////////////////////////////////////////////////////////
void Soils::SetNLayer( int const& layer )
{
    m_nlayer = layer;
}
////////////////////////////////////////////////////////////////////////////////
double Soils::GetSilt()
{
    return m_silt;
}
////////////////////////////////////////////////////////////////////////////////
void Soils::SetSilt( double const& silt )
{
    m_silt = silt;
}
////////////////////////////////////////////////////////////////////////////////
double Soils::GetSand()
{
    return m_sand;
}
////////////////////////////////////////////////////////////////////////////////
void Soils::SetSand( double const& sand )
{
    m_sand = sand;
}
////////////////////////////////////////////////////////////////////////////////
double Soils::GetClay()
{
    return m_clay;
}
////////////////////////////////////////////////////////////////////////////////
void Soils::SetClay( double const& clay )
{
    m_clay = clay;
}
////////////////////////////////////////////////////////////////////////////////
double Soils::GetBulkDensity()
{
    return m_bulkden;
}
////////////////////////////////////////////////////////////////////////////////
void Soils::SetBulkDensity( double const& bulkden )
{
    m_bulkden = bulkden;
}
////////////////////////////////////////////////////////////////////////////////

} //end soil
} //end daycent
} //end wrapper
} //end leaf
