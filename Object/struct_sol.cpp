/*
 * struct_sol.cpp
 *
 *  Created on: Jan 29, 2021
 *      Author: jiren
 */

#include "../mtrpp_lib.h"


struct_route::struct_route(){

	perm=new int[num_city+1];		//num_city+1
	memset(perm,-1,(num_city+1)*sizeof(int));

	vec_sd=new int[num_city];
	memset(vec_sd,0,(num_city)*sizeof(int));

	Sb=new int[num_city];
	memset(Sb,0,(num_city)*sizeof(int));

	Se=new int[num_city];
	memset(Se,0,(num_city)*sizeof(int));

	profit=0;
	Nsel=0;
	cdis=0;
}

struct_route::~struct_route(){
	delete [] perm;
	delete [] vec_sd;
	delete [] Sb;
	delete [] Se;
}

struct_sol::struct_sol(){

	routeS=new struct_route[num_car];
	position=new int *[2];
	for(int i=0;i<2;i++){
		position[i]=new int[num_city];
		memset(position[i], -1, num_city*sizeof(int));
	}

	f_city=new int [num_city];										//f_city[ind_car] or -1
	memset(f_city,-1,num_city*sizeof(int));

	totalprofit=0;
	totalNsel=0;
	totalcdis=0;
}

struct_sol::~struct_sol(){
	delete [] routeS;
	delete [] position[0];
	delete [] position[1];
	delete [] position;
	delete [] f_city;

}

void struct_sol::grasp_inisol(void){

	int sizesub=5;
	bool Fsel[num_city];
	int **CandL=new int *[sizesub];
	for(int i=0;i<sizesub;i++) {
		CandL[i]=new int [4]; //CandL[x][0]-> car, CandL[x][1]->position, CandL[x][2]-> city, CandL[x][3]-> contribute;
		memset(CandL[i],-1, 4*sizeof(int));
	}
	int num_candidate=0;
	int count __attribute__((unused));
	count=0;
	memset(Fsel,false,num_city*sizeof(bool));

	int car, posi, city, contribute;
	int index=-1;
	/*Initial all structure*/
	for(int i=0;i<num_car;i++) {
		memset(routeS[i].perm,-1,(num_city+1)*sizeof(int));
		memset(routeS[i].vec_sd,0,(num_city)*sizeof(int));
		memset(routeS[i].Sb,0,(num_city)*sizeof(int));
		memset(routeS[i].Se,0,(num_city)*sizeof(int));
		routeS[i].profit=0;
		routeS[i].Nsel=0;
		routeS[i].cdis=0;
	}
	for(int i=0;i<2;i++) memset(position[i],-1,(num_city)*sizeof(int));
	memset(f_city,-1,num_city*sizeof(int));
	totalprofit=0;
	totalNsel=0;
	totalcdis=0;
	/*Assign the node*/
	for(int i=0;i<num_car;i++)	{
		routeS[i].perm[0]=0;
		routeS[i].Nsel++;
	}
	position[0][0]=0;
	position[1][0]=0;
	f_city[0]=9999;
	totalNsel++;

	while(totalNsel<num_city){
		num_candidate=0;
		num_candidate=sel_car_position_profit(CandL, Fsel, sizesub);
		if(num_candidate!=0){

			index=rand()%num_candidate;
			car=CandL[index][0];
			posi=CandL[index][1];
			city=CandL[index][2];
			contribute=CandL[index][3];
			routeS[car].perm[posi]=city;
			routeS[car].vec_sd[posi]=routeS[car].vec_sd[posi-1]+glo_map->matrix_dis[routeS[car].perm[posi-1]][city];
			f_city[city]=car;
			if(contribute>0) routeS[car].profit+=contribute;
			routeS[car].Nsel++;
			routeS[car].cdis+=glo_map->matrix_dis[routeS[car].perm[posi-1]][city];
			if(contribute>0) totalprofit+=contribute;
			totalNsel++;
			totalcdis+=glo_map->matrix_dis[routeS[car].perm[posi-1]][city];
		}
	}
	/*evaluate the solution*/
	fetch_all();
	for(int i=0;i<sizesub;i++) delete [] CandL[i];
	delete [] CandL;

	/*check*/
//	for(int i=0;i<num_car;i++){
//		printf("\n");
//		for(int j=0;j<num_city+1;j++)
//			printf("%d ",routeS[i].perm[j]);
//	}
//	printf("\n");
}

int struct_sol::sel_car_position_profit(int **CandL, bool *Fsel, int sizesub){
	//stated in ALNS
	for(int i=0;i<sizesub;i++) CandL[i][2]=-1;
	memset(Fsel, false, num_city*sizeof(bool));
	int maxr=min(sizesub, num_city-totalNsel);
	int count=0;
	int tempcity;
	int tempprofit;
	int tempcar;
	int temppos;

	while(count<maxr){
		for(int i=1; i<num_city;i++){
			tempcity=i;
			if(Fsel[tempcity]) continue;
			if(f_city[tempcity]!=-1) continue; 		//-1 means unselcted
			for(int j=0;j<num_car;j++){
				tempcar=j;
				temppos=routeS[tempcar].Nsel;
				tempprofit=glo_map->profitA[tempcity]-routeS[tempcar].vec_sd[temppos-1]-glo_map->matrix_dis[routeS[tempcar].perm[temppos-1]][tempcity];
				if(CandL[count][2]==-1){		//the first city, it is surely accepted.
					CandL[count][0]=tempcar;
					CandL[count][1]=temppos;
					CandL[count][2]=tempcity;
					CandL[count][3]=tempprofit;
				}
				else if(tempprofit>CandL[count][3]){	//not the first, replace the candidate city conditionally.
					CandL[count][0]=tempcar;
					CandL[count][1]=temppos;
					CandL[count][2]=tempcity;
					CandL[count][3]=tempprofit;
				}
			}
		}
		Fsel[CandL[count][2]]=true;
		count++;
	}
	return maxr;


}


void struct_sol::random_inisol(void){
	int alltour[num_city];
	int ct=0;
	int tmpnode;
	for(int i=0;i<num_city;i++) alltour[i]=i;
	ExcGene(alltour, num_city);

	for(int i=0;i<num_city;i++){
		if(alltour[i]==0){
			alltour[i]=alltour[0];
			alltour[0]=0;
			break;
		}
	}

	ct=1;
	for(int i=0;i<num_car-1;i++){
		routeS[i].perm[0]=0;
		for(int j=1;j<num_city;j++){
			if(j>num_city/num_car) break;
			routeS[i].perm[j]=alltour[ct];
			ct++;
		}
	}

	routeS[num_car-1].perm[0]=0;
	for(int j=1;j<num_city;j++){
		routeS[num_car-1].perm[j]=alltour[ct];
		ct++;
		if(ct>=num_city) break;
	}

	fetch_all();


}

void struct_sol::greedy_inisol(void){
	int nowc;
	int citysel=-1;
	int deg_good=3;
	int	deg_bad=1;
	int count=0;

	/*get one long permutation*/
	int long_perm[num_city+1];
	int long_flag[num_city];
	memset(long_perm,-1,(num_city+1)*sizeof(int));
	memset(long_flag,0,num_city*sizeof(int));
	nowc=0;
	long_perm[nowc]=0;
	long_flag[nowc]=1;
	count++;

	do{
		citysel=sel_greedy_random_city(deg_good,deg_bad,nowc,long_flag);	//if there is no greedy city, choose random one
		if(citysel!=-1) {										//if the city is selected is identified in Sel_greedy_random_city, if citysel==-1 which means there is no free city.
			long_perm[count]=citysel;
			long_flag[citysel]=1;
			nowc=citysel;
			count++;
		}
	}while(citysel!=-1);

	/*separate into several permutations*/
	int i;
	int num_eachperm=num_city/num_car;
	int aux_count=1;
	count=1;


	/*check*/
	printf("\n");
	for(i=0;i<num_city+1;i++) printf("%d ",long_perm[i]);
	printf("\n");
	/**/
	for(i=0;i<num_car;i++){
		routeS[i].perm[0]=0;
		for(int j=1;j<num_eachperm+1;j++){
			routeS[i].perm[j]=long_perm[count];
			count++;
		}
	}
	while(count<num_city+1 && long_perm[count]!=-1){
		routeS[i-1].perm[num_eachperm+aux_count]=long_perm[count];
		count++;
		aux_count++;
	}

	/*check*/
	for(i=0;i<num_car;i++){
		printf("\n");
		for(int j=0;j<num_city+1;j++)
			printf("%d ",routeS[i].perm[j]);
	}
	printf("\n");
	/**/


	fetch_all();
}

