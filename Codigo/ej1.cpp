#include <iostream>
#include <time.h>
#include <stdio.h>
#include <vector>
#include <mpi.h>
#include <algorithm>

#define SIZE 9
#define N 10
#define DOMAIN 18
using namespace std;
 
void bucketSort(int a[],int me, int numprocs) {
    int i, j, k;
    int range_per_node = N/(numprocs-1);
    int buckets_per_node = SIZE/(numprocs-1);
    vector<int> buckets[SIZE];
    if(me==0){ 
        for(i = 0; i < N; ++i){
            buckets[a[i]/range_per_node].push_back(a[i]);
        }
        for(i = 1;i<numprocs;i++){
            for(j = 0;j<buckets_per_node;j++){
                int tamano = buckets[(i-1)*buckets_per_node+j].size();
                MPI_Send(&tamano,1,MPI_INT,i,2*j,MPI_COMM_WORLD);
                MPI_Send(&buckets[i*buckets_per_node+j],tamano,MPI_INT,i,2*j+1,MPI_COMM_WORLD);

            }
        }
        int count=0;
        for(i = 1;i<numprocs-1;i++){
            for(j = 0;j<buckets_per_node;j++){
                MPI_Recv(&a[count],buckets[(i-1)*buckets_per_node+j].size(),MPI_INT,i,j,MPI_COMM_WORLD,MPI_STATUS_IGNORE); 
                count+=buckets[(i-1)*buckets_per_node+j].size();
            }
        }
    }else{
        int tamano;
        for(j=0;j<buckets_per_node;j++){
            MPI_Recv(&tamano,1,MPI_INT,0,2*j,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            printf("Tamaño recibido es %d en %d\n", tamano, me);
            int bucket[tamano];
            MPI_Recv(&bucket,tamano,MPI_INT,0,2*j+1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            sort(&bucket[0],&bucket[tamano-1]);
            //cout<<"hola desde el proceso"<< me <<endl;
            MPI_Send(&bucket[0],tamano,MPI_INT,0,j,MPI_COMM_WORLD);
        }
    }

}
 
int main(int argc, char *argv[]) {
   int me, numprocs;
   double data[2];
   MPI_Init(&argc,&argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &me);
   MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
   srand(time(NULL));
   int i, a[long(N)], n = long(N);

    if (me ==0){
        cout << "Before sorting:\n";
        for(int i = 0; i<N; ++i){
            a[i] = rand()%DOMAIN;
            cout << a[i] << " ";
        }
        cout << endl;            
    }
    bucketSort(a,me,numprocs);
    // MPI_Barrier(MPI_COMM_WORLD);
    // if(me==0){
    //     cout<< "\n\nAfter sorting:\n";
    //     for(i = 0; i < n; ++i){
    //     cout<<a[i]<<" ";
    //     }
    //     //cout<<endl;
    //     cout << "Validation: " << endl;
    //     int temp = 0;
    //     for(i = 0; i < n; ++i){
    //         if(temp > a[i]){
    //             cout<<"wrong\n";
    //             break;
    //         }
    //         temp = a[i];
    //     }
    // }
    MPI_Finalize();
   return 0;
}
