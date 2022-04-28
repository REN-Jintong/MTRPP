/*
 * struct_sol.cpp
 *
 *  Created on: Jan 29, 2021
 *      Author: jiren
 */

#include "../mtrpp_lib.h"
#include "../Operator/Ope_add.h"

Population::Population(){

	PSol=new struct_sol[numpop];
	dismat=new double*[numpop];
	for(int i=0;i<numpop;i++) {
		dismat[i]=new double[numpop];
		memset(dismat[i], -1, numpop*sizeof(double));
	}
//	avg_dis=new double[numpop];
//	memset(avg_dis, -1, numpop*sizeof(double));
//	quality=new double[numpop];
//	memset(quality, -1, numpop*sizeof(double));
//	score=new double[numpop];
//	memset(score, -1, numpop*sizeof(double));
}


Population::~Population(){

	delete [] PSol;
	for(int i=0;i<numpop;i++) delete[] dismat[i];
	delete [] dismat;
//	delete [] avg_dis;
//	delete [] quality;
//	delete [] score;
}

void Population::Ini_Pop(void){
	for(int i=0;i<numpop;i++)
		PSol->grasp_inisol();

}

void Population::Create_dismat(void){

	for(int i=0;i<numpop;i++) memset(dismat[i], -1, numpop*sizeof(double));

	for(int i=0;i<numpop;i++)
		for(int j=i;j<numpop;j++){
			if(i==j) dismat[i][j]=1.0;
			else{
				dismat[i][j]=get_dis_arc(&PSol[i], &PSol[j]);
				dismat[j][i]=dismat[i][j];
			}
		}

//	/*get the avg_dis*/
//	for(int i=0;i<numpop;i++){
//		tmpsum=0.0;
//		for(int j=0;j<numpop;j++){
//			if(i==j) continue;
//			tmpsum+=dismat[i][j];
//		}
//		avg_dis[i]=tmpsum*1.0/(numpop-1);
//	}
//	/*get the quality*/
//	max_obj=PSol[0].totalprofit;
//	min_obj=PSol[0].totalprofit;
//
//	for(int i=1; i<numpop; i++){
//		max_obj=max(max_obj, PSol[i].totalprofit);
//		min_obj=min(max_obj, PSol[i].totalprofit);
//	}
//	delta_obj=max_obj-min_obj;
//	for(int i=0;i<numpop;i++) quality[i]=(PSol[i].totalprofit-min_obj)*1.0/delta_obj;
//
//	/*get the score*/
//	for(int i=0;i<numpop;i++) score[i]=alpha*quality[i]+(1.0-alpha)*avg_dis[i];

}

void Population::diversification_update_sol(struct_sol *S){
	double tmpsum=0.0;
	int max_obj, min_obj;
	int delta_obj=-1;
	double vec_dis[numpop];
	double avg_new[numpop+1];
	double norm_quality[numpop+1];
	double score_new[numpop+1];
	int idx;
	double worstscore;

	for(int i=0;i<numpop;i++){
	       	vec_dis[i]=get_dis_arc(&PSol[i], S);
		if(vec_dis[i]<1e-5) return;
	}
	/*get the avg_dis*/
	for(int i=0;i<numpop;i++){
		tmpsum=0.0;
		for(int j=0;j<numpop;j++){
			if(i==j) continue;
			tmpsum+=dismat[i][j];
		}
		tmpsum+=vec_dis[i];
		avg_new[i]=tmpsum*1.0/(numpop);
	}

	tmpsum=0.0;
	for(int i=0;i<numpop;i++){
		tmpsum+=vec_dis[i];
	}
	avg_new[numpop]=tmpsum*1.0/numpop;
	/*get the quality*/
	max_obj=S->totalprofit;
	min_obj=S->totalprofit;

	for(int i=0; i<numpop; i++){
		max_obj=max(max_obj, PSol[i].totalprofit);
		min_obj=min(min_obj, PSol[i].totalprofit);
	}

	delta_obj=max_obj-min_obj;
	for(int i=0;i<numpop;i++) norm_quality[i]=(PSol[i].totalprofit-min_obj)*1.0/delta_obj;
	norm_quality[numpop]=(S->totalprofit-min_obj)*1.0/delta_obj;

	/*get the score*/
	for(int i=0;i<numpop+1;i++) score_new[i]=alpha*norm_quality[i]+(1.0-alpha)*avg_new[i];

	/*update the pop*/
	idx=0;
	worstscore=score_new[0];
	for(int i=0;i<numpop;i++)
		if(worstscore>score_new[i]){
			idx=i;
			worstscore=score_new[i];
		}

	if(worstscore>score_new[numpop]) return;

	duplicate_solution(S, &PSol[idx]);

	for(int i=0;i<numpop;i++) {
		if(i==idx) dismat[i][i]=1.0;
		else {
			dismat[i][idx]=get_dis_arc(&PSol[i], S);
			dismat[idx][i]=dismat[i][idx];
		}
	}

}


