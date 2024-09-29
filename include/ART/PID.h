/**
 * @file PID.h
 * @author Jath Alison (Jath.Alison@gmail.com)
 * @brief Header containing a general use PID class
 * @version 1.0-beta
 * @date 07-01-2024
 *
 * @copyright Copyright (c) 2024
 *
 * This is the header that needs to be included to use the PID class. The class
 * aims to package PID functions together to be used across different
 * applications.
 */

#pragma once
#include <cmath>
#include <chrono>

namespace art
{
    /**
     * @brief A Point in time
     *
     * A TimePoint is just an alias for
     * std::chrono::time_point<std::chrono::high_resolution_clock>. TimePoint is
     * just way easier to type and makes the code more readable
     */
    typedef std::chrono::time_point<std::chrono::high_resolution_clock> TimePoint;

    /**
     * @brief A general use PID class
     *
     * A PID controller class. The class bundles all the functionality of a PID
     * controller into a single class so it can simply be fed inputs and output
     * values back.
     * 
     * Most often, the PID class will be used like this:
     * 
     * @code {.cpp}
     * art::PID pid = art::PID().withConstants(2, 10, -5)
                                .withIntegralZone(10)
                                .withTimeout(2)
                                .withSettleZone(3)
                                .withSettleTimeout(0.25);
        while( pid.isCompleted() == false ){
            double out = pid.calculate( input, target );
            // do soemthing with output
            vex::wait(20, vex::msec);
        }
     * @endcode
     * First, the object is constructed and set up with certain parameters.
     * Then, input values can be fed in from sensors or some other feedback
     * device while feeding the output to a motor or some other thing to produce
     * the desired outcome. This can be repeated until the conditions the PID is
     * set to are complete, or simply as long as you want the PID to move toward
     * it's target.
     */
    class PID
    {
    public:
        /**
         * @brief Constructs a new PID object
         *
         * Takes no parameters, so a new object can be constructed either by
         * simply defining it or by defining an object and using the assignment
         * operator (the equals sign =) to assign a newly constructed object to
         * it.
         * @code {.cpp}
         * art::PID pid; //declare a PID object
         * art::PID pid2 = art::PID(); //declare a PID object and assign a new PID to it
         * @endcode
         * 
         * The second option is recommended because calling the constructor
         * returns a refrence to a PID object. As long as a PID object refrence
         * is on the right side of the equals sign, the PID object on the left
         * will copy the attributes of the refrence. This allows methods that
         * return a refrence to a PID obect to be chained to the constructor.
         * These methods all start with "with" and can also be chained to each
         * other.
         * 
         * @b Constructor @b with @b chained @b methods:
         * 
         * @code {.cpp}
         * art::PID pid2 = art::PID().withConstants(2, 10, -5)
                                    .withIntegralZone(10)
                                    .withTimeout(2)
                                    .withSettleZone(3)
                                    .withSettleTimeout(0.25);
         * @endcode
         * 
         * @note The constants default to 1(m_kp), 0(m_ki), 0(m_kd) and 0(m_ff)
         */
        PID();

        /**
         * @brief Resets the PID object
         *
         * Once reset, the PID's derivative and integral terms are set back to
         * 0. Both the error and previous-error are also set to 0 and both
         *    TimePoints are reset to the present.
         *
         * By doing this, the PID will be ready to be used from a new starting
         * state. If the PID is not reset, it may not function properly, as it
         * may thing it is finished when it is not, or the integral term will
         * persist and may cause the output to be unpredictable.  
         */
        void reset();

