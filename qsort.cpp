/* MPI Program Template */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"

int partition(int* arr, int start, int end)
{
    srand(time(NULL));
    int ind = start + rand()%(end+1-start);
    int pivot = arr[ind];
    swap(&arr[ind], &arr[end])
    int itr = start;
    for(int i = start ; i < end ; i++)
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

int quicksort(int* arr, int start, int end)
{
    if(start < end)
    {
        int part = partition(arr, start, end);
        quicksort(arr, start, part - 1);
        quicksort(arr, part + 1, end);
    }
}

int main( int argc, char **argv ) {
    int rank, numprocs;
    std::vector<int> v(n);
    for (int i = 0; i < n; ++i)
    {
        cin>>v[i];
    }
    /* start up MPI */
    MPI_Init( &argc, &argv );

    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &numprocs );
    
    /*synchronize all processes*/
    // MPI_Barrier( MPI_COMM_WORLD );
    // double tbeg = MPI_Wtime();

    /* write your code here */
    int send_count = n / numprocs;
    int extra = n % numprocs;
    int* recvbuf = (int*)malloc(sizeof(int) * send_count);
    MPI_Scatter(
        v,
        send_count,
        MPI_INT,
        recvbuf,
        send_count,
        MPI_INT,
        0,
        MPI_COMM_WORLD
    );

    quicksort(recvbuf, 0, send_count-1);

    MPI_Barrier( MPI_COMM_WORLD );
    double elapsedTime = MPI_Wtime() - tbeg;
    double maxTime;
    MPI_Reduce( &elapsedTime, &maxTime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD );
    if ( rank == 0 ) {
        printf( "Total time (s): %f\n", maxTime );
        cout<<"Output array \n";
        for (int i = 0; i < n; ++i)
        {
            cout<<arr[i]<<" ";
        }cout<<endl;
    }

    /* shut down MPI */
    MPI_Finalize();
    return 0;
}