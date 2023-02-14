#include <iostream>
#include <mpi.h>

/*
Suma de un conjunto de datos
Sumatoria desde i=0 hasta 99 de x
*/

int main(int argc, char** argv) {
    int rank;
    int size;
    int data[100];
    
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

    if(  rank == 0){
        for(int i = 0; i<100; i++){
            data[i] = i;
        }
        printf("Rank 0 enviando...\n");
        MPI_Send(&data[50], 50, MPI_INT, 1, 0,MPI_COMM_WORLD);

        int suma_parcial = 0;
        for(int i = 0; i<50; i++){
            suma_parcial = suma_parcial + data[i];
        }

        int suma_parcial_2 = 0;
        MPI_Recv(&suma_parcial_2, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        int suma_total = suma_parcial + suma_parcial_2;

        printf("Suma total: %d\n",suma_total);
    }
    else if( rank == 1 ){
        printf("Rank %d recibiendo...\n",rank);
        MPI_Recv(data, 50, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Rank %d recibiendo...datos[99]= %d\n", rank, data[0]);

        int suma_parcial=0;
        for(int i = 0; i<50; i++){
            suma_parcial = suma_parcial + data[i];
        }

        MPI_Send(&suma_parcial, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}