#include <omp.h>
#include <iostream>
 
#define N 10
#define nthreads 4
 
int main (){
  int thread;
  omp_set_num_threads(nthreads);
  #pragma omp parallel private(thread)
  {
      thread = omp_get_thread_num();
 
      //#pragma omp for     //ndicar que las iteraciones de un bucle for han de ser repartidas entre las distintas hebras
      //#pragma omp for schedule( static, 4 )     // static recibe por parámetro el numero de iteraciones que reparte a cada hebra, reparte asta que se queda sin iteraciones
      //#pragma omp for schedule( dynamic, 4 )    // dynamic recibe por parámetro el numero de iteraciones por bloque. Cuando una hebra se queda sin trabajo pide un nuevo bloque, así hasta que no hay mas iteraciones
      #pragma omp for schedule( auto )      //el sistema se encarga
      for (int i = 0 ; i < N; i++){
      std::cout<<"Soy el proceso "<<thread<<" ejecuto al iteracion "<<i<<std::endl;
      }
 
  }

}