void struct_sol::fetch_all(void){


	fetch_nsel();
	fetch_cdis();
	fetch_position();
	fetch_f_city();
	fetch_profit();
	fetch_vec_sd();
	fetch_SbSe();
}

void struct_sol::fetch_nsel(void){
	int count=0;
	int nowc=-1;

	totalNsel=0;
	for(int i=0;i<num_car;i++){
		count=0;
		nowc=routeS[i].perm[count];
		while(nowc!=-1){
			count++;
			nowc=routeS[i].perm[count];
		}
		routeS[i].Nsel=count;
		totalNsel+=routeS[i].Nsel;
	}

	totalNsel=totalNsel+1-num_car;
}

void struct_sol::fetch_position(void){
	int nowcity=-1;

	for(int i=0;i<num_car;i++)
		for(int j=0;i<num_city;j++){
			nowcity=routeS[i].perm[j];
			if(nowcity==-1) break;
			position[0][nowcity]=i;
			position[1][nowcity]=j;
		}
}

void struct_sol::fetch_cdis(void){	//need to do it
	int nowcity, nexcity;
	int ind_nex=0;
	int vecdis=0;

	totalcdis=0;

	for(int i=0;i<num_car;i++){

		routeS[i].cdis=0;
		nowcity=0;
		ind_nex=1;
		vecdis=0;
		nexcity=routeS[i].perm[ind_nex];
		while(nexcity!=-1){
			vecdis+=glo_map->matrix_dis[nowcity][nexcity];
			routeS[i].cdis+=vecdis;
			nowcity=nexcity;
			ind_nex++;
			nexcity=routeS[i].perm[ind_nex];
			if(ind_nex>num_city+1){
				printf("error in fetch_cdis\n");
				exit(-2);
			}
		}
		totalcdis+=routeS[i].cdis;
	}
}

void struct_sol::fetch_f_city(void){
	int nowcity=-1;
	memset(f_city,-1,num_city*sizeof(int));
	for(int i=0;i<num_car;i++)
		for(int j=0;j<num_city;j++){
			nowcity=routeS[i].perm[j];
			if(nowcity==-1) break;
			f_city[nowcity]=i;
		}
}

void struct_sol::fetch_profit(void){
	int nowcity, nexcity;
	int ind_nex=0;
	int acc_dis=0;

	totalprofit=0;

	for(int i=0;i<num_car;i++){

		routeS[i].profit=0;
		acc_dis=0;
		nowcity=0;
		ind_nex=1;
		nexcity=routeS[i].perm[ind_nex];
		while(nexcity!=-1){
//			acc_dis+=(Nsel[i]-ind_nex)*glo_map->matrix_dis[nowcity][nexcity];
			acc_dis+=glo_map->matrix_dis[nowcity][nexcity];
			if(glo_map->profitA[nexcity]-acc_dis>0) routeS[i].profit+=glo_map->profitA[nexcity]-acc_dis;
			nowcity=nexcity;
			ind_nex++;
			nexcity=routeS[i].perm[ind_nex];
			if(ind_nex>num_city+1){
				printf("error in fetch_profit\n");
				exit(-2);
			}
		}
		totalprofit+=routeS[i].profit;
	}
}

void struct_sol::fetch_vec_sd(void){
	int nowc;
	int count=0;

	for(int i=0;i<num_car;i++){
		count=0;
		routeS[i].vec_sd[0]=0;
		count++;
		nowc=routeS[i].perm[count];
		while(nowc!=-1){
			routeS[i].vec_sd[count]=routeS[i].vec_sd[count-1]+glo_map->matrix_dis[routeS[i].perm[count-1]][nowc];
			count++;
			nowc=routeS[i].perm[count];
			if(count>num_city+2) {
				cout<<"error in fetch_vec_sd, no -1 at the end"<<endl;
				exit(-5);
			}
		}
	}
}

void struct_sol::fetch_SbSe(void){
	int nowc;
	int count=0;

	/*get the number of the selected nodes*/
	for(int t=0;t<num_car;t++){
		count=0;
		do{
			nowc=routeS[t].perm[count];
			if(nowc==-1) break;
			count++;
		}while(1);
		routeS[t].Nsel=count;

		routeS[t].Sb[0]=0;
		for(int i=1;i<routeS[t].Nsel;i++) routeS[t].Sb[i]=routeS[t].Sb[i-1]+(routeS[t].Nsel-i)*glo_map->matrix_dis[routeS[t].perm[i-1]][routeS[t].perm[i]];
		routeS[t].Se[routeS[t].Nsel-1]=0;
		for(int i=routeS[t].Nsel-2;i>=0;i--) routeS[t].Se[i]=routeS[t].Se[i+1]+(routeS[t].Nsel-i-1)*glo_map->matrix_dis[routeS[t].perm[i]][routeS[t].perm[i+1]];
	}
}

void struct_sol::update_Vsd(int *single_Vsd, int startpos, int endpos, int *single_perm){

//	if(startpos<1){
//		cout<<"error in updateVsd"<<endl;
//		exit(-3);
//	}
	int nowc;
	int count;

	if(startpos==0) {
		single_Vsd[0]=0;
		startpos=1;
	}
	count=startpos;
	nowc=single_perm[count];
//	while(count<=endpos){
	while(nowc!=-1){
		single_Vsd[count]=single_Vsd[count-1]+glo_map->matrix_dis[single_perm[count-1]][nowc];
		count++;
		nowc=single_perm[count];
		if(count>num_city+2) {
			cout<<"error in update single_Vsd, no -1 at the end"<<endl;
			exit(-5);
		}
	}
	while(count<num_city && single_perm[count]==-1 && single_Vsd[count]!=0){
		single_Vsd[count]=0;
		count++;
		if(count>=num_city+1) break;
	}
}

void struct_sol::update_SeSe(int i, int j, int N, int *single_perm, int *single_Vsd, int *single_Sb, int *single_Se){
	int p;

	if(j==N-1){
		for(p=i+1;p<N;p++) 	single_Sb[p]=single_Sb[p-1]+(N-p)*glo_map->matrix_dis[single_perm[p-1]][single_perm[p]];
		single_Se[j]=0;
		for(p=j-1;p>=0;p--)	single_Se[p]=single_Se[p+1]+(N-1-p)*glo_map->matrix_dis[single_perm[p+1]][single_perm[p]];
	}
	else{
		for(p=i+1;p<N;p++) 	single_Sb[p]=single_Sb[p-1]+(N-p)*glo_map->matrix_dis[single_perm[p-1]][single_perm[p]];
		for(p=j;p>=0;p--)	single_Se[p]=single_Se[p+1]+(N-1-p)*glo_map->matrix_dis[single_perm[p+1]][single_perm[p]];
	}

	update_Vsd(single_Vsd,i,j,single_perm);
}

void struct_sol::update_chosenT(int node1, int node2){
	if(node1==-1 || node2==-1) return;

	glo_map->matrix_chosenT[node1][node2]++;
	glo_map->matrix_chosenT[node2][node1]++;
}


void struct_sol::update_profit(int indroute){
	int nowcity, nexcity;
	int ind_nex=0;
	int acc_dis=0;

	totalprofit-=routeS[indroute].profit;

	routeS[indroute].profit=0;
	acc_dis=0;
	nowcity=0;
	ind_nex=1;
	nexcity=routeS[indroute].perm[ind_nex];
	while(nexcity!=-1){
		acc_dis+=glo_map->matrix_dis[nowcity][nexcity];
		if(glo_map->profitA[nexcity]-acc_dis>0) routeS[indroute].profit+=glo_map->profitA[nexcity]-acc_dis;
		nowcity=nexcity;
		ind_nex++;
		nexcity=routeS[indroute].perm[ind_nex];
		if(ind_nex>num_city+1){
			printf("error in fetch_profit\n");
			exit(-2);
		}
	}
	totalprofit+=routeS[indroute].profit;

}


int struct_sol::locate_nextcity(int nowc, int *single_perm, int *single_position){

	int nexcity=-1;
	if(nowc== 0 && num_car!=1) {
		cout<<"multiple route, unindicate the route, cannot locate the next city of the depot"<<endl;
		exit(-2);
	}
	if(nowc==-1) {
		cout<<"error in lcoate nexcity"<<endl;
		exit(-2);
	}
	int poscur=single_position[nowc];
	nexcity=single_perm[poscur+1];

	return nexcity;
}

