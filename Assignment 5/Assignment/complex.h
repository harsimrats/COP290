#include <stdio.h>
#include <math.h>
typedef struct
{
	double real,imag;
}complex;

void initialise_complex(complex * c)
{
	c = (complex*)malloc(sizeof(complex));
	c->real=0;c->imag=0;
}

complex multiply(complex a, complex b)
{
	complex c;
	c.real = (a.real * b.real) - (a.imag * b.imag);
	c.imag = (a.imag*b.real) + (a.real*b.imag);
	return c;
}

complex multiply_imm(complex a,double b)
{
	complex c;
	c.real = a.real*b;
	c.imag = a.imag*b;
	return c;
}

complex add(complex a, complex b)
{
	complex c;
	c.real = a.real  +b.real;
	c.imag  = a.imag + b.imag;
	return c;
}

complex subtract(complex a,complex b)
{
	b.real=-b.real;
	b.imag = -b.imag;
	return add(a,b);
}

complex reciprocal(complex a)
{
	complex b;
	double temp = a.real * a.real + a.imag * a.imag;
	b.real = a.real/temp;
	b.imag = -a.imag/temp;
	return b;
}

double angle(complex * c)
{
	if(c->real==0)
	{
		if(c->imag>0)return 90;
		else return -90;
	}
	return tan (c->imag/c->real);
}

double modulo(complex *c)
{
	double temp = c->real*c->real + c->imag*c->imag;
	return temp;
}

void make(complex * c, double a, double b)
{
	c->real=a;c->imag=b;
}