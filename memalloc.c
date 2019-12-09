/*
	Zeynep Sonkaya
	Gökçe Sefa
	CS 342
	Project 3
	memalloc.c
 */

#include	<stdlib.h>
#include	<stdio.h>
#include "memalloc.h"
#include <math.h>
#include <string.h>
#define BLOCK_SIZE 128
void  *chunkpoint;
int lastId;
void* blocksPointer;
//	printfs	are	for	debugging;	remove	them	when	you	use/submit	your	library
int	mem_init	(void	*chunkpointer,	int	chunksize,	int	method)
{
    lastId=0;
    printf("init called\n");
    if ((method!=BEST_FIT)&&(method!=FIRST_FIT)&&(method!=WORST_FIT))
        return -1;

    if (( chunksize < 32 )||(chunksize> 32*1024))
        return -1;
    chunkpoint=chunkpointer;
   // long int*  ptr	=	(long int	*)chunkpointer;
   *(int*)chunkpointer = method;
  //  *chunkpoint =  method;
    void *p = (char*)chunkpointer+sizeof(int);
    int t =(chunksize*1024)-(2* sizeof(int))- ((int)floor((chunksize*1024)/BLOCK_SIZE));
    printf("Size::: %d \n",t);
   *(int*) p = t ;
printf("POinterSize:::  %d\n",*(int*)(p));

// Stores block id of the block allocated to a
    // process

  //   int allocation[(int)floor((chunksize*1024)/BLOCK_SIZE)];
// long * allocation=   chunkpoint[2] =&allocation;
    int k=0;
    // Initially no block is assigned to any process
   // memset(chunkpoint[2], -1, sizeof((int)floor((chunksize*1024)/BLOCK_SIZE)));
    for(k=0;k< (int)floor(t/BLOCK_SIZE) ;k++) //calculate according to left over space byte/byte
    {
        void *p = (char*)chunkpointer+sizeof(int)*(2+k);
        *(int*) p = -1;
    }
    blocksPointer = (char*)chunkpointer+sizeof(int)*(2+k);

    return	(0); //	if	success
}
void	*mem_allocate	(int	objectsize)
{
    lastId ++;
    void *p = (char*)chunkpoint+sizeof(int)*(1);
    printf("ChunkSize: %d ...\n",*(int*)p);
    printf("alloc	 called\n");
    printf("Memory:%d\n",*(int*)p);
    if (( objectsize < BLOCK_SIZE)|| (objectsize> 1024*2048))
        return (NULL); //	if	not	success
    int  n = (int)floor((*(int*)p )/BLOCK_SIZE);
    p =(char*)chunkpoint+sizeof(int)*(2);
    int *allocation = (int*) p;
    int blocksize=0;    int blockSize[n];
    int blockBase[n];
    int lastBlock=0;
    memset(blockBase, -1, sizeof(blockBase));
    memset(blockSize, -1, sizeof(blockSize));

    for (int k=0; k<n;k++){
        if (allocation[k]==-1)
        {
            if( blocksize==0)
                blockBase[lastBlock]=k;
            blocksize++;
        }
        else{
            if(blocksize>0){
                blockSize[lastBlock]=blocksize;
                blocksize=0;
                lastBlock++;
            }
        }

    }
    if(blocksize>0)
        blockSize[lastBlock]= blocksize;


    if (*(int*) (chunkpoint)== BEST_FIT)
    {
        // pick each process and find suitable blocks
        // according to its size ad assign to it
     printf("Found Best fit\n");
     int objectSizeAsBlock  = (int)floor(objectsize/BLOCK_SIZE);
            // Find the best fit block for current process
            int bestIdx = -1;
            for (int j=0; j< n; j++)
            {
                if (blockSize[j]==-1)
                    break;
                if (blockSize[j] >= objectSizeAsBlock)
                {
                    if (bestIdx == -1)
                        bestIdx = j;
                    else if (blockSize[bestIdx] > blockSize[j])
                        bestIdx = j;
                }
            }
            printf("BestIDx %d\n",bestIdx);
            // If we could find a block for current process
            if (bestIdx != -1)
            {
                for(int f=0;f<objectSizeAsBlock;f++)
                    allocation[  blockBase[bestIdx]+f]=lastId;
            }
            void *p = (char*)blocksPointer+sizeof(int)*(blockBase[bestIdx]);
        return p;

    } else if (*(int*)chunkpoint==FIRST_FIT)
    {
        printf("Found First fit\n");
        int objectSizeAsBlock  = (int)floor(objectsize/BLOCK_SIZE);
        // Find the best fit block for current process
        int bestIdx = -1;
        for (int j=0; j< n; j++)
        {
            if (blockSize[j]==-1)
                break;
            if (blockSize[j] >= objectSizeAsBlock)
            {
                bestIdx=j;
                for(int f=0;f<objectSizeAsBlock;f++)
                    allocation[  blockBase[bestIdx]+f]=lastId;
            }
        }
        printf("BestIDx %d\n",bestIdx);
        // If we could find a block for current process
        if (bestIdx == -1)
        {
            return NULL;
        }
        void *p = (char*)blocksPointer+sizeof(int)*(blockBase[bestIdx]);
        return p;

    }else if (*(int*)chunkpoint==WORST_FIT)
    {
        printf("Found Worst fit\n");
        int objectSizeAsBlock  = (int)floor(objectsize/BLOCK_SIZE);
        // Find the best fit block for current process
        int worstIdx = -1;
        for (int j=0; j< n; j++)
        {
            if (blockSize[j]==-1)
                break;
            if (blockSize[j] >= objectSizeAsBlock)
            {
                if (worstIdx == -1)
                    worstIdx = j;
                else if (blockSize[worstIdx] <blockSize[j])
                    worstIdx = j;
            }
        }
        printf("WorstID %d\n",worstIdx);
        // If we could find a block for current process
        if (worstIdx != -1)
        {
            for(int f=0;f<objectSizeAsBlock;f++)
                allocation[  blockBase[worstIdx]+f]=lastId;
        }
        void *p = (char*)blocksPointer+sizeof(int)*(blockBase[worstIdx]);
        return p;

    } else
        return NULL; //	if	not	success
}


