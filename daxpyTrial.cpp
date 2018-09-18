#include<iostream>
#include<fstream>
#include<string>
#include <vector>
#include <sstream>
#include<algorithm>
#include<petsc.h>
#include<petscmat.h>

using namespace std;

int main(int argc, char *argv[])
{
    string row,vecrow;
    double element,vecElement;
    int rowNum = 0;
    double num;
    string filename = argv[1];
    string vecFilename = argv[2];
    fstream file;
    fstream vecFile;
    // cout<<filename<<endl;
    file.open(filename,ios::in);
    vecFile.open(vecFilename,ios::in);
    getline(file,row);
    stringstream ss(row);
    while(ss>>element){
        rowNum++;
    }
    file.clear();
    file.seekg(0, std::ios::beg);

    MPI_Comm comm;
    PetscInt rank, size;
    Mat A;
    Vec x,y,z;
    PetscBool printChoice = PETSC_FALSE ;
    PetscErrorCode ierr;

    PetscInitialize(&argc,&argv,(char *) 0,PETSC_NULL);
    comm = PETSC_COMM_WORLD;
    PetscPrintf(comm,"element of colums %d \n",rowNum);
    MPI_Comm_rank(comm,&rank);MPI_Comm_size(comm, &size);

    PetscOptionsGetBool(NULL,NULL,"-print",&printChoice,NULL);

    MatCreate(comm,&A);
    VecCreate(comm,&x);
    MatSetSizes(A,PETSC_DECIDE,PETSC_DECIDE,rowNum,rowNum);
    VecSetSizes(x,PETSC_DECIDE,rowNum);
    MatSetFromOptions(A);
    VecSetFromOptions(x);
    MatMPIAIJSetPreallocation(A,0.01*rowNum,PETSC_NULL,0.01*rowNum,PETSC_NULL);
    MatSeqAIJSetPreallocation(A,0.01*rowNum,PETSC_NULL);
    MatMPIDenseSetPreallocation(A,PETSC_NULL);
    MatSeqDenseSetPreallocation(A,PETSC_NULL);

    VecDuplicate(x,&y);
    VecDuplicate(x,&z);

    if (rank==0){
        double *dataArray = new double[rowNum];
        int *colList = new int[rowNum];
        int *rowList = new int[rowNum];

        for(int i = 0; i<rowNum;i++){
            fill_n(dataArray,rowNum,0);
            fill_n(rowList,rowNum,i);
            fill_n(colList,rowNum,0);
            getline(file,row);
            getline(vecFile,vecrow);
            ss.clear();
            stringstream ss2;
            ss.str(row);
            for(int j =0;j<rowNum;j++){
                ss>>element;
                dataArray[j]=element;
                colList[j]=j;
            }
            ss2.str(vecrow);
            ss2>>vecElement;
            // PetscPrintf(comm,"%f\n",vecElement);
            VecSetValue(x,i,vecElement,INSERT_VALUES);
            // PetscPrintf(comm,"%f  %f  %f\n",dataArray[0],dataArray[1],dataArray[2]);
            MatSetValues(A,1,rowList,rowNum,colList,dataArray,INSERT_VALUES);
        }
        delete [] dataArray;
    }
    MatAssemblyBegin(A,MAT_FINAL_ASSEMBLY);
    MatAssemblyEnd(A,MAT_FINAL_ASSEMBLY);
    VecAssemblyBegin(x);
    VecAssemblyEnd(x);
    VecCopy(x,z);


    MatMultAdd(A,x,z,y);
    // PetscPrintf(comm,printChoice);
    if (printChoice){
        PetscPrintf(comm,"Matrices and vector are:\n");
        MatView(A,PETSC_VIEWER_STDOUT_WORLD);
        VecView(y,PETSC_VIEWER_STDOUT_WORLD);
    }

    PetscFinalize();
    return 0;
}
