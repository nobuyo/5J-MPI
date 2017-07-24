#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define HEIGHT 100
#define WIDTH 100
#define MAX_GEN 100

int field[HEIGHT][WIDTH];
int next_gen[HEIGHT][WIDTH];

int nsize;
int myrank;
int my_name_len;
char my_name[MPI_MAX_PROCESSOR_NAME];

void display() {
    int i,j;

    if (myrank != 0) return;

    printf("\033[;H\033[2J");
    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            if (field[i][j] == 1) {
                printf("@");
            }
            else {
                printf("-");
            }
        }
        printf("\n");
    }
}

void init() {
    int i,j;
    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            field[i][j] = 0;
            if (random() % 100 < 9) {
                field[i][j] = 1;
            }
            else {
                field[i][j] = 0;
            }
        }
    }

    // field[3][3] = 1;
    // field[3][4] = 1;
    // field[3][5] = 1;
    // field[4][3] = 1;
    // field[5][4] = 1;
}

void copy(int begin, int end) {
    int i, j;
    for (i = begin; i < end; i++) {
        for (j = 0; j < WIDTH; j++) {
            field[i][j] = next_gen[i][j];
        }
    }
}

void build_new_gen(int begin, int end) {
    int i, j;
    int k, l, n, m;
    int count;

    for (i = begin; i < end; i++) {
        for (j = 0; j < WIDTH; j++) {
            k = (HEIGHT + i - 1) % HEIGHT;
            l = (WIDTH + j - 1) % WIDTH;
            m = (i + 1) % HEIGHT;
            n = (j + 1) % WIDTH;

            count = field[k][l]   +  field[k][j]   +  field[k][n]
                  + field[i][l]   /*field[i][j]*/  +  field[i][n]
                  + field[m][l]   +  field[m][j]   +  field[m][n];

            if (count == 3) {
                next_gen[i][j] = 1;
            }
            else if (count == 2) {
                next_gen[i][j] = field[i][j];
            }
            else {
                next_gen[i][j] = 0;
            }
        }
    }
}


void mpi_send(int line[HEIGHT], int dest, int tag) {
    MPI_Send(line, WIDTH, MPI_INTEGER, dest, tag, MPI_COMM_WORLD);
}

void mpi_receive(int line[HEIGHT], int source, int tag) {
    MPI_Status status;
    MPI_Recv(line, WIDTH, MPI_INTEGER, source, tag, MPI_COMM_WORLD, &status);
}

void destribute() {
    int i,j;
    int range = HEIGHT / nsize;
    if (myrank == 0) {
        for (i = 1; i < nsize; i++) {
            for (j = range * i - 1; j <= range * (i+1); j++) {
                mpi_send(field[(j+HEIGHT) % HEIGHT], i, 38);
            }
        }
        return;
    }

    for (j = range * myrank - 1; j <= range * (myrank+1); j++) {
        mpi_receive(field[(j+HEIGHT) % HEIGHT], 0, 38);
    }
}

void merge() {
    int i,j;
    int range = HEIGHT / nsize;
    if (myrank == 0) {
        for (i = 1; i < nsize; i++) {
            for (j = range * i; j < range * (i+1); j++) {
                mpi_receive(field[j], i, 100);
            }
        }

        return;
    }

    for (j = range * myrank; j < range * (myrank+1); j++) {
        mpi_send(next_gen[j], 0, 100);
    }
}

void comm_with_neighbor(int h, int is_odd, int direction) {
    int cpu;
    if (direction == 0) {
        cpu = (myrank - 1 + nsize) % nsize;
        if (is_odd) {
            mpi_receive(field[h], cpu, 99);
            h = (h + 1) % HEIGHT;
            mpi_send(next_gen[h], cpu, 99);
        }
        else {
            mpi_send(next_gen[h], cpu, 99);
            h = (h - 1 + HEIGHT) % HEIGHT;
            mpi_receive(field[h], cpu, 99);
        }
    }
    else {
        cpu = (myrank + 1) % nsize;
        if (is_odd) {
            mpi_receive(field[h], cpu, 99);
            h = (h - 1 + HEIGHT) % HEIGHT;
            mpi_send(next_gen[h], cpu, 99);
        } 
        else {
            mpi_send(next_gen[h], cpu, 99);
            h = (h + 1) % HEIGHT;
            mpi_receive(field[h], cpu, 99);
        }
    }
}

void comm() {
    if (myrank % 2 == 1) {
        comm_with_neighbor((HEIGHT / nsize * myrank - 1 + HEIGHT) % HEIGHT, 1, 0);
        comm_with_neighbor((HEIGHT / nsize * (myrank + 1)) % HEIGHT, 1, 1);
    }
    else {
        comm_with_neighbor((HEIGHT / nsize * (myrank + 1) - 1) % HEIGHT, 0, 1);
        comm_with_neighbor(HEIGHT / nsize * myrank, 0, 0);
    }
}

int main(int argc, char **argv) {
    int i, j;
    int gen = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    MPI_Get_processor_name(my_name, &my_name_len);

    double time_before;
    double time_after;


    if (HEIGHT % nsize != 0) {
        printf("cpu num must be even number.\n");
        MPI_Abort(MPI_COMM_WORLD,1);
    }

    if (myrank == 0) {
        time_before = MPI_Wtime();
    }

    int range_for_each_cpu = HEIGHT / nsize;
    int begin = range_for_each_cpu * myrank;
    int end = range_for_each_cpu * (myrank + 1);

    if (myrank == 0) {
        srandom(time(NULL + myrank));
        init();
    }

    if (nsize == 1) {
        while (gen < MAX_GEN) {
            build_new_gen(begin, end);
            copy(begin, end);
            usleep(40000);
            // display();
            gen++;
        }
    }
    else {
        while (gen < MAX_GEN) {
            destribute();
            build_new_gen(begin, end);
            copy(begin, end);
            merge();
            comm();
            usleep(40000);
            // display();
            MPI_Barrier(MPI_COMM_WORLD);
            gen++;
        }
    }

    if (myrank == 0) {
        time_after = MPI_Wtime();
        printf("time %f\n", time_after - time_before);
    }

    MPI_Finalize();

    return 0;
}