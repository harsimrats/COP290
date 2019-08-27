#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include"create_svg.h"                      /////mult add // actual val of pc in mem

pthread_t peer[6];

int lines;
FILE* out;
bool notComplete = false;

typedef struct s
{
    int instNum;
    int type[6];
    int rs[5], rt[5];
    int sa[5], rd[5], opcode[6];
    int imm[16];
    bool isImm;
    bool isoffset;
    int offset[16];
    int word[4];
    int byte;
    int result;
    long long mult;
    char operation[100];
    int op1, op2, shiftAmt, immi;
}instStruct;

int memory[64*1024*1024];
int temp_size=0;
int instructions[1000][32];
int registerArray[34]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

int fetch_ctrl=0, decode_ctrl=0, execute_ctrl=0, memory_ctrl=0, wb_ctrl=0, ctrl=0, display_ctrl = 0;
int* fetchToDec;
instStruct decToExe, exeToMem, memToWb;
instStruct inst_decode, inst_execute, inst_memory, inst_writeback;
int pc=0;

int lookup(char ch)
{
    int ret;
    if(ch=='0')
        ret = 0;
    else if(ch=='1')
        ret = 1;
    else if(ch=='2')
        ret = 10;
    else if(ch=='3')
        ret = 11;
    else if(ch=='4')
        ret = 100;
    else if(ch=='5')
        ret = 101;
    else if(ch=='6')
        ret = 110;
    else if(ch=='7')
        ret = 111;
    else if(ch=='8')
        ret = 1000;
    else if(ch=='9')
        ret = 1001;
    else if(ch=='A' || ch=='a')
        ret = 1010;
    else if(ch=='B' || ch=='b')
        ret = 1011;
    else if(ch=='C' || ch=='c')
        ret = 1100;
    else if(ch=='D' || ch=='d')
        ret = 1101;
    else if(ch=='E' || ch=='e')
        ret = 1110;
    else if(ch=='F' || ch=='f')
        ret = 1111;
    return ret;
}

void hex_to_binary(char* str)
{
    int i,j=0;

    for(i=0; i<8; i++)
    {
    	int temp = lookup(str[i]);
        instructions[temp_size][4*i+3] = temp%10;
        temp = temp/10;
        instructions[temp_size][4*i+2] = temp%10;
        temp = temp/10;
        instructions[temp_size][4*i+1] = temp%10;
        temp = temp/10;
        instructions[temp_size][4*i]   = temp%10;

    }
    temp_size++;
}

void yyerror(char *s)
{
	fprintf(stderr, "%s\n", s);
}

int compare(int* x, int* y, int size)
{
    int i, flag=0;
    for(i=0;i<size;i++)
    {
        if(x[i] != y[i])
        {
            flag = 1;       //1-false
            break;
        }
    }
    return flag;
}

instStruct bin_to_struct(int* result)
{
    instStruct temp;
    int i;
    for(i=0;i<6;i++)                                        // assuming all registers and DP inst
    {  
        temp.opcode[i] = result[i];
        temp.type[i] = result[26+i];
    }
    for(i=0;i<5;i++)
    {
       temp.rt[i] =  result[16+i];
       temp.rs[i] =  result[21+i];
    }
    temp.instNum = pc-5;
    if(compare(temp.type, (int[6]){0,0,1,0,0,0}, 6) == 0 || compare(temp.type, (int[6]){0,0,1,1,0,1}, 6) == 0 ||
        compare(temp.type, (int[6]){0,0,1,0,1,0}, 6) == 0 || compare(temp.type, (int[6]){0,0,1,1,1,1}, 6) == 0)
    {
        temp.isImm = true;
        temp.isoffset = false;
        for(i=0;i<16;i++)
            temp.imm[i] = result[i];
    }
    else if(compare(temp.type, (int[6]){0,0,0,1,0,0}, 6) == 0 || compare(temp.type, (int[6]){0,0,0,0,0,1}, 6) == 0 || 
            compare(temp.type, (int[6]){0,0,0,1,1,1}, 6) == 0 || compare(temp.type, (int[6]){0,0,0,1,1,0}, 6) == 0 || 
            compare(temp.type, (int[6]){0,0,1,0,0,0}, 6) == 0)
    {
        temp.isImm = false;
        temp.isoffset = true;
         for(i=0;i<16;i++)
            temp.offset[i] = result[i];
    }
    else
    {
        temp.isImm = false;
        temp.isoffset = false;
            for(i=0;i<5;i++)
           temp.rd[i] =  result[11+i];
        temp.isImm = false;
    }
    return temp;
}

