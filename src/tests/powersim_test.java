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


import leaf.wrapper.powersim.*;

public class powersim_test
{
    public static void main(
        String[] args )
    {
        System.out.println( "Initializing CComAutoInit" );
        CComAutoInit.Instance();

        SimulationEngine simulationEngine = SimulationEngine.Create();

        String fileName = "Algae Production Module Ver 1.sip";
        String key = "";
        String password = "";
        SimulationProject simulationProject =
            simulationEngine.OpenProject( fileName );

        String simCompName = "Production";
        SimulationComponent simulationComponent =
            simulationProject.GetComponent( simCompName );

        String simName = "Production Model";
        Simulation simulation =
            simulationComponent.GetSimulation( simName );

        String modVarName = "Number of Ponds";
        ModelVariable numberOfPonds =
            simulation.GetModelVariable( modVarName );
        Value nopValue = numberOfPonds.GetValue();
        Var nopVar = nopValue.GetNumber();
        Double nopD = nopVar.ConvertDouble();
        System.out.println( nopD );

        //We need to release our shared pointer references here before calling
        //CoUnitialize so that the CComPtr(s) can destruct themseleves
        //Do so in reverse order of creation to be safe
        nopValue.delete();
        numberOfPonds.delete();

        modVarName = "Plant Cost Index";
        ModelVariable plantCostIndex =
            simulation.GetModelVariable( modVarName );
        Value pciValue = plantCostIndex.GetValue();
        Var pciVar = pciValue.GetNumber();
        Var pciVar1 = pciVar.At( 1 );
        Double pciD = pciVar1.ConvertDouble();
        System.out.println( pciD );

        pciVar.At( 7 ).Set( 4.0 );
        Value.PutNumberResult pnr = pciValue.PutNumber( pciVar );
        System.out.println( pnr );

        //We need to release our shared pointer references here before calling
        //CoUnitialize so that the CComPtr(s) can destruct themseleves
        //Do so in reverse order of creation to be safe
        pciValue.delete();
        plantCostIndex.delete();

        //We need to release our shared pointer references here before calling
        //CoUnitialize so that the CComPtr(s) can destruct themseleves
        //Do so in reverse order of creation to be safe
        simulation.delete();
        simulationComponent.delete();
        simulationProject.delete();
        simulationEngine.delete();
        CComAutoInit.Instance( true );
        System.out.println( "Cleaned up CComAutoInit" );
    }
}
