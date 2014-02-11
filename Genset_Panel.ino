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
/*** START/STOP PARAMETERS ***/
#define START_RETRIES 3           // Try start the stinker three times before giving up.
#define START_RETRY_REST 15000    // Milliseconds to wait before retrying to start.
#define START_WAIT_PERIOD 5000    // Milliseconds to wait before starting 
#define START_OIL_P_PERIOD 15000  // How long to wait for oil pressure to rise before emergency shutdown.
#define START_GLOW_PERIOD 6000    // Milliseconds to warm the glow plugs before cranking. Per manual.
#define START_CRANK_TIME 15000    // Manual allows 60s cranking. 15s seems right to me.
#define SHUTDOWN_DELAY 15000      // Let the engine run with no load before shutdown.
#define SHUTDOWN_WAIT 5000        // How long before we cut the fuel to wait before sounding the alarm.


/*** ENGINE SPEC. PARAMS ***/
#define WARMUP_PERIOD 180000      // We are not meant to load the engine during the warm up period.
#define MIN_COOLANT_TEMP 71       // Minimum normal operating temerature. We need this < 180s from start.
#define MAX_COOLANT_TEMP 110      // Max allowable temperature 
#define MAX_OIL_TEMP 120          // Guessing. No idea and the manual is no help. Internet suggest Abs. Max 150.
#define MIN_OIL_PRESSURE 200      // Manual suggest typical min pressure is 207kPa at rated RPM.
#define MAX_OIL_PRESSURE 420      // Manual suggest typical max pressure is 413kPa at rated RPM.
#define MIN_COOLANT_FLOW 5        // Guessing at this value.
#define ENGINE_TARGET_RPM 1500    // The LSA 40 wants 1500rpm.
#define ENGINE_IDLE_RPM 1000      // Idle RPM per manual
#define MIN_RPM TARGET_RPM * 0.8  // Shut down if engine underspeeds.
#define MAX_RPM TARGET_RPM * 1.2  // Shut down if engine overspeeds.
#define MIN_BATT_V 12.2           // Warn if the battery voltage falls below this level. 



/***********************************************************************************************************************/
/*                                                   IO PINS                                                           */
/***********************************************************************************************************************/

#define SPI_PORT 10                 // SPI port needs to be on pin 10 - All a/c and d/c current/voltage is read over SPI
#define OIL_T_PORT A0               // Oil temperature
#define OIL_P_PORT A1               // Oil Pressure
#define COOLANT_T_PORT A2           // Water temperature
#define COOLANT_F_PORT A3           // Coolant water output flow rate
#define ENGINE_TACHO_PORT 2         // Port 2 services an interrupt.
#define FUEL_SOLENOID_PORT 7        // Fuel solenoid needs to be on to run the engine.
#define COOLANT_PUMP_PORT 8    // Water pump needs to be started after starting engine.
#define ALTERNATATOR_LOAD_PORT 9    // Load should be enabled after warm up period, and disabled before stop.
#define GLOW_PLUG_PORT  11          // Glow plugs
#define STARTER_PORT 12             // Starter motor port
#define BUZZER_PORT 13              // Connected to a piezo buzzer.
#define BUTTON_PORT 6               // Staert/stop control button.

/***********************************************************************************************************************/
/*                                                   ERROR CODES                                                       */
/***********************************************************************************************************************/
#define E_ALREADY_RUNNING 0x0001
#define E_COOLANT_TEMP_HIGH 0x0002
#define E_OIL_TEMP_HIGH 0x0006
#define E_BATTERY_LOW 0x0008
#define E_LOW_OIL_RPRESSURE 0x0016