        /**
         * @brief Sets the PID constants
         *
         * @param kp a new value for m_kp
         * @param ki a new value for m_ki
         * @param kd a new value for m_kd
         *
         * Sets the values for m_kp, m_ki and m_kd using the parameters.
         */
        void setConstants(double kp, double ki, double kd);
        /**
         * @brief Sets the PID constans and returns a refrence to the PID object
         *
         * @param kp a new value for m_kp
         * @param ki a new value for m_ki
         * @param kd a new value for m_kd
         * @return PID& (a PID refrence)
         *
         * Sets the values for m_kp, m_ki and m_kd using the parameters. Then,
         * returns a refrence to the PID object, so that more methods can be
         * chained and it can be assigned to an object using the assignment
         * operator (=).
         * 
         * @code {.cpp}
         * art::PID pid = art::PID().withConstants(2, 10, -5)
         *                          .withTimeout(2);
         * @endcode
         * 
         */
        PID &withConstants(double kp, double ki, double kd);

        /**
         * @brief Sets the PID constants
         *
         * @param kp a new value for m_kp
         * @param ki a new value for m_ki
         * @param kd a new value for m_kd
         * @param ff a new value for m_ff
         *
         * Sets the values for m_kp, m_ki, m_kd, and ff using the parameters.
         */
        void setConstants(double kp, double ki, double kd, double ff);
        /**
         * @brief Sets the PID constans and returns a refrence to the PID object
         *
         * @param kp a new value for m_kp
         * @param ki a new value for m_ki
         * @param kd a new value for m_kd
         * @param ff a new value for m_ff
         * @return PID& (a PID refrence)
         *
         * Sets the values for m_kp, m_ki, m_kd, and ff using the parameters. Then,
         * returns a refrence to the PID object, so that more methods can be
         * chained and it can be assigned to an object using the assignment
         * operator (=).
         * 
         * @code {.cpp}
         * art::PID pid = art::PID().withConstants(2, 10, -5, 1)
         *                          .withTimeout(2);
         * @endcode
         * 
         */
        PID &withConstants(double kp, double ki, double kd, double ff);

        /**
         * @brief Returns the value of m_kp
         * 
         * @return double (the value of m_kp)
         */
         double getkp();
        /**
         * @brief Returns the value of m_ki
         * 
         * @return double (the value of m_ki)
         */
         double getki();
        /**
         * @brief Returns the value of m_kd
         * 
         * @return double (the value of m_kd)
         */
         double getkd();
        /**
         * @brief Returns the value of m_ff
         * 
         * @return double (the value of m_ff)
         */
         double getff();

        /**
         * @brief Sets the range within which the error will be applied to the
         * integral term
         *
         * @param integralZone a positive number representing the highest absolute value of the error applied to the integral term
         *
         * This method sets m_integralZone which is the range of the error that
         * the integral will be applied using. Setting the integral zone to 0
         * will always apply the error to the integral.
         *
         * The inputted number should be positive beacuse it is compared with
         * the absolute value of the error. If the integralZone is set to 5, the
         * error will be applied when it is between -5 and 5.
         */
        void setIntegralZone(double integralZone);
        /**
         * @brief Set range within which the error will be applied to the
         * integral term and returns a PID refrence
         *
         * @param integralZone a positive number representing the highest absolute value of the error applied to the integral term
         *
         * @return PID& (a PID refrence)
         *
         * Calls setIntegralZone(double integralZone), but also returns a
         * refrence to the PID object, so that more methods can be chained and
         * it can be assigned to an object using the assignment operator (=).
         * 
         * @code {.cpp}
         * art::PID pid = art::PID().withConstants(2, 10, -5)
         *                          .withIntegralZone(2);
         * @endcode
         * 
         */
        PID &withIntegralZone(double integralZone);

        /**
         * @brief Sets the timeout duration
         *
         * @param timeout the number of seconds the PID should run for before
         * timing out
         *
         * This method sets m_timeout, which is the number of seconds the PID
         * will run for before isCompleted() returns true.
         */
        void setTimeout(double timeout);
        /**
         * @brief Sets the timeout duration and returns a refrence to the PID object
         *
         * @param timeout the number of seconds the PID should run for before
         * timing out
         * @return PID& (a PID refrence)
         *
         * This method calls setTimeout(double timeout), but also returns a
         * refrence to the PID object, so that more methods can be chained and
         * it can be assigned to an object using the assignment operator (=).
         * 
         * @code {.cpp}
         * art::PID pid = art::PID().withConstants(2, 10, -5)
         *                          .withTimeout(3);
         * @endcode
         * 
         */
        PID &withTimeout(double timeout);

