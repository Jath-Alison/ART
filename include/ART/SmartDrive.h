/**
 * @file SmartDrive.h
 * @author Jath Alison (Jath.Alison@gmail.com)
 * @brief Header containing the SmartDrive class
 * @version 0.1
 * @date 09-01-2024
 *
 * @copyright Copyright (c) 2024
 *
 * This is the header that needs to be included to use the SmartDrive
 * class. The class aims to package more complex base controls, making it
 * easier to write autos by providing consistant high-level controls for
 * more complex tasks.
 */
#pragma once

#include "ART/Vec2.h"
#include "ART/Units.h"
#include "ART/TankDrive.h"

#include <memory>

namespace art
{

    /**
     * @brief A Smarter Drive Class
     *
     * This class is a wrapper around the TankDrive class, adding more complex functionality like
     * odometry, and a few simple(ish) controlled drive functions. All the functionality of the
     * TankDrive is retained in the SmartDrive class.
     *
     * @see TankDrive
     *
     * The SmartDrive class requires an Inertial sensor and prefers to have information on gear
     * ratio and wheel size. However, the latter is optional, although the distances the robot
     * travel will be inaccurate. Using just this, the SmartDrive can also calculate its position
     * over time using odometry. The SmartDrive class can also be configured with a
     * HorizontalTracker, which can be used to track the robot's lateral displacement in the case
     * that it has the ability to slide sideways(ie. running an all omni tank-drive). Once again,
     * this is not a requirement, but it can be useful in some cases and often increases tracking
     * resolution.
     *
     */
    class SmartDrive : public TankDrive
    {
    public:
        /**
         * @brief Construct a new Smart Drive object
         *
         * @param drive a tankDrive object representing the drivetrain
         * @param inert a vex inertial sensor object
         *
         * The SmartDrive constructor only takes and initializes the features and member variables
         * that are absolutely necessary for use, those being the drivetrain and the inertial
         * sensor. Everything else(odometry and/or horizontal tracker) can be added as optional
         * additions to the constructed SmartDrive.
         */
        SmartDrive(TankDrive drive, vex::inertial inert);

        /**
         * @brief Sets the wheel size of the Smart Drive
         *
         * @param size a Length object representing the diameter of the wheel
         * @return SmartDrive& a reference to the SmartDrive
         *
         * Sets the wheel diameter of the wheel using a Distance object, allowing any unit to be
         * used. Because it returns a reference to the SmartDrive object, other methods can be
         * chained to the end of this one, or the result can be stored into a SmartDrive object.
         *
         * The wheel size (@ref m_wheelSize "m_wheelSize") is only used for odometry, so it isn't necessary if you
         * don't care for odometry. However, it is also one of 2 requirements for basic odometry (withGearRatio()), so
         * it can be useful, as the data on the position will be at least somewhat accurate and useful.
         *
         * For example:
         * @code {.cpp}
         * art::SmartDrive smart = art::SmartDrive( drive, inert )
         *  .withWheelSize(art::Inches(2.75));
         * @endcode
         * Or
         * @code {.cpp}
         * art::SmartDrive smart2 = art::SmartDrive( drive, inert )
         *  .withWheelSize(art::Inches(2.75))
         *  .withGearRatio(3.0/4.0);
         * @endcode
         *
         */
        SmartDrive &withWheelSize(Length size);

        /**
         * @brief Sets the gear ratio of the drivetrain
         *
         * @param ratio a double representing the ratio of output to input gears(output/input)
         * @return SmartDrive& a reference to the SmartDrive
         *
         * Sets the gear ratio which can be calculated using (output teeth#)/(input teeth#). Because
         * it returns a reference to the SmartDrive object, other methods can be chained to the end
         * of this one, or the result can be stored into a SmartDrive object. 
         * Look at withWheelSize() for an example of this.
         *
         * It is advised to simply type out both tooth counts in the constructor, like
         * `60.0/36.0`. Note the ".0" added to each number to ensure that the result is a double
         * and not an integer. If it converts to an integer, then all the decimal places will be
         * truncated and the odometry will not work.
         * 
         * The gear ratio (@ref m_gearRatio "m_gearRatio") is only used for odometry, so it isn't necessary if you
         * don't care for odometry. However, it is also one of 2 requirements for basic odometry
         * (withWheelSize()), so it can be useful, as the data on the position will be at least
         * somewhat accurate and useful.
         */
        SmartDrive &withGearRatio(double ratio);

