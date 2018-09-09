#include<iostream>
#include<chrono>
#include<random>
#include<petsc.h>
#include<petscmat.h>

using namespace std;

int main(int argc, char *argv[])
{
    MPI_Comm comm;
    PetscInt rank, size, approximateRows, _tempRemainder, _tempIndex;
    Mat A, B, C;
    PetscInt m,low,high,diagElements,nonDiagElements,indexSelect;
    PetscBool mGiven = PETSC_FALSE;
    // double value,values[m];

    uniform_real_distribution<double> randomDouble(0,100000);
    default_random_engine eng;

    PetscInitialize(&argc, &argv, (char *) 0, PETSC_NULL);
    comm = PETSC_COMM_WORLD;
    MPI_Comm_rank(comm,&rank);
    MPI_Comm_size(comm,&size);
    PetscOptionsGetInt(PETSC_NULL,PETSC_NULL, "-m", &m,&mGiven);
    if(mGiven==PETSC_FALSE){
        PetscPrintf(comm,"Improper Arguements\n");
        return -1;
    }

    MatCreate(comm,&A);
    MatSetSizes(A,PETSC_DECIDE,PETSC_DECIDE, m,m);
    MatSetFromOptions(A);
    
    approximateRows = (int)m/size;
    _tempRemainder = m%size;

    if (_tempRemainder>rank){
        approximateRows+=1;
    }
    
    diagElements = static_cast<int>(0.2*approximateRows);
    nonDiagElements = static_cast<int>(0.2*(m-approximateRows));

    MatMPIAIJSetPreallocation(A,diagElements,PETSC_NULL,nonDiagElements,PETSC_NULL);
    MatSeqAIJSetPreallocation(A,4,PETSC_NULL);
    
    MatGetOwnershipRange(A,&low,&high);
    PetscSynchronizedPrintf(PETSC_COMM_WORLD,"rank = %d:  Low, High = %d,%d\n Calculated Rows:%d\n", rank,low,high,approximateRows);
    PetscSynchronizedFlush(comm,PETSC_STDOUT);
    // int *rowVal = new int[diagElements+nonDiagElements]{0};
    // int *colVal = new int[diagElements+nonDiagElements]{0};
    // double *values = new double[diagElements+nonDiagElements]{0};
    
    // int rowVal, colVal;
    // double values;
    // uniform_int_distribution<> diagIndex(low,high-1);
    // // default_random_engine diagIndices;
    // for (int i = low; i<high;i++){
    //     // memset(values, 0, sizeof(values));
    //     if (low==0){
    //         uniform_int_distribution<> nonDiagIndex1(high,m-1);
    //         uniform_int_distribution<> nonDiagIndex2(high,m-1);
    //         uniform_int_distribution<> selector(0,1);
    //         //for diagonal elements
    //         for(int j = 0;j<diagElements;j++){
    //             rowVal=i;
    //             colVal =diagIndex(eng);
    //             values  = randomDouble(eng);
    //             MatSetValue(A,rowVal,colVal,values,INSERT_VALUES);
    //         }
    //         //for non diagonal elements
    //         for(int j = diagElements; j<(nonDiagElements+nonDiagElements);j++){
    //             indexSelect = selector(eng);
    //             values = randomDouble(eng);
    //             _tempIndex = nonDiagIndex1(eng)*indexSelect + abs(indexSelect-1)*nonDiagIndex2(eng);
    //             rowVal = i;
    //             colVal = _tempIndex;
    //             MatSetValue(A,rowVal,colVal,values,INSERT_VALUES);
    //         }
    //     } else if (high==m){
    //         uniform_int_distribution<> nonDiagIndex1(0,low-1);
    //         uniform_int_distribution<> nonDiagIndex2(0,low-1);
    //         uniform_int_distribution<> selector(0,1);
    //         //for diagonal elements
    //         for(int j = 0;j<diagElements;j++){
    //             rowVal=i;
    //             colVal =diagIndex(eng);
    //             values  = randomDouble(eng);
    //             MatSetValue(A,rowVal,colVal,values,INSERT_VALUES);
    //         }
    //         //for non diagonal elements
    //         for(int j = 0; j<(nonDiagElements+nonDiagElements);j++){
    //             indexSelect = selector(eng);
    //             values = randomDouble(eng);
    //             _tempIndex = nonDiagIndex1(eng)*indexSelect + abs(indexSelect-1)*nonDiagIndex2(eng);
    //             rowVal = i;
    //             colVal = _tempIndex;
    //             MatSetValue(A,rowVal,colVal,values,INSERT_VALUES);
    //         }
    
    //     } else{
    //         uniform_int_distribution<> nonDiagIndex1(0,low-1);
    //         uniform_int_distribution<> nonDiagIndex2(high,m-1);
    //         uniform_int_distribution<> selector(0,1);
    //         //for diagonal elements
    //         for(int j = 0;j<diagElements;j++){
    //             rowVal=i;
    //             colVal =diagIndex(eng);
    //             values  = randomDouble(eng);
    //             MatSetValue(A,rowVal,colVal,values,INSERT_VALUES);
    //         }
    //         //for non diagonal elements
    //         for(int j = 0; j<(nonDiagElements+nonDiagElements);j++){
    //             indexSelect = selector(eng);
    //             values = randomDouble(eng);
    //             _tempIndex = nonDiagIndex1(eng)*indexSelect + abs(indexSelect-1)*nonDiagIndex2(eng);
    //             rowVal = i;
    //             colVal = _tempIndex;
    //             MatSetValue(A,rowVal,colVal,values,INSERT_VALUES);
    //         }
    //     }
    // }
    MatAssemblyBegin(A,MAT_FINAL_ASSEMBLY);
    MatAssemblyEnd(A,MAT_FINAL_ASSEMBLY);
    PetscFinalize();
    MatView(A,PETSC_VIEWER_STDOUT_WORLD);
    return 0;
}
