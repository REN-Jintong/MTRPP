/*
 * struct_map.cpp
 *
 *  Created on: Jan 29, 2021
 *      Author: jiren
 */
#include "../mtrpp_lib.h"



struct_map::struct_map(){

	ifstream FIC;
	int count_city=-1;
	int x,y,prof;

	FIC.open(benchmark);
	if ( FIC.fail() ){
	 cout << "### No way,check your fiche " << benchmark << endl;
	 exit(-1);
	}
	FIC>>count_city>>num_car;
	num_city=count_city+1;

	/*allocat the space*/
	profitA=new int [num_city];
	cordx=new int [num_city];
	cordy=new int [num_city];
	Num_pernode_nf=new int [num_city];
	Num_pernode_lk=new int [num_city];
	Fsel=new bool [num_city];

	matrix_dis=new int *[num_city];
	for(int i=0;i<num_city;i++) {
		matrix_dis[i]=new int [num_city];
		memset(matrix_dis[i],0,num_city*sizeof(int));
	}

	matrix_chosenT=new int *[num_city];
	for(int i=0;i<num_city;i++) {
		matrix_chosenT[i]=new int [num_city];
		memset(matrix_chosenT[i],0,num_city*sizeof(int));
	}

	CandidateSet_NF=new int *[num_city];
	for(int i=0;i<num_city;i++) {
		CandidateSet_NF[i]=new int [NumCandidateSet_NF];
		memset(CandidateSet_NF[i],-1,NumCandidateSet_NF*sizeof(int));
	}

	CandidateSet_LK=new int *[num_city];
	for(int i=0;i<num_city;i++) {
		CandidateSet_LK[i]=new int [NumCandidateSet_LK];
		memset(CandidateSet_LK[i],-1,NumCandidateSet_LK*sizeof(int));
	}

	Profit_dis=new double *[num_city];
	for(int i=0;i<num_city;i++) Profit_dis[i]=new double [num_city];


	/*read fiche*/
	for(int i=0;i<num_city;i++){
		FIC>>x>>y>>prof;
		cordx[i]=x;
		cordy[i]=y;
		profitA[i]=prof;
	}
	/*finished*/
	remplir_map();
}

struct_map::~struct_map(){
	delete[] profitA;
	delete[] cordx;
	delete[] cordy;
	delete[] Num_pernode_nf;
	delete[] Num_pernode_lk;
	delete[] Fsel;

	for(int i=0;i<num_city;i++) {
		delete[] matrix_dis[i];
		delete[] matrix_chosenT[i];
		delete[] CandidateSet_NF[i];
		delete[] CandidateSet_LK[i];
		delete[] Profit_dis[i];
	}
	delete[] matrix_dis;
	delete[] matrix_chosenT;
	delete[] CandidateSet_NF;
	delete[] CandidateSet_LK;
	delete[] Profit_dis;
}

void struct_map::remplir_map(void){
	/*remplir the matrix_dis*/
	for(int i=0;i<num_city;i++)
		for(int j=0;j<num_city;j++)
			if(i==j) matrix_dis[i][j]=10000000;
			else {
				matrix_dis[i][j]=round(sqrt((cordx[i]-cordx[j])*(cordx[i]-cordx[j])+(cordy[i]-cordy[j])*(cordy[i]-cordy[j])));
				matrix_dis[j][i]=matrix_dis[i][j];
			}
	/*remplir the Profit_dis*/

	for(int i=0;i<num_city;i++)
		for(int j=0;j<num_city;j++)
			if(i==j) Profit_dis[i][j]=0;
			else{
				if(Para_M!=0) Profit_dis[i][j]=profitA[j]*1.0/(Para_M*matrix_dis[i][j]);
				else Profit_dis[i][j]=profitA[j]*1.0/matrix_dis[i][j];
			}
	/*remplir the CandidateSet_NF and CandidateSet_LK*/
	int chosenCity=-1;

	for(int i=0;i<num_city;i++){
		Num_pernode_nf[i]=0;		//find the candidate_city for city i;
		for(int j=0;j<num_city;j++) Fsel[j]=false;
		Fsel[0]=true;
		for(int j=0;j<NumCandidateSet_NF;j++){
			chosenCity=FindGoodCity(i);
			if(chosenCity==-1) break;
			CandidateSet_NF[i][j]=chosenCity;
			Num_pernode_nf[i]++;
			Fsel[chosenCity]=true;
		}
	}

	for(int i=0;i<num_city;i++){
		Num_pernode_lk[i]=0;		//find the candidate_city for city i;
		for(int j=0;j<num_city;j++) Fsel[j]=false;
		Fsel[0]=true;
		for(int j=0;j<NumCandidateSet_LK;j++){
			chosenCity=FindGoodCity(i);
			if(chosenCity==-1) break;
			CandidateSet_LK[i][j]=chosenCity;
			Num_pernode_lk[i]++;
			Fsel[chosenCity]=true;
		}
	}
}

int struct_map::FindGoodCity(int nowc){
	int cityFind=-1;
	int i;
	for(i=0;i<num_city;i++){
		if(i==nowc) continue;
		if(cityFind==-1 && Fsel[i]==false) cityFind=i;
		else if(cityFind!=-1 && Fsel[i]==false) {
			if(Profit_dis[nowc][i]>Profit_dis[nowc][cityFind]) cityFind=i;
		}
	}
	return cityFind;
}

void struct_map::layoutmap(){

	printf("\n");
	for(int i=0;i<num_city;i++){
		for(int j=0;j<num_city;j++)
					printf("%d ",matrix_dis[i][j]);
		printf("\n");
	}
}

void struct_map::ini_chosenT(void){

	for(int i=0;i<num_city;i++) memset(matrix_chosenT[i],0,num_city*sizeof(int));

}

