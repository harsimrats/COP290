#include <math.h>
#include "inverse.h"
#include <string.h>

# define MAX_NETS 1000
#define MIN_AMP 0.0000000001
#define MIN_FREQ 0.000001

// innermost
int count=0, size_connected;
int i,j,k;
int margin=50;
int check[2];
int name_count=0;							// number of nets
double scale_factor=1.5;
 FILE* out;

int max(int a, int b)
{
	if(a>b)
		return a;
	return b;
}

int min(int a, int b)
{
	if(a<b)
		return a;
	return b;
}

void func();

char * names[MAX_NETS];						// array of names of nets

typedef struct
{
	char *name, type, *power;
	char * Net_Connected_to_terminal, * Net_Connected_to_other_terminal;
	double value;
	char *toprint;
}component;

typedef struct
{
	char *name,type;
	char * Net_Connected_to_terminal;
	char * Net_Connected_to_other_terminal;
	double dcoffset,amplitude,frequency,delay,damping, temp_freq, temp_amplitude;
	char * freq_power,* delay_power;
	char * sin_type;
	char *toprint[2];
	complex c;
}source;

typedef struct ns
{
	char *Net_Connected_to_terminal,*Net_Connected_to_other_terminal;
	component *c;
	source *s;
	struct ns *next;
}node;

node *nets[MAX_NETS];


typedef struct a
{
	component *comp ;
	struct a *next;
}complist;

typedef struct s
{
	char *x;
	struct a *next;
}llist;			//Net_Connected_to_terminal of the component list

llist list[MAX_NETS];

typedef struct b
{
	source *s ;
	struct b *next ;
}sourcelist;


int num_voltages=0, num_currents=0;

sourcelist *slist = NULL;
sourcelist *slist2 = NULL;

int checkUnitsComponent(char *q)
{
	if(strcmp(q,"O") && strcmp(q,"F") && strcmp(q,"H"))return 0;
	else return 1;
}

char setType(char *q)
{
	if(!strcmp(q,"O"))return 'R';
	else if(!strcmp(q,"F"))return 'C';
	else return 'L';
}

int checkNode(node n,int j)
{
	//printf("enetred checkNode with j: %d and count: %d\n",j,count);
	if(count==0)
	{
		
		return -1;
	}
	//printf("nets[j]->Net_Connected_to_terminal: %s and n.Net_Connected_to_terminal: %s\n",nets[j].Net_Connected_to_terminal,n.Net_Connected_to_terminal);
	//printf("nets[j]->Net_Connected_to_other_terminal: %s and n.Net_Connected_to_other_terminal: %s\n",nets[j].Net_Connected_to_other_terminal,n.Net_Connected_to_other_terminal);
	if(j>=count)
	{
		return -1;
	}
	else if(!strcmp(nets[j]->Net_Connected_to_terminal,n.Net_Connected_to_terminal) && !strcmp(nets[j]->Net_Connected_to_other_terminal,n.Net_Connected_to_other_terminal))			//eneters if strings are equal
	{
		return j;
	}
	else
	{
		//printf("nets: %s %s checked against: %s %s\n",nets[j].Net_Connected_to_terminal,nets[j].Net_Connected_to_other_terminal,n.Net_Connected_to_terminal,n.Net_Connected_to_other_terminal);
		//printf("being returned: %d\n",j);
		return checkNode(n,j+1);
	}
}

double checkValue(char* s)
{
	if(!strcmp(s, "F"))
	{
		return pow(10,-15);
	}
	if(!strcmp(s, "P"))
	{
		return pow(10,-12);
	}
	if(!strcmp(s, "M"))
	{
		return pow(10,-3);
	}
	if(!strcmp(s, "N"))
	{
		return pow(10,-9);
	}
	if(!strcmp(s, "MEG"))
	{
		return pow(10,6);
	}
	if(!strcmp(s, "U"))
	{
		return pow(10,-6);
	}
	if(!strcmp(s, "K"))
	{
		return pow(10,3);
	}
	else return 1;
}

