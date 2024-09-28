/**
 * @file SimpleMotor.h
 * @author Jath Alison (Jath.Alison@gmail.com)
 * @brief Header containing a simple motor wrapper class
 * @version 0.1
 * @date 06-26-2024
 *
 * @copyright Copyright (c) 2024
 *
 * This is the header that needs to be included to use the SimpleMotor class.
 * The class aims to package a few motor features in an easy to use way,
 * allowing the code to be cleaner and a bit more verbose.
 */

#pragma once

#include "vex.h"

namespace art
{

    /**
     * @brief A Simple Motor Wrapper Class
     *
     * The Simple Motor class aims to provide a simpler and more efficient way
     * to command motors. The class inherits the vex::motor class and retains
     * all the same functionality, but gains quality of life functions, allowing
     * the motor's use to be as simple as get() and set().
     *
     * For more complex motor controls, such as position or velocity, use the
     * SmartMotor class (planned for a later version)
     *
     * @todo make stop() functions withbrakeModes, also set the cmd to 0
     */

    class SimpleMotor : public vex::motor
    {
    public:
        /**
         * @brief Construct a new Simple Motor object
         *
         * Uses a vex::motor (a normal v5 motor) to construct a Simple Motor
         * object
         *
         * @param mot A vex::motor, which is a normal v5 motor
         *
         * @b Example:
         * @code {.cpp}
         * SimpleMotor testMotor = SimpleMotor( vex::motor(vex::PORT10 ) );
         * @endcode
         *
         */
        SimpleMotor(vex::motor mot);

        /**
         * @brief Returns a SimpleMotor refrence with the specified speedMode
         *
         * Modifies the SimpleMotor object so that it has the speedMode
         * specified by the parameter. A refrence to the SimpleMotor is
         * returned, allowing more methods to be chained to it. This also means
         * that it can be used directly as an extension to the contructor.
         *
         * @param speedMode Whether or not the motor should use speed, rather
         * than voltage
         * @return SimpleMotor& (SimpleMotor Refrence)
         *
         * @b Example:
         * @code {.cpp}
         * SimpleMotor testMotor = SimpleMotor( vex::motor(vex::PORT10 ) );
         * testMotor.withSpeedMode( true );
         * @endcode
         *
         * @b Chained @b Methods @b Example:
         *
         * Because a refrence to the object is returned, that refrence can be
         * used to call other methods.
         *
         * @code {.cpp}
         * SimpleMotor testMotor = SimpleMotor( vex::motor(vex::PORT10 ) );
         * testMotor.withSpeedMode( true ).set( 50 );
         * @endcode
         *
         * @b Chained @b Methods @b Example:
         *
         * @warning Methods that don't return a refrence to a SimpleMotor
         * Object can't be used to create a new Object. Even though you could
         * chain .set(double cmd) onto the end, it doesn't return an object
         * refrence, so it couldn't be used to construct testMotor.
         *
         * @code {.cpp}
         * SimpleMotor testMotor = SimpleMotor( vex::motor(vex::PORT10 ) ).withSpeedMode( true );
         * @endcode
         */
        SimpleMotor &withSpeedMode(bool speedMode);

        /**
         * @brief Set the SpeedMode of the SimpleMotor
         *
         * This method simply sets the SpeedMode of the SimpleMotor. It doesn't
         * return anything, so additional methods cannot be chained to it.
         *
         * @param speedMode Whether or not the motor should use speed, rather
         * than voltage
         * 
         * @b Example:
         * @code {.cpp}
         * SimpleMotor testMotor = SimpleMotor( vex::motor(vex::PORT10 ) );
         * testMotor.setSpeedMode( false );
         * @endcode
         */
        void setSpeedMode(bool speedMode);

        /**
         * @brief Get the SpeedMode of the SimpleMotor
         *
         * This method just returns whether the SimpleMotor is using SpeedMode.
         * Nothing special here.
         *
         * @return true the SimpleMotor is using SpeedMode
         * @return false the SimpleMotor is not using SpeedMode
         */
        bool getSpeedMode();

        /**
         * @brief Sets the motor to spin at the specified command
         *
         * This method sets the motor's command and makes it spin according to
         * that. The command should be a percentage with 100 being the max in
         * the positive direction and -100 being the max in the negative
         * direction. Commanding the motor to 0 will make it stop, but if you
         * wish to control the kind of stop (coast, brake, hold), I'd recommend
         * using the .stop() method from the vex::motor class;
         *
         * Aditionally, if the SimpleMotor currently has SpeedMode set to true,
         * the Motor will spin at a percent of the max speed, otherwise, it will
         * simply command a percentage of the maximum of 12 volts. In some
         * scenarios, SpeedMode will allow the motor to maintain approximately
         * constant speed even once a load is applied.
         *
         * @param cmd The command to set the motor to, as a percentage of the
         * max
         *
         * @b Example:
         * @code {.cpp} SimpleMotor testMotor = SimpleMotor( vex::motor(vex::PORT10 ) );
         * testMotor.set( -75 );
         * @endcode
         */
        void set(double cmd);

        /**
         * @brief Returns the command the motor was last commanded to
         *
         * This method returns the command of the SimpleMotor. The command
         * defaults to 0 and is changed whenever SimpleMotor::set(double cmd) is
         * called. If the motor is stopped using vex::motor::stop() without
         * using SimpleMotor::set(0), the command will continue to be whatever
         * it last was, although the motor will continue to spin.
         * 
         * @todo Fix persisting motor command with vex::motor::stop();
         *
         * @return The command the motor is set to, as a percentage of the max
         */
        double get();

    private:
        /**
         * @brief The command the motor is set to, as a percentage of the max
         *
         * The command represents a percentage of the maximum voltage, 12 volts, or a
         * percentage of the motor's max speed, depending on whether m_speedMode
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
         * @brief Whether or not the motor should use speed, rather than voltage
         *
         * The SpeedMode simple stores whether the motor interprets the command
         * as a percentage of the max speed, or the precentage of the max
         * voltage, 12 volts. Using speed rather than voltage should allow the
         * motor to perform similarly whether or not load is applied, although
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
