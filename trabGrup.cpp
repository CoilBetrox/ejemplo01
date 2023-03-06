#include <iostream>
#include <cmath>
#include <omp.h>
#include <mpi.h>  
#include <ctime>
using namespace std;
double f(double x) {
    return 4/(1+pow(x, 2)); // Por ejemplo, la función cos(x)
}
//Limites de la función y número de iteraciones.  
double trapecioSerial( double a, double b, double n){
    float h = (b-a)/n; 
    double integral = (f(a) + f(b))/2;
    for(int i = a; i < n; i++){
        integral += f(a + i * h);
    }
    return h * integral; 
}
/*double trapecioOMP(double a, double b, double n){
    int numero_hilos = 0; 
    float h = (b-a)/n; 
    #pragma omp parallel
    {   
        #pragma opm master//le pone en hilo cero
            numero_hilos= omp_get_num_threads();
            
    };
    double sumas_parciales[numero_hilos]; 
    #pragma omp parallel shared (sumas_parciales)
    {
        int thread_id= omp_get_thread_num();
        for(long i=thread_id;i<=n;i+=numero_hilos){
            sumas_parciales[thread_id]+= f(a + i * h);
        }
    }
    double respuesta=0;
    //suma de todos los hilos
    for(int i=0;i<=numero_hilos;i++){
       respuesta= respuesta+sumas_parciales[i];
    }
    return h * respuesta;
}*/

int main(int argc, char ** argv) {
    //LLamada al metodo serial 
    //double resultado = trapecioSerial(0,1,10000000);
    //std::printf("El resultado de forma serial es: %lf", resultado); 
    //Llamada al metodo utilizando OMP 
    /*double resultadoOMP = trapecioOMP(0,1,10000000);
    std::printf("El resultado de MPI es: %lf \n", resultadoOMP ); */

    //Utilizando MPI
    MPI_Init(&argc,&argv); 
    int rank; 
    int size; 
    int n = 10000000; 
    int* data = new int[n]; 
    double a = 0; 
    double b = 1; 
    double h = (b-a)/n; 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    MPI_Comm_size(MPI_COMM_WORLD,&size); 

    //El rank 0 se encarga de distribuir los datos 
    if(rank == 0 ){
        double start_time = MPI_Wtime(); 
        std::printf("Hola, soy el rank %d, de un total de %d procesos \n", rank, size);
        //Se llena el array de datos 
        for(int i = 0; i<= n; i++){
            data[i]=i; 
        }
        
        //Se envian los datos 
        for(int i=1; i<size; i++){
            std::printf("El valor que se va a enviar sera desde: %d hasta %d al rank %d \n", i*(n/size), ((i+1)* (n/size)-1), i); 
            MPI_Send(&data[i*(n/size)],n/size -1,MPI_INT,i,0,MPI_COMM_WORLD);
        }
        double respuesta = (f(a) + f(b))/2;
        for (int i = a; i < n/size ; i++ ){
            respuesta += f(a + i * h);    
        } 
        double sumaParcial[size-1]; 
        for(int i = 0 ; i<size -1; i++) {
           MPI_Recv(&sumaParcial[i],1 ,MPI_DOUBLE,MPI_ANY_SOURCE,0,MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
        } 
        for(int i = 0 ; i<size -1; i++){
            respuesta+=sumaParcial[i]; 
        }
        double resultado = h * respuesta; 
        std::printf("El resultado final es: %lf \n" , resultado );    
        double main_time = MPI_Wtime()-start_time;
        std::printf("El tiempo de trabajo es: %lf ", main_time); 
    }else{
        MPI_Recv(data, n/size -1,MPI_INT,MPI_ANY_SOURCE,0,MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
        std::printf("Hola, soy el rank %d, de un total de %d procesos \n", rank, size);
        double sumaParcial = 0;
        for (int i = 0; i < n/size ; i++ ){
            sumaParcial += f(a + data[i] * h);    
        }             
        MPI_Send(&sumaParcial,1,MPI_DOUBLE,0,0,MPI_COMM_WORLD);
    }

    MPI_Finalize(); 
    return 0;
}