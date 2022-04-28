/*
 * PERTURB_H_.cpp
 *
 *  Created on: Feb 3, 2021
 *      Author: jiren
 */


#include "Perturb.h"
#include "mtrpp_lib.h"
#include "MTRPP_Functions.h"
#include "Utility.h"


/*declaration of functions*/
void Perturb_add(struct_sol *S);
void Perturb_alladd(struct_sol *S);
void Perturb_insert(struct_sol *S);
void Perturb_drop(struct_sol *S);
void Perturb_interx(struct_sol *S);
void Perturb_interi(struct_sol *S);
void Perturb_swap(struct_sol *S);
void Perturb_2opt(struct_sol *S);

void add_oneroute(struct_sol *S, int indroute);
void insert_oneroute(struct_sol *S, int indroute);
void drop_oneroute(struct_sol *S, int indroute);
void interx_onefois(struct_sol *S);
void interi_onefois(struct_sol *S);
void swap_oneroute(struct_sol *S, int indroute);
void Topt_oneroute(struct_sol *S, int indroute);


/*main functions*/
void Perturb(struct_sol *S){
//	printf("one route\n");
//	S->fetch_vec_sd();
	Perturb_insert(S);
//	Perturb_swap(S);
//	Perturb_2opt(S);
//	Perturb_drop(S);

//	Perturb_add(S);
	Perturb_alladd(S);

//	Perturb_interx(S);
//	Perturb_interi(S);

}

void Perturb_add(struct_sol *S){
	S->fetch_vec_sd();
	int indroute=-1;
	for(int i=0;i<ip2;i++){
		indroute=rand()%num_car;
		add_oneroute(S,indroute);
	}
}

void Perturb_alladd(struct_sol *S){
	S->fetch_vec_sd();
	if(S->totalNsel>num_city) {
		fprintf(stderr,"error in totalNsel, exceed the maximum number of city.\n");
		exit(-2);
	}
	if(S->totalNsel==num_city) return;

	int nodeadd, posadd;
	int indroute;

	for(int i=0;i<num_city;i++){
		if(S->f_city[i]==-1) {
			nodeadd=i;
			indroute=rand()%num_car;
			posadd=rand()%S->routeS[indroute].Nsel+1;
			S->move_add(indroute, nodeadd, posadd);
			S->update_profit(indroute);
		}
	}

}

void Perturb_insert(struct_sol *S){
	S->fetch_vec_sd();
	int indroute=-1;
	for(int i=0;i<ip1;i++){
		indroute=rand()%num_car;
		insert_oneroute(S,indroute);
//		layout_solution(S);
	}
}

void Perturb_drop(struct_sol *S){
	int indroute=-1;
	for(int i=0;i<ip1*2;i++){
		indroute=rand()%num_car;
		drop_oneroute(S,indroute);
	}
}

void Perturb_interx(struct_sol *S){

	S->fetch_vec_sd();
//	int maxi_interx=0.2*S->totalNsel;
	int maxi_interx=10;
	for(int i=0;i<maxi_interx;i++) interx_onefois(S);
}


void Perturb_interi(struct_sol *S){
	S->fetch_vec_sd();
//	int maxi_interx=0.2*S->totalNsel;
	int maxi_interx=10;
	for(int i=0;i<maxi_interx;i++) interi_onefois(S);
}

void Perturb_swap(struct_sol *S){
	S->fetch_vec_sd();
	int max_ip=10;
	int indroute=-1;
	for(int i=0;i<max_ip;i++){
		indroute=rand()%num_car;
		swap_oneroute(S,indroute);
//		layout_solution(S);
	}
}

void Perturb_2opt(struct_sol *S){
	S->fetch_vec_sd();
	S->fetch_SbSe();
	int max_ip=10;
	int indroute=-1;
	for(int i=0;i<max_ip;i++){
		indroute=rand()%num_car;
		Topt_oneroute(S,indroute);
//		layout_solution(S);
	}
}