int struct_sol::locate_prevcity(int nowc, int *single_perm, int *single_position){
	int precity=-1;

	if(nowc==0) return precity;
	if(nowc==-1) {
		cout<<"error in lcoate precity"<<endl;
		exit(-2);
	}

	int poscur=single_position[nowc];
	precity=single_perm[poscur-1];

	return precity;
}


/*auxiliary function*/

int struct_sol::sel_greedy_random_city(int deg_good, int deg_bad, int nowc, int *long_flag){

	int startp=0;
	int deg_choose=0;
	int i=0;
	int candicity=0;
	//choose a greedy city;
	deg_choose=min(min(deg_good,num_city+1-deg_bad),glo_map->Num_pernode_lk[nowc]);	//well, deg_bad is useless
	startp=rand()%deg_choose;

	for(i=0;i<glo_map->Num_pernode_lk[nowc];i++){
		candicity=glo_map->CandidateSet_LK[nowc][(i+startp)%glo_map->Num_pernode_lk[nowc]];
		if(candicity==-1) {
			cout<<"candicity==-1"<<endl;
			exit(-3);
		}
		if(long_flag[candicity]==0) return candicity;
	}
	//if there is no greedy city, choose a random one;

	startp=rand()%num_city;
	for(i=0;i<num_city;i++){
		candicity=(i+startp)%num_city;							//random choose is not from the candidate set
		if(long_flag[candicity]==0) return candicity;
	}

	return -1;
}

/*Functions for 2opt*/
int struct_sol::fast_2opt(int i, int j, int *single_Vsd, int N, int *single_perm, int *single_Sb, int *single_Se, int single_cdis){
	int delta_dis=0;

	if(j==N-1) {
		delta_dis+=2*single_Sb[i];
		delta_dis+=(N-i-1)*(glo_map->matrix_dis[single_perm[i]][single_perm[i+1]]+glo_map->matrix_dis[single_perm[i]][single_perm[j]]);
		delta_dis+=(N-i-1)*(single_Vsd[j]-single_Vsd[i+1]);
	}
	else{
		delta_dis+=2*(single_Sb[i]+single_Se[j+1]);
		delta_dis+=(N-i-1)*(glo_map->matrix_dis[single_perm[i]][single_perm[i+1]]+glo_map->matrix_dis[single_perm[i]][single_perm[j]]);
		delta_dis+=(N-j-1)*(glo_map->matrix_dis[single_perm[j]][single_perm[j+1]]+glo_map->matrix_dis[single_perm[i+1]][single_perm[j+1]]);
		delta_dis+=(2*N-i-j-2)*(single_Vsd[j]-single_Vsd[i+1]);
	}

	return delta_dis-2*single_cdis;
}

void struct_sol::exe_2opt(struct_sol *S, int indroute, int smaposnode, int bigposnode){
	int E1pre=smaposnode;
	int pos1=position[1][smaposnode];
	int E2nex=locate_nextcity(bigposnode, routeS[indroute].perm, position[1]);
	int pos2=position[1][bigposnode];

	int length=pos2-pos1+2;
	int arr_v[length];
	int count=0;

	/*update the cdis*/
	totalcdis-=routeS[indroute].cdis;

	routeS[indroute].cdis+=fast_2opt(pos1, pos2, routeS[indroute].vec_sd,routeS[indroute].Nsel,\
	routeS[indroute].perm, routeS[indroute].Sb, routeS[indroute].Se, routeS[indroute].cdis);

	totalcdis+=routeS[indroute].cdis;
	/*update the perm*/
	//need to correct
	arr_v[0]=E1pre;
	count=1;
	for(int i=pos2;i>pos1;i--) {
		arr_v[count]=routeS[indroute].perm[i];
		count++;
	}
	arr_v[length-1]=E2nex;

	count=0;
	for(int i=pos1; i<=pos2+1;i++){
		routeS[indroute].perm[i]=arr_v[count];
		count++;
	}
	/*update position*/
	for(int i=pos1; i<=pos2+1;i++) {
		if(routeS[indroute].perm[i]==-1) break;
		position[1][routeS[indroute].perm[i]]=i;
	}
	/*update SeSe, vec, chosenT*/

	update_SeSe(pos1, pos2, routeS[indroute].Nsel, routeS[indroute].perm,\
			routeS[indroute].vec_sd, routeS[indroute].Sb, routeS[indroute].Se);
//	update_chosenT(arr_v[0],arr_v[1]);
//	update_chosenT(arr_v[length-2],arr_v[length-1]);
}

int struct_sol::try_swap(int N, int *single_perm, int pos1, int pos2, int node1, int node2){

	int npre1=single_perm[pos1-1];
	int nnex1=single_perm[pos1+1];
	int npre2=single_perm[pos2-1];
	int nnex2=single_perm[pos2+1];

	int Before_swap=0;
	int After_swap=0;

	if(pos1==N-1){
		if(nnex1!=-1) {						//verify if pos1 is the bottom position;
			cout<<"error in pos1"<<endl;
			exit(-3);
		}
		Before_swap=glo_map->matrix_dis[npre1][node1] + get_pre_nex_cdis(N, pos2,node2, npre2,nnex2);
		After_swap =glo_map->matrix_dis[npre1][node2] + get_pre_nex_cdis(N, pos2,node1, npre2,nnex2);

//		return After_swap-Before_swap;
	}
	else if(pos2==N-1){
		if(nnex2!=-1) {						//verify if pos2 is the bottom position;
			cout<<"error in pos2 in get_delta_swap 1 "<<endl;
			exit(-3);
		}
		Before_swap=get_pre_nex_cdis(N,pos1,node1, npre1,nnex1) + glo_map->matrix_dis[npre2][node2];
		After_swap =get_pre_nex_cdis(N,pos1,node2, npre1,nnex1) + glo_map->matrix_dis[npre2][node1];

//		return After_swap-Before_swap;
	}
	else{
		Before_swap=get_pre_nex_cdis(N,pos1,node1, npre1,nnex1) + get_pre_nex_cdis(N, pos2,node2, npre2,nnex2);
		After_swap= get_pre_nex_cdis(N,pos1,node2, npre1,nnex1) + get_pre_nex_cdis(N, pos2,node1, npre2,nnex2);

//		return After_swap-Before_swap;
	}


//	/*check the cdis*/
//	int real_delta0=0;
//	int real_delta1=0;
//	int esti_delta=0;
//	int old0=CurrentS->routeS[0].cdis;
//	int old1=CurrentS->routeS[1].cdis;
//	single_perm[pos1]=node2;
//	single_perm[pos2]=node1;
//	CurrentS->fetch_cdis();
//	real_delta0=CurrentS->routeS[0].cdis-old0;
//	real_delta1=CurrentS->routeS[1].cdis-old1;
//	esti_delta=After_swap-Before_swap;
//	printf("node1=%d, node2=%d, esti0=%d, real0=%d, real1=%d\n",node1, node2, esti_delta, real_delta0, real_delta1);

	return After_swap-Before_swap;
	return 0;
}

int struct_sol::try_swapadj(int N, int *single_perm, int pos1, int pos2, int node1, int node2){

	int npre1=single_perm[pos1-1];
	int nnex1=single_perm[pos1+1];
	int npre2=single_perm[pos2-1];
	int nnex2=single_perm[pos2+1];

	int Before_swap=0;
	int After_swap=0;

	if(pos1==N-1){
		if(nnex1!=-1) {						//verify if pos1 is the bottom position;
			cout<<"error in pos1"<<endl;
			exit(-3);
		}
		Before_swap= glo_map->matrix_dis[npre2][node2]*2;
		After_swap = glo_map->matrix_dis[npre2][node1]*2;

//		return After_swap-Before_swap;
	}
	else if(pos2==N-1){
		if(nnex2!=-1) {						//verify if pos2 is the bottom position;
			cout<<"error in pos2 in get_delta_swap 1 "<<endl;
			exit(-3);
		}
		Before_swap=glo_map->matrix_dis[npre1][node1]*2;
		After_swap =glo_map->matrix_dis[npre1][node2]*2;

//		return After_swap-Before_swap;
	}
	else if(pos1<pos2){						// node 1 is in front of node2
		Before_swap=glo_map->matrix_dis[npre1][node1]*(N-pos1) + glo_map->matrix_dis[node2][nnex2]*(N-pos2-1);
		After_swap =glo_map->matrix_dis[npre1][node2]*(N-pos1) + glo_map->matrix_dis[node1][nnex2]*(N-pos2-1);

//		return After_swap-Before_swap;
	}
	else if(pos1>pos2){
		Before_swap=glo_map->matrix_dis[node1][nnex1]*(N-pos1-1) + glo_map->matrix_dis[npre2][node2]*(N-pos2);
		After_swap =glo_map->matrix_dis[node2][nnex1]*(N-pos1-1) + glo_map->matrix_dis[npre2][node1]*(N-pos2);

//		return After_swap-Before_swap;
	}
	return After_swap-Before_swap;
//	/*check the cdis*/
//	int real_delta0=0;
//	int real_delta1=0;
//	int esti_delta=0;
//	int old0=CurrentS->routeS[0].cdis;
//	int old1=CurrentS->routeS[1].cdis;
//	single_perm[pos1]=node2;
//	single_perm[pos2]=node1;
//	CurrentS->fetch_cdis();
//	real_delta0=CurrentS->routeS[0].cdis-old0;
//	real_delta1=CurrentS->routeS[1].cdis-old1;
//	esti_delta=After_swap-Before_swap;
//	printf("node1=%d, node2=%d, esti0=%d, real0=%d, real1=%d\n",node1, node2, esti_delta, real_delta0, real_delta1);

	return 0;
}

