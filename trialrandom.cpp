#include<iostream>
#include<random>

int main(int argc, char const *argv[])
{
    int result,sum=0,low,high;
    double resultD;
    std::cin>>low>>high;
    std::default_random_engine eng;
    std::uniform_int_distribution<> intGen(0,1);
    std::uniform_real_distribution<float> floatGen(0,1);
    for(int i =0 ; i<1000; i++){
        // std::cout<<intGen(eng)<<"\n";
        result = (floatGen(eng)<0.3)?1:0;
        // std::cout<<result<<"\n";
        sum+=result;
    }
    std::cout<<sum<<std::endl;
    std::cout<<static_cast<float>(1)/static_cast<float>(2)<<"\n";
    std::piecewise_constant_distribution<int> distr()
    return 0;
}
