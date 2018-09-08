#include<petsc.h>
#include<petscmat.h>
#include<iostream>
#include <chrono>

using namespace std;

int main(int argc, char *argv[])
{
    MPI_Comm comm;
    PetscInt rank;
    Mat A;
    Vec V1, V2, V3;
    PetscInt m=0,n=0,low,high;
    PetscScalar value;
    PetscBool mFound = PETSC_FALSE, nFound = PETSC_FALSE;
    PetscInitialize(&argc, &argv, (char *) 0, PETSC_NULL);
    comm = PETSC_COMM_WORLD;
    MPI_Comm_rank(comm,&rank);
    PetscOptionsGetInt(PETSC_NULL,PETSC_NULL, "-m",&m, &mFound);
    PetscOptionsGetInt(PETSC_NULL,PETSC_NULL, "-n",&n, &nFound);
    // cout<<index<<endl;
    if (!(mFound==PETSC_TRUE&&nFound==PETSC_TRUE)){
        cout<<"Improper Arguements!"<<endl;
        return -1;
    } 
    // else {
    //     cout<<"m="<<m<<";  n="<<n<<endl;
    // }
    PetscErrorCode ierr;
    // MatCreate(comm,&A);
    // MatSetFromOptions(A);
    // MatSetSizes(A,PETSC_DECIDE, PETSC_DECIDE, 4,4);
    // MatMPIAIJSetPreallocation(A,3,PETSC_NULL,);
    // VecCreateMPI(comm);
    auto begin = std::chrono::steady_clock::now();
    VecCreate(PETSC_COMM_WORLD, &V1);
    // VecSetSizes(V1,m,PETSC_DETERMINE);
    VecSetSizes(V1,PETSC_DECIDE,m);
    VecSetFromOptions(V1);
    // VecSetType(V1,VECMPI);
    VecSet(V1,123.4);
    VecAssemblyBegin(V1);
    VecAssemblyEnd(V1);
    VecGetOwnershipRange(V1, &low, &high);
    const PetscInt index1 = low;
    VecGetValues(V1,1,&index1,&value);
    // VecView(V1,PETSC_VIEWER_STDOUT_WORLD);
    // cout<<value<<endl;

    VecCreate(comm, &V2);
    VecSetSizes(V2,PETSC_DECIDE,m);
    VecSetFromOptions(V2);
    // VecSetType(V2,VECMPI);
    VecSet(V2,123.4);
    VecAssemblyBegin(V2);
    VecAssemblyEnd(V2);
    VecGetOwnershipRange(V2, &low, &high);
    const PetscInt index2 = low;
    VecGetValues(V2,1,&index2,&value);
    // VecView(V2,PETSC_VIEWER_STDERR_WORLD);
    // cout<<value<<endl;
    VecCreate(comm,&V3);
    VecSetSizes(V3,PETSC_DECIDE,m);
    VecSetFromOptions(V3);
    VecWAXPY(V3,10,V1,V2);
    VecAssemblyBegin(V3);
    VecAssemblyEnd(V3);
    VecGetOwnershipRange(V3, &low, &high);
    const PetscInt index3 = low;
    VecGetValues(V3,1,&index3,&value);
    // VecView(V3,PETSC_VIEWER_STDOUT_WORLD);
    auto end = std::chrono::steady_clock::now();
    cout<<"Time elapsed: "<<std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count()<<"ms"<<endl;
    cout<<value<<endl;

    PetscFinalize();
    return 0;
}
