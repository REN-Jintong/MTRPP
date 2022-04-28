/*
 * Ope_inter2opt.cpp
 *
 *  Created on: Feb 3, 2021
 *      Author: jiren
 */


#include "../mtrpp_lib.h"
#include "../MTRPP_Functions.h"
#include "../Utility.h"
#include "Ope_inter2opt.h"

/*declaration of local functions*/
void Dinter2opt(struct_sol *S);
bool Inter2opt_oneroute(struct_sol *S, int indroute);
/*main functions*/


void NF_inter2opt(struct_sol *S){
	S->fetch_vec_sd();
	S->fetch_SbSe();
	Dinter2opt(S);
	return;
}

void Dinter2opt(struct_sol *S){
	bool Fimp=false;
	do{
		Fimp=false;
		for(int i=0; i<num_car;i++){
			if(Inter2opt_oneroute(S,i)) Fimp=true;
			if(check_time()) return;
		}

//		cout<<"finish one"<<endl;
//		if(Fimp) cout<<"true"<<endl;
//		else cout<<"false"<<endl;
//		break;

	}while(Fimp);

}

bool Inter2opt_oneroute(struct_sol *S, int indroute){
	bool Fimp=false;
	int pos1, pos2;

	for(pos1=0;pos1<S->routeS[indroute].Nsel;pos1++){
		for(int var_car=0;var_car<num_car;var_car++){
			if(indroute==var_car) continue;
			for(pos2=0;pos2<S->routeS[var_car].Nsel;pos2++){
				if(pos1==S->routeS[indroute].Nsel-1 && S->routeS[var_car].Nsel-1==pos2 ) continue;
				if(check_time()) return Fimp;


//				printf("pos1=%d, pos2=%d, deltacdis=%d ",pos1, pos2, S->try_inter2opt(indroute, var_car, pos1, pos2));
//				cout<<Fimp<<endl;

				if(S->try_inter2opt(indroute, var_car, pos1, pos2)<0){
					Fimp=true;
					S->move_inter2opt(indroute, var_car, pos1, pos2);
					S->update_profit(indroute);
					S->update_profit(var_car);
//					check_move(S);
					Update_BestS(S,0);

					return Fimp;
				}


			}
		}
	}

	return Fimp;
}
