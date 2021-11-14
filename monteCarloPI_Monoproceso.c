#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <limits.h>
#include <math.h>
#include <time.h>
 
double monteCarloPi(int64_t samples) {
    //Seed para el rand
    srand(time(NULL));

    // Declaro dos variables para contabilizar los puntos que cayeron al circulo y los que cayeron al cuadrado
    int64_t circle = 0, square = 0;

    for (int64_t i = 0; i < samples; i++) {
 
        // Los puntos (x, y) que se generan cumplen con:
        // (x, y) ∈ {(x, y) / x ∈ [0, 1] / y ∈ [0, 1]}
        // En otras palabras todos los puntos pertenecen al cuadrante positivo.
        // Esto significa que el cuadrado es de 1x1, mientras que el circulo por el que evaluamos tiene radio 1.
        // Es decir que solo consideramos puntos de un cuarto del area del circulo, y un cuarto del cuadrado.
        // La relacion a evaluar entonces es 
        // 1/4c / 1/4s = c / s = pi/4
        // Para expandir a un circulo completo se puede expandir el rango en el que se generan las coordenadas de la siguiente forma.
        // (((double)rand()/(double)RAND_MAX) * 2) - 1
        // Sin embargo, al mantenerse la relacion optamos por mantener el rango reducido ya que los resultados son acertados tambien aunque con otra precision.

        //Genero dos valores random para formar el punto (x, y)
        double x = ((double)rand()/(double)RAND_MAX);
        double y = ((double)rand()/(double)RAND_MAX);

        // Distancia al (0, 0) del punto (x, y)
        double d = sqrt(x*x + y*y);
        // Incrementa circle en 1 si d <= 1
        circle += d <= 1 ? 1 : 0;
        // Incrementa square si el punto pertenece a el.
        square += (x <= 1 && x >= -1 && y <= 1 && y >= -1) ? 1 : 0;
    }
    
    // Dividimos la cantidad de puntos dentro del circulo con N, para hallar una aproximacion de π/4 y la multiplicamos por 4 para obtener la aproximacion de π.
    double pi = ((double)circle/(double)square) * 4;
    return pi;
}

int main() {
    // Cantidad de puntos a generar
    int64_t n = 100000000;
    // Aproxima pi a partir de la relacion de los puntos n generados.
    double pi = monteCarloPi(n);

    // Muestra por pantalla la aproximacion de pi obtenida.
    printf("%f\n", pi);

    return 0;
}
