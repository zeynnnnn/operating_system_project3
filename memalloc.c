/*
	Zeynep Sonkaya
	Gökçe Sefa
	CS 342
	Project 3
	memalloc.c
 */
#include	<stdlib.h>
#include	<stdio.h>
//	printfs	are	for	debugging;	remove	them	when	you	use/submit	your	library
int	mem_init	(void	*chunkpointer,	int	chunksize,	int	method)
{
    printf("init called\n");
    if (( chunksize < 32*1024 )|| (chunksize> 32*1024*1024))
        return -1;


    return	(0); //	if	success
}
void	*mem_allocate	(int	objectsize)
{
    printf("alloc	 called\n");
    if (( objectsize < 128)|| (objectsize> 1024*2048))
        return (NULL); //	if	not	success

    return	(NULL); //	if	not	success
}
void	mem_free(void	*objectptr)
{
    printf("free	called\n");
    return;
}

void	mem_print	(void)
{
    printf("print	called\n");
    return;
}