int struct_sol::get_pre_nex_cdis(int N, int pos, int node, int prenode, int nexnode){
	return (N-pos)*glo_map->matrix_dis[prenode][node] + (N-pos-1)*glo_map->matrix_dis[node][nexnode];
}


void struct_sol::move_swap(int indroute, int N, int *single_perm, int pos1, int pos2, int node1, int node2){
	int npre1=single_perm[pos1-1];
	int nnex1=single_perm[pos1+1];
	int npre2=single_perm[pos2-1];
	int nnex2=single_perm[pos2+1];

	int Before_swap=0;
	int After_swap=0;

	if(pos1==N-1){
		if(nnex1!=-1) {						//verify if pos1 is the bottom position;
			cout<<"error in pos1"<<endl;
			exit(-3);
		}
		Before_swap=glo_map->matrix_dis[npre1][node1] + get_pre_nex_cdis(N, pos2,node2, npre2,nnex2);
		After_swap =glo_map->matrix_dis[npre1][node2] + get_pre_nex_cdis(N, pos2,node1, npre2,nnex2);

	}
	else if(pos2==N-1){
		if(nnex2!=-1) {						//verify if pos2 is the bottom position;
			cout<<"error in pos2 in get_delta_swap 1 "<<endl;
			exit(-3);
		}
		Before_swap=get_pre_nex_cdis(N,pos1,node1, npre1,nnex1) + glo_map->matrix_dis[npre2][node2];
		After_swap =get_pre_nex_cdis(N,pos1,node2, npre1,nnex1) + glo_map->matrix_dis[npre2][node1];

	}
	else{
		Before_swap=get_pre_nex_cdis(N,pos1,node1, npre1,nnex1) + get_pre_nex_cdis(N, pos2,node2, npre2,nnex2);
		After_swap= get_pre_nex_cdis(N,pos1,node2, npre1,nnex1) + get_pre_nex_cdis(N, pos2,node1, npre2,nnex2);

	}

	/*update the cdis*/
	totalcdis-=routeS[indroute].cdis;
	routeS[indroute].cdis+=-Before_swap+After_swap;
	totalcdis+=routeS[indroute].cdis;

	/*update the perm*/
	routeS[indroute].perm[pos1]=node2;
	routeS[indroute].perm[pos2]=node1;

	/*update the position*/
	position[1][node1]=pos2;
	position[1][node2]=pos1;
	/*no need to update f_city*/
	/*no need to update Vec, SbSe because we dont use them here even if it changes*/
}

void struct_sol::move_swapadj(int indroute, int N, int *single_perm, int pos1, int pos2, int node1, int node2){
	int npre1=single_perm[pos1-1];
	int nnex1=single_perm[pos1+1];
	int npre2=single_perm[pos2-1];
	int nnex2=single_perm[pos2+1];

	int Before_swap=0;
	int After_swap=0;

	if(pos1==N-1){
		if(nnex1!=-1) {						//verify if pos1 is the bottom position;
			cout<<"error in pos1"<<endl;
			exit(-3);
		}
		Before_swap= glo_map->matrix_dis[npre2][node2]*2;
		After_swap = glo_map->matrix_dis[npre2][node1]*2;

	}
	else if(pos2==N-1){
		if(nnex2!=-1) {						//verify if pos2 is the bottom position;
			cout<<"error in pos2 in get_delta_swap 1 "<<endl;
			exit(-3);
		}
		Before_swap=glo_map->matrix_dis[npre1][node1]*2;
		After_swap =glo_map->matrix_dis[npre1][node2]*2;


	}
	else if(pos1<pos2){						// node 1 is in front of node2
		Before_swap=glo_map->matrix_dis[npre1][node1]*(N-pos1) + glo_map->matrix_dis[node2][nnex2]*(N-pos2-1);
		After_swap =glo_map->matrix_dis[npre1][node2]*(N-pos1) + glo_map->matrix_dis[node1][nnex2]*(N-pos2-1);


	}
	else if(pos1>pos2){
		Before_swap=glo_map->matrix_dis[node1][nnex1]*(N-pos1-1) + glo_map->matrix_dis[npre2][node2]*(N-pos2);
		After_swap =glo_map->matrix_dis[node2][nnex1]*(N-pos1-1) + glo_map->matrix_dis[npre2][node1]*(N-pos2);

	}
	/*update the cdis*/
	totalcdis-=routeS[indroute].cdis;
	routeS[indroute].cdis+=-Before_swap+After_swap;
	totalcdis+=routeS[indroute].cdis;

	/*update the perm*/
	routeS[indroute].perm[pos1]=node2;
	routeS[indroute].perm[pos2]=node1;

	/*update the position*/
	position[1][node1]=pos2;
	position[1][node2]=pos1;
	/*no need to update f_city*/
	/*no need to update Vec, SbSe because we dont use them here even if it changes*/
}

int struct_sol::FwI(int i, int j, int k, int *single_Vsd, int N, int *single_perm){
	int delta_cdis=0;

	delta_cdis+=(N-i)*(glo_map->matrix_dis[single_perm[i-1]][single_perm[i+k]]-glo_map->matrix_dis[single_perm[i-1]][single_perm[i]]);
	if((N-j-1)!=0) delta_cdis+=(N-j-1)*(glo_map->matrix_dis[single_perm[i+k-1]][single_perm[j+1]]-glo_map->matrix_dis[single_perm[j]][single_perm[j+1]]);
	delta_cdis+=(N-j+k-1)*glo_map->matrix_dis[single_perm[j]][single_perm[i]];
	delta_cdis-=(N-i-k)*glo_map->matrix_dis[single_perm[i+k-1]][single_perm[i+k]];
	delta_cdis+=(k+i-j-1)*(single_Vsd[i+k-1]-single_Vsd[i]);
	delta_cdis+=k*(single_Vsd[j]-single_Vsd[i+k]);

	return delta_cdis;
}



int struct_sol::BwI(int i, int j, int k, int *single_Vsd, int N, int *single_perm){
	int delta_cdis=0;

	if(k==1 && j+1==i) return 0;

	delta_cdis+=(N-j-1)*(glo_map->matrix_dis[single_perm[j]][single_perm[i]]-glo_map->matrix_dis[single_perm[j]][single_perm[j+1]]);
	if((N-i-k)!=0) delta_cdis+=(N-i-k)*(glo_map->matrix_dis[single_perm[i-1]][single_perm[i+k]]-glo_map->matrix_dis[single_perm[i+k-1]][single_perm[i+k]]);
	delta_cdis+=(N-k-j-1)*glo_map->matrix_dis[single_perm[i+k-1]][single_perm[j+1]];
	delta_cdis-=(N-i)*glo_map->matrix_dis[single_perm[i-1]][single_perm[i]];
	delta_cdis+=(i-j-1)*(single_Vsd[i+k-1]-single_Vsd[i]);
	delta_cdis+=(-k)*(single_Vsd[i-1]-single_Vsd[j+1]);
	return delta_cdis;
}

