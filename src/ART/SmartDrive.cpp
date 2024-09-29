/**
 * @file SmartDrive.cpp
 * @author Jath Alison (Jath.Alison@gmail.com)
 * @brief Source containing the SmartDrive class
 * @version 1.0-beta
 * @date 09-25-2024
 *
 * @copyright Copyright (c) 2024
 *
 * This is the source file defining the SmartDrive
 * class. The class aims to package more complex base controls, making it
 * easier to write autos by providing consistant high-level controls for
 * more complex tasks.
 */

#include "ART/SmartDrive.h"

namespace art
{

    SmartDrive::SmartDrive(TankDrive drive, vex::inertial inert) : TankDrive(drive), m_inert(inert)
    {
        m_inert.calibrate();
    }

    SmartDrive &SmartDrive::withWheelSize(Length size)
    {
        m_wheelSize = size;
        return *this;
    }

    SmartDrive &SmartDrive::withGearRatio(double ratio)
    {
        m_gearRatio = ratio;
        return *this;
    }

    SmartDrive &SmartDrive::withHorizontalTracker(vex::rotation rotation, Length wheelSize, double gearRatio)
    {
        m_tracker = HorizontalTracker(rotation, wheelSize, gearRatio);
        return *this;
    }

    SmartDrive &SmartDrive::withHorizontalTracker(vex::rotation rotation, Length wheelSize, double gearRatio, Length wheelOffset)
    {
        m_tracker = HorizontalTracker(rotation, wheelSize, gearRatio, wheelOffset);
        return *this;
    }

    int SmartDrive::track(){
        if(m_tracker.m_rotation){
            m_tracker.m_rotation->setPosition(0, vex::deg);
        }
        while (m_inert.isCalibrating())
        {
            vex::wait(5, vex::msec);
        }

        while (true)
        {
            static Angle prevDir;
            if (m_dir != prevDir)
            {
                m_inert.setHeading(m_dir.degrees(), vex::deg);
            }
            m_dir = Degrees(m_inert.heading(vex::degrees));
            prevDir = m_dir;

            Angle wheelTravel = Angle((getLeftTravel() + getRightTravel()) / 2.0);
            Length travel = Length(wheelTravel.revolutions() * getWheelTravel());

            // Distance travel = Inches(change*((3.25 * 3.1415)/360.f) / (72.f/48.f));
            Vec2 posChange = Vec2::dirAndMag(m_dir, travel);

            if (m_tracker.m_rotation != nullptr)
            {
                Length hTravel = m_tracker.getTravel();

                Vec2 trackerTravel = Vec2::dirAndMag(m_dir + Degrees(90), hTravel);

                posChange = posChange + trackerTravel;
            }

            m_pos = m_pos + posChange;
            m_centerPos = m_pos + Vec2::dirAndMag(m_dir, m_tracker.m_offset);

            vex::wait(20, vex::msec);
        }
    }

    void SmartDrive::driveFor(Length target, double speed){

        Angle offset = Degrees((m_left.position(vex::degrees) + m_right.position(vex::degrees)) / 2.f);
        Angle targetRot = Revolutions(target / getWheelTravel()) + offset;

        int dir = std::abs(target)/target;

        Angle pos = offset;

        while ((targetRot - pos) * dir > 0)
        {
            pos = Degrees((m_left.position(vex::degrees) + m_right.position(vex::degrees)) / 2.f);

            arcade(speed, 0);

            wait(20, vex::msec);
        }
        
    }
    void SmartDrive::driveForPID(Length target){

        double out= 0;

        Angle offset = Degrees((m_left.position(vex::degrees) + m_right.position(vex::degrees)) / 2.f);
        Angle targetRot = Revolutions(target / getWheelTravel()) + offset;

        m_driveForPID.reset();
        while (!m_driveForPID.isCompleted())
        {
            Angle pos = Degrees((m_left.position(vex::degrees) + m_right.position(vex::degrees)) / 2.f);
            out = m_driveForPID.calculate(targetRot - pos);

            arcade(out, 0);

            wait(20, vex::msec);
        }
        arcade(0, 0, 0);
    }
    SmartDrive &SmartDrive::withDriveForPID(PID pid){
        m_driveForPID = pid;
        return *this;
    }