        /**
         * @brief Set the range within which the PID is considered settled
         *
         * @param settleZone the value below which the error is considered
         * settled
         *
         * This method sets m_settleZone. While the absolute value of the error
         * is less than or equal to this value, the settleTimeout will continue
         * to count down. Once that time is up, the PID will be considered
         * complete.
         */
        void setSettleZone(double settleZone);
        /**
         * @brief Set the range within which the PID is considered settled and returns a refrence to the PID object
         *
         * @param settleZone the value below which the error is considered
         * settled
         * @return PID& (a PID refrence)
         *
         * This method calls setSettleZone(double settleZone), but also returns a
         * refrence to the PID object, so that more methods can be chained and
         * it can be assigned to an object using the assignment operator (=).
         * 
         * @code {.cpp}
         * art::PID pid = art::PID().withConstants(2, 10, -5)
         *                          .withSettleZone(3)
         *                          .withSettleTimeout(0.5);
         * @endcode
         * 
         */
        PID &withSettleZone(double settleZone);

        /**
         * @brief Set the duration the PID must be settled for before being
         * complete
         *
         * @param settleTimeout the number of seconds the PID must be settled
         * before being complete
         *
         * This method sets m_settleTimeout. Once the PID is considered settled,
         * a TimePoint is set and counts down until the settletimeout is
         * reached, at which point the PID is complete and isComplete() returns
         * true.
         */
        void setSettleTimeout(double settleTimeout);
        /**
         * @brief Set the duration the PID must be settled for before being
         * complete
         *
         * @param settleTimeout the number of seconds the PID must be settled
         * before being complete
         * @return PID& (a PID refrence)
         *
         * This method calls setSettleTimout(double settleTimout), but also returns
         * a refrence to the PID object, so that more methods can be chained and
         * it can be assigned to an object using the assignment operator (=).
         *
         * @code {.cpp} art::PID pid = art::PID().withConstants(2, 10, -5)
         *                                       .withSettleZone(3)
         *                                       .withSettleTimeout(0.5);
         * @endcode
         *
         */
        PID &withSettleTimeout(double settleTimeout);

        /**
         * @brief Runs the PID by providing an input, then calculates and
         * returns an output
         *
         * @param error the PID's input, the target minus the current value 
         * @return double the PID's output
         *
         * This method is the heart of the PID class. It takes the error as an
         * input and uses it to calculate each individual term before outputting
         * the sum of the 3 terms. Calling this method updates several of the is
         * the only was to update several of the member variables, so they will
         * remain constant until calculate(double error) is called.
         *
         * Both of the timeouts will continue to count down regaurdless of
         * whether calculate is called, however the PID will not update whether
         * it is settled or not unless it is fed values using calculate.
         */
        double calculate(double error);
        /**
         * @brief An alternate call to calculate(double error) that seperates the target and feedback
         * for ease of use
         *
         * @param target the value the PID should move toward
         * @param feedback the value representing the PID's current state or
         * position
         * @return double the output of the PID
         *
         * This method simply calls calculate(double error), but takes 2
         * parameters and calculates error to be the target minus the feedback.
         * This method is to simply make it easier to ensure the calculations
         * are done right by seperating and naming the target and the feedback
         * to prevent flipping them.
         */
        double calculate(double target, double feedback);

