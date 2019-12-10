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
#include <stdbool.h>
#define MAX_CHUNK 33554432
#define MIN_BLOCK 128
//#define BLOCK_SIZE 128
void  *chunkpoint;
int lastId;
int* blocksPointer;

int	mem_init	(void	*chunkpointer,	int	chunksize,	int	method)
{
    lastId=0;
    printf("init called\n");
    if ((method!=BEST_FIT)&&(method!=FIRST_FIT)&&(method!=WORST_FIT))
        return -1;

    if (( chunksize < 32 )||(chunksize> 32*1024))
        return -1;
    chunkpoint=chunkpointer;
   *(int*)chunkpointer = method;
    void *p = (char*)chunkpointer+sizeof(int);
    *(int*) p = chunksize ;
   p = p+ sizeof(int);
   blocksPointer=p;
   // int t =(chunksize*1024)-(2* sizeof(int));
 //   int afterAlloSize=   t - (int)floor(t/(BLOCK_SIZE+ sizeof(int)));
      //      printf("Size::: %d \n",afterAlloSize);
  // *(int*) p = afterAlloSize ;

    *(int*) blocksPointer = (int)((1024 * chunksize)- sizeof(int)*(3)- sizeof(char)- sizeof(long int)); // first bock initialization
     p = (char*)p+sizeof(int);
     *(long int *)p = 999999; ///////////////////////////////
    printf("\nAddress of nextPointer:%lx  Inside: %lx",(long int )p,*(long int *) p);
    p = (char*)p+sizeof(long int);
    *(char *)p = 'E';
 printf("\nAddress of size: %lx Inside:%d",(int*)blocksPointer,*(int*) blocksPointer);

    printf("\nAddress of empty-full char:%lx  Inside: %s",(long int )p,p);
    /*
    int k=0;
    // Initially no block is assigned to any allocation
    for(k=0;k< (int)floor(1.0*afterAlloSize/BLOCK_SIZE) ;k++) //calculate according to left over space byte/byte
    {
        void *p = (char*)chunkpointer+sizeof(int)*(2+k);
        *(int*) p = -1;
    }
    blocksPointer = (char*)chunkpointer+sizeof(int)*(2+k);
*/

    return	(0); //	if	success
}
void	*mem_allocate	(int	objectsize)
{
  //  void *p = (char*)chunkpoint+sizeof(int)*(1);
 //   printf("ChunkSize: %d ...\n",*(int*)p);
    printf("alloc	 called\n");

    if (( objectsize < 128)|| (objectsize> 1024*2048))
        return (NULL); //	if	not	success
    int n = MAX_CHUNK  /MIN_BLOCK;

   void* p = (char *) chunkpoint + sizeof(int) * (2);
    //  int *allocation = (int *) p;
    int blocksize = 0;
    int blockSize[n];
    long int  blockBase[n];
    char blockEmpty[n];
    int includedID[n];
    int lastBlock = 0;

    memset(blockBase, -1, sizeof(blockBase));
    memset(blockSize, -1, sizeof(blockSize));
    memset(includedID, -2, sizeof(blockSize));

    int* sizePointer =blocksPointer;
    int k =0;
    blockBase[k] =(long int*)(( char*) (sizePointer)+ sizeof(int)+ sizeof(long int )+ sizeof(char));
    long int * nextPointer =(long*)((char*)sizePointer + sizeof(int));
    char* emptyPointer= (char *)((char*)nextPointer + sizeof(long int));
    while( (*(long int *)nextPointer)!=999999) ///////////////////
    {
        blockBase[k] =(long int)(( char*) (sizePointer)+ sizeof(int)+ sizeof(long int)+ sizeof(char));
        blockSize[k]= *sizePointer;
        blockEmpty[k]=*emptyPointer;
        emptyPointer= (int*)((char*)emptyPointer + sizeof(int) + *sizePointer + sizeof(long int)+ sizeof(char));
        nextPointer =(long int *)((char*) nextPointer + sizeof(int) + *sizePointer + sizeof(long int)+sizeof(char));
        sizePointer = (int*)((char*)sizePointer + sizeof(int) + *sizePointer + sizeof(long int)+ sizeof(char));
        k++;
    }
    blockEmpty[k]= *emptyPointer;
    blockSize[k]= *sizePointer;
    k++;

    printf("\n");
    for (int t =0;t< k;t++){
    printf("%lx",(unsigned long )blockBase[t]);

    }
    printf("\n");
    for (int t =0;t< k;t++){

        printf("%d",blockSize[t]);
    }

    if (*(int*) (chunkpoint)== BEST_FIT)
    {
        // pick each process and find suitable blocks
        // according to its size ad assign to it
    // printf("Found Best fit\n");
    // int objectSizeAsBlock  = (int)ceil(1.0*objectsize/BLOCK_SIZE);
            // Find the best fit block for current process
            int bestIdx = -1;
            for (int j=0; j< n; j++)
            {
                if (blockSize[j]==-1)
                    break;
                if ((blockSize[j] >= objectsize)&& (blockEmpty[j]=='e'))
                {
                    if (bestIdx == -1)
                        bestIdx = j;
                    else if (blockSize[bestIdx] > blockSize[j])
                        bestIdx = j;
                }
            }
         //   printf("BestIDx %d\n",bestIdx);
            // If we could find a block for current process
            if (bestIdx != -1) {
              /*  long int* movedNextPointer ;
                if (bestIdx==0)
                {
                    movedNextPointer = (long int *) ((char *) blockBase[bestIdx] - sizeof(long int)- sizeof(char));
                }
                else{
                    movedNextPointer = (long int *) ((char *) blockBase[bestIdx - 1] - sizeof(long int)- sizeof(char));
                    blockBase[bestIdx-1]=
                }*/

                long int * originalNext ;
                originalNext =    *(long int **) ((char *) blockBase[bestIdx] - sizeof(long int)- sizeof(char));
                 int  firstSize =    *(int *) ((char *) blockBase[bestIdx] - sizeof(long int) - sizeof(int) - sizeof(char) );
                *(int *) ((char *) blockBase[bestIdx] +objectsize - sizeof(long int) - sizeof(int) - sizeof(char) )= firstSize- objectsize;
                *(long int **) ((char *) blockBase[bestIdx] - sizeof(long int)- sizeof(char)) = *(long int **) ((char *) blockBase[bestIdx]+objectsize - sizeof(long int)- sizeof(char));
                *(int *) ((char *) blockBase[bestIdx] - sizeof(long int) - sizeof(int) - sizeof(char) )= objectsize;
                *(char *) ((char *) blockBase[bestIdx]  - sizeof(char)) = 'f';
                *(long int **) ((char *) blockBase[bestIdx]+objectsize - sizeof(long int)- sizeof(char)) = originalNext;
                *(char *) ((char *) blockBase[bestIdx] +objectsize - sizeof(char)) = 'e';


            }

        return (void*)blockBase[bestIdx];

    } else if (*(int*)chunkpoint==FIRST_FIT)
    {
        printf("Found First fit\n");

        // Find the best fit block for current process
        int bestIdx = -1;
        for (int j=0; j< n; j++)
        {
            if (blockSize[j]==-1)
                break;
            if (blockSize[j] >= objectsize)
            {
                bestIdx=j;
                long int *movedNextPointer = (long int*) ((char *) blockBase[bestIdx - 1] - sizeof(long int));
                *(long int **) ((char *) blockBase[bestIdx] - sizeof(long int )) = movedNextPointer;
                *(int *) ((char *) blockBase[bestIdx] - sizeof(long int ) - sizeof(int)) = objectsize;
            }
        }
     //   printf("BestIDx %d\n",bestIdx);
        // If we could find a block for current process
        if (bestIdx == -1)
        {
            return NULL;
        }
        return blockBase[bestIdx];

    }else if (*(int*)chunkpoint==WORST_FIT)
    {
        printf("Found Worst fit\n");

        // Find the best fit block for current process
        int worstIdx = -1;
        for (int j=0; j< n; j++)
        {
            if (blockSize[j]==-1)
                break;
            if (blockSize[j] >= objectsize)
            {
                if (worstIdx == -1)
                    worstIdx = j;
                else if (blockSize[worstIdx] <blockSize[j])
                    worstIdx = j;
            }
        }
       // printf("WorstID %d\n",worstIdx);
        // If we could find a block for current process
        if (worstIdx != -1)
        {
            long int *movedNextPointer = (long int *) ((char *) blockBase[worstIdx - 1] - sizeof(long int));
            *(long int **) ((char *) blockBase[worstIdx] - sizeof(long int)) = movedNextPointer;
            *(int *) ((char *) blockBase[worstIdx] - sizeof(long int) - sizeof(int)) = objectsize;
        }

        return blockBase[worstIdx];

    } else
        return NULL; //	if	not	success
}


