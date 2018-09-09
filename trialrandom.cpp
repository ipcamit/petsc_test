#include<iostream>
#include<random>

int main(int argc, char const *argv[])
{
    int result;
    double resultD;

    std::default_random_engine eng;
    std::uniform_int_distribution<> intGen(0,1);
    std::uniform_real_distribution<double> floatGen(0,10000);
    for(int i =0 ; i<10; i++){
        std::cout<<intGen(eng)<<"\n";
        std::cout<<floatGen(eng)<<"\n";
    }
    intGen(5,6);
    for(int i =0 ; i<10; i++){
        std::cout<<intGen(eng)<<"\n";
        std::cout<<floatGen(eng)<<"\n";
    }
    return 0;
}
