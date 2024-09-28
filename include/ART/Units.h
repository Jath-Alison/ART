/**
 * @file Units.h
 * @author Jath Alison (Jath.Alison@gmail.com)
 * @brief Header containing several Unit classes
 * @version 0.1
 * @date 07-13-2024
 *
 * @copyright Copyright (c) 2024
 *
 * This is the header that needs to be included to use the various Unit classes.
 * The classes aim to aid in dealing with angles and distances by providing a
 * standard form for other classes to use. That way, any of the other units can
 * be used and converted rather than writing seperate functions for different
 * units.
 */
#pragma once
#include <cmath>

namespace art
{
    /**
     * @brief A Utility Unit class for Length
     * 
     * The Length class aims to provide a better way to convert and store
     * different units for length. This way, when a function asks for a distance
     * or a length to drive, any unit can be used to specify the distance. This
     * makes it easier to write and tune autons as well as perform calculations,
     * as you can use whatever unit is most comfortable.
     * 
     * The Pixel unit is leftover from a Vex simulation project I started. I
     * built the Length class around the pixels in the sim, making the pixel the
     * default/internal unit. There isn't any purpose to using them, but it does make it
     * so that if a unit is not specified, the results can be quite unexpected.
     * @todo remove the Pixel unit and choose a different unit as the default/internal unit
     */
    class Length
    {
    public:
        /**
         * @brief Constructs a Length from Pixels
         * 
         * @param pixels the length in Pixels
         * @return Length - the constructed Length
         */
        friend Length Pixels(double pixels);
        /**
         * @brief Constructs a Length from Inches
         * 
         * @param inches the length in Inches
         * @return Length - the constructed Length
         */
        friend Length Inches(double inches);
        /**
         * @brief Constructs a Length from Feet
         * 
         * @param feet the length in Feet
         * @return Length - the constructed Length
         */
        friend Length Feet(double feet);
        /**
         * @brief Constructs a Length from Meters
         * 
         * @param meters the length in Meters
         * @return Length - the constructed Length
         */
        friend Length Meters(double meters);
        /**
         * @brief Constructs a Length from Centimeters
         * 
         * @param centimeters the length in Centimeters
         * @return Length - the constructed Length
         */
        friend Length Centimeters(double centimeters);
        /**
         * @brief Constructs a Length from Millimeters
         * 
         * @param millimeters the length in Millimeters
         * @return Length - the constructed Length
         */
        friend Length Millimeters(double millimeters);
        /**
         * @brief Constructs a Length from Tiles
         * 
         * @param tiles the length in Tiles
         * @return Length - the constructed Length
         */
        friend Length Tiles(double tiles);

        /**
         * @brief Returns the Length in pixels
         * 
         * @return double - the Length in pixels
         */
        double pixels();
        /**
         * @brief Returns the Length in inches
         * 
         * @return double - the Length in inches
         */
        double inches();
        /**
         * @brief Returns the Length in feet
         * 
         * @return double - the Length in feet
         */
        double feet();
        /**
         * @brief Returns the Length in meters
         * 
         * @return double - the Length in meters
         */
        double meters();
        /**
         * @brief Returns the Length in centimeters
         * 
         * @return double - the Length in centimeters
         */
        double centimeters();
        /**
         * @brief Returns the Length in millimeters
         * 
         * @return double - the Length in millimeters
         */
        double millimeters();
        /**
         * @brief Returns the Length in tiles
         * 
         * @return double - the Length in tiles
         */
        double tiles();

        /**
         * @brief Construct a new Length object
         * 
         * Initializes the value to 0. 
         */
        Length();
        /**
         * @brief Construct a new Length object
         * 
         * @param f - a number(or Length) to initialize m_value to
         * 
         * This constructor is used similarly to the assignment operator. It can
         * be used to construct a Length to copy the value of another length
         * without needing to convert to and from a specified unit. In order
         * words, instead of a number, another Length can be used and/or
         * modified by treating it as a number.
         * 
         * As of right now, this number(f) is in Pixels, which is also the
         * default output when a Length is used as a number.
         */
        Length(double f);