void	mem_free(void	*objectptr) {
    printf("free	called\n");
    void *p = (char *) chunkpoint + sizeof(int) * (1);
   // int n = (int) floor((*(int *) p) / BLOCK_SIZE);
    int n = MAX_CHUNK  /MIN_BLOCK;
    p = (char *) chunkpoint + sizeof(int) * (2);
    int *allocation = (int *) p;
    int blocksize = 0;
    int blockSize[n];
    int blockBase[n];
    int lastBlock = 0;
    int lastHand = allocation[0];

    memset(blockBase, -1, sizeof(blockBase));
    memset(blockSize, -1, sizeof(blockSize));

    int* sizePointer =blocksPointer;
    int k =0;
    blockBase[k] =( char*) (sizePointer)+ sizeof(int)+ sizeof(long int);
    long int* nextPointer =(long int*)((char*)sizePointer + sizeof(int));
    while( (*(long int**)nextPointer)!=NULL)
    {

        blockBase[k] =( char*) (sizePointer)+ sizeof(int)+ sizeof(long int);
        blockSize[k]= *sizePointer;
        nextPointer =(long int*)((char*) nextPointer + sizeof(int) + *sizePointer + sizeof(long int ));
        sizePointer = (int*)((char*)sizePointer + sizeof(int) + *sizePointer + sizeof(long int ));
        k++;

    }
    blockBase[k] =( char*) (sizePointer)+ sizeof(int)+ sizeof(long int);
    blockSize[k]= *sizePointer;

    /*

        for (int i = 0; i < lastBlock; i++) {
            void *p = (char *) (blocksPointer) + sizeof(int) * (blockBase[i]);
          //  printf("\nCalcuLATED:%lx", (unsigned long) p);
      //      printf("\ngiven :%lx", (unsigned long) objectptr);
            if (objectptr == p) {
                void *p = (char *) (chunkpoint + sizeof(int) * (2 + blockBase[i])); //2 COMES FROM METHOD AND SIZE INTS
                int deletedId = *(int *) p;
              //  printf("\nDeletedId :%d\n", deletedId);
                int y = 0;
                while (allocation[blockBase[i] + y] == deletedId) {
                    allocation[blockBase[i] + y] = -1;
                    y++;
                }
                return;
            }
        }
        */
    }

    void mem_print(void) {

        void *p = (char *) chunkpoint + sizeof(int) * (1);
     //   int n = (int) floor((*(int *) p) / BLOCK_SIZE);
        int n = MAX_CHUNK  /MIN_BLOCK;
        p = (char *) chunkpoint + sizeof(int) * (2);
      //  int *allocation = (int *) p;
        int blocksize = 0;
        int blockSize[n];
        long int  blockBase[n];
        char blockEmpty[n];
        int includedID[n];
        int lastBlock = 0;

        memset(blockBase, -1, sizeof(blockBase));
        memset(blockSize, -1, sizeof(blockSize));
        memset(includedID, -2, sizeof(blockSize));

        int* sizePointer =blocksPointer;
        int k =0;
        blockBase[k] =(long int*)(( char*) (sizePointer)+ sizeof(int)+ sizeof(long int )+ sizeof(char));
        long int * nextPointer =(long*)((char*)sizePointer + sizeof(int));
        char* emptyPointer= (char *)((char*)nextPointer + sizeof(long int));
        while( (*(long int *)nextPointer)!=999999)
        {
            blockBase[k] =(long int)(( char*) (sizePointer)+ sizeof(int)+ sizeof(long int)+ sizeof(char));
            blockSize[k]= *sizePointer;
            blockEmpty[k]=*emptyPointer;
            emptyPointer= (int*)((char*)emptyPointer + sizeof(int) + *sizePointer + sizeof(long int)+ sizeof(char));
            nextPointer =(long int *)((char*) nextPointer + sizeof(int) + *sizePointer + sizeof(long int)+sizeof(char));
            sizePointer = (int*)((char*)sizePointer + sizeof(int) + *sizePointer + sizeof(long int)+ sizeof(char));
            k++;
        }
        blockEmpty[k]= *emptyPointer;
        blockSize[k]= *sizePointer;
        k++;
        printf("print	called\n");
        printf("\nStatus \tBlock Size\tBlock  Base\n");
        for (int i = 0; i < k; i++) {
          //  void *p = (char *) (blocksPointer) +  (blockBase[i]);
            printf("%c  \t \t\t %d \t\t %lx \n", blockEmpty[i], blockSize[i] ,blockBase[i]);

        }

    }