int binToInt( int* a, int size )
{
    int result = 0;
    int power = 1;
    for(int i =0 ; i<size; i++)
    {
        result = result + power*a[i];
        power = power*2;
    }
    return result;
}

int* intToBin( int a )
{
    int *result = malloc(32*4);
    int power = 1;
    for(int i =0 ; i<32; i++)
    {
        result[i] =  a%2;
        a = a/2;
    }
    return result;
}

int* longToBin(long long a )
{
    int *result = malloc(64*4);
    for(int i =0 ; i<64; i++)
    {
        result[i] =  a%2;
        a = a/2;
    }
    return result;
}

int* extend(int* a, int b, int c)
{
    int *result = malloc(32*4);
    for(int i = 0 ; i < b ; i++)
    {
        result[i] = a[i];
    }
    for(int i = b ; i < c ; i++)
    {
        result[i] = 0;
    }
    return result;
}
    
int* andBits(int* a, int* b )
{
    //and of a and b 
    int *result = malloc(32*4);
    for(int i=0;i<32;i++)
    {
        result[i] = a[i]&&b[i];
    }
    return result;
}

int* orBits(int* a, int* b )
{
    //and of a and b 
    int *result = malloc(32*4);
    for(int i=0;i<32;i++)
    {
        result[i] = a[i]||b[i];
    }
    return result;
}

int* norBits(int* a, int* b )
{
    //and of a and b 
    int *result = malloc(32*4);
    for(int i=0;i<32;i++)
    {
        result[i] = !(a[i]||b[i]);
    }
    return result;
}

int* shiftLeft(int* a, int b)
{
    int *result = malloc(32*4);
    for(int i=0;i<b;i++)
    {
        result[i]=0;
    }
    for(int i=b;i<32;i++)
    {
        result[i] = a[i-b];
    }
    return result;
}

void* fetch()
{
    while(true)
    {
         
        if(fetch_ctrl==1)
        {
            if(pc<lines)
            {
                fetchToDec = instructions[pc];
                pc++;
                fetch_ctrl=0;
            }
            else
            {
                fetchToDec = NULL;
                break;
            }
        }
    }
}

