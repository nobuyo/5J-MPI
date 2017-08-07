#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int sum(int i, int n) {
    int summation = 0;
    int k;
    for (k=i; k<=n; k++) {
        summation += k;
    }
    return summation;
}

int main(int argc, char **argv) {
    int nsize;
    int myrank;
    int my_name_len;
    char my_name[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    MPI_Get_processor_name(my_name, &my_name_len);

    if (myrank == 0 && (atoi(argv[1]) > 65535 || atoi(argv[1]) <= 0 || argc != 2)) {
        printf("argment invalid.\n");
        MPI_Abort(MPI_COMM_WORLD,1);
    }

    int range_for_each_cpu = atoi(argv[1]) / nsize;
    int min = range_for_each_cpu * myrank + 1;
    int max = range_for_each_cpu * (myrank + 1);
    int result;

    if (atoi(argv[1]) % nsize != 0 && myrank == nsize-1) {
        max = atoi(argv[1]);
    }

    int result_of_each_cpu = sum(min, max);


    MPI_Reduce(&result_of_each_cpu, &result, 1, MPI_INTEGER, MPI_SUM, 0, MPI_COMM_WORLD);


    if (myrank == 0) {
        printf("%d", result);
    }

    MPI_Finalize();

    return 0;
}