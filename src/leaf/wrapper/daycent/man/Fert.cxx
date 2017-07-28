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
#include "leaf/wrapper/daycent/man/Fert.h"

#include "leaf/wrapper/daycent/file/f_header.h"

// --- Boost Includes --- //
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/assign.hpp>

namespace fs = boost::filesystem;

namespace leaf
{
namespace wrapper
{
namespace daycent
{
namespace man
{
////////////////////////////////////////////////////////////////////////////////
std::set< std::string > const Fert::fert_crops = boost::assign::list_of
    ( "CG" ) ( "CG1" );
////////////////////////////////////////////////////////////////////////////////
Fert* Fert::instance(
    bool const& erase )
{
    static Fert* sFert = new Fert();
    if( erase )
    {
        delete sFert;
        sFert = NULL;
    }

    return sFert;
}
////////////////////////////////////////////////////////////////////////////////
Fert::Fert(void)
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
Fert::~Fert(void)
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
void Fert::UpdateFertFile(
    double const& nitrogen,
    double const& phosphate,
    double const& potassium,
    std::string const& filePath )
{
    std::string line;
    std::ofstream outfile;
    if( fs::exists( filePath ) )
    {
        fs::path file( filePath + "/fert.100" );
        if( !( fs::exists( file ) ) )
        {
            outfile.open( file.string().c_str() );
        }
        else
        {
            outfile.open( file.string().c_str(), std::ios::app );
        }

        outfile << "N" << nitrogen << "Nitrogen__" << nitrogen << "_gN/m2" << std::endl;
        outfile << nitrogen << "              'FERAMT(1)'"  << std::endl;
        outfile << phosphate << "               'FERAMT(2)'"  << std::endl;
        outfile << potassium << "               'FERAMT(3)'"  << std::endl;
        outfile << "0.0               'AUFERT'"     << std::endl;
        outfile << "1.00000           'NINHIB'"     << std::endl;
        outfile << "7.0               'NINHTM'"     << std::endl;
        outfile << "0.75              'FRAC_NH4'"   << std::endl;
        outfile << "0.25              'FRAC_NO3'"   << std::endl;
    }
    outfile.close();
}
////////////////////////////////////////////////////////////////////////////////
std::vector< std::vector< std::string > > 
    Fert::CreateFertEvent( const int year, std::string const& crop )
{
    //std::vector< std::vector< std::string > >  fertEvent;

    std::vector< std::string > day;
	day.push_back("100");
	day.push_back("135");

    std::vector< std::string > fertAp;
    fertAp.push_back( "NAPP" );
    fertAp.push_back( "NSIDE" );

	std::vector< std::vector< std::string > > fert;
	for( size_t j=0; j<day.size(); ++j )
	{
        std::vector< std::string > temp;
        if( fert_crops.find( crop ) != fert_crops.end() )
        {
            temp.push_back( boost::lexical_cast< std::string >(year+1) ); 
		    temp.push_back( day.at(j) ); 
		    temp.push_back( "FERT" );
		    temp.push_back( fertAp.at(j) );
		    fert.push_back( temp );
        }
	}

    return fert;
}
////////////////////////////////////////////////////////////////////////////////
void Fert::UpdateFert( double const& yield, std::string const& abbr )
{
    double fertapp = yield * 0.1121;//conversion rate from bu/ac to g/m^2

    Poco::Dynamic::Var var;
    file::F_100&  f100 = *file::F_100::instance();
    assert( f100.OpenFile( FERT, "", abbr ) );
    file::Option const& opt = f100.GetOption( "NAPP" );
    //var.push_back( diff.at(i) );
    opt.GetValue( "FERAMT(1)", var );
    std::cout << var.convert< double >() << std::endl;
    var = fertapp * 0.4;
    std::cout << var.convert< double >() << std::endl;
    opt.SetValue( "FERAMT(1)", var );

    file::Option const& opt2 = f100.GetOption( "NSIDE" );
    //var.push_back( diff.at(i) );
    opt2.GetValue( "FERAMT(1)", var );
    std::cout << var.convert< double >() << std::endl;
    var = fertapp * 0.6;
    std::cout << var.convert< double >() << std::endl;
    opt2.SetValue( "FERAMT(1)", var );

    f100.WriteOptions();
    f100.CloseFile();

    /*Poco::Dynamic::Var cropvar;
    assert( f100.OpenFile( CROP, "daycent", "crop" ) );
    file::Option const& optii = f100.GetOption( "OAT3" );
    optii.GetValue( "PRDX(1)", cropvar );
    std::cout << "PRDX(1): " << Convert< std::string >( cropvar ) << std::endl;
    optii.SetValue( "PRDX(1)", "0.77" );
    optii.GetValue( "PRDX(1)", cropvar );
    std::cout << "PRDX(1): " << Convert< std::string >( cropvar ) << std::endl;
    f100.WriteOptions();
    f100.CloseFile();*/
/*
    opt = f100.GetOption( "NSIDE" );
    opt.GetValue( "FERAMT(1)", var );
    std::cout << var.convert< double >() << std::endl;
    opt.SetValue( "FERAMT(1)", fertapp * 0.6667 );
    opt.GetValue( "FERAMT(1)", var );
    std::cout << var.convert< double >() << std::endl;
*/
    
}
////////////////////////////////////////////////////////////////////////////////
void Fert::SetFert( 
    std::string const& abbr,
    std::string const& rot, 
    std::string const& fert,
    std::string const& path )
{
    fs::path curpath( fs::current_path() );
    fs::current_path( path.c_str() );

    if( fert == "F" )
    {
        file::F_100& f100 = *file::F_100::instance();
        assert( f100.OpenFile( FERT, "", abbr ) );
        file::Option const& opt = f100.GetOption( "NAPP" );
        file::Option const& opt2 = f100.GetOption( "NSIDE" );
        if( rot == "CG,CG" )
        {
            opt.SetValue( "FERAMT(1)", 23.874 );
            opt.SetValue( "FERAMT(2)", 7.2855 );
            opt.SetValue( "FERAMT(3)", 8.406 );
            opt2.SetValue( "FERAMT(1)", 5.604 );
        }
        else if( rot == "CG,SB" )
        {
            opt.SetValue( "FERAMT(1)", 5.604 );
            //opt.SetValue( "FERAMT(2)", 7.2855 );
            //opt.SetValue( "FERAMT(3)", 8.406 );
            opt2.SetValue( "FERAMT(1)", 17.934 );
        }
        f100.WriteOptions();
        f100.CloseFile();
    }
    else
    {
        file::F_100& f100 = *file::F_100::instance();
        assert( f100.OpenFile( FERT, "", abbr ) );
        file::Option const& opt = f100.GetOption( "NSIDE" );
        opt.SetValue( "FERAMT(1)", 3.923 );
        f100.WriteOptions();
        f100.CloseFile();

        assert( f100.OpenFile( OMAD, "", abbr ) );
        file::Option const& omad = f100.GetOption( "M_P" );
        omad.SetValue( "ASTREC(1)", 15 );
        omad.SetValue( "ASTREC(2)", 7.5 );
        omad.SetValue( "ASTREC(3)", 12 );
        if( rot == "CG,CG" )
        {
            omad.SetValue( "ASTGC", 336.25 );
        }
        else if( rot == "SB,CG" )
        {
            omad.SetValue( "ASTGC", 252.19 );
        }
        f100.WriteOptions();
        f100.CloseFile();
    }

    fs::current_path( curpath );
}
////////////////////////////////////////////////////////////////////////////////
std::string Fert::SetFertOp( std::string fert, std::string day )
{
    std::string op;

    if( fert.find( "surface broadcast" ) != std::string::npos )
	{ 
        if(boost::lexical_cast< double >(day) < 200 )
        {
            op = "NSIDE";
        }
        else
        {
            op = "NAPP";
        }
    }
	//else if( fert.find( "anhyd" ) != std::string::npos )
    //{ op = "NAPP"; }
    else if( fert.find( "Manure" ) != std::string::npos )
    { op = "M_P"; }
    
    return op;
}
} //end man
} //end daycent
} //end wrapper
} //end leaf