void* decode()
{
    while(true)
    {
        while(true)
        {
            if(wb_ctrl == 0)
            {
                break;
            }
        }
        if(decode_ctrl == 1)
        {
            if(compare(inst_decode.opcode, (int[6]){0,0,0,0,0,0}, 6) == 0)
            {
                if(compare(inst_decode.sa, (int[6]){0,0,0,0,0,0}, 6) != 0)
                {
                    //shift left immediate using sa
                    strcpy(inst_decode.operation , "slsa"); 
                    inst_decode.op1 = registerArray[ binToInt(inst_decode.rt,5) ];
                    inst_decode.shiftAmt = binToInt(inst_decode.sa,5);
                }
                else
                {
                    char* chakshu = malloc(100);
                    char* simmi = malloc(100);
                    chakshu = "Invalid instruction no. - ";
                    snprintf(simmi,100,"%d",inst_decode.instNum);
                    strcat(chakshu, simmi);
                    yyerror(chakshu);
                    exit(EXIT_FAILURE);
                }
            }
            if(compare(inst_decode.opcode, (int[6]){1,0,0,0,0,0}, 6) == 0)
            {
                //  add
                strcpy(inst_decode.operation , "add");
                inst_decode.op1 = registerArray[ binToInt(inst_decode.rs,5) ];
                inst_decode.op2 = registerArray[ binToInt(inst_decode.rt,5) ];
            }
            else if(compare(inst_decode.opcode, (int[6]){1,0,0,1,0,0}, 6) == 0)
            {
                // and
                strcpy(inst_decode.operation , "and");
                inst_decode.op1 = registerArray[binToInt(inst_decode.rs,5)];
                inst_decode.op2 = registerArray[binToInt(inst_decode.rt,5)];
            }
            else if(compare(inst_decode.opcode, (int[6]){1,0,0,1,1,1}, 6) == 0)
            {
                // nor
                strcpy(inst_decode.operation , "nor");
                inst_decode.op1 = registerArray[binToInt(inst_decode.rs,5)];
                inst_decode.op2 = registerArray[binToInt(inst_decode.rt,5)];
            }
            else if(compare(inst_decode.opcode, (int[6]){1,0,0,1,0,1}, 6) == 0)
            {
                //or
                strcpy(inst_decode.operation , "or");
                inst_decode.op1 = registerArray[binToInt(inst_decode.rs,5)];
                inst_decode.op2 = registerArray[binToInt(inst_decode.rt,5)];
            }
            else if(compare(inst_decode.opcode, (int[6]){1,0,1,0,1,1}, 6) == 0)
            {
                //set on less than unsigned
                strcpy(inst_decode.operation , "slu");
                inst_decode.op1 = registerArray[binToInt(inst_decode.rs,5)];
                inst_decode.op2 = registerArray[binToInt(inst_decode.rt,5)];
            }
            else if(compare(inst_decode.opcode, (int[6]){1,0,0,0,1,0}, 6) == 0)
            {
                // sub
                strcpy(inst_decode.operation , "sub");
                inst_decode.op1 = registerArray[binToInt(inst_decode.rs,5)];
                inst_decode.op2 = registerArray[binToInt(inst_decode.rt,5)];
            }
            else if(compare(inst_decode.opcode, (int[6]){0,0,0,1,0,0}, 6) == 0)
            {
                //shift left from register
                strcpy(inst_decode.operation , "slr");
                inst_decode.op1 = registerArray[binToInt(inst_decode.rs,5)];
                inst_decode.op2 = registerArray[binToInt(inst_decode.rt,5)];
            }
            else if(compare(inst_decode.opcode, (int[6]){0,1,1,0,0,0}, 6) == 0)
            {
                // mul
                if(compare(inst_decode.rd, (int[5]){0,0,0,0,0} , 5) != 0)
                {
                    char* chakshu = malloc(100);
                    char* simmi = malloc(100);
                    chakshu = "Invalid instruction no. - ";
                    snprintf(simmi,100,"%d",inst_decode.instNum);
                    strcat(chakshu, simmi);
                    yyerror(chakshu);
                    exit(EXIT_FAILURE);
                }
                strcpy(inst_decode.operation , "mul");
                inst_decode.op1 = registerArray[binToInt(inst_decode.rs,5)];
                inst_decode.op2 = registerArray[binToInt(inst_decode.rt,5)];
            }
            else if(compare(inst_decode.type, (int[6]){0,0,1,0,0,0}, 6) == 0)
            {
                // add immediate
                strcpy(inst_decode.operation , "addi");
                inst_decode.op1 = registerArray[ binToInt(inst_decode.rs,5) ];
                inst_decode.immi =  binToInt(inst_execute.imm , 16);
            }
            else if(compare(inst_decode.type, (int[6]){1,0,0,1,0,0}, 6) == 0)
            {
                // lui or load immediate
                strcpy(inst_decode.operation , "lui");
                inst_decode.immi = binToInt(inst_execute.imm , 16);
            }
            else if(compare(inst_decode.type, (int[6]){0,0,1,1,0,1} , 6) == 0)
            {
                //or immediate
                strcpy(inst_decode.operation , "ori");
                inst_decode.op1 = registerArray[binToInt(inst_execute.rs,5)];
            }
            else if(compare(inst_decode.type, (int[6]){0,0,1,0,1,0}, 6) == 0)
            {
                //set on less than immediate
                strcpy(inst_decode.operation , "sli");
                inst_decode.op1 = registerArray[binToInt(inst_decode.rs,5)];
                inst_decode.immi = binToInt(inst_decode.imm,16);
            }
            else if(compare(inst_decode.type, (int[6]){0,0,0,1,0,0}, 6) == 0)
            {
                //branch on equal
                strcpy(inst_decode.operation , "beq");
                inst_decode.op1 = registerArray[binToInt(inst_decode.rs,5)];
                inst_decode.op2 = registerArray[binToInt(inst_decode.rt,5)];
            }
            else if((compare(inst_decode.type, (int[6]){0,0,0,0,0,1}, 6) == 0)&&(compare(inst_decode.rt, (int[5]){0,0,0,0,1}, 5) == 0))
            {
                //branch on >=0
               strcpy(inst_decode.operation , "bge");
               inst_decode.op1 = registerArray[binToInt(inst_decode.rs,5)];
               inst_decode.immi = binToInt(inst_decode.imm,16);
            }
            else if((compare(inst_decode.type, (int[6]){0,0,0,1,1,1},6) == 0)&&(compare(inst_decode.rt, (int[5]){0,0,0,0,0}, 5) == 0))
            {
                //branch on >0
                strcpy(inst_decode.operation , "bgt");
                inst_decode.op1 = registerArray[binToInt(inst_decode.rs,5)];
                inst_decode.immi = binToInt(inst_decode.imm,16);
            }
            else if((compare(inst_decode.type, (int[6]){0,0,0,1,1,0}, 6) == 0)&&(compare(inst_decode.rt, (int[5]){0,0,0,0,0}, 5) == 0))
            {
                //branch on <=0
                strcpy(inst_decode.operation , "ble");
                inst_decode.op1 = registerArray[binToInt(inst_decode.rs,5)];
               inst_decode.immi = binToInt(inst_decode.imm,16);
            }
            else if((compare(inst_decode.type, (int[6]){0,0,0,0,0,1}, 6) == 0)&&(compare(inst_decode.rt, (int[5]){0,0,0,0,0}, 5) == 0))
            {
                //branch on <0
                strcpy(inst_decode.operation , "blt");
                inst_decode.op1 = registerArray[binToInt(inst_decode.rs,5)];
                inst_decode.immi = binToInt(inst_decode.imm,16);
            }
            else if((compare(inst_decode.type, (int[6]){1,0,0,0,0,0}, 6) == 0)||(compare(inst_decode.type, (int[6]){1,0,0,0,1,1}, 6) == 0)||
                    (compare(inst_decode.type, (int[6]){1,0,1,0,0,0}, 6) == 0)||(compare(inst_decode.type, (int[6]){1,0,1,0,1,1}, 6) == 0))
            {
                //load byte
                strcpy(inst_decode.operation , "lors");
                inst_decode.op1 = registerArray[binToInt(inst_execute.rs,5)];
                
            }
            decToExe = inst_decode;
            decode_ctrl = 0;
        }
    }
}