int abs(int x)
{
	if(x>0)return x;
	else return -x;
}

int isPresent(char *s)
{
	if(name_count==0)
	{
		names[0] = s;
		name_count=1;
		return 0;
	}
	else
	{
		for(int i=0;i<name_count;i++)
		{
			if(!strcmp(s,names[i]))return 1;
		}
	}
	names[name_count] = s;
	name_count++;
	return 0;
}

void adds(node *n)
{
	//printf("count: %d\n",count);
	//printf("to be checked: %s %s\n",n.Net_Connected_to_terminal, n.Net_Connected_to_other_terminal);
	int a = isPresent(n->Net_Connected_to_terminal);
	int b = isPresent(n->Net_Connected_to_other_terminal);
	int x=-1;
	if(a && b)x = checkNode(*n,0);
	//printf("%d\n", x);
	if(x==-1 && count<MAX_NETS-1)
	{
		nets[count]= n;
		count++;
	}
	else if(count==MAX_NETS-1)
	{
		printf("MAX_NETS exceeded");
	}
	else
	{
		printf("entering here with x: %d\n",x);
		n->next = nets[x];
		nets[x] = n;
		//printf("%s\n", nets[x]->s->name);
	}
}

void printdot(int x, int y)
{
	fprintf(out, "<circle cx = \"%d\" cy = \"%d\" r = \"2\" />\n", x, y);
}

void printLine(int x1, int y1 , int x2 , int y2)
{
	fprintf(out,"<line x1 = \"%d\" y1 = \"%d\" x2 = \"%d\" y2 = \"%d\" stroke=\"black\"/>\n",x1,y1,x2,y2);
}

void printInductor(component *c,int x,int y)
{
	fprintf(out,"<path fill=\"none\" stroke=\"black\" d=\"M %d,%d L %d,%d Q %d,%d %d,%d Q %d,%d %d,%d Q %d,%d %d,%d L %d,%d\"/>\n",x,y,x,y+10,x+10,y+15,x,y+20,x+10,y+25,x,y+30,x+10,y+35,x,y+40,x,y+50);
	fprintf(out,"</g>\n<text x=\"%f\" y=\"%f\" font-size=\"%f\" fill=\"black\">%s %.3f %sH</text>\n<g transform=\"scale(%f)\">\n", scale_factor*(x+10),scale_factor*(y+25),scale_factor*15, c->name,c->value,c->toprint,scale_factor);
}

void printResistor(component *c,int x,int y)
{
	fprintf(out,"<path fill=\"none\" stroke=\"black\" d=\"M %d,%d L %d,%d L %d,%d %d,%d L %d,%d %d,%d L %d,%d %d,%d L %d,%d\"/>\n",x,y,x,y+10,x+10,y+15,x,y+20,x+10,y+25,x,y+30,x+10,y+35,x,y+40,x,y+50);
	fprintf(out,"</g>\n<text x=\"%f\" y=\"%f\" font-size=\"%f\" fill=\"black\">%s %.3f %s</text>\n<g transform=\"scale(%f)\">\n", scale_factor*(x+13),scale_factor*(y+25),scale_factor*15, c->name,c->value,c->toprint,scale_factor);
}

void printCapacitor(component *c,int x, int y)
{
	fprintf(out,"<path fill=\"none\" stroke=\"black\" d=\"M %d,%d L %d,%d M %d,%d L %d,%d M %d,%d L %d,%d M %d,%d L %d,%d\"/>\n",x,y,x,y+20,x-8,y+20,x+8,y+20,x-8,y+30,x+8,y+30,x,y+30,x,y+50);
	fprintf(out,"</g>\n<text x=\"%f\" y=\"%f\" font-size=\"%f\" fill=\"black\">%s %.3f %sF</text>\n<g transform=\"scale(%f)\">\n", scale_factor*(x+10),scale_factor*(y+25),scale_factor*12, c->name,c->value,c->toprint,scale_factor);
}

