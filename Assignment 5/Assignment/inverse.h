#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "complex.h"

int forwardElim(complex *mat, int N);

complex* backSub(complex *mat, int N);

complex * gaussianElimination(complex *mat, int N)
{
    int i,j,k;
    int singular_flag = forwardElim(mat, N);

    if (singular_flag != -1)
    {
        return NULL;
    }

    return backSub(mat, N);
}

// function for elemntary operation of swapping two rows
void swap_row(complex *mat, int i, int j, int N)
{
    int k;
    //printf("Swapped rows %d and %d\n", i, j);

    for (k=0; k<=N; k++)
    {
        complex temp = mat[i*(N+1)+k];
        make(&mat[i*(N+1)+k], mat[j*(N+1)+k].real, mat[j*(N+1)+k].imag);
        make(&mat[j*(N+1)+k], temp.real, temp. imag);
    }
}

// function to print matrix content at any stage
void print(complex *mat, int N)
{
    int i,j,k;
    for (i=0; i<N; i++, printf("\n"))
        for (j=0; j<=N; j++)
            printf("%f %f\t", mat[i*(N+1)+j].real, mat[i*(N+1)+j].imag);

    printf("\n");
}

// function to reduce matrix to r.e.f.
int forwardElim(complex *mat, int N)
{
    int i,j,k;
    for (k=0; k<N; k++)
    {
        // Initialize maximum value and index for pivot
        int i_max = k;
        complex v_max;
        make(&v_max, mat[i_max*(N+1)+k].real, mat[i_max*(N+1)+k].imag);

        /* find greater amplitude for pivot if any */
        for (i = k+1; i < N; i++)
        {
            if (modulo(&mat[i*k]) > modulo(&v_max))
            {
                make(&v_max, mat[i*(N+1)+k].real, mat[i*(N+1)+k].imag);
                i_max = i;
            }
        }

        /* if a prinicipal diagonal element  is zero,
         * it denotes that matrix is singular, and
         * will lead to a division-by-zero later. */
        if (!modulo(&mat[k*(N+1)+i_max]))
            return k; // Matrix is singular

        /* Swap the greatest value row with current row */
        if (i_max != k)
            swap_row(mat, k, i_max, N);

        for (i=k+1; i<N; i++)
        {
            /* factor f to set current row kth elemnt to 0,
             * and subsequently remaining kth column to 0 */
            complex f = multiply(mat[i*(N+1)+k], reciprocal(mat[k*(N+1)+k]));

            /* subtract fth multiple of corresponding kth
               row element*/
            for (j=k+1; j<=N; j++)
              mat[i*(N+1)+j] = subtract(mat[i*(N+1)+j], multiply(f, mat[k*(N+1)+j]));

            /* filling lower triangular matrix with zeros*/
            make(&mat[i*(N+1)+k], 0, 0);
        }

        //print(mat);        //for matrix state
    }
    //print(mat);            //for matrix state
    return -1;
}

// function to calculate the values of the unknowns
complex* backSub(complex *mat, int N)
{
    int i,j,k;
    complex *x;  // An array to store solution
    x=malloc(N*sizeof(complex));
    /* Start calculating from last equation up to the
       first */
    for ( i = N-1; i >= 0; i--)
    {
        /* start with the RHS of the equation */
        x[i].real = mat[i*(N+1)+N].real;
        x[i].imag = mat[i*(N+1)+N].imag;

        /* Initialize j to i+1 since matrix is upper
           triangular*/
        for (j=i+1; j<N; j++)
        {
            /* subtract all the lhs values
             * except the coefficient of the variable
             * whose value is being calculated */
          x[i] = subtract(x[i], multiply(mat[i*(N+1)+j], x[j]));
        }

        /* divide the RHS by the coefficient of the
           unknown being calculated */
        x[i] =multiply(x[i], reciprocal(mat[i*(N+1)+i]));
    }
    return x;
}

// Driver program
complex * inverse(complex * mat, int N)
{
    return gaussianElimination(mat, N);
}