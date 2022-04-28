/*
 * Utility.h
 *
 *  Created on: Feb 3, 2021
 *      Author: jiren
 */

#ifndef UTILITY_H_
#define UTILITY_H_

#include "mtrpp_lib.h"

double get_time(void);
bool check_time(void);
bool check_move(struct_sol *S);
void check_result(void);
void layout(struct_sol *S);
void layout_solution(struct_sol *S);
#endif /* UTILITY_H_ */
