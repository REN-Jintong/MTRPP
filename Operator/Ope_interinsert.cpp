/*
 * Ope_insert.cpp
 *
 *  Created on: April 22, 2021
 *      Author: jiren
 */


#include "../mtrpp_lib.h"
#include "../MTRPP_Functions.h"
#include "../Utility.h"
#include "Ope_interinsert.h"

/*declaration of local functions*/
void Dinterinsert(struct_sol *S);
bool Interinsert_oneroute(struct_sol *S, int indroute);
bool Interinsert_onefix(struct_sol *S, int indroute, int fixnode, int pos1);
bool Interinsert_onefix_CL(struct_sol *S, int indroute, int fixnode, int pos1);
/*main functions*/

void NF_interinsert(struct_sol *S){

	S->fetch_vec_sd();
	Dinterinsert(S);
	return;
}

void Dinterinsert(struct_sol *S){
	bool Fimp=false;

	// the operation is carried on route by route;
	do{
		Fimp=false;
		for(int i=0;i<num_car;i++){
			if(Interinsert_oneroute(S,i)) Fimp=true;
			if(check_time()) return;
		}
	}while(Fimp);
}

bool Interinsert_oneroute(struct_sol *S, int indroute){
	bool Fimp=false;
	int fixnode;

	for(int i=1;i<S->routeS[indroute].Nsel;i++){			//depot city is excluded
		fixnode=S->routeS[indroute].perm[i];
		if(fixnode==-1) {
			printf("fixnode=-1 in interinsert.");
			continue;
		}
		if(Interinsert_onefix(S,indroute,fixnode,i)) Fimp=true;
//		if(Interinsert_onefix_CL(S,indroute,fixnode,i)) Fimp=true;
		if(check_time()) return false;

	}
	return Fimp;
}


bool Interinsert_onefix(struct_sol *S, int indroute, int fixnode, int pos1){
	int objnode;

	for(int j=0;j<num_car;j++){
		if(j==indroute) continue;
		for(int k=0;k<S->routeS[j].Nsel;k++){		//fixnode is moved after the objnode， so depot is in the list;
			objnode=S->routeS[j].perm[k];
			if(check_time()) return false;
			if(S->try_interinsert(fixnode, objnode, indroute, j , pos1, k)<0){
				S->move_interinsert(fixnode, objnode, indroute, j, pos1, k);
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

bool Interinsert_onefix_CL(struct_sol *S, int indroute, int fixnode, int pos1){
	int objnode;
	int route2;
	int pos2;

	for(int i=0;i<glo_map->Num_pernode_nf[fixnode];i++){
		objnode=glo_map->CandidateSet_NF[fixnode][i];
		route2=S->position[0][objnode];
		if(route2==indroute) continue;
		if(route2==-1) continue;
		pos2=S->position[1][objnode];
		if(check_time()) return false;
		if(S->try_interinsert(fixnode, objnode, indroute, route2 , pos1, pos2)<0){
			S->move_interinsert(fixnode, objnode, indroute, route2, pos1, pos2);
			S->update_profit(indroute);
			S->update_profit(route2);
//				check_move(S);
			Update_BestS(S,0);
			return true;
		}

	}
	return false;
}



