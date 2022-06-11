#include <iostream>
#include <time.h>
#include <stdio.h>
#include <vector>
#include <mpi.h>
#include <algorithm>

#define SIZE 524287
#define N 524288 // 2e19
using namespace std;

int findBucketIndex(float elem){
    return (int) SIZE*elem/1000;
}
 
void bucketSort(float a[], int num_per_bucket[], int me, int numprocs) {
    int buckets_per_node = SIZE/(numprocs-1);
    if(me==0){
        vector<float*> buckets(SIZE);
        int current_index_per_bucket[SIZE];

        for(int i = 0; i < SIZE; i++){
            buckets[i] = new float[num_per_bucket[i]];
            current_index_per_bucket[i] = 0;
        }
        for(int i = 0; i < N; i++){
            int bucket_index = findBucketIndex(a[i]);
            buckets[bucket_index][current_index_per_bucket[bucket_index]] = a[i];
            current_index_per_bucket[bucket_index]++;
        }

        for(int i = 1; i < numprocs; i++){
            for(int j = 0; j < buckets_per_node; j++){
                int index = (i-1)*buckets_per_node+j;
                int tamano = num_per_bucket[index];
                MPI_Send(&tamano, 1, MPI_INT, i, 2*j, MPI_COMM_WORLD);
                MPI_Send(buckets[index], tamano, MPI_FLOAT, i, 2*j+1, MPI_COMM_WORLD);
            }
        }

        for(int i = 0; i < buckets.size(); i++)
            delete[] buckets[i];

        int count=0;
        
        for(int i = 1; i < numprocs; i++){
            for(int j = 0; j < buckets_per_node; j++){
                int index = (i-1)*buckets_per_node+j;
                int tamano = num_per_bucket[index];
                MPI_Recv(&a[count], tamano, MPI_FLOAT,i,j,MPI_COMM_WORLD,MPI_STATUS_IGNORE); 
                count += tamano;
            }
        }
    }else{
        int tamano;
        for(int j = 0; j < buckets_per_node; j++){
            MPI_Recv(&tamano, 1, MPI_INT, 0, 2*j, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            float bucket[tamano];
            MPI_Recv(&bucket,tamano,MPI_FLOAT,0,2*j+1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            if(tamano != 0)
                sort(bucket, bucket + tamano);
            MPI_Send(&bucket[0], tamano, MPI_FLOAT, 0, j, MPI_COMM_WORLD);
        }
    }

}
 
int main(int argc, char *argv[]) {
   int me, numprocs;
   MPI_Init(&argc,&argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &me);
   MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
   srand(time(NULL));
   int n = long(N);
   int num_per_bucket[SIZE];
   float a[n];
   double t0, tf;

    if (me ==0){
        for(int i = 0; i < N; i++)
            num_per_bucket[i] = 0;

        for(int i = 0; i<N; ++i){
            a[i] = (float)rand()/(float)(RAND_MAX/999.0);
            int bucket_index = findBucketIndex(a[i]);
            num_per_bucket[bucket_index]++;
        }
        t0 = MPI_Wtime();
    }
    bucketSort(a, num_per_bucket, me, numprocs);
    if(me == 0){
        tf = MPI_Wtime();
        cout << tf - t0 << endl;
    }
    // MPI_Barrier(MPI_COMM_WORLD);
    // if(me == 0){
    //     cout << "Validacion: " << endl;
    //     int temp = 0;
    //     for(int i = 0; i < n; ++i){
    //         if(temp > a[i]){
    //             cout << "ERROR\n";
    //             break;
    //         }
    //         temp = a[i];
    //     }
    // }
    MPI_Finalize();
    return 0;
}
