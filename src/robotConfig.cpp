/**
 * @file robotConfig.cpp
 * @author Jath Alison (Jath.Alison@gmail.com)
 * @brief Source defining objects that configure the robot such as the Brain,
 * Motors, and additional Devices
 * @version 0.1
 * @date 06-26-2024
 *
 * @copyright Copyright (c) 2024
 *
 * This file will definine all the devices that were delcared in the various
 * headers. Anything that can be defined to change the the configuration of the
 * robot will be put here. This includes, but isn't limited to motors, sensors
 * and important variables.
 */

#include "robotConfig.h"

vex::brain Brain;
vex::controller Controller1;

vex::motor a = vex::motor( vex::PORT20 );
vex::motor b = vex::motor( vex::PORT12, true );

art::SimpleMotorGroup testMotorGroup = art::SimpleMotorGroup( vex::motor_group(
       a , b
) ).withSpeedMode( true );