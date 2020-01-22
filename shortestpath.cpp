/* MPI Program Template */

// #include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <limits.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <vector>
#include "mpi.h"
using  namespace std;

std::vector<pair<int, int>> adj[100002];
int main( int argc, char **argv ) {
    int rank, numprocs;
    int n,m;
    string line;
    ifstream f(argv[1]);
    getline(f, line);
    bool flag = true;
    string temp = "";
    for (int i = 0; i < line.size(); ++i)
    {
        if(i==line.size()-1)
        {
            temp+=line[i];
            m = stoi(temp);
        }
        else if(line[i]==' ' || line[i]=='\n')
        {
            if(flag) {n = stoi(temp);flag=false;}
            else m = stoi(temp);
            temp = "";
        }
        else temp+=line[i];
    }
    // cout<<n<<" "<<m<<endl;
    for (int i = 0; i < m; ++i)
    {
        int a[3];
        int ind = 0;
        getline(f, line);
        temp="";
        for (int i = 0; i < line.size(); ++i)
        {
            if(i == line.size()-1)
            {
                temp+=line[i];
                a[ind++] = stoi(temp);
            }
            else if(line[i]==' ' || line[i]=='\n')
            {
                a[ind++] = stoi(temp);
                temp = "";
            }
            else temp+=line[i];
        }
        adj[a[0]].push_back({a[1], a[2]});
        adj[a[1]].push_back({a[0], a[2]});
        // cout<<a[0]<<" <-> "<<a[1]<<" ** "<<a[2]<<endl;
    }
    getline(f, line, ' ');
    int source = stoi(line);
    // cout<<source<<endl;

    MPI_Init( &argc, &argv );

    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &numprocs );
    
    /*synchronize all processes*/
    MPI_Barrier( MPI_COMM_WORLD );
    double tbeg = MPI_Wtime();

    /* write your code here */


    MPI_Barrier( MPI_COMM_WORLD );
    double elapsedTime = MPI_Wtime() - tbeg;
    double maxTime;
    MPI_Reduce( &elapsedTime, &maxTime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD );
    if ( rank == 0 ) {
        printf( "Total time (s): %f\n", maxTime );
    }

    /* shut down MPI */
    MPI_Finalize();
    return 0;
}