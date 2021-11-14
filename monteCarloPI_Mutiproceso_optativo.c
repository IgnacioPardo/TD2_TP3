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
    // Abre el archivo llamado file en modo escritura
    FILE* pfile = fopen(file,"w");
    // Escribe los valores de v1 y v2 en el archivo.
    fprintf(pfile, "%lld\n%lld", v1, v2);
    // Cierra el archivo
    fclose(pfile);
}

// Lee dos valores linea por linea de un mismo archivo.
void fileReadPair(char* file, int64_t* v1, int64_t* v2) {
    // Abre el archivo llamado file en modo lectura
    FILE* pfile = fopen(file,"r");
    // Lee del archivo los valores v1 y v2.
    fscanf(pfile, "%lld\n%lld", v1, v2);
    // Cierra el archivo
    fclose(pfile);
}


void monteCarloPi(int semilla, int64_t samples, int64_t* circle, int64_t* square) {
    // Seed para el rand
    srand(semilla);

    // Genera una cantidad samples de puntos al azar.
    for (int64_t i = 0; i < samples; i++) {

        //Genero dos valores random para formar el punto (x, y)
        double x = ((double)rand()/(double)RAND_MAX);
        double y = ((double)rand()/(double)RAND_MAX);

        // Distancia al (0, 0) del punto (x, y)
        double d = sqrt(x*x + y*y);
        // Incrementa el valor al que apunta el puntero circle en 1 si d <= 1
        (*circle) += d <= 1 ? 1 : 0;
        // Incrementa el valor al que apunta el puntero square si el punto pertenece a el.
        (*square) += (x <= 1 && x >= -1 && y <= 1 && y >= -1) ? 1 : 0;
    }
}

// Retorna la cantidad digitos de n.
int digits(int n){
    int c = 1;
    // Copia n casteado a double en otra variable.
    double n_ = (double)n;

    // Cuenta 1 por cada division de n_ por 10 hasta que n_ es de un solo digito.
    while (n_ >= 10){
        n_ = n_/10;
        c++;
    }
    return c;
}

int main()
{

    // pid inicial
    int pidRoot = getpid();

    // Procesos a crear
    int processCount = 10;

    // Puntos a generar por cada proceso
    int64_t n = 100000000;

    for (int i = 0; i<processCount; i++) {

        // Crea nuevo proceso
        pid_t pid = fork();

        if (pid == 0) {

            // De ser un proceso child...
            int mypid = getpid();
            printf("%i -- Procesando...\n", mypid);
            int64_t square = 0;
            int64_t circle = 0;
            
            // Genera puntos al azar y los contabiliza si pertenecen al circle y o al square.
            monteCarloPi(mypid + time(NULL), n, &circle, &square);

            // Nombramos a los archivos por el indice de cada proceso
            char file_index[digits(i)];
            // Vuelca el entero i en el string file_index para usarlo como nombre de archivo.
            sprintf(file_index, "%d", i);
            // Escribe los valores de cirlce y square en el archivo llamado file_index.
            fileWritePair(file_index, circle, square);

            printf("%i -- Contando %lli circle para %lli square\n", mypid, circle, square);
            
            // Termina el proceso con status 0.
            exit(0);
        }
    }

    printf("%i -- Terminado de lanzar procesos\n", pidRoot);

    for (int i = 0; i<processCount; i++) {

        // Espera a que termine el proceso hijo
        int status;
        pid_t child = wait(&status);

        printf("%i -- Esperando por %i terminado con status %i\n", pidRoot, child, status);
    }

    printf("%i -- Terminado de esperar procesos\n", pidRoot);    

    // Define variables donde contabilizar el total de puntos caidos en el circle y/o en el square
    int64_t circleTotal = 0;
    int64_t squareTotal = 0;

    for (int i = 0; i<processCount; i++) {

        int64_t circle = 0;
        int64_t square = 0;
        // Nombres de archivos
        char file_index[digits(i)];
        sprintf(file_index, "%d", i);
        // Lee el archivo donde el proceso guardo los valores generados para circle y square.
        fileReadPair(file_index, &circle, &square);

        // Los suma al total de puntos pertenecientes al circle y/o al square.
        circleTotal += circle;
        squareTotal += square;

    }
    
    // Dividimos la cantidad de puntos dentro del circulo con N, para hallar una aproximacion de π/4 y la multiplicamos por 4 para obtener la aproximacion de π.
    double pi = ((double)circleTotal/((double)squareTotal)) * 4;
    
    printf("%i -- Borrado de archivos temporales\n", pidRoot);   
   
    // Genera los nombres de los archivos y los borra.    
    for (int i = 0; i<processCount; i++) {
        char file_index[digits(i)];
        sprintf(file_index, "%d", i);
        remove(file_index);
    }
    
    // Muestra por pantalla la aproximacion de pi obtenida.
    printf("%i -- PI: %f\n", pidRoot, pi);

    return 0;
}
