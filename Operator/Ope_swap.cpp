/*
 * Ope_swap.cpp
 *
 *  Created on: April 21, 2021
 *      Author: jiren
 */


#include "../mtrpp_lib.h"
#include "../MTRPP_Functions.h"
#include "../Utility.h"
#include "Ope_swap.h"

/*declaration of local functions*/
void Dswap(struct_sol *S);
bool Swap_oneroute_completeNF(struct_sol *S, int indroute);
bool Swap_oneroute_randomorder(struct_sol *S, int indroute);
/*main functions*/

void NF_swap(struct_sol *S){

	Dswap(S);
	return;
}

void Dswap(struct_sol *S){
	bool Fimp=false;

	// the operation is carried on route by route;
	do{
		Fimp=false;
		for(int i=0;i<num_car;i++) {
//			if(Swap_oneroute_completeNF(S,i)) Fimp=true;
			if(Swap_oneroute_randomorder(S,i)) Fimp=true;
			if(check_time()) return;
		}
	}while(Fimp);
}

bool Swap_oneroute_completeNF(struct_sol *S, int indroute){
	bool Fimp=false;
	int fixnode, swapnode;

//	duplicate_solution(S, LBS);
	for(int i=1;i<S->routeS[indroute].Nsel;i++){			// depot is always excluded from the selected set.
		fixnode=S->routeS[indroute].perm[i];
		for(int j=i+1;j<S->routeS[indroute].Nsel;j++){
			if(check_time()) return false;
			swapnode=S->routeS[indroute].perm[j];
			if(S->f_city[swapnode]!=indroute || S->f_city[fixnode]!=indroute) {
				cout<<"error in completeNF, not on the indroute car"<<endl;
				exit(-2);
			}
			if(fixnode==swapnode) continue; // filter the same node

			if(abs(S->position[1][fixnode]-S->position[1][swapnode])==1) { //adjacent nodes
				if(check_time()) return true;
				if(S->try_swapadj(S->routeS[indroute].Nsel,S->routeS[indroute].perm, i, j, fixnode, swapnode)<0){
					Fimp=true;
					S->move_swapadj(indroute, S->routeS[indroute].Nsel,S->routeS[indroute].perm, i, j, fixnode, swapnode);
					S->update_profit(indroute);
//					check_move(S);
					Update_BestS(S,0);
					break;
				}
			}

			else if(abs(S->position[1][fixnode]-S->position[1][swapnode])>=2){
				if(check_time()) return true;
				if(S->try_swap(S->routeS[indroute].Nsel,S->routeS[indroute].perm, i, j, fixnode, swapnode)<0){
					Fimp=true;
					S->move_swap(indroute, S->routeS[indroute].Nsel,S->routeS[indroute].perm, i, j, fixnode, swapnode);
					S->update_profit(indroute);
//					check_move(S);
					Update_BestS(S,0);
					break;
				}
			}

			else {
				cout<<"other condition in complete NF in swap.cpp"<<endl;
				exit(-3);
			}
		}
	}
	return Fimp;
}

bool Swap_oneroute_randomorder(struct_sol *S, int indroute){
	bool Fimp=false;
	int fixnode, swapnode;
	int vec_ord[num_city];
	int id1,id2;

	for(int i=0;i<S->routeS[indroute].Nsel;i++) vec_ord[i]=i;
	ExcGene(vec_ord, S->routeS[indroute].Nsel);

	for(int i=0;i<S->routeS[indroute].Nsel;i++){
		id1=vec_ord[i];
		if(id1==0) continue;
		fixnode=S->routeS[indroute].perm[id1];
		for(int j=i+1;j<S->routeS[indroute].Nsel;j++){
			id2=vec_ord[j];
			if(id2==0) continue;
			swapnode=S->routeS[indroute].perm[id2];
			if(fixnode==swapnode) continue; // filter the same node

			if(abs(S->position[1][fixnode]-S->position[1][swapnode])==1) { //adjacent nodes
				if(check_time()) return true;
				if(S->try_swapadj(S->routeS[indroute].Nsel,S->routeS[indroute].perm, id1, id2, fixnode, swapnode)<0){
					Fimp=true;
					S->move_swapadj(indroute, S->routeS[indroute].Nsel,S->routeS[indroute].perm, id1, id2, fixnode, swapnode);
					S->update_profit(indroute);
//					check_move(S);
					Update_BestS(S,0);
					break;
				}
			}

			else if(abs(S->position[1][fixnode]-S->position[1][swapnode])>=2){
				if(check_time()) return true;
				if(S->try_swap(S->routeS[indroute].Nsel,S->routeS[indroute].perm, id1, id2, fixnode, swapnode)<0){
					Fimp=true;
					S->move_swap(indroute, S->routeS[indroute].Nsel,S->routeS[indroute].perm, id1, id2, fixnode, swapnode);
					S->update_profit(indroute);
//					check_move(S);
					Update_BestS(S,0);
					break;
				}
			}

			else {
				cout<<"other condition in complete NF in swap.cpp"<<endl;
				exit(-3);
			}
		}
	}

	return Fimp;
}




