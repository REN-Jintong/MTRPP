/*
 * Ope_interswap.cpp
 *
 *  Created on: April 21, 2021
 *      Author: jiren
 */


#include "../mtrpp_lib.h"
#include "../MTRPP_Functions.h"
#include "../Utility.h"
#include "Ope_interswap.h"

/*declaration of local functions*/
void Dinterswap(struct_sol *S);
bool InterSwap_oneroute(struct_sol *S, int indroute);
bool Interx_onefix(struct_sol *S, int indroute, int fixnode, int pos1);
bool Interx_onefix_CL(struct_sol *S, int indroute, int fixnode, int pos1);

/*main functions*/

void NF_interswap(struct_sol *S){

	Dinterswap(S);
	return;
}

void Dinterswap(struct_sol *S){
	bool Fimp=false;

	// the operation is carried on route by route;
	do{
		Fimp=false;
		for(int i=0;i<num_car;i++){
			if(InterSwap_oneroute(S,i)) Fimp=true;
			if(check_time()) return;
		}
	}while(Fimp);
}

bool InterSwap_oneroute(struct_sol *S, int indroute){
	bool Fimp=false;
	int fixnode, swapnode;

	for(int i=1;i<S->routeS[indroute].Nsel;i++){
		fixnode=S->routeS[indroute].perm[i];			//exclude the depot
		if(Interx_onefix(S,indroute,fixnode,i)) Fimp=true;
//		if(Interx_onefix_CL(S,indroute,fixnode,i)) Fimp=true;
		if(check_time()) return false;
	}
	return Fimp;
}

bool Interx_onefix(struct_sol *S, int indroute, int fixnode, int pos1){
	int swapnode;


//	int esticdis, realcdis, oldcdis, newcdis;
//
//	duplicate_solution(S,LBS);

	for(int j=0;j<num_car;j++){						//j is the car number
		if(j==indroute) continue;
		for(int k=1;k<S->routeS[j].Nsel;k++){		//exclude the depot
			swapnode=S->routeS[j].perm[k];
			if(check_time()) return false;


			if(S->try_interswap(fixnode, swapnode, indroute, j, pos1, k)<0){ 	//estimate of cdis
				S->move_interswap(fixnode, swapnode, indroute, j, pos1, k);
				S->update_profit(indroute);
				S->update_profit(j);
//				check_move(S);
				Update_BestS(S,0);
				return true;
			}
		}
	}
	return false;
}

bool Interx_onefix_CL(struct_sol *S, int indroute, int fixnode, int pos1){

	int swapnode;
	int route2, pos2;


	for(int i=0;i<glo_map->Num_pernode_nf[fixnode];i++){
		swapnode=glo_map->CandidateSet_NF[fixnode][i];
		route2=S->position[0][swapnode];
		if(route2==indroute) continue;
		if(route2==-1) continue;
		pos2=S->position[1][swapnode];
		if(check_time()) return false;
		if(S->try_interswap(fixnode, swapnode, indroute, route2 , pos1, pos2)<0){
			S->move_interswap(fixnode, swapnode, indroute, route2, pos1, pos2);
			S->update_profit(indroute);
			S->update_profit(route2);
//			check_move(S);
			Update_BestS(S,0);
			return true;
		}

	}
	return false;
}