void struct_sol::move_insert_oropt(int movpos, int fixpos, int indroute, int k){

	int count __attribute__((unused));
	int deltadis __attribute__((unused));

	if(movpos==0){
		fprintf(stderr, "error: depot is considered as the movnode.");
		exit(-3);
	}
	if(movpos==fixpos){
		fprintf(stderr, "error: fixpos is equal to movpos.");
		exit(-3);
	}
	if(movpos+k-1>=fixpos && fixpos>movpos){
		fprintf(stderr, "error: movpattern includes the fixpos");
		exit(-3);
	}

	int premov=movpos-1;
	int nexmov=movpos+k;
	int prefix=fixpos-1;
	int nexfix=fixpos+1;
	int tmpvec[num_city];
	memset(tmpvec, -1, num_city*sizeof(int));

	/*update the cdis*/
	totalcdis-=routeS[indroute].cdis;
	if (movpos<fixpos) deltadis= FwI(movpos,fixpos,k,routeS[indroute].vec_sd,routeS[indroute].Nsel,routeS[indroute].perm);
	else deltadis=BwI(movpos,fixpos,k,routeS[indroute].vec_sd,routeS[indroute].Nsel,routeS[indroute].perm);
	routeS[indroute].cdis+=deltadis;
	totalcdis+=routeS[indroute].cdis;
	/*update the perm*/
	if(movpos<fixpos){
		count=0;
		for(int i=nexmov;i<=fixpos;i++) {
			tmpvec[count]=routeS[indroute].perm[i];
			count++;
		}
		for(int i=movpos;i<movpos+k;i++){
			tmpvec[count]=routeS[indroute].perm[i];
			count++;
		}
		count=0;
		for(int i=movpos;i<=fixpos;i++){
			routeS[indroute].perm[i]=tmpvec[count];
			count++;
		}
	}
	else if(fixpos<movpos){
		count=0;
		for(int i=movpos;i<movpos+k;i++){
			tmpvec[count]=routeS[indroute].perm[i];
			count++;
		}
		for(int i=nexfix;i<=premov;i++) {
			tmpvec[count]=routeS[indroute].perm[i];
			count++;
		}
		count=0;
		for(int i=nexfix;i<movpos+k;i++){
			routeS[indroute].perm[i]=tmpvec[count];
			count++;
		}
	}
	else{
		fprintf(stderr, "error: movpos is the same as fixpos.");
		exit(-3);
	}
	/*update the position*/
	if(movpos<fixpos){
		for(int i=movpos;i<=fixpos;i++) position[1][routeS[indroute].perm[i]]=i;
	}
	else{
		for(int i=nexfix;i<movpos+k;i++) position[1][routeS[indroute].perm[i]]=i;
	}
	/*update the vec*/
	if(movpos<fixpos) update_Vsd(routeS[indroute].vec_sd, movpos, fixpos, routeS[indroute].perm);
	else update_Vsd(routeS[indroute].vec_sd, nexfix, movpos+k-1, routeS[indroute].perm);
	/*profit is updated outside and SbSe, f_city and Nsel don not need to update.*/
}

int struct_sol::try_add(int i, int nodeadd, int N, int *Vec, int *single_perm){
	int deltap=0;

	deltap+=glo_map->profitA[nodeadd];
	deltap-=Vec[i-1]-Vec[0];
	deltap-=(N+1-i)*glo_map->matrix_dis[single_perm[i-1]][nodeadd];
	if(N!=i) deltap-=(N-i)*glo_map->matrix_dis[nodeadd][single_perm[i]];
	if(N!=i) deltap-=-(N-i)*glo_map->matrix_dis[single_perm[i-1]][single_perm[i]];

	return deltap;
}

void struct_sol::move_add(int indroute, int nodeadd, int posadd){

	/*update the flag*/
	f_city[nodeadd]=indroute;

	/*update the cdis*/
	totalcdis-=routeS[indroute].cdis;
	routeS[indroute].cdis+=update_cdis_add(posadd, nodeadd, routeS[indroute].Nsel, routeS[indroute].vec_sd, routeS[indroute].perm);
	totalcdis+=routeS[indroute].cdis;
	/*update the perm*/
	for(int i=routeS[indroute].Nsel-1;i>=posadd;i--) routeS[indroute].perm[i+1]=routeS[indroute].perm[i];
	routeS[indroute].perm[posadd]=nodeadd;
	/*update the Nsel*/
	routeS[indroute].Nsel++;
	totalNsel++;
	/*update the position*/
	for(int i=posadd; i<routeS[indroute].Nsel;i++) position[1][routeS[indroute].perm[i]]=i;
	position[0][nodeadd]=indroute;
	/*update the vec*/
	update_Vsd(routeS[indroute].vec_sd, posadd,1, routeS[indroute].perm);
}

int struct_sol::update_cdis_add(int i, int nodeadd, int N, int *Vsd, int *single_perm){

	int deltacdis=0;
	if(single_perm[i]==-1 && N!=i) cout<<"exceed to perm in fast_update_cdis"<<endl;
	deltacdis+=Vsd[i-1]-Vsd[0];
	deltacdis+=(N+1-i)*glo_map->matrix_dis[single_perm[i-1]][nodeadd];
	if(N!=i) deltacdis+=(N-i)*glo_map->matrix_dis[nodeadd][single_perm[i]];
	if(N!=i) deltacdis+=-(N-i)*glo_map->matrix_dis[single_perm[i-1]][single_perm[i]];

	return deltacdis;
}

int struct_sol::try_drop_profit(int i, int nodedrop, int N, int *Vsd, int *single_perm){
	int deltap=0;

	if(single_perm[i]==-1 && N!=i) cout<<"exceed to perm in fast_update_cdis"<<endl;
	deltap+=glo_map->profitA[nodedrop];
	deltap-=(Vsd[i-1]-Vsd[0]);
	deltap-=(N-i)*glo_map->matrix_dis[single_perm[i-1]][single_perm[i]];
	if(N!=i+1) deltap+=(N-1-i)*glo_map->matrix_dis[single_perm[i-1]][single_perm[i+1]];
	if(N!=i+1) deltap-=(N-i-1)*glo_map->matrix_dis[single_perm[i]][single_perm[i+1]];

	return -deltap;
}

void struct_sol::move_drop(int indroute, int nodedrop, int posdrop){
	/*update the flag*/
	f_city[nodedrop]=-1;

	/*update the cdis*/
	totalcdis-=routeS[indroute].cdis;
	routeS[indroute].cdis+=update_cdis_drop(posdrop,nodedrop, routeS[indroute].Nsel,routeS[indroute].vec_sd, routeS[indroute].perm);
	totalcdis+=routeS[indroute].cdis;

	/*update the perm*/
	for(int i=posdrop;i<routeS[indroute].Nsel;i++) routeS[indroute].perm[i]=routeS[indroute].perm[i+1];

	/*update the position*/
	position[0][nodedrop]=-1;
	position[1][nodedrop]=-1;

	for(int i=posdrop;i<routeS[indroute].Nsel-1;i++) position[1][routeS[indroute].perm[i]]=i;

	/*update the Nsel*/

	totalNsel-=routeS[indroute].Nsel;
	routeS[indroute].Nsel--;
	totalNsel+=routeS[indroute].Nsel;

	/*update the vsd*/

	update_Vsd(routeS[indroute].vec_sd, posdrop, 1, routeS[indroute].perm);

}

int struct_sol::update_cdis_drop(int i, int nodedrop, int N, int *Vsd, int *single_perm){

	int deltacdis=0;
	if(single_perm[i]==-1 && N!=i) cout<<"exceed to perm in fast_update_cdis"<<endl;
	deltacdis-=(Vsd[i-1]-Vsd[0]);
	deltacdis-=(N-i)*glo_map->matrix_dis[single_perm[i-1]][single_perm[i]];
//	if(N==i+1) deltacdis-=glo_map->matrix_dis[single_perm[i-1]][single_perm[i]];
	if(N!=i+1) deltacdis+=(N-1-i)*glo_map->matrix_dis[single_perm[i-1]][single_perm[i+1]];
	if(N!=i+1) deltacdis-=(N-i-1)*glo_map->matrix_dis[single_perm[i]][single_perm[i+1]];

	return deltacdis;
}

int struct_sol::try_interswap(int fixnode, int swapnode, int route1, int route2, int pos1, int pos2){
	int deltacdis=0;
	int N1=routeS[route1].Nsel;
	int N2=routeS[route2].Nsel;
	int	route1_pre=routeS[route1].perm[pos1-1];
	int	route1_nex=routeS[route1].perm[pos1+1];
	int	route2_pre=routeS[route2].perm[pos2-1];
	int	route2_nex=routeS[route2].perm[pos2+1];

	deltacdis+=(N2-pos2)*glo_map->matrix_dis[route2_pre][fixnode];
	deltacdis+=(N1-pos1)*glo_map->matrix_dis[route1_pre][swapnode];
	deltacdis-=(N2-pos2)*glo_map->matrix_dis[route2_pre][swapnode];
	deltacdis-=(N1-pos1)*glo_map->matrix_dis[route1_pre][fixnode];
	if(pos1!=N1-1){
		deltacdis+=(N1-1-pos1)*glo_map->matrix_dis[swapnode][route1_nex];
		deltacdis-=(N1-1-pos1)*glo_map->matrix_dis[fixnode][route1_nex];
	}
	if(pos2!=N2-1){
		deltacdis+=(N2-1-pos2)*glo_map->matrix_dis[fixnode][route2_nex];
		deltacdis-=(N2-1-pos2)*glo_map->matrix_dis[swapnode][route2_nex];
	}
	return deltacdis;
}

