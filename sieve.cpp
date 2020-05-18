#include <ctime>
#include <mutex>
#include "iostream"
#include "vector"
#include "pthread.h"
#include "erathosphen.cpp"

std::vector<int> erathosvenBase(unsigned int n){
    std::vector<int> sv;
    std::vector<int> tmp;

    for (int i=0; i<n; i++){ sv.push_back(i); }
    for (int i=2; i<n; i++){
        if (sv[i] != 0 && sv[i]*sv[i] < n){
            unsigned int t = i;
            unsigned int ind = sv[i] * t; 
            while (ind < n){
                sv[ind] = 0;
                t++;
                ind = sv[i] * t; 
            }
        }
    }
    for (int i=0; i<sv.size(); i++){ if (sv[i] != 0) tmp.push_back(i); }
    return tmp;
}

int main(){
    Erathosven_class er;
    unsigned int st_time, end_time;
    std::vector<int> t;
    ull x = 9900000000;
    std::cout << "N: " << x << std::endl;

    /* ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ */
    // std::cout << "start Sundaram alg: ";
        
    // st_time = clock();
    // std::cout << Sundaram(x).size() << std::endl;
    // end_time = clock();

    // std::cout << "in time: " << end_time - st_time << std::endl;
    // /* ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ */
    // std::cout << "start base alg: ";
    
    // st_time = clock();
    // t = erathosvenBase(x);
    // end_time = clock();

    // std::cout  << (t).size() << std::endl;
    // t.clear();

    // std::cout << "in time: " << end_time - st_time << std::endl;

    // /* ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ */
    // std::cout << "start circle fact: ";

    // st_time = clock();
    // std::vector<ull> ptr= erathosvenCircleFact(x);
    // end_time = clock();
    
    // std::cout  << ptr.size() << std::endl;
    // // for (auto i : ptr) cout << i << ' ';
    // ptr.clear();
    // std::cout << "in time: " << end_time - st_time << std::endl;
    // std::cout << std::endl;

    // /* ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ */
    std::cout << "start thread: ";
    
    st_time = clock();
    er.create_array(x);
    end_time = clock();

    std::cout<< er.get_count_prime() << std::endl;  
    
    std::cout << std::endl << "in time: " << end_time - st_time << std::endl;

    // std::cout << cnt << std::endl;
    // std::cout << std::endl << er._elements.size() << std::endl;
    // std::cout << er.BASIS << std::endl;
    // std::cout << er.COUNT_NUM << std::endl;
    // std::cout << er.low.size() << std::endl;
    return 0;
}