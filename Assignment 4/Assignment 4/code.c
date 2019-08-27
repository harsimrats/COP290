#include "calc.h"

bignum int_to_bignum(int x)
{
	bignum b;
	int i=0;
	if(x>0)
		b.neg = 0;
	if(x<0)
		b.neg = 1;
	while(x != 0)
	{
		b.a[i] = x%10;
		x = x/10;
		i++;
	}
	b.decimal = 0;
	return b;
}

bignum rounding(bignum x)
{
	bignum r;
	int i, j=max_size-1, flag = 0, size = 0;
	for (i = 2*max_size-1; i >= 0; i--)
	{
		if(flag == 1 || x.a[i] != 0)
		{
			flag = 1;
			size = i+1;
			break;
		}
	}
	if(size-1<x.decimal)
	{
		size=1+x.decimal;
	}
	if(size-x.decimal > max_size)
	{
		fprintf(output,"LowPrec\n");
		exit(0);
	}
	else
	{
		r.a = (int *) malloc(max_size*4);
		
		for (i = size-1; i > x.decimal-1; i--)
		{
			r.a[j] = x.a[i];
			j--;
		}
		r.decimal = j+1;
		for (i = x.decimal-1; j>=0 && i>=0; i--)
		{
			r.a[j] = x.a[i];
			j--;
		}
		if (j<0 && i>=0 && x.a[i]>=5)
		{
			int k,carry=1;
			for(k=0;k<max_size;k++)
			{
				if((r.a[k]+carry)<10)
				{
					r.a[k]=r.a[k]+carry;
					carry=0;
					break;
				}
				else
				{
					r.a[k]=r.a[k]+carry-10;
				}
			}	
			if(carry==1)
			{
				fprintf(output,"LowPrec\n");
				exit(0);	
			}
		}
	}
	r.neg = x.neg;
	return r;
}

bignum extend(bignum x,int d)
{
	bignum y;
	int i=0;
	int j=0;
	y.decimal=d;
	y.a=(int *)malloc(8*max_size);
	int k=x.decimal;
	while(k!=(d))
	{
		*(y.a+i)=0;
		i++;
		k++;
	}
	for(j=0;j<max_size;j++)
	{
		*(y.a+i)=*(x.a+j);
		i++;
	}
	while(i!=(2*max_size))
	{
		*(y.a+i)=0;
		i++;
	}
	y.neg=x.neg;
	return y;
}

bignum string_to_bignum(char c[])
{
	bignum temp;
	temp.a = malloc(10000000*sizeof(int));
	temp.decimal=0;
	int i, size=0;
	for (i = 0; c[i]!='\0'; i++)
	{
		if (c[i]!='.' && c[i]!='-')
		{
			size ++;
		}
	}
	temp.neg=0;
	int o = 0;
	for(i=max_size-1;i>size-1;i--)
	{
		*(temp.a+i)=0;
	}
	for (i = 0; c[i]!='\0'; i++)
	{
		if (c[i]!='.' && c[i]!='-' && o<size)
		{ 
			*(temp.a-o+size-1) = c[i] - '0';
			o++;
		}
		else if( c[i]=='.')
		{
			temp.decimal = size-o;
		}
		else if( c[i]=='-')
		{
			temp.neg = 1;
		}
	}
	return temp;
}

int negationsign(int sign)
{
	if(sign==0)
		return 1;
	else return 0;
}

void printbignum(bignum a)
{
	int i;
	if(a.neg==0);
	else
		fprintf(output,"-");
	for(i=(max_size-1);i>=a.decimal;i--)
	{
		fprintf(output,"%d",*(a.a+i));
	}
	if(a.decimal>0)
	fprintf(output,".");
	for(i=a.decimal-1;i>=0;i--)
	{
		fprintf(output,"%d",*(a.a+i));
	}
}

