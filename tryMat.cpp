#include<iostream>
#include<chrono>
#include<random>
#include<petsc.h>
#include<petscmat.h>
#include<thread>
#include<vector>

using namespace std;

int main(int argc, char *argv[])
{
    MPI_Comm comm;
    PetscInt sum=0, rank, size, approximateRows, _tempRemainder, _tempIndex;
    Mat A, B, C;
    PetscInt m,low,lowb,highb,high,diagElements,nonDiagElements,indexSelect;
    PetscBool mGiven = PETSC_FALSE;
    PetscScalar value;
    int pr =0;
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
    PetscOptionsGetInt(PETSC_NULL,PETSC_NULL, "-showMat", &pr,&mGiven);

    MatCreate(comm,&A);
    MatSetSizes(A,PETSC_DECIDE,PETSC_DECIDE, m,m);
    MatSetFromOptions(A);
    MatCreate(comm,&B);
    MatSetSizes(B,PETSC_DECIDE,PETSC_DECIDE, m,m);
    MatSetFromOptions(B);


    approximateRows = (int)m/size;
    _tempRemainder = m%size;

    if (_tempRemainder>rank){
        approximateRows+=1;
    }
    
    diagElements = static_cast<int>(0.05*approximateRows);
    nonDiagElements = static_cast<int>(0.05*(m-diagElements));


    MatMPIAIJSetPreallocation(A,diagElements,PETSC_NULL,nonDiagElements,PETSC_NULL);
    MatSeqAIJSetPreallocation(A,nonDiagElements+diagElements,PETSC_NULL);
    MatMPIAIJSetPreallocation(B,diagElements,PETSC_NULL,nonDiagElements,PETSC_NULL);
    MatSeqAIJSetPreallocation(B,nonDiagElements+diagElements,PETSC_NULL);

    MatGetOwnershipRange(A,&low,&high);
    MatGetOwnershipRange(B,&lowb,&highb);

    //PetscSynchronizedPrintf(PETSC_COMM_WORLD,"A rank = %d:  Low, High = %d,%d\n Calculated Rows:%d\n", rank,low,high,approximateRows);
    //PetscSynchronizedPrintf(PETSC_COMM_WORLD,"B rank = %d:  Low, High = %d,%d\n Calculated Rows:%d\n", rank,lowb,highb,approximateRows);
    //PetscSynchronizedFlush(comm,PETSC_STDOUT);

    int diagElementsPerRank = static_cast<int>(diagElements/size);
    int nonDiagElementsPerRank = static_cast<int>(nonDiagElements/size);
    int rowVal, colVal;
    double values;
    uniform_int_distribution<> diagIndex(low,high-1);
    // cout<<low<<" / "<< (m-high)<<"="<< (float)low/(float)(m-high)<<endl;

    for (int i = low; i<high;i++){
        if (low==0){
            uniform_int_distribution<> nonDiagIndex1(high,m-1);
            uniform_int_distribution<> nonDiagIndex2(high,m-1);
            uniform_int_distribution<> selector(0,1);
            //for diagonal elements
            for(int j = 0;j<diagElementsPerRank;j++){
                rowVal=i;
                colVal =diagIndex(eng);
                values  = randomDouble(eng);
                MatSetValue(A,rowVal,colVal,values,INSERT_VALUES);
                colVal =diagIndex(eng);
                values  = randomDouble(eng);
                MatSetValue(B,rowVal,colVal,values,INSERT_VALUES);
            }
            //for non diagonal elements
            for(int j = 0; j<(nonDiagElementsPerRank);j++){
                values = randomDouble(eng);
                _tempIndex = nonDiagIndex1(eng);
                rowVal = i;
                colVal = _tempIndex;
                MatSetValue(A,rowVal,colVal,values,INSERT_VALUES);
                _tempIndex = nonDiagIndex1(eng);
                colVal = _tempIndex;
                values = randomDouble(eng);
                MatSetValue(B,rowVal,colVal,values,INSERT_VALUES);
            }
        } else if (high==m){
            uniform_int_distribution<> nonDiagIndex1(0,low-1);
            uniform_int_distribution<> nonDiagIndex2(0,low-1);
            uniform_int_distribution<> selector(0,1);
            //for diagonal elements
            for(int j = 0;j<diagElementsPerRank;j++){
                rowVal=i;
                colVal =diagIndex(eng);
                values  = randomDouble(eng);
                MatSetValue(A,rowVal,colVal,values,INSERT_VALUES);
                colVal =diagIndex(eng);
                values  = randomDouble(eng);
                MatSetValue(B,rowVal,colVal,values,INSERT_VALUES);
            }
            //for non diagonal elements
            for(int j = 0; j<(nonDiagElementsPerRank);j++){
                // indexSelect = selector(eng);
                values = randomDouble(eng);
                _tempIndex = nonDiagIndex1(eng);//*indexSelect + abs(indexSelect-1)*nonDiagIndex2(eng);
                rowVal = i;
                colVal = _tempIndex;
                MatSetValue(A,rowVal,colVal,values,INSERT_VALUES);
                _tempIndex = nonDiagIndex1(eng);//*indexSelect + abs(indexSelect-1)*nonDiagIndex2(eng);
                colVal = _tempIndex;
                values = randomDouble(eng);
                MatSetValue(B,rowVal,colVal,values,INSERT_VALUES);
            }
    
        } else{
            uniform_int_distribution<> nonDiagIndex1(0,low-1);
            uniform_int_distribution<> nonDiagIndex2(high,m-1);
            uniform_int_distribution<> selector(0,1);
            // vector<int> indexRange{0,low,high-1,m-1}, weights {low,0,m-high};
            // piecewise_constant_distribution<int> selector(indexRange.begin(),indexRange.end(),weights.begin());
            //for diagonal elements
            for(int j = 0;j<diagElementsPerRank;j++){
                rowVal=i;
                colVal =diagIndex(eng);
                values  = randomDouble(eng);
                MatSetValue(A,rowVal,colVal,values,INSERT_VALUES);
                values  = randomDouble(eng);
                colVal =diagIndex(eng);
                MatSetValue(B,rowVal,colVal,values,INSERT_VALUES);
            }
            //for non diagonal elements
            for(int j = 0; j<(nonDiagElementsPerRank);j++){
                indexSelect = selector(eng);//<((float)low/(float)(m-high)))?1:0;
                values = randomDouble(eng);
                _tempIndex = nonDiagIndex1(eng)*indexSelect + abs(indexSelect-1)*nonDiagIndex2(eng);
                rowVal = i;
                colVal = _tempIndex;
                MatSetValue(A,rowVal,colVal,values,INSERT_VALUES);
                values = randomDouble(eng);
                _tempIndex = nonDiagIndex1(eng)*indexSelect + abs(indexSelect-1)*nonDiagIndex2(eng);
                colVal = _tempIndex;
                MatSetValue(B,rowVal,colVal,values,INSERT_VALUES);
            }
        }
    }
    auto begin = std::chrono::steady_clock::now();
    MatAssemblyBegin(A,MAT_FINAL_ASSEMBLY);
    MatAssemblyEnd(A,MAT_FINAL_ASSEMBLY);
    MatAssemblyBegin(B,MAT_FINAL_ASSEMBLY);
    MatAssemblyEnd(B,MAT_FINAL_ASSEMBLY);



    MatCreate(comm,&C);
    MatMatMultSymbolic(A,B,PETSC_DEFAULT,&C);
    MatMatMultNumeric(A,B,C);

    auto end = std::chrono::steady_clock::now();
    if(pr!=0){
        MatView(B,PETSC_VIEWER_DRAW_WORLD);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        MatView(A,PETSC_VIEWER_DRAW_WORLD);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        MatView(C,PETSC_VIEWER_DRAW_WORLD);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    
    PetscSynchronizedPrintf(PETSC_COMM_WORLD,"Time elapsed: %d ms\n",std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count());
    // cout<<value<<endl;
    PetscSynchronizedFlush(comm,PETSC_STDOUT);    
    PetscFinalize();
return 0;
}
