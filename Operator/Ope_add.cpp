/*
 * Ope_add.cpp
 *
 *  Created on: April 23, 2021
 *      Author: jiren
 */


#include "../mtrpp_lib.h"
#include "../MTRPP_Functions.h"
#include "../Utility.h"
#include "Ope_add.h"

/*declaration of local functions*/
void Dadd(struct_sol *S);
bool Add_completeNF(struct_sol *S);
bool judge_oneroute(struct_sol *S, int indroute, int nodeadd);

/*main functions*/

void NF_add(struct_sol *S){
	S->fetch_vec_sd();
	Dadd(S);
	return;
}

void Dadd(struct_sol *S){
	bool Fimp=false;

	// the operation is carried on route by route;
	do{
		Fimp=false;
		for(int i=0;i<num_car;i++) {
			if(Add_completeNF(S)) Fimp=true;
			if(check_time()) return;
		}
	}while(Fimp);
}

bool Add_completeNF(struct_sol *S){
	bool Fimp=false;
	int nodeadd;
	int selectnode __attribute__((unused));
	int indroute __attribute__((unused));
	int posadd __attribute__((unused));

//	for(int i=0;i<num_city;i++){
//		if(S->f_city[i]!=-1) continue;
//		nodeadd=i;
//		for(int j=1;j<num_city;j++){
//			if(check_time()) return false;
//			if(S->f_city[j]==-1) continue;
//			selectnode=j;
//			indroute=S->position[0][selectnode];
//			posadd=S->position[1][selectnode];
//
//			if(S->try_add(posadd, nodeadd, S->routeS[indroute].Nsel, S->routeS[indroute].vec_sd, S->routeS[indroute].perm)>0){
//				// this is an estimating profit, so greater than 0 is the key.
//				Fimp=true;
//				S->move_add(indroute, nodeadd, posadd);
//				S->update_profit(indroute);
////				check_move(S);
//				Update_BestS(S,0);
//				break;
//			}
//		}
//	}

	for(int i=0;i<num_city;i++){
		if(S->f_city[i]!=-1) continue;
		nodeadd=i;

		for(int j=0;j<num_car;j++){
			if(judge_oneroute(S, j, nodeadd)){
				Fimp=true;
				break;
			}
		}
	}
	return Fimp;
}

bool judge_oneroute(struct_sol *S, int indroute, int nodeadd){
	bool Fimp=false;
	int posadd;

	for(int i=1;i<S->routeS[indroute].Nsel+1;i++){		//depot is excluded, and the last point adds 1;
		posadd=i;
		if(S->try_add(posadd, nodeadd, S->routeS[indroute].Nsel, S->routeS[indroute].vec_sd, S->routeS[indroute].perm)>0){
			// this is an estimating profit, so greater than 0 is the key.
			Fimp=true;
			S->move_add(indroute, nodeadd, posadd);
			S->update_profit(indroute);
//			check_move(S);
			Update_BestS(S,0);
			break;
		}
	}

	return Fimp;
}

