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
typedef long long int ll;
struct edges{
	ll a,b,w;
};
int main( int argc, char **argv ) {
    int rank, numprocs;
    ll n,m;
    string line;
    ifstream f(argv[1]);
    getline(f, line);
    bool flag = true;
    string temp = "";
    for (ll i = 0; i < line.size(); ++i)
    {
        if(i==line.size()-1)
        {
            temp+=line[i];
            m = stoll(temp);
        }
        else if(line[i]==' ' || line[i]=='\n')
        {
            if(flag) {n = stoll(temp);flag=false;}
            else m = stoll(temp);
            temp = "";
        }
        else temp+=line[i];
    }
    edges ed[2*m];
    for (ll i = 0; i < m; ++i)
    {
        ll a[3];
        ll ind = 0;
        getline(f, line);
        temp="";
        for (ll i = 0; i < line.size(); ++i)
        {
            if(i == line.size()-1)
            {
                temp+=line[i];
                a[ind++] = stoll(temp);
            }
            else if(line[i]==' ' || line[i]=='\n')
            {
                a[ind++] = stoll(temp);
                temp = "";
            }
            else temp+=line[i];
        }
        ed[i].a = a[0];
        ed[i].b = a[1];
        ed[i].w = a[2];

        ed[m+i].b = a[0];
        ed[m+i].a = a[1];
        ed[m+i].w = a[2];
    }
    getline(f, line, ' ');
    ll source = stoll(line);

    MPI_Init( &argc, &argv );

    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &numprocs );
    
    /*synchronize all processes*/
    MPI_Barrier( MPI_COMM_WORLD );
    double tbeg = MPI_Wtime();

    /* write your code here */
 	MPI_Datatype mpi_ed;
  	MPI_Type_contiguous(3, MPI_LONG_LONG, &mpi_ed);
 	MPI_Type_commit(&mpi_ed);
    ll extra =( 2 * m) % numprocs;
    ll send_count = (2 * m) / numprocs; 
    
    edges * edgebuf = (edges*)malloc(sizeof(edges) * send_count);
    MPI_Scatter(
        ed + extra,
        send_count,
        mpi_ed,
        edgebuf,
        send_count,
        mpi_ed,
        0,
        MPI_COMM_WORLD
    );
    ll *dist = (ll*)malloc(sizeof(ll*)*(n+1)); 
	ll* t_d = (ll*)malloc(sizeof(ll*)*(n+1));
    for (ll i = 0; i < n+1; ++i)
    {
    	dist[i] = LLONG_MAX;
    }
    dist[source] = 0;
    
    ll rec_count = send_count;
 	MPI_Barrier( MPI_COMM_WORLD ); 
    for (ll k = 0; k < n; ++k)
    {
	    for (ll i = 0; i < rec_count; ++i)
	    {
			ll u = edgebuf[i].a;
			ll v = edgebuf[i].b;
			ll w = edgebuf[i].w;
			if(dist[u]!= LLONG_MAX && dist[u] + w < dist[v])
			{
				dist[v] = dist[u] + w;	
			}
	    }
	    if(rank == 0)
	    {
	    	for (ll i = 0; i < extra; ++i)
	    	{
				ll u = ed[i].a;
				ll v = ed[i].b;
				ll w = ed[i].w;
				if(dist[u]!= LLONG_MAX && dist[u] + w < dist[v])
				{
					dist[v] = dist[u] + w;	
				}
	    	}	
	    }
 		MPI_Barrier( MPI_COMM_WORLD );
	    MPI_Allreduce(
	    	dist,
	    	t_d,
	    	n+1,
	    	MPI_LONG_LONG,
	    	MPI_MIN,
	    	MPI_COMM_WORLD
	    );
	    for (int i = 1; i <= n; ++i)
	    {
	    	dist[i] = t_d[i];
	    }
    }
    MPI_Barrier( MPI_COMM_WORLD );
    double elapsedTime = MPI_Wtime() - tbeg;
    double maxTime;
    MPI_Reduce( &elapsedTime, &maxTime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD );
    
    if ( rank == 0 ) {
        
        printf( "Total time (s): %f\n", maxTime );

    	ofstream myfile;
    	myfile.open(argv[2]);
    	for(int i = 1 ; i <= n ; i++)
    	{
            if(dist[i]==LLONG_MAX) dist[i] = -1;
    		string temp = to_string(i) + " " + to_string(dist[i]) + "\n";
    		myfile << temp;
    	}
    	myfile.close();
    }
    MPI_Finalize();
    return 0;
}