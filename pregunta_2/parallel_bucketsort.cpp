#include <mpi.h>
#include <math.h>
#include <time.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
using namespace std;


int main(int argc, char **argv) {
  
  int rank, size;
  MPI_Init(&argc, &argv);                   // Initialize MPI
  MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Rank of the processor
  MPI_Comm_size(MPI_COMM_WORLD, &size); // Total number of processors
  
  int bucket_size = size;
  int bucket_size_p = size;
  int i_p,j_p,bucket_index_p,index_p = 0;
  int i, n = pow(2, 3);     
  float* randArray,*resultArray;
  std::vector<float>bucket_p((int)n/bucket_size_p);
  randArray = new float[n];
  resultArray = new float[n];
  if(rank == 0) {
    srand((int)time(0));
    for(int i = 0; i < n; ++i)  randArray[i]=(float)rand()/(float)(RAND_MAX/999.0);
    
    for (i = 0; i < n; ++i) 
    	printf("%1.2f, ", randArray[i]);
	  printf("\n");
    
    // ordenar array en buckets
    //bucketSort(randArray, n, 8);

    // Crear buckets      
    std::vector<float> bucket[bucket_size];
    int i, j, bucket_index, index = 0;
  
    // asignar elementos a los buckets
    

    /*for (i = 0; i < n; ++i) {
        bucket_index = bucket_size*randArray[i]/1000;
        bucket[bucket_index].push_back(randArray[i]);
    }

    // ordenar buckets
    for (j = 0; j < bucket_size; ++j)
      sort(bucket[j].begin(), bucket[j].end());

    // Concatenar buckets en arr[]
    for (i = 0; i < bucket_size; i++) {
      for (j = 0; j < bucket[i].size(); j++)
        randArray[index++] = bucket[i][j];
    }
    */
    for (i = 0; i < n; ++i) printf("%1.2f, ", randArray[i]);
  }

  /*for(int g = 1; g < n; g++){
    MPI_Send( array_bucket , n/bucket_size_p , MPI_FLOAT , g , MPI_ANY_TAG , MPI_COMM_WORLD);
  }*/
  MPI_Scatter( randArray , n/bucket_size_p , MPI_FLOAT , &bucket_p.front() , n/bucket_size_p , MPI_FLOAT , 0 , MPI_COMM_WORLD);    
  MPI_Barrier( MPI_COMM_WORLD);
  
  for(int k = 0 ; k < n; k++){
    bucket_index_p = bucket_size_p * (randArray[k])/1000;
    bucket_p[bucket_index_p] = (randArray[k]);
    std::cout<<"Index: "<<bucket_index_p<<" "<<bucket_p[bucket_index_p]<<std::endl;
  }

  sort(bucket_p.begin(), bucket_p.end());
  MPI_Gather( &bucket_p.front() , n/bucket_size_p , MPI_FLOAT , resultArray , n/bucket_size_p , MPI_FLOAT , 0 , MPI_COMM_WORLD);

  /*for(int f = 0; f < (int)n/bucket_size_p ; f++){
    std::cout<<bucket_p[f]<<" ";
  }*/
  std::cout<<std::endl;
  if(rank == 0){
    for(int f = 0; f < n ; f++){
      std::cout<<bucket_p[f]<<" ";
    }
  }

  delete[] randArray;
  MPI_Finalize();
}