void printVoltage(source *s,int x1, int y1)
{
	int x= x1, y= y1+25;
	fprintf(out,"<circle fill=\"none\" stroke=\"black\" cx=\"%d\" cy=\"%d\" r=\"15\"/>\n<path fill=\"none\" stroke=\"black\" d=\"M %d,%d Q %d,%d %d,%d M %d,%d Q %d,%d %d,%d\"/>\n",x,y,x,y,x+5,y-10,x+10,y,x,y,x-5,y+10,x-10,y);
	fprintf(out,"</g>\n<text x=\"%f\" y=\"%f\" font-size=\"%f\" fill=\"black\">%s (%.3f</text>\n", scale_factor*(x+18),scale_factor*(y-10),scale_factor*10,s->sin_type,s->dcoffset);
	fprintf(out,"<text x=\"%f\" y=\"%f\" font-size=\"%f\" fill=\"black\">%.3f %.3f%shz</text>\n", scale_factor*(x+18),scale_factor*(y),scale_factor*10,s->amplitude,s->frequency,s->toprint[0]);
	fprintf(out,"<text x=\"%f\" y=\"%f\" font-size=\"%f\" fill=\"black\">%.3f %.3f%s)</text>\n<g transform=\"scale(%f)\">\n", scale_factor*(x+18),scale_factor*(y+10),scale_factor*10,s->delay,s->damping,s->toprint[1],scale_factor);
	printLine(x1,y1,x1,y1+10);
	printLine(x1,y1+40,x1,y1+50);
}

void printCurrent(source *s,int x1, int y1)
{
	int x= x1, y= y1+25;
	fprintf(out,"<circle fill=\"none\" stroke=\"black\" cx=\"%d\" cy=\"%d\" r=\"15\"/>\n<path fill=\"none\" stroke=\"black\" d=\"M %d,%d L %d,%d L %d,%d L %d,%d L %d,%d\"/>\n",x,y,x1,y1+35,x,y1+15,x-2,y1+17,x+2,y1+17,x,y1+15);
	fprintf(out,"</g>\n<text x=\"%f\" y=\"%f\" font-size=\"%f\" fill=\"black\">%s (%.3f</text>\n", scale_factor*(x+18),scale_factor*(y-10),scale_factor*10,s->sin_type,s->dcoffset);
	fprintf(out,"<text x=\"%f\" y=\"%f\" font-size=\"%f\" fill=\"black\">%.3f %.3f%shz</text>\n", scale_factor*(x+18),scale_factor*(y+3),scale_factor*10,s->amplitude,s->frequency,s->toprint[0]);
	fprintf(out,"<text x=\"%f\" y=\"%f\" font-size=\"%f\" fill=\"black\">%.3f %.3f%s)</text>\n<g transform=\"scale(%f)\">\n", scale_factor*(x+18),scale_factor*(y+13),scale_factor*10,s->delay,s->damping,s->toprint[1],scale_factor);
	printLine(x1,y1,x1,y1+10);
	printLine(x1,y1+40,x1,y1+50);
}

void printmain(node n, int x, int y, int y2)
{	
	char type;
	if(n.s==NULL)type=n.c->type;
	else type=n.s->type;
	//printf("printmain called with: %c %d %d %d",type,x,y,y2);
	printLine(x,y+50,x,y2);
	printdot(x,y2);
	printdot(x,y);
	if(type== 'R')
			printResistor(n.c,x, y);

	else if(type== 'L')
			printInductor(n.c,x, y);

	else if(type=='C')
			printCapacitor(n.c,x, y);

	else if(type== 'V')
			printVoltage(n.s,x, y);
		
	else if(type== 'I')
			printCurrent(n.s,x, y);
}

void printtext(int x, int y, char* s)
{
	fprintf(out, "<text x=\"%f\" y=\"%f\" font-size=\"%f\" fill=\"black\">%s</text>", scale_factor*x, scale_factor*y,scale_factor*15, s );
}

