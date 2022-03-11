# MTRPP

Multiple traveling repariman problem with profits.

We propose an effective hybrid search algorithm under the framework of memetic algorithm.

Here is the benchmark instances and the source code of the proposed algorithm.

The benchmark instances are grouped into 2 groups collected from [1][2]: Ins_Avci (230 instances) and Ins_Lu (240 instances).

The Ins_Avci can be downloaded from https://www.sciencedirect.com/science/article/pii/S030505481930187X.
The Ins_Lu can be downloaded here (We have get the permission of the orginal authors).

The source code is implemented in C++ and compiled with g++ 7.5.0. It will be made available upon the publication of the manuscript.
The manuscript is in submitting. 

For each instance, the first line gives the number of the nodes (depot and customers) and the number of vehicles (repairmen).
Following that, the coordinates as well as the profits of each node are listed.

[1] M. G. Avci, M. Avci, An adaptive large neighborhood search approach for multiple traveling repairman problem with profits, Computers & Operations Research 111 (2019) 367–385.

[2] Y. Lu, U. Benlic, Q. Wu, B. Peng, Memetic algorithm for the multiple traveling repairman problem with profits, Engineering Applications of Artificial Intelligence 80 (2019) 35–47.