/***********************************************************************************************************************/
/*                                                  ENGINE STATES                                                      */
/* Engine states are modelled on a bitmap:                                                                             */
/*    Bit 1 = running                                                                                                  */
/*    Bit 2 = starting                                                                                                 */
/*    Bit 3 = warmup                                                                                                   */
/*    Bit 4 = coolant pump running                                                                                     */
/*    Bit 5 =                                                                                                          */                                                                              
/*    Bit 6 =                                                                                                          */
/*    Bit 7 =                                                                                                          */
/*    Bit 8 = fault                                                                                                    */
/***********************************************************************************************************************/

#define S_ENGINE_STOPPED 0x0000
#define S_ENGINE_RUNNING 0x0001
#define S_ENGINE_STARTING 0x0002
#define S_ENGINE_WARMUP 0x0004
#define S_ENGINE_FAULT 0x0128

/***********************************************************************************************************************/
/*                                 GLOBAL VARIABLES FOR THINGS TO KEEP TRACK OF                                        */
/***********************************************************************************************************************/

/*** ENGINE DATA ***/
/* Engine state */
int gEngineState = S_ENGINE_STOPPED;

/* Start time */
unsigned long gStartMillis = 0;

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

float gFaultCode = 0;


#define S_ON  LOW
#define S_OFF HIGH

#include <serial.io>

/***********************************************************************************************************************/
/*                                                   SETUP                                                             */
/***********************************************************************************************************************/

void setup(){
  Serial.begin(57600);
  Serial.println("Initialising all ports");
  //Work out current state (are we running, was there an error)
  pinMode(FUEL_SOLENOID_PORT, OUTPUT);
  pinMode(COOLANT_PUMP_PORT, OUTPUT);
  pinMode(ALTERNATATOR_LOAD_PORT, OUTPUT);
  pinMode(GLOW_PLUG_PORT, OUTPUT);
  pinMode(STARTER_PORT, OUTPUT);
  pinMode(BUZZER_PORT, OUTPUT);
  
  digitalWrite(FUEL_SOLENOID_PORT, S_OFF);
  digitalWrite(COOLANT_PUMP_PORT, S_OFF);
  digitalWrite(ALTERNATATOR_LOAD_PORT, S_OFF);
  digitalWrite(GLOW_PLUG_PORT, S_OFF);
  digitalWrite(STARTER_PORT, S_OFF);
  digitalWrite(BUZZER_PORT, LOW);
  start();
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
  //check for error condidtions that would preclude a safe start.
  gFaultCode = getPreStartFaults();
  if (gFaultCode) {
    Serial.print("A fault was detected. Setting engine to fault. Code: ");
    Serial.println(gFaultCode);
    gEngineState = gEngineState & S_ENGINE_FAULT;
    return;
  }
  // Nothing will stop us trying to start now.
  gEngineState = S_ENGINE_STOPPED;
  Serial.println("Waiting 200ms before attempting to start");
  delay(200);
  for ( int startRetries = START_RETRIES; startRetries > 1; startRetries--){
    Serial.print("Starting sequence begin: Attempt #");
    Serial.println(startRetries);
    engineStart();
    if (isRunning())
      break;
    Serial.println("Engine failed to start. Taking a break before retrying.");
    delay(START_RETRY_REST);
  }
  // If we did all that and it's not running, we should tell someone.
}