        SmartDrive &withHorizontalTracker(vex::rotation rotation, Length wheelSize, double gearRatio);
        SmartDrive &withHorizontalTracker(vex::rotation rotation, Length wheelSize, double gearRatio, Length wheelOffset);

        /**
         * @brief Continuously tracks and updates the position of the SmartDrive
         * 
         * @return int runs infinitely and will only return 0 when the task ends
         * 
         * This method exists to run in the background and update the position of the SmartDrive
         * while other things are happening. In order to do this, a <a
         * href="https://api.vex.com/v5/home/cpp/Thread.html">vex::thread</a> should be created
         * when you want tracking to start.
         * 
         * Warning:
         * Calling this somewhere else will cause the code to become stuck within this method and
         * not run anything else. Do not run this outside of a separate thread from the rest of your
         * code.
         */
        int track();

        void driveTo(Length target);
        void driveToPID(Length target);

        void turnTo(Angle target);
        void turnToPID(Angle target);

    private:
        /**
         * @brief A struct for tracking the robot's lateral displacement
         *
         * Basically just a nested class to handle a horizontal tracking wheel if the robot has one.
         * This supplements the odometry by allowing it to observe travel in another direction,
         * which can be essential to robots that can slide sideways.
         *
         * As with the odometry, the HorizontalTracker requires an Inertial as well as wheel size
         * and gear ratio. The wheel offset can also be used if the tracking wheel isn't in the
         * center of the robot, but rather in front or behind it.
         *
         * A HorizontalTracker object is kept internally in the SmartDrive class. The definition of
         * the class is private, so you can't create your own Horizontal Tracker. It is only meant
         * to be used internally within the SmartDrive class.
         *
         */
        struct HorizontalTracker
        {
            /**
             * @brief Construct an empty Horizontal Tracker object
             *
             * The default empty constructor, which is used to create an empty
             * HorizontalTracker if the robot doesn't have one or isn't
             * configured to have one. Basically, this is the default and
             * adding a functional tracker is optional.
             *
             * Because the class is private, there isn't a way to construct your own horizontal
             * tracker. This is just used internally.
             */
            HorizontalTracker();

            /**
             * @brief Construct a new Horizontal Tracker object
             *
             * @param rotation a rotation sensor to use for tracking
             * @param wheelSize a Length object representing the diameter of the wheel
             * @param gearRatio a double representing the ratio of output to input gears(output/input)
             *
             * Because the class is private, there isn't a way to construct your own horizontal
             * tracker. This is just used internally to add a tracker to the SmartDrive.
             */
            HorizontalTracker(vex::rotation rotation, Length wheelSize, double gearRatio);
            /**
             * @brief Construct a new Horizontal Tracker object
             *
             * @param rotation a rotation sensor to use for tracking
             * @param wheelSize a Length object representing the diameter of the wheel
             * @param gearRatio a double representing the ratio of output to input gears(output/input)
             * @param wheelOffset a Length object representing the vertical distance from horizontal
             * tracker to the tracking center (positive is toward the front of the robot)
             *
             * Because the class is private, there isn't a way to construct your own horizontal
             * tracker. This is just used internally to add a tracker to the SmartDrive.
             */
            HorizontalTracker(vex::rotation rotation, Length wheelSize, double gearRatio, Length wheelOffset);

            /**
             * @brief Get the distance traveled by the Horizontal Tracker
             *
             * @return Length a length object representing the distance traveled by the tracker
             *
             * Uses the difference in the encoder's position to calculate how far the robot has
             * traveled laterally. The value returned is the distance from the last getTravel()
             * call. Therefore, it should be called only within the track function and not used in
             * other places, lest it throw off the values used for tracking.
             *
             * Of course, the HorizontalTracker is stored privately by the SmartDrive, so you
             * wouldn't be able to call this unless you modified the source code.
             */
            Length getTravel();