void printnames()
{
	int i;
	fprintf(out,"</g>");
	for (i = 0; i < name_count; i++)
	{
		printtext(margin-10*(strlen(names[i])+1),(i*50+margin),names[i]);
	}
	fprintf(out,"<g transform=\"scale(%f)\">",scale_factor);
}

int getNum(char *s)
{
	int i=0;
	for(i=0;i<name_count;i++)
	{
		if(!strcmp(s,names[i]))return i;
	}
	printf("could not find the net");
	return -1;
}

int sort_net_list()
{
	int i, flag =0;
	for(i=0; i<name_count; i++)
	{
		if (strcmp(names[i], "0")==0)
		{
			flag=1;
			names[i] = names[0];
			names[0] = "0";
			break;
		}
	}
	if (flag == 0)
	{	
		printf("Error: Ground doesnot exist");
		return 1;
	}
	return 0;
}

void compute(llist l[MAX_NETS],sourcelist * s, sourcelist * cu);


sourcelist *dclist_v;

void dc_remove_v()
{
	sourcelist *temp = slist;
	while(temp!=NULL)
	{
		if(temp->s->dcoffset != 0)
		{
			sourcelist *temp1;
			temp1 = malloc(sizeof(sourcelist));
			temp1->s = temp->s;
			temp1->next = dclist_v;
			temp->s->dcoffset = 0;
			dclist_v= temp1;
		}
		temp = temp->next;
	}
}

sourcelist *dc_current_list;

void dc_current_remove()
{
	sourcelist *temp = slist2;
	while(temp!=NULL)
	{
		if(temp->s->dcoffset != 0 && temp->s->type == 'C')
		{
			sourcelist *temp1 = (sourcelist *)malloc(sizeof(sourcelist));
			temp1->s = temp->s;
			temp1->next = dc_current_list;
			temp->s->dcoffset = 0;
			dc_current_list = temp1;
		}
		temp = temp->next;
	}
}

void freqsort()
{
	sourcelist *ptr = slist;
	while(ptr != NULL)
	{
		sourcelist *ptr2 = ptr;
		while(ptr2 != NULL)
		{
			sourcelist *ptr3 = ptr2->next;
			if(ptr3->s->frequency <= ptr->s->frequency)
			{
				sourcelist *temp = ptr;
				ptr->next = ptr3->next;
				ptr3->next = temp->next;
				ptr2->next = temp;
			}
			ptr2 = ptr2->next;
		}
		ptr = ptr->next;
	}
}

void fnc()
{
	//printf("yes: fnc\n");
	int random = sort_net_list();
	if(random)return;
	int i=0;
	out = fopen("output.svg","w");
	fprintf(out, "<svg xmlns=\"http://www.w3.org/2000/svg\" height=\"1000000\" width=\"1000000\">\n<g transform=\"scale(%f)\">\n",scale_factor);
	int sizes[name_count];
	for(i=0;i<name_count;i++)sizes[i]=margin;
	//printf("yes: %d\n",count);
	for(i=0;i<count;i++)
	{
		int r=0;
		node *n;
		n=nets[i];

		while(n!=NULL)
		{
			int x = getNum(n->Net_Connected_to_terminal);
			int y = getNum(n->Net_Connected_to_other_terminal);

		

			if(x==-1 || y==-1)
			{
				printf("Net was not found. returned from fnc\n");
				return;
			}

			int z=abs(x-y);
			int max,min;
			if(x>y){max=x;min=y;}
			else {max=y;min=x;}

			
				if(z==1 && r==0)
				{
					printmain(*n,margin,min*50+margin,max*50+margin);
					r++;
				}
				else
				{
					int max_size;
					if(sizes[min]>sizes[max])max_size=sizes[min];
					else max_size= sizes[max];
					printLine(margin,min*50+margin,max_size+100,min*50+margin);
					printLine(margin,max*50+margin,max_size+100,max*50+margin);
					printmain(*n,max_size+100,min*50+margin,max*50+margin);
					for(j=min;j<max;j++)
					{
						sizes[j]=max_size+100;
					}
				}
				
			n=n->next;
		}
		
	}
	printnames();
	fprintf(out, "%s\n", "</g>\n</svg>");
	//printf("oooohoooo");
	//fflush(stdout);

	freqsort();
	func();
	compute(list, slist, slist2);

}


