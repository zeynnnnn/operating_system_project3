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
#include <pthread.h>
#include	"memalloc.h"
#include <time.h>
void *memTest(void* j){
    void	*x1,	*x2,	*x3,*x4,*x5,*x6; //	object	pointers
//	below	we	allocate	and	deallocate	memory	dynamically

   mem_print();
    x1	=	mem_allocate(1012);
    mem_print();
 //   printf("Returned x1 :%lx",(unsigned long)x1);
    x2	=	mem_allocate(4500);
    mem_print();
    mem_free(x1);

    mem_print();
    x5 = mem_allocate(1000);
//    printf("Returned x2 :%lx",(unsigned long)x2);
    mem_print();
   mem_free(x2);
//   mem_print();

    x6	=	mem_allocate(500);
    //mem_print();
    mem_free(x5);
   // mem_print();

    x3	=	mem_allocate(1300);
   // mem_print();
  //  printf("Returned x3 :%lx",(unsigned long)x3);
    mem_free(x3);
//    mem_print();
    x4	=	mem_allocate(500);
  //  printf("Returned x4 :%lx",(unsigned long)x4);
 //   mem_print();
    mem_free(x6);
 //   mem_print();
    mem_free(x4);
 //   mem_print();
/*
    mem_free(x1);
    mem_print();
    mem_free(x2);
    mem_print();
    mem_free(x4);
    mem_print();*/
}
int	main(int	argc,	char	*argv[]) {
    void *chunkptr;
    void *endptr;
    char *charptr;
    int ret;
    int i;
    int size;

    if (argc != 2) {
        printf("usage:	app	<size	in	KB>\n");
        exit(1);
    }
    size = atoi(argv[1]);            //	unit	is	in	KB
//	allocate	a	chunk
    chunkptr = sbrk(0);    //	end	of	data	segment
    sbrk(size * 1024);    //	extend	data	segment	by	indicated	amount	(bytes)
    endptr = sbrk(0); //	new	end	of	data	segment
    printf("chunkstart=%lx,	chunkend=%lx,	chunksize=%lu	bytes\n",
           (unsigned long) chunkptr,
           (unsigned long) endptr, (unsigned long) (endptr - chunkptr));
//test	the	chunk
    printf("---starting	testing	chunk\n");
    charptr = (char *) chunkptr;
    for (i = 0; i < size; ++i)
        charptr[i] = 0;
    printf("---chunk	test	ended	- success\n");
    ret = mem_init(chunkptr, size, BEST_FIT);
    if (ret == -1) {
        printf("could	not	initialize	\n");
        exit(1);

    }
    pthread_t thread1[100];
    int msec = 0;

    clock_t before;
    before = clock();
    for (int i = 0; i < 1; ++i) {
        pthread_create(&thread1[i], NULL, &memTest, NULL);
    }

    //parent thread
    for (int i = 0; i < 1; ++i)
        pthread_join(thread1[i], NULL);

    clock_t difference = clock() - before;

    msec = difference * 1000000 / CLOCKS_PER_SEC;
    printf("Time taken %d seconds %d milliseconds %d microsecond\n",
           msec/1000000, msec/1000,msec%1000);
    return 0;
}