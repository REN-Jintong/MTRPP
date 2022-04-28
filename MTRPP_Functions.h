/*
 * MTRPP_Functions.h
 *
 *  Created on: Jan 19, 2021
 *      Author: jiren
 */

#ifndef MTRPP_FUNCTIONS_H_
#define MTRPP_FUNCTIONS_H_

#include "mtrpp_lib.h"

void cover_detail(void);
void duplicate_solution(struct_sol *fromSol, struct_sol *toSol);
void copyarray(int *fromv, int *tov, int l);
void copymatrix(int **frommat, int **tomat, int l1, int l2);
void Update_BestS(struct_sol *S, int ite);
int try_vec_profit(int *arr);
void ExcGene(int *lvec, int l);

#endif /* MTRPP_FUNCTIONS_H_ */
