/*
 * Ope_2opt.cpp
 *
 *  Created on: Feb 3, 2021
 *      Author: jiren
 */


#include "../mtrpp_lib.h"
#include "../MTRPP_Functions.h"
#include "../Utility.h"
#include "Ope_2opt.h"

/*declaration of local functions*/
void D2opt(struct_sol *S);
bool D2opt_oneroute(struct_sol *S, int indroute);
bool D2opt_oneroute_CompleteNeighborhood(struct_sol *S, int indroute);
bool D2opt_oneroute_randomorder(struct_sol *S, int indroute);
int switch_2opt(struct_sol *S, int indroute, int node1, int node2);
int get_ope_2opt(struct_sol *S, int indroute, int smaposnode, int bigposnode);
void switch_ope2opt(struct_sol *S, int indroute, int node1, int node2);


/*The function based on the CL*/
bool D2opt_oneroute_CL(struct_sol *S, int indroute);

/*function for the threshold*/
void TH2opt(struct_sol *S);
bool D2opt_oneroute_threshold(struct_sol *S, int indroute, int threshold);

/*main functions*/

void NF_2opt(struct_sol *S){
	S->fetch_SbSe();
	S->fetch_vec_sd();

	D2opt(S);
	if(check_time()) return;
}

void D2opt(struct_sol *S){
	bool Fimp=false;

	// the operation is carried on route by route;
	do{
		Fimp=false;
		for(int i=0;i<num_car;i++) {
//			Fimp=D2opt_oneroute(S,i);
//			if(D2opt_oneroute_CompleteNeighborhood(S,i)) Fimp=true;
			if(D2opt_oneroute_randomorder(S,i)) Fimp=true;
			if(check_time()) return;
		}

	}while(Fimp);
}

bool D2opt_oneroute(struct_sol *S, int indroute){
	bool Fimp=false;
	int fixnode, swapnode;

	for(int i=0;i<num_city;i++){
		if(S->f_city[i]!=indroute && i!=0) continue; 	//filter the purpose route, city 0 is always excluded
		fixnode=i;
		for(int j=0;j<glo_map->Num_pernode_nf[fixnode];j++){
			if(check_time()) return false;
			swapnode=glo_map->CandidateSet_NF[fixnode][j];
			if(S->f_city[swapnode]!=indroute) continue;	//only consider the cities on the same route
			if(swapnode==fixnode){
				printf("error in D2opt, swapnode=fixnode\n");
				exit(-1);
			}
			if(abs(S->position[1][fixnode]-S->position[1][swapnode])<=1) continue;	//filter the adjacent nodes
			if(check_time()) return true;
			if(switch_2opt(S,indroute,fixnode, swapnode)<0){
				switch_ope2opt(S,indroute,fixnode, swapnode);

				Fimp=true;
				S->update_profit(indroute);
				Update_BestS(S,0);
			}
		}
	}
	return Fimp;
}

bool D2opt_oneroute_CompleteNeighborhood(struct_sol *S, int indroute){
	bool Fimp=false;
	int fixnode, swapnode;


	for(int i=0;i<S->routeS[indroute].Nsel;i++){
//		if(S->f_city[i]!=indroute && i!=0) continue; 	//filter the purpose route, city 0 is always excluded
		fixnode=S->routeS[indroute].perm[i];
		for(int j=0;j<S->routeS[indroute].Nsel;j++){

			swapnode=S->routeS[indroute].perm[j];
//			if(S->f_city[swapnode]!=indroute || S->f_city[fixnode]!=indroute) {
//				cout<<"error in completeNF, not on the indroute car"<<endl;
//				exit(-2);
//			}
			if(abs(S->position[1][fixnode]-S->position[1][swapnode])<=1) continue;	//filter the adjacent nodes
			if(check_time()) return true;

			if(switch_2opt(S,indroute,fixnode, swapnode)<0){
				switch_ope2opt(S,indroute,fixnode, swapnode);
				Fimp=true;
				S->update_profit(indroute);
//				check_move(S);
				Update_BestS(S,0);
			}
		}
	}
	return Fimp;
}

bool D2opt_oneroute_randomorder(struct_sol *S, int indroute){
	bool Fimp=false;
	int fixnode, swapnode;
	int vec_ord[num_city];

	for(int i=0;i<S->routeS[indroute].Nsel;i++) vec_ord[i]=S->routeS[indroute].perm[i];
	ExcGene(vec_ord, S->routeS[indroute].Nsel);

	for(int i=0;i<S->routeS[indroute].Nsel;i++){
		fixnode=vec_ord[i];
//		fixnode=S->routeS[indroute].perm[i];
		for(int j=0;j<S->routeS[indroute].Nsel;j++){
//			swapnode=S->routeS[indroute].perm[j];
			swapnode=vec_ord[j];
			if(abs(S->position[1][fixnode]-S->position[1][swapnode])<=1) continue;	//filter the adjacent nodes
			if(check_time()) return true;

			if(switch_2opt(S,indroute,fixnode, swapnode)<0){
				switch_ope2opt(S,indroute,fixnode, swapnode);
				Fimp=true;
				S->update_profit(indroute);
//				check_move(S);
				Update_BestS(S,0);
			}
		}
	}
	return Fimp;
}