void Population::Generate_offspring_routebased(struct_sol *S){
	int idx1, idx2;
	int idroute1, idroute2;
	int indroute, nodedrop, posdrop;


	idx1=rand()%numpop;
	do{
		idx2=rand()%numpop;
	}while(idx1==idx2);
	/***************************************/
	duplicate_solution(&PSol[idx1], S);
	idroute1=rand()%num_car;
	idroute2=rand()%num_car;

	/*replace a random route with one from S2*/
	/*drop the nodes route of idroute2 from the S0*/
	for(int i=1;i<PSol[idx2].routeS[idroute2].Nsel;i++){
		nodedrop=PSol[idx2].routeS[idroute2].perm[i];
		if(S->position[0][nodedrop]==-1) continue;
		indroute=S->position[0][nodedrop];
		posdrop=S->position[1][nodedrop];
		S->move_drop(indroute, nodedrop, posdrop);
		S->update_profit(indroute);
	}

	/*drop the rest nodes in idroute1*/
	for(int i=1;i<S->routeS[idroute1].Nsel;i++){
		nodedrop=S->routeS[idroute1].perm[i];
		indroute=S->position[0][nodedrop];
		posdrop=S->position[1][nodedrop];
		S->move_drop(indroute, nodedrop, posdrop);
		S->update_profit(indroute);
	}

	for(int i=1;i<PSol[idx2].routeS[idroute2].Nsel;i++){
		nodedrop=PSol[idx2].routeS[idroute2].perm[i];
		indroute=idroute1;
		posdrop=S->routeS[idroute1].Nsel;
		S->move_add(indroute, nodedrop, posdrop);
		S->update_profit(indroute);
	}

	/*Insert the unselected nodes*/

	NF_add(S);




}

/**************************arc based operation*******************************/


double Population::get_dis_arc(struct_sol *S1, struct_sol *S2){


	int edge_set1[num_city][2], edge_set2[num_city][2];
	int len1=0, len2=0;
	int numcommon=0;

	/*Get the edge set*/
	len1=0;
	for(int i=0;i<num_car;i++){
		for(int j=0;j<S1->routeS[i].Nsel-1;j++){
			edge_set1[len1][0]=S1->routeS[i].perm[j];
			edge_set1[len1][1]=S1->routeS[i].perm[j+1];
			len1++;
		}
	}
	/******************another one***************************/
	len2=0;
	for(int i=0;i<num_car;i++){
		for(int j=0;j<S2->routeS[i].Nsel-1;j++){
			edge_set2[len2][0]=S2->routeS[i].perm[j];
			edge_set2[len2][1]=S2->routeS[i].perm[j+1];
			len2++;
		}
	}
	/***********************************************/

	for(int i=0;i<len1;i++){
		for(int j=0;j<len2;j++){
			if(edge_set1[i][0]==edge_set2[j][0] && edge_set1[i][1]==edge_set2[j][1]){
				numcommon++;
			}
		}
	}
	return 1.0-(numcommon*1.0/(len1+len2-numcommon));
}

