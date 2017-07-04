#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include "mla/multiple-length-arithmetic.h"

#define NUMBER_OF_TERM 100

void get10pow1000(NUMBER *a) {
    clearByZero(a);
    a->n[1010] = 1;
}

NUMBER partial_sum_for_arctan(int i, int n, int x) {
    NUMBER summation;
    NUMBER denom; // bumbo
    NUMBER result;
    NUMBER x_inverse, _x_inverse;
    NUMBER one;
    NUMBER tmp, _tmp;
    clearByZero(&summation);
    clearByZero(&denom);
    clearByZero(&result);
    clearByZero(&x_inverse);
    clearByZero(&tmp);

    get10pow1000(&one);
    setInt(&x_inverse, x);
    setInt(&_x_inverse, x);

    int k;
    for (k=i; k<=n; k+=8) {
        setInt(&denom, k);
        multiple(&x_inverse, &denom, &result);

        column_divide(&one, &result, &tmp, &_tmp);
        result = tmp;

        add(&result, &summation, &tmp);
        summation = tmp;

        power(&_x_inverse, &denom, &x_inverse);
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

    if (nsize % 2 != 0) {
        printf("cpu num must be even number.\n");
        MPI_Abort(MPI_COMM_WORLD,1);
    }

    // define MPI_NUMBER for MPI_Send/MPI_Recv
    const int nitems=2;
    int blocklengths[2] = {1,1};
    MPI_Datatype types[2] = {MPI_INT, MPI_INT};
    MPI_Datatype MPI_NUMBER;
    MPI_Aint offsets[2];

    offsets[0] = offsetof(NUMBER, n);
    offsets[1] = offsetof(NUMBER, sign);

    MPI_Type_create_struct(nitems, blocklengths, offsets, types, &MPI_NUMBER);
    MPI_Type_commit(&MPI_NUMBER);

    int i;

    int range_for_each_cpu = NUMBER_OF_TERM / nsize;
    int min = range_for_each_cpu * myrank + 1;
    int max = range_for_each_cpu * (myrank + 1);

    NUMBER partial_sum_8 = partial_sum_for_arctan(min, max, 8);
    NUMBER partial_sum_57 = partial_sum_for_arctan(min, max, 57);
    NUMBER partial_sum_239 = partial_sum_for_arctan(min, max, 239);

    if (myrank % 2 == 1) {
        setSign(&partial_sum_8, -1);
        setSign(&partial_sum_57, -1);
        setSign(&partial_sum_239, -1);
    }

    dispNumberZeroSuppress(&partial_sum_8);
    putchar('\n');
    dispNumberZeroSuppress(&partial_sum_57);
    putchar('\n');
    dispNumberZeroSuppress(&partial_sum_239);
    putchar('\n');

    MPI_Status status;

    NUMBER tmp, _tmp, sum_8, sum_57, sum_239, answer;
    NUMBER twentyfour, four, eight;

    clearByZero(&sum_8);
    clearByZero(&sum_57);
    clearByZero(&sum_239);
    clearByZero(&answer);

    setInt(&four, 4);
    setInt(&eight, 8);
    setInt(&twentyfour, 24);

    if (myrank == 0) {
        for (i=1; i<nsize; i++) {
            MPI_Recv(&tmp, 1, MPI_NUMBER, i, 100+i, MPI_COMM_WORLD, &status);
            add(&sum_8, &tmp, &_tmp);
            sum_8 = _tmp;
            MPI_Recv(&tmp, 1, MPI_NUMBER, i, 200+i, MPI_COMM_WORLD, &status);
            add(&sum_57, &tmp, &_tmp);
            sum_8 = _tmp;
            MPI_Recv(&tmp, 1, MPI_NUMBER, i, 300+i, MPI_COMM_WORLD, &status);
            add(&sum_239, &tmp, &_tmp);
            sum_8 = _tmp;
        }
    }
    else {
        MPI_Recv(&partial_sum_8, 1, MPI_INTEGER, 0, 100+myrank, MPI_COMM_WORLD);
        MPI_Recv(&partial_sum_57, 1, MPI_INTEGER, 0, 200+myrank, MPI_COMM_WORLD);
        MPI_Recv(&partial_sum_239, 1, MPI_INTEGER, 0, 300+myrank, MPI_COMM_WORLD);
    }

    multiple(&sum_8, &twentyfour, &tmp);
    add(&answer, &tmp, _tmp);


    MPI_Finalize();

    return 0;
}