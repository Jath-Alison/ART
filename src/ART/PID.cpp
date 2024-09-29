/**
 * @file PID.cpp
 * @author Jath Alison (Jath.Alison@gmail.com)
 * @brief Source containing a general use PID class
 * @version 1.0-beta
 * @date 07-13-2024
 * 
 * @copyright Copyright (c) 2024
 * 
 * This is the source defining the PID class. The class
 * aims to package PID functions together to be used across different
 * applications.
 */
#include "ART/PID.h"

namespace art
{

    PID::PID()
        : m_startTime(std::chrono::high_resolution_clock::now())
    {
        setConstants(1, 0, 0, 0);
    }

    void PID::reset()
    {
        m_error = 0;
        m_prevError = 0;

        m_derivative = 0;
        m_integral = 0;

        m_startTime = std::chrono::high_resolution_clock::now();
        m_startSettledTime = std::chrono::high_resolution_clock::now();
    }

    void PID::setConstants(double kp, double ki, double kd)
    {
        m_kp = kp;
        m_ki = ki;
        m_kd = kd;
    }
    PID &PID::withConstants(double kp, double ki, double kd)
    {
        setConstants(kp, ki, kd);
        return *this;
    }
    void PID::setConstants(double kp, double ki, double kd, double ff)
    {
        m_kp = kp;
        m_ki = ki;
        m_kd = kd;
        m_ff = ff;
    }
    PID &PID::withConstants(double kp, double ki, double kd, double ff)
    {
        setConstants(kp, ki, kd, ff);
        return *this;
    }

    double PID::getkp(){ return m_kp; }
    double PID::getki(){ return m_ki; }
    double PID::getkd(){ return m_kd; }
    double PID::getff(){ return m_ff; }

    void PID::setIntegralZone(double integralZone) { m_integralZone = integralZone; }
    PID &PID::withIntegralZone(double integralZone)
    {
        setIntegralZone(integralZone);
        return *this;
    }

    void PID::setTimeout(double timeout) { m_timeout = timeout; }
    PID &PID::withTimeout(double timeout)
    {
        setTimeout(timeout);
        return *this;
    }

    void PID::setSettleZone(double settleZone) { m_settleZone = settleZone; }
    PID &PID::withSettleZone(double settleZone)
    {
        setSettleZone(settleZone);
        return *this;
    }

    void PID::setSettleTimeout(double settleTimeout) { m_settleTimeout = settleTimeout; }
    PID &PID::withSettleTimeout(double settleTimeout)
    {
        setSettleTimeout(settleTimeout);
        return *this;
    }

    double PID::calculate(double error)
    {
        m_error = error;
        m_derivative = m_prevError - m_error;

        if (std::abs(m_error) < m_integralZone)
        {
            m_integral += m_error;
        }
        else
        {
            m_integral = 0;
        }
        if ((m_error > 0 && m_prevError < 0) || (m_error < 0 && m_prevError > 0))
        {
            // m_integral *= -0.5;
        }

        if (std::abs(error) > m_settleZone)
        {
            m_startSettledTime = std::chrono::high_resolution_clock::now();
        }

        double output = (m_kp * m_error) + (m_kd * m_derivative) + (m_ki * m_integral) + m_ff;

        m_prevError = m_error;
        return output;
    }
    double PID::calculate(double target, double feedback)
    {
        return calculate(target - feedback);
    }

    bool PID::isCompleted()
    {
        if (timePassed() > m_timeout && m_timeout != 0)
        {
            return true;
        }
        if (settledTimePassed() > m_settleTimeout && m_settleTimeout != 0)
        {
            return true;
        }
        return false;
    }

    double PID::timePassed()
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - m_startTime).count() / 1000.f;
    }

    double PID::settledTimePassed()
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - m_startSettledTime).count() / 1000.f;
    }

    double PID::getProportional(){ return m_error * m_kp; }
    double PID::getIntegral(){ return m_integral * m_ki; }
    double PID::getDerivative(){ return m_derivative * m_kd; }

} // namespace Jath