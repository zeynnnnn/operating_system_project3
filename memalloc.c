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

#include <pthread.h>
#define MAX_CHUNK 33554432
#define MIN_BLOCK 128
#define END_MARKER 999999
//#define BLOCK_SIZE 128
void  *chunkpoint;
int* blocksPointer;
int leftOverAtEnd  =0;
pthread_mutex_t lock;
int	mem_init	(void	*chunkpointer,	int	chunksize,	int	method)
{
    printf("init called\n");
    if ((method!=BEST_FIT)&&(method!=FIRST_FIT)&&(method!=WORST_FIT))
        return -1;
    if (( chunksize < 32 )||(chunksize> 32*1024))
        return -1;
    leftOverAtEnd=0;
    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return -1;
    }
    chunkpoint=chunkpointer;
   *(int*)chunkpointer = method;
    void *p = (char*)chunkpointer+sizeof(int);
    *(int*) p = chunksize ;
   p = p+ sizeof(int);
   blocksPointer=p;
    *(int*) blocksPointer = (int)((1024 * chunksize)- sizeof(int)*(3)- sizeof(char)- sizeof(long int)); // first bock initialization
     p = (char*)p+sizeof(int);
     *(long int *)p = END_MARKER;
   // printf("\nAddress of nextPointer:%lx  Inside: %lx",(long int )p,*(long int *) p);
    p = (char*)p+sizeof(long int);
    *(char *)p = 'E';

    return	(0); //	if	success
}
void	*mem_allocate	(int	objectsize)
{
    pthread_mutex_lock(&lock);

  //  void *p = (char*)chunkpoint+sizeof(int)*(1);
 //   printf("ChunkSize: %d ...\n",*(int*)p);
    printf("alloc	 called with object size: %d\n",objectsize);

    if (( objectsize < 128)|| (objectsize> 1024*2048))
    {

        pthread_mutex_unlock(&lock);
        return (NULL); //	if	not	success
    }



    int n = MAX_CHUNK  /MIN_BLOCK;


    if (objectsize>(*(int*)((char*)chunkpoint+ sizeof(int))*1024- sizeof(int)- sizeof(char)- sizeof(long int))){
        printf("Object is bigger than chucksize!");
        pthread_mutex_unlock(&lock);
        return (NULL);
    }


    int objectAndInfoSize= objectsize+ sizeof(int)+ sizeof(char)+ sizeof(long int);

    int blockSize[n];
    long int  blockBase[n];
    char blockEmpty[n];

    memset(blockBase, -1, sizeof(blockBase));
    memset(blockSize, -1, sizeof(blockSize));


    int* sizePointer =blocksPointer;
    int k =0;

    long int * nextPointer =(long*)((char*)sizePointer + sizeof(int));
    char* emptyPointer= (char *)((char*)nextPointer + sizeof(long int));
    while( (*(long int *)nextPointer)!=END_MARKER)
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

    printf("\n");
   /* for (int t =0;t< k;t++){
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
    printf("\n");*/

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
              //  printf("\nBlockSize Of Current : %d%c\n",blockSize[j],blockEmpty[j]);
                if ((blockSize[j] >= objectsize) && (blockEmpty[j]=='E'))
                {
                    if (bestIdx == -1)
                        bestIdx = j;
                    else if (blockSize[bestIdx] > blockSize[j])
                        bestIdx = j;
                }
            }
        if (bestIdx == -1)
        {
            printf("Could not found a suitable place for the allocation!\n");
            pthread_mutex_unlock(&lock);
            return NULL;
        }
   //         printf("BestIDx %d\n",bestIdx);
            // If we could find a block for current process
            if (bestIdx != -1) {

                if((blockSize[bestIdx] < objectAndInfoSize) ) {

                    printf("k:%d  object and ınfo sze = %d", k, objectAndInfoSize);
                    if ((bestIdx == (k - 1)) && ((blockSize[bestIdx]) < (objectAndInfoSize))) {
                         int firstSize = *(int *) ((char *) blockBase[bestIdx] - sizeof(long int) - sizeof(int) - sizeof(char));
                        printf("left over: %d\n",leftOverAtEnd);
                        leftOverAtEnd = firstSize - objectsize;
                        printf("left over: %d\n",leftOverAtEnd);
                        *(char *) ((char *) blockBase[bestIdx] - sizeof(char)) = 'F';
                        *(long int *) ((char *) blockBase[bestIdx] - sizeof(long int) -
                                       sizeof(char)) = END_MARKER; //////////////////not so sure if it is needed
                        *(int *) ((char *) blockBase[bestIdx] - sizeof(long int) - sizeof(int) -
                                  sizeof(char)) = objectsize;
                    }
                }
                else {
                    int firstSize = *(int *) ((char *) blockBase[bestIdx] - sizeof(long int) - sizeof(int) -
                                              sizeof(char));
                    *(int *) ((char *) blockBase[bestIdx] + objectsize) =
                            firstSize - objectsize - sizeof(long int) - sizeof(int) - sizeof(char);
                    *(int *) ((char *) blockBase[bestIdx] - sizeof(long int) - sizeof(int) - sizeof(char)) = objectsize;
                    *(char *) ((char *) blockBase[bestIdx] - sizeof(char)) = 'F';
                    long int originalNext = *(long int *) ((char *) blockBase[bestIdx] - sizeof(long int) -
                                                           sizeof(char));
                    if (originalNext == END_MARKER)
                        *(long int *) ((char *) blockBase[bestIdx] + objectsize +
                                       sizeof(int)) = END_MARKER; //////////////////not so sure if it is needed
                    else
                        *(long int *) ((char *) blockBase[bestIdx] + objectsize + sizeof(int)) = originalNext;
                    *(long int *) ((char *) blockBase[bestIdx] - sizeof(long int) - sizeof(char)) = *(long int *) (
                            (char *) blockBase[bestIdx] + objectsize);
                    *(char *) ((char *) blockBase[bestIdx] + objectsize + sizeof(long int) + sizeof(int)) = 'E';


                }
            }
        pthread_mutex_unlock(&lock);
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
            //printf("\nBlockSize Of Current : %d%c\n",blockSize[j],blockEmpty[j]);


            if ((blockSize[j] >= objectsize) && (blockEmpty[j]=='E'))
            {
                if((blockSize[j] < objectAndInfoSize) ) {
                    bestIdx = j;
    //                printf("k:%d  object and ınfo sze = %d", k, objectAndInfoSize);
                    if ((bestIdx == (k - 1)) && ((blockSize[bestIdx]) < (objectAndInfoSize))) {
         //               printf("rjfjokdplcdıo");

                        int firstSize = *(int *) ((char *) blockBase[bestIdx] - sizeof(long int) - sizeof(int) -
                                                  sizeof(char));
                      //  printf("left over: %d\n",leftOverAtEnd);
                        leftOverAtEnd = firstSize - objectsize;
                       // printf("left over: %d\n",leftOverAtEnd);
                        *(char *) ((char *) blockBase[bestIdx] - sizeof(char)) = 'F';
                        *(long int *) ((char *) blockBase[bestIdx] - sizeof(long int) -
                                       sizeof(char)) = END_MARKER; //////////////////not so sure if it is needed

                        *(int *) ((char *) blockBase[bestIdx] - sizeof(long int) - sizeof(int) -
                                  sizeof(char)) = objectsize;
                        pthread_mutex_unlock(&lock);
                        return (void *) blockBase[bestIdx];
                    }
                }
                else {
                    bestIdx = j;

                    int firstSize = *(int *) ((char *) blockBase[bestIdx] - sizeof(long int) - sizeof(int) -
                                              sizeof(char));
                    *(int *) ((char *) blockBase[bestIdx] + objectsize) =
                            firstSize - objectsize - sizeof(long int) - sizeof(int) - sizeof(char);
                    *(int *) ((char *) blockBase[bestIdx] - sizeof(long int) - sizeof(int) - sizeof(char)) = objectsize;
                    *(char *) ((char *) blockBase[bestIdx] - sizeof(char)) = 'F';
                    long int originalNext = *(long int *) ((char *) blockBase[bestIdx] - sizeof(long int) -
                                                           sizeof(char));
                    if (originalNext == END_MARKER)
                        *(long int *) ((char *) blockBase[bestIdx] + objectsize +
                                       sizeof(int)) = END_MARKER; //////////////////not so sure if it is needed
                    else
                        *(long int *) ((char *) blockBase[bestIdx] + objectsize + sizeof(int)) = originalNext;
                    *(long int *) ((char *) blockBase[bestIdx] - sizeof(long int) - sizeof(char)) = *(long int *) (
                            (char *) blockBase[bestIdx] + objectsize);
                    *(char *) ((char *) blockBase[bestIdx] + objectsize + sizeof(long int) + sizeof(int)) = 'E';

                    pthread_mutex_unlock(&lock);
                    return (void *) blockBase[bestIdx];

                }
            }
        }

        // If we could  not find a block for current process
        if (bestIdx == -1)
        {
            printf("Could not found a suitable place for the allocation!\n");
            pthread_mutex_unlock(&lock);

            return NULL;
        }


    }else if (*(int*)chunkpoint==WORST_FIT)
    {
        printf("Found Worst fit\n");

        // Find the best fit block for current process
        int worstIdx = -1;
        for (int j=0; j< n; j++)
        {
            if (blockSize[j]==-1)
                break;
            if ((blockSize[j] >= objectAndInfoSize)&&(blockEmpty[j]=='E'))
            {
                if (worstIdx == -1)
                    worstIdx = j;
                else if (blockSize[worstIdx] <blockSize[j])
                    worstIdx = j;
            }
        }
        if (worstIdx == -1)
        {
            printf("Could not found a suitable place for the allocation!\n");
            pthread_mutex_unlock(&lock);
            return NULL;
        }

     //   printf("WorstIDx %d\n",worstIdx);
        // If we could find a block for current process
        if (worstIdx != -1) {

            if ((blockSize[worstIdx] < objectAndInfoSize)) {

                printf("k:%d  object and ınfo sze = %d", k, objectAndInfoSize);
                if ((worstIdx == (k - 1)) && ((blockSize[worstIdx]) < (objectAndInfoSize))) {
                    printf("rjfjokdplcdıo");

                    int firstSize = *(int *) ((char *) blockBase[worstIdx] - sizeof(long int) - sizeof(int) -
                                              sizeof(char));
            //        printf("left over: %d\n", leftOverAtEnd);
                    leftOverAtEnd = firstSize - objectsize;
              //      printf("left over: %d\n", leftOverAtEnd);
                    *(char *) ((char *) blockBase[worstIdx] - sizeof(char)) = 'F';
                    *(long int *) ((char *) blockBase[worstIdx] - sizeof(long int) -
                                   sizeof(char)) = END_MARKER; //////////////////not so sure if it is needed

                    *(int *) ((char *) blockBase[worstIdx] - sizeof(long int) - sizeof(int) -
                              sizeof(char)) = objectsize;

                }
            } else {
                int firstSize = *(int *) ((char *) blockBase[worstIdx] - sizeof(long int) - sizeof(int) -
                                          sizeof(char));
                *(int *) ((char *) blockBase[worstIdx] + objectsize) =
                        firstSize - objectsize - sizeof(long int) - sizeof(int) - sizeof(char);
                *(int *) ((char *) blockBase[worstIdx] - sizeof(long int) - sizeof(int) - sizeof(char)) = objectsize;
                *(char *) ((char *) blockBase[worstIdx] - sizeof(char)) = 'F';
                long int originalNext = *(long int *) ((char *) blockBase[worstIdx] - sizeof(long int) -
                                                       sizeof(char));
                if (originalNext == END_MARKER)
                    *(long int *) ((char *) blockBase[worstIdx] + objectsize +
                                   sizeof(int)) = END_MARKER; //////////////////not so sure if it is needed
                else
                    *(long int *) ((char *) blockBase[worstIdx] + objectsize + sizeof(int)) = originalNext;
                *(long int *) ((char *) blockBase[worstIdx] - sizeof(long int) - sizeof(char)) = *(long int *) (
                        (char *) blockBase[worstIdx] + objectsize);
                *(char *) ((char *) blockBase[worstIdx] + objectsize + sizeof(long int) + sizeof(int)) = 'E';



            }
        }
        pthread_mutex_unlock(&lock);
        return (void*)blockBase[worstIdx];

    }
    pthread_mutex_unlock(&lock);
    return NULL; //	if	not	success
}


