/*
  Marine Genset Engine Start and management
  
  This sketch will control and monitor engine and generator parameters for a Perkins 403D-15 / Leyroy Somer 12 kVA genset.
  
  Primary functions:
    Start/stop diesel engine
    Monitor engine parameters and supervise engine shutdown if these fall outside spec. Monitored parameters include:
      - Oil pressure/temperatature.
      - Cooling water flow. 
      - Engine RPM
      - Starter battery voltage
      - Current draw
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



