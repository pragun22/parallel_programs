/* MPI Program Template */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"
using namespace std;

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}
int partition(int* arr, int start, int end)
{
    srand(time(NULL));
    int ind = start + rand()%(end+1-start);
    int pivot = arr[ind];
    swap(&arr[ind], &arr[end]);
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

void quicksort(int* arr, int start, int end)
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
    /* start up MPI */
    // int n;cin>>n;
    int n = atoi(argv[1]);
    int sz = n;
    int arr[n];
    for (int i = 0; i < n; ++i)
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

    /* write your code here */
    // cout<<rank<<" with n = "<<n<<" and numprocs = "<<numprocs<<endl;
    int send_count = n / numprocs + (n%numprocs==0?0:1); 
    // cout<<(n%numprocs==0?0:1)<<" !"<<endl;
    int* recvbuf = (int*)malloc(sizeof(int) * send_count);
    MPI_Scatter(
        arr,
        send_count,
        MPI_INT,
        recvbuf,
        send_count,
        MPI_INT,
        0,
        MPI_COMM_WORLD
    );
    int t1 = send_count;
    if(n < (rank+1)*send_count) send_count = n - rank*send_count;
    if(send_count < 0) send_count = 0;
    printf("explain\n");
    cout<<rank<<" "<<send_count<<endl;    
    quicksort(recvbuf, 0, max(send_count-1,0));

    for (int i = 0; i < send_count; ++i)
    {
        cout<<recvbuf[i]<<" -- ";
    }cout<<endl;
    
    int* final = NULL;
    if(rank==0) final  = (int*)malloc(sizeof(int)*n);
    MPI_Gather(
        recvbuf,
        send_count,
        MPI_INT,
        final,
        send_count,
        MPI_INT,
        0,
        MPI_COMM_WORLD
    );
    if(rank==0)
    {
        // cout<<"HEY"<<endl;
        int run = send_count;
        while(run < n)
        {
           int* temp = (int*)malloc(sizeof(int)*(run+send_count));
           int i =0,j=0,k=0;
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
           for (int i = 0; i < k; ++i)
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
        for (int i = 0; i < n; ++i)
        {
            cout<<final[i]<<" ";
        }cout<<endl;
    }

    /* shut down MPI */
    MPI_Finalize();
    return 0;
}