#include <iostream>
#include <cmath>
#include <mpi.h>

/*
Calcula todos los numeros primos dado un numero

*/

bool es_primo(int n){
    if(n < 2) return false;
    for(int i = 2; i<= sqrt(n); i++){
        if(n % i == 0) return false;
    }
    return true;
}


int main(int argc, char** argv) {
    int rank;
    int size;
    int n = 100;
    
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if( rank == 0 ){
        int limite = n;
        int salto = limite / size;
        int num_inicial = 2;

        for(int i = 1; i<size; i++){
            int numero_final = num_inicial + salto -1;
            MPI_Send(&num_inicial, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&numero_final, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            num_inicial = numero_final + 1;
        }

        int contador_primos = 0;
        for(int i=num_inicial; i<= limite; i++){
            if(es_primo(i)){
                contador_primos ++;
            }
        }

        for(int i = 1; i<size; i++){
            int contador;
            MPI_Recv(&contador, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            contador_primos += contador;
        }

        printf("Se encontró: %d numeros primos\n", contador_primos);
    }else{
        int num_inicial;
        int numero_final;
        MPI_Recv(&num_inicial, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&numero_final, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        int contador_primos = 0;
        for(int i = num_inicial; i<numero_final; i++){
            if(es_primo(i)){
                contador_primos ++;
            }
        }

        MPI_Send(&contador_primos, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    
    MPI_Finalize();

    return 0;
}