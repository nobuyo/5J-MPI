#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char **argv) {
    int nsize;
    int myrank;
    int my_name_len;
    char my_name[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    MPI_Get_processor_name(my_name, &my_name_len);

    int i;
    int trial_num = 1000;
    int count = 0;
    int total;

    // set random seed
    int seed = time(NULL) + (myrank);
    srandom(seed);
    // printf("[rank%d] = %d\n", myrank, seed);

    double _x, _y; // random point
    double x, y;
    // int r = 1;

    for (i = 0; i < trial_num; i++) {
        _x = ((double)random() / (double)RAND_MAX)/4.0;
        _y = (double)random() / (double)RAND_MAX;

        x = _x + 0.25 * myrank;
        y = _y;

        if (x*x + y*y <= 1.0) {
            count++;
        }
    }

    MPI_Reduce(&count, &total, 1, MPI_INTEGER, MPI_SUM, 0, MPI_COMM_WORLD);

    // printf("%f, %f\n", _x, _y);

    if (myrank == 0) {
        printf("trial: %d\ninner: %d\n", trial_num*nsize, total);
        printf("computed approx = %.10f\n", (double)total/(double)trial_num);
        printf("true pi         = %.10f\n", M_PI);
        printf("error           = %.10f\n", M_PI - total/(double)trial_num);
    }

    MPI_Finalize();

    return 0;
}