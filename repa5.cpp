#include <iostream>
#include <mpi.h>

/*
Pi por 1000000000 iteraciones
*/
#define NUM_ITERAC 1000000000

int main(int argc, char** argv) {
    int rank;
    int size;
    double pi = 0.0;
    double term = 0.0;

    MPI_Init(&argc,&argv);
    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    for(int i = rank; i< NUM_ITERAC; i += size){
        term = (i % 2 ==0 ? 1 : -1) * 4.0 / (2*i+1);
        pi += term;
    }
    
    double suma_total;
    MPI_Reduce(&pi, &suma_total, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if( rank == 0 ){
        printf("Valor de pi: %f\n",suma_total);
    }

    MPI_Finalize();

    return 0;
}