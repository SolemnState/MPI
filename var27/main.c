/*
 Процесс-1 передает число процессу-2. 
 Процесс-2 умножает число на 2 и пересылает результат процессу-1.
 Процесс-1 добавляет к числу 1 и пересылает результат процессу-2.
 Повторить 5 раз. Результат вывести на экран.
*/
 // Чтобы скомпилировать:
 // mpicc main.c
 // Чтобы запустить:
 // mpirun -n 3 a.out

#include <stdio.h>
#include <mpi/mpi.h>
#include <stdlib.h>
#include <string.h>

#define CYCLES 5
#define NUMBER 5

int main(int argc, char** argv)
{   
    int number;
    int cycles = CYCLES;
    int size, rank;
    MPI_Status status;
    
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
    if (rank == 1)
         number = NUMBER;
    if (rank == 0)
        printf("Our number: %d\n", number);
    // Цикл    
    for (int n = 0; n < cycles; n++)
    {
        switch(rank)
        {
        case 1: // первый процесс
        {
            printf("Iteration number: %d\n", n+1);
            // генерация рандомного числа и отправка второму процессу
            MPI_Send(&number, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
            // получение числа от второго процесса
            MPI_Recv(&number, 1, MPI_INT, 2, 0, MPI_COMM_WORLD,
            MPI_STATUS_IGNORE);
            printf("Number from process 2: %d\n", number);
            number += 1;
        }
        break;
        case 2: // второй процесс
        { 
            // получение рандомного числа от первого процесса 
            MPI_Recv(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD,
            MPI_STATUS_IGNORE);
            number *= 2;
            // отправка первому процессу числа, умноженного на 2
            MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        }
        break;
        }
    }
    // Очистка состояния MPI
    MPI_Finalize();
    return 0;
}