#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <limits.h>
#include <math.h>
#include <time.h>


// Escribe dos valores linea por linea en un mismo archivo.
void fileWritePair(char* file, int64_t v1, int64_t v2) {
    FILE* pfile = fopen(file,"w");
    fprintf(pfile, "%lld\n%lld", v1, v2);
    fclose(pfile);
}

// Lee dos valores linea por linea de un mismo archivo.
void fileReadPair(char* file, int64_t* v1, int64_t* v2) {
    FILE* pfile = fopen(file,"r");
    fscanf(pfile, "%lld\n%lld", v1, v2);
    fclose(pfile);
}


void monteCarloPi(int semilla, int64_t samples, int64_t* circle, int64_t* square) {
    srand(semilla);
    for (int64_t i = 0; i < samples; i++) {
 
        double x = ((double)rand()/(double)RAND_MAX);
        double y = ((double)rand()/(double)RAND_MAX);

        // Distancia al (0, 0) del punto (x, y)
        double d = sqrt(x*x + y*y);
        // Incrementa circle en 1 si d <= 1
        (*circle) += d <= 1 ? 1 : 0;
        (*square)++;
    }
}

int digits(int n){
    int c = 1;
    double n_ = (double)n;
    while (n_ >= 10){
        n_ = n_/10;
        c++;
    }
    return c;
}

int main()
{
    int pidRoot = getpid();
    int processCount = 50;
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


            // Nombramos a los archivos por su indice de subproceso
            char file_index[digits(i)];
            // Vuelca el entero i en el string file_index para usarlo como nombre de archivo.
            sprintf(file_index, "%d", i);
            fileWritePair(file_index, circle, square);

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

    int64_t circleTotal = 0;
    int64_t squareTotal = 0;

    for (int i = 0; i<processCount; i++) {

        int64_t circle = 0;
        int64_t square = 0;
        char file_index[digits(i)];
        sprintf(file_index, "%d", i);
        fileReadPair(file_index, &circle, &square);
        circleTotal += circle;
        squareTotal += square;

    }
    
    // Dividimos la cantidad de puntos dentro del circulo con N, para hallar una aproximacion de π/4 y la multiplicamos por 4 para obtener la aproximacion de π.
    double pi = ((double)circleTotal/((double)n*processCount)) * 4;
    
    printf("%i -- Borrado de archivos temporales\n", pidRoot);   
    
    for (int i = 0; i<processCount; i++) {
        char file_index[digits(i)];
        sprintf(file_index, "%d", i);
        remove(file_index);
    }

    printf("%i -- PI: %f\n", pidRoot, pi);

    return 0;
}