void Population::Generate_offspring_arcbased(struct_sol *S){

	int idx1, idx2;
	int edge_set1[num_city][2], edge_set2[num_city][2];
	int len1=0, len2=0;
	int numcommon=0;
	int vec_fix[num_city];
	int unique_idx[num_city],uniquenum=0;
	int tmppre=-1, tmpnex=-1;
	int indroute=-1;
	int posadd;
	int comidx[num_city];


	memset(comidx, 0, num_city*sizeof(int));
	/****************************/
	idx1=rand()%numpop;
	do{
		idx2=rand()%numpop;
	}while(idx1==idx2);

	duplicate_solution(&PSol[idx1], S);

	/*Get the edge set*/
	len1=0;
	for(int i=0;i<num_car;i++){
		for(int j=0;j<PSol[idx1].routeS[i].Nsel-1;j++){
			edge_set1[len1][0]=PSol[idx1].routeS[i].perm[j];
			edge_set1[len1][1]=PSol[idx1].routeS[i].perm[j+1];
			len1++;
		}
	}
	/******************another one***************************/
	len2=0;
	for(int i=0;i<num_car;i++){
		for(int j=0;j<PSol[idx2].routeS[i].Nsel-1;j++){
			edge_set2[len2][0]=PSol[idx2].routeS[i].perm[j];
			edge_set2[len2][1]=PSol[idx2].routeS[i].perm[j+1];
			len2++;
		}
	}
	/***************************************************************************/
	/***************************************************************************/
	/*identify the common edges*/
	memset(vec_fix, 0, num_city*sizeof(int));
	memset(unique_idx, 0, num_city*sizeof(int));
	uniquenum=0;
	numcommon=0;

	for(int i=0;i<len1;i++){
		for(int j=0;j<len2;j++){
			if(edge_set1[i][0]==edge_set2[j][0] && edge_set1[i][1]==edge_set2[j][1]){
				vec_fix[edge_set1[i][0]]=1;
				vec_fix[edge_set1[i][1]]=1;
				numcommon++;
				comidx[j]=1;
			}
//			else{
//				unique_idx[uniquenum]=j;
//				uniquenum++;
//			}
		}
	}

	for(int i=0;i<len2;i++)
		if(comidx[i]==0){
			unique_idx[uniquenum]=i;
			uniquenum++;
		}

//	cout<<endl;
//	printf("all profits:");
//	for(int i=0;i<numpop;i++) printf("%d ", PSol[i].totalprofit);
//	cout<<endl;
//	cout<<"parents quality:"<<PSol[idx1].totalprofit<<" "<<PSol[idx2].totalprofit<<"  dis="<<numcommon<<endl;
//	cout<<endl;

	/*choose the half number of edges and perform the operation on the offspring solution */
	ExcGene(unique_idx, uniquenum);
	for(int i=0;i<uniquenum*0.5;i++){
		tmppre=edge_set2[unique_idx[i]][0];
		tmpnex=edge_set2[unique_idx[i]][1];

		if(tmppre==0 && S->position[0][tmpnex]==-1) {	// the previous node is 0 and tmpnex is an unselected node;

			for(int indroute=0;indroute<num_car;indroute++){
				if(S->routeS[indroute].perm[1]==-1) continue;
				if(vec_fix[S->routeS[indroute].perm[1]]!=1) {
					S->move_add(indroute, tmpnex, 1);
					S->update_profit(indroute);
					break;
				}
			}
			vec_fix[tmppre]=1;
			vec_fix[tmpnex]=1;
//			check_move(S);
			continue;
		}

		if(tmppre==0 && S->position[0][tmpnex]!=-1) {


			if(vec_fix[tmpnex]==1) continue;

			/*find the first one node whose is not fixed*/
			for(int indroute=0;indroute<num_car;indroute++){
				if(S->routeS[indroute].perm[1]==-1) continue;
				if(vec_fix[S->routeS[indroute].perm[1]]!=1) {

					indroute=S->position[0][tmpnex];
					S->move_drop(S->position[0][tmpnex], tmpnex, S->position[1][tmpnex]);			// to simplify the operation, we do the dropping and adding
					S->update_profit(indroute);

					S->move_add(indroute, tmpnex, 1);
					S->update_profit(indroute);
					break;
				}
			}

			vec_fix[tmppre]=1;
			vec_fix[tmpnex]=1;
//			check_move(S);
			continue;
		}

		if(S->position[0][tmppre]==-1 && S->position[0][tmpnex]==-1){	// the two nodes are not selected in the solution, perform the adding operation;
			indroute=rand()%num_car;
			posadd=S->routeS[indroute].Nsel;
			S->move_add(indroute, tmppre, posadd);
			S->update_profit(indroute);

			posadd=S->routeS[indroute].Nsel;
			S->move_add(indroute, tmpnex, posadd);
			S->update_profit(indroute);

			vec_fix[tmppre]=1;
			vec_fix[tmpnex]=1;
//			check_move(S);
			continue;
		}
		if(S->position[0][tmppre]!=-1 && S->position[0][tmpnex]==-1){ // the previous is in the solution while the next is not.


			S->move_add(S->position[0][tmppre], tmpnex, S->position[1][tmppre]+1);
			S->update_profit(S->position[0][tmppre]);

			vec_fix[tmppre]=1;
			vec_fix[tmpnex]=1;
//			check_move(S);
			continue;
		}
		if(S->position[0][tmppre]==-1 && S->position[0][tmpnex]!=-1){ // the previous is not in the solution while the next is yes.

			S->move_add(S->position[0][tmpnex], tmppre, S->position[1][tmpnex]);
			S->update_profit(S->position[0][tmpnex]);

			vec_fix[tmppre]=1;
			vec_fix[tmpnex]=1;
//			check_move(S);
			continue;
		}

		if(S->position[0][tmppre]!=-1 && S->position[0][tmpnex]!=-1){ // both of them are in the solution;


			if(vec_fix[tmppre]==1 && vec_fix[tmpnex]==1) continue;
			if(vec_fix[tmppre]==1 && vec_fix[tmpnex]!=1){
				indroute=S->position[0][tmpnex];
				S->move_drop(S->position[0][tmpnex], tmpnex, S->position[1][tmpnex]);			// to simplify the operation, we do the dropping and adding
				S->update_profit(indroute);

				S->move_add(S->position[0][tmppre], tmpnex, S->position[1][tmppre]+1);
				S->update_profit(S->position[0][tmppre]);
			}
			else if(vec_fix[tmppre]!=1 && vec_fix[tmpnex]==1){
				indroute=S->position[0][tmppre];
				S->move_drop(indroute, tmppre, S->position[1][tmppre]);
				S->update_profit(indroute);

				S->move_add(S->position[0][tmpnex], tmppre, S->position[1][tmpnex]);
				S->update_profit(S->position[0][tmpnex]);
			}

			vec_fix[tmppre]=1;
			vec_fix[tmpnex]=1;
//			check_move(S);
			continue;
		}

		fprintf(stderr,"error in the arc based generate offspring solution, other choices./n");
		exit(-2);


	}

	/*return and check move */
	check_move(S);
	S->fetch_all();

}


