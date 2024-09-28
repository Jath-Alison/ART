/**
 * @file SimpleMotor.cpp
 * @author Jath Alison (Jath.Alison@gmail.com)
 * @brief Source containing a simple motor wrapper class
 * @version 1.0-beta
 * @date 06-26-2024
 *
 * @copyright Copyright (c) 2024
 *
 * This is the Source file defining the SimpleMotor class. The class aims to
 * package a few motor features in an easy to use way, allowing the code to be
 * cleaner and a bit more verbose.
 */

#include "ART/SimpleMotor.h"

namespace art
{

	SimpleMotor::SimpleMotor(vex::motor mot) : vex::motor(mot) {}

	SimpleMotor &SimpleMotor::withSpeedMode(bool speedMode)
	{
		setSpeedMode(speedMode);
		return *this;
	}

	void SimpleMotor::setSpeedMode(bool speedMode)
	{
		m_speedMode = speedMode;
	}

    bool SimpleMotor::getSpeedMode()
    {
        return m_speedMode;
    }

	void SimpleMotor::set(double cmd){
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

	double SimpleMotor::get(){
		return m_cmd;
	}
} // namespace art
