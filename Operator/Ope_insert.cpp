/*
 * Ope_insert.cpp
 *
 *  Created on: April 22, 2021
 *      Author: jiren
 */


#include "../mtrpp_lib.h"
#include "../MTRPP_Functions.h"
#include "../Utility.h"
#include "Ope_insert.h"

/*declaration of local functions*/
void Dinsert(struct_sol *S);
bool Insert_oneroute_completeNF(struct_sol *S, int indroute);
bool Insert_oneroute_randomorder(struct_sol *S, int indroute);
bool Insert_oneroute_CL(struct_sol *S, int indroute);
int try_insert(struct_sol *S, int movpos, int fixpos, int N, int *single_Vec, int *single_perm);
/*main functions*/

void NF_insert(struct_sol *S){

	S->fetch_vec_sd();
	Dinsert(S);
	return;
}

void Dinsert(struct_sol *S){
	bool Fimp=false;

	// the operation is carried on route by route;
	do{
		Fimp=false;
		for(int i=0;i<num_car;i++) {
//			if(Insert_oneroute_completeNF(S,i)) Fimp=true;
			if(Insert_oneroute_randomorder(S,i)) Fimp=true;
			if(check_time()) return;
		}
	}while(Fimp);
}

bool Insert_oneroute_completeNF(struct_sol *S, int indroute){
	bool Fimp=false;
	int movenode, fixnode;

	duplicate_solution(S,LBS);

	for(int i=1;i<S->routeS[indroute].Nsel;i++){ //depot is excluded from the movenode;
		for(int j=1;j<S->routeS[indroute].Nsel;j++){
			movenode=S->routeS[indroute].perm[i];
			if(check_time()) return false;
			fixnode=S->routeS[indroute].perm[j];
			if(fixnode==movenode) continue;

			if(try_insert(S,i,j,S->routeS[indroute].Nsel, S->routeS[indroute].vec_sd, S->routeS[indroute].perm)<0){
				Fimp=true;
				S->move_insert_oropt(i, j, indroute, 1);
				S->update_profit(indroute);
//				check_move(S);
				Update_BestS(S,0);
				break;
			}
		}

	}

	return Fimp;
}

bool Insert_oneroute_randomorder(struct_sol *S, int indroute){
	bool Fimp=false;
//	int movenode, fixnode;
	int vec_ord[num_city];
//	duplicate_solution(S,LBS);
	int id1, id2;

	for(int i=0;i<S->routeS[indroute].Nsel;i++) vec_ord[i]=i;
	ExcGene(vec_ord, S->routeS[indroute].Nsel);

	for(int i=0;i<S->routeS[indroute].Nsel;i++){
		id1=vec_ord[i];
		if(id1==0) continue;
		for(int j=0;j<S->routeS[indroute].Nsel;j++){
			id2=vec_ord[j];
			if(id2==0) continue;
			if(id1==id2) continue;
			if(try_insert(S,id1,id2,S->routeS[indroute].Nsel, S->routeS[indroute].vec_sd, S->routeS[indroute].perm)<0){
				Fimp=true;
				S->move_insert_oropt(id1, id2, indroute, 1);
				S->update_profit(indroute);
//				check_move(S);
				Update_BestS(S,0);
				break;
			}
		}
	}

	return Fimp;
}


int try_insert(struct_sol *S, int movpos, int fixpos, int N, int *single_Vec, int *single_perm){
	if(movpos-fixpos==1) return 0;

	if (movpos<fixpos) return S->FwI(movpos,fixpos,1,single_Vec,N,single_perm);
	else return S->BwI(movpos,fixpos,1,single_Vec,N,single_perm);

}