    void SmartDrive::turnFor(Angle target, double speed){

        Angle targetAngle = Degrees(target.degrees() + m_inert.rotation(vex::degrees));

        int dir = double(std::abs(target) / target);

        while (Degrees(targetAngle.degrees() - m_inert.rotation(vex::degrees)) * dir > 0)
        {
            arcade(0, speed);

            wait(20, vex::msec);
        }
        arcade(0, 0, 0);
    }
    void SmartDrive::turnForPID(Angle target){

        double out = 0;

        Angle targetAngle = Degrees(m_inert.rotation(vex::degrees) + target.degrees());

        m_turnForPID.reset();
        while (!m_turnForPID.isCompleted())
        {
            Angle error = Degrees(targetAngle.degrees() - m_inert.rotation(vex::degrees));

            out = m_turnForPID.calculate(error);

            arcade(0, out);

            wait(20, vex::msec);
        }
        arcade(0, 0, 0);
    }
    SmartDrive &SmartDrive::withTurnForPID(PID pid){
        m_turnForPID = pid;
        return *this;
    }

    void SmartDrive::turnTo(Angle target, double speed){

        Angle error = shortestTurnPath(Degrees(target.degrees() - m_inert.heading(vex::degrees)));
        int errorSign = std::abs(error)/error;

        while (!(std::abs(error.degrees()) < 10) || errorSign == std::abs(error)/error)
        {
            error = shortestTurnPath(Degrees(target.degrees() - m_inert.heading(vex::degrees)));

            arcade(0, speed);
            
            if((std::abs(error.degrees()) < 5)){
                arcade(0, 0, 0);
                return;
            }

            errorSign = std::abs(error)/error;
            wait(20, vex::msec);
        }
        arcade(0, 0, 0);
    }
    void SmartDrive::turnToPID(Angle target){

        double out = 0;

        m_turnToPID.reset();
        while (!m_turnToPID.isCompleted())
        {
            Angle error = shortestTurnPath(Degrees(target.degrees() - m_inert.heading(vex::degrees)));

            out = m_turnToPID.calculate(error);

            arcade(0, out);

            wait(20, vex::msec);
        }
        arcade(0, 0, 0);
    }
    SmartDrive &SmartDrive::withTurnToPID(PID pid){
        m_turnToPID = pid;
        return *this;
    }

    Length SmartDrive::getWheelTravel(){
        return Length(M_PI * m_wheelSize * m_gearRatio);
    }

    SmartDrive::HorizontalTracker::HorizontalTracker(){}
    SmartDrive::HorizontalTracker::HorizontalTracker(vex::rotation rotation, Length wheelSize, double gearRatio) : m_wheelSize(wheelSize), m_gearRatio(gearRatio){
        m_rotation = std::make_shared<vex::rotation>(rotation);
    }
    SmartDrive::HorizontalTracker::HorizontalTracker(vex::rotation rotation, Length wheelSize, double gearRatio, Length wheelOffset) : m_wheelSize(wheelSize), m_gearRatio(gearRatio), m_offset(wheelOffset){
        m_rotation = std::make_shared<vex::rotation>(rotation);
    }
    Length  SmartDrive::HorizontalTracker::getTravel(){
        Angle currentAngle = Degrees(m_rotation->position(vex::degrees));

        m_travelAngle = Angle(currentAngle - m_lastAngle);
        m_lastAngle = currentAngle;

        m_travelDistance = Length(m_travelAngle * (m_wheelSize / 2.0) * m_gearRatio);

        return m_travelDistance;
    }

    Angle SmartDrive::getLeftTravel(){
        Angle currentAngle = Degrees(m_left.position(vex::degrees));

        Angle tempAngle = Angle(currentAngle - m_LastLeftPos);
        m_LastLeftPos = currentAngle;

        return tempAngle;
    }
    Angle SmartDrive::getRightTravel(){
        Angle currentAngle = Degrees(m_right.position(vex::degrees));

        Angle tempAngle = Angle(currentAngle - m_LastRightPos);
        m_LastRightPos = currentAngle;

        return tempAngle;
    }

} // namespace art