bignum subtraction(bignum a ,bignum b);
bignum addition(bignum a ,bignum b)
{
	bignum c,d;
	bignum e;
	int i,x, carry = 0;
	if(a.decimal>=b.decimal)
	{
		c=extend(a,a.decimal);
		d=extend(b,a.decimal);
	}

	else
	{
		c=extend(a,b.decimal);
		d=extend(b,b.decimal);
	}

	e.decimal=c.decimal;
	e.a=(int *)malloc(max_size*8);
	if(c.neg==d.neg)
	{
		e.neg = c.neg;
		for (i = 0; i <2*max_size; i++)
		{
			x = *(c.a+i) + *(d.a+i) + carry;
			*(e.a+i) = x%10;
			carry = x/10;
		}
	}
	else
	{	
		b.neg=negationsign(b.neg);
		e=subtraction(a,b);
	}
	return e;
}

bignum additionmult(bignum c ,bignum d)
{
	bignum e;
	int i,x, carry = 0;
	e.decimal=0;
	e.a=(int *)malloc(max_size*8);
	e.neg = 0;
	for (i = 0; i <2*max_size; i++)
	{
		x = *(c.a+i) + *(d.a+i) + carry;
		*(e.a+i) = x%10;
		carry = x/10;
	}
	return e;
}

bignum subtractiondivide(bignum c ,bignum d)
{
	bignum e;
	int i,x, borrow = 0;
	e.decimal=max_size-1;
	e.a=(int *)malloc(max_size*8);
	e.neg = 0;
	for (i = 0; i <2*max_size; i++)
	{
		x = *(c.a+i) - *(d.a+i) - borrow;
		if(x<0)
		{
			borrow=1;
			x=x+10;
		}
		else
		{
			borrow=0;
		}
		*(e.a+i) = x;
	}
	return e;
}

bignum subtraction(bignum a ,bignum b)
{
	bignum c,d;
	bignum e;
	int i,x, borrow = 0;
	if(a.decimal>=b.decimal)
	{
		c=extend(a,a.decimal);
		d=extend(b,a.decimal);
	}
	else
	{
		c=extend(a,b.decimal);
		d=extend(b,b.decimal);
	}
	
	e.decimal=c.decimal;
	e.a=(int *)malloc(max_size*8);
	if (a.neg == b.neg)
	{
		e.neg = a.neg;
		for (i = 0; i <2*max_size; i++)
		{
			x = *(c.a+i) - *(d.a+i) -borrow;
			if(x<0)
			{
				borrow=1;
				x=x+10;
			}
			else
			{
				borrow=0;
			}
			*(e.a+i) = x;
		}

		if(borrow==1)
		{
			e=subtraction(b,a);
			e.neg=negationsign(e.neg);
		}
	}
	else
	{
		b.neg=negationsign(b.neg);
		e=addition(a,b);
	}

	return e;
}

bignum multiplication(bignum a ,bignum b)
{
	bignum e;
	bignum f;
	int i,j,x, carry = 0;
	
	bignum g;
	f.decimal=0;
	e.decimal=0;
	e.neg=0;
	f.neg=0;
	e.a=(int *)malloc(max_size*8);
	f.a=(int *)malloc(max_size*8);
	g.a=(int *)malloc(max_size*8);
	for (i = 0; i <max_size; i++)
	{	
		carry=0;
		for(j=0;j<i;j++)
		{
			*(f.a+j)=0;
		}
		for (j = i; j <(max_size+i); j++)
		{
			x = (*(a.a+i)) * (*(b.a+j-i)) + carry;
			*(f.a+j) = x%10;
			carry = x/10;
		}
		
		*(f.a+j)=carry;

		for(j=j+1;j<2*max_size;j++)
		{
			*(f.a+j) =0;
		}	
		
		if(i>0)
		{
			e=additionmult(g,f);
			g=e;
		}
		if(i==0)
		{
			g.neg=f.neg;
			g.decimal=f.decimal;
			for(j=0;j<2*max_size;j++)	
			*(g.a+j)=*(f.a+j);
		}
	}
	e.decimal=a.decimal+b.decimal;
	e.neg=(a.neg^b.neg);
	return e;
}

