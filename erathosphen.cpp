#include <iostream>
#include <math.h>
#include <pthread.h>
#include "vector"
#include <limits>

#define NUMBER_PROCESSOR 6

typedef unsigned long long ull;

class Erathosven_class;



class Erathosven_class {
public:
    struct Thread_data{
        int ind;
        Erathosven_class*er;
    };
    pthread_mutex_t mu;
    int run_pthread;

    int COUNT_NUM;
    int BASIS;
    ull roof;
    std::vector<int> num;
    std::vector<int> num_ind;
    bool**_elements;

    
    void create_array(ull N){
        std::vector<pthread_t> pth;

        Thread_data*pth_dt  = (Thread_data*)malloc(sizeof(Thread_data)*COUNT_NUM);
        _elements           = (bool**)malloc(sizeof(bool*)*COUNT_NUM);
        
        roof = (N/30);
        
        for (int i=0; i<COUNT_NUM; i++){
            bool*tmp = (bool*)malloc(sizeof(bool)*roof);

            for(ull j=0; j < roof; j++){
                tmp[j] = 1;
            }
            _elements[i]=tmp;
        }


        for (int i=0; i<COUNT_NUM; i++){
            pthread_t t;

            if (run_pthread >= NUMBER_PROCESSOR){
                pthread_join(pth.front(), NULL);
                run_pthread--;
                pth.erase(pth.begin());

            }
            pth_dt[i].er = this;
            pth_dt[i].ind = i;

            pthread_create (&t, NULL, _remove_non_prime, &pth_dt[i]);
            
            run_pthread++;
            pth.push_back(t);
        }

        for(auto i : pth) pthread_join(i, NULL);
        run_pthread=0;
        
        
        // std::cout << "Find non-prime" << std::endl;
        pth.clear();
        delete (pth_dt);

        // std::cout << "complete" << std::endl;
    }

    static void* _remove_non_prime(void* data){
        // std::cout << "Thread " << pthread_getunique_np(pthread_self()) << "start" << std::endl;
        Thread_data*dt = (Thread_data*)data;
        pthread_mutex_t mu = dt->er->mu;

        std::vector<int>num_ind =  dt->er->num_ind;        
        std::vector<int>num =  dt->er->num;   
        ull roof =  dt->er->roof;
        int ind = dt->ind;
        ull BASIS = dt->er->BASIS;
        int COUNT_NUM = dt->er->COUNT_NUM;

        bool**el = dt->er->_elements;

        for (ull i = 0; i<roof; i++){
            ull f_dg = (i*BASIS + num[ind]);
            if (el[ind][i] == 0 || f_dg == 1) continue;
            if (f_dg > sqrt(roof*BASIS)) break;
            // if (f_dg > sqrt(roof*BASIS)) break;

            ull cur = 0;

            while (true){
                cur++;

                ull s_dg = ((cur/COUNT_NUM)*BASIS + num[cur%COUNT_NUM]);
                ull dg = f_dg * s_dg;

                if(s_dg < f_dg) continue;
                if (dg > roof*BASIS) break;

                int reminder = dg % BASIS;
                ull tind = num_ind[reminder];

                el[tind][ull(dg / BASIS)]=0;

            }
        }


        pthread_exit(0);
        return NULL;
    }

    ull get_count_prime(){
        ull cnt = 0;
        for (auto i=0; i<COUNT_NUM; i++)
            for (auto j=0; j<roof; j++)
                if (_elements[i][j] != 0) 
                    cnt++;
        return cnt;
    }
    
    std::vector<ull> get_arr(){
        std::vector<ull> tmp;
        for (ull j=0; j<roof; j++)
            for (ull i=0; i<COUNT_NUM; i++)
                    if (_elements[i][j] != 0) 
                        tmp.push_back(j*BASIS + num[i]);
        return tmp;
    }
    
    Erathosven_class(){
        SYSTEM_INFO sysinfo;
        GetSystemInfo(&sysinfo);

        run_pthread = 0;
        COUNT_NUM = 8;
        num = {1, 7, 11, 13, 17, 19, 23, 29};
        roof = 0;
        BASIS = 30;

        for (int i=0; i<BASIS; i++){ num_ind.push_back(-1); }
        for (int i=0; i<COUNT_NUM; i++){ num_ind[num[i]] = i; }

        pthread_mutex_init(&mu, NULL);
    }
};

