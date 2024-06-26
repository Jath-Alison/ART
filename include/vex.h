/**
 * @file vex.h
 * @author Jath Alison (Jath.Alison@gmail.com)
 * @brief The include file for the Vex Library
 * @version 0.1
 * @date 06-25-2024
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "v5.h"
#include "v5_vcs.h"

extern vex::brain Brain; ///< @brief a brain object representing the V5 Brain
extern vex::controller Controller1; ///< @brief a controller object representing the Primary Controller

#define waitUntil(condition)                                                   \
  do {                                                                         \
    wait(5, msec);                                                             \
  } while (!(condition))

#define repeat(iterations)                                                     \
  for (int iterator = 0; iterator < iterations; iterator++)