/*
 * struct_map.h
 *
 *  Created on: Jan 19, 2021
 *      Author: jiren
 */

#ifndef STRUCT_MAP_H_
#define STRUCT_MAP_H_

class struct_map{
public:
	/*variables*/

	int **matrix_chosenT;
	int **matrix_dis;
	int **CandidateSet_NF;
	int	**CandidateSet_LK;
	int *Num_pernode_nf;
	int *Num_pernode_lk;
	int *profitA;
	int *cordx;
	int *cordy;
	double **Profit_dis;
	bool *Fsel;
	/*functions*/
	struct_map();
	~struct_map();
	/*check-out*/
	void remplir_map(void);
	void layoutmap(void);
	int FindGoodCity(int nowc);
	void ini_chosenT(void);
};

#endif /* STRUCT_MAP_H_ */
