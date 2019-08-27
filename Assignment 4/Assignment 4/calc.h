

#ifndef CALC_H
#define CALC_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>



typedef struct bc
{
	int *a;
	int neg, decimal; 
}bignum;

bignum int_to_bignum(int x);
bignum rounding(bignum x);
bignum extend(bignum x,int d);
bignum string_to_bignum(char c[]);
int negationsign(int sign);
void printbignum(bignum a);
bignum subtraction(bignum a ,bignum b);
bignum addition(bignum a ,bignum b);
bignum additionmult(bignum c ,bignum d);
bignum subtractiondivide(bignum c ,bignum d);
bignum multiplication(bignum a ,bignum b);
int greater(bignum a,bignum b);
int greaterdivide(bignum c,bignum d);
int max(int a,int b);
bignum extenddivide(bignum a);
bignum bt(bignum a);
int dividepart(bignum a,bignum b);
bignum dividepartremainder(bignum a,bignum b);
bignum multiplyby10(bignum a);
bignum division(bignum c,bignum d);
bignum negation(bignum a);
bignum convert(bignum a);
bignum power(bignum a, bignum b);
bignum logarithm(bignum a);
bignum squareRoot(bignum n);

int max_size;
extern FILE* output;

#endif
