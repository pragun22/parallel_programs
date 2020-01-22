/* MPI Program Template */

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
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
    /* start up MPI */
    // ll n;cin>>n;
    ll n = atoi(argv[1]);
    ll sz = n;
    ll arr[n+100000];
    for (ll i = 0; i < n; ++i)
    {
        // cin>>arr[i];
        arr[i] = atoi(argv[i+2]);
    }
    MPI_Init( &argc, &argv );

    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &numprocs );
    
    /*synchronize all processes*/
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
    /* write your code here */
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
    // prllf("explain\n");
    // cout<<rank<<" "<<send_count<<endl;    
    quicksort(recvbuf, 0, max(send_count-1,0LL));

    // for (ll i = 0; i < send_count; ++i)
    // {
    //     cout<<recvbuf[i]<<" -- ";
    // }cout<<endl;
    
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
        // cout<<"HEY"<<endl;
        ll run = send_count;
        ll* temp = (ll*)malloc(sizeof(ll)*n);
        while(run < n)
        {
           ll i =0,j=0,k=0;
           while(i < run && j < send_count)
           {
                if(final[i] <= final[j+run])
                {
                    temp[k++] = final[i];
                    i++; 
                }
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
    double elapsedTime = MPI_Wtime() - tbeg;
    double maxTime;
    MPI_Reduce( &elapsedTime, &maxTime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD );
    if ( rank == 0 ) {
        printf( "Total time (s): %f\n", maxTime );
        cout<<"Output array \n";
        for (ll i = 0; i < orig_n; ++i)
        {
            cout<<final[i]<<" ";
        }cout<<endl;
    }

    /* shut down MPI */
    MPI_Finalize();
    return 0;
}