        /**
         * @brief Assign a value to the Length object
         * 
         * @param f - a number(or Length) to initialize m_value to
         * @return Length - The new modified length
         * 
         * This overloaded assignment operator allows the result of an operation
         * using a Length to be stored by another Length.
         * 
         * Keep in mind this works best when the value on the right side is a
         * Length. Modifying the Length with operators is okay, but keep in mind
         * how those operations distribute.
         * @code {.cpp}
         * art::Length length1 = art::Inches(5) + 5; //stores a length of 6 inches(don't do this)
         * art::Length length2 = art::Inches(5) + art::Inches(5); //stores a length of 10 inches(do this instead)
         * art::Length length3 = art::Inches(5 + 5); //stores a length of 10 inches(this is best)
         * 
         * art::Length length4 = art::Inches(5) * 5; //stores a length of 25 inches
         * @endcode
         * 
         * As of right now, this number(f) is in Pixels, which is also the
         * default output when a Length is used as a number.
         */
        Length operator=(double const &f);

        /**
         * @brief Returns the Length as a default value(Pixels)
         * 
         * @return double - the Length as a default value(Pixels)
         * 
         * This operator overload allows Length to be treated as a plain old
         * number. I can be typecasted to a double explicitly or just used in an
         * expression. The value returned is technically in Pixels, but when
         * adding different lengths together or multiplying by a scale, this
         * doesn't matter as long as the result is stored using Length(double f)
         * or Length operator=(double const &f), as oppsosed to a
         * Unit-specifying constructing function (Inches, Tiles, Meters etc.).
         * @code {.cpp}
         * art::Length length = art::Inches(5) * 5 + art::Feet(2); //stores a length of 49 (25+24) inches
         * @endcode
         * 
         * Normal C++ order of operations applies, as each Length is simply replaced by it's underlying value.
         */
        operator double();

    private:
        /**
         * @brief The underlying value of Length
         * 
         * Stores the Length in pixels. This is what is returned and modified by
         * some methods, but this is also converted to and from other units
         * using the corresponding constructing function or get method.
         */
        double m_value{0.0};
    };

    /**
     * @brief Constructs a Length from Pixels
     * 
     * @param pixels the length in Pixels
     * @return Length - the constructed Length
     */
    Length Pixels(double pixels);
    /**
     * @brief Constructs a Length from Inches
     * 
     * @param inches the length in Inches
     * @return Length - the constructed Length
     */
    Length Inches(double inches);
    /**
     * @brief Constructs a Length from Feet
     * 
     * @param feet the length in Feet
     * @return Length - the constructed Length
     */
    Length Feet(double feet);
    /**
     * @brief Constructs a Length from Meters
     * 
     * @param meters the length in Meters
     * @return Length - the constructed Length
     */
    Length Meters(double meters);
    /**
     * @brief Constructs a Length from Centimeters
     * 
     * @param centimeters the length in Centimeters
     * @return Length - the constructed Length
     */
    Length Centimeters(double centimeters);
    /**
     * @brief Constructs a Length from Millimeters
     * 
     * @param millimeters the length in Millimeters
     * @return Length - the constructed Length
     */
    Length Millimeters(double millimeters);
    /**
     * @brief Constructs a Length from Tiles
     * 
     * @param tiles the length in Tiles
     * @return Length - the constructed Length
     */
    Length Tiles(double tiles);

    /**
     * @brief A Utility Unit class for Angles
     * 
     * The Length class aims to provide a better way to convert and store
     * different units for angles. This way, when a function asks for a
     * direction to turn to, any unit can be used to specify the angle. This
     * makes it easier to write and tune autons as well as perform calculations,
     * as you can use whatever unit is most comfortable.
     * 
     * The Default/Internal unit for Angles is radians, since all the
     * trignometric functions included in <cmath> use radians by default. This
     * makes calculations easier and allows for the Angle class to be used to
     * easily specify direction for the Vec2 class.
     */ 
    class Angle
    {
    public:
        /**
         * @brief Constructs an Angle from Degrees
         * 
         * @param degrees the angle in Degrees
         * @return Angle - the constructed Angle
         */
        friend Angle Degrees(double degrees);
        /**
         * @brief Constructs an Angle from Radians
         * 
         * @param radians the angle in Radians
         * @return Angle - the constructed Angle
         */
        friend Angle Radians(double radians);
        /**
         * @brief Constructs an Angle from Revolutions
         * 
         * @param revolutions the angle in Revolutions
         * @return Angle - the constructed Angle
         */
        friend Angle Revolutions(double revolutions);