std::vector<ull>erathosvenCircleFact(ull n, bool rang3=1){
    int COUNT_NUM, BASIS = 30;
    std::vector<int> num;

    if (rang3){
        BASIS = 30;
        num = {1, 7, 11, 13, 17, 19, 23, 29};
    }else{
        BASIS = 210;
        num = {1, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 121, 127, 131, 137, 139, 143, 149, 151, 157, 163, 167, 169, 173, 179, 181, 187, 191, 193, 197, 199, 209};
    }

    COUNT_NUM = num.size();
    ull roof = (n/BASIS);
    int num_ind[BASIS];

    bool**sv = (bool**)malloc(sizeof(bool*)*(COUNT_NUM));

    for (ull i=0; i<COUNT_NUM; i++) {
        bool*tmp = (bool*)malloc(sizeof(bool)*(roof));
        sv[i] = tmp;

        for(ull j=0; j<roof; j++) tmp[j] = 1;
        // std::cout << i << ' ';
    }

    for (int i=0; i<BASIS; i++){ num_ind[i] = -1; }
    for (int i=0; i<COUNT_NUM; i++){ 
        int ind = num[i];
        num_ind[ind] = i; 
    }

    for(ull i=1; i<roof*COUNT_NUM; i++){
        if (sv[i%COUNT_NUM][i/COUNT_NUM] == 0) continue;;
        for(ull j=i; j<roof*COUNT_NUM; j++){
            ull t = ((j/COUNT_NUM)*BASIS + num[j%COUNT_NUM]) * ((i/COUNT_NUM)*BASIS + num[i%COUNT_NUM]);
            
            if (t > n) break;

            int reminder = t % BASIS;
            ull ind = num_ind[reminder];
            // if ((t / BASIS)*8 + ind > roof) break;
            // if ((t / BASIS) %100000==0)
            //     std::cout <<(t / BASIS) << ' ' << ind << std::endl;
            sv[ind][(t / BASIS)] = 0;
        }
    }

    std::vector<ull> tmp = {0}; 
    for (ull i=0; i<COUNT_NUM; i++){
        for (ull j=0; j<roof; j++)
            if (sv[i][j]!= 0)
                // tmp.push_back((i/COUNT_NUM)*BASIS + num[i%COUNT_NUM]);
                tmp[0]++;    
        delete sv[i];
    }
    delete sv;
    return tmp;
}

std::vector<ull>  Sundaram(ull n){
    ull k = (n)/2;
    bool*a = (bool*)malloc(sizeof(bool)*(ull(k+1)));
    std::vector<ull > tmp;
    for (ull i=0; i<(k); i++){ a[i]=(false); }

    for(ull i=1; i<k+1; i++){
        ull j=i;
        while((ull)(i+j+2*i*j) <= k){
            a[(ull)(i+j+2*i*j)] = true;
            j++;
        }
    }
    
    tmp.push_back(0);
    for (int i=0; i<(k+1); i++){
        // if (a[i] == 0) { tmp.push_back(2*i + 1); }
        if (a[i] == 0) { tmp[0]++; }
    }
    delete a;
    return tmp;
}

std::vector<ull> AtkinBase(ull n){
    bool*a = (bool*)malloc(sizeof(bool)*(n));

    for(ull i = 0; i<n; i++) a[i] = 0;

    ull x2 = 0;
    for(ull i=1; i<sqrt(n); i++){
        ull y2 = 0;
        x2 += 2*i -1; // i*i
        for(ull j=1; j<sqrt(n); j++){
            ull tmp = 0;
            y2 += 2*j -1; // j*j
            
            tmp = (4*x2 + y2);

            if(tmp <= n && (tmp % 12 == 1 || tmp % 12 == 5)) a[tmp] = !a[tmp];
            
            // (3*i*i + j*j)
            tmp -= x2;
            if(tmp <= n && (tmp % 12 == 7)) a[tmp] = !a[tmp];

            if(i > j){
                tmp -= 2*y2;
                // (3*i*i - j*j)
                if(tmp <= n && tmp % 12 == 11) a[tmp] = !a[tmp];
            }
        }
    }
    
    std::vector<ull> sv = {0};
    // std::vector<ull> sv;
    for(ull i=5; i<n; i++){
        if (a[i]){
            // sv.push_back(i);
            sv[0]++;
            for(ull j=i*i; j<n; j+=i*i){
                a[j] = 0;
            }
        }
    }

    return sv;
}

std::vector<ull> Atkin2(ull n){
    bool*a = (bool*)malloc(sizeof(bool)*(n));
    const std::vector<short> rem_int1 = { 1, 13, 17, 29, 37, 41, 49,53 };
    const std::vector<short> rem_int2 = { 1, 7, 13, 19, 31, 37, 43, 49};
    const std::vector<short> rem_int3 = { 11, 23, 47, 59};
    bool rem1[60] = {};
    bool rem2[60] = {};
    bool rem3[60] = {};

    for(ull i = 0; i<n; i++) a[i] = 0;
    for(int i=0; i<60; i++){ rem1[i] = 0; rem2[i] = 0; rem3[i] = 0; }
    for(int i=0; i<rem_int1.size(); i++){ rem1[rem_int1[i]] = 1; }
    for(int i=0; i<rem_int2.size(); i++){ rem2[rem_int2[i]] = 1; }
    for(int i=0; i<rem_int3.size(); i++){ rem3[rem_int3[i]] = 1; }

    ull x2 = 0;
    for(ull i=1; i<sqrt(n); i++){
        x2 += 2*i -1; // i*i
        ull y2 = 0;
        ull j = 1;

        while(j < sqrt(n)){
            y2 = j*j;
            ull tmp = 4*x2 + y2;
            
            if(tmp < n && rem1[tmp%60])
                a[tmp] = !a[tmp];
            j+=1;

            y2 = j*j;
            tmp = 3*x2 + y2;
            
            if(tmp < n && rem2[tmp%60])
                a[tmp] = !a[tmp];
            j++;
        }

        y2 = 0;
        j = i%2 + 1;
        while(j <= i){
            y2 = j*j;
            ull tmp = 3*x2 - y2;
            
            if(tmp < n && rem3[tmp%60])
                a[tmp] = !a[tmp];
            j++;
        }


    }
    
    std::vector<ull> sv = {0};

    for(ull i=7; i<n; i++){
        if (a[i]){
            // sv.push_back(i);
            sv[0]++;
            for(ull j=i*i; j<n; j+=i*i){
                a[j] = 0;
            }
        }
    }

    return sv;
}