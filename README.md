# MTRPP

Multiple traveling repariman problem with profits.

We propose an effective hybrid search algorithm under the framework of memetic algorithm.

Here is the benchmark instances and the source code of the proposed algorithm.

The benchmark instances are grouped into 2 groups collected from [1][2]: Ins_Avci (230 instances) and Ins_Lu (240 instances).

Both instances can be downloaded here (We have get the permission of the orginal authors).

The orginal source of  Instances_Avci is https://www.sciencedirect.com/science/article/pii/S030505481930187X. Here we made some changes to the interfaces of the instances to adapt to our code. 

The source code of EHSA-MTRPP is implemented in C++ and compiled with g++ 7.5.0. It will be made available upon the publication of the manuscript.
The manuscript is in submitting. 


/*update 2022.05.11*/

For each instance, the first line gives the number of the nodes (depot + customers) and the number of vehicles (repairmen).
Following that, the coordinates as well as the profits of each node are listed.

[1] M. G. Avci, M. Avci, An adaptive large neighborhood search approach for multiple traveling repairman problem with profits, Computers & Operations Research 111 (2019) 367–385.

[2] Y. Lu, U. Benlic, Q. Wu, B. Peng, Memetic algorithm for the multiple traveling repairman problem with profits, Engineering Applications of Artificial Intelligence 80 (2019) 35–47.


This work is already accepted by European journal of Operational Research.

Compile: g++ /*.cpp Object/*.cpp Operator/*.cpp -O3 -lm -o MTRPP
Run: ./MTRPP -i instance --seed 0 -rep 0

notes: 
0. g++ with 7.5.0 version or more recent.
1. "-i" is the benchmark instance.
2. "--seed" is the seed for the random, you can change it according to your requirements.
3. "-rep" is a flag for the output file, you can ignore it if you do not use it.