        /**
         * @brief Returns the Angle in degrees
         * 
         * @return double - the Angle in degrees
         */
        double degrees();
        /**
         * @brief Returns the Angle in radians
         * 
         * @return double - the Angle in radians
         */
        double radians();
        /**
         * @brief Returns the Angle in revolutions
         * 
         * @return double - the Angle in revolutions
         */
        double revolutions();

        /**
         * @brief Construct a new Angle object
         * 
         * Initializes the value to 0. 
         */
        Angle();
        /**
         * @brief Construct a new Angle object
         * 
         * @param f - a number(or Angle) to initialize m_value to
         * 
         * This constructor is used similarly to the assignment operator. It can
         * be used to construct an Angle to copy the value of another length
         * without needing to convert to and from a specified unit. In order
         * words, instead of a number, another Angle can be used and/or
         * modified by treating it as a number.
         * 
         * The number(f) is in radians, which is also the
         * default output when a Angle is used as a number.
         */
        Angle(double f);

        /**
         * @brief Constrains the Angle to the -180 to 180 range
         * 
         * @todo add seperate constrains for 0 to 360 and -180 to 180 (this one is -180 to 180)
         * 
         * Wraps the Angle around until it fits within a single circle. This
         * preserves the direction, but prevents over-representing the value.
         * This can be used in places to optimize turns and rotations, but is
         * mostly just utility should you want to reduce an angle while
         * preserving the absolute direction.
         */
        void constrain();

        /**
         * @brief Assign a value to the Angle object
         * 
         * @param f - a number(or Angle) to initialize m_value to
         * @return Angle - The new modified Angle
         * 
         * This overloaded assignment operator allows the result of an operation
         * using an Angle to be stored by another Angle.
         * 
         * Keep in mind this works best when the value on the right side is an
         * Angle. Modifying the Angle with operators is okay, but keep in mind
         * how those operations distribute.
         * @code {.cpp}
         * art::Angle angle1 = art::Degrees(5) + 5; //stores an angle of ~ 291 inches(don't do this)
         * art::Angle angle2 = art::Degrees(5) + art::Degrees(5); //stores an angle of 10 degrees(do this instead)
         * art::Angle angle3 = art::Degrees(5 + 5); //stores an angle of 10 degrees(this is best)
         * 
         * art::Angle angle4 = art::Degrees(5) * 5; //stores an angle of 25 degrees
         * @endcode
         * 
         * As of right now, this number(f) is in radians, which is also the
         * default output when a Angle is used as a number.
         */
        Angle operator=(double const &f);
        /**
         * @brief Returns the Angle as a default value(Radians)
         * 
         * @return double - the Angle as a default value(Radians)
         * 
         * This operator overload allows Angle to be treated as a plain old
         * number. It can be typecasted to a double explicitly or just used in
         * an expression. The value returned is technically in radians, but so
         * long as the result is stored using Angle(double f) or Angle
         * operator=(double const &f), as oppsosed to a Unit-specifying
         * constructing function (Inches, Tiles, Meters etc.), it shouldn't be
         * that important.
         * @code {.cpp}
         * art::Angle angle = art::Degrees(5) * 5 + art::Revolutions(1); //stores an Angle of 385 (25+360) degrees
         * @endcode
         * 
         * Normal C++ order of operations applies, as each Angle is simply replaced by it's underlying value.
         */
        operator double();

    private:
        /**
         * @brief The underlying value of the Angle
         * 
         * Stores the Angle in Radians. This is what is returned and modified by
         * some methods, but this is also converted to and from other units
         * using the corresponding constructing function or get method.
         */
        double m_value{0.0};
    };

    /**
     * @brief Constructs an Angle from Degrees
     * 
     * @param degrees the angle in Degrees
     * @return Angle - the constructed Angle
     */
    Angle Degrees(double degrees);
    /**
     * @brief Constructs an Angle from Radians
     * 
     * @param radians the angle in Radians
     * @return Angle - the constructed Angle
     */
    Angle Radians(double radians);
    /**
     * @brief Constructs an Angle from Revolutions
     * 
     * @param revolutions the angle in Revolutions
     * @return Angle - the constructed Angle
     */
    Angle Revolutions(double revolutions);

    /**
     * @brief Returns the shortest turn path to reach the target angle
     * 
     * @param target - an Angle to reach
     * @return Angle - the shortest angle(with direction) to the target angle
     */
    Angle shortestTurnPath(const Angle target);

} // namespace art
