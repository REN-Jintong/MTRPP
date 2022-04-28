/*
 * MTRPP_search.cpp
 *
 *  Created on: Jan 21, 2021
 *      Author: jiren
 */

#include "mtrpp_lib.h"
#include "MTRPP_Functions.h"
#include "Utility.h"
#include "Operator/Ope_2opt.h"
#include "Operator/Ope_swap.h"
#include "Operator/Ope_insert.h"
#include "Operator/Ope_oropt.h"
#include "Operator/Ope_add.h"
#include "Operator/Ope_drop.h"
#include "Operator/Ope_interswap.h"
#include "Operator/Ope_interinsert.h"
#include "Operator/Ope_inter2opt.h"
#include "Operator/Ope_interoropt.h"
#include "Operator/Ope_doublebridge.h"
#include "Perturb.h"

void normal_search(struct_sol *S);
void dgma_search(struct_sol *S);
void pr_search(struct_sol *S);
void RVNS(struct_sol *S);
void DVNS(struct_sol *S);
void TVNS(struct_sol *S);
int arr_order[11];

/*main search functions*/
void TVNS(struct_sol *S){

	int tmpbest=S->totalprofit;

	do{
		tmpbest=S->totalprofit;
		DVNS(S);
		if(check_time()) return;
	}while(S->totalprofit>tmpbest);

}

void RVNS(struct_sol *S){
	ExcGene(arr_order, 11);
	double tmpt=0.0;

	for(int i=0;i<11;i++){
		switch (arr_order[i]){
		case 0:
			tmpt=get_time();
			NF_2opt(S);
			t_2opt+=get_time()-tmpt;

			if(check_time()) return;
			break;
		case 1:
			tmpt=get_time();
			NF_swap(S);
			t_swap+=get_time()-tmpt;

			if(check_time()) return;
			break;
		case 2:
			tmpt=get_time();
			NF_insert(S);
			t_insert+=get_time()-tmpt;

			if(check_time()) return;
			break;
		case 3:
			tmpt=get_time();
			NF_oropt(S);
			t_oropt+=get_time()-tmpt;

			if(check_time()) return;
			break;
		case 4:
			tmpt=get_time();
			NF_add(S);
			t_add+=get_time()-tmpt;

			if(check_time()) return;
			break;
		case 5:
			tmpt=get_time();
			NF_drop(S);
			t_drop+=get_time()-tmpt;

			if(check_time()) return;
			break;
		case 6:
			tmpt=get_time();
			NF_interswap(S);
			t_interswap+=get_time()-tmpt;

			if(check_time()) return;
			break;
		case 7:
			tmpt=get_time();
			NF_interinsert(S);
			t_interinsert+=get_time()-tmpt;

			if(check_time()) return;
			break;
		case 8:
			tmpt=get_time();
			NF_inter2opt(S);
			t_inter2opt+=get_time()-tmpt;

			if(check_time()) return;
			break;
		case 9:
			tmpt=get_time();
//			NF_interoropt(S);
			t_interoropt+=get_time()-tmpt;

			if(check_time()) return;
			break;
		case 10:
			tmpt=get_time();
//			NF_doublebridge(S);
			t_doublebridge+=get_time()-tmpt;

			if(check_time()) return;
			break;
		default:
			break;
		}
	}
}

void DVNS(struct_sol *S){
	ExcGene(arr_order, 11);
	double tmpt=0.0;

	tmpt=get_time();
	NF_add(S);
	t_add+=get_time()-tmpt;
	cover_detail();

	for(int i=0;i<11;i++){
		switch (arr_order[i]){
		case 0:
			tmpt=get_time();
			NF_2opt(S);
			t_2opt+=get_time()-tmpt;
			cover_detail();

			tmpt=get_time();
			NF_drop(S);
			t_drop+=get_time()-tmpt;
			cover_detail();

			if(check_time()) return;
			break;
		case 1:
			tmpt=get_time();
			NF_swap(S);
			t_swap+=get_time()-tmpt;
			cover_detail();

			tmpt=get_time();
			NF_drop(S);
			t_drop+=get_time()-tmpt;
			cover_detail();

			if(check_time()) return;
			break;
		case 2:
			tmpt=get_time();
			NF_insert(S);
			t_insert+=get_time()-tmpt;
			cover_detail();

			tmpt=get_time();
			NF_drop(S);
			t_drop+=get_time()-tmpt;
			cover_detail();

			if(check_time()) return;
			break;
		case 3:
			tmpt=get_time();
			NF_oropt(S);
			t_oropt+=get_time()-tmpt;
			cover_detail();

			tmpt=get_time();
			NF_drop(S);
			t_drop+=get_time()-tmpt;
			cover_detail();

			if(check_time()) return;
			break;
		case 4:
//			tmpt=get_time();
//			NF_add(S);
//			t_add+=get_time()-tmpt;
//
//			tmpt=get_time();
//			NF_drop(S);
//			t_drop+=get_time()-tmpt;

			if(check_time()) return;
			break;
		case 5:
			tmpt=get_time();
			NF_drop(S);
			t_drop+=get_time()-tmpt;
			cover_detail();

			tmpt=get_time();
			NF_drop(S);
			t_drop+=get_time()-tmpt;
			cover_detail();

			if(check_time()) return;
			break;
		case 6:
			tmpt=get_time();
			NF_interswap(S);
			t_interswap+=get_time()-tmpt;
			cover_detail();

			tmpt=get_time();
			NF_drop(S);
			t_drop+=get_time()-tmpt;
			cover_detail();

			if(check_time()) return;
			break;
		case 7:
			tmpt=get_time();
			NF_interinsert(S);
			t_interinsert+=get_time()-tmpt;
			cover_detail();

			tmpt=get_time();
			NF_drop(S);
			t_drop+=get_time()-tmpt;
			cover_detail();

			if(check_time()) return;
			break;
		case 8:
			tmpt=get_time();
			NF_inter2opt(S);
			t_inter2opt+=get_time()-tmpt;
			cover_detail();

			tmpt=get_time();
			NF_drop(S);
			t_drop+=get_time()-tmpt;
			cover_detail();

			if(check_time()) return;
			break;
		case 9:
			tmpt=get_time();
//			NF_interoropt(S);
			t_interoropt+=get_time()-tmpt;

			if(check_time()) return;
			break;
		case 10:
			tmpt=get_time();
//			NF_doublebridge(S);
			t_doublebridge+=get_time()-tmpt;

			if(check_time()) return;
			break;
		default:
			break;
		}
	}
}

