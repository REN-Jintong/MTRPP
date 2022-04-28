/*
 * mtrpp_lib.h
 *
 *  Created on: Jan 19, 2021
 *      Author: jiren
 */

#ifndef MTRPP_LIB_H_
#define MTRPP_LIB_H_

/*lib*/

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string.h>
#include <time.h>
#include <ctime>
#include <vector>
#include <cmath>
#include <math.h>
#include <complex>
#include <assert.h>
#include <iomanip>
//#include <sys/times.h>
#include <unistd.h>
#include "Object/struct_map.h"
#include "Object/struct_sol.h"
#include "Object/population.h"
#include "MTRPP_Functions.h"
#include "Utility.h"

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
using namespace std;

/*global variable*/
extern int seed;
extern int alreadybest;
extern char *rep;
extern char *name_instance;		//-i
extern int seed;					//-seed
extern char resultsFile[100];
extern char name_final_result[256];
extern char benchmark[100];
extern char detail_running_profil[256];
/**/
extern int num_city;
extern int num_car;
extern int NumCandidateSet_NF;
extern int NumCandidateSet_LK;
extern int *CandiList;
/**/

extern int ip1;
extern int ip2;
extern int rdg;
extern int numpop;

extern double cuttime;
extern double Para_M;
extern double alpha;

extern struct_map *glo_map;
extern struct_sol *CurrentS, *BestS, *LBS, *LLBS, *CBS;
extern Population *PopS;

extern clock_t glo_start;
extern clock_t glo_end;
extern int *vec_seqV;
extern int totalT;

extern double t_swap;
extern double t_2opt;
extern double t_insert;
extern double t_oropt;

extern double t_add;
extern double t_drop;
extern double t_interswap;
extern double t_interinsert;
extern double t_inter2opt;
extern double t_interoropt;
extern double t_doublebridge;

#endif /* MTRPP_LIB_H_ */
