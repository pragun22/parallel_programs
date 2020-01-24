/* MPI Program Template */

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <fstream>
#include "mpi.h"
using namespace std;
typedef long long int ll;
void swap(ll *a, ll *b)
{
    ll temp = *a;
    *a = *b;
    *b = temp;
}
ll partition(ll* arr, ll start, ll end)
{
    srand(time(NULL));
    ll ind = start + rand()%(end+1-start);
    ll pivot = arr[ind];
    swap(&arr[ind], &arr[end]);
    ll itr = start;
    for(ll i = start ; i < end ; i++)
    {
        if( arr[i]  < pivot)
        {   
            swap(&arr[itr], &arr[i]);
            itr++;
        }
    }
    swap(&arr[itr], &arr[end]);
    return itr;
}

void quicksort(ll* arr, ll start, ll end)
{
    if(start < end)
    {
        ll part = partition(arr, start, end);
        quicksort(arr, start, part - 1);
        quicksort(arr, part + 1, end);
    }
}

int main( int argc, char **argv ) {
    int rank, numprocs;
    string line;
    ifstream f(argv[1]);
    getline(f, line);
    string temp = "";
    ll cnt = 0;
    for (int i = 0; i < line.size(); ++i)
    {
        if(i==line.size() || line[i] == ' ') cnt++;
    }
    ll* arr = (ll*)malloc(sizeof(ll)*cnt);
    ll n = 0;
    for (int i = 0; i < line.size(); ++i)
    {
        if(i==line.size()-1)
        {
            temp+=line[i];
            arr[n++] = stoll(temp);
        }
        else if(line[i]==' ' || line[i]=='\n')
        {
            arr[n++] = stoll(temp);
            temp = "";
        }
        else temp+=line[i];
    }
    MPI_Init( &argc, &argv );

    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &numprocs );

    MPI_Barrier( MPI_COMM_WORLD );
    double tbeg = MPI_Wtime();
    ll orig_n = n;
    if(n%numprocs != 0)
    {
        ll extra = numprocs - n%numprocs;
        for (ll i = 0; i < extra; ++i)
        {
            arr[i+n] = LLONG_MAX;
        }
        n+=extra;
    }
    ll send_count = n / numprocs; 
    ll* recvbuf = (ll*)malloc(sizeof(ll) * send_count);
    MPI_Scatter(
        arr,
        send_count,
        MPI_LONG_LONG,
        recvbuf,
        send_count,
        MPI_LONG_LONG,
        0,
        MPI_COMM_WORLD
    );
    if(n < (rank+1)*send_count) send_count = n - rank*send_count;
    if(send_count < 0) send_count = 0;
    quicksort(recvbuf, 0, max(send_count-1,0LL));

    ll* final = NULL;
    if(rank==0) final  = (ll*)malloc(sizeof(ll)*n);
    MPI_Gather(
        recvbuf,
        send_count,
        MPI_LONG_LONG,
        final,
        send_count,
        MPI_LONG_LONG,
        0,
        MPI_COMM_WORLD
    );
    if(rank==0)
    {
        ll run = send_count;
        ll* temp = (ll*)malloc(sizeof(ll)*n);
        while(run < n)
        {
           ll i =0,j=0,k=0;
           while(i < run && j < send_count)
           {
                if(final[i] <= final[j+run]) temp[k++] = final[i++];
                else
                {
                    temp[k++] = final[j+run];
                    j++;
                } 
           }
           while(i<run) temp[k++] = final[i++];
           while(j<send_count) temp[k++] = final[j + run], j++;
           for (ll i = 0; i < k; ++i)
           {
                final[i] = temp[i];
           }
            run+=send_count;
        }
    }
    MPI_Barrier( MPI_COMM_WORLD );
    if ( rank == 0 ) {

        ofstream myfile;
        myfile.open(argv[2]);

        for (ll i = 0; i < orig_n; ++i)
        {
            string temp = to_string(final[i]) + " ";
            myfile << temp;
        }
        myfile << "\n";
        myfile.close();
    }
    /* shut down MPI */
    MPI_Finalize();
    return 0;
}