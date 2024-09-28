/**
 * @file Vec2.h
 * @author Jath Alison (Jath.Alison@gmail.com)
 * @brief Header containing the 2d Vector class
 * @version 1.0-beta
 * @date 07-13-2024
 *
 * @copyright Copyright (c) 2024
 *
 * This is the header that needs to be included to use the Vec2 class. The class
 * aims to provide utilities for vectors in 2 dimentions, such as locations,
 * travel, and displacement as well as several methods to interact with these vectors.
 */

#pragma once

#include <cmath>

namespace art
{

    /**
     * @brief A Utility 2D Vector class
     *
     * The Vec2 class aims to provide a better way to store coordinates, travel
     * and displacement in a 2d space. Vec2 objects store thier x and y
     * components, but has methods to access thier direction and magnitude as
     * well. Vectors can be added together, scaled or multiplied by each other
     * to find the dot product.
     *
     * The components' units can be whatever you want, but the direction will
     * always be in radians, as the math works best that way. The Angle Unit
     * Utility Class defaults to Radians as a result, but as of right now, using
     * the Angle class is optional.
     * 
     * @todo integrate Unit Classes to Vec2
     */
    struct Vec2
    {
        /**
         * @brief Stores the X component of the Vector
         *
         * A Unitless value, so any unit can be used. However, its best to keep
         * the components and magnitude all the same unit.
         */
        double x;

        /**
         * @brief Stores the Y component of the Vector
         *
         * A Unitless value, so any unit can be used. However, its best to keep
         * the components and magnitude all the same unit.
         */
        double y;

        /**
         * @brief Construct a new Vec 2 object
         *
         * The default constructor to be used when no constructing function is
         * called. Initializes the X and Y coordinates to 0, resulting in
         * the direction and magnitude also being 0 as well.
         */
        Vec2();

        /**
         * @brief Gets the magnitude of the Vector
         *
         * @return double - The Magnitude of the Vector
         *
         * The unit returned is dependant on the units used when the vector was
         * constructed. If different units were mixed, this value is probably
         * useless.
         */
        double magnitude() const;
        /**
         * @brief Gets the direction of the Vector
         *
         * @return double - The Direction of the Vector
         *
         * The value returned is in radians, but is calculated based on the units used when the vector was
         * constructed. If different units were mixed, this value is probably
         * useless.
         */
        double direction() const;

        /**
         * @brief Returns a unit Vector with the same direction
         *
         * @return Vec2 - The normalized Vector
         *
         * The process of normalization divides a vector by it's magnitude,
         * resulting in a vector with a magnitude of 1 unit. This vector
         * preserves the original direction, so it can be useful for other
         * calculations or simply to change the scale of a vector.
         */
        Vec2 normalize() const;

        /**
         * @brief Gets the distance from this vector to another
         * 
         * @param target the vector to find the distance too
         * @return double - the distance to the other vector
         * 
         *  Finds the magnitude of the difference of 2 vectors. In practice,
         *  this is most often used to find the distance between 2 points. This
         *  method simply returns the magnitude after finding the difference
         *  between the two vectors, so if you want both the direction and
         *  magnitude, you can simply store the difference between the vectors:
         * @code {.cpp}
         * art::Vec2 difference = ending - starting;
         * @endcode
         * Both ending and starting are Vec2s as well. 
         * 
         */
        double distTo(Vec2 target) const;
        /**
         * @brief Gets the angle from this vector to another
         * 
         * @param target the vector to find the angle too
         * @return double - the angle to the other vector
         * 
         *  Finds the angle(in radians) of the difference of 2 vectors. In practice,
         *  this is most often used to find the angle connecting 2 points. This
         *  method simply returns the angle after finding the difference
         *  between the two vectors, so if you want both the direction and
         *  magnitude, you can simply store the difference between the vectors:
         * @code {.cpp}
         * art::Vec2 difference = ending - starting;
         * @endcode
         * Both ending and starting are Vec2s as well. 
         */
        double angleTo(Vec2 target) const;

