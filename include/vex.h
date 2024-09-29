/**
 * @file vex.h
 * @author Jath Alison (Jath.Alison@gmail.com)
 * @brief The include file for the Vex Library
 * @version 1.0-beta
 * @date 06-25-2024
 *
 * @copyright Copyright (c) 2024
 *
 * This file simply contains the neccessary includes to access any part of the
 * Vex V5 library. Chances are, the code here will never need to be modified.
 * Any file that needs to interact with a vex controller, brain, motor or other
 * device will need to include this file to access those classes.
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "v5.h"
#include "v5_vcs.h"

extern vex::brain Brain;			/**< a brain object representing the V5 Brain */
extern vex::controller Controller1; /**< a controller object representing the Primary Controller */

#define waitUntil(condition) \
	do                       \
	{                        \
		wait(5, msec);       \
	} while (!(condition))

#define repeat(iterations) \
	for (int iterator = 0; iterator < iterations; iterator++)