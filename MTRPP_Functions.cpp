/*
 * MTRPP_Functions.cpp
 *
 *  Created on: Jan 19, 2021
 *      Author: jiren
 */

#include "mtrpp_lib.h"
#include "MTRPP_Functions.h"
#include "Utility.h"

/*functions*/

void cover_detail(void){
//	double tempt=get_time();
//	while(totalT<tempt){
//		vec_seqV[totalT]=BestS->totalprofit;
//		totalT++;
//	}
}

int try_vec_profit(int *arr){
	int profit_get=0;
	int nowcity, nexcity;
	int ind_nex=0;
	int acc_dis=0;

	acc_dis=0;
	nowcity=0;
	ind_nex=1;
	nexcity=arr[ind_nex];
	while(nexcity!=-1){
		acc_dis+=glo_map->matrix_dis[nowcity][nexcity];
		if(glo_map->profitA[nexcity]-acc_dis>0) profit_get+=glo_map->profitA[nexcity]-acc_dis;
		nowcity=nexcity;
		ind_nex++;
		nexcity=arr[ind_nex];
		if(ind_nex>num_city+1){
			printf("error in fetch_profit\n");
			exit(-2);
		}
	}

	return profit_get;
}

void Update_BestS(struct_sol *S, int ite){

	double tget;

	for(int i=0;i<num_car;i++) if(S->routeS[i].perm[0]!=0){
		fprintf(stderr, "error in perm[0]!=0");
		exit(-2);
	}

//	check_move(S);

	if(BestS->totalprofit<S->totalprofit){
		duplicate_solution(S,BestS);
//		layout_solution(BestS);
		tget=get_time();
		printf("%d %d %f %d\n",ite,S->totalprofit, tget,S->totalNsel);
		ofstream caout(name_final_result,ios::out|ios::app);
		if (caout.is_open()){
			caout<<"1 ";
			caout<<BestS->totalprofit<<" ";
			caout<<tget<<endl;
			caout.close();
		}
	}
}

void duplicate_solution(struct_sol *fromSol, struct_sol *toSol){
	toSol->totalNsel=fromSol->totalNsel;
	toSol->totalprofit=fromSol->totalprofit;
	toSol->totalcdis=fromSol->totalcdis;

	copyarray(fromSol->f_city, toSol->f_city, num_city);
	copymatrix(fromSol->position, toSol->position, 2, num_city);
	for(int i=0;i<num_car;i++){
		copyarray(fromSol->routeS[i].perm, toSol->routeS[i].perm, num_city+1);
		copyarray(fromSol->routeS[i].vec_sd, toSol->routeS[i].vec_sd, num_city);
		copyarray(fromSol->routeS[i].Sb, toSol->routeS[i].Sb, num_city);
		copyarray(fromSol->routeS[i].Se, toSol->routeS[i].Se, num_city);

		toSol->routeS[i].profit=fromSol->routeS[i].profit;
		toSol->routeS[i].Nsel=fromSol->routeS[i].Nsel;
		toSol->routeS[i].cdis=fromSol->routeS[i].cdis;
	}
}

void copyarray(int *fromv, int *tov, int l){
	for (int i=0;i<l;i++) tov[i]=fromv[i];
}

void copymatrix(int **frommat, int **tomat, int l1, int l2){
	for (int i=0;i<l1;i++)
		for (int j=0;j<l2;j++)
			tomat[i][j]=frommat[i][j];
}

void ExcGene(int *lvec, int l){
	int exc[l];
	int i;
	int temp;

	for(i=0;i<l;i++) exc[i]=rand()%l;

	for(i=0;i<l;i++) {
		temp=lvec[i];
		lvec[i]=lvec[exc[i]];
		lvec[exc[i]]=temp;
	}
}