void add_oneroute(struct_sol *S, int indroute){
	if(S->totalNsel>num_city) {
		fprintf(stderr,"error in totalNsel, exceed the maximum number of city.\n");
		exit(-2);
	}
	if(S->totalNsel==num_city) return;

	int nodeadd, posadd;

	for(int i=0;i<num_city;i++){
		if(S->f_city[i]==-1) {
			nodeadd=i;
			break;
		}
	}
	posadd=rand()%S->routeS[indroute].Nsel+1;
	S->move_add(indroute, nodeadd, posadd);
	S->update_profit(indroute);
//	check_move(S);
}

void insert_oneroute(struct_sol *S, int indroute){
	int movpos, fixpos;

	if(S->routeS[indroute].Nsel<3) return;

	movpos=rand()%(S->routeS[indroute].Nsel-1)+1;
	do{
		fixpos=rand()%(S->routeS[indroute].Nsel-1)+1;
	}while(fixpos==movpos);

	S->move_insert_oropt(movpos, fixpos, indroute, 1);
	S->update_profit(indroute);
//	check_move(S);

}

void drop_oneroute(struct_sol *S, int indroute){
	int N_drop=0.4*S->routeS[indroute].Nsel;
	int idx_pos;

	for(int i=0;i<N_drop;i++){
		idx_pos=rand()%(S->routeS[indroute].Nsel-1)+1;
		S->move_drop(indroute, S->routeS[indroute].perm[idx_pos], idx_pos);
		S->update_profit(indroute);
	}

}
void interx_onefois(struct_sol *S){
	int route1, route2;
	int pos1, pos2;
	int N1, N2;

	route1=rand()%num_car;
	do{
		route2=rand()%num_car;
	}while(route1==route2);

	N1=S->routeS[route1].Nsel;
	N2=S->routeS[route2].Nsel;

	if(N1<2 || N2<2 ) return;

	pos1=rand()%(N1-1)+1;
	pos2=rand()%(N2-1)+1;

	S->move_interswap(S->routeS[route1].perm[pos1], S->routeS[route2].perm[pos2], route1, route2, pos1, pos2);
	S->update_profit(route1);
	S->update_profit(route2);
//	check_move(S);

}

void interi_onefois(struct_sol *S){
	int route1, route2;
	int pos1, pos2;
	int N1, N2;

	route1=rand()%num_car;
	do{
		route2=rand()%num_car;
	}while(route1==route2);

	N1=S->routeS[route1].Nsel;
	N2=S->routeS[route2].Nsel;

	if(N1<2) return;

	pos1=rand()%(N1-1)+1;
	pos2=rand()%N2;

	S->move_interinsert(S->routeS[route1].perm[pos1], S->routeS[route2].perm[pos2], route1, route2, pos1, pos2);
	S->update_profit(route1);
	S->update_profit(route2);
//	check_move(S);
}

void swap_oneroute(struct_sol *S, int indroute){
	int pos1, pos2;

	if(S->routeS[indroute].Nsel<2) return;

	pos1=rand()%(S->routeS[indroute].Nsel-1)+1;
	do{
		pos2=rand()%(S->routeS[indroute].Nsel-1)+1;
	}while(pos1==pos2);

	if(abs(pos1-pos2)==1) S->move_swapadj(indroute, S->routeS[indroute].Nsel, S->routeS[indroute].perm, pos1, pos2, S->routeS[indroute].perm[pos1], S->routeS[indroute].perm[pos2]);
	else S->move_swap(indroute, S->routeS[indroute].Nsel, S->routeS[indroute].perm, pos1, pos2, S->routeS[indroute].perm[pos1], S->routeS[indroute].perm[pos2]);
//	S->move_insert_oropt(movpos, fixpos, indroute, 1);
	S->update_profit(indroute);
//	check_move(S);
}

void Topt_oneroute(struct_sol *S, int indroute){
	int pos1, pos2;

	if(S->routeS[indroute].Nsel<3) return;

	pos1=rand()%(S->routeS[indroute].Nsel);		//it could be the depot
	do{
		pos2=rand()%(S->routeS[indroute].Nsel);
	}while(pos1==pos2);
	if(abs(pos1-pos2)<1) return;

	if(pos1<pos2) S->exe_2opt(S,indroute,S->routeS[indroute].perm[pos1],S->routeS[indroute].perm[pos2]);
	else S->exe_2opt(S,indroute,S->routeS[indroute].perm[pos2],S->routeS[indroute].perm[pos1]);
	S->update_profit(indroute);
//	check_move(S);

}