bool D2opt_oneroute_CL(struct_sol *S, int indroute){
	bool Fimp=false;
	int fixnode, swapnode;

	for(int i=0;i<S->routeS[indroute].Nsel;i++){
		fixnode=S->routeS[indroute].perm[i];
		for(int j=0;j<glo_map->Num_pernode_nf[fixnode];j++){
			swapnode=glo_map->CandidateSet_NF[fixnode][j];
			if(S->f_city[swapnode]!=indroute) continue;
			if(abs(S->position[1][fixnode]-S->position[1][swapnode])<=1) continue;	//filter the adjacent nodes
			if(check_time()) return true;

			if(switch_2opt(S,indroute,fixnode, swapnode)<0){
				switch_ope2opt(S,indroute,fixnode, swapnode);
				Fimp=true;
				S->update_profit(indroute);
//				check_move(S);
				Update_BestS(S,0);
			}

		}
	}
	return Fimp;
}

int switch_2opt(struct_sol *S, int indroute, int node1, int node2){
 	if(S->position[1][node1]<S->position[1][node2]) return get_ope_2opt(S, indroute, node1, node2);
	else if(S->position[1][node1]>S->position[1][node2]) return get_ope_2opt(S, indroute, node2, node1);
	else {
		cout<<"error in equal city number"<<endl;
		exit(-2);
	}
	return 0;
}

int get_ope_2opt(struct_sol *S, int indroute, int smaposnode, int bigposnode){
	int pos1=S->position[1][smaposnode];
	int pos2=S->position[1][bigposnode];

	return S->fast_2opt(pos1, pos2, S->routeS[indroute].vec_sd,S->routeS[indroute].Nsel,\
	S->routeS[indroute].perm, S->routeS[indroute].Sb, S->routeS[indroute].Se, S->routeS[indroute].cdis);
}

void switch_ope2opt(struct_sol *S, int indroute, int node1, int node2){
	if(S->position[1][node1]<S->position[1][node2]) {
		S->exe_2opt(S,indroute,node1,node2);
		return;
	}
	else if(S->position[1][node1]>S->position[1][node2]) {
		S->exe_2opt(S,indroute, node2,node1);
		return;
	}
	else {
		cout<<"error in equal city number"<<endl;
		exit(-2);
	}
	return;
}

/*****************************************************************************/
void TH_2opt(struct_sol *S){
	S->fetch_SbSe();
	S->fetch_vec_sd();

	TH2opt(S);
	if(check_time()) return;
}


void TH2opt(struct_sol *S){

	CBS->totalprofit=S->totalprofit;
	double ratio=0.2;

	int tmpbst;
	int threshold=(1.0-ratio)*LBS->totalprofit;

	// the operation is carried on route by route;
	do{
		tmpbst=CBS->totalprofit;

		for(int i=0;i<num_car;i++) {
			D2opt_oneroute_threshold(S,i, threshold);
			if(check_time()) return;
		}

	}while(tmpbst<CBS->totalprofit);


}

bool D2opt_oneroute_threshold(struct_sol *S, int indroute, int threshold){

	bool Fimp=false;
	int fixnode, swapnode;
	int num_cl=0;
	int delta_p=-1;



	for(int i=0;i<S->routeS[indroute].Nsel;i++){
		fixnode=S->routeS[indroute].perm[i];

		num_cl=0;
		Fimp=false;
		for(int j=0;j<S->routeS[indroute].Nsel;j++){
			swapnode=S->routeS[indroute].perm[j];
			if(abs(S->position[1][fixnode]-S->position[1][swapnode])<=1) continue;	//filter the adjacent nodes
			if(check_time()) return true;

			delta_p=switch_2opt(S,indroute,fixnode, swapnode);

			if(delta_p<0){
				switch_ope2opt(S,indroute,fixnode, swapnode);
				Fimp=true;
				S->update_profit(indroute);
//				check_move(S);
				Update_BestS(S,0);
				if(S->totalprofit>CBS->totalprofit) CBS->totalprofit=S->totalprofit;
				if(S->totalprofit>LBS->totalprofit) LBS->totalprofit=S->totalprofit;
			}
			else if(S->totalprofit-delta_p>threshold){
				CandiList[num_cl]=swapnode;
				num_cl++;
			}
		}
		if(!Fimp && num_cl>0){
			swapnode=CandiList[rand()%num_cl];
			switch_ope2opt(S,indroute,fixnode, swapnode);
			S->update_profit(indroute);
			Update_BestS(S,0);
			if(S->totalprofit>CBS->totalprofit) CBS->totalprofit=S->totalprofit;
		}
	}

	return true;

}


