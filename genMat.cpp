#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<iomanip>

using namespace std;

int main(int argc, char const *argv[])
{
    if (argc!=4){
        cout<<"Improper arguements\n";
        return -1;
    }
    string filename(argv[1]);
    string vecFilename(argv[2]);
    int dim = atoi(argv[3]);

    fstream file;
    file.open(filename,ios::out);
    fstream vecFile;
    vecFile.open(vecFilename,ios::out);


    for (int j = 0;j<dim;j++){
        for (int i = 0;i<dim;i++){
            if(i==j){
                file<<i+1<<" ";
            }else{
                file<<0<<" ";
            }
        }
        file<<"\n";
        vecFile<<j+1<<"\n";
    }
    file.close();
    vecFile.close();
    return 0;
}