void	mem_free(void	*objectptr)
{
    printf("free	called\n");
    void* p=  (char*)chunkpoint+sizeof(int)*(1);
    int  n = (int)floor((*(int*)p )/BLOCK_SIZE);
    p =  (char*)chunkpoint+sizeof(int)*(2);
    int *allocation = (int*) p;
    int blocksize=0;
    int blockSize[n];
    int blockBase[n];
    int lastBlock=0;

    memset(blockBase, -1, sizeof(blockBase));
    memset(blockSize, -1, sizeof(blockSize));


    for (int k=0; k<n;k++){
        int t =(int)allocation[k];
        printf("%d ",t);
        if (t!=-1)
        {
            if( blocksize==0)
                blockBase[lastBlock]=k;
            blocksize++;
        }
        else{
            if(blocksize>0){
                blockSize[lastBlock]=blocksize;
                blocksize=0;
                lastBlock++;
            }
        }

    }
    if(blocksize>0){
        blockSize[lastBlock]= blocksize;
        lastBlock++;
    }


    for (int i = 0; i < lastBlock; i++) {
        void* p= (char*)(blocksPointer)+sizeof(int)*(blockBase[i]);
        printf("\nCalcuLATED:%lx",(unsigned long)p);
        printf("\ngiven :%lx",(unsigned long)objectptr);
        if (objectptr==p){
            void* p= (char*)(chunkpoint+sizeof(int)*(2+blockBase[i])); //2 COMES FROM METHOD AND SIZE INTS
            int deletedId= *(int*)p;
            printf("\nDeletedId :%d\n",deletedId);
            int y=0;
            while(  allocation[blockBase[i]+y] == deletedId) {
                allocation[blockBase[i] + y] = -1;
                y++;
            }
            return;
        }
    }
}

void	mem_print	(void) {

   void* p=  (char*)chunkpoint+sizeof(int)*(1);
    int  n = (int)floor((*(int*)p )/BLOCK_SIZE);
    p =  (char*)chunkpoint+sizeof(int)*(2);
    int *allocation = (int*) p;
    int blocksize=0;
    int blockSize[n];
    int blockBase[n];
    int lastBlock=0;
    int emptysize=0;
    int emptySize[n];
    int emptyBase[n];
    int emptylastBlock=0;
    memset(blockBase, -1, sizeof(blockBase));
    memset(blockSize, -1, sizeof(blockSize));
    memset(emptyBase, -1, sizeof(emptyBase));
    memset(emptySize, -1, sizeof(emptySize));

    for (int k=0; k<n;k++){
        int t =(int)allocation[k];
      printf("%d ",t);
        if (t!=-1)
        {
            if( blocksize==0)
                blockBase[lastBlock]=k;
            blocksize++;
        }
        else{
            if(blocksize>0){
                blockSize[lastBlock]=blocksize;
                blocksize=0;
                lastBlock++;
            }
        }
        if (t==-1)
        {
            if( emptysize==0)
                emptyBase[emptylastBlock]=k;
            emptysize++;
        }
        else{
            if(emptysize>0){
                emptySize[emptylastBlock]=emptysize;
                emptysize=0;
                emptylastBlock++;
            }
        }
    }
    if(blocksize>0){
        blockSize[lastBlock]= blocksize;
        lastBlock++;
    }


    if(emptySize>0){
        emptySize[emptylastBlock]= emptysize;
        emptylastBlock++;
    }



    printf("print	called\n");
    printf( "\nOccupied Block No\tBlock Size\tBlock  Base\n");
    for (int i = 0; i < lastBlock; i++) {
       void* p= (char*)(blocksPointer+sizeof(int)*(blockBase[i]));
        printf( "  %d  \t \t\t %d \t\t %lx \n",i + 1,blockSize[i]*BLOCK_SIZE, (unsigned long )p);

    }
    printf( "\nEmpty Block No\tBlock Size\tBlock  Base\n");
    for (int i = 0; i < emptylastBlock; i++) {
        void* p= (char*)(blocksPointer+sizeof(int)*(emptyBase[i]));
        printf( "  %d  \t \t\t %d \t\t %lx \n",i + 1,emptySize[i]*BLOCK_SIZE, (unsigned long )p);

    }
}