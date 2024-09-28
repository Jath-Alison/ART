/**
 * @file SimpleMotorGroup.h
 * @author Jath Alison (Jath.Alison@gmail.com)
 * @brief Header containing a simple motor_group wrapper class
 * @version 0.1
 * @date 06-27-2024
 *
 * @copyright Copyright (c) 2024
 *
 * This is the header that needs to be included to use the SimpleMotorGroup class.
 * The class aims to package a few motor-group features in an easy to use way,
 * allowing the code to be cleaner and a bit more verbose.
 */

#pragma once

#include "vex.h"

namespace art
{

    /**
     * @brief A Simple Motor_Group Wrapper Class
     *
     * The Simple Motor_Group class aims to provide a simpler and more efficient way
     * to command groups of motors. The class inherits the vex::motor_group class and retains
     * all the same functionality, but gains quality of life functions, allowing
     * the motor group's use to be as simple as get() and set().
     *
     * For more complex motor controls, such as position or velocity, use the
     * SmartMotor class (planned for a later version)
     *
     * @todo make stop() functions withbrakeModes, also set the cmd to 0
     */

    class SimpleMotorGroup : public vex::motor_group
    {
    public:
        /**
         * @brief Construct a new Simple Motor_Group object
         *
         * Uses a vex::motor_group (a normal v5 motor-group) to construct a Simple Motor_Group
         * object
         *
         * @param mot A vex::motor_group, which is a normal v5 motor_group
         *
         * @b Example:
         * @code {.cpp}
         * SimpleMotor testMotor = SimpleMotor( vex::motor_group(
         * 		vex::motor(vex::PORT10 ), vex::motor(vex::PORT11 )
		 * ) );
         * @endcode
         *
         */
        SimpleMotorGroup(vex::motor_group mot);

        /**
         * @brief Returns a SimpleMotorGroup refrence with the specified speedMode
         *
         * Modifies the SimpleMotorGroup object so that it has the speedMode
         * specified by the parameter. A refrence to the SimpleMotor is
         * returned, allowing more methods to be chained to it. This also means
         * that it can be used directly as an extension to the contructor.
         *
         * @param speedMode Whether or not the motor-group should use speed, rather
         * than voltage
         * @return SimpleMotorGroup& (SimpleMotorGroup Refrence)
         *
         * @b Example:
         * @code {.cpp}
		 * SimpleMotorGroup testMotorGroup = SimpleMotorGroup( vex::motor_group(
         * 		vex::motor( vex::PORT10 ), vex::motor( vex::PORT11 ) 
		 * ) ); 
		 * testMotorGroup.withSpeedMode( true );
         * @endcode
         *
         * @b Chained @b Methods @b Example:
         *
         * Because a refrence to the object is returned, that refrence can be
         * used to call other methods.
         *
         * @code {.cpp}
		 * SimpleMotor testMotorGroup = SimpleMotorGroup( vex::motor_group(
         * 		vex::motor( vex::PORT10 ), vex::motor( vex::PORT11 ) 
		 * ) );
		 * testMotorGroup.withSpeedMode( true ).set( 50 );
         * @endcode
         *
         * @b Chained @b Methods @b Example:
         *
         * Keep in mind, methods that don't return a refrence to a SimpleMotor
         * Object can't be used to create a new Object. Even though you could
         * chain .set(double cmd) onto the end, it doesn't return an object
         * refrence, so it couldn't be used to construct testMotor.
         *
         * @code {.cpp}
		 * SimpleMotorGroup testMotorGroup = SimpleMotorGroup( vex::motor_group(
         * 		vex::motor( vex::PORT10 ), vex::motor( vex::PORT11 ) 
		 * ) ).withSpeedMode( true );
         * @endcode
         */
        SimpleMotorGroup &withSpeedMode(bool speedMode);

