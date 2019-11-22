/*
  Процесс-1 передает число   процессу-2.
   Процесс-2 пересылает  процессу-1 строку символов, длиной "число".
   Повторить 3 раза. Вывести на экран строку символов с ее длиной.
 */
 // mpicc main.c COMPILE
 // mpirun -n 3 a.out EXECUTE
#include <stdio.h>
#include <mpi/mpi.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define CYCLES 3

int main(int argc, char** argv)
{   
    int number;
    int cycles = CYCLES;
    int number_of_operations;
    int size, rank;
    char* array;
    MPI_Status status;
    srand(time(NULL));
    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size != 3){
        if (rank == 0)
        printf("Error: 3 processes required,\
        instead of %d\n", size);
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Abort(MPI_COMM_WORLD, MPI_ERR_OTHER);
    return -1;
    }
    for (int n = 0; n < cycles; n++)
    {
        switch(rank)
        {
        case 1:
        {
            printf("Iteration number: %d\n", n+1);
            number=rand()%20 + 1;
            MPI_Send(&number, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
            array = (char*)malloc((number + 1) * sizeof(char));
            MPI_Recv(array, number + 1, MPI_CHAR, 2, 0, MPI_COMM_WORLD,
            MPI_STATUS_IGNORE);
            printf("Word from process 2: %s\n", array);
            printf("Length of word from process 2: %ld\n\n", strlen(array));
            free(array);
        }
        break;
        case 2:
        {
            char str[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!"; 
            MPI_Recv(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD,
            MPI_STATUS_IGNORE);
            array = (char*)malloc((number + 1) * sizeof(char));
            for (int i = 0; i < number; i++)
            {
                int key = rand() % (int)(strlen(str));
                array[i]= str[key];
            }
            array[number] = '\0';
            MPI_Send(array, number + 1, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
            free(array);
        }
        break;
        }
    }
    MPI_Finalize();
    return 0;
}