void engineStart(){
  // Sound the annoyer
   Serial.println("Sound the buzzer as a warning");
  digitalWrite(BUZZER_PORT, HIGH);
  //delay(START_WAIT_PERIOD);
  digitalWrite(BUZZER_PORT, LOW);
  Serial.println("Setting engine state to S_ENGINE_STARTING");
  gEngineState = gEngineState & S_ENGINE_STARTING;
  // turn on the fuel
  digitalWrite(FUEL_SOLENOID_PORT, S_ON);
  // let the plugs gS_OFF...
  digitalWrite(GLOW_PLUG_PORT, S_ON);
  delay(START_GLOW_PERIOD);
  // let the cranking begin
  digitalWrite(STARTER_PORT, S_ON);
  int crankMillis = millis() + START_CRANK_TIME;
  // crank until either the engine starts or we exceed the start crank time.
  while (millis() < crankMillis){
    delay(200);
    gFaultCode = getRunFaults();
    if (gFaultCode > 0){
      gEngineState = gEngineState & S_ENGINE_FAULT;
      break;
    }
    if (isRunning()) {
      gEngineState = gEngineState & S_ENGINE_RUNNING & S_ENGINE_WARMUP;
      gStartMillis = millis();
      break;
    }
  }
  digitalWrite(STARTER_PORT, S_OFF);
  digitalWrite(GLOW_PLUG_PORT, S_OFF);
  gEngineState = gEngineState ^ S_ENGINE_STARTING;
  if (gEngineState & S_ENGINE_FAULT){
    digitalWrite(ALTERNATATOR_LOAD_PORT, S_OFF);
    engineImmediateStop();
  }
}

void engineStop(){
  digitalWrite(ALTERNATATOR_LOAD_PORT, S_OFF);
  delay(SHUTDOWN_DELAY);
  engineImmediateStop();
}

void engineImmediateStop(){
  digitalWrite(FUEL_SOLENOID_PORT, S_OFF);
  int shutdownMillis = millis() + SHUTDOWN_WAIT;
  while (millis() < shutdownMillis){
    delay(200);
    if (!isRunning()){
      gEngineState = gEngineState ^ S_ENGINE_RUNNING;
      break;
    }
  }
  // if after trying to shut down it's still running
  if (gEngineState & S_ENGINE_RUNNING){
      // Sound the annoyer until someone presses the button
      while (digitalRead(BUTTON_PORT) == S_OFF) {
        digitalWrite(BUZZER_PORT, HIGH);
        delay(200);
        digitalWrite(BUZZER_PORT, LOW);
        delay(200);
      }
  }
}

void manageEngine(){
  // we can't manage it if it isn't running.
  if (! gEngineState & S_ENGINE_RUNNING)
    return;
  gOilPressure = analogRead(OIL_P_PORT);
  gOilTemp = analogRead(OIL_T_PORT);
  gCoolantTemp = analogRead(COOLANT_T_PORT);
  gCoolantFlow = analogRead(COOLANT_F_PORT);

  if ((gEngineState & S_ENGINE_WARMUP) && ((gCoolantTemp > MIN_COOLANT_TEMP) || (millis() > gStartMillis + WARMUP_PERIOD))) {
    digitalWrite(ALTERNATATOR_LOAD_PORT, S_ON);
    digitalWrite(COOLANT_PUMP_PORT, S_ON);
    gEngineState = gEngineState ^ S_ENGINE_WARMUP;
  }

  gFaultCode = getRunFaults();
  if (gFaultCode) {
    gEngineState = gEngineState & S_ENGINE_FAULT;
    engineStop();
  }
}

int getPreStartFaults(){
  int fault = 0;
  if (isRunning())
    fault = fault & E_ALREADY_RUNNING;
  fault = fault & getRunFaults();
  return fault;
}

int getRunFaults(){
  int fault = 0;
  if (gCoolantTemp >= MAX_COOLANT_TEMP)
    fault = fault & E_COOLANT_TEMP_HIGH;
  if (gOilTemp >= MAX_OIL_TEMP)
    fault = fault & E_OIL_TEMP_HIGH;
  if (gStartBattVolts <= MIN_BATT_V)
    fault = fault & E_BATTERY_LOW;
  if ((millis() > gStartMillis + START_OIL_P_PERIOD) && (gOilPressure < MIN_OIL_PRESSURE))
    fault = fault & E_LOW_OIL_RPRESSURE;
  return fault;
}


boolean isRunning(){
  // if we think the engine is running we believe ourself.
  if (gEngineState & S_ENGINE_RUNNING){
    return true;
  }
  if (gEngineRpm >= ENGINE_IDLE_RPM)
    return true;
  return false;
}
