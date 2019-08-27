#include<stdio.h>

int arr[100][100],flag=0, beg_x, beg_y,max_x,max_y,size = 0,out[100][2],dis=0;

void check_flag(int u,int v,FILE *fptr)
{
	if(flag == 1)
	{
		flag = 0;
		fprintf(fptr,"%d%d%d%d\n",u/10,u%10,v/10,v%10);
	}
}

void dfs(int x, int y,FILE *fptr)
{
	if(x>=0 && x<=max_x && y>=0 && y<=max_y && arr[x][y] == 1)
	{
		fprintf(fptr,"%d%d%d%d\n",x/10,x%10,y/10,y%10);
		arr[x][y] = 2;
		dfs(x,y+1,fptr);
		check_flag(x,y,fptr);
		dfs(x+1,y,fptr);
		check_flag(x,y,fptr);
		dfs(x,y-1,fptr);
		check_flag(x,y,fptr);
		dfs(x-1,y,fptr);
		check_flag(x,y,fptr);
		flag = 1;
	}	
}

int main(int di,char* arg[])
{
	FILE *fp;
	fp = fopen(arg[1], "r");
	FILE *fptr;
	fptr = fopen(arg[2],"w");
	char ch[20];

	fscanf(fp,"%s",ch);
	beg_x = (ch[6]-'0')*10 + (ch[7] - '0');
	beg_y = (ch[8]-'0')*10 + (ch[9] - '0');
	max_x = 0;
	max_y = 0;

	while(!feof(fp))
	{
		fscanf(fp,"%s",ch);
		int x = (ch[0] - '0')*10 + ch[1] - '0';
		int y = (ch[2] - '0')*10 + ch[3] - '0';
		arr[x][y] = ch[5] - '0';
		if(max_y<y)
			max_y = y;
		if(max_x<x)
			max_x = x;
	}

	fclose(fp);

	dfs(beg_x, beg_y, fptr);

	int count = 0,i,j;
	for(i=0;i<=max_x;i++)
	{
		for(j=0;j<=max_y;j++)
		{
			if(arr[i][j]==2)
				count++;
		}
	}
	fprintf(fptr,"%d",count );
	return 0;
}