int greater(bignum a,bignum b)
{
	bignum c,d;
	if(a.decimal>=b.decimal)
	{
	c=extend(a,a.decimal);
	d=extend(b,a.decimal);
	}
	else
	{
	c=extend(a,b.decimal);
	d=extend(b,b.decimal);
	}
	int i;
	int ans=0;
	for(i=2*max_size-1;i>=0;i--)
	{
	if(c.a[i]>d.a[i])
	{	ans=1;
		break;
	}
	else if(c.a[i]<d.a[i])
	{	ans=-1;
		break;
	}
	}
	return ans;
}

int greaterdivide(bignum c,bignum d)
{
	int i;
	int ans=0;
	if(c.decimal>d.decimal)
	return -1;
	else if(c.decimal<d.decimal)
	return 1;
	for(i=2*max_size-1;i>=0;i--)
	{
	if(c.a[i]>d.a[i])
	{	ans=1;
		break;
	}
	else if(c.a[i]<d.a[i])
	{	ans=-1;
		break;
	}
	}
	return ans;
}

int max(int a,int b)
{
	if(a>=b)
		return a;
	else 
		return b;
}

bignum extenddivide(bignum a)
{
	bignum b;
	int i;
	int j=0;
	b.neg=a.neg;
	b.decimal=max_size-1;
	b.a=(int *)malloc(max_size*8);
	for(i=0;i<max_size-a.decimal-1;i++)
	{
		b.a[i]=0;
	}
	for(i=0;i<a.decimal;i++)
	{
		b.a[max_size-a.decimal-1+i]=a.a[i];
	}
	for(i=max_size-1;i<2*max_size-a.decimal-1;i++)
	{
		b.a[i]=a.a[a.decimal-max_size+i+1];
	}
	for(i=2*max_size-1;i>=2*max_size-a.decimal-1;i--)
	{
		b.a[i]=0;
	}
	return b;
}

bignum bt(bignum a)
{	
	int j;
	bignum c;
	c.decimal=a.decimal;
	c.a=(int *)malloc(sizeof(int *));
	c.neg=a.neg;
	return c;
}

int dividepart(bignum a,bignum b)
{
	int count=0;
	int j;
	bignum c;
	c.decimal=a.decimal;
	c.a=(int *)malloc(8*max_size);
	for(j=0;j<2*max_size;j++)	
		*(c.a+j)=*(a.a+j);
	c.neg=b.neg;
	
	while(greaterdivide(c,b)!=-1)
	{
		count++;
		c=subtractiondivide(c,b);
	}
	return count;
}

bignum dividepartremainder(bignum a,bignum b)
{
	int count=0;
	int j;
	bignum c;
	c.decimal=a.decimal;
	c.a=(int *)malloc(8*max_size);
	for(j=0;j<2*max_size;j++)	
		*(c.a+j)=*(a.a+j);
	c.neg=b.neg;
	
	while(greaterdivide(c,b)!=-1)
	{
		count++;
		c=subtractiondivide(c,b);
	}
	return c;
}

bignum multiplyby10(bignum a)
{
	bignum b;
	int i;
	b.neg=a.neg;
	b.decimal=max_size-1;
	b.a=(int*)malloc(8*max_size);
	b.a[0]=0;
	for(i=0;i<=2*max_size-2;i++)
	{
		b.a[i+1]=a.a[i];
	}
	return b;
}

