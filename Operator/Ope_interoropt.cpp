/*
 * OPE_INTEROROPT_H_.cpp
 *
 *  Created on: April 22, 2021
 *      Author: jiren
 */


#include "../mtrpp_lib.h"
#include "../MTRPP_Functions.h"
#include "../Utility.h"
#include "Ope_interoropt.h"

/*declaration of local functions*/
void Dinteroropt(struct_sol *S);
bool Interoropt_oneroute(struct_sol *S, int route1, int length);

/*main functions*/

void NF_interoropt(struct_sol *S){
	S->fetch_vec_sd();
	Dinteroropt(S);
	return;
}

void Dinteroropt(struct_sol *S){
	bool Fimp=false;
	do{
		Fimp=false;
		for(int i=0; i<num_car;i++){
//			if(Interoropt_oneroute(S,i,1)) Fimp=true;
			if(Interoropt_oneroute(S,i,2)) Fimp=true;
			if(Interoropt_oneroute(S,i,3)) Fimp=true;
//			if(Interoropt_oneroute(S,i,4)) Fimp=true;
			if(check_time()) return;
		}

	}while(Fimp);
}

bool Interoropt_oneroute(struct_sol *S, int route1, int length){
	bool Fimp=false;
	int route2;
	int pos1, pos2;



	for(pos1=1;pos1+length<S->routeS[route1].Nsel+1;pos1++){		//the depot is excluded from the candidate
		for( route2=0;route2<num_car;route2++){
			if(route1==route2) continue;
			for(pos2=0;pos2<S->routeS[route2].Nsel;pos2++){	//insert position could be depot (after the insert position)
				if(check_time()) return Fimp;

//				printf("%d\n",S->try_interoropt(length, pos1, pos2, route1, route2));
				if(S->try_interoropt(length, pos1, pos2, route1, route2)<0){
					Fimp=true;
					S->move_interoropt(length, pos1, pos2, route1, route2);
					S->update_profit(route1);
					S->update_profit(route2);
//					check_move(S);
					Update_BestS(S,0);

					return Fimp;
				}
			}
		}
	}
	return Fimp;
}
