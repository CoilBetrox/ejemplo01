#include <iostream>
#include <mpi.h>

/*
Encadena el envío de mensajes
Proceso 1 envía al 2, el 2 al 3, el 3 al 4...

*/

int main(int argc, char** argv) {
    int rank;
    int size;
    int contador;
    
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
        MPI_Send(&rank, 1, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
    }
    else {
        MPI_Recv(&contador, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD, &status);
        printf("Proceso %d y he recibido %d\n", rank, contador);
        contador++;
        if( rank != size-1 ){
            MPI_Send(&contador, 1, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();

    return 0;
}