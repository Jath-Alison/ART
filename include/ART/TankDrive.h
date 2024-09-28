/**
 * @file TankDrive.h
 * @author Jath Alison (Jath.Alison@gmail.com)
 * @brief Header containing the TankDrive class
 * @version 0.1
 * @date 07-14-2024
 *
 * @copyright Copyright (c) 2024
 *
 * This is the header that needs to be included to use the TankDrive class. The
 * class aims to package TankDrive base controls, making it easier
 * to write driver control code as well as autons by providing the low level
 * controls for more complex functions to use.
 */
#pragma once

#include "ART/SimpleMotorGroup.h"

namespace art
{
	/**
	 * @brief A Simple TankDrive class
	 * 
	 * The aim of the TankDrive class is to package low-level tank controls to
	 * be called both during directly to drive and by other classes to allow for
	 * more complex functions. The SmartDrive class for example, expands on the
	 * functionality of the TankDrive by using the basic functions to perform
	 * more complex maneuvers.
	 * 
	 * The TankDrive can be commanded through several different functions. These
	 * commands are stored and used to command the motors. However, the benefit
	 * of storing these commands rather than directly commanding the motors is
	 * that 1) they can be accessed later and 2) they don't cap out at a maximum
	 * value. This gives the ability to pass whatever commands you wish as when
	 * the motor commands are calculated, one of the commands can be overwritten
	 * if the other is more significant. For example, if a small turn input is
	 * paired with a large drive input (greater than 100), the drive input can
	 * cause the rotation input to be insignificant.
	 * 
	 * Commands are percentages of the max voltage(12). The left of the robot
	 * takes the sum of the drive and turn inputs, while the right takes the
	 * difference. Thus, the motor command can exceed 100(or -100), which would
	 * just make the motor move at 100 percent. Commanding more than that
	 * doesn't change anything, but because the commands are added, commanding
	 * more than a 100 percent means that the command can remain past 100 even
	 * after adding the turn command.
	 * 
	 * @todo add a way to enable speedMode
	 * @todo add a method to retain proportionality between inputs
	 * @todo add getters for stuff
	 * @todo add curvature control scheme
	 * @todo provide several other preset control schemes
	 */
	class TankDrive
	{
	public:
		/**
		 * @brief Construct a new Tank Drive object
		 * 
		 * @param left a motor_group for the left side
		 * @param right a motor_group for the right side
		 * 
		 * The TankDrive class requires 2 vex::motor_groups, one for each side.
		 */
		TankDrive(vex::motor_group left, vex::motor_group right);

		/**
		 * @brief Commands the TankDrive using arcade inputs
		 * 
		 * @param x the command for the X-direction(the TankDrive ignores this
		 * input)
		 * @param y the command for the Y-direcction(driving)
		 * @param rot the command for rotation(turning)
		 * 
		 * Because TankDrives don't have wheels powered in the X-direction, the
		 * X-input is ignored. The reason this function still accepts it is
		 * because there were originally plans to include several other drive
		 * types, but were never fully fleshed out, leaving an extra input that
		 * is currently unused.
		 */
		void arcade(double x, double y, double rot);
		/**
		 * @brief Commands the TankDrive using Tank-only arcade inputs
		 * 
		 * @param drive the command for the Y-direcction(driving)
		 * @param rot the command for rotation(turning
		 * 
		 * A modified arcade method to remove the ignored x-input.
		 */
		void arcade(double drive, double rot);
		/**
		 * @brief Commands the TankDrive using tank inputs
		 * 
		 * @param left the command for the left motors
		 * @param right the command for the right motors
		 * 
		 * Internally, the 2 inputs are simply used to calculate the y and rot
		 * commands. The notes on commands past 100 in the class description
		 * remains true for these calculated inputs.
		 */
		void tank(double left, double right);

		/**
		 * @brief Commands the TankDrive using arcade inputs directly from a controller
		 * 
		 * @param cont a refrence to a controller object
		 * 
		 * This method simply provides a pre-set control scheme using a
		 * controller as an input. The commands are passed directly from the
		 * joysticks to the arcade function.
		 * 
		 * The Left Joystick's vertical axis(Axis3) is mapped to driving and the
		 * Right Joystick's horizontal Axis(Axis1) is mapped to turning.
		 */
		void LeftSplitArcade(const vex::controller &cont);
		/**
		 * @brief Commands the TankDrive using arcade inputs directly from a controller
		 * 
		 * @param cont a refrence to a controller object
		 * 
		 * This method simply provides a pre-set control scheme using a
		 * controller as an input. The commands from the joysticks are cubed and
		 * divided by 10,000 before being passed to the arcade function. This
		 * results in an input curve that decreases lower values, decreasing the
		 * sensitivity of inputs and making it easier to drive slow.
		 * 
		 * The Left Joystick's vertical axis(Axis3) is mapped to driving and the
		 * Right Joystick's horizontal Axis(Axis1) is mapped to turning.
		 * 
		 * For example, an input of 80 from the controller only leads to an
		 * output of ~50 percent. While an input of 50 leads to an output of
		 * only ~12 percent.
		 * 
		 * @htmlonly 
		 * <iframe
		 * 	src="https://www.desmos.com/calculator/tk6sdxfzy6?embed"
		 * width="99%"
		 * height="auto"
		 * style="border: 5px solid #1982d2"
		 * frameborder=10></iframe>
		 * @endhtmlonly
		 */
		void LeftSplitArcadeCurved(const vex::controller &cont);

		/**
		 * @brief Uses the stored commands to command the motors
		 * 
		 * Originally, this had use to actually command the motors, but now each
		 * method internally calls update, so this just remains here, as it was
		 * part of a shared interface between periodically updating objects.
		 * Essentially, it does nothing notable and should be ignored.
		 */
		void update();
	protected:
		/**
		 * @brief a SimpleMotorGroup for the left side
		 * 
		 * Currently, there is no way to access this SimpleMotorGroup using the
		 * TankDrive object. However, you can still use methods on the
		 * vex::motor_groups you used to contruct the TankDrive object. Telling
		 * those motor groups to spin will likely cause undefined behaviour
		 * though.
		 */
		SimpleMotorGroup m_left;
		/**
		 * @brief a SimpleMotorGroup for the right side
		 * 
		 * Currently, there is no way to access this SimpleMotorGroup using the
		 * TankDrive object. However, you can still use methods on the
		 * vex::motor_groups you used to contruct the TankDrive object. Telling
		 * those motor groups to spin will likely cause undefined behaviour
		 * though.
		 */
		SimpleMotorGroup m_right;

		/**
		 * @brief The Stored command for the X-direction
		 * 
		 * The command is a positive or negative percentage, but can exceed 100
		 * as well. Doing so will make the command more significant in
		 * calculations with the other commands.
		 * 
		 * Because TankDrives can't drive in the X-direction, this command is ignored.
		 */
		double m_cmdX{0};
		/**
		 * @brief The Stored command for the Y-direction
		 * 
		 * The command is a positive or negative percentage, but can exceed 100
		 * as well. Doing so will make the command more significant in
		 * calculations with the other commands.
		 */
		double m_cmdY{0};
		/**
		 * @brief The Stored command for rotation
		 * 
		 * The command is a positive or negative percentage, but can exceed 100
		 * as well. Doing so will make the command more significant in
		 * calculations with the other commands.
		 */
		double m_cmdRot{0};
	};
} // namespace art
