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

/// \brief The heat demand pin.
#define HEAT_PIN 3

/// \brief The cooling demand pin.
#define COOL_PIN 4

/// \brief The input pin (analog) for the temperature sensor.
#define TEMP_PIN 0

/// \brief The occupied setpoint in heating mode (in Celsius).
#define HEAT_OCC_SP 23

/// \brief The unoccupied setpoint in heating mode (in Celsius).
#define HEAT_UNOCC_SP 18

/// \brief The occupied setpoint in cooling mode (in Celsius).
#define COOL_OCC_SP 25

/// \brief The unoccupied setpoint in cooling mode (in Celsius).
#define COOL_UNOCC_SP 30

/// \brief The thermostat deadband (in Celsius).
#define DEADBAND 2

/// \brief The occupancy timeout (in minutes).
#define OCC_TIMEOUT 30

/// \brief Off mode:
#define OFF_MODE 0

/// \brief Heating mode:
#define HEAT_MODE 1

/// \brief Cooling mode:
#define COOL_MODE 2

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

/// \brief Determines the operating mode.
/// \return HEAT_MODE if in heating mode, COOL_MODE if in cooling
/// mode, or OFF_MODE if disabled.
int get_operating_mode(void);

/// \brief Heating mode logic.
void heat_logic(void);

/// \brief Cooling mode logic.
void cool_logic(void);

/// \brief Checks the occupancy of the building.
/// \return true if the building is occupied, false otherwise.
bool check_occupancy(void);

// FUNCTION DEFINITIONS:

void setup()
{
    pinMode(OCC_PIN, INPUT);
    digitalWrite(OCC_PIN, HIGH); // enable pull-up resistor
    pinMode(HEAT_PIN, OUTPUT);
    digitalWrite(HEAT_PIN, LOW);
    pinMode(COOL_PIN, OUTPUT);
    digitalWrite(COOL_PIN, LOW);
    last_event = millis();
    Serial.begin(9600);
    delay(1000);
}

void loop()
{
    read_temp();
    calc_setpoint();
    switch(get_operating_mode())
    {

    case HEAT_MODE:
        heat_logic();
        break;

    case COOL_MODE:
        cool_logic();
        break;

    default:
        digitalWrite(HEAT_PIN, LOW);
        digitalWrite(COOL_PIN, LOW);

    }
}

void read_temp()
{
    // TODO: implement temperature reading
    act_temp = 0;
}

void calc_setpoint()
{
    switch(get_operating_mode())
    {

    case HEAT_MODE:
        setpoint = check_occupancy() ? HEAT_OCC_SP : HEAT_UNOCC_SP;
        break;

    case COOL_MODE:
        setpoint = check_occupancy() ? COOL_OCC_SP : COOL_UNOCC_SP;
        break;

    default:
        setpoint = 0;

    }
}

int get_operating_mode()
{
    // TODO: implement operating mode
    return HEAT_MODE;
}

void heat_logic()
{
    digitalWrite(COOL_PIN, LOW);
    if(act_temp <= setpoint - (DEADBAND / 2.0))
        digitalWrite(HEAT_PIN, HIGH);
    else if(act_temp >= setpoint + (DEADBAND / 2.0))
        digitalWrite(HEAT_PIN, LOW);
}

void cool_logic()
{
    digitalWrite(HEAT_PIN, LOW);
    if(act_temp <= setpoint - (DEADBAND / 2.0))
        digitalWrite(COOL_PIN, LOW);
    else if(act_temp >= setpoint + (DEADBAND / 2.0))
        digitalWrite(COOL_PIN, HIGH);
}

bool check_occupancy()
{
    if(digitalRead(OCC_PIN) == LOW)
        last_event = millis();
    return millis() - last_event < (OCC_TIMEOUT * 1000ul * 60ul);
}

// jl
