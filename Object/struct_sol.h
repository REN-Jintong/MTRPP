/*
 * struct_sol.h
 *
 *  Created on: Jan 19, 2021
 *      Author: jiren
 */

#ifndef STRUCT_SOL_H_
#define STRUCT_SOL_H_


class struct_route{
public:
	int *perm;
//	int *position;
//	int *f_city;

	int profit;
	int Nsel;
	int cdis;

	/*acceleration*/
	int *vec_sd;
	int *Sb;
	int *Se;

	/*generate and delete*/
	struct_route();
	~struct_route();
	void reset(void);

};

class struct_sol{
public:

	struct_route *routeS;
	/*total valse*/
	int **position;			// position[0][node] is the car, position[1][node] is the pos
	int *f_city;			//-1 no select,
	int totalprofit;
	int totalNsel;
	int totalcdis;
	/*generate and delete*/
	struct_sol();
	~struct_sol();
	/*Other member functions*/
	void reset(void);
	void random_inisol(void);
	void grasp_inisol(void);
	void greedy_inisol(void);
	void fetch_all(void);
	void fetch_nsel(void);
	void fetch_position(void);
	void fetch_cdis(void);
	void fetch_f_city(void);
	void fetch_profit(void);
	void fetch_vec_sd(void);
	void fetch_SbSe(void);

	void update_Vsd(int *single_Vsd, int startpos, int endpos, int *single_perm);
	void update_SeSe(int i, int j, int N, int *single_perm, int *single_Vsd, int *single_Sb, int *single_Se);
	void update_chosenT(int node1, int node2);
	void update_profit(int indroute);

	int locate_nextcity(int nowc, int *single_perm, int *single_position);
	int locate_prevcity(int nowc, int *single_perm, int *single_position);
	/*Auxiliary function*/
	int sel_greedy_random_city(int deg_good, int deg_bad, int nowc, int *long_flag);
	int sel_car_position_profit(int **CandL, bool *Fsel, int sizesub);
//	int standard_ratio_select(int nowc, int num_current);
//	int standard_nearst_select(int nowc, int num_current);

	/*function for 2opt*/
	int fast_2opt(int i, int j, int *single_Vsd, int N, int *single_perm, \
	int *single_Sb, int *single_Se, int single_cdis);
	void exe_2opt(struct_sol *S, int indroute, int smaposnode, int bigposnode);

	/*function for swap*/
	int try_swapadj(int N, int *single_perm, int pos1, int pos2, int node1, int node2);
	int try_swap(int N, int *single_perm, int pos1, int pos2, int node1, int node2);
	int get_pre_nex_cdis(int N, int pos, int node, int prenode, int nexnode);
	void move_swapadj(int indroute, int N, int *single_perm, int pos1, int pos2, int node1, int node2);
	void move_swap(int indroute, int N, int *single_perm, int pos1, int pos2, int node1, int node2);

	/*function for insert and or-opt*/
	int FwI(int i, int j, int k, int *single_Vsd, int N, int *single_perm);
	int BwI(int i, int j, int k, int *single_Vsd, int N, int *single_perm);
	void move_insert_oropt(int movpos, int fixpos, int indroute, int k);

	/*function for add*/
	int try_add(int i, int nodeadd, int N, int *Vec, int *perm);
	void move_add(int indroute, int nodeadd, int posadd);
	int update_cdis_add(int i, int nodeadd, int N, int *Vsd, int *perm);

	/*function for drop*/
	int try_drop_profit(int i, int nodedrop, int N, int *Vsd, int *perm);
	void move_drop(int indroute, int nodedrop, int posdrop);
	int update_cdis_drop(int i, int nodedrop, int N, int *Vsd, int *perm);

	/*function for interswap*/
	int try_interswap(int fixnode, int swapnode, int route1, int route2, int pos1, int pos2); //It is an estimate of cdis
	void move_interswap(int fixnode, int swapnode, int route1, int route2, int pos1, int pos2);
	void update_cdis_interswap(int fixnode, int swapnode, int route1, int route2, int pos1, int pos2);

	/*function for interinsert*/
	int try_interinsert(int fixnode, int objnode, int route1, int route2, int pos1, int pos2); //It is an estimate of cdis
	void move_interinsert(int fixnode, int objnode, int route1, int route2, int pos1, int pos2);
	void update_cdis_interinsert(int fixnode, int objnode, int route1, int route2, int pos1, int pos2);

	/*functions for inter2opt*/
	int try_inter2opt(int route1, int route2, int pos1, int pos2);
	void move_inter2opt(int route1, int route2, int pos1, int pos2);
	void update_cdis_inter2opt(int route1, int route2, int pos1, int pos2);

	/*functions for interoropt*/
	int try_interoropt(int length, int pos1 ,int pos2, int route1, int route2);
	void move_interoropt(int length, int pos1 ,int pos2, int route1, int route2);
	void update_cdis_interoropt(int length, int pos1 ,int pos2, int route1, int route2);

	/*functions for doublebridge*/
	int try_doublebridge(int route, int pos1, int pos2, int pos3, int pos4);
	void move_doublebridge(int route, int pos1, int pos2, int pos3, int pos4);
	void update_cdis_doublebridge(int route, int pos1, int pos2, int pos3, int pos4);
};

#endif /* STRUCT_SOL_H_ */
