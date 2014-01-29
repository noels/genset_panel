/*
  Marine Genset Engine Start and management
  
  This sketch will control and monitor engine and generator parameters for a Perkins 403D-15 / Leyroy Somer 12 kVA genset.
  
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
  
}


/***********************************************************************************************************************/
/*                                                 MAIN LOOP                                                           */
/***********************************************************************************************************************/

void loop(){
  
}


