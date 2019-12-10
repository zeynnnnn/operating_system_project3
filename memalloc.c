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

    memset(blockBase, -1, sizeof(blockBase));
    memset(blockSize, -1, sizeof(blockSize));


    int* sizePointer =blocksPointer;
    int k =0;

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
    blockBase[k] =(long int) ((char*)emptyPointer+ sizeof(char) );
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
    printf("\n");
    for (int t =0;t< k;t++){

        printf("%c",blockEmpty[t]);
    }
    printf("\n");

    if (*(int*) (chunkpoint)== BEST_FIT)
    {
        // pick each process and find suitable blocks
        // according to its size ad assign to it
     printf("Found Best fit\n");

            // Find the best fit block for current process
            int bestIdx = -1;
            for (int j=0; j< k; j++)
            {
                if (blockSize[j]==-1)
                    break;
                printf("\nBlockSize Of Current : %d%c\n",blockSize[j],blockEmpty[j]);
                if ((blockSize[j] >= objectsize) && (blockEmpty[j]=='E'))
                {

                    if (bestIdx == -1)
                        bestIdx = j;
                    else if (blockSize[bestIdx] > blockSize[j])
                        bestIdx = j;
                }
            }
            printf("BestIDx %d\n",bestIdx);
            // If we could find a block for current process
            if (bestIdx != -1) {
                printf("Girdiiiiiii");
                long int  originalNext ;

                originalNext =    *(long int *) ((char *) blockBase[bestIdx] - sizeof(long int)- sizeof(char));
                printf("1");
                 int  firstSize =    *(int *) ((char *) blockBase[bestIdx] - sizeof(long int) - sizeof(int) - sizeof(char) );
                printf("2");
                *(int *) ((char *) blockBase[bestIdx] +objectsize )= firstSize- objectsize;
                printf("3");
                *(long int *) ((char *) blockBase[bestIdx] - sizeof(long int)- sizeof(char)) = (long int) (long int *) ( (char *) blockBase[bestIdx] + objectsize);
                printf("4");
                *(int *) ((char *) blockBase[bestIdx] - sizeof(long int) - sizeof(int) - sizeof(char) )= objectsize;
                printf("5");
                *(char *) ((char *) blockBase[bestIdx]  - sizeof(char)) = 'F';
                printf("6");
                *(long int *) ((char *) blockBase[bestIdx]+objectsize + sizeof( int)) = originalNext;
                printf("7");
                *(char *) ((char *) blockBase[bestIdx] +objectsize + sizeof(long int)+ sizeof(int)) = 'E';

                printf("8");
            }

        return (void*)blockBase[bestIdx];

    } else if (*(int*)chunkpoint==FIRST_FIT)
    {
        printf("Found First fit\n");


        // Find the best fit block for current process
        int bestIdx = -1;
        for (int j=0; j< k; j++)
        {
            if (blockSize[j]==-1)
                break;
            printf("\nBlockSize Of Current : %d%c\n",blockSize[j],blockEmpty[j]);
            if ((blockSize[j] >= objectsize) && (blockEmpty[j]=='E'))
            {
                printf("Girdiiiiiii");

                    bestIdx = j;
                    long int  originalNext ;
                    originalNext =    *(long int *) ((char *) blockBase[bestIdx] - sizeof(long int)- sizeof(char));
                    int  firstSize =    *(int *) ((char *) blockBase[bestIdx] - sizeof(long int) - sizeof(int) - sizeof(char) );
                    *(int *) ((char *) blockBase[bestIdx] +objectsize )= firstSize- objectsize;
                    *(long int *) ((char *) blockBase[bestIdx] - sizeof(long int)- sizeof(char)) = (long int) (long int *) ( (char *) blockBase[bestIdx] + objectsize);
                    *(int *) ((char *) blockBase[bestIdx] - sizeof(long int) - sizeof(int) - sizeof(char) )= objectsize;
                    *(char *) ((char *) blockBase[bestIdx]  - sizeof(char)) = 'F';
                    *(long int *) ((char *) blockBase[bestIdx]+objectsize + sizeof( int)) = originalNext;
                    *(char *) ((char *) blockBase[bestIdx] +objectsize + sizeof(long int)+ sizeof(int)) = 'E';
                       return (void*)blockBase[bestIdx];


            }
        }

        // If we could find a block for current process


     //   printf("BestIDx %d\n",bestIdx);
        // If we could  not find a block for current process
        if (bestIdx == -1)
        {
            return NULL;
        }
        return (void*)blockBase[bestIdx];

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
    // void *p = (char *) chunkpoint + sizeof(int) * (1);
    // int n = (int) floor((*(int *) p) / BLOCK_SIZE);
    int n = MAX_CHUNK  /MIN_BLOCK;

    int blockSize[n];
    long int  blockBase[n];
    char blockEmpty[n];


    memset(blockBase, -1, sizeof(blockBase));
    memset(blockSize, -1, sizeof(blockSize));

    int* sizePointer =blocksPointer;
    int k =0;

    long int * nextPointer =(long*)((char*)sizePointer + sizeof(int));
    char* emptyPointer= (char *)((char*)nextPointer + sizeof(long int));
    while( (*(long int *)nextPointer)!=999999)
    {
        blockBase[k] =(long int)(( char*) (sizePointer)+ sizeof(int)+ sizeof(long int)+ sizeof(char));
        blockSize[k]= *sizePointer;
        blockEmpty[k]=*emptyPointer;
        emptyPointer= (char *)((char*)emptyPointer + sizeof(int) + *sizePointer + sizeof(long int)+ sizeof(char));
        nextPointer =(long int *)((char*) nextPointer + sizeof(int) + *sizePointer + sizeof(long int)+sizeof(char));
        sizePointer = (int*)((char*)sizePointer + sizeof(int) + *sizePointer + sizeof(long int)+ sizeof(char));
        k++;
    }
    blockBase[k] =(long int)(( char*) (sizePointer)+ sizeof(int)+ sizeof(long int )+ sizeof(char));
    blockEmpty[k]= *emptyPointer;
    blockSize[k]= *sizePointer;
    k++;

        for (int i = 0; i < k; i++) {
       //    void *p = (char *) (blocksPointer (blockBase[i]);
          //  printf("\nCalcuLATED:%lx", (unsigned long) p);
      //      printf("\ngiven :%lx", (unsigned long) objectptr);

            if (objectptr == (void*)blockBase[i]) {

                printf("Girdiiii! object ptr:%lx,    block base :%lx  ", (long )objectptr,blockBase[i]);
                char * startDeleted = (char*)(blockBase[i]- (sizeof(int)+ sizeof(long int )+ sizeof(char)) );
               if (i==0)
               {
                   if( *((char*)(blockBase[i+1])- sizeof(char)) == 'E'){
                       //size
                       *(int*)(blockBase[i]- (sizeof(int)+ sizeof(long int )+ sizeof(char))  )= *(int*)(blockBase[i+1]- (sizeof(int)+ sizeof(long int )+ sizeof(char))) +(*startDeleted);
                       //next
                       * (long int*) (  startDeleted+ sizeof(int))= *(long int *)((char*)blockBase[i+1]- sizeof(long int)-sizeof(char));
                       //empty
                       *(char *) (((char*) startDeleted )+ sizeof(int)+ sizeof(long int)) = 'E';
                   } else
                   {
                       *(char *) (((char*) startDeleted )+ sizeof(int)+ sizeof(long int)) = 'E';
                   }

               }
               else if (i==k)
               {
                   if( *((char*)(blockBase[i-1])- sizeof(char)) == 'E')
                   {
                       //size
                       *(int*)(blockBase[i-1]- (sizeof(int)+ sizeof(long int )+ sizeof(char))  )= *(int*)(blockBase[i-1]- (sizeof(int)+ sizeof(long int )+ sizeof(char))) +(*startDeleted);
                       //next
                       *(long int*) ((char*)blockBase[i-1]- sizeof(char)- sizeof(long int))= *(long int *)((char*)startDeleted+ sizeof(int));
                   } else{
                       *(char *) (((char*) startDeleted )+ sizeof(int)+ sizeof(long int)) = 'E';
                   }
               }
               else
               {

                   if( *((char*)(blockBase[i-1])- sizeof(char)) == 'E')
                   {
                       printf("fhdsugdpjfgpiedş\n");
                       //size
                       *(int*)((char*)blockBase[i-1]- (sizeof(int)+ sizeof(long int )+ sizeof(char))  )= *(int*)((char*)blockBase[i-1]- (sizeof(int)+ sizeof(long int )+ sizeof(char))) +*(int*)(startDeleted);
                        //next
                        printf("Değiştirdiğim next pointer içi :%lx\n",*(long int*) ( (char*)blockBase[i-1] - sizeof(char)- sizeof(long int)));
                       printf("Değiştirdiğim  pointer:%lx\n",(long int) ( (char*)blockBase[i-1] - sizeof(char)- sizeof(long int)));
                        *(long int*) ( (char*)blockBase[i-1] - sizeof(char)- sizeof(long int))= *(long int *)((char*)startDeleted + sizeof(int)+  sizeof(char));
                       printf("Değiştirdiğim hali pointer içi :%lx\n",*(long int*) ( (char*)blockBase[i-1] - sizeof(char)- sizeof(long int)));
                   }
                   else
                   {
                       if (* ((char*)blockBase[i+1])- sizeof(char) == 'E'){
                           //size
                           *(int*)((char*)blockBase[i]- (sizeof(int)+ sizeof(long int )+ sizeof(char))  )= *(int*)((char*)blockBase[i+1]- (sizeof(int)+ sizeof(long int )+ sizeof(char))) +*(int*)(startDeleted);
                           //next
                              * (long int*) (  startDeleted+ sizeof(int))= *(long int *)((char*)blockBase[i+1]- sizeof(long int)-sizeof(char));
                              //empty
                           *(char *) ( startDeleted+ sizeof(char)+ sizeof(long int)) = 'E';
                         }
                       else
                           {

                               *(char *) (((char*) startDeleted )+ sizeof(int)+ sizeof(long int)) = 'E';
                             }
                   }

               }
                return;
            }
        }

    }

    void mem_print(void) {
        int n = MAX_CHUNK  /MIN_BLOCK;
        void* p = (char *) chunkpoint + sizeof(int) * (2);

        int blockSize[n];
        long int  blockBase[n];
        char blockEmpty[n];


        memset(blockBase, -1, sizeof(blockBase));
        memset(blockSize, -1, sizeof(blockSize));


        int* sizePointer =blocksPointer;
        int k =0;

        long int * nextPointer =(long*)((char*)sizePointer + sizeof(int));
        char* emptyPointer= (char *)((char*)nextPointer + sizeof(long int));
        while( (*(long int *)nextPointer)!=999999)
        {
            blockBase[k] =(long int)(( char*) (sizePointer)+ sizeof(int)+ sizeof(long int)+ sizeof(char));
            blockSize[k]= *sizePointer;
            blockEmpty[k]=*emptyPointer;
            emptyPointer= (char *)((char*)emptyPointer + sizeof(int) + *sizePointer + sizeof(long int)+ sizeof(char));
            nextPointer =(long int *)((char*) nextPointer + sizeof(int) + *sizePointer + sizeof(long int)+sizeof(char));
            sizePointer = (int*)((char*)sizePointer + sizeof(int) + *sizePointer + sizeof(long int)+ sizeof(char));
            k++;
        }
        blockBase[k] =(long int)(( char*) (sizePointer)+ sizeof(int)+ sizeof(long int )+ sizeof(char));
        blockEmpty[k]= *emptyPointer;
        blockSize[k]= *sizePointer;
        k++;
        printf("print	called\n");
        printf("\nStatus \tBlock Size\tBlock  Base\n");
        for (int i = 0; i < k; i++) {
            printf("%c  \t \t\t %d \t\t %lx \n", blockEmpty[i], blockSize[i] ,blockBase[i]);

        }

    }