            /**
             * @brief A Pointer to a rotation sensor object
             *
             * A pointer to a rotation sensor object. A shared pointer is kept so that the same
             * object can be stored and the pointer can be copied between objects without losing the
             * original sensor.
             *
             * Because the tracker is private, there isn't a way to access this from outside the
             * SmartDrive class. Its use is completely internal to the SmartDrive class.
             *
             * @todo check the HorizontalTracking's m_rotation can just be a normal object
             */
            std::shared_ptr<vex::rotation> m_rotation{nullptr};
            /**
             * @brief A Length representing the diameter of the wheel
             *
             * The standard Vex Wheel sizes are 2.75, 3,25 and 4 inches. As with any other Length,
             * you are free to use any unit you wish, but keep in mind that this is the *Diameter*
             * and not the travel of the wheel.
             *
             * Defaults to 2.75 inches, but the set-up function attached to the SmartDrive
             * constructor will make you input your own wheel size anyway.
             *
             * @see withHorizontalTracker
             */
            Length m_wheelSize{Inches(2.75)};
            /**
             * @brief A double representing the gear ratio of the tracker
             *
             * If the Tracker has a gear ratio between the encoder and the wheel, a gear ratio will
             * need to be entered to convert the encoder values to wheel travel. The gear ratio is a
             * number that can be calculated using (output teeth#)/(input teeth#).
             *
             * It is advised to simply type out both tooth counts in the constructor, like
             * `60.0/36.0`. Note the ".0" added to each number to ensure that the result is a double
             * and not an integer. If it converts to an integer, then all the decimal places will be
             * truncated and the odometry will not work.
             */
            double m_gearRatio{1.0};

            /**
             * @brief Stores the last angle recorded from the encoder
             *
             * This is used to calculate the difference in angle, m_travel angle, which is then used
             * to calculate the distance traveled, m_travelDistance.
             */
            Angle m_lastAngle = Angle(0);
            /**
             * @brief Stores the change in angle since the last call of getTravel()
             *
             * This is calculated using m_lastAngle and helps to calculate the distance traveled, m_travelDistance.
             */
            Angle m_travelAngle = Angle(0);
            /**
             * @brief Stores the distance traveled
             *
             * This is calculated using m_travelAngle as well as the information about the wheel
             * size from m_wheelSize. Then it stores a Length
             */
            Length m_travelDistance = Length(0);

            /**
             * @brief Stores a Length representing the vertical distance from horizontal
             * tracker to the tracking center (positive is toward the front of the robot)
             *
             */
            Length m_offset = Length(0);
        };

        /**
         * @brief Stores the Horizontal Tracker
         *
         * Only used internally, as it is a private instance. This handles the calculations for the
         * lateral displacement of the robot.
         */
        HorizontalTracker m_tracker;

        /**
         * @brief Stores the Position of the robot
         *
         * Stores a Vec2(2-dimensional vector) storing the position of the robot. This is the
         * position that is used and incremented constantly by the tracking function. On the robot,
         * this position is found in the center of the robot, or where the HorizontalTracker is, if
         * you have one.
         */
        Vec2 m_pos;
        /**
         * @brief Stores the Center Position of the robot
         *
         * Uses the HorizontalTracker's offset to calculate and store where the center of the robot would be.
         * If there isn't a tracker, this will be the same as m_pos, since that will store the
         * center of the robot if there isn't a tracker.
         */
        Vec2 m_centerPos;
        /**
         * @brief Stores the current heading of the robot
         *
         * An Angle representing the direction the robot is currently facing. Taken directly from
         * the inertial sensor and used to calculate odometry. This value is still used outside the
         * odometry tracking including the turning functions and some drive functions.
         *
         * This value is mostly internal, the getDir() method returns a value directly from the
         * inertial sensor and doesn't necessarily return this value, although they are often the same.
         */
        Angle m_dir;

        /**
         * @brief Stores the inertial sensor object
         *
         * Used to observe the heading of the robot. The inertial sensor added to the SmartDrive
         * isn't removed or deleted(this is basically just a copy), so it can still be used outside the class. However, because
         * this inertial sensor instance is private, it can't be accessed from outside the
         * SmartDrive class.
         */
        vex::inertial m_inert;

        /**
         * @brief A Length representing the diameter of the wheel
         *
         * The standard Vex Wheel sizes are 2.75, 3,25 and 4 inches. As with any other Length,
         * you are free to use any unit you wish, but keep in mind that this is the *Diameter*
         * and not the travel of the wheel.
         *
         * Defaults to 3.25 inches, but you can(and should) attach withWheelSize to the constructor
         * and set the wheel size yourself.
         *
         * @see withWheelSize
         */
        Length m_wheelSize{Inches(3.25)};
        /**
         * @brief A double representing the gear ratio of the drivetrain
         *
         * If the Base has a gear ratio between the motors and the wheels, a gear ratio will
         * need to be entered to convert the motor's encoder values to wheel travel. The gear ratio is a
         * number that can be calculated using (output teeth#)/(input teeth#).
         *
         * It is advised to simply type out both tooth counts in the constructor, like
         * `60.0/36.0`. Note the ".0" added to each number to ensure that the result is a double
         * and not an integer. If it converts to an integer, then all the decimal places will be
         * truncated and the odometry will not work.
         */
        double m_gearRatio{1.0};
    };

} // namespace art
