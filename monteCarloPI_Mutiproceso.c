#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <limits.h>
#include <time.h>
#include <math.h>
#include <inttypes.h>

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
    // COMPLETAR

    for (int64_t i = 0; i < samples; i++) {
 
        double x = ((double)rand()/(double)RAND_MAX);
        double y = ((double)rand()/(double)RAND_MAX);

        double d = sqrt(x*x + y*y);
        if (d <= 1)
            circle++;
        else
            square++;
    }
}

int main()
{
    int pidRoot = getpid();
    int processCount =  10;
    int64_t n = 100000000;

    for (int i = 0; i<processCount; i++) {
        // COMPLETAR

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
        // COMPLETAR
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
        // COMPLETAR
    }

    double pi = // COMPLETAR
    
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
