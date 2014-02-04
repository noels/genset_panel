/*
  Marine Genset Engine Start and management
  
  This sketch will control and monitor engine and generator parameters for a Perkins 403D-15 / Leyroy Somer LSA 40 12 kVA genset.
  
  Primary functions:
    Start/stop diesel engine
    Monitor engine parameters and supervise engine shutdown if these fall outside spec. Monitored parameters include:
      - Oil pressure/temperatature.
      - Cooling water flow. 
      - Coolant Temperature.
      - Engine RPM.
      - Starter battery voltage.
      - DC Current draw.
    Monitor and supervise generator parameters and provide warnings if these fall outside spec.
    Monitor and supervise battery bank.
    Provide automatic start and stop to maintain battery banks in optimal condition.
    Log engine, generator and electrical usage data for later analysis
    
    
    Copyright 2014 Noel Sharpe
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.    
*/

/***********************************************************************************************************************/
/*                                            CONFIG PARAMETERS                                                        */
/*                                                                                                                     */
/*               Customise the behaviour of the controller by altering the parametes below.                            */
/*                                                                                                                     */
/*                                                                                                                     */
/***********************************************************************************************************************/
/*** START PARAMETERS ***/
#define START_RETRIES = 3;           // Try start the stinker three times before giving up.
#define START_RETRY_REST = 15;       // Seconds to wait before retrying to start.
#define START_WAIT = 5;              // Seconds to wait before starting 
#define START_GLOW_PERIOD = 6;       // Seconds to warm the glow plugs before cranking. Per manual.
#define START_CRANK_TIME = 15;       // Manual allows 60s cranking. 15s seems right to me.


/*** ENGINE SPEC. PARAMS ***/
#define WARMUP_PERIOD = 180;         // We are not meant to load the engine during the warm up period.
#define MIN_COOLANT_T = 71;          // Minimum normal operating temerature. We need this < 180s from start.
#define MAX_COOLANT_T = 110;         // Max allowable temperature 
#define MAX_OIL_T = 120;             // Guessing. No idea and the manual is no help. Internet suggest Abs. Max 150.
#define MIN_OIL_P = 200;             // Manual suggest typical min pressure is 207kPa at rated RPM.
#define MAX_OIL_P = 420;             // Manual suggest typical max pressure is 413kPa at rated RPM.
#define MIN_COOLANT_FLOW = 5;        // Guessing at this value.
#define TARGET_RPM = 1500;           // The LSA 40 wants 1500rpm.
#define MIN_RPM = TARGET_RPM * 0.8;  // Shut down if engine underspeeds.
#define MAX_RPM = TARGET_RPM * 1.2;  // Shut down if engine overspeeds.
#define MIN_BATT_V = 12.2;           // Warn if the battery voltage falls below this level. 



/***********************************************************************************************************************/
/*                                                   IO PINS                                                           */
/***********************************************************************************************************************/

#define SPI_PORT = 10;               // SPI port needs to be on pin 10 - All a/c and d/c current/voltage is read over SPI
#define OIL_T_PORT = A0;             // Oil temperature
#define OIL_P_PORT = A1;             // Oil Pressure
#define COOLANT_T_PORT = A2;         // Water temperature
#define COOLANT_F_PORT = A3;         // Coolant water output flow rate
#define ENGINE_TACHO_PORT = 2;       // Port 2 services an interrupt.
#define ENGINE_FUEL_SOLENOID = 7;    // Fuel solenoid needs to be on to run the engine.
#define ENGINE_WATER_PUMP = 8;       // Water pump needs to be started after starting engine.
#define ALTERNATATOR_LOAD = 9;       // Load should be enabled after warm up period, and disabled before stop.

/***********************************************************************************************************************/
/*                                 GLOBAL VARIABLES FOR THINGS TO KEEP TRACK OF                                        */
/***********************************************************************************************************************/

/*** ENGINE DATA ***/
/* Engine RPM from the engine tacho sensor */
int gEngineRpm = 0;

/* Engine oil pressure measured in Pascal */
int gOilPressure  = 0;

/* Engine oil temperature in Degrees Celsius */
int gOilTemp = 0;

/* Coolant temperature in Degrees Celsius */
int gCoolantTemp = 0;

/* Coolant flow in L/m */
int gCoolantFlow = 0;

/* Starter battery voltage */
float gStartBattVolts = 0;

/* Backup battery voltage */
float gBackupBattVolts = 0;

/* Total DC current draw */
float gDCCurrent = 0;

/*** GENSET DATA ***/
/* L1 voltage */
float gL1Volts = 0;

/* L2 voltage */
float gL2Volts = 0;

/* L3 voltage */
float gL3Volts = 0;

/* L1 current */
float gL1Amps = 0;

/* L2 current */
float gL2Amps = 0;

/* L3 current */
float gL3Amps = 0;

/*** BATTERY BANK DATA ***/
/* Battery bank voltage (V) */
float gBatteryBankVols = 0;

/* Battery bank current (A) */
float gBatteryBankAmps = 0;

/* Battery bank capacity (Ah) */
float gBatteryBankAmpHours = 0;



/***********************************************************************************************************************/
/*                                                   SETUP                                                             */
/***********************************************************************************************************************/

void setup(){
  //Work out current state (are we running, was there an error)
  
  
}


/***********************************************************************************************************************/
/*                                                 MAIN LOOP                                                           */
/***********************************************************************************************************************/

void loop(){
  //Check for any inputs
  //CASE:
    //request start
    //request stop
  //ESAC
  //Update state
  //Check parameters within spec.
  //Log data
}

void start(){
    
}




