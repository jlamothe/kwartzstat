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

/// \brief The fan override pin.
#define FAN_PIN 5

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

/// \brief Heating override mode:
#define HEAT_OVER 3

/// \brief Coolding override mode:
#define COOL_OVER 4

// GLOBAL VARIABLES:

/// \brief The actual temperature (in Celsius).
double act_temp;

/// \brief The temperature setpoint (in Celsius).
double setpoint;

/// \brief Operating mode.
int mode;

/// \brief The time of the last event (in ms).
unsigned long last_event;

// FUNCTION PROTOTYPES:

/// \brief Reads the actual temperature.
/// \return The temperature.
double read_temp(void);

/// \brief Calculates the setpoint.
/// \return The calculated setpoint.
double calc_setpoint(void);

/// \brief Determines the operating mode.
/// \return HEAT_MODE if in heating mode, COOL_MODE if in cooling
/// mode, or OFF_MODE if disabled.
int get_operating_mode(void);

/// \brief Sets the outputs.
void set_outputs(void);

/// \brief Heating mode logic.
void heat_logic(void);

/// \brief Cooling mode logic.
void cool_logic(void);

/// \brief Checks the occupancy of the building.
/// \return true if the building is occupied, false otherwise.
bool check_occupancy(void);

/// \brief Checks the fan override.
/// \return true if the fan is in override, false otherwise.
bool check_fan_override(void);

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
    act_temp = read_temp();
    setpoint = calc_setpoint();
    mode = get_operating_mode();
    set_outputs();
}

double read_temp()
{
    // TODO: implement temperature reading
    return 0;
}

double calc_setpoint()
{
    switch(mode)
    {

    case HEAT_MODE:
        return check_occupancy() ? HEAT_OCC_SP : HEAT_UNOCC_SP;

    case COOL_MODE:
        return check_occupancy() ? COOL_OCC_SP : COOL_UNOCC_SP;

    }
    return 0;
}

int get_operating_mode()
{
    // TODO: implement operating mode
    return HEAT_MODE;
}

void set_outputs()
{
    switch(mode)
    {

    case HEAT_MODE:
        heat_logic();
        break;

    case COOL_MODE:
        cool_logic();
        break;

    case HEAT_OVER:
      digitalWrite(HEAT_PIN, HIGH);
      digitalWrite(COOL_PIN, LOW);
      break;

    case COOL_OVER:
      digitalWrite(HEAT_PIN, LOW);
      digitalWrite(COOL_PIN, HIGH);
      break;

    default:
        digitalWrite(HEAT_PIN, LOW);
        digitalWrite(COOL_PIN, LOW);

    }

    digitalWrite(FAN_PIN, check_fan_override());
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

bool check_fan_override(void)
{
  // TODO: implement fan override
  return false;
}

// jl