void* execute()
{
    while(true)
    {
        if(execute_ctrl == 1)
        {  
            if(strcmp(inst_execute.operation, "slsa")==0)                 //shift left immediate using sa
            {
                inst_execute.result = binToInt(shiftLeft( intToBin(inst_execute.op1), inst_execute.shiftAmt) , 32 );  
            }
            if(strcmp(inst_execute.operation, "add")==0)
            {
                //  add
                inst_execute.result = inst_execute.op1 + inst_execute.op2 ;
            }
            else if(strcmp(inst_execute.operation, "and")==0)
            {
                // and
                inst_execute.result = binToInt( andBits( intToBin(inst_execute.op1), intToBin(inst_execute.op2)),32 );
            }
            else if (strcmp(inst_execute.operation, "nor")==0)
            {
                // nor
                inst_execute.result = binToInt( norBits(intToBin(inst_execute.op1), intToBin(inst_execute.op2)),32 );
            }
            else if(strcmp(inst_execute.operation, "or")==0)
            {
                //or
                inst_execute.result = binToInt(orBits(intToBin(inst_execute.op1), intToBin(inst_execute.op1)),32 );
            }
            else if(strcmp(inst_execute.operation, "slu")==0)
            {
                //set on less than unsigned
                if(inst_execute.op1<inst_execute.op2)
                {
                    inst_execute.result = 1;
                }
                else inst_execute.result = 0;
            }
            else if(strcmp(inst_execute.operation, "sub")==0)
            {
                // sub
                 inst_execute.result = inst_execute.op1 - inst_execute.op2 ;
            }
            else if(strcmp(inst_execute.operation, "slr")==0)
            {
                //shift left from register
                inst_execute.result = binToInt( shiftLeft(intToBin(inst_execute.op2), inst_execute.op1), 32  );
            }
            else if(strcmp(inst_execute.operation, "mul")==0)
            {
                // mul
                 inst_execute.result = inst_execute.op1 * inst_execute.op2 ;
            }
            else if(strcmp(inst_execute.operation, "addi")==0)
            {
                // add immediate
                inst_execute.result =  inst_execute.op1 + inst_execute.immi;
            }
            else if(strcmp(inst_execute.operation, "lui")==0)
            {
                // lui or load immediate
                inst_execute.result = binToInt(shiftLeft(intToBin(inst_execute.immi), 16), 16);
            }
            else if(strcmp(inst_execute.operation, "ori") == 0)
            {
                //or immediate
                inst_execute.result = binToInt(orBits(intToBin(inst_execute.op1),  extend(inst_execute.imm,16,32)), 5);
            }
            else if(strcmp(inst_execute.operation, "sli") == 0)
            {
                //set on less than immediate
                if((inst_execute.op1 < inst_execute.immi))
                {
                    inst_execute.result = 1;
                }
                else inst_execute.result = 0;
            }
            else if(strcmp(inst_execute.operation, "beq") == 0)
            {
                //branch on equal
                if(inst_execute.op1 == inst_execute.op2)
                {
                    pc = inst_execute.instNum + binToInt(inst_execute.imm,16) + 1; 
                }
            }
            else if(strcmp(inst_execute.operation, "bge") == 0)
            {
                //branch on >=0
                if(inst_execute.op1 >=0 )
                {
                    pc = inst_execute.instNum + inst_execute.immi + 1; 
                }
            }
            else if(strcmp(inst_execute.operation, "bgt"))
            {
                //branch on >0
                if(inst_execute.op1 >0 )
                {
                    pc = inst_execute.instNum + inst_execute.immi + 1; 
                }
            }
            else if(strcmp(inst_execute.operation, "ble"))
            {
                //branch on <=0
                if(inst_execute.op1 <=0 )
                {
                    pc = inst_execute.instNum + inst_execute.immi + 1; 
                }
            }
            else if(strcmp(inst_execute.operation, "blt"))
            {
                //branch on <0
                if(inst_execute.op1 <0 )
                {
                    pc = inst_execute.instNum + inst_execute.immi + 1; 
                }
            }
            else if(strcmp(inst_execute.operation, "lors"))
            {
                //load byte
                int i;
                int *temp = intToBin( binToInt(inst_execute.offset, 16) + inst_execute.op1);
                for(i=0;i<16;i++)
                {
                    inst_execute.offset[i] = temp[i];
                }
            }
            else
            {
                //error
                char* chakshu = malloc(100);
                char* simmi = malloc(100);
                chakshu = "Invalid instruction no. - ";
                snprintf(simmi,100,"%d",inst_execute.instNum);
                strcat(chakshu, simmi);
                yyerror(chakshu);
                exit(EXIT_FAILURE);
            }
            inst_execute =  exeToMem;
            execute_ctrl = 0;
        }   
    }   
}   