/****************************Pos based Operation*******************************/


int Population::get_dis_pos(int *vec1, int *vec2){
	int tmpdis=0;
	for(int i=0;i<num_city+num_car;i++){
		if(vec1[i]!=vec2[i]) tmpdis++;
	}
	return tmpdis;
}


void Population::Generate_offspring(struct_sol *S){
	int numlarge=num_city+num_car;
	int ind1=rand()%numpop;
	int ind2=rand()%numpop;
	int giant_tour1[numlarge];
	int giant_tour2[numlarge];
	int off_tour[numlarge];
	int exe_rand[numlarge];
	int soldis=-1;
	int ct=0;
	int tmpind=0;
	int tmpnode=0;
	int targpos=-1;


	for(int i=0;i<numlarge;i++) exe_rand[i]=i;
	ExcGene(exe_rand, numlarge);


	while(ind1==ind2){
		ind2=rand()%numpop;
	}
	remplir_gt(giant_tour1, &PSol[ind1]);
	remplir_gt(giant_tour2, &PSol[ind2]);

	remplir_gt(off_tour, &PSol[ind1]);

	soldis=get_dis_pos(giant_tour1, giant_tour2);


	for(int i=1;i<numlarge;i++){
		tmpind=exe_rand[i];
		targpos=find_pos(off_tour, giant_tour2, tmpind);
		if(targpos==tmpind) continue; // the duplicate nodes are at the same position;

		tmpnode=off_tour[tmpind];
		off_tour[tmpind]=off_tour[targpos];
		off_tour[targpos]=tmpnode;

		ct++;
		if(ct>=soldis*0) break;
	}

//	int tmpdd=0;
//	for(int i=0;i<numlarge;i++) printf("%d\t",giant_tour2[i]);
//	cout<<endl;
//	for(int i=0;i<numlarge;i++) printf("%d\t",off_tour[i]);
//	cout<<endl;
//	for(int i=0;i<numlarge;i++)
//		if(giant_tour2[i]!=off_tour[i]) tmpdd++;
//	cout<<"tmpdd="<<tmpdd<<endl;

	ct=1;
	for(int i=0;i<num_car;i++){
		S->routeS[i].perm[0]=0;
		for(int j=1;j<num_city;j++){

			if(off_tour[ct]>0){
				S->routeS[i].perm[j]=off_tour[ct];
				ct++;
			}
			else{
				ct++;
				break;
			}

		}
	}
	S->fetch_all();
}


void Population::remplir_gt(int *vec, struct_sol *S){
	int ct=0;

	for(int i=0;i<num_car;i++){
		vec[ct]=-i;
		ct++;
		for(int j=1;j<S->routeS[i].Nsel;j++){
			vec[ct]=S->routeS[i].perm[j];
			ct++;
		}
	}

	vec[ct]=-num_car;
	ct++;

	for(int i=1;i<num_city;i++){
		if(S->position[0][i]==-1) {
			vec[ct]=i;
			ct++;
		}
	}

	if(ct!=num_city+num_car){
		fprintf(stderr, "error in the remplir_gt, the total number is not correct\n");
		exit(-1);
	}

}





int Population::find_pos(int *vec1, int *gt2, int orpos1){
	int targpos=-1;

	if(vec1[orpos1]==gt2[orpos1]) return orpos1;

	for(int i=0;i<num_city+num_car;i++)
		if(gt2[orpos1]==vec1[i]){
			targpos=i;
			return targpos;
		}

	return targpos;
}



void Population::simple_update_pool(struct_sol *S){
	int idx=0;
	int worstprofit=PSol[0].totalprofit;

	for(int i=1;i<numpop;i++)
		if(worstprofit>PSol[i].totalprofit){
			worstprofit=PSol[i].totalprofit;
			idx=i;
		}
	if(worstprofit<S->totalprofit) duplicate_solution(S, &PSol[idx]);

}