void normal_search(struct_sol *S){
	int tmpbest=0;

	while(1){

		do{
			tmpbest=S->totalprofit;
			RVNS(S);
			if(check_time()) return;
		}while(S->totalprofit>tmpbest);

//		check_move(S);
		Perturb(S);
//		check_move(S);
	}
}

void dgma_search(struct_sol *S){

	int Ct=0;
	int Radius=rdg;

	int tmpbest=S->totalprofit;
	int lbest=S->totalprofit;

	while(1){

		do{
			tmpbest=S->totalprofit;
//			RVNS(S);
 			DVNS(S);
			if(check_time()) return;
		}while(S->totalprofit>tmpbest);
		check_move(S);
//		NF_drop(S);
//		layout_solution(S);

		if(tmpbest>lbest){
			Ct=0;
			lbest=tmpbest;
		}
		else if(Ct<Radius) Ct++;
		else if(Ct>=Radius){
			Ct=0;
			duplicate_solution(BestS,S);
		}
		Perturb(S);
//		check_move(S);
	}
}

void pr_search(struct_sol *S){
	int Ct=0;
	int Radius=rdg;
	int tmpbest=S->totalprofit;
	int lbest=S->totalprofit;

	duplicate_solution(S, LLBS);

	while(Ct<Radius){

		do{
			tmpbest=S->totalprofit;
 			DVNS(S);
// 			RVNS(S);

 			if(S->totalprofit>LLBS->totalprofit) duplicate_solution(S,LLBS);

			if(check_time()) return;
		}while(S->totalprofit>tmpbest);
//		check_move(S);
		Perturb(S);
//		TH_2opt(S);

		if(tmpbest>lbest){
			Ct=0;
			lbest=tmpbest;
		}
		else Ct++;
	}
}

int main_search(void){
	/*allocate the structure*/
	int gloprofit=-1;
	CurrentS=new struct_sol();
	BestS=new struct_sol();
	LBS=new struct_sol();
	LLBS=new struct_sol();
	CBS=new struct_sol();
	PopS=new Population();
	CandiList=new int[num_city];


	/*test for randomini and */

	/*initial solution*/
	BestS->totalprofit=-1;
//	CurrentS->grasp_inisol();
//	Update_BestS(CurrentS,0);
	for(int i=0;i<11;i++) arr_order[i]=i;
	for(int i=0;i<numpop;i++){
		if(rand()%2==0)	PopS->PSol[i].grasp_inisol();
		else PopS->PSol[i].random_inisol();
		TVNS(&PopS->PSol[i]);
	}
	PopS->Create_dismat();

//	for(int i=0;i<10;i++) {
//		CurrentS->reset();
//		PopS->Generate_offspring(CurrentS);
//		layout_solution(CurrentS);
//		check_move(CurrentS);
//
//	}
//
//	exit(0);

	/*initial the file*/
    ofstream caout(name_final_result,ios::out|ios::trunc);
	if (caout.is_open()){
		caout<<"0 ";
		caout<<BestS->totalprofit<<" ";
		caout<<get_time()<<endl;
		caout.close();
	}

	/*main search procedure*/

	while(get_time()<cuttime){
//		normal_search(CurrentS);
//		dgma_search(CurrentS);
		CurrentS->reset();
		PopS->Generate_offspring_arcbased(CurrentS);
//		PopS->Generate_offspring_routebased(CurrentS);
//		cout<<"oneGeneration:"<<get_time()<<endl;
//		cout<<"InputS:"<<CurrentS->totalprofit<<endl;
		pr_search(CurrentS);
//		dgma_search(CurrentS);
//		cout<<"OutputS:"<<LLBS->totalprofit<<" time="<<get_time()<<endl;
		PopS->diversification_update_sol(LLBS);
//		PopS->simple_update_pool(LLBS);
	}
	/*delete structure and return value*/
	BestS->fetch_profit();
	gloprofit=BestS->totalprofit;
	layout_solution(BestS);
	delete CurrentS;
	delete BestS;
	delete LBS;
	delete LLBS;
	delete CBS;
	delete PopS;
	delete []CandiList;
	return gloprofit;
}