void struct_sol::move_interswap(int fixnode, int swapnode, int route1, int route2, int pos1, int pos2){


//	if(fixnode==36 && swapnode==41){
//
//		cout<<"break point"<<endl;
//		cout<<"break point"<<endl;
//		cout<<"break point"<<endl;
//	}

	//update the cdis
	update_cdis_interswap( fixnode,  swapnode,  route1,  route2,  pos1,  pos2);
	//update the permutation
	routeS[route1].perm[pos1]=swapnode;
	routeS[route2].perm[pos2]=fixnode;
	//update the position
	position[0][fixnode]=route2;
	position[1][fixnode]=pos2;
	position[0][swapnode]=route1;
	position[1][swapnode]=pos1;
	//update the flag
	f_city[fixnode]=route2;
	f_city[swapnode]=route1;
	//Vec, Sb, Se do not need to update
	//update the Nsel, no need to change
	//profit is updated outside

}

void struct_sol::update_cdis_interswap(int fixnode, int swapnode, int route1, int route2, int pos1, int pos2){
	int N1=routeS[route1].Nsel;
	int N2=routeS[route2].Nsel;
	int	route1_pre=routeS[route1].perm[pos1-1];
	int	route1_nex=routeS[route1].perm[pos1+1];
	int	route2_pre=routeS[route2].perm[pos2-1];
	int	route2_nex=routeS[route2].perm[pos2+1];

	totalcdis-=routeS[route1].cdis;
	totalcdis-=routeS[route2].cdis;

	routeS[route1].cdis-=(N1-pos1)*glo_map->matrix_dis[route1_pre][fixnode];
	routeS[route1].cdis+=(N1-pos1)*glo_map->matrix_dis[route1_pre][swapnode];

	routeS[route2].cdis-=(N2-pos2)*glo_map->matrix_dis[route2_pre][swapnode];
	routeS[route2].cdis+=(N2-pos2)*glo_map->matrix_dis[route2_pre][fixnode];

	if(pos1!=N1-1){
		routeS[route1].cdis-=(N1-1-pos1)*glo_map->matrix_dis[fixnode][route1_nex];
		routeS[route1].cdis+=(N1-1-pos1)*glo_map->matrix_dis[swapnode][route1_nex];
	}
	if(pos2!=N2-1){
		routeS[route2].cdis-=(N2-1-pos2)*glo_map->matrix_dis[swapnode][route2_nex];
		routeS[route2].cdis+=(N2-1-pos2)*glo_map->matrix_dis[fixnode][route2_nex];
	}
	totalcdis+=routeS[route1].cdis;
	totalcdis+=routeS[route2].cdis;
}

int struct_sol::try_interinsert(int fixnode, int objnode, int route1, int route2, int pos1, int pos2){
	int deltacdis=0;
	int N1=routeS[route1].Nsel;
	int N2=routeS[route2].Nsel;
	int	route1_pre=routeS[route1].perm[pos1-1];
	int	route1_nex=routeS[route1].perm[pos1+1];
	int	route2_pre=routeS[route2].perm[pos2-1];
	int	route2_nex=routeS[route2].perm[pos2+1];

	deltacdis-=routeS[route1].vec_sd[pos1-1];
	deltacdis-=(N1-pos1)*glo_map->matrix_dis[route1_pre][fixnode];
	if(pos1!=N1-1)	{
		deltacdis-=(N1-pos1-1)*glo_map->matrix_dis[fixnode][route1_nex];
		deltacdis+=(N1-1-pos1)*glo_map->matrix_dis[route1_pre][route1_nex];
	}

	deltacdis+=routeS[route2].vec_sd[pos2];
	deltacdis+=(N2-pos2)*glo_map->matrix_dis[objnode][fixnode];
	if(pos2!=N2-1){
		deltacdis+=(N2-pos2-1)*glo_map->matrix_dis[fixnode][route2_nex];
		deltacdis-=(N2-pos2-1)*glo_map->matrix_dis[objnode][route2_nex];
	}

	return deltacdis;
}

void struct_sol::move_interinsert(int fixnode, int objnode, int route1, int route2, int pos1, int pos2){

	//update the cdis
	update_cdis_interinsert(fixnode,  objnode,  route1,  route2,  pos1,  pos2);

	//update the perm
	for(int i=pos1;i<routeS[route1].Nsel;i++) routeS[route1].perm[i]=routeS[route1].perm[i+1];
	for(int i=routeS[route2].Nsel;i>pos2;i--) routeS[route2].perm[i]=routeS[route2].perm[i-1];
	routeS[route2].perm[pos2+1]=fixnode;
	//update the Nsel
	routeS[route1].Nsel--;
	routeS[route2].Nsel++;
	//update the position
	for(int i=pos1;i<routeS[route1].Nsel;i++) position[1][routeS[route1].perm[i]]=i;
	for(int i=pos2;i<routeS[route2].Nsel;i++) position[1][routeS[route2].perm[i]]=i;
	position[0][fixnode]=route2;
	//update the flag
	f_city[fixnode]=route2;
	//update the vec
	update_Vsd(routeS[route1].vec_sd, pos1, 0, routeS[route1].perm);
	update_Vsd(routeS[route2].vec_sd, pos2, 0, routeS[route2].perm);
	//Sb, Se no updating, Profit update outside
}


void struct_sol:: update_cdis_interinsert(int fixnode, int objnode, int route1, int route2, int pos1, int pos2){
	int N1=routeS[route1].Nsel;
	int N2=routeS[route2].Nsel;
	int	route1_pre=routeS[route1].perm[pos1-1];
	int	route1_nex=routeS[route1].perm[pos1+1];
	int	route2_pre=routeS[route2].perm[pos2-1];
	int	route2_nex=routeS[route2].perm[pos2+1];

	totalcdis-=routeS[route1].cdis;
	totalcdis-=routeS[route2].cdis;

	routeS[route1].cdis-=routeS[route1].vec_sd[pos1-1];
	routeS[route1].cdis-=(N1-pos1)*glo_map->matrix_dis[route1_pre][fixnode];
	if(pos1!=N1-1){
		routeS[route1].cdis-=(N1-pos1-1)*glo_map->matrix_dis[fixnode][route1_nex];
		routeS[route1].cdis+=(N1-1-pos1)*glo_map->matrix_dis[route1_pre][route1_nex];
	}

	routeS[route2].cdis+=routeS[route2].vec_sd[pos2];
	routeS[route2].cdis+=(N2-pos2)*glo_map->matrix_dis[objnode][fixnode];
	if(pos2!=N2-1){
		routeS[route2].cdis+=(N2-pos2-1)*glo_map->matrix_dis[fixnode][route2_nex];
		routeS[route2].cdis-=(N2-pos2-1)*glo_map->matrix_dis[objnode][route2_nex];
	}

	totalcdis+=routeS[route1].cdis;
	totalcdis+=routeS[route2].cdis;
}


int struct_sol:: try_inter2opt(int route1, int route2, int pos1, int pos2){
	int deltacdis=0;
	int N1=routeS[route1].Nsel;
	int N2=routeS[route2].Nsel;
	int N1_prime=N2+pos1-pos2;
	int N2_prime=N1-pos1+pos2;
	int node1=routeS[route1].perm[pos1];
	int node2=routeS[route2].perm[pos2];
	int n1_nex=routeS[route1].perm[pos1+1];
	int n2_nex=routeS[route2].perm[pos2+1];


	/*update the cdis for the first route*/
	deltacdis+=(N1_prime-N1)*routeS[route1].vec_sd[pos1];
	if(pos2<N2-1) deltacdis+=(N1_prime-pos1-1)*glo_map->matrix_dis[node1][n2_nex];
	if(pos1<N1-1) deltacdis-=(N1-pos1-1)*glo_map->matrix_dis[node1][n1_nex];
	if(pos2+1<N2) deltacdis+=routeS[route2].Se[pos2+1];
	if(pos1+1<N1) deltacdis-=routeS[route1].Se[pos1+1];

	/*update the cdis for the second route*/
	deltacdis+=(N2_prime-N2)*routeS[route2].vec_sd[pos2];
	if(pos1<N1-1) deltacdis+=(N2_prime-pos2-1)*glo_map->matrix_dis[node2][n1_nex];
	if(pos2<N2-1) deltacdis-=(N2-pos2-1)*glo_map->matrix_dis[node2][n2_nex];
	if(pos1+1<N1) deltacdis+=routeS[route1].Se[pos1+1];
	if(pos2+1<N2) deltacdis-=routeS[route2].Se[pos2+1];

//	deltacdis=

	return deltacdis;
}

