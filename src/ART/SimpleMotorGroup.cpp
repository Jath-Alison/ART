/**
 * @file SimpleMotorGroup.cpp
 * @author Jath Alison (Jath.Alison@gmail.com)
 * @brief Source containing a simple motor-group wrapper class
 * @version 1.0-beta
 * @date 06-26-2024
 *
 * @copyright Copyright (c) 2024
 *
 * This is the Source file defining the SimpleMotorGroup class. The class aims to
 * package a few motor-group features in an easy to use way, allowing the code to be
 * cleaner and a bit more verbose.
 */

#include "ART/SimpleMotorGroup.h"

namespace art
{

	SimpleMotorGroup::SimpleMotorGroup(vex::motor_group mot) : vex::motor_group(mot) {}

	SimpleMotorGroup &SimpleMotorGroup::withSpeedMode(bool speedMode)
	{
		setSpeedMode(speedMode);
		return *this;
	}

	void SimpleMotorGroup::setSpeedMode(bool speedMode)
	{
		m_speedMode = speedMode;
	}

    bool SimpleMotorGroup::getSpeedMode()
    {
        return m_speedMode;
    }

	void SimpleMotorGroup::set(double cmd){
		m_cmd = cmd;
        if (m_speedMode)
        {
            spin(vex::fwd, m_cmd, vex::pct);
        }
        else
        {
            spin(vex::fwd, m_cmd * 12 / 100.f, vex::volt);
        }
	}

	double SimpleMotorGroup::get(){
		return m_cmd;
	}
} // namespace art