        /**
         * @brief Set the SpeedMode of the SimpleMotorGroup
         *
         * This method simply sets the SpeedMode of the SimpleMotorGroup. It doesn't
         * return anything, so additional methods cannot be chained to it.
         *
         * @param speedMode Whether or not the motor-group should use speed, rather
         * than voltage
         * 
         * @b Example:
         * @code {.cpp}
         * SimpleMotorGroup testMotorGroup = SimpleMotorGroup( vex::motor_group(
         * 		vex::motor( vex::PORT10 ), vex::motor( vex::PORT11 ) 
		 * ) );
         * testMotorGroup.setSpeedMode( false );
         * @endcode
         */
        void setSpeedMode(bool speedMode);

        /**
         * @brief Get the SpeedMode of the SimpleMotorGroup
         *
         * This method just returns whether the SimpleMotorGroup is using SpeedMode.
         * Nothing special here.
         *
         * @return true the SimpleMotorGroup is using SpeedMode
         * @return false the SimpleMotorGroup is not using SpeedMode
         */
        bool getSpeedMode();

        /**
         * @brief Sets the motor to spin at the specified command
         *
         * This method sets the motor-group's command and makes it spin according to
         * that. The command should be a percentage with 100 being the max in
         * the positive direction and -100 being the max in the negative
         * direction. Commanding the motor-group to 0 will make it stop, but if you
         * wish to control the kind of stop (coast, brake, hold), I'd recommend
         * using the .stop() method from the vex::motor_group class;
         *
         * Aditionally, if the SimpleMotorGroup currently has SpeedMode set to true,
         * the motor-group will spin at a percent of the max speed, otherwise, it will
         * simply command a percentage of the maximum of 12 volts. In some
         * scenarios, SpeedMode will allow the motor-group to maintain approximately
         * constant speed even once a load is applied.
         *
         * @param cmd The command to set the motor-group to, as a percentage of the
         * max
         *
         * @b Example:
         * @code {.cpp}
		 * SimpleMotorGroup testMotorGroup = SimpleMotorGroup( vex::motor_group(
         * 		vex::motor( vex::PORT10 ), vex::motor( vex::PORT11 ) 
		 * ) );
         * testMotorGroup.set( -75 );
         * @endcode
         */
        void set(double cmd);

        /**
         * @brief Returns the command the motor-group was last commanded to
         *
         * This method returns the command of the SimpleMotor. The command
         * defaults to 0 and is changed whenever SimpleMotorGroup::set(double cmd) is
         * called. If the motor is stopped using vex::motor_group::stop() without
         * using SimpleMotorGroup::set(0), the command will continue to be whatever
         * it last was, although the motor-group will continue to spin.
         * 
         * @todo Fix persisting motor-group command with vex::motor_group::stop();
         *
         * @return The command the motor-group is set to, as a percentage of the max
         */
        double get();

    private:
        /**
         * @brief The command the motor-group is set to, as a percentage of the max
         *
         * The command represents a percentage of the maximum voltage, 12 volts, or a
         * percentage of the motor-group's max speed, depending on whether m_speedMode
         * is set to true of false. The command is a percentage with 100 being
         * the max in the positive direction and -100 being the max in the
         * negative direction.
         *
         * Because this member variable is private, it can only be accessed
         * within the class. In order to modify or access this member, use the
         * "get" and "set" methods provided.
         */
        double m_cmd{0};

        /**
         * @brief Whether or not the motor-group should use speed, rather than voltage
         *
         * The SpeedMode simple stores whether the motor-group interprets the command
         * as a percentage of the max speed, or the precentage of the max
         * voltage, 12 volts. Using speed rather than voltage should allow the
         * motor-group to perform similarly whether or not load is applied, although
         * this can lead to some unexpected behavior if something gets jammed.
         * In most applications, using either is acceptable and safe.
         *
         * Because this member variable is private, it can only be accessed
         * within the class. In order to modify or access this member, use the
         * "get" and "set" methods provided.
         */
        bool m_speedMode{false};
    };

} // namespace ART
