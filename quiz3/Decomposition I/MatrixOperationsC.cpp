#include <iostream>
#include <vector>
#include <cstdlib>
#include <mpi.h>

using namespace std;

const int TAG_DIM = 0;
const int TAG_DATA = 1;
const int TAG_RESULT = 2;

void fillMatrix(vector<vector<int>>& mat) {
    for(auto& row : mat)
        for(auto& val : row)
            val = rand() % 10;
}

void printMatrix(const vector<vector<int>>& mat) {
    for(const auto& row : mat) {
        for(int val : row) cout << val << " ";
        cout << endl;
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(size < 4) {
        if(rank == 0) cerr << "Se necesitan al menos 4 procesos MPI\n";
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    if(rank == 0) {
        int matSize = atoi(argv[1]);
        vector<vector<int>> matrixA(matSize, vector<int>(matSize));
        vector<vector<int>> matrixB(matSize, vector<int>(matSize));
        
        fillMatrix(matrixA);
        fillMatrix(matrixB);

        double startTime = MPI_Wtime();

        for(int dest = 1; dest <= 3; dest++) {
            MPI_Send(&matSize, 1, MPI_INT, dest, TAG_DIM, MPI_COMM_WORLD);
            
            int* buffer = new int[matSize*matSize];
            for(int i = 0; i < matSize; i++) {
                for(int j = 0; j < matSize; j++) {
                    buffer[i*matSize + j] = (dest == 3) ? matrixA[i][j] : matrixB[i][j];
                }
            }
            
            MPI_Send(buffer, matSize*matSize, MPI_INT, dest, TAG_DATA, MPI_COMM_WORLD);
            delete[] buffer;
        }

        vector<vector<int>> sum(matSize, vector<int>(matSize));
        vector<vector<int>> product(matSize, vector<int>(matSize));
        vector<vector<int>> transpose(matSize, vector<int>(matSize));
        
        MPI_Recv(&sum[0][0], matSize*matSize, MPI_INT, 1, TAG_RESULT, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&product[0][0], matSize*matSize, MPI_INT, 2, TAG_RESULT, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&transpose[0][0], matSize*matSize, MPI_INT, 3, TAG_RESULT, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        double endTime = MPI_Wtime();

        cout << "Suma:\n"; printMatrix(sum);
        cout << "\nProducto:\n"; printMatrix(product);
        cout << "\nTranspuesta:\n"; printMatrix(transpose);
        cout << "\nTiempo: " << endTime - startTime << "s" << endl;
    }
    else {
        int matSize;
        MPI_Recv(&matSize, 1, MPI_INT, 0, TAG_DIM, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        int* buffer = new int[matSize*matSize];
        MPI_Recv(buffer, matSize*matSize, MPI_INT, 0, TAG_DATA, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        vector<vector<int>> result(matSize, vector<int>(matSize));

        switch(rank) {
            case 1:
                for(int i = 0; i < matSize; i++)
                    for(int j = 0; j < matSize; j++)
                        result[i][j] = buffer[i*matSize + j] + buffer[i*matSize + j];
                break;
                
            case 2:
                for(int i = 0; i < matSize; i++)
                    for(int j = 0; j < matSize; j++)
                        for(int k = 0; k < matSize; k++)
                            result[i][j] += buffer[i*matSize + k] * buffer[k*matSize + j];
                break;
                
            case 3:
                for(int i = 0; i < matSize; i++)
                    for(int j = 0; j < matSize; j++)
                        result[j][i] = buffer[i*matSize + j];
                break;
        }

        MPI_Send(&result[0][0], matSize*matSize, MPI_INT, 0, TAG_RESULT, MPI_COMM_WORLD);
        delete[] buffer;
    }

    MPI_Finalize();
    return 0;
}
