/*
 * mtrpp_lib.cpp
 *
 *  Created on: Jan 19, 2021
 *      Author: jiren
 */


#include <iostream>
#include <time.h>
#include <ctime>
#include "Object/struct_map.h"
#include "Object/struct_sol.h"
#include "Object/population.h"
using namespace std;

int alreadybest=999999999;
char *rep;
char *name_instance;		//-i
int seed;					//-seed
char resultsFile[100];
char name_final_result[256];
char detail_running_profil[256];
char benchmark[100];
int *CandiList;


int ip1;
int ip2;
int rdg;
int numpop;

int num_city;
int num_car;

int NumCandidateSet_NF;
int NumCandidateSet_LK;

double cuttime;
double Para_M;
double alpha;

struct_map *glo_map;
struct_sol *CurrentS, *BestS, *LBS, *LLBS, *CBS;
Population *PopS;

clock_t glo_start;
clock_t glo_end;
int *vec_seqV;
int totalT;

double t_swap;
double t_2opt;
double t_insert;
double t_oropt;

double t_add;
double t_drop;
double t_interswap;
double t_interinsert;
double t_inter2opt;
double t_interoropt;
double t_doublebridge;
