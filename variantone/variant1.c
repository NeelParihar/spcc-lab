#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node
{
    char symb[20];
    int pos, addr, val, len;
    struct node *next;
} NODE;

int cnt = 0;

NODE *first, *last;

void add_symb(char *s, int a, int v, int l) //ONE 0 0 0	//A 0 0 0 //B 0 0 0	//C 0 0 0
{
    NODE *p;

    p = (NODE *)malloc(sizeof(NODE)); //1000 //2000 //3000 //4000
    strcpy(p->symb, s);
    cnt++; //1 //2 //3	//4
    p->pos = cnt;
    p->addr = a;
    p->val = v;
    p->len = l;
    p->next = NULL;

    if (first == NULL)
        first = p;
    else
        last->next = p;

    last = p;
}

void display_symbtab()
{
    NODE *p;

    printf("#\tsymb\taddr\tval\tlen\n");

    p = first; //1000
    while (p != NULL)
    {
        printf("%d\t%s\t%d\t%d\t%d\n",
               p->pos, p->symb, p->addr, p->val, p->len);

        p = p->next; //2000	//3000 //4000	//NUll
    }
}

NODE *search_symb(char *s) //t3
{
    NODE *p;

    p = first;
    while (p != NULL)
    {
        if (strcmp(p->symb, s) == 0)
            return p;

        p = p->next;
    }

    return NULL;
}

char optab[][6] = {"STOP", "ADD", "SUB", "MULT",
                   "MOVER", "MOVEM", "COMP", "BC",
                   "DIV", "READ", "PRINT"};

int search_op(char *s)
{
    int i;

    for (i = 0; i < 11; i++)
    {
        if (strcmp(optab[i], s) == 0)
            return i; //5
    }

    return -1;
}

char regtab[][5] = {"AREG", "BREG", "CREG", "DREG"};

int search_reg(char *s)
{
    int i;
    for (i = 0; i < 4; i++)
    {
        if (strcmp(regtab[i], s) == 0)
            return i; //1
    }

    return -1;
}

char adtab[][7] = {"START", "END",
                   "ORIGIN", "EQU", "LTORG"};

int search_ad(char *s)
{
    int i;
    for (i = 0; i < 5; i++)
    {
        if (strcmp(adtab[i], s) == 0)
            return i; //0
    }

    return -1;
}

int main()
{
    NODE *p;
    FILE *fp, *fp1;
    char fname[40], buff[80], t1[20], t2[20], t3[20], t4[20];
    int n, i, j, pc = 0; //START 100

    printf("Enter source file name:");
    scanf("%s", fname);

    fp = fopen(fname, "r");
    if (fp == NULL)
    {
        printf("File %s not found.\n", fname);
        exit(1);
    }

    fp1 = fopen("temp.i", "w");

    while (fgets(buff, 80, fp) != NULL)
    {
        n = sscanf(buff, "%s %s %s %s", t1, t2, t3, t4);
        //START 100 //PRINT A
        switch (n)
        {
        case 1:
            i = search_op(t1);
            if (i == 0) // STOP
            {
                fprintf(fp1, "%03d) (IS, %02d)\n", pc, i);
                break;
            }
            i = search_ad(t1);
            fprintf(fp1, "%03d) (AD, %02d)\n", pc, i + 1);
            break;
        case 2:
            i = search_ad(t1); //i=0
            if (i == 0)        // START
            {
                fprintf(fp1, "%03d) (AD, %02d) (C, %s)\n", pc, i + 1, t2);
                pc = atoi(t2) - 1; //3-digit 0-prefix =>001
                break;
            }
            i = search_op(t1);
            if (i == 9 || i == 10) // READ or PRINT
            {
                p = search_symb(t2); //2000

                if (p == NULL)
                {
                    add_symb(t2, 0, 0, 0);
                    fprintf(fp1, "%03d) (IS, %02d) (S, %d)\n", pc, i, cnt);
                }
                else
                {
                    fprintf(fp1, "%03d) (IS, %02d) (S, %d)\n", pc, i, p->pos);
                }
                break;
            }
            p = search_symb(t1);
            if (p == NULL)
            {
                add_symb(t1, pc, 0, 0);
            }
            else
            {
                p->addr = pc;
            }
            fprintf(fp1, "%03d) (IS, 00)\n", pc);
            break;
        case 3:
            if (strcmp(t2, "DS") == 0) //A DS 1 //B DS 2 //C DS 1
            {
                p = search_symb(t1); //p=2000	//3000 //4000
                if (p == NULL)
                {
                    add_symb(t1, pc, 0, atoi(t3));
                }
                else
                {
                    p->addr = pc;      //108	//109	//111
                    p->len = atoi(t3); //1	//2	//
                }
                fprintf(fp1, "%03d) (DL, 02) (C, %s)\n", pc, t3);
                pc += atoi(t3) - 1; //109+2-1=110
                break;
            }
            if (strcmp(t2, "DC") == 0) //ONE DC '1'
            {
                p = search_symb(t1);
                if (p == NULL)
                {
                    add_symb(t1, pc, atoi(t3 + 1), 1);
                }
                else
                {
                    p->addr = pc;
                    p->val = atoi(t3 + 1);
                    p->len = 1;
                }
                fprintf(fp1, "%03d) (DL, 01) (C, %d)\n", pc, atoi(t3 + 1));
                break;
            }
            i = search_op(t1);    //i=5
            if (i >= 1 && i <= 8) // ADD to DIV
            {
                t2[strlen(t2) - 1] = '\0';
                j = search_reg(t2); //j=1

                p = search_symb(t3); //

                if (p == NULL) //T
                {
                    add_symb(t3, 0, 0, 0); //ONE 0 0 0	//A 0 0 0
                    fprintf(fp1, "%03d) (IS, %02d) (%d) (S, %d)\n", pc, i, j + 1, cnt);
                }
                else
                {
                    fprintf(fp1, "%03d) (IS, %02d) (%d) (S, %d)\n", pc, i, j + 1, p->pos);
                }
                break;
            }
            i = search_op(t2);
            p = search_symb(t1);
            if (p == NULL)
            {
                add_symb(t1, pc, 0, 0);
            }
            else
            {
                p->addr = pc;
            }
            p = search_symb(t3);
            if (p == NULL)
            {
                add_symb(t3, 0, 0, 0);
                fprintf(fp1, "%03d) (IS, %02d) (S, %d)\n", pc, i, cnt);
            }
            else
            {
                fprintf(fp1, "%03d) (IS, %02d) (S, %d)\n", pc, i, p->pos);
            }
            break;
        case 4:
            i = search_op(t2);
            t3[strlen(t3) - 1] = '\0';
            j = search_reg(t3);
            p = search_symb(t1);
            if (p == NULL)
            {
                add_symb(t1, pc, 0, 0);
            }
            else
            {
                p->addr = pc;
            }
            p = search_symb(t4);
            if (p == NULL)
            {
                add_symb(t4, 0, 0, 0);
                fprintf(fp1, "%03d) (IS, %02d) (%d) (S, %d)\n", pc, i, j + 1, cnt);
            }
            else
            {
                fprintf(fp1, "%03d) (IS, %02d) (%d) (S, %d)\n", pc, i, j + 1, p->pos);
            }
        }
        pc++; //108=>109	//111
    }

    fclose(fp);
    fclose(fp1);

    fp1 = fopen("temp.i", "r");
    // while (fgets(buff, 80, fp1) != NULL)
    //     printf("%s", buff);

    display_symbtab();

    return 0;
}