bignum division(bignum c,bignum d)
{
	if(greater(d,string_to_bignum("0"))==0)
	{
		fprintf(output,"ZeroError");
		exit(0);
	}

	int pointer=max_size-1;
	bignum a,b;
	int flag=0;
	int dig=max(c.decimal,d.decimal);
	a=extenddivide(c);
	b=extenddivide(d);
	int dbdot=1;
	bignum e,f;
	f=b;
	e.decimal=0;
	e.neg=(a.neg^b.neg);
	e.a=(int *)malloc(8*(max_size));
	if(greaterdivide(a,b)==(-1))
	{
		e.a[pointer]=0;
		pointer--;
		e.decimal=max_size-1;
		a=multiplyby10(a);
		while(greaterdivide(a,b)==-1)
		{
			e.a[pointer]=0;
			a=multiplyby10(a);
			pointer--;
			if(pointer<0)
			{
				return rounding(extend(e,e.decimal));
			}
		}		
	}

	else
	{
		while((greaterdivide(a,multiplyby10(b)))!=-1)
		{
			b=multiplyby10(b);	
			dbdot++;
		}
		e.decimal=max_size-dbdot;
	}
	e.a[pointer]=dividepart(a,b);

	pointer--;
	a=(dividepartremainder(a,b));
	while(pointer>=0)
	{
		b=f;
		if(greaterdivide(a,b)==(-1))
		{
			a=multiplyby10(a);
			while(greaterdivide(a,b)==-1)
			{
				e.a[pointer]=0;
				a=multiplyby10(a);
				pointer--;
				if(pointer<0)
				{
					return rounding(extend(e,e.decimal));
				}
			}		
		}

		while((greaterdivide(a,multiplyby10(b)))!=-1)
		{
			b=multiplyby10(b);	
		}
		e.a[pointer]=dividepart(a,b);
		pointer--;
		if(pointer<0)
			return rounding(extend(e,e.decimal));
		a=(dividepartremainder(a,b));
	}
	return rounding(extend(e,e.decimal));
}

bignum negation(bignum a)
{
	if(a.neg == 0)
		a.neg = 1;
	if (a.neg == 1)
		a.neg = 0;
	return a;
}

bignum convert(bignum a)
{
	bignum c;
	c.neg=a.neg;
	c.decimal=0;
	c.a=(int*)malloc(4*max_size);
	int i;
	for(i=a.decimal;i<max_size;i++)
		c.a[i-a.decimal] = a.a[i];

	for(i=max_size-a.decimal;i<max_size;i++)
		c.a[i] = 0;
	return c;
}

bignum power(bignum a, bignum b)
{
	bignum c = string_to_bignum("2");
	bignum d = string_to_bignum("1");
	bignum e = string_to_bignum("0");

	if( greater(a, e) == 1)
	{
		if (b.neg == 0)
		{
			if(greater(b, e)==0)
				return d;

			if(greater(b, d)==0)
				return a;

			if((b.a[b.decimal]) % 2 == 0)
			{
				bignum temp = power(a, convert(division(b,c)));
				return multiplication(temp, temp);
			}
			else
			{
				bignum temp = power(a, convert(division(b,c)));
				bignum temp2 = multiplication(temp, temp);
				bignum temp1 = rounding(multiplication(temp2, a));
				return temp1;
			}
		}
		else
		{
			bignum x = b;
			x.neg = 0;
			x.decimal = b.decimal;
			bignum y = division(d, a);
			return power(y, x);
		}
	}
	else
	{
		if (greater(a, e) == 0 && greater(b, e) == 0)
		{
			fprintf(output,"Not Defined");
			exit(0);
		}
		bignum x = a;
		x.neg = 0;
		x.decimal = a.decimal;
		bignum y = power(x, b);

		if(b.a[b.decimal]% 2 == 0)
			y.neg = 0;
		if(b.a[b.decimal]% 2 == 1)
			y.neg = 1;
	}
}
	
bignum logarithm(bignum a)
{
	bignum e = string_to_bignum("0");
	if(greater(a, e) == 1)
	{
		bignum b = string_to_bignum("1");
		bignum c = string_to_bignum("2");
		bignum temp = division(subtraction(a, b), addition(a, b));
		bignum ln, temp1;
		for (int i = 0; i < 100; i++)
		{
			int n = 2*i + 1;
			temp1 = power(temp, int_to_bignum(n));
			ln = addition(ln, division(temp1, int_to_bignum(n)));
		}
		ln = multiplication(c, ln);
	}
	else
		fprintf(output,"Not Defined");
}

bignum squareRoot(bignum n)
{
	if(n.neg == 0)
	{
 		bignum x = n;
 		bignum y = string_to_bignum("1") ;
 	 	bignum z = string_to_bignum("2") ;
  		bignum e = string_to_bignum("0.00000001");
  		while(greater( subtraction(x, y), e) == 1)
  		{
  			x = division(addition(x, y), z);
  			y = division(n, x);
  		}
  		return x;
  	}
  	else
  	{
  		fprintf(output,"ERROR");
  		exit(0);
  	}
  	return n;
}