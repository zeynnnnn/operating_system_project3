/*
	Zeynep Sonkaya
	Gökçe Sefa
	CS 342
	Project 3
	app.c
 */
#include	<stdlib.h>
#include	<stdio.h>
#include	<unistd.h>
#include	"memalloc.h"
int	main(int	argc,	char	*argv[])
{
    void	*chunkptr;
    void	*endptr;
    char	*charptr;
    int	ret;
    int	i;
    int	size;
    void	*x1,	*x2,	*x3,*x4; //	object	pointers
    if	(argc	!=	2)	{
        printf("usage:	app	<size	in	KB>\n");
        exit(1);
    }
    size	=	atoi(argv[1]);			//	unit	is	in	KB
//	allocate	a	chunk
    chunkptr	=	sbrk(0);	//	end	of	data	segment
    sbrk(size	*	1024);	//	extend	data	segment	by	indicated	amount	(bytes)
    endptr	=	sbrk(0); //	new	end	of	data	segment
    printf("chunkstart=%lx,	chunkend=%lx,	chunksize=%lu	bytes\n",
           (unsigned	long)chunkptr,
           (unsigned	long)endptr,	(unsigned	long)(endptr	- chunkptr));
//test	the	chunk
    printf("---starting	testing	chunk\n");
    charptr	=	(char	*)chunkptr;
    for	(i	=	0;	i	<	size;	++i)
        charptr[i]	=	0;
    printf("---chunk	test	ended	- success\n");
    ret	=	mem_init(chunkptr,	size,	BEST_FIT);
    if	(ret	==	-1)	{
        printf("could	not	initialize	\n");
        exit(1);

    }
//	below	we	allocate	and	deallocate	memory	dynamically
    x1	=	mem_allocate(600);
    mem_print();
    printf("Returned x1 :%lx",(unsigned long)x1);
    x2	=	mem_allocate(4500);
    printf("Returned x2 :%lx",(unsigned long)x2);
    mem_print();
    mem_free(x1);
    mem_print();
    x3	=	mem_allocate(1300);
    printf("Returned x3 :%lx",(unsigned long)x3);
    x4	=	mem_allocate(500);
    printf("Returned x4 :%lx",(unsigned long)x4);
    mem_free(x3);
    mem_print();

    mem_print();
    mem_free(x1);
    mem_print();
    mem_free(x2);
    mem_free(x3);
    mem_print();
    return	0;
}