void func()
{
	int i, j, k=0;
	for (i = 0; i < name_count; i++)
	{
		list[i].x = names[i];	
		for (j = 0; j < count; j++)
		{
			node *temp2 = nets[j];
			while(temp2 != NULL)
			{
				if(temp2->c!= NULL)
				{
					if(strcmp(temp2->Net_Connected_to_terminal, names[i])==0 || strcmp(temp2->Net_Connected_to_other_terminal, names[i])==0)
					{
						complist *temp;
						temp=malloc(sizeof(complist));
						temp->comp = temp2->c;
						temp->next = list[i].next;
						list[i].next = temp;
					}
				}
				else
				{

					sourcelist *temp;
					temp=malloc(sizeof(sourcelist));
					temp->s=NULL;
					temp->next=NULL;


					temp->s = temp2->s;

					make(&temp->s->c,temp->s->amplitude*sin(2*3.142857*temp->s->frequency*temp->s->delay),temp->s->amplitude*cos(2*3.142857*temp->s->frequency*temp->s->delay));			
					if(temp->s->type=='V')
					{
						temp->next = slist;
						num_voltages++;
						slist=temp;	
					}
					else
					{
						temp->next = slist2;
						num_currents++;
						slist2=temp;
					}
				}

				temp2 = temp2->next;
			}
		}
	}
	dc_remove_v();

	dc_current_remove();
	
}




int volt_num_func(sourcelist *s)
{
	int count=0;
	sourcelist *ptr = slist;
	while(ptr != NULL)
	{
		if (ptr == s)
			break;
		ptr = ptr->next;
		count++;
	}
	return count;
}



complex *impedance(char c, double value, double w)
{
	complex *com=malloc(sizeof(complex));
	switch(c)
    {
        case 'R':
        {
            make(com,1/value,0);
            break;
        }
        case 'L':
        {
            make(com,0,-1/(w*value));
            break;
        }
        case 'C':
        {
            make(com,0,w*value);
            break;
        }
    }
    return com;
}

void printMat(complex * mat, int a,int b, double f)
{
	int i;
	printf("frequency: %f\n", f);
	printf("VOLTAGES\n");
	for(i=0;i<a;i++)
	{
		printf("%s %f %f\n", names[i+1],mat[i].real,mat[i].imag);
	}
	printf("\nCURRENTS\n");
	for(i=0;i<b;i++)
	{
		printf(" %f %f\n", mat[i].real,mat[i].imag);
	}
}

