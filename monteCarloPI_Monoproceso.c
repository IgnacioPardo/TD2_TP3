#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <limits.h>
#include <time.h>
#include <math.h>
#include <inttypes.h>

double monteCarloPi(int64_t samples) {
    srand(time(NULL));

    int64_t circle = 0, square = 0;

    for (int64_t i = 0; i < samples; i++) {
 
        double x = ((double)rand()/(double)RAND_MAX);
        double y = ((double)rand()/(double)RAND_MAX);

		// Distancia al (0, 0) del punto (x, y)
        double d = sqrt(x*x + y*y);
				
		// De ser su distancia al (0, 0) <= 1 determinamos el punto (x, y) esta contenido en el circulo
        if (d <= 1)
            circle++;
        else
            square++;
        //circle += d <= 1 ? 1 : 0;
        //square += d > 1 ? 1 : 0;
    }
    //printf("C: %"PRId64", S: %"PRId64, circle, square);
    //printf(", T: %"PRId64, samples);
    //printf(", Sum: %"PRId64"\n", circle + square);
    
	// Dividimos la cantidad de puntos dentro del circulo con N, para hallar una aproximacion de π/4 y la multiplicamos por 4 para obtener la aproximacion de π.
	double pi = ((double)circle/(double)samples) * 4;// COMPLETAR
    return pi;
}

int main() {
    int64_t n = 100000000;
    double pi = monteCarloPi(n);

    printf("%f\n",pi);

    return 0;
}
