#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <limits.h>
#include <math.h>
#include <time.h>
 
double monteCarloPi(int64_t samples) {
    srand(time(NULL));

    int64_t circle = 0, square = 0;

    for (int64_t i = 0; i < samples; i++) {
 
        double x = ((double)rand()/(double)RAND_MAX);
        double y = ((double)rand()/(double)RAND_MAX);

        // Distancia al (0, 0) del punto (x, y)
        double d = sqrt(x*x + y*y);
        // Incrementa circle en 1 si d <= 1
        circle += d <= 1 ? 1 : 0;
        square++;
    }
    
    // Dividimos la cantidad de puntos dentro del circulo con N, para hallar una aproximacion de π/4 y la multiplicamos por 4 para obtener la aproximacion de π.
    double pi = ((double)circle/(double)square) * 4;
    return pi;
}

int main() {
    int64_t n = 100000000;
    double pi = monteCarloPi(n);

    printf("%f\n",pi);

    return 0;
}