void	mem_free(void	*objectptr) {
    pthread_mutex_lock(&lock);
    printf("free	called\n");
    // void *p = (char *) chunkpoint + sizeof(int) * (1);
    // int n = (int) floor((*(int *) p) / BLOCK_SIZE);
    if (objectptr==NULL)
    {
        pthread_mutex_unlock(&lock);
        printf("Given Pointer is NUll!");
        return;
    }


    if( *((char*) objectptr - sizeof(char)) != 'F')
    {
        printf("! object ptr:%lx\n  ", (long )objectptr);
        printf("This memory location is not good for delete!\n");
        pthread_mutex_unlock(&lock);
        return;
    }
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
    while(( (*(long int *)nextPointer)!=END_MARKER)&& (k<n))
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
 //   printf("Left Over:%d",leftOverAtEnd);
        for (int i = 0; i < k; i++) {
       //    void *p = (char *) (blocksPointer (blockBase[i]);
          //  printf("\nCalcuLATED:%lx", (unsigned long) p);
      //      printf("\ngiven :%lx", (unsigned long) objectptr);
            int oneInfoSize= (sizeof(int) + sizeof(long int) + sizeof(char));

            if( objectptr == (void*)blockBase[i]) {

              //  printf("Girdiiii! object ptr:%lx,    block base :%lx  ", (long )objectptr,blockBase[i]);
                char * startDeleted = (char*)(blockBase[i]- oneInfoSize );

                if (i==(k-1))
               {
                   if (k==1){
                       *(char *) ((char*) startDeleted + sizeof(int)+ sizeof(long int))= 'E';
                       *(long int*) ((char*)blockBase[i]- sizeof(char)- sizeof(long int))= END_MARKER;
                       *(int*)(startDeleted )= *(int*)(startDeleted)+leftOverAtEnd;

                   } else{

                       if( *((char*)(blockBase[i-1])- sizeof(char)) == 'E')
                       {
                           //size
                           *(int*)(blockBase[i-1]- oneInfoSize )= *(int*)(blockBase[i-1]- oneInfoSize)+oneInfoSize +( *(int*)startDeleted)+leftOverAtEnd;
                           //next
                           *(long int*) ((char*)blockBase[i-1]- sizeof(char)- sizeof(long int))=END_MARKER;
                           //empty
                           *(char *) (((char*) startDeleted )+ sizeof(int)+ sizeof(long int)) = 'E'; // not used but if same place tried to be deleted twice
                       } else{
                           //size for left over
                           *(char *) (((char*) startDeleted )+ sizeof(int)+ sizeof(long int)) = 'E';

                           *(int*)(startDeleted )= *(int*)(startDeleted)+leftOverAtEnd;
                       }

                   }
                   leftOverAtEnd=0;

               } else if (i==0)
                     {
                    if( *((char*)blockBase[i+1]- sizeof(char)) == 'E'){
                        printf("fuhdjsıcpkağxpl<zpdfo\n");
                        //size
                        *(int*)(startDeleted )= *(int*)(blockBase[i+1]- oneInfoSize) +( *(int*)startDeleted)+ oneInfoSize;
                        //next
                        * (long int*) (  startDeleted+ sizeof(int))= *(long int *)((char*)blockBase[i+1]- sizeof(long int)-sizeof(char));
                        //empty
                        *(char *) (((char*) startDeleted )+ sizeof(int)+ sizeof(long int)) = 'E';
                    } else
                    {
                        *(char *) (((char*) startDeleted )+ sizeof(int)+ sizeof(long int)) = 'E';
                    }

                 }
                else
               {

                   if( *((char*)(blockBase[i-1])- sizeof(char)) == 'E') {
                       if (*(((char *) blockBase[i + 1]) - sizeof(char)) == 'F') {
                           //size
                           *(int *) ((char *) blockBase[i - 1] -oneInfoSize) =  *(int *) ((char *) blockBase[i - 1] - oneInfoSize) + *(int *) (startDeleted)+ oneInfoSize;
                           //next
                           *(long int *) ((char *) blockBase[i - 1] - sizeof(char) - sizeof(long int)) = *(long int *) (
                                   (char *) startDeleted + sizeof(int) + sizeof(char));
                           *(char *) (((char*) startDeleted )+ sizeof(int)+ sizeof(long int)) = 'E'; // not used but if same place tried to be deleted twice
                       } else {
                           //SİZE
                           *(int *) ((char *) blockBase[i - 1] - oneInfoSize) = *(int *) ((char *) blockBase[i - 1] -  oneInfoSize) +// öncekinin
                                   *(int *) (startDeleted)+ oneInfoSize + *(int *) ((char *) blockBase[i + 1] -oneInfoSize) +oneInfoSize ;
                           //next
                           *(long int *) ((char *) blockBase[i - 1] - sizeof(char) - sizeof(long int)) = *(long int *) (
                                   (char *) blockBase[i + 1] - sizeof(long int) - sizeof(char));
                           *(char *) (((char*) startDeleted )+ sizeof(int)+ sizeof(long int)) = 'E'; // not used but if same place tried to be deleted twice
                       }
                   }
                   else
                   {
                       if (* ((char*)blockBase[i+1]- sizeof(char) )== 'E'){
                           //size
                           *(int*)(startDeleted )= *(int*)((char*)blockBase[i+1]- oneInfoSize)+ oneInfoSize +*(int*)(startDeleted);
                          // printf("son size : %d \n",*(int*)startDeleted);
                           //next
                              * (long int*) ( startDeleted+ sizeof(int))= *(long int *)((char*)blockBase[i+1]- sizeof(long int)-sizeof(char));
                        //      printf("New next pointer inside:%lx\n\n\n",*(long int *)((char*)blockBase[i+1]- sizeof(long int)-sizeof(char)));
                        //   printf("son nextpointer : %lu \n",* (long int*) ( startDeleted+ sizeof(int)));
                              //empty
                           *(char *) ( startDeleted+ sizeof(int)+ sizeof(long int)) = 'E';
                         }
                       else
                           {

                               *(char *) (((char*) startDeleted )+ sizeof(int)+ sizeof(long int)) = 'E';
                             }
                   }

               }
                pthread_mutex_unlock(&lock);
                return;
            }
        }
    pthread_mutex_unlock(&lock);
    }

    void mem_print(void) {
        pthread_mutex_lock(&lock);
        void* p = (char *) chunkpoint + sizeof(int) ;
        int n = ( (*(int*)p ) *1024)/MIN_BLOCK; // turn to byte
        printf("print	called\n");
        int blockSize[n];
        long int  blockBase[n];
        char blockEmpty[n];

        memset(blockBase, -1, sizeof(blockBase));
        memset(blockSize, -1, sizeof(blockSize));

        int* sizePointer =blocksPointer;
        int k =0;

        long int * nextPointer =(long*)((char*)sizePointer + sizeof(int));
        char* emptyPointer= (char *)((char*)nextPointer + sizeof(long int));
        while( ( (*(long int *)nextPointer)!=END_MARKER) && (k<n))
        {
           // printf("K:%d \n",k);
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

      //  printf("k: %d n:%d",k,n);
        printf("\nStatus \tBlock Size\tBlock  Base\n");
        for (int i = 0; i < k; i++) {
            printf("%c  \t \t\t %d \t\t %lx \n", blockEmpty[i], blockSize[i] ,blockBase[i]);

        }
        pthread_mutex_unlock(&lock);

    }
