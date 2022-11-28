#include <iostream>
#include <mpi.h>
#include <bits/stdc++.h>

using namespace std;

void initialize(int number, int array[]) {
    for (int i = 2; i < number + 2; i++) {
        array[i - 2] = i;
    }
}

template<typename T>
void printArray(T array[], int size) {
    for (int i = 0; i < size; i++) {
        printf("%d\t", array[i]);
    }
    cout << endl;
}

void mark(const int array[], int marked[], int end, int number) {
    for (int i = number - 1; i <= end; ++i) {
        if (!marked[i] && array[i] % number == 0) {
            marked[i] = true;
        }
    }
}

void printMarked(int *array, const int *marked, int size) {
    for (int i = 0; i < size; ++i) {
        if (!marked[i]) {
            printf("%d\t", array[i]);
        }
    }
    cout << endl;
}

int main(int argc, char *argv[]) {
    int P = 4, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &P);
    int array_size = stoi(argv[1]) - 1;
    int array[array_size];
    int *marked = new int[array_size]{0};
    int cluster_size = array_size / P;

    initialize(array_size, array);
    if (rank == 0) {
        for (int i = 1; i < P; ++i) {
            int start = i * cluster_size;
            MPI_Send(&start, 1, MPI_INT, i, i, MPI_COMM_WORLD);
        }
        for (int i = 0; i < cluster_size; ++i) {
            MPI_Bcast(array + i, 1, MPI_INT, 0, MPI_COMM_WORLD);
            mark(array, marked, cluster_size, array[i]);


        }
        delete[] marked;
        MPI_Finalize();
        return 0;

    }

    int start = 0, buffer = 0;

    MPI_Recv(&start, 1, MPI_INT, 0, rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    for (int i = start; i < start + cluster_size; ++i) {
        MPI_Bcast(&buffer, 1, MPI_INT, 0, MPI_COMM_WORLD);
        int end = (cluster_size * rank) + cluster_size;
        mark(array, marked, end, buffer);


    }

    if (rank == P - 1) {
        printMarked(array, marked, array_size);
    }
    delete[] marked;
    MPI_Finalize();
    return 0;
}
