#include<iostream>
#include<chrono>
#include<petsc.h>
#include<petscmat.h>

using namespace std;

int main(int argc, char *argv[])
{
    MPI_Comm comm;
    PetscInt rank;
    Mat A, B, C;
    PetscInt m,low,high,totalRow,totalCol, localRow, localCol;
    PetscBool mGiven = PETSC_FALSE;

    PetscInitialize(&argc, &argv, (char *) 0, PETSC_NULL);
    comm = PETSC_COMM_WORLD;
    MPI_Comm_rank(comm,&rank);
    PetscOptionsGetInt(PETSC_NULL,PETSC_NULL, "-m", &m,&mGiven);
    if(mGiven==PETSC_FALSE){
        PetscPrintf(comm,"Improper Arguements\n");
        return -1;
    }

    MatCreate(comm,&A);
    MatSetSizes(A,PETSC_DECIDE,PETSC_DECIDE, m,m);
    MatSetFromOptions(A);
    MatMPIAIJSetPreallocation(A,2,PETSC_NULL,2,PETSC_NULL);
    MatSeqAIJSetPreallocation(A,4,PETSC_NULL);
    MatGetOwnershipRange(A,&low,&high);
    PetscSynchronizedPrintf(PETSC_COMM_WORLD,"rank = %d:  Low, High = %d,%d\n", rank,low,high);
    MatGetLocalSize(A,&localRow,&localCol);
    // MatGetSize(A,&totalRow,&totalCol);
    PetscSynchronizedPrintf(PETSC_COMM_WORLD,"Local Rows %d, Col %d \n",localRow,localCol);
    PetscSynchronizedFlush(comm,PETSC_STDOUT);
    // PetscSynchronizedFlush(comm,PETSC_STDOUT);

    // for(int i = low; i<high;i++){

    // }
    
    // MatSetValue()
    PetscFinalize();
    return 0;
}
