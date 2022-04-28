//============================================================================
// Name        : MTRPP_basic.cpp
// Author      : Jintong
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "mtrpp_lib.h"
#include "MTRPP_search.h"
#include "Utility.h"


int parameters(int count, char *arguments[])  {
	char *temp, filename[80] = "no";
	char *nf=filename;

	strcpy(resultsFile, filename);
	strcpy(benchmark, filename);
	strcpy(nf, filename);

	while (count != 1) {
		temp = arguments[count - 2];
		if (strcmp(temp,"-i") == 0) strcpy(benchmark, arguments[count - 1]);
		else
			if (strcmp(temp,"--seed") == 0) seed= atoi(arguments[count - 1]);
		else
			if (strcmp(temp,"-rep") == 0) rep=arguments[count - 1];
//		else
//			if (strcmp(temp,"-m") == 0) num_car= atoi(arguments[count - 1]);
//		else
//			if (strcmp(temp,"-lnf") == 0) NumCandidateSet_NF=atoi(arguments[count - 1]);
//		else
//			if (strcmp(temp,"-llk") == 0) NumCandidateSet_LK=atoi(arguments[count - 1]);
//		else
//			if (strcmp(temp,"-fpm") == 0) Para_M=atof(arguments[count - 1]);
		// else
		// 	if (strcmp(temp,"-ip1") == 0) ip1=atoi(arguments[count - 1]);
		// else
		// 	if (strcmp(temp,"-ip2") == 0) ip2=atoi(arguments[count - 1]);
		// else
		// 	if (strcmp(temp,"-rdg") == 0) rdg=atoi(arguments[count - 1]);
		// else
		// 	if (strcmp(temp,"-p") == 0) numpop=atoi(arguments[count - 1]);
		// else
		// 	if (strcmp(temp,"-alpha") == 0) alpha=atof(arguments[count - 1]);
//		else
//			if (strcmp(temp,"-iMs") == 0) Max_candidate=atoi(arguments[count - 1]);
//		else
//			if (strcmp(temp,"-iSs") == 0) Max_smallset=atoi(arguments[count - 1]);
//		else
//			if (strcmp(temp,"-idlk") == 0) dep_lk=atoi(arguments[count - 1]);
//		else
//			if (strcmp(temp,"-idgd") == 0) diameter_gd=atoi(arguments[count - 1]);
//		else
//			if (strcmp(temp,"-ipb1") == 0) perturb1=atoi(arguments[count - 1]);
//		else
//			if (strcmp(temp,"-ipb2") == 0) perturb2=atoi(arguments[count - 1]);
//		else
//			if (strcmp(temp,"-ipb3") == 0) perturb3=atoi(arguments[count - 1]);
//		else
//			if (strcmp(temp,"-ipb4") == 0) perturb4=atoi(arguments[count - 1]);
//		else
//			if (strcmp(temp,"-alpha") == 0) Alpha=atof(arguments[count - 1]);
//		else
//			if (strcmp(temp,"-fph") == 0) Param_H=atof(arguments[count - 1]);
//		else
//			if (strcmp(temp,"-fpb") == 0) Beta=atof(arguments[count - 1]);
//		else
//			if (strcmp(temp,"-s") == 0) strcpy(inputsol, arguments[count - 1]);
//		else
//			if (strcmp(temp,"-p") == 0) strcpy(pure_name, arguments[count - 1]);
		else {  // unknow parameter
			return 0;
		}
		count = count - 2;
	}
	if (strcmp(benchmark, "no") == 0 || (alreadybest==-1)) {
		printf("enter error\n");
		exit(-1);
	}
	strcpy(nf, benchmark);

	rdg=2;					// Limi in the article
	ip1=11;					// St
	numpop=10;				// Np

	alpha=1.0;				// not used in this work, in fact, we have also try a distance-quality strategy
	ip2=10;					// not used in this work
	return 1;
}

int main(int argc, char **argv){
	char final[50]="F";
	char allal[50]="A";

	double allt=0.0;
	int allprofit=-1;
	if(parameters(argc, argv)==0) exit(-1);
	glo_map=new struct_map();

	srand(seed);
//	srand(time(NULL));
	cuttime=(num_city-1)*4.0;
	vec_seqV=new int[(num_city-1)*2];
	memset(vec_seqV, -1, (num_city-1)*2*sizeof(int));
	totalT=0;
	/**************************/
	strcpy(name_final_result,benchmark);
	strcat(name_final_result,final);
	strcat(name_final_result,rep);
	printf("%s\n",name_final_result);
	/**************************/
	strcpy(detail_running_profil,benchmark);
	strcat(detail_running_profil,allal);
	strcat(detail_running_profil,rep);
	printf("%s\n",detail_running_profil);

//	glo_map->layoutmap();
//	cuttime=10.0;

	Para_M=1.0;

	t_swap=0.0;
	t_2opt=0.0;
	t_insert=0.0;
	t_oropt=0.0;
	t_add=0.0;
	t_drop=0.0;
	t_interswap=0.0;
	t_interinsert=0.0;
	t_inter2opt=0.0;
	t_interoropt=0.0;
	t_doublebridge=0.0;

	allprofit=main_search();
	allt=get_time();
	printf("Finish Time=%f, Final Profit=%d\n", allt, allprofit);
	printf("Time swap  =\t%f, \t percent=%f\n", t_swap, t_swap*100.0/allt);
	printf("Time 2opt  =\t%f, \t percent=%f\n", t_2opt, t_2opt*100.0/allt);
	printf("Time insert=\t%f, \t percent=%f\n", t_insert, t_insert*100.0/allt);
	printf("Time oropt =\t%f, \t percent=%f\n", t_oropt, t_oropt*100.0/allt);
	printf("Time add   =\t%f, \t percent=%f\n", t_add, t_add*100.0/allt);
	printf("Time drop  =\t%f, \t percent=%f\n", t_drop, t_drop*100.0/allt);
	printf("Time interx=\t%f, \t percent=%f\n", t_interswap, t_interswap*100.0/allt);
	printf("Time interi=\t%f, \t percent=%f\n", t_interinsert, t_interinsert*100.0/allt);
	printf("Time inter2=\t%f, \t percent=%f\n", t_inter2opt, t_inter2opt*100.0/allt);
	printf("Time intero=\t%f, \t percent=%f\n", t_interoropt, t_interoropt*100.0/allt);
	printf("Time double=\t%f, \t percent=%f\n", t_doublebridge, t_doublebridge*100.0/allt);
	printf("Time other =\t%f, \t percent=%f\n", allt-t_swap-t_2opt-t_insert-t_oropt-t_add-t_drop-t_interswap-t_interinsert-t_inter2opt-t_interoropt-t_doublebridge, (allt-t_swap-t_2opt-t_insert-t_oropt-t_add-t_drop-t_interswap-t_interinsert-t_inter2opt-t_interoropt-t_doublebridge)*100.0/allt);

//	ofstream allo(detail_running_profil,ios::out|ios::trunc);
//	if (allo.is_open()){
//		for(int i=0;i<(num_city-1)*2;i++) allo<<i<<" "<<vec_seqV[i]<<endl;
//		allo.close();
//	}


	delete glo_map;
	delete []vec_seqV;
	return 0;
}
