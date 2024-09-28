/**
 * @file TankDrive.cpp
 * @author Jath Alison (Jath.Alison@gmail.com)
 * @brief Source containing the TankDrive class
 * @version 0.1
 * @date 07-15-2024
 *
 * @copyright Copyright (c) 2024
 *
 * This is the source defining the TankDrive class. The
 * class aims to provide a wrapper for TankDrive base controls, making it easier
 * to write driver control code as well as autons by providing the low level
 * controls for more complex functions to use.
 */

#include "ART/TankDrive.h"

namespace art
{
    TankDrive::TankDrive(vex::motor_group left, vex::motor_group right) : m_left(left), m_right(right) {}

    void TankDrive::arcade(double x, double y, double rot)
    {
        m_cmdX = x;
        m_cmdY = y;
        m_cmdRot = rot;
        update();
    }
    void TankDrive::arcade(double drive, double rot)
    {
        m_cmdY = drive;
        m_cmdRot = rot;
        update();
    }
    void TankDrive::tank(double left, double right){
        m_cmdY = left + right;
        m_cmdRot = left - right;
        update();
    }

    void TankDrive::LeftSplitArcade(const vex::controller &cont)
    {
        arcade(cont.Axis3.position(), cont.Axis1.position());
        update();
    }
    void TankDrive::LeftSplitArcadeCurved(const vex::controller &cont)
    {
        arcade(cont.Axis3.position() * cont.Axis3.position() * cont.Axis3.position() * 0.01 * 0.01,
               cont.Axis1.position() * cont.Axis1.position() * cont.Axis1.position() * 0.01 * 0.01);
        update();
    }
    void TankDrive::update()
    {
        m_left.set(m_cmdY + m_cmdRot);
        m_right.set(m_cmdY - m_cmdRot);
    }
} // namespace art
