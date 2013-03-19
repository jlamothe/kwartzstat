// -*- mode: c++ -*-

/// \file kwartzstat.ino

// kwartzstat

// Copyright (C) 2013 Jonathan Lamothe <jonathan@jlamothe.net>

// This program is free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.

// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see: http://www.gnu.org/licenses/

// #DEFINES:

/// \brief The input pin for the occupancy switch.
#define OCC_PIN 2

/// \brief The thermostat output pin (would need to drive a relay).
#define TT_PIN 3

/// \brief The input pin (analog) for the temperature sensor.
#define TEMP_PIN 0

/// \brief The occupied setpoint (in Celsius).
#define OCC_SP 23

/// \brief The unoccupied setpoint (in Celsius).
#define UNOCC_SP 18

/// \brief The thermostat deadband (in Celsius).
#define DEADBAND 2

/// \brief The occupancy timeout (in minutes).
#define OCC_TIMEOUT 30

// GLOBAL VARIABLES:

/// \brief The actual temperature (in Celsius).
double act_temp;

/// \brief The temperature setpoint (in Celsius).
double setpoint;

/// \brief The time of the last event (in ms).
unsigned long last_event;

// FUNCTION PROTOTYPES:

/// \brief Reads the actual temperature.
void read_temp(void);

/// \brief Calculates the setpoint.
void calc_setpoint(void);

/// \brief Checks the occupancy of the building.
/// \return true if the building is occupied, false otherwise.
bool check_occupancy(void);

// FUNCTION DEFINITIONS:

void setup()
{
    pinMode(OCC_PIN, INPUT);
    digitalWrite(OCC_PIN, HIGH); // enable pull-up resistor
    pinMode(TT_PIN, OUTPUT);
    digitalWrite(TT_PIN, LOW);
    last_event = millis();
    Serial.begin(9600);
    delay(1000);
}

void loop()
{
    read_temp();
    calc_setpoint();
    if(act_temp <= setpoint - (DEADBAND / 2.0))
        digitalWrite(TT_PIN, HIGH);
    else if(act_temp >= setpoint + (DEADBAND / 2.0))
        digitalWrite(TT_PIN, LOW);
}

void read_temp()
{
    // TODO: implement temperature reading
    act_temp = 0;
}

void calc_setpoint()
{
    setpoint = check_occupancy() ? OCC_SP : UNOCC_SP;
}

bool check_occupancy()
{
    if(digitalRead(OCC_PIN) == LOW)
        last_event = millis();
    return millis() - last_event < (OCC_TIMEOUT * 1000ul * 60ul);
}

// jl
