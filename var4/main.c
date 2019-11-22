/*
  Процесс-1 передает число   процессу-2.
   Процесс-2 пересылает  процессу-1 строку символов, длиной "число".
   Повторить 3 раза. Вывести на экран строку символов с ее длиной.
 */
 // Чтобы скомпилировать:
 // mpicc main.c
 // Чтобы запустить:
 // mpirun -n 3 a.out
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
    // Функция инициализации MPI
    MPI_Init(&argc, &argv);
    // Указывает на число процессов в коммуникаторе
    // для MPI_COMM_WORLD указывает на общее количество процессов
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    // Указывает на номер процесса в частной группе коммуникатора
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // Если запущено не 3 процесса, то выход из программы
    if (size != 3){
        if (rank == 0)
        printf("Error: 3 processes required,\
        instead of %d\n", size);
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Abort(MPI_COMM_WORLD, MPI_ERR_OTHER);
    return -1;
    }
    // Цикл 
    for (int n = 0; n < cycles; n++)
    {
        switch(rank)
        {
        case 1: // первый процесс
        {
            printf("Iteration number: %d\n", n+1);
            // генерация рандомного числа и отправка второму процессу
            number=rand()%20 + 1;
            MPI_Send(&number, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
            array = (char*)malloc((number + 1) * sizeof(char));
            // получение от второго процесса массива символов (слово)
            MPI_Recv(array, number + 1, MPI_CHAR, 2, 0, MPI_COMM_WORLD,
            MPI_STATUS_IGNORE);
            printf("Word from process 2: %s\n", array);
            printf("Length of word from process 2: %ld\n\n", strlen(array));
            free(array);
        }
        break;
        case 2: // второй процесс
        {
            char str[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!"; 
            // получение рандомного числа от первого процесса 
            MPI_Recv(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD,
            MPI_STATUS_IGNORE);
            array = (char*)malloc((number + 1) * sizeof(char));
            for (int i = 0; i < number; i++)
            {
                // получение рандомного символа из str
                // и запись в массив array
                int key = rand() % (int)(strlen(str));
                array[i]= str[key];
            }
            array[number] = '\0';
            // отправка первому процессу массива символов (слово)
            MPI_Send(array, number + 1, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
            free(array);
        }
        break;
        }
    }
    // Очистка состояния MPI
    MPI_Finalize();
    return 0;
}