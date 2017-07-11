#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define FIELD_SIZE 100

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
    int n, m;
    int count;
    int era = 0;
    int field[FIELD_SIZE][FIELD_SIZE];

    if (myrank == 0) {

        srandom(time(NULL + myrank));

        for (i = 0; i < FIELD_SIZE+2; i++) {
            for (j = 0; j <FIELD_SIZE+2; j++) {
                field[i][j] = 0;
            }
        }

        for (i = 1; i <= FIELD_SIZE; i++) {
            for (j = 1; j <= FIELD_SIZE; j++) {
                if (random() % 100 < 8) {
                    field[i][j] = 1;
                }
                else {
                    field[i][j] = 0;
                }
            }
        }

        while (era < 100) {
            for (i = 1; i <= FIELD_SIZE; i++) {
                for (j = 1; j <= FIELD_SIZE; j++) {

                    count = field[i-1][j-1] + field[i-1][j] + field[i-1][j+1]
                          + field[i][j-1]   +               + field[i][j+1]
                          + field[i+1][j-1] + field[i+1][j] + field[i+1][j+1];
                    if (count == 3) {
                        field[i][j] = 1;
                    }
                    else if (count == 2) {
                        continue;
                    }
                    else {
                        field[i][j] = 0;
                    }
                }
            }

            printf("\033[;H\033[2J");
            for (i = 1; i <= FIELD_SIZE; i++) {
                for (j = 1; j <= FIELD_SIZE; j++) {
                    if (field[i][j] == 1) {
                        printf("@");
                    }
                    else {
                        printf(" ");
                    }
                }
                printf("\n");
            }
            sleep(1);
            era++;
        }
    }

    MPI_Finalize();

    return 0;
}