        /**
         * @brief Gets the current state of the PID
         *
         * @return bool - whether or not the PID is finished running
         *
         * This method decides whether or not the PID is finished running using
         * two checks. The first check is that the PID has been running for
         * longer than the timeout specified by m_timeout. The second checks if
         * the error has been within the range of m_settleZone for more than
         * m_settleZoneTimeout seconds. If either of these conditions is true,
         * the PID is considered completed and the method returns true.
         *
         * Once the PID is considered complete, it can still be rendered
         * incomplete if calculate is called with a new error outside the
         * m_settleZone or it is reset() to be run again from scratch.
         */
        bool isCompleted();

        /**
         * @brief Gets the amoount of time passed since the PID started
         *
         * @return double - the number of seconds the PID has been running
         *
         * This method returns the amount of time, in seconds, since the PID was
         * constructed or last reset. This is calculated using the TimePoint
         * m_startTime, which is set upon creation and updated using reset().
         */
        double timePassed();
        /**
         * @brief Gets the amoount of time the PID has been settled for
         *
         * @return double - the number of seconds the error has remained in the
         * settleZone
         *
         * This method returns the amount of time, in seconds, since the error
         * was last outside the settleZone (m_settleZone). It keeps a TimePoint
         * to this time and returns the time since then. If the PID is not
         * settled, the time will be incredibly short (less than a few
         * milliseconds) and can go back to being near 0 if the error exists the
         * settleZone range.
         */
        double settledTimePassed();

        /**
         * @brief Get the output produced by the Proportional(P) term
         *
         * @return double (the output of the P term)
         *
         * The P term of a PID controller is the error times a constant.
         * Therefore, the output of the Proportional term here is m_error times
         * m_kp, which is what this method returns.
         */
        double getProportional();
        /**
         * @brief Get the output produced by the Integral(I) term
         *
         * @return double (the output of the I term)
         *
         * The I term of a PID controller is the sumation of the error over a
         * period of time multiplied by a constant. Therefore, the output of the
         * Integral term here is m_integral times m_ki, which is what this
         * method returns.
         */
        double getIntegral();
        /**
         * @brief Get the output produced by the Derivative(D) term
         *
         * @return double (the output of the D term)
         *
         * The D term of a PID controller is the change in error times a constant.
         * Therefore, the output of the Proportional term here is m_derivative times
         * m_kd, which is what this method returns
         */
        double getDerivative();
    private:
        
        /**
         * @brief The error last fed into the PID controller
         *
         * Stores the current error of the PID controller. This is calculated
         * and stored when calculate(double target, double feedback) is called
         * or simply stored from calculate(double error);
         */
        double m_error{0};
        /**
         * @brief The error fed into the PID controller on it's previous
         * iteration
         *
         * Stores the pervious error of the PID controller. After all
         * calculations are complete, the error is stored as the previous error
         * so that the derivative(the change in error) can be calculated upon another call of calculate().'
         * 
         * The P term in PID stands for proportional. The term is equal to the constant multiplied by the error.
         */
        double m_prevError{0};

        /**
         * @brief The derivate calculated by the PID controller
         *
         * Calculated using the error and previous error, the derivative
         * represents the change in error.
         * 
         * The D term in PID is the Derivative multiplied by the constant.
         */
        double m_derivative{0};

        /**
         * @brief The integral caluculated by the PID controller
         *
         * The integral is calculated as the sum of the error over an extended
         * period of time. The error is only added to the sum if it is less than
         * or equal to the integral zone.
         * 
         * The I term in PID is the Integral multiplied by the constant.
         */
        double m_integral{0};
        /**
         * @brief The range within which the error is applied to calculate the integral
         * 
         * The error is only added to the sum if it is less than
         * or equal to the integral zone.
         */
        double m_integralZone{0};

