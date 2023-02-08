#include <iostream>
//#include <chrono>
//#include <>
#include <thread>
#include <mpi.h>

//para utlizar la sintaxis ms
using namespace std::chrono_literals;

int main(int argc, char** argv) {
    int rank;
    int size;
    int data[100];

    //numero aleatorio 1-30
    //int number = 1 + rand() % 30;

    MPI_Init(&argc,&argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::printf( "Hola, soy el rank %d, de un total de %d procesos\n", rank, size );

    //Sincroniza todos los procesos para que empiecen al mismo tiempo
    MPI_Barrier(MPI_COMM_WORLD);
    
    double start_time = MPI_Wtime();

    //trabajo aleatorio 1-30seg
    //int number = 1 + rand() % 30;
    //std::this_thread::sleep_for( number * 1000ms );
    double rank_time = ( rank + 1 );
    std::this_thread::sleep_for( ( rank + 1 ) * 1000ms );
    
    //volver a sincronizar
    //MPI_Barrier(MPI_COMM_WORLD);

    double main_time = MPI_Wtime() - start_time;
    
    if(rank == 0){
        std::printf("Time for work is %lf seconds\n", main_time);
    }else {
        std::printf("Rank %d - Time for work is %lf seconds\n",rank, main_time);
    }

    MPI_Finalize();

    return 0;
}