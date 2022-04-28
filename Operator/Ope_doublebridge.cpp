/*
 * Ope_2opt.cpp
 *
 *  Created on: Feb 3, 2021
 *      Author: jiren
 */


#include "../mtrpp_lib.h"
#include "../MTRPP_Functions.h"
#include "../Utility.h"
#include "Ope_doublebridge.h"

/*declaration of local functions*/
void Ddoublebridge(struct_sol *S);
bool Ddoublebridge_oneroute_CompleteNeighborhood(struct_sol *S, int indroute);
/*main functions*/

void NF_doublebridge(struct_sol *S){
	S->fetch_SbSe();
	S->fetch_vec_sd();

	Ddoublebridge(S);
	if(check_time()) return;
}

void Ddoublebridge(struct_sol *S){
	bool Fimp=false;

	// the operation is carried on route by route;
	do{
		Fimp=false;
		for(int i=0;i<num_car;i++) {
			if(Ddoublebridge_oneroute_CompleteNeighborhood(S,i)) Fimp=true;
			if(check_time()) return;
		}

	}while(Fimp);
}


bool Ddoublebridge_oneroute_CompleteNeighborhood(struct_sol *S, int indroute){
	bool Fimp=false;
	int pos1, pos2, pos3, pos4;


	if(S->routeS[indroute].Nsel<8) return Fimp;

	duplicate_solution(S,LBS);

	for(pos1=0; pos1<S->routeS[indroute].Nsel;pos1++){
		for(pos2=pos1+2;pos2<S->routeS[indroute].Nsel;pos2++){
			for(pos3=pos2+2;pos3<S->routeS[indroute].Nsel;pos3++){
				for(pos4=pos3+2;pos4<S->routeS[indroute].Nsel;pos4++){

					if(check_time()) return Fimp;

					if(S->try_doublebridge(indroute, pos1, pos2, pos3, pos4)<0){
						S->move_doublebridge(indroute, pos1, pos2, pos3, pos4);
						Fimp=true;
						S->update_profit(indroute);
//						check_move(S);
						Update_BestS(S,0);
						return Fimp;
					}
				}
			}
		}
	}

	return Fimp;
}