void* mem()
{
    while(true)
    {
        if(memory_ctrl == 1)
        {
            if(compare(inst_execute.type, (int[6]){1,0,0,0,0,0}, 6) == 0)
            {
                inst_memory.byte = memory[binToInt(inst_memory.offset, 16)];
            }
            if(compare(inst_execute.type, (int[6]){1,0,0,0,1,1}, 6) == 0)
            {
                inst_memory.word[0] = memory[binToInt(inst_memory.offset, 16)];
                inst_memory.word[1] = memory[binToInt(inst_memory.offset, 16)+1];
                inst_memory.word[2] = memory[binToInt(inst_memory.offset, 16)+2];
                inst_memory.word[3] = memory[binToInt(inst_memory.offset, 16)+3];
            }
            else if( (compare(inst_execute.type, (int[6]){1,0,1,0,0,0}, 6) == 0)||(compare(inst_execute.type, (int[6]){1,0,1,0,1,1}, 6) == 0))
            {
                memory[binToInt(inst_memory.offset, 16)] = registerArray[binToInt(inst_writeback.rt, 5)];
            }
            memToWb = inst_memory;
            memory_ctrl = 0;
        }
    }
}

void* writeback()
{
    while(true)
    {
        if(wb_ctrl == 1)
        {
            if(inst_writeback.isoffset)
            {
                if((compare(inst_execute.type, (int[6]){1,0,0,0,1,1}, 6) == 0))
                {
                    registerArray[binToInt(inst_writeback.rt, 5)] = inst_writeback.word[0];
                    registerArray[binToInt(inst_writeback.rt, 5)+1] = inst_writeback.word[1];
                    registerArray[binToInt(inst_writeback.rt, 5) +2] = inst_writeback.word[2];
                    registerArray[binToInt(inst_writeback.rt, 5) +3] = inst_writeback.word[3];
                }
                else
                     registerArray[binToInt(inst_writeback.rt, 5)] = inst_writeback.byte;
            }
            else if(inst_writeback.isImm)
            {
                registerArray[binToInt(inst_writeback.rt, 5)] = inst_writeback.result;
            }
            else
            {
                if(compare(inst_writeback.opcode, (int[7]){0,1,1,0,0,0,0}, 7)==0)
                {
                    int *temp = longToBin(inst_writeback.mult);
                    registerArray[32] = binToInt(temp , 32);
                    registerArray[33] = binToInt(temp+32 , 32);
                }
                else
                {
                    registerArray[binToInt(inst_writeback.rd, 5)] = inst_writeback.result;
                }
            }
            memory_ctrl = 0;
        }
    }
}

