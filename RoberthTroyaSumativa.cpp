//Roberth Troya 
#include <iostream>
#include <mpi.h>
#include <omp.h>
#include <fstream>
#include <vector>
#include <ctime>

using namespace std;
#define nthreads 4

string nombreArchivo = "datos.txt";
ifstream archivo(nombreArchivo.c_str());
string linea;
int n = calculaTamanio(nombreArchivo);


//Serial
std::vector<int> frec_serial( string archiv ) {
    ifstream archivo(archiv.c_str());
    std::vector<int> repeticiones;
    for(int i=0; i<=100; i++){
        repeticiones[i]=0;
    }
    string linea;
    double start_time = MPI_Wtime();
    while (getline(archivo, linea)) {
        
        int num = stoi(linea);    
        for ( int i = 0; i <= 100; i++ ){
            if(num == i){
                repeticiones[i] += 1;
            }
        }
    }
    double main_time = MPI_Wtime()-start_time;
    std::printf("El tiempo de trabajo es: %lf ", main_time);
    return repeticiones;
}


//genera formato de impresion
void format_serial(std::vector<int> frec_serial){
    int n = 17000000; 
    for( int i = 0; i <= 100; i++ ){
        std::printf("%d %d %d\n",i , frec_serial[i], frec_serial[i]/n *100 );
    }
}

int calculaTamanio(string archiv){
    int n = 0;
    ifstream archivo(archiv.c_str()); 
    string linea;
    while (getline(archivo, linea)) {
        int num = stoi(linea);    
        n += 1;
    }
    return n;
}


//OMP
std::vector<int> frec_omp( string archiv ) {
    ifstream archivo(archiv.c_str());
    std::vector<int> repeticiones;
    for(int i=0; i<=100; i++){
        repeticiones[i]=0;
    }
    string linea;

    int thread;
    omp_set_num_threads(nthreads);
    double start_time = MPI_Wtime();
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
    double main_time = MPI_Wtime()-start_time;
    std::printf("El tiempo de trabajo es: %lf ", main_time);
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
    //int n=0;

    string nombreArchivo = "datos.txt";
    ifstream archivo(nombreArchivo.c_str());
    std::vector<int> repeticiones;
    std::vector<int> datos;
    
    string linea;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if( rank == 0 ){
        while (getline(archivo, linea)) {
            int num = stoi(linea);  
            datos.push_back(num);
            n += 1 ;
        }
        int cant = datos.size(); 
        double start_time = MPI_Wtime();
        //envío de datos
        for(int i=1; i<size; i++){
            std::printf("El valor que se va a enviar sera desde: %d hasta %d al rank %d \n", i*(n/size), ((i+1)* (n/size)-1), i); 
            MPI_Send(&datos[i*(n/size)],n/size -1,MPI_INT,i,0,MPI_COMM_WORLD);
        }

        std::vector<int> vectorParcial;

        for (int i = 0; i<=100; i++){
            vectorParcial[i] = 0;
        }
        for(int i = 0; i < n/size; i++){
            while (getline(archivo, linea)) {
                int num = stoi(linea);  
                if(i == num){
                    vectorParcial[i] += 1;
                }
            }
        }

        std::vector<int> vectorParcial1;
        for(int i = 0 ; i<size -1; i++) {
           MPI_Recv(&vectorParcial1[i],1 ,MPI_DOUBLE,MPI_ANY_SOURCE,0,MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
        } 
        for(int i = 0 ; i<size -1; i++){
            for( int i = 0; i<=100; i++){
                vectorParcial[i] += vectorParcial1[i]; 
            }
        }

        format_serial(vectorParcial);
        double main_time = MPI_Wtime()-start_time;
        std::printf("El tiempo de trabajo es: %lf ", main_time);
    }
    else {
        printf("Rank %d recibiendo...\n",rank);
        MPI_Recv(datos.data(), n/size -1,MPI_INT,MPI_ANY_SOURCE,0,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::vector<int> vectorParcial1;
        for (int i = 0; i<=100; i++){
            vectorParcial1[i] = 0;
        }

        for(int i = 0; i < n/size; i++){
            while (getline(archivo, linea)) {
                int num = stoi(linea);  
                if(i == num){
                    vectorParcial1[i] += 1;
                }
            }
        }
        MPI_Send(vectorParcial1.data(), n/size -1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    MPI_Finalize();

    return 0;

}