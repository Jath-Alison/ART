/**
 * @file SmartDrive.h
 * @author Jath Alison (Jath.Alison@gmail.com)
 * @brief Header containing the SmartDrive class
 * @version 1.0-beta
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
#include "ART/PID.h"
#include "ART/Units.h"
#include "ART/TankDrive.h"

#include <memory>
#include <cmath>

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

        /**
         * @brief Add Horizontal Tracker to the SmartDrive
         *
         * @param rotation a rotation sensor to use for tracking
         * @param wheelSize a Length object representing the diameter of the wheel
         * @param gearRatio a double representing the ratio of output to input gears(output/input)
         * @return SmartDrive& a reference to the SmartDrive
         *
         * While the HorizontalTracker is store privately in the SmartDrive class, one can be added
         * to the SmartDrive by calling this method. It is recommended to attach this to the end of
         * the constructor, like so:
         *
         * @code {.cpp}
         * art::SmartDrive smart = art::SmartDrive( drive, inert )
         *  .withWheelSize(art::Inches(2.75))
         *  .withHorizontalTracker(
         *       vex::rotation(vex::PORT2, true),
         *       art::Inches(2.75),
         *       3.f/5.f
         *  );
         * @endcode
         *
         * @see HorizontalTracker(vex::rotation rotation, Length wheelSize, double gearRatio)
         * HorizontalTracker(vex::rotation rotation, Length wheelSize, double gearRatio, Length wheelOffset)
         *
         */
        SmartDrive &withHorizontalTracker(vex::rotation rotation, Length wheelSize, double gearRatio);

        /**
         * @brief Add a Horizontal Tracker to the SmartDrive with a wheel offset
         *
         * @param rotation a rotation sensor to use for tracking
         * @param wheelSize a Length object representing the diameter of the wheel
         * @param gearRatio a double representing the ratio of output to input gears(output/input)
         * @param wheelOffset a Length object representing the vertical distance from horizontal
         * tracker to the tracking center (positive is toward the front of the robot)
         * @return SmartDrive& a reference to the SmartDrive
         *
         * While the HorizontalTracker is store privately in the SmartDrive class, one can be added
         * to the SmartDrive by calling this method. It is recommended to attach this to the end of
         * the constructor, like so:
         *
         * @code {.cpp}
         * art::SmartDrive smart = art::SmartDrive( drive, inert )
         *  .withWheelSize(art::Inches(2.75))
         *  .withHorizontalTracker(
         *       vex::rotation(vex::PORT2, true),
         *       art::Inches(2.75),
         *       3.f/5.f,
         *       art::Inches(1.5)
         *  );
         * @endcode
         *
         * @see HorizontalTracker(vex::rotation rotation, Length wheelSize, double gearRatio, Length wheelOffset)
         *
         */
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

        /**
         * @brief Drives for a specified distance
         *
         * @param target the distance to drive for
         * @param speed the speed to drive at
         *
         * Utilizes the motor encoders as well as information about the wheelSize and gear ratio to
         * travel a specified distance. Use a Length object to specify the distance in any unit. The
         * speed is a number from -100 to 100 as a percentage of max voltage(12). Setting the speed
         * to 100 and -100 commands the motors at 12 and -12 volts respectively.
         *
         * The SmartDrive will drive at this speed until it passes the target distance threshold,
         * immediately exiting. This method is blocking, so the function where this is called will
         * not continue until this is finished running. Keep in mind that if the speed makes the
         * robot travel in the incorrect direction, it will never reach its target.
         *
         * The SmartDrive will not slow down as it approaches the target either. If you want to to
         * stop precisely at a target distance, it may be a good idea to divide the distance into
         * smaller chunks and slow down the latter chunks so it slows as it approaches the overall
         * target.
         *
         * Example:
         * @code {.cpp}
         * smartDrive.driveFor(art::Length::tiles(2), 75);
         * @endcode
         */
        void driveFor(Length target, double speed);

        /**
         * @brief Drives for a specified distance (and stops)
         *
         * @param target the distance to drive for
         *
         * Performs a similar function to driveFor(), but utilizes a PID loop to better control the
         * SmartDrive's motion. Assuming the PID is tuned correctly, the SmartDrive will slow down
         * and stop at the target distance. Both the speed and direction will be calculated to reach
         * this distance.
         *
         * To see how the PID runs, take a look at the PID class.
         *
         * The PID object for this method can be configured by modifying @ref driveForPID.
         */
        void driveForPID(Length target);

        /**
         * @brief The PID object for the @ref driveFor Method
         *
         * Modifying this PID object will change how @ref driveFor works.
         *
         * Wherever you configure your robot, it is recommended to configure this as well. You can
         * simply set this PID object equal to another defined PID object, but this code will need
         * to run in the pre_auton or main functions. To put it with the rest of the Robot
         * Configuration, use the withDriveForPID() method.
         *
         * @see PID
         */
        PID m_driveForPID;

        /**
         * @brief Sets the PID object for the @ref driveForPID Method
         *
         * @param pid the PID object to copy
         * @return SmartDrive& a reference to the SmartDrive
         *
         * This method allows you to set the PID object for the @ref driveForPID method. It can be
         * set using another PID object or one that is constructed inline like so:
         *
         * @code {.cpp}
         * smartDrive.withDriveForPID(
         *  art::PID().withConstants(3.0, 0.2, 1.0)
         * );
         * @endcode
         *
         * Take a look at the documentation for the PID class for more information on how to
         * configure the PID object.
         */
        SmartDrive &withDriveForPID(PID pid);

        /**
         * @brief Turns for a specified Angle
         *
         * @param target the angle to turn for
         * @param speed the speed to turn at
         *
         * Utilizes the inertial sensor's gyro to turn a specified Angle. Use an Angle object to
         * specify the angler in any unit. The speed is a number from -100 to 100 as a percentage of
         * max voltage(12). Setting the speed to 100 commands the left and right motors at 12 and -12
         * volts respectively and -100 flips those values.
         *
         * The SmartDrive will turn at this speed until it passes the target angle threshold,
         * immediately exiting. This method is blocking, so the function where this is called will
         * not continue until this is finished running. Keep in mind that if the speed makes the
         * robot travel in the incorrect direction, it will never reach its target.
         *
         * @todo Check if the robot will wrap around and stop.
         *
         * The SmartDrive will not slow down as it approaches the target either. If you want to to
         * stop precisely at a target angle, it may be a good idea to divide the distance into
         * smaller chunks and slow down the latter chunks so it slows as it approaches the overall
         * target.
         *
         * Example:
         * @code {.cpp}
         * smartDrive.turnFor(art::Angle::Rotations(2), 75);
         * @endcode
         */
        void turnFor(Angle target, double speed);

        /**
         * @brief Turns for a specified angle (and stops)
         *
         * @param target the angle to turn for
         *
         * Performs a similar function to turnFor(), but utilizes a PID loop to better control the
         * SmartDrive's motion. Assuming the PID is tuned correctly, the SmartDrive will slow down
         * and stop at the target Angle. Both the speed and direction will be calculated to reach
         * this angle.
         *
         * To see how the PID runs, take a look at the PID class.
         *
         * The PID object for this method can be configured by modifying @ref turnForPID.
         */
        void turnForPID(Angle target);

        /**
         * @brief The PID object for the @ref turnForPID Method
         *
         * Modifying this PID object will change how @ref turnForPID works.
         *
         * Wherever you configure your robot, it is recommended to configure this as well. You can
         * simply set this PID object equal to another defined PID object, but this code will need
         * to run in the pre_auton or main functions. To put it with the rest of the Robot
         * Configuration, use the withTurnForPID() method.
         *
         * @see PID
         */
        PID m_turnForPID;

        /**
         * @brief Sets the PID object for the @ref turnForPID Method
         *
         * @param pid the PID object to copy
         * @return SmartDrive& a reference to the SmartDrive
         *
         * This method allows you to set the PID object for the @ref turnForPID method. It can be
         * set using another PID object or one that is constructed inline like so:
         *
         * @code {.cpp}
         * smartDrive.withTurnForPID(
         *  art::PID().withConstants(3.0, 0.2, 1.0)
         * );
         * @endcode
         *
         * Take a look at the documentation for the PID class for more information on how to
         * configure the PID object.
         */
        SmartDrive &withTurnForPID(PID pid);

        /**
         * @brief Turns to a specified field-centric Angle
         *
         * @param target the angle to turn to
         * @param speed the speed to turn at
         *
         * Utilizes the inertial sensor's gyro to turn to a specified Angle on the field. Use an Angle object to
         * specify the angler in any unit. The speed is a number from -100 to 100 as a percentage of
         * max voltage(12). Setting the speed to 100 commands the left and right motors at 12 and -12
         * volts respectively and -100 flips those values.
         *
         * The SmartDrive will turn at this speed until it falls within the target angle threshold,
         * immediately exiting. This method is blocking, so the function where this is called will
         * not continue until this is finished running. Keep in mind that depending the speed, the
         * robot may rotate and reach the angle using the longer route.
         *
         * The Angle will be wrapped around and constrained to a circle. Therefore, even if the
         * angle resides outside a circle, like 540 degrees, it will be wrapped around to 180
         * degrees instead. Unless the gyro is reset or re-calibrated, the angle specified will
         * always be relative to the initial gyro reading. Therefore, all angles will be relative to
         * the position when the robot is powered on. It is possible to power the robot on and move
         * it to the starting position, but if you do this, keep in mind where the starting location
         * is set in the code and ensure the odometry is not confused by this process.
         *
         * The SmartDrive will not slow down as it approaches the target, so if you want to to
         * stop precisely at a target angle, it may be a good idea to divide the turn into
         * smaller chunks and slow down the latter chunks so it slows as it approaches the overall
         * target.
         *
         * Example:
         * @code {.cpp}
         * smartDrive.turnTo(art::Angle::Degrees(90), 75);
         * @endcode
         */
        void turnTo(Angle target, double speed);

        /**
         * @brief Turns tp a specified angle (and stops)
         *
         * @param target the angle to turn to
         *
         * Performs a similar function to turnTo(), but utilizes a PID loop to better control the
         * SmartDrive's motion. Assuming the PID is tuned correctly, the SmartDrive will slow down
         * and stop at the target Angle. Both the speed and direction will be calculated to reach
         * this angle.
         *
         * To see how the PID runs, take a look at the PID class.
         *
         * The PID object for this method can be configured by modifying @ref turnToPID.
         */
        void turnToPID(Angle target);

        /**
         * @brief The PID object for the @ref turnToPID Method
         *
         * Modifying this PID object will change how @ref turnToPID works.
         *
         * Wherever you configure your robot, it is recommended to configure this as well. You can
         * simply set this PID object equal to another defined PID object, but this code will need
         * to run in the pre_auton or main functions. To put it with the rest of the Robot
         * Configuration, use the withTurnToPID() method.
         *
         * @see PID
         */
        PID m_turnToPID;

        /**
         * @brief Sets the PID object for the @ref turnToPID Method
         *
         * @param pid the PID object to copy
         * @return SmartDrive& a reference to the SmartDrive
         *
         * This method allows you to set the PID object for the @ref turnToPID method. It can be
         * set using another PID object or one that is constructed inline like so:
         *
         * @code {.cpp}
         * smartDrive.withTurnToPID(
         *  art::PID().withConstants(3.0, 0.2, 1.0)
         * );
         * @endcode
         *
         * Take a look at the documentation for the PID class for more information on how to
         * configure the PID object.
         */
        SmartDrive &withTurnToPID(PID pid);

        /**
         * @brief Get the Travel of the wheels for one revolution of the motor 
         * 
         * @return Distance that would be traveled if the base motor were turned
         * onces
         * 
         * Calculates and returns the distance that would be traveled if the base motor were turned
         * once. This is used to calculate the odometry and some drive functions.
         */
        Length getWheelTravel();

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

        /**
         * @brief Gets the Angle traveled by the left motors
         * 
         * @return angle the angle the left motors have traveled through
         * 
         * Only used internally, inaccessible externally. Used for odometry and some drive
         * functions.
         */
        Angle getLeftTravel();

        /**
         * @brief The last angle read from the Left motors
         * 
         * Used internally to calculate the travel of the motors and the base
         */
        Angle m_LastLeftPos;

        /**
         * @brief Gets the Angle traveled by the left motors
         * 
         * @return angle the angle the left motors have traveled through
         * 
         * Only used internally, inaccessible externally. Used for odometry and some drive
         * functions.
         */
        Angle getRightTravel();

        /**
         * @brief The last angle read from the Right motors
         * 
         * Used internally to calculate the travel of the motors and the base
         */
        Angle m_LastRightPos;

    };

} // namespace art
