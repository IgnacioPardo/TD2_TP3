#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <limits.h>
#include <math.h>
#include <time.h>

void fileWrite(char* file, int64_t value ) {
    FILE* pfile = fopen(file,"w");
    fprintf(pfile,"%lld\n",value);
    fclose(pfile);
}

void fileRead(char* file, int64_t* value ) {
    FILE* pfile = fopen(file,"r");
    fscanf(pfile, "%lld", value);
    fclose(pfile);
}

void monteCarloPi(int semilla, int64_t samples, int64_t* circle, int64_t* square) {
    srand(semilla);
    for (int64_t i = 0; i < samples; i++) {

        // Los puntos que se generan 
        // (x, y) ∈ {(x, y) / x ∈ [0, 1] / y ∈ [0, 1]}
        // En otras palabras todos los puntos pertenecen al cuadrante positivo.
        // Esto significa que el cuadrado es de 1x1, mientras que el circulo por el que evaluamos tiene radio 1.
        // Es decir que solo consideramos puntos de un cuarto del area del circulo, y un cuarto del cuadrado.
        // La relacion a evaluar entonces es 
        // 1/4c / 1/4s = c / s = pi/4
        // Para expandir a un circulo completo se puede expandir el rango en el que se generan las coordenadas de la siguiente forma.
        // (((double)rand()/(double)RAND_MAX) * 2) - 1
        // Sin embargo, al mantenerse la relacion optamos por mantener el rango reducido ya que los resultados son acertados tambien aunque con otra precision.

        double x = ((double)rand()/(double)RAND_MAX);
        double y = ((double)rand()/(double)RAND_MAX);

        // Distancia al (0, 0) del punto (x, y)
        double d = sqrt(x*x + y*y);
        // Incrementa circle en 1 si d <= 1
        (*circle) += d <= 1 ? 1 : 0;
        (*square)++;
    }
}

int main()
{

    int pidRoot = getpid();
    int processCount =  10;
    int64_t n = 100000000;

    for (int i = 0; i<processCount; i++) {

        // Crea nuevo proceso
        pid_t pid = fork();

        if (pid == 0) {
            int mypid = getpid();
            printf("%i -- Procesando...\n", mypid);
            int64_t square = 0;
            int64_t circle = 0;
            monteCarloPi(mypid + time(NULL), n, &circle, &square);
            char nameSquare[3] = {'s','0',0};
            char nameCircle[3] = {'c','0',0};
            nameSquare[1] = nameSquare[1] + i;
            nameCircle[1] = nameCircle[1] + i;
            fileWrite(nameCircle, circle);
            fileWrite(nameSquare, square);
            printf("%i -- Contando %lli circle para %lli square\n", mypid, circle, square);
            exit(0);
        }
    }

    printf("%i -- Terminado de lanzar procesos\n", pidRoot);

    for (int i = 0; i<processCount; i++) {

        // Espera a que termine el proceso hijo
        pid_t child = getpid();
        int status;
        wait(&status);

        printf("%i -- Esperando por %i terminado con status %i\n", pidRoot, child, status);
    }

    printf("%i -- Terminado de esperar procesos\n", pidRoot);    

    int64_t squareTotal = 0;
    int64_t circleTotal = 0;

    for (int i = 0; i<processCount; i++) {

        char nameSquare[3] = {'s','0',0};
        char nameCircle[3] = {'c','0',0};
        nameSquare[1] = nameSquare[1] + i;
        nameCircle[1] = nameCircle[1] + i;
        int64_t square = 0;
        int64_t circle = 0;

        // Lee los valores guardados por cada proceso en los archivos y los suma
        fileRead(nameSquare, &square);
        fileRead(nameCircle, &circle);
        squareTotal += square;
        circleTotal += circle;

    }

    // Dividimos la cantidad de puntos dentro del circulo con N, para hallar una aproximacion de π/4 y la multiplicamos por 4 para obtener la aproximacion de π.
    double pi = ((double)circleTotal/(double)squareTotal) * 4;
    
    printf("%i -- Borrado de archivos temporales\n", pidRoot);   
    char nameSquare[3] = {'s','0',0};
    char nameCircle[3] = {'c','0',0};
    for (int i = 0; i<processCount; i++) {
        remove(nameSquare);
        remove(nameCircle);
        nameSquare[1] = nameSquare[1] + 1;
        nameCircle[1] = nameCircle[1] + 1;
    }

    printf("%i -- PI: %f\n", pidRoot, pi);

    return 0;
}
