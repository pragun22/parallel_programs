# ASSIGNMENT 1 DS
- Pragun Saxena
- 20171127

# Quick Sort
## Description
This a master-slave type of construction. Master (process 0) distributes the array in chunks and send them to other processes. Then, each process parallely sorts their respective chunks using quick sort algorithm. Then, they send their sorted chunks back to process 0. Process 0 then merges those chunks as follows :-
- Initially, it adds minimum element of each sorted chunk into a min heap.
- It takes out the minimum element from the min heap, add it into sorted list. Next element from the chunk which this element belonged to, is added into the heap.
- Above step is repeated until the priority queue is empty.

## Analysis
For an array of 1000000 elements, time taken data is :
- Sequential quick sort : 0.238 seconds
- Parallel quick sort : 0.144 seconds

# Single-Source Shortest Path
## Description
Algorithm used is bellman-ford. This is also a master-slave type of construction. Master (process 0) distributes the vertices into chunks and sends their respective adjacency lists (storing edges involving those vertices) to corresponding processes. Now, each process has been assigned some vertices and they have adjacency list of those vertices. Process 0 has a array named wt where wt[i] denotes distance between source vertex and ith vertex. Initially, all values in wt array is infinity except source vertex for which it is zero. Now, following process is repeated n times where n is number of vertices of the graph :
- Parent process broadcasts the wt array to all other processes.
- Each child process then iterate to all edges in their adjacency list and update their wt array. Suppose they have an edge u to v with weight w, then wt[u] is made min(wt[u], w+wt[v]). (Other way around i.e. v to u is done in adjacency list of v).
- After updation, each process sends back their local wt arrays to process 0.
- Process 0 thus takes p-1 arrays (p being number of processes), takes minimum for each index of the array and updates its wt array.
After this procedure, the final wt array contains required minimum distance.

## Analysis
For 1000 vertices and 10000 edges (random), time taken data is :
- Sequential Bellman-Ford : 0.257 seconds
- Parallel Bellman-Ford : 0.201 seconds
