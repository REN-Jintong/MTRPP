/*
 * Utility.cpp
 *
 *  Created on: Feb 3, 2021
 *      Author: jiren
 */


#include "Utility.h"
#include "mtrpp_lib.h"
#include "MTRPP_Functions.h"


double get_time(void){
	double endTimeSeconds=0.0;
	glo_end=clock();
	endTimeSeconds = (glo_end-glo_start)*1.0/CLOCKS_PER_SEC;
	return (endTimeSeconds);
}

bool check_time(void){
	double timenow=get_time();
	if(timenow>cuttime || BestS->totalprofit>=alreadybest) return true;
	else return false;
	return true;
}

bool check_move(struct_sol *S){

	//check the indicator for one solution after one move
	struct_sol tmpS;
	duplicate_solution(S, &tmpS);

//	cout<<"start check"<<endl;

	for(int i=1;i<num_city;i++){
		if(S->position[0][i]==-1) continue;
		if(S->routeS[S->position[0][i]].perm[S->position[1][i]]!=i){
			fprintf(stderr ,"error in position, node1=%d\n", i);
//			fprintf(stderr ,"error in position, node1=%d\n", i);
		}
	}


	for(int i=1;i<num_city;i++){
		if(S->position[0][i]==-1) continue;
		for(int j=i+1;j<num_city;j++){
			if(S->position[0][i]==S->position[0][j] && S->position[1][i]==S->position[1][j]){
				fprintf(stderr ,"error in duplicate position, node1=%d, node2=%d\n", i,j);
//				fprintf(stderr ,"error in duplicate position, node1=%d, node2=%d\n", i,j);
			}
		}
	}


	S->fetch_all();
//	layout_solution(S);

	for(int i=0;i<num_car;i++){
		//check the vec_sd
		for(int j=0;j<num_city;j++){

//			if(S->routeS[i].vec_sd[j]!=tmpS.routeS[i].vec_sd[j]) fprintf(stderr ,"error in vec_sd, %d car, %d pos\n", i, j);
//			if(S->routeS[i].Sb[j]!=tmpS.routeS[i].Sb[j]) fprintf(stderr ,"error in Sb, %d car, %d pos\n", i, j);
//			if(S->routeS[i].Se[j]!=tmpS.routeS[i].Se[j]) fprintf(stderr ,"error in Se, %d car, %d pos\n", i, j);
		}
		if(S->routeS[i].perm[0]!=0) {
			fprintf(stderr ,"error in depot not 0, %d car\n", i);
//			fprintf(stderr ,"error in depot not 0, %d car\n", i);
		}
		if(S->routeS[i].profit!=tmpS.routeS[i].profit) fprintf(stderr ,"error in profit, %d car\n", i);
		if(S->routeS[i].Nsel!=tmpS.routeS[i].Nsel) fprintf(stderr ,"error in Nsel, %d car\n", i);
		if(S->routeS[i].cdis!=tmpS.routeS[i].cdis) {
			fprintf(stderr ,"error in cdis, %d car\n", i);
//			fprintf(stderr ,"error in cdis, %d car\n", i);
		}
	}

	for(int i=0;i<num_city;i++) if(S->position[0][i]!=tmpS.position[0][i]) fprintf(stderr ,"error in position[0], %d pos\n", i);
	for(int i=0;i<num_city;i++) if(S->position[1][i]!=tmpS.position[1][i]) fprintf(stderr ,"error in position[1], %d pos\n", i);
	for(int i=0;i<num_city;i++) if(S->f_city[i]!=tmpS.f_city[i]) fprintf(stderr ,"error in f_city, %d pos\n", i);

	if(S->totalprofit!=tmpS.totalprofit) fprintf(stderr ,"error in totalprofit\n");
	if(S->totalNsel!=tmpS.totalNsel) fprintf(stderr ,"error in totalNsel\n");
	if(S->totalcdis!=tmpS.totalcdis) fprintf(stderr ,"error in totalcdis\n");

	return false;
}

void layout(struct_sol *S){
	for(int i=0;i<num_city;i++){
		for(int j=0;j<num_city;j++){
			printf("%d\t",glo_map->matrix_dis[i][j]);
		}
		cout<<endl;
	}


	for(int i=0;i<num_car;i++){
		cout<<"Sb=";
		for(int j=0;j<num_city;j++){
			printf("%d ",S->routeS[i].Sb[j]);
		}
		cout<<endl;
	}

	cout<<endl;
	for(int i=0;i<num_car;i++){
		cout<<"Se=";
		for(int j=0;j<num_city;j++){
			printf("%d ",S->routeS[i].Se[j]);
		}
		cout<<endl;
	}
	cout<<endl;
	for(int i=0;i<num_car;i++){
		cout<<"Vec=";
		for(int j=0;j<num_city;j++){
			printf("%d ",S->routeS[i].vec_sd[j]);
		}
		cout<<endl;
	}
	cout<<endl;
}

void layout_solution(struct_sol *S){
	S->fetch_all();

	printf("obj=%d\n", S->totalprofit);
	printf("Nsel=%d\n", S->totalNsel);

	for(int i=0;i<num_car;i++){
		printf("Route  %d=",i);
		for(int j=0;j<S->routeS[i].Nsel;j++){
			printf("%d\t",S->routeS[i].perm[j]);
		}
//		cout<<endl;
//		printf("Profit %d=",i);
//		for(int j=0;j<num_city;j++){
//			printf("%d\t",glo_map->profitA[S->routeS[i].perm[j]]-S->routeS[i].vec_sd[j]);
//		}
		cout<<endl;
		cout<<endl;
	}

	for(int i=0;i<num_city;i++){
		printf("%d\t",i);
	}
	cout<<endl;
	for(int i=0;i<num_city;i++){
		printf("%d\t",S->position[0][i]);
	}
	cout<<endl;
	for(int i=0;i<num_city;i++){
		printf("%d\t",S->position[1][i]);
	}
	cout<<endl<<endl;

}