void struct_sol:: move_inter2opt(int route1, int route2, int pos1, int pos2){

	int tmp[num_city];
	int cnt=0;

	//update cdis;
	update_cdis_inter2opt(route1, route2, pos1, pos2);
	//update perm
	for(int i=pos1+1;i<routeS[route1].Nsel;i++) {
		tmp[cnt]=routeS[route1].perm[i];
		routeS[route1].perm[i]=-1;
		cnt++;
	}
	for(int i=pos2+1;i<routeS[route2].Nsel;i++){
		routeS[route1].perm[i-pos2+pos1]=routeS[route2].perm[i];
		routeS[route2].perm[i]=-1;
	}
	for(int i=0;i<cnt;i++) routeS[route2].perm[i+pos2+1]=tmp[i];

	//update Nsel
	cnt=routeS[route1].Nsel;
	routeS[route1].Nsel=routeS[route2].Nsel+pos1-pos2;
	routeS[route2].Nsel=cnt-pos1+pos2;
	//update f_city
	for(int i=pos1+1;i<routeS[route1].Nsel;i++) f_city[routeS[route1].perm[i]]=route1;
	for(int i=pos2+1;i<routeS[route2].Nsel;i++) f_city[routeS[route2].perm[i]]=route2;

	//update position
	for(int i=pos1+1;i<routeS[route1].Nsel;i++){
		position[0][routeS[route1].perm[i]]=route1;
		position[1][routeS[route1].perm[i]]=i;
	}
	for(int i=pos2+1;i<routeS[route2].Nsel;i++){
		position[0][routeS[route2].perm[i]]=route2;
		position[1][routeS[route2].perm[i]]=i;
	}
//	//update Vec_sd
//	update_Vsd(routeS[route1].vec_sd, pos1+1, 1, routeS[route1].perm);
//	update_Vsd(routeS[route2].vec_sd, pos2+1, 1, routeS[route2].perm);
	//update Vec_sd, Sb,Se
	update_SeSe(0, routeS[route1].Nsel-1, routeS[route1].Nsel, routeS[route1].perm,\
			routeS[route1].vec_sd, routeS[route1].Sb, routeS[route1].Se);
	update_SeSe(0, routeS[route2].Nsel-1, routeS[route2].Nsel, routeS[route2].perm,\
				routeS[route2].vec_sd, routeS[route2].Sb, routeS[route2].Se);
	//profit is updated outside

}

void struct_sol:: update_cdis_inter2opt(int route1, int route2, int pos1, int pos2){

	int N1=routeS[route1].Nsel;
	int N2=routeS[route2].Nsel;
	int N1_prime=N2+pos1-pos2;
	int N2_prime=N1-pos1+pos2;
	int node1=routeS[route1].perm[pos1];
	int node2=routeS[route2].perm[pos2];
	int n1_nex=routeS[route1].perm[pos1+1];
	int n2_nex=routeS[route2].perm[pos2+1];


	totalcdis-=routeS[route1].cdis;
	totalcdis-=routeS[route2].cdis;

	/*update the cdis for the first route*/
	routeS[route1].cdis+=(N1_prime-N1)*routeS[route1].vec_sd[pos1];
	if(pos2<N2-1) routeS[route1].cdis+=(N1_prime-pos1-1)*glo_map->matrix_dis[node1][n2_nex];
	if(pos1<N1-1) routeS[route1].cdis-=(N1-pos1-1)*glo_map->matrix_dis[node1][n1_nex];
	if(pos2+1<N2) routeS[route1].cdis+=routeS[route2].Se[pos2+1];
	if(pos1+1<N1) routeS[route1].cdis-=routeS[route1].Se[pos1+1];

	/*update the cdis for the second route*/
	routeS[route2].cdis+=(N2_prime-N2)*routeS[route2].vec_sd[pos2];
	if(pos1<N1-1) routeS[route2].cdis+=(N2_prime-pos2-1)*glo_map->matrix_dis[node2][n1_nex];
	if(pos2<N2-1) routeS[route2].cdis-=(N2-pos2-1)*glo_map->matrix_dis[node2][n2_nex];
	if(pos1+1<N1) routeS[route2].cdis+=routeS[route1].Se[pos1+1];
	if(pos2+1<N2) routeS[route2].cdis-=routeS[route2].Se[pos2+1];

	totalcdis+=routeS[route1].cdis;
	totalcdis+=routeS[route2].cdis;
}


int struct_sol::try_interoropt(int length, int pos1 ,int pos2, int route1, int route2){
	int delta_cdis=0;
	int N1=routeS[route1].Nsel;
	int N2=routeS[route2].Nsel;
	int N1_prime=N1-length;
	int N2_prime=N2+length;
	int r1_pre=routeS[route1].perm[pos1-1];				//x_i-1
	int node1=routeS[route1].perm[pos1];				//x_i
	int r1_last=routeS[route1].perm[pos1+length-1];		//x_i+k-1
	int r1_nex=routeS[route1].perm[pos1+length];		//x_i+k
	int node2=routeS[route2].perm[pos2];				//x_j
	int r2_nex=routeS[route2].perm[pos2+1];				//x_j+1

	delta_cdis+=-length*routeS[route1].vec_sd[pos1-1];
	if(pos1+length<N1) delta_cdis+=(N1_prime-pos1)*glo_map->matrix_dis[r1_pre][r1_nex];
	for(int i=0;i<length-1;i++) delta_cdis-=(N1-pos1-1-i)*glo_map->matrix_dis[routeS[route1].perm[pos1+i]][routeS[route1].perm[pos1+i+1]];
	if(pos1+length<N1) delta_cdis-=(N1-pos1-length)*glo_map->matrix_dis[r1_last][r1_nex];
	delta_cdis-=(N1-pos1)*glo_map->matrix_dis[r1_pre][node1];

	delta_cdis+=length*routeS[route2].vec_sd[pos2];
	delta_cdis+=(N2_prime-pos2-1)*glo_map->matrix_dis[node2][node1];
	if(pos2+1<N2) delta_cdis-=(N2-pos2-1)*glo_map->matrix_dis[node2][r2_nex];
	for(int i=0;i<length-1;i++) delta_cdis+=(N2_prime-pos2-2-i)*glo_map->matrix_dis[routeS[route1].perm[pos1+i]][routeS[route1].perm[pos1+i+1]];
	if(pos2+1<N2) delta_cdis+=(N2_prime-pos2-length-1)*glo_map->matrix_dis[r1_last][r2_nex];

	return delta_cdis;
}


void struct_sol::move_interoropt(int length, int pos1 ,int pos2, int route1, int route2){

	int N1=routeS[route1].Nsel;
	int N2=routeS[route2].Nsel;
	//update the cdis;
	update_cdis_interoropt( length,  pos1 , pos2,  route1,  route2);
	//update the perm

	for(int i=N2-1;i>pos2;i--) routeS[route2].perm[i+length]=routeS[route2].perm[i];
	for(int i=0;i<length;i++) routeS[route2].perm[i+pos2+1]=routeS[route1].perm[i+pos1];
	for(int i=pos1+length;i<N1;i++) routeS[route1].perm[i-length]=routeS[route1].perm[i];
	for(int i=0;i<length;i++) routeS[route1].perm[N1-1-i]=-1;
	//update the Nsel
	routeS[route1].Nsel-=length;
	routeS[route2].Nsel+=length;
	//update the f_city
	for(int i=pos2+1;i<pos2+1+length;i++) f_city[routeS[route2].perm[i]]=route2;
	//update the position
	for(int i=pos2+1;i<pos2+1+length;i++) position[0][routeS[route2].perm[i]]=route2;
	for(int i=pos1;i<routeS[route1].Nsel;i++) position[1][routeS[route1].perm[i]]=i;
	for(int i=pos2+1;i<routeS[route2].Nsel;i++) position[1][routeS[route2].perm[i]]=i;
	//update the Vec_sd
	update_Vsd(routeS[route1].vec_sd, pos1, 0, routeS[route1].perm);
	update_Vsd(routeS[route2].vec_sd, pos2+1, 0, routeS[route2].perm);
	//profit is updated outside

}


