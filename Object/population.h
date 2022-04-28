/*
 * struct_sol.h
 *
 *  Created on: Jan 19, 2021
 *      Author: jiren
 */

#ifndef POPULATION_H_
#define POPULATION_H_

class Population{
public:
	struct_sol *PSol;
	double **dismat;
//	double *avg_dis;
//	double *quality;
//	double *score;


	Population();
	~Population();

	void Ini_Pop(void);
	void Create_dismat(void);
	double get_dis_arc(struct_sol *S1, struct_sol *S2);
	void Generate_offspring_arcbased(struct_sol *S);
	void diversification_update_sol(struct_sol *S);

	void Generate_offspring_routebased(struct_sol *S);


	/*********Pos based operation***********/
	int get_dis_pos(int *vec1, int *vec2);
	void Generate_offspring(struct_sol *S);
	void simple_update_pool(struct_sol *S);
	void Update_Pool(struct_sol *S);

	void remplir_gt(int *vec, struct_sol *S);
	int find_pos(int *vec1, int *gt2, int orpos1);
};


#endif /* POPULATION_H_ */
