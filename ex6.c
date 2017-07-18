#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define FIELD_SIZE 50
// #define HEIGHT 40
// #define WIDTH 40
#define MAX_GEN 100

void display(int field[FIELD_SIZE][FIELD_SIZE]) {
    int i,j;
    printf("\033[;H\033[2J");
    for (i = 0; i < FIELD_SIZE; i++) {
        for (j = 0; j < FIELD_SIZE; j++) {
            if (field[i][j] == 1) {
                printf("@");
            }
            else {
                printf(" ");
            }
        }
        printf("\n");
    }
}

void init(int field[FIELD_SIZE][FIELD_SIZE]) {
    int i,j;
    for (i = 0; i < FIELD_SIZE; i++) {
        for (j = 0; j < FIELD_SIZE; j++) {
            if (random() % 100 < 9) {
                field[i][j] = 1;
            }
            else {
                field[i][j] = 0;
            }
        }
    }
}

void generate_new_era(int field[FIELD_SIZE][FIELD_SIZE],
        int next_gen[FIELD_SIZE][FIELD_SIZE], int begin, int end) {
    int i, j;
    int k, l, n, m;
    int count;

    for (i = 0; i < FIELD_SIZE; i++) {
        for (j = begin+1; j < end-1; j++) {
            k = (FIELD_SIZE + i - 1) % FIELD_SIZE;
            l = (FIELD_SIZE + j - 1) % FIELD_SIZE;
            m = (i + 1) % FIELD_SIZE;
            n = (j + 1) % FIELD_SIZE;

            count = field[k][l]   +  field[k][j]   +  field[k][n]
                  + field[i][l]   /*field[i][j]*/  +  field[i][n]
                  + field[m][l]   + field[m][j]    +  field[m][n];

            if (count == 3) {
                next_gen[i][j] = 1;
            }
            else if (count == 2) {
                continue;
            }
            else {
                next_gen[i][j] = 0;
            }
        }
    }
}

void merge(int field[FIELD_SIZE][FIELD_SIZE],
        int next_gen[FIELD_SIZE][FIELD_SIZE], int begin, int end) {
}

void send() {
    // todo
}

void recv() {
    // todo
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

    int i, j;
    int gen = 0;
    int field[FIELD_SIZE][FIELD_SIZE];
    int next_gen[FIELD_SIZE][FIELD_SIZE];

    if (FIELD_SIZE % nsize != 0) {
        printf("cpu num must be even number.\n");
        MPI_Abort(MPI_COMM_WORLD,1);
    }

    int range_for_each_cpu = FIELD_SIZE / nsize;
    int begin = range_for_each_cpu * myrank - 1;
    int end = nsize == 1 ? FIELD_SIZE + 1
              : range_for_each_cpu * (myrank + 1) + 1;

    if (myrank == 0) {
        srandom(time(NULL + myrank));
        init(field);
    }

    while (gen < MAX_GEN) {
        generate_new_era(field, next_gen, begin, end);
        // display(field);

        usleep(40000);
        gen++;
        for (i = 0; i < FIELD_SIZE; i++) {
            for (j = 0; j <FIELD_SIZE; j++) {
                field[i][j] = next_gen[i][j];
            }
        }
    }

    MPI_Finalize();

    return 0;
}