        /**
         * @brief A constant applied to the Proportional term
         *
         * A constant applied to the Proportional term, allowing the term to be
         * tuned individually of the other terms, adjusting the significance of
         * the term by increasing or decreasing its value. 
         * 
         * This constant can be set using any of these Methods:
         * > setConstants(double kp, double ki, double kd)  
         * > withConstants(double kp, double ki, double kd)  
         * > setConstants(double kp, double ki, double kd, double ff)  
         * > withConstants(double kp, double ki, double kd, double ff)  
         */
        double m_kp{0};
        /**
         * @brief A constant applied to the Integral term
         *
         * A constant applied to the Integral term, allowing the term to be
         * tuned individually of the other terms, adjusting the significance of
         * the term by increasing or decreasing its value. 
         * 
         * This constant can be set using any of these Methods:
         * > setConstants(double kp, double ki, double kd)  
         * > withConstants(double kp, double ki, double kd)  
         * > setConstants(double kp, double ki, double kd, double ff)  
         * > withConstants(double kp, double ki, double kd, double ff)  
         * 
         */
        double m_ki{0};
        /**
         * @brief A constant applied to the Derivative term
         *
         * A constant applied to the Derivative term, allowing the term to be
         * tuned individually of the other terms, adjusting the significance of
         * the term by increasing or decreasing its value. 
         * 
         * This constant can be set using any of these Methods:
         * > setConstants(double kp, double ki, double kd)  
         * > withConstants(double kp, double ki, double kd)  
         * > setConstants(double kp, double ki, double kd, double ff)  
         * > withConstants(double kp, double ki, double kd, double ff)  
         */
        double m_kd{0};
        /**
         * @brief A constant applied directly to the output
         *
         * A constant applied directly to the output, meaning it doesn't change
         * in relation to the error in any way. Useful for heavy position
         * control for arms or anytime a base output is desired. Honestly
         * doesn't have that many good use-cases though.
         *
         * This constant can be set using either of these Methods:
         * > setConstants(double kp, double ki, double kd, double ff)  
         * > withConstants(double kp, double ki, double kd, double ff)  
         */
        double m_ff{0};

        /**
         * @brief A point in time representing when the PID started
         *
         * The point in time is taken when the PID object is constructed or when
         * it is reset. This can then be used to keep a timer running tracking
         * the duration the PID has been trying to reach its target. This allows
         * for a timeout to be added to isComplete(), preventing the PID from
         * stalling indefinetely.
         *
         * The time since this point can be accessed using timePassed();
         */
        TimePoint m_startTime;
        /**
         * @brief The duration the PID should run for before it is considered
         * finished
         *
         * After this amount of time(in seconds) isCompleted() will return true.
         * This is calculated using m_startTime, so it either starts from when
         * the PID was constructed or last reset.
         *
         * This can be set using setTimeout(double timeout) or
         * withTimeout(double timeout).
         */
        double m_timeout{0};

        /**
         * @brief A point in time representing when the PID first entered the
         * acceptable error range
         *
         * The point in time is taken when the absolute value of the error first
         * becomes less than or equal to m_settleZone. This can then be used to
         * keep track of how long the error has been within an acceptable range.
         * This allows for the PID to be considered complete once the error
         * settles for a specified amount of time.
         *
         * The time since this point can be accessed using settledTimePassed().
         */
        TimePoint m_startSettledTime;
        /**
         * @brief The duration the error must be within before the PID is considered complete
         * 
         * After this amount of time(in seconds) isCompleted() will return true.
         * This is calculated using m_startSettledTime, when the absolute value of the error first
         * becomes less than or equal to m_settleZone.
         * 
         * This can be set using setSettleTimeout(double timeout) or
         * withSettleTimeout(double timeout).
         */
        double m_settleTimeout{0};
        /**
         * @brief The range which the error must be for the PID to be considered
         * settled
         *
         * While the absolute value of the error is less than or equal to this
         * value, the settleTimeout will continue to count down. Once that time
         * is up, the PID will be considered complete.
         *
         * The unit for values is up to you, but keep it consistant with the
         * inputs and the error.
         *
         * This can be set using setSettleZone(double settleZone) or
         * withSettleZone(double settleZone).
         */
        double m_settleZone{0};
    };
    
    


} // namespace art
