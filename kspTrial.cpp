#include <iostream>
#include<petsc.h>
#include<petscmat.h>
#include<petscksp.h>

using namespace std;

int main(int argc, char *argv[])
{
    Mat A;
    Vec b,x;
    MPI_Comm comm;
    PetscInt rank,size;

    PetscInitialize(&argc, &argv, (char *)0, PETSC_NULL);
    comm = PETSC_COMM_WORLD;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    MatCreate(comm,&A);
    MatSetSizes(A,PETSC_DECIDE,PETSC_DECIDE,3,3);
    MatSetFromOptions(A);

    MatMPIAIJSetPreallocation(A,2,PETSC_NULL,2,PETSC_NULL);
    MatSeqAIJSetPreallocation(A,2,PETSC_NULL);

    // MatGetOwnershipRange(A,)
    MatSetValue(A,0,0,1,INSERT_VALUES);
    MatSetValue(A,0,1,2,INSERT_VALUES);
    MatSetValue(A,0,2,1,INSERT_VALUES);
    MatSetValue(A,1,0,2,INSERT_VALUES);
    MatSetValue(A,1,1,1,INSERT_VALUES);
    MatSetValue(A,1,2,1,INSERT_VALUES);
    MatSetValue(A,2,0,1,INSERT_VALUES);
    MatSetValue(A,2,1,1,INSERT_VALUES);
    MatSetValue(A,2,2,2,INSERT_VALUES);

    MatAssemblyBegin(A,MAT_FINAL_ASSEMBLY);
    MatAssemblyEnd(A,MAT_FINAL_ASSEMBLY);
    MatView(A,PETSC_VIEWER_STDOUT_WORLD);
    PetscFinalize();
    return 0;
}
