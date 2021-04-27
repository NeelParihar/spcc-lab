/* Program Name:-	
Write an assembler that will display the following errors and warnings:
a) Symbol used but not defined
b) Symbol defined but not used
c) Re-declaration of the Symbol
Present in the following assembly program:

	START 	100
	READ 	X
Y	MOVER	BREG, X
	ADD 	BREG, X
X	MOVEM	AREG, Z
	STOP
X 	DS	1
Y	DS	1
	END
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct node
{
	char symb[20];
	int decl, used;
	struct node *next;
} NODE;

NODE *first, *last;

void add_symb1(char *s) //Declared	//Y
{
	NODE *p;

	p = (NODE *)malloc(sizeof(NODE)); //2000
	strcpy(p->symb, s);
	p->decl = 1;
	p->used = 0;
	p->next = NULL;

	if (first == NULL)
		first = p;
	else
		last->next = p;

	last = p;
}

void add_symb2(char *s) //Used Symbol //X	//Z
{
	NODE *p;

	p = (NODE *)malloc(sizeof(NODE)); //1000 //3000
	strcpy(p->symb, s);				  //Z
	p->decl = 0;					  //
	p->used = 1;
	p->next = NULL;

	if (first == NULL)
		first = p;
	else
		last->next = p;

	last = p;
}

NODE *search(char *s) //X	//Y 	//X	//X	//Z	//X	//Y
{
	NODE *p;

	p = first;		  //1000
	while (p != NULL) //T//F		//T	//T
	{
		if (strcmp(p->symb, s) == 0)
			return p; //1000 //1000	//2000

		p = p->next; //Null	//2000	//Null	//2000
	}

	return NULL;
}

char optab[][6] = {"STOP", "ADD", "SUB", "MULT",
				   "MOVER", "MOVEM", "COMP", "BC",
				   "DIV", "READ", "PRINT"};

int search_op(char *s) //READ	//X
{
	int i;
	for (i = 0; i < 11; i++) //0-10 //i=09
		if (strcmp(optab[i], s) == 0)
			return i; //9	//1

	return -1;
}

int main()
{
	NODE *p;
	FILE *fp;
	char buff[80], t1[20], t2[20],
		t3[20], t4[20], fname[40];
	int n, i;

	printf("Enter source file name:");
	scanf("%s", fname);

	fp = fopen(fname, "r");
	if (fp == NULL)
	{
		printf("File %s not found.\n", fname);
		exit(1);
	}

	while (fgets(buff, 80, fp) != NULL)
	{
		n = sscanf(buff, "%s %s %s %s", t1, t2, t3, t4); //START 100 // READ X

		switch (n) //X	MOVEM	AREG, Z 	//ADD 	BREG, X   //Y DS 1
		{
		case 2:
			if (strcmp(t1, "START") == 0)
				break;

			i = search_op(t1);	   //READ //i=9
			if (i == 9 || i == 10) // READ or PRINT
			{
				p = search(t2); //x= check	=>NULL
				if (p == NULL)
					add_symb2(t2); //X
				else
					p->used = 1;

				break;
			}

			p = search(t1);
			if (p == NULL)
				add_symb1(t1);
			else
				p->decl++;

			break;
		case 3:
			i = search_op(t1);	  //ADD	//i=1	//X	//i=-1	//Y//	i=-1
			if (i >= 1 && i <= 8) // ADD to DIV
			{
				p = search(t3); //X	//p=1000
				if (p == NULL)
					add_symb2(t3);
				else
					p->used = 1; //1

				break;
			}

			if (strcmp(t2, "DS") == 0 || //t2=DS	//t2=DS
				strcmp(t2, "DC") == 0)
			{
				p = search(t1); //X	//p=1000	//Y	//p=2000
				if (p == NULL)	//False //False
					add_symb1(t1);
				else
					p->decl++; //X=2	//Y=2
				break;
			}

			p = search(t1);
			if (p == NULL)
				add_symb1(t1);
			else
				p->decl++;

			p = search(t3);
			if (p == NULL)
				add_symb2(t3);
			else
				p->used = 1;

			break;
		case 4:
			p = search(t1); //Y	//p=NULL	//X	//p=1000
			if (p == NULL)
				add_symb1(t1); //Y
			else
				p->decl++; //X=1

			p = search(t4); //X	//p=1000	//t4=Z	//p=Null
			if (p == NULL)
				add_symb2(t4); //Z
			else
				p->used = 1;
		} // switch
	}	  // while

	fclose(fp);

	p = first;
	while (p != NULL)
	{
		if (p->decl != 0 && p->used == 0)
			printf("Symb %s declared but not used.\n", p->symb);
		if (p->used == 1 && p->decl == 0)
			printf("Symb %s used but not declared.\n", p->symb);
		if (p->decl > 1)
			printf("Symb %s redeclared.\n", p->symb);

		p = p->next;
	}

	return 0;
}