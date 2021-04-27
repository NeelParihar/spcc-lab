//Program Name:-
/*Implement a menu driven simulator for hypothetical Simple Instruction Computer that provides
the following functionalities:
a. Load – Loading of the program from file into memory
b. Print – Printing the program loaded in memory.
c. Run – Executing the loaded program
The machine has the basic instruction set comprising of following:

Mnemonic      Opcode                    Meaning

STOP		00  	  Stop the execution.

ADD		01  	  Add the memory operand to register operand.

SUB		02  	  Subtract the memory operand to register operand.

MULT	       03  	  Multiply memory operand to register operand.

MOVER 		04  	  Move the memory operand content to register.

MOVEM		05  	  Move the register content to memory operand.

COMP	       06  	  Compare Register and memory operand to set appropriate condition code.

BC	     	07  	  Branch to second operator depending on condition code specified as first operand.

DIV		08  	  Divide memory operand to register operand.

READ		09  	  Read into memory operand.

PRINT         10  	  Print the content of memory operand.

Create a file containing the machine language code to find the addition of first “n” numbers by
using above instruction opcodes.

*/

#include <stdio.h>
#include <stdlib.h>

int printoption();
int print();
int run();
int load();                                                        //ccode[1]=LT,ccode[2]=LE
int operation, op1, op2, opcode, ccode[7] = {0, 0, 0, 0, 0, 0, 1}; //1=LT,2=LE,3=EQ,4=GT,5=GE,6=ANY(1)
int reg[4], pc;
char s;
int lc = 0, address;
long int content, mem[1000];
int n, i;

int main()
{
z: //(while(1))
    printoption();
    printf("\n choose the number::");
    scanf("%d", &n);
    switch (n)
    {
    case 1:
        load();
        break;
    case 2:
        print();
        break;
    case 3:
        run();
        break;
    case 4:
        exit(0);
    }
    goto z;
}

int printoption()
{
    printf("\n1.LOAD\n");
    printf("2.PRINT\n");
    printf("3.RUN\n");
    printf("4.EXIT");
}

int load()
{
    FILE *fp;
    char fname[30];
    lc = 0;
    printf("enter the file name:");
    scanf("%s", fname);
    fp = fopen(fname, "r"); // r- read, w-write
    if (fp == NULL)
    {
        printf("unable to open");
        return (0);
    }
    else
    {
        while (!feof(fp)) //file end of file
        {

            fscanf(fp, "%d %ld", &address, &content); //100  090007 //101 090008
            if (address == -1)
                pc = content;
            else
            {
                if (lc == 0)
                {
                    pc = address;
                }
                lc = address;
                mem[address] = content;
            }

            lc++; //lc=100,101,102,103,104,105,106,107,108
        }         //end of while
        printf("\nfile loaded successfully");
        fclose(fp);
    }
}

int print()
{
    int i; //100<108//101<108
    for (i = pc; i < lc; i++)
    {
        printf("\n %d \t %ld", i, mem[i]);
    }
}

int run()
{

    while (1)
    {
        opcode = mem[pc];           //     090007
        operation = opcode / 10000; //  09=> 9

        op1 = opcode % 10000 / 1000; //  0
        op2 = opcode % 10000 % 1000; //  007 =>7

        switch (operation)
        {
        case 0:

            return 0;

            break;

        case 1:
            reg[op1] += mem[op2]; //reg[op1]=reg[op1]+mem[op2]	//ADD
            pc++;
            break;
        case 2:
            reg[op1] -= mem[op2]; //SUB
            pc++;
            break;
        case 3:
            reg[op1] *= mem[op2]; //MULT
            pc++;
            break;
        case 4:
            reg[op1] = mem[op2]; //MOVER
            pc++;
            break;
        case 5:
            mem[op2] = reg[op1]; //MOVEM
            pc++;
            break;
        case 6: //COMP
            if (reg[op1] < mem[op2])
                ccode[1] = 1;
            else
                ccode[1] = 0;

            if (reg[op1] <= mem[op2])
                ccode[2] = 1;
            else
                ccode[2] = 0;

            if (reg[op1] == mem[op2])
                ccode[3] = 1;
            else
                ccode[3] = 0;

            if (reg[op1] > mem[op2])
                ccode[4] = 1;
            else
                ccode[4] = 0;

            if (reg[op1] >= mem[op2])
                ccode[5] = 1;
            else
                ccode[5] = 0;

            pc++;
            break;
        case 7:
            if (ccode[op1] == 1) //BC
                pc = op2;
            else
                pc++;
            break;
        case 8:
            reg[op1] /= mem[op2]; //DIV
            pc++;
            break;
        case 9:
            printf("\nEnter Number "); //READ
            scanf("%ld", &mem[op2]);
            pc++;
            break;
        case 10:
            printf("\nAns is %ld", mem[op2]); //PRINT
            printf("\nCCODE is");
            for (int i = 0; i < 7; i++)
                printf("\t%d", ccode[i]);
            pc++;
            break;
        }
    }
}