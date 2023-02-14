#include <iostream>
#include <mpi.h>

/*
Calculo de Pi como la integral de 0 a 1 de 4/(1+x*x)

*/

int main(int argc, char** argv) {
    int rank;
    int size;
    int n;
    double PI = 3.141592653589793238462643;
    double myPi;
    double pi;
    double h;
    double sum;
    bool val_por_parametro = true;
    
    MPI_Status status;

    MPI_Init(&argc,&argv);
    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    /*
    MPI_Send(&rank //referencia al vector de elementos a enviar
            ,1 // tamaño del vector a enviar
            ,MPI_INT // Tipo de dato que envias
            ,rank // pid del proceso destino
            ,0 //etiqueta
            ,MPI_COMM_WORLD); //Comunicador por el que se manda
 
    MPI_Recv(&contador // Referencia al vector donde se almacenara lo recibido
            ,1 // tamaño del vector a recibir
            ,MPI_INT // Tipo de dato que recibe
            ,rank // pid del proceso origen de la que se recibe
            ,0 // etiqueta
            ,MPI_COMM_WORLD // Comunicador por el que se recibe
            ,&estado); // estructura informativa del estado
    */

    if( rank == 0 ){
        n = 20;
    }
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if( n <= 0 ){
        MPI_Finalize();
        exit(0);
    }else{
        h = 1.0 / (double) n;
        sum = 0.0;
        for(int i = rank + 1; i <= n; i += size){
            double x = h * ((double)i - 0.5);
            sum += (4.0 / (1.0 + x*x));
        }
        myPi = h * sum;

        MPI_Reduce(&myPi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

        if( rank == 0 ){
            printf("PI: %f ,error: %f", pi, (pi-PI));
        }
    }

    MPI_Finalize();

    return 0;
}