void struct_sol::update_cdis_interoropt(int length, int pos1 ,int pos2, int route1, int route2){
	int N1=routeS[route1].Nsel;
	int N2=routeS[route2].Nsel;
	int N1_prime=N1-length;
	int N2_prime=N2+length;
	int r1_pre=routeS[route1].perm[pos1-1];				//x_i-1
	int node1=routeS[route1].perm[pos1];				//x_i
	int r1_last=routeS[route1].perm[pos1+length-1];		//x_i+k-1
	int r1_nex=routeS[route1].perm[pos1+length];		//x_i+k
	int node2=routeS[route2].perm[pos2];				//x_j
	int r2_nex=routeS[route2].perm[pos2+1];				//x_j+1

	totalcdis-=routeS[route1].cdis;
	totalcdis-=routeS[route2].cdis;

	routeS[route1].cdis+=-length*routeS[route1].vec_sd[pos1-1];
	if(pos1+length<N1) routeS[route1].cdis+=(N1_prime-pos1)*glo_map->matrix_dis[r1_pre][r1_nex];
	for(int i=0;i<length-1;i++) routeS[route1].cdis-=(N1-pos1-1-i)*glo_map->matrix_dis[routeS[route1].perm[pos1+i]][routeS[route1].perm[pos1+i+1]];
	if(pos1+length<N1) routeS[route1].cdis-=(N1-pos1-length)*glo_map->matrix_dis[r1_last][r1_nex];
	routeS[route1].cdis-=(N1-pos1)*glo_map->matrix_dis[r1_pre][node1];

	routeS[route2].cdis+=length*routeS[route2].vec_sd[pos2];
	routeS[route2].cdis+=(N2_prime-pos2-1)*glo_map->matrix_dis[node2][node1];
	if(pos2+1<N2) routeS[route2].cdis-=(N2-pos2-1)*glo_map->matrix_dis[node2][r2_nex];
	for(int i=0;i<length-1;i++) routeS[route2].cdis+=(N2_prime-pos2-2-i)*glo_map->matrix_dis[routeS[route1].perm[pos1+i]][routeS[route1].perm[pos1+i+1]];
	if(pos2+1<N2) routeS[route2].cdis+=(N2_prime-pos2-length-1)*glo_map->matrix_dis[r1_last][r2_nex];

	totalcdis+=routeS[route1].cdis;
	totalcdis+=routeS[route2].cdis;
}

int struct_sol::try_doublebridge(int route, int pos1, int pos2, int pos3, int pos4){
	int deltacdis=0;
	int N=routeS[route].Nsel;
	int node1=routeS[route].perm[pos1];
	int node1_nex=routeS[route].perm[pos1+1];
	int node2=routeS[route].perm[pos2];
	int node2_nex=routeS[route].perm[pos2+1];
	int node3=routeS[route].perm[pos3];
	int node3_nex=routeS[route].perm[pos3+1];
	int node4=routeS[route].perm[pos4];
	int node4_nex=routeS[route].perm[pos4+1];

	deltacdis+=(pos3-pos1)*(routeS[route].vec_sd[pos4]-routeS[route].vec_sd[pos3+1]);
	deltacdis+=(pos2+pos3-pos1-pos4)*(routeS[route].vec_sd[pos3]-routeS[route].vec_sd[pos2+1]);
	deltacdis+=(pos2-pos4)*(routeS[route].vec_sd[pos2]-routeS[route].vec_sd[pos1+1]);

	deltacdis+=(N-pos1-1)*glo_map->matrix_dis[node1][node3_nex];
	deltacdis+=(N-pos1-pos4+pos3-1)*glo_map->matrix_dis[node4][node2_nex];
	deltacdis+=(N-pos1-pos4+pos2-1)*glo_map->matrix_dis[node3][node1_nex];
	if(pos4+1<N) deltacdis+=(N-pos4-1)*glo_map->matrix_dis[node2][node4_nex];

	deltacdis-=(N-pos1-1)*glo_map->matrix_dis[node1][node1_nex];
	deltacdis-=(N-pos2-1)*glo_map->matrix_dis[node2][node2_nex];
	deltacdis-=(N-pos3-1)*glo_map->matrix_dis[node3][node3_nex];
	if(pos4+1<N) deltacdis-=(N-pos4-1)*glo_map->matrix_dis[node4][node4_nex];

	return deltacdis;
}

void struct_sol::move_doublebridge(int route, int pos1, int pos2, int pos3, int pos4){
	int N=routeS[route].Nsel;
	int tmperm[N];
	//update the cdis;
	update_cdis_doublebridge(route, pos1, pos2, pos3, pos4);
	//update the perm
	for(int i=0;i<N;i++) tmperm[i]=routeS[route].perm[i];
	for(int i=pos1+1;i<pos1+pos4-pos3+1;i++) routeS[route].perm[i]=tmperm[i-pos1+pos3];
	for(int i=pos1+pos4-pos3+1;i<pos1+pos4-pos2+1;i++) routeS[route].perm[i]=tmperm[i-pos1-pos4+pos3+pos2];
	for(int i=pos1+pos4-pos2+1;i<pos4+1;i++) routeS[route].perm[i]=tmperm[i-pos4+pos2];
	//update the position
	for(int i=pos1+1;i<pos4+1;i++) position[1][routeS[route].perm[i]]=i;
	//update the vecsd
	update_Vsd(routeS[route].vec_sd, pos1, N, routeS[route].perm);
	//no need to update the f_city

	//no need to update the SbSe,

	//Nsel do not need to update, profit is updated outside;

}

void struct_sol::update_cdis_doublebridge(int route, int pos1, int pos2, int pos3, int pos4){

	int N=routeS[route].Nsel;
	int node1=routeS[route].perm[pos1];
	int node1_nex=routeS[route].perm[pos1+1];
	int node2=routeS[route].perm[pos2];
	int node2_nex=routeS[route].perm[pos2+1];
	int node3=routeS[route].perm[pos3];
	int node3_nex=routeS[route].perm[pos3+1];
	int node4=routeS[route].perm[pos4];
	int node4_nex=routeS[route].perm[pos4+1];

	totalcdis-=routeS[route].cdis;

	routeS[route].cdis+=(pos3-pos1)*(routeS[route].vec_sd[pos4]-routeS[route].vec_sd[pos3+1]);
	routeS[route].cdis+=(pos2+pos3-pos1-pos4)*(routeS[route].vec_sd[pos3]-routeS[route].vec_sd[pos2+1]);
	routeS[route].cdis+=(pos2-pos4)*(routeS[route].vec_sd[pos2]-routeS[route].vec_sd[pos1+1]);

	routeS[route].cdis+=(N-pos1-1)*glo_map->matrix_dis[node1][node3_nex];
	routeS[route].cdis+=(N-pos1-pos4+pos3-1)*glo_map->matrix_dis[node4][node2_nex];
	routeS[route].cdis+=(N-pos1-pos4+pos2-1)*glo_map->matrix_dis[node3][node1_nex];
	if(pos4+1<N) routeS[route].cdis+=(N-pos4-1)*glo_map->matrix_dis[node2][node4_nex];

	routeS[route].cdis-=(N-pos1-1)*glo_map->matrix_dis[node1][node1_nex];
	routeS[route].cdis-=(N-pos2-1)*glo_map->matrix_dis[node2][node2_nex];
	routeS[route].cdis-=(N-pos3-1)*glo_map->matrix_dis[node3][node3_nex];
	if(pos4+1<N) routeS[route].cdis-=(N-pos4-1)*glo_map->matrix_dis[node4][node4_nex];

	totalcdis+=routeS[route].cdis;
}


/***************************************************************/

void struct_route::reset(void){

	memset(perm,-1,(num_city+1)*sizeof(int));
	memset(vec_sd,0,(num_city)*sizeof(int));
	memset(Sb,0,(num_city)*sizeof(int));
	memset(Se,0,(num_city)*sizeof(int));
	profit=0;
	Nsel=0;
	cdis=0;
}

void struct_sol::reset(void){
	for(int i=0;i<num_car;i++) routeS[i].reset();

	memset(position[0], -1, num_city*sizeof(int));
	memset(position[1], -1, num_city*sizeof(int));
	memset(f_city,-1,num_city*sizeof(int));

	totalprofit=0;
	totalNsel=0;
	totalcdis=0;

}



