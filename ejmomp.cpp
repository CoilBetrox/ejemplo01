#include <iostream>
#include <omp.h>
#include <chrono>

namespace ch=std::chrono;

#define NUMERO_ITERACIONES 1000000000

double pi_serial() {
    double respuesta = 0.0;
    bool esIndicePar = true;
    for(long indice = 0; indice <= NUMERO_ITERACIONES; indice++){
        if(esIndicePar == true){
            respuesta += 4.0 / (2.0 * indice + 1.0);
        }
        else{
            respuesta -= 4.0 / (2.0 * indice + 1.0);
        }
        esIndicePar = !esIndicePar;
    }
    return respuesta;
}

double pi_omp1() {

    //omp_set_num_threads(16);

    int numero_hilos = 0;

#pragma omp parallel
    {
        #pragma omp master
        numero_hilos = omp_get_num_threads();
    }

    std::printf("Thread count: %d\n", numero_hilos );

    double sumas_parciales[numero_hilos];

#pragma omp parallel shared(sumas_parciales)
    {
        int thread_id = omp_get_thread_num(); // id del hilo

        for (long indice = thread_id; indice <= NUMERO_ITERACIONES; indice+=numero_hilos) {
            if (indice%2==0) {
                sumas_parciales[thread_id] += 4.0 / (2.0 * indice + 1.0);
            } else {
                sumas_parciales[thread_id] -= 4.0 / (2.0 * indice + 1.0);
            }
        }
    }

    double respuesta = 0;
    for(int i=0;i<numero_hilos;i++) {
        respuesta = respuesta + sumas_parciales[i];
    }

    return respuesta;
}

int main() {

    double pi1;
    auto start = ch::high_resolution_clock::now();
    {
        pi1 = pi_omp1();
        //pi1 = pi_serial();
    }
    auto end = ch::high_resolution_clock ::now();
    ch::duration<double,std::milli> duracion = end-start;

    std::printf("Tiempo serial: %lf, pi=%.10lf\n", duracion.count(), pi1 );

    return 0;
}