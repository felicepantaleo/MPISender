/*
 * pingpong.cpp
 *
 *  Created on: Sep 25, 2015
 *      Author: fpantale
 */


#include <mpi.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
//#ifdef _ENABLE_CUDA_
#include <cuda.h>
#include <cuda_runtime.h>
//#endif


#ifndef FIELD_WIDTH
#   define FIELD_WIDTH 20
#endif

#ifndef FLOAT_PRECISION
#   define FLOAT_PRECISION 2
#endif

#define MESSAGE_ALIGNMENT 64
#define MAX_ALIGNMENT 65536
#define MAX_MSG_SIZE (1<<22)
#define MYBUFSIZE (MAX_MSG_SIZE + MAX_ALIGNMENT)

#define LOOP_LARGE  100
#define SKIP_LARGE  10
#define LARGE_MESSAGE_SIZE  8192

#ifdef _ENABLE_CUDA_
#   define CUDA_ENABLED 1
#else
#   define CUDA_ENABLED 0
#endif

char s_buf_original[MYBUFSIZE];
char r_buf_original[MYBUFSIZE];

int skip = 1000;
int loop = 10000;


int
main (int argc, char *argv[])
{
    int myid, numprocs, i;
    int size;
    MPI_Status reqstat;
    char *s_buf, *r_buf;
    double t_start = 0.0, t_end = 0.0;


    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);

    if(numprocs != 2) {
        if(myid == 0) {
            fprintf(stderr, "This test requires exactly two processes\n");
        }

        MPI_Finalize();
        exit(EXIT_FAILURE);
    }

    /* Latency test */
    for(size = 0; size <= MAX_MSG_SIZE; size = (size ? size * 2 : 1)) {


        if(size > LARGE_MESSAGE_SIZE) {
            loop = LOOP_LARGE;
            skip = SKIP_LARGE;
        }

        MPI_Barrier(MPI_COMM_WORLD);

        if(myid == 0) {
            for(i = 0; i < loop + skip; i++) {
                if(i == skip) t_start = MPI_Wtime();

                MPI_Send(s_buf, size, MPI_CHAR, 1, 1, MPI_COMM_WORLD);
                MPI_Recv(r_buf, size, MPI_CHAR, 1, 1, MPI_COMM_WORLD, &reqstat);
            }

            t_end = MPI_Wtime();
        }

        else if(myid == 1) {
            for(i = 0; i < loop + skip; i++) {
                MPI_Recv(r_buf, size, MPI_CHAR, 0, 1, MPI_COMM_WORLD, &reqstat);
                MPI_Send(s_buf, size, MPI_CHAR, 0, 1, MPI_COMM_WORLD);
            }
        }

        if(myid == 0) {
            double latency = (t_end - t_start) * 1e6 / (2.0 * loop);

            fprintf(stdout, "%-*d%*.*f\n", 10, size, FIELD_WIDTH,
                    FLOAT_PRECISION, latency);
            fflush(stdout);
        }
    }

//    free_memory(s_buf, r_buf, myid);
    MPI_Finalize();

   return EXIT_SUCCESS;
}

/* vi:set sw=4 sts=4 tw=80: */
