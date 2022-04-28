/*
 * OPE_OROPT_H_.cpp
 *
 *  Created on: April 22, 2021
 *      Author: jiren
 */


#include "../mtrpp_lib.h"
#include "../MTRPP_Functions.h"
#include "../Utility.h"
#include "Ope_oropt.h"

/*declaration of local functions*/
void Doropt(struct_sol *S);
bool Oropt_oneroute_completeNF(struct_sol *S, int indroute, int k);
bool Oropt_oneroute_randomorder(struct_sol *S, int indroute, int k);
bool Oropt_oneroute_mixNF(struct_sol *S, int indroute);
int try_oropt(struct_sol *S, int movpos, int fixpos, int N, int *single_Vec, int *single_perm, int k);
/*main functions*/

void NF_oropt(struct_sol *S){
	S->fetch_vec_sd();
	Doropt(S);
	return;
}

void Doropt(struct_sol *S){
	bool Fimp=false;

	// the operation is carried on route by route;
	do{
		Fimp=false;
		for(int i=0;i<num_car;i++) {
//			if(Oropt_oneroute_completeNF(S,i,4)) Fimp=true;

//			if(Oropt_oneroute_completeNF(S,i,2)) Fimp=true;
//			if(Oropt_oneroute_completeNF(S,i,3)) Fimp=true;
			if(Oropt_oneroute_randomorder(S,i,2)) Fimp=true;
			if(Oropt_oneroute_randomorder(S,i,3)) Fimp=true;

//			if(Oropt_oneroute_mixNF(S, i)) Fimp=true;
//
//			if(Oropt_oneroute_completeNF(S,i,4)) Fimp=true;
//			if(Oropt_oneroute_completeNF(S,i,5)) Fimp=true;
			if(check_time()) return;
		}
	}while(Fimp);
}

bool Oropt_oneroute_completeNF(struct_sol *S, int indroute, int k){
	bool Fimp=false;
	int movenode, fixnode;

	for(int i=1;i<S->routeS[indroute].Nsel+1-k;i++){ //depot is excluded from the movenode;
		for(int j=1;j<S->routeS[indroute].Nsel;j++){
			movenode=S->routeS[indroute].perm[i];
			if(check_time()) return false;
			if(i+k-1>=j && j>i) continue;
			fixnode=S->routeS[indroute].perm[j];
			if(fixnode==movenode) continue;

			if(try_oropt(S,i,j,S->routeS[indroute].Nsel, S->routeS[indroute].vec_sd, S->routeS[indroute].perm, k)<0){
				Fimp=true;
				S->move_insert_oropt(i, j, indroute, k);
				S->update_profit(indroute);
//				check_move(S);
				Update_BestS(S,0);
				break;
			}
		}
	}

	return Fimp;
}

bool Oropt_oneroute_randomorder(struct_sol *S, int indroute, int k){
	bool Fimp=false;
	int movenode, fixnode;
	int vec_ord[num_city];
	int vec_tar[num_city];
	int id1, id2;

	for(int i=0;i<S->routeS[indroute].Nsel+1-k;i++) vec_ord[i]=i;
	for(int i=0;i<S->routeS[indroute].Nsel;i++) vec_tar[i]=i;
	ExcGene(vec_ord, S->routeS[indroute].Nsel+1-k);
	ExcGene(vec_tar, S->routeS[indroute].Nsel);

	for(int i=0;i<S->routeS[indroute].Nsel+1-k;i++){
		id1=vec_ord[i];
		if(id1==0) continue;
		for(int j=0;j<S->routeS[indroute].Nsel;j++){
			if(check_time()) return false;
			id2=vec_tar[j];
			if(id2==0) continue;
			if(id1+k-1>=id2 && id2>id1) continue;
			if(id1==id2) continue;

			if(try_oropt(S,id1,id2,S->routeS[indroute].Nsel, S->routeS[indroute].vec_sd, S->routeS[indroute].perm, k)<0){
				Fimp=true;
				S->move_insert_oropt(id1, id2, indroute, k);
				S->update_profit(indroute);
//				check_move(S);
				Update_BestS(S,0);
				break;
			}

		}
	}

	return Fimp;
}

bool Oropt_oneroute_mixNF(struct_sol *S, int indroute){
	bool Fimp=false;
	int movenode, fixnode;
	int delta1, delta2;

	for(int i=1;i<S->routeS[indroute].Nsel;i++){ //depot is excluded from the movenode;
		movenode=S->routeS[indroute].perm[i];
		for(int j=1;j<S->routeS[indroute].Nsel;j++){
			if(check_time()) return false;
//			if(i+k-1>=j && j>i) continue;
			fixnode=S->routeS[indroute].perm[j];
			if(fixnode==movenode) continue;

			if(i+2-1>=j && j>i) continue;
			delta1=try_oropt(S,i,j,S->routeS[indroute].Nsel, S->routeS[indroute].vec_sd, S->routeS[indroute].perm, 2);

			if(i+3-1>=j && j>i) delta2=99999;
			else delta2=try_oropt(S,i,j,S->routeS[indroute].Nsel, S->routeS[indroute].vec_sd, S->routeS[indroute].perm, 3);


			if(delta1<0 && delta2<0){
				if(delta1<delta2){
					Fimp=true;
					S->move_insert_oropt(i, j, indroute, 2);
					S->update_profit(indroute);
//					check_move(S);
					Update_BestS(S,0);
					break;
				}
				else {
					Fimp=true;
					S->move_insert_oropt(i, j, indroute, 3);
					S->update_profit(indroute);
//					check_move(S);
					Update_BestS(S,0);
					break;
				}

			}
			else if(delta1<0 && delta2>0){
				Fimp=true;
				S->move_insert_oropt(i, j, indroute, 2);
				S->update_profit(indroute);
//				check_move(S);
				Update_BestS(S,0);
				break;
			}
			else if(delta1>0 && delta2<0){
				Fimp=true;
				S->move_insert_oropt(i, j, indroute, 3);
				S->update_profit(indroute);
//				check_move(S);
				Update_BestS(S,0);
				break;
			}
		}
	}
	return Fimp;
}

int try_oropt(struct_sol *S, int movpos, int fixpos, int N, int *single_Vec, int *single_perm, int k){
	if(movpos-fixpos==1) return 0;
	if(movpos+k-1>=N) return 99999;

	if (movpos<fixpos) return S->FwI(movpos,fixpos,k,single_Vec,N,single_perm);
	else return S->BwI(movpos,fixpos,k,single_Vec,N,single_perm);

}

