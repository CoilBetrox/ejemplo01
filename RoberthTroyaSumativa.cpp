//Roberth Troya 
#include <iostream>
#include <mpi.h>
#include <omp.h>
#include <fstream>
#include <vector>

using namespace std;
#define nthreads 4

string nombreArchivo = "datos.txt";
ifstream archivo(nombreArchivo.c_str());
string linea;

//Serial
std::vector<int> frec_serial( string archiv ) {
    ifstream archivo(archiv.c_str());
    std::vector<int> repeticiones;
    string linea;
    while (getline(archivo, linea)) {
        
        //cout << "Una línea: ";
        int num = stoi(linea);      //convierte a int
        for ( int i = 0; i <= 100; i++ ){
            if(num == i){
                repeticiones[i] += 1;
            }
        }
        //cout << linea << endl;
    }
    return repeticiones;
}


//genera formato de impresion
void format_serial(std::vector<int> frec_serial){
    for( int i = 0; i <= 100; i++ ){
        std::printf("%d %d %d\n",i , frec_serial[i], frec_serial[i]/100 );
    }
}


//OMP
std::vector<int> frec_omp( string archiv ) {
    ifstream archivo(archiv.c_str());
    std::vector<int> repeticiones;
    string linea;

    int thread;
    omp_set_num_threads(nthreads);
    #pragma omp parallel private(thread)
    {
        #pragma omp for schedule( auto )
        for (int i = 0; i <= 100; i++) {
            
            while (getline(archivo, linea)) {
                int num = stoi(linea);  
                if( i == num ){
                    repeticiones[i] +=1;
                }
            }
        }
    }
    return repeticiones;
}


//MPI
int main(int argc, char** argv) {

/*
    string nombreArchivo = "datos.txt";
    ifstream archivo(nombreArchivo.c_str());
    string linea;
    
    //Serial
    std::vector<int> frec1 = frec_serial(nombreArchivo);
    format_serial(frec1);

    //omp
    std::vector<int> frec2 = frec_omp(nombreArchivo);
    format_serial(frec2);

*/

    //MPI
    int rank;
    int size;

    string nombreArchivo = "datos.txt";
    ifstream archivo(nombreArchivo.c_str());
    std::vector<int> repeticiones;
    std::vector<int> datos;
    
    string linea;



    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(  rank == 0 ){
        while (getline(archivo, linea)) {
            int num = stoi(linea);  
            datos.insert(num);
        }
        int cant = datos.size();
        
        MPI_Send(&datos/4, cant/4, MPI_INT, 1, 0,MPI_COMM_WORLD);

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
        MPI_Recv(repeticiones, cant/4, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        for(int i = 0; i<cant/4; i++){
            while (getline(archivo, linea)) {
                int num = stoi(linea);  
                if(i == num){
                    repeticiones[i] += 1;
                }
            }
        }

        MPI_Send(&repeticiones, cant/4, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();

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

    return 0;

}