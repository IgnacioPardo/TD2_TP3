#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <limits.h>
#include <math.h>
#include <time.h>

void fileWrite(char* file, int64_t value ) {
    // Abre el archivo llamado file en modo escritura
    FILE* pfile = fopen(file,"w");

    // Vuelca el contenido del int64 value al archivo abierto
    fprintf(pfile,"%lld\n",value);

    // Cierra el archivo
    fclose(pfile);
}

void fileRead(char* file, int64_t* value ) {
    // Abre el archivo llamado file en modo lectura
    FILE* pfile = fopen(file,"r");

    // Vuelca el int64 contenido en el archivo en el puntero value
    fscanf(pfile, "%lld", value);

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

            // Genera nombre de los archivos donde guardar los valores obtenidos para circle y para square
            char nameSquare[3] = {'s','0',0};
            char nameCircle[3] = {'c','0',0};
            nameSquare[1] = nameSquare[1] + i;
            nameCircle[1] = nameCircle[1] + i;

            // Escribe los valores obtenidos para circle y para square
            fileWrite(nameCircle, circle);
            fileWrite(nameSquare, square);
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
    int64_t squareTotal = 0;
    int64_t circleTotal = 0;

    for (int i = 0; i<processCount; i++) {

        // Genera nombres de los archivos a leer
        char nameSquare[3] = {'s','0',0};
        char nameCircle[3] = {'c','0',0};
        nameSquare[1] = nameSquare[1] + i;
        nameCircle[1] = nameCircle[1] + i;
        int64_t square = 0;
        int64_t circle = 0;

        // Lee los valores guardados por cada proceso en los archivos y los suma a los totales.
        fileRead(nameSquare, &square);
        fileRead(nameCircle, &circle);
        squareTotal += square;
        circleTotal += circle;

    }

    // Dividimos la cantidad de puntos dentro del circulo con N, para hallar una aproximacion de π/4 y la multiplicamos por 4 para obtener la aproximacion de π.
    double pi = ((double)circleTotal/(double)squareTotal) * 4;
    
    printf("%i -- Borrado de archivos temporales\n", pidRoot);   

    // Genera los nombres de los archivos y los borra.
    char nameSquare[3] = {'s','0',0};
    char nameCircle[3] = {'c','0',0};
    for (int i = 0; i<processCount; i++) {
        remove(nameSquare);
        remove(nameCircle);
        nameSquare[1] = nameSquare[1] + 1;
        nameCircle[1] = nameCircle[1] + 1;
    }
    
    // Muestra por pantalla la aproximacion de pi obtenida.
    printf("%i -- PI: %f\n", pidRoot, pi);

    return 0;
}