void compute(llist l[MAX_NETS], sourcelist * s, sourcelist * cu)
{
	if(s == NULL && cu==NULL)return;
	sourcelist * temps =s;
	double w;
	while(temps!=NULL)
	{

		while(temps->next!=NULL && temps->next->s->frequency==temps->s->frequency){
			temps=temps->next;

		}
		int hor = name_count+num_voltages,ver=name_count-1+num_voltages;
		sourcelist * temp= s;

		//make_connected_list(s);	
		int j;
		w = 2*3.142857*(temps->s->frequency);

		complex * A = malloc((ver)*(hor)*sizeof(complex));
		for(j=0;j<ver*hor;j++)
		{
			 make(&A[j],0,0);
		}
		for(j=0;j<name_count;j++)
		{

			if(strcmp(l[j].x,"0"))
			{
				complist * c = l[j].next;
				while(c!=NULL)
				{
					complex * qw = impedance(c->comp->type,c->comp->value,w);
					int a,b;
					if(strcmp(c->comp->Net_Connected_to_terminal,"0")!=0 && strcmp(c->comp->Net_Connected_to_other_terminal,"0")!=0)
					{
						a= getNum(c->comp->Net_Connected_to_terminal)-1,b=getNum(c->comp->Net_Connected_to_other_terminal)-1;
						A[b*hor+b]=add(A[b*hor+b],*qw);
						A[a*hor+b]=add(A[a*hor+b],multiply_imm(*qw,-1));
						A[b*hor+a]=add(A[b*hor+a],multiply_imm(*qw,-1));
					}
					else if(strcmp(c->comp->Net_Connected_to_terminal,"0")==0)
					{
						a = getNum(c->comp->Net_Connected_to_other_terminal)-1;
					}
					else
					{
						a= getNum(c->comp->Net_Connected_to_terminal)-1;
					}
					A[a*hor+a] = add(A[a*hor+a],*qw);
					c=c->next;
				}
			}
		}

		sourcelist * s1 =s;

		while(s1!=NULL)
		{
			if(s1->s->frequency!=temps->s->frequency)
			{
				s1->s->temp_amplitude = MIN_AMP;
			}
			else
			{
				s1->s->temp_amplitude = s1->s->amplitude;
			}

			int qwer = volt_num_func(s1);

			int a = getNum(s1->s->Net_Connected_to_terminal)-1;
			int b = getNum(s1->s->Net_Connected_to_other_terminal)-1;

			if(strcmp(temps->s->Net_Connected_to_terminal,"0")==0)
			{
				make(&A[(name_count+qwer)*hor+a],1,0);
				make(&A[a*hor+(qwer+name_count)],1,0);
			}
			else if(strcmp(temps->s->Net_Connected_to_other_terminal,"0")==0)
			{
				make(&A[(name_count+qwer)*hor+a],-1,0);
				make(&A[a*hor+(qwer+name_count)],-1,0); 
			}
			else
			{
				make(&A[(name_count+qwer)*hor+a],-1,0);
				make(&A[a*hor+(name_count+qwer)],-1,0);
				make(&A[(name_count+qwer)*hor+b],1,0);
				make(&A[b*hor+(name_count+qwer)],1,0);
			}
			A[(name_count+qwer)*hor+hor-1]=multiply_imm(temps->s->c,(temps->s->temp_amplitude/temps->s->amplitude));
			A = inverse(A,size_connected);
			s1=s1->next;
		}

		s1= cu;

		while(s1!=NULL)
		{
			if(s1->s->frequency!=temps->s->frequency)
			{
				s1->s->temp_amplitude = MIN_AMP;
			}
			else
			{
				s1->s->temp_amplitude = s1->s->amplitude;
			}

			//int qwer = volt_num_func(s1);

			int a = getNum(s1->s->Net_Connected_to_terminal)-1;
			int b = getNum(s1->s->Net_Connected_to_other_terminal)-1;

			if(strcmp(temps->s->Net_Connected_to_terminal,"0")==0)
			{
				A[(name_count+b)*hor+hor-1]= multiply_imm(temps->s->c,(temps->s->temp_amplitude/temps->s->amplitude));
			}
			else if(strcmp(temps->s->Net_Connected_to_other_terminal,"0")==0)
			{
				A[(name_count+a)*hor+hor-1]= multiply_imm(temps->s->c,-1*(temps->s->temp_amplitude/temps->s->amplitude));
			}
			else
			{
				A[(name_count+a)*hor+hor-1]= multiply_imm(temps->s->c,-1*(temps->s->temp_amplitude/temps->s->amplitude));
				A[(name_count+b)*hor+hor-1]= multiply_imm(temps->s->c,(temps->s->temp_amplitude/temps->s->amplitude));
			}
			s1=s1->next;
		}
			complex *d = inverse(A,size_connected);
	printMat(d,name_count,num_voltages, temps->s->frequency);
	temps = temps->next;

	//	printf("\nSolution for the system:\n");								//A has the inverse
	//	for (i=0; i<size_connected+num_voltages; i++) printf("%lf\n", A[i]);							
	}
}


