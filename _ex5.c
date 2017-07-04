#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include "mla/integer.h"

void get10pow1000(Number *a) {
    clear_by_zero(a);
    a->n[1010] = 1;
}

Number partial_sum_for_arctan(int start, int n, int x, int rank) {
    Number summation;
    Number denom; // bumbo
    Number result;
    Number x_inverse, _x_inverse;
    Number one;
    Number tmp, _tmp;
    clear_by_zero(&summation);
    clear_by_zero(&denom);
    clear_by_zero(&result);
    clear_by_zero(&x_inverse);
    clear_by_zero(&tmp);

    get10pow1000(&one);
    set_int(&_x_inverse, x);

    int k = start;
    while(1) {
        set_int(&denom, k);

        power(&_x_inverse, &denom, &x_inverse);

        multiple(&x_inverse, &denom, &result);

        if (compare_number(&one, &result) == -1) break;
    
        divide(&one, &result, &tmp, &_tmp);
        copy_number(&tmp, &result);

        add(&result, &summation, &tmp);
        copy_number(&tmp, &summation);

        k += n * 2;
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

    // define MPI_Number for MPI_Send/MPI_Recv
    const int nitems = 2;
    int blocklengths[2] = {DIGIT_NUMBER, 1};
    MPI_Datatype types[2] = {MPI_INT, MPI_INT};
    MPI_Datatype MPI_Number;
    MPI_Aint offsets[2];

    offsets[0] = offsetof(Number, n);
    offsets[1] = offsetof(Number, sign);

    MPI_Type_create_struct(nitems, blocklengths, offsets, types, &MPI_Number);
    MPI_Type_commit(&MPI_Number);

    int i;
    int start = 2*myrank + 1;

    Number partial_sum_8 = partial_sum_for_arctan(start, nsize, 8, myrank);
    Number partial_sum_57 = partial_sum_for_arctan(start, nsize, 57, myrank);
    Number partial_sum_239 = partial_sum_for_arctan(start, nsize, 239, myrank);

    if (myrank % 2 == 1) {
        set_sign(&partial_sum_8, -1);
        set_sign(&partial_sum_57, -1);
        set_sign(&partial_sum_239, -1);
    }

    // printf("partial sums:\n");
    // printf("atan(1/8): ");
    // display_number_zero_suppress(&partial_sum_8);
    // putchar('\n');
    // printf("atan(1/57): ");
    // display_number_zero_suppress(&partial_sum_57);
    // putchar('\n');
    // printf("atan(1/239): ");
    // display_number_zero_suppress(&partial_sum_239);
    // putchar('\n');

    MPI_Status status;

    Number tmp, _tmp, sum_8, sum_57, sum_239, answer;
    Number twentyfour, four, eight;

    clear_by_zero(&sum_8);
    clear_by_zero(&sum_57);
    clear_by_zero(&sum_239);
    clear_by_zero(&answer);
    clear_by_zero(&tmp);
    clear_by_zero(&_tmp);

    set_int(&four, 4);
    set_int(&eight, 8);
    set_int(&twentyfour, 24);

    if (myrank == 0) {
        copy_number(&partial_sum_8, &sum_8);
        copy_number(&partial_sum_57, &sum_57);
        copy_number(&partial_sum_239, &sum_239);

        for (i=1; i<nsize; i++) {
            MPI_Recv(&tmp, 1, MPI_Number, i, 100+i, MPI_COMM_WORLD, &status);
            add(&sum_8, &tmp, &_tmp);
            sum_8 = _tmp;

            MPI_Recv(&tmp, 1, MPI_Number, i, 200+i, MPI_COMM_WORLD, &status);
            add(&sum_57, &tmp, &_tmp);
            sum_57 = _tmp;

            MPI_Recv(&tmp, 1, MPI_Number, i, 300+i, MPI_COMM_WORLD, &status);
            add(&sum_239, &tmp, &_tmp);
            sum_239 = _tmp;
        }

        multiple(&sum_8, &twentyfour, &tmp);
        add(&answer, &tmp, &_tmp);
        answer = _tmp;

        multiple(&sum_57, &eight, &tmp);
        add(&answer, &tmp, &_tmp);
        answer = _tmp;

        multiple(&sum_239, &four, &tmp);
        add(&answer, &tmp, &_tmp);
        answer = _tmp;

        display_number_zero_suppress(&answer);
        putchar('\n');
    }
    else {
        MPI_Send(&partial_sum_8, 1, MPI_Number, 0, 100+myrank, MPI_COMM_WORLD);
        MPI_Send(&partial_sum_57, 1, MPI_Number, 0, 200+myrank, MPI_COMM_WORLD);
        MPI_Send(&partial_sum_239, 1, MPI_Number, 0, 300+myrank, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}