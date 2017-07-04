#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv) {
    int nsize;
    int myrank;
    int my_name_len;
    char my_name[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    MPI_Get_processor_name(my_name, &my_name_len);

    int i,j;
    int r;
    int b[6] = {0};
    MPI_Status status;

    if (myrank == 0) {
        int a[6] = {3, 1, 4, 1, 5, 9};

        for (i=1; i<nsize; i++) {
            MPI_Send(a, 6, MPI_INTEGER, i, 100+i, MPI_COMM_WORLD);
        }

        for (i=1; i<nsize; i++) {
            MPI_Recv(b, 6, MPI_INTEGER, i, 200+i, MPI_COMM_WORLD, &status);
            MPI_Recv(&r, 1, MPI_INTEGER, i, 300+i, MPI_COMM_WORLD, &status);
            printf("myrank = %d, r = %d\n", i, r);
            for (j=0; j<6; j++) {
                printf("%d, ", b[j]);
            }
            putchar('\n');
        }

    }
    else {
        srandom(time(NULL) + myrank * 10);
        r = random() % 100;

        MPI_Recv(b, 6, MPI_INTEGER, 0, 100+myrank, MPI_COMM_WORLD, &status);
        for (i=0; i<6; i++) {
            b[i] += r;
        }

        MPI_Send(b, 6, MPI_INTEGER, 0, 200+myrank, MPI_COMM_WORLD);
        MPI_Send(&r, 1, MPI_INTEGER, 0, 300+myrank, MPI_COMM_WORLD);
    }


    MPI_Finalize();

    return 0;
}