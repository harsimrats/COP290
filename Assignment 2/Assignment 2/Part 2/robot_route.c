#include<stdio.h>

int arr[100][100][100],flag=0, beg_x, beg_y,beg_z,max_x,max_y,max_z,size = 0;

void check_flag(int u,int v,int w, FILE *fptr)
{
	if(flag == 1)
	{
		flag = 0;
		fprintf(fptr,"%d%d%d%d%d%d\n",u/10,u%10,v/10,v%10,w/10,w%10);
	}
}

void dfs(int x, int y, int z, FILE *fptr)
{
	if(x>=0 && x<=max_x && y>=0 && y<=max_y && z>=0 && z<=max_z && arr[x][y][z] == 1)
	{
		fprintf(fptr,"%d%d%d%d%d%d\n",x/10,x%10,y/10,y%10,z/10,z%10);
		arr[x][y][z] = 2;
		dfs(x,y+1,z,fptr);
		check_flag(x,y,z,fptr);
		dfs(x+1,y,z,fptr);
		check_flag(x,y,z,fptr);
		dfs(x,y-1,z,fptr);
		check_flag(x,y,z,fptr);
		dfs(x-1,y,z,fptr);
		check_flag(x,y,z,fptr);
		dfs(x,y,z-1,fptr);
		check_flag(x,y,z,fptr);
		dfs(x,y,z+1,fptr);
		check_flag(x,y,z,fptr);
		flag = 1;
	}	
}

int main(int di, char *arg[])
{
	FILE *fp; FILE *fptr;
	fp = fopen(arg[1], "r");
	fptr = fopen(arg[2],"w");
	char ch[20];

	fscanf(fp,"%s",ch);
	beg_x = (ch[6]-'0')*10 + (ch[7] - '0');
	beg_y = (ch[8]-'0')*10 + (ch[9] - '0');
	beg_z = (ch[10]-'0')*10 + (ch[11] - '0');
	max_x = 0;
	max_y = 0;
	max_z = 0;

	while(!feof(fp))
	{
		fscanf(fp,"%s",ch);
		int x = (ch[0] - '0')*10 + ch[1] - '0';
		int y = (ch[2] - '0')*10 + ch[3] - '0';
		int z = (ch[4] - '0')*10 + ch[5] - '0';
		arr[x][y][z] = ch[7] - '0';
		if(max_y<y)
			max_y = y;
		if(max_x<x)
			max_x = x;
		if(max_z<z)
			max_z = z;
	}

	fclose(fp);

	dfs(beg_x,beg_y,beg_z,fptr);

	int count = 0,i,j,k;
	for(i=0;i<=max_x;i++)
	{
		for(j=0;j<=max_y;j++)
		{
			for(k=0;k<=max_z;k++)
			{
				if(arr[i][j][k]==2)
					count++;
			}
		}
	}
	fprintf(fptr,"%d",count );
	return 0;
}