int flag1=0, flag2=0, flag3=0, flag4=0;

void* display()
{
    while(true)
    {
        if(display_ctrl == 1)
        {
            create_svg(flag1, flag2, flag3, flag4);
            display_ctrl = 0;
            flag1 = 0;
            flag2 = 0;
            flag3 = 0;
            flag4 = 0;
        }
    }
}


void start()
{
    char next[100];
    pthread_create(&(peer[0]), NULL, (void *)fetch, NULL);
    pthread_create(&(peer[1]), NULL, (void *)decode, NULL);
    pthread_create(&(peer[2]), NULL, (void *)execute, NULL);
    pthread_create(&(peer[3]), NULL, (void *)mem, NULL);
    pthread_create(&(peer[4]), NULL, (void *)writeback, NULL);
    pthread_create(&(peer[5]), NULL, (void *)display, NULL);
    while(pc<=lines+4)
    {
        if(fetch_ctrl == 0 && decode_ctrl == 0 && execute_ctrl == 0 && memory_ctrl == 0 && wb_ctrl == 0 && display_ctrl == 0)
        {
            int x = pc;
            if(fetchToDec != NULL)
                inst_decode = bin_to_struct(fetchToDec);
            inst_execute = decToExe;
            inst_memory = exeToMem;
            inst_writeback = memToWb;
            if(x<=lines)
               fetch_ctrl = 1;
            else 
                pc++;
            if((x<=lines+1)&&(x>=1))
            {
                flag1 = 1;
               decode_ctrl = 1;
            }
            if((x<=lines+2)&&(x>=2))
            {
                flag2 = 1; 
                execute_ctrl = 1;
            }
            if((x<=lines+3)&&(x>=3))
            {
                flag3 = 1;
                memory_ctrl = 1;
            }
            if((x<=lines+4)&&(x>=4))
            {
                flag4 = 1;
                wb_ctrl = 1;
            }
            display_ctrl = 1;
            
            Label: printf("Shell>>");
            scanf("%s", next);
            if(strcmp(next, "step")!=0)
            {
            //     int i=0;
            //     char chara[100];
            //     while(next[i] != "\0" && next[i] != " ")
            //     {
            //         chara[i] = next[i];
            //         i++;
            //     }
            //     if(strcmp(next, "regdump")!=0)
            //     {
            //         int i;
            //         for(i=0;i<32;i++)
            //         {
            //             printf("$%d%d:", i/10, i%10);
            //             char* ch = intToHex(registerArray[i]);
            //             printf(" %s\n", ch);
            //         }
            //     }
            //     else if(strcmp(chara, "memdump")!=0)
            //     {
            //         i++;
            //         while(next[i] != "\0" && next[i] != " ")
            //         {
            //             chara[i] = next[i];
            //             i++;
            //         }
            //         int ii = hexToInt(chara);
            //         i++;
            //         while(next[i] != "\0" && next[i] != " ")
            //         {
            //             chara[i] = next[i];
            //             i++;
            //         }
            //         int xyz = atoi(chara);
            //         for(i=ii; i<ii+xyz; i++)
            //         {
            //             char* ch = intToHex(memory[i]);
            //             printf("%s: %s", intToHex(i), ch);
            //         }
            //     }
            //     else
                     printf("You have commited a crime. Please try again.\n");
                 goto Label;
            }
        }
    }
}