        /**
         * @brief Returns the sum of 2 vectors
         * 
         * @param obj the other vector to add
         * @return Vec2 the resultant vector
         * 
         * Neither of the vectors used in this operation are modified, they
         * retain thier original values. The result must be stored or used immediately.
         */
        Vec2 operator+(Vec2 const &obj) const;
        /**
         * @brief Returns the difference of 2 vectors
         * 
         * @param obj the other vector to subtract
         * @return Vec2 the resultant vector
         * 
         * Functionally, this just flips the second vector and takes the sum.
         * Neither of the vectors used in this operation are modified, they
         * retain thier original values. The result must be stored or used immediately.
         */
        Vec2 operator-(Vec2 const &obj) const;
        /**
         * @brief Returns a vector after scaling it
         * 
         * @param scale the number to scale by
         * @return Vec2 the resultant vector
         * 
         * Functionally, this just multiplies the components by the scale. The
         * vector used in this operation ins't modified, so it retains its
         * original values. The result must be stored or used immediately.
         */
        Vec2 operator*(double const &scale) const;

        /**
         * @brief Returns the Dot Product of 2 vectors
         * 
         * @param other the other vector to calculate using
         * @return double - the Dot Product r scalar product of the 2 vectors
         * 
         * The Dot Product doesn't have that many use-cases, but it has use in
         * some more calculations for more complex things. For example in physics, it is used
         * to calculate work by using the vectors of the force and distance travelled.
         * 
         * Neither of the vectors used in this operation are modified, they
         * retain thier original values.
         * 
         */
        double operator*(Vec2 const &other) const;

        /**
         * @brief Adds another vector to this one
         * 
         * @param obj the vector to add
         * @return Vec2& - a refrence to this vector
         * 
         * This *does* modify the vector and can be used to increment one vector by another.
         */
        Vec2 &operator+=(Vec2 const &obj);
        /**
         * @brief Substracts another vector from this one
         * 
         * @param obj the vector to subtract
         * @return Vec2& - a refrence to this vector
         * 
         * This *does* modify the vector and can be used to decrement one vector by another.
         */
        Vec2 &operator-=(Vec2 const &obj);
        /**
         * @brief Changes the vector by a scale
         * 
         * @param scale the scale to multiply by
         * @return Vec2& - a refrence to this vector
         * 
         * This *does* modify the vector and can be used to scale a vector.
         */
        Vec2 &operator*=(double const &scale);

        /**
         * @brief Constructs a vector with the specified x and y components
         * 
         * @param x the x component of the vector
         * @param y the y component of the vector
         * @return Vec2 - the constructed vector
         *
         * One of 2 construction functions within the Vec2 class. If you want to
         * create a new vector with a specified x and y, you will need to call
         * this by using the accessor operator(::) and store the returned value.
         * @code {.cpp}
         * art::Vec2 vec = art::Vec2::XandY(1,2);
         * @endcode
         * 
         */
        static Vec2 XandY(double x, double y);
        /**
         * @brief Constructs a vector with the specified direction and magnitude
         * 
         * @param dir the direction of the vector(in radians)
         * @param mag the magnitude of the vector
         * @return Vec2 - the constructed vector
         *
         * One of 2 construction functions within the Vec2 class. If you want to
         * create a new vector with a specified direction(in radians) and magnitude, you will need to call
         * this by using the accessor operator(::) and store the returned value.
         * @code {.cpp}
         * art::Vec2 vec = art::Vec2::dirAndMag(M_PI_2,5);
         * @endcode
         * 
         * Understanding directions using radians can be somewhat strange if
         * you're not already familiar. The Angle Unit class can help by
         * converting degrees or revolutions to radians.
         */
        
        static Vec2 dirAndMag(double dir, double mag);
        
    };

} // namespace art
