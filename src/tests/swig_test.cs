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

using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection;

using std;

using Poco;
using Poco.Dynamic;

using leaf.util;

using leaf.open;
using ero = leaf.open.ero;
using scn = leaf.open.scn;
using soi = leaf.open.soi;

using leaf.wrapper;
using weru = leaf.wrapper.weru;
using rusle2 = leaf.wrapper.rusle2;

public class Program
{

    ///
    static void Main(
        string[] args )
    {
        //string jnk = Console.ReadLine();

        Program program = new Program();
        program.Init( args );
        program.Main();
        program.Exit();
    }

    ///
    void Init(
        string[] args )
    {
        //This name has to have .wjr as the extension
        string wepsRunDir = "1\\weps.wjr";
        string r2RunDir = "1";

        leaf_open.RegisterConnectors();

        weru.leaf_wrapper_weru.Init( wepsRunDir );
        weru.leaf_wrapper_weru.SetMaxRotYears( 5 );
        rusle2.leaf_wrapper_rusle2.Init( "nelson_2013.gdb", r2RunDir );
    }

    ///
    void Main()
    {
        //
        cmz = new Var( "05" );
        name = new Var( "CO001" );
        soil = new Var( "94035:139959" );
        rotation = new Var( "CG" );
        tillage = new Var( "NT" );
        removal = new Var( "MHH" );
        yldreg = new Var( "2010YLD" );
        yield = new Var( "88" );
        clir2 = new Var( "USA\\Colorado\\Adams County\\CO_Adams_R_13" );
        man = new Var( "05_CG_NT_MHH" );

        //Create a new scenario object
        scenario = scn.Scenario.Create(
            name, soil, rotation, tillage, removal, yldreg );

        //If we've already done these, continue
        bool exWeps = ero.Erosion.Exists( ero.Type.WIND, scenario );
        bool exR2 = ero.Erosion.Exists( ero.Type.WATER, scenario );
        if( exWeps && exR2 ) return;

        //Get SSURGO data for this mapunit
        mapunit = soi.MapUnit.Create( soil.ConvertString() );
        component = mapunit.GetComponent( soil.ConvertString() );

        //Get the yield and rotation string
        cropMap = new CROP_MAP();
        cropMap[ Crop.Type.CORN ] = yield;

        //Run the models
        if( !WEPS() ) return;
        if( !RUSLE2() ) return;

        //If we made it here, everything was successful
        dataWeps.WriteToDB();
        dataR2.WriteToDB();
    }

    ///
    bool WEPS()
    {
        weru.leaf_wrapper_weru.UseCycleCount( cycleCount );
        weru.leaf_wrapper_weru.SetLocation( name.ConvertString() );

        //Set climate pointer
        string cliWeps = name.ConvertString() + ".cli";
        weru.leaf_wrapper_weru.SetRfdFilePtr( weru.File.CLIMATE, cliWeps );

        //Set wind pointer
        string winWeps = name.ConvertString() + ".win";
        weru.leaf_wrapper_weru.SetRfdFilePtr( weru.File.WIND, winWeps );

        //Create WEPS soil file
        string ck = soil.ConvertString(); ck = ck.Replace( ':', '-' );
        string ifcWeps = name.ConvertString() + "_" + ck + ".ifc";
        SoilError err =
            weru.leaf_wrapper_weru.CreateSoilFile( component, ifcWeps );
        if( err != SoilError.SOIERR_NONE ) return false;

        //Set soil pointer
        weru.leaf_wrapper_weru.SetRfdFilePtr( weru.File.SOIL, ifcWeps );
        double slope = component.GetSlope().value().ConvertDouble();
        if( slope <= 0.0 || slope > 100.0 /*|| slope == NaN*/ ) return false;
        weru.leaf_wrapper_weru.AssignSlopeVals(
            new Var( slope ), new Var( lightle.SlopeLen( slope ) ) );

        //Set management pointer
        string manWeps = man.ConvertString() + ".man";
        weru.leaf_wrapper_weru.SetRfdFilePtr( weru.File.MANAGEMENT, manWeps );
        weru.leaf_wrapper_weru.AssignYields( cropMap );

        //Set water erosion loss
        weru.leaf_wrapper_weru.SetWaterErosionLoss( new Var( 0.0 ) );

        //Run Weps model
        Console.WriteLine( "{0}: Running Weps........", 1 );
        dataWeps = weru.leaf_wrapper_weru.CallWeps(
            scenario, numCropCalItrs, cycleCount, 0 );

        return true;
    }

    ///
    bool RUSLE2()
    {
        //Set climate pointer
        rusle2.leaf_wrapper_rusle2.SetObjPtr(
            rusle2.ObjectType.OBJTYP_CLIMATE, clir2.ConvertString() );

        //Create RUSLE2 soil file
        string ck = soil.ConvertString(); ck = ck.Replace( ':', '-' );
        string soiR2 = name.ConvertString() + "_" + ck + ".soi";
        SoilError err = rusle2.leaf_wrapper_rusle2.CreateSoilFile(
            component, soiR2 );
        if( err != SoilError.SOIERR_NONE ) return false;

        //Set soil pointer
        rusle2.leaf_wrapper_rusle2.SetObjPtr(
            rusle2.ObjectType.OBJTYP_SOIL, soiR2,
            rusle2.FilePrefix.HASHTAG_XML );
        double slope = component.GetSlope().value().ConvertDouble();
        if( slope <= 0.0 || slope > 100.0 /*|| slope == NaN*/ ) return false;
        rusle2.leaf_wrapper_rusle2.AssignSlopeVals(
            new Var( slope ), new Var( lightle.SlopeLen( slope ) ) );

        //Set management pointer
        string manr2 = man.ConvertString();
        rusle2.leaf_wrapper_rusle2.SetObjPtr(
            rusle2.ObjectType.OBJTYP_MANAGEMENT, manr2 );
        rusle2.leaf_wrapper_rusle2.AssignYields( cropMap );

        //Set wind erosion loss
        rusle2.leaf_wrapper_rusle2.SetWindErosionLoss( dataWeps.GetWindEros() );

        //Run Rusle2 model
        Console.WriteLine( "{0}: Running Rusle2........", 1 );
        dataR2 = rusle2.leaf_wrapper_rusle2.CallRusle2( scenario, false );

        //Manual reset...this is still janky...
        rusle2.leaf_wrapper_rusle2.ResetObjPtr(
            rusle2.ObjectType.OBJTYP_MANAGEMENT );
        rusle2.leaf_wrapper_rusle2.ResetObjPtr(
            rusle2.ObjectType.OBJTYP_CLIMATE );
        rusle2.leaf_wrapper_rusle2.ResetObjPtr(
            rusle2.ObjectType.OBJTYP_SOIL );

        return true;
    }

    ///
    void Exit()
    {
        leaf_open.UnregisterConnectors();

        weru.leaf_wrapper_weru.Exit();
        rusle2.leaf_wrapper_rusle2.Exit();
    }

    ///
    private uint cycleCount = 50, numCropCalItrs = 15;

    ///
    private Var
        cmz, name, soil, rotation, tillage, removal, yldreg, yield, clir2, man;

    ///
    private CROP_MAP cropMap;

    ///
    private ero.Erosion dataWeps, dataR2;

    ///
    private scn.Scenario scenario;

    ///
    private soi.MapUnit mapunit;
    private soi.Component component;
}
