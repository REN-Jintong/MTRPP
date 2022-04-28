/*
 * Ope_drop.cpp
 *
 *  Created on: April 21, 2021
 *      Author: jiren
 */


#include "../mtrpp_lib.h"
#include "../MTRPP_Functions.h"
#include "../Utility.h"
#include "Ope_drop.h"

/*declaration of local functions*/
void Ddrop(struct_sol *S);
bool Drop_oneroute_CompleteNeighborhood(struct_sol *S, int indroute);
int fetch_delta_profit(int *single_perm, int N, int nodepos, int p);
void dropbot_oneroute(struct_sol *S, int indroute);
/*main functions*/

void NF_drop(struct_sol *S){
	S->fetch_vec_sd();
	Ddrop(S);
	Bot_drop(S);
	return;
}

void Bot_drop(struct_sol *S){
	S->fetch_vec_sd();
	for(int i=0;i<num_car;i++) {
		dropbot_oneroute(S,i);
		if(check_time()) return;
	}
}


void Ddrop(struct_sol *S){
	bool Fimp=false;

	// the operation is carried on route by route;
	do{
		Fimp=false;
		for(int i=0;i<num_car;i++) {
			if(Drop_oneroute_CompleteNeighborhood(S,i)) Fimp=true;
			if(check_time()) return;
		}

	}while(Fimp);
}

bool Drop_oneroute_CompleteNeighborhood(struct_sol *S, int indroute){
	bool Fimp=false;
	int nodedrop;

	for(int i=1;i<S->routeS[indroute].Nsel;i++){
		if(check_time()) return false;
		nodedrop=S->routeS[indroute].perm[i];

//		if(S->try_drop_profit(i, nodedrop, S->routeS[indroute].Nsel, S->routeS[indroute].vec_sd, S->routeS[indroute].perm)>0){
		if(fetch_delta_profit(S->routeS[indroute].perm, S->routeS[indroute].Nsel, i, S->routeS[indroute].profit)>0){
			Fimp=true;
			S->move_drop(indroute, nodedrop, i);
			S->update_profit(indroute);
			check_move(S);
			Update_BestS(S,0);

			break;
		}

	}


	return Fimp;
}

int fetch_delta_profit(int *single_perm, int N, int nodepos, int p){

	int arr[N];

	for(int i=0;i<nodepos;i++)	arr[i]=single_perm[i];
	for(int i=nodepos;i<N;i++) arr[i]=single_perm[i+1];

	return try_vec_profit(arr)-p;
}

void dropbot_oneroute(struct_sol *S, int indroute){
	for(int i=S->routeS[indroute].Nsel-1;i>0;i--){
		if(glo_map->profitA[S->routeS[indroute].perm[i]]-S->routeS[indroute].vec_sd[i]<0) {
			S->move_drop(indroute, S->routeS[indroute].perm[i], i);
			S->update_profit(indroute);
			Update_BestS(S,0);
		}
		else return;
	}
}
