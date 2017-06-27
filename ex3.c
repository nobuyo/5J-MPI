#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char **argv) {
    int nsize;
    int myrank;
    int my_name_len;
    char my_name[MPI_MAX_PROCESSOR_NAME];

    int i;
    int trial_num = 10000;
    int total_trial;
    int inne_cicle_count = 0;
    int total_count;
    int seed;

    double result;
    double range;
    double _x, _y;
    double x, y;

    double time_start;
    double time_end;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    MPI_Get_processor_name(my_name, &my_name_len);

    time_start = MPI_Wtime();

    seed = time(NULL) + (myrank);
    srandom(seed);

    range = 1.0 / (double)nsize;

    for (i = 0; i < trial_num; i++) {
        if (myrank == nsize - 1) {
            _x = ((double)random() / (double)((unsigned)RAND_MAX + 1)) * range;
            _y = (double)random() / (double)((unsigned)RAND_MAX + 1);
        }
        else {
            _x = ((double)random() / (double)RAND_MAX) * range;
            _y = (double)random() / (double)RAND_MAX;
        }

        x = _x + (myrank * range);
        y = _y;

        if (x*x + y*y <= 1.0) {
            inne_cicle_count++;
        }
    }

    MPI_Reduce(&inne_cicle_count, &total_count, 1, MPI_INTEGER, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&trial_num, &total_trial, 1, MPI_INTEGER, MPI_SUM, 0, MPI_COMM_WORLD);

    time_end = MPI_Wtime();

    if (myrank == 0) {
        result = (double)total_count/(double)(total_trial) * 4;
        printf("total trial : %d\ninner circle: %d\n", total_trial, total_count);
        printf("computed approx = %.10f\n", result);
        printf("true pi         = %.10f\n", M_PI);
        printf("error           = %.10f\n", M_PI - result);
        printf("time            = %.10f\n", time_end - time_start);
    }

    MPI_Finalize();

    return 0;
}