#include	<stdlib.h>
#include	<stdio.h>
//	printfs	are	for	debugging;	remove	them	when	you	use/submit	your	library
int	mem_init	(void	*chunkpointer,	int	chunksize,	int	method)
{
    printf("init called\n");
    return	(0); //	if	success
}
void	*mem_allocate	(int	objectsize)
{
    printf("alloc	 called\n");
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