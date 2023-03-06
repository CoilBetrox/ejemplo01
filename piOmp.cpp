#include <math.h>
#include <iostream>
using namespace std;
 
int main(int argc, char *argv[])
{
    int n;
    cout<<"introduce la precision del calculo (n > 0): ";
    cin>>n;
    double PI25DT = 3.141592653589793238462643;
    double h = 1.0 / (double) n;
    double sum = 0.0;
 
 /*
    #pragma omp parallel for shared( sum )
    for (int i = 1; i <= n; i++) {
        double x = h * ((double)i - 0.5);
        sum += (4.0 / (1.0 + x*x));
    }
*/

/*
    #pragma omp parallel for shared( sum )
    for (int i = 1; i <= n; i++) {
        double x = h * ((double)i - 0.5);
        //#pragma omp critical        //conjunto de instrucciones no se ejecute a la vez por varias hebras
        #pragma omp atomic            //mejora la eficiencia

        sum += (4.0 / (1.0 + x*x));
    }
*/

    //mejora eficiencia, la variable compartida se trata por separado y luego se mezcla los resultado de cada hebra
    #pragma omp parallel for reduction(+:sum)
    for (int i = 1; i <= n; i++) {
        double x = h * ((double)i - 0.5);
        sum += (4.0 / (1.0 + x*x));
    }


 
    double pi = sum * h;
    cout << "El valor aproximado de PI es: " << pi << ", con un error de " << fabs(pi - PI25DT) << endl;
    return 0;
}