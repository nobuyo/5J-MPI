#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define FIELD_SIZE 100;

int main(int argc, char **argv) {
    int nsize;
    int myrank;
    int my_name_len;
    char my_name[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    MPI_Get_processor_name(my_name, &my_name_len);

    int i, j;
    int field[FIELD_SIZE][FIELD_SIZE];

    srandom(time(NULL + myrank))

    for (i = 0; i < FIELD_SIZE; i++) {
        for (j = 0; j <FIELD_SIZE; j++) {
            field[i][j] = random() % 2;
        }
    }

    MPI_Finalize();

    return 0;
}