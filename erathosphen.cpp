#include <iostream>
#include <math.h>
#include <pthread.h>
#include "vector"
#include <limits>

using namespace std;

typedef unsigned long long ull;

class Erathosven_class;

struct Thread_data{
    int ind;
    Erathosven_class*er;
};

class Erathosven_class {
public:
    pthread_mutex_t mu;
    int COUNT_NUM;
    int BASIS;
    int roof;
    vector<int> num;
    vector<int> num_ind;
    vector<int> low;
    vector<vector<int> > rm;
    vector<vector<int>> _elements;

    
    void create_array(int N){
        vector<pthread_t> pth;
        Thread_data*pth_dt = (Thread_data*)malloc(sizeof(Thread_data)*COUNT_NUM);
        roof = N;
        
        for (int i=0; i<COUNT_NUM; i++){
            vector<int> tmp;

            tmp.push_back(num[i]);
            if (i > 0) { 
                if (num[i] < sqrt(N)) low.push_back(num[i]);
            }
            _elements.push_back(tmp);
            
            pth_dt[i].ind = i;
            pth_dt[i].er = this;

            pthread_t t;
            int flag = pthread_create (&t, NULL, _create_array, &pth_dt[i]);

            if (flag != 0){
                std::cout << "Error thread";
                exit(1);
            }

            pth.push_back(t);
        }

        for(auto i=0; i<COUNT_NUM; i++){ pthread_join(pth[i], NULL); }
        // std::cout << "Sieve fill" << std::endl;
        pth.clear();

        for (int i=0; i<COUNT_NUM; i++){
            pthread_t t;
            pthread_create (&t, NULL, _remove_non_prime, &pth_dt[i]);

            pth.push_back(t);
        }
        for(auto i=0; i<COUNT_NUM; i++){ pthread_join(pth[i], NULL); }
        pth.clear();
        // std::cout << "Find non-prime" << std::endl;

        for(auto i : rm){
            _elements[i[0]][i[1]] = 0;
        }
        // std::cout << "complete" << std::endl;
    }

    static void* _create_array(void* data){
        Thread_data*dt = (Thread_data*)data;
        pthread_mutex_t mu = dt->er->mu;

        vector<int> ov;
        int cur = 1;
        
        int roof =  dt->er->roof;
        int k = dt->er->num[dt->ind];
        int BASIS = dt->er->BASIS;


        while (true){
            int dg = BASIS*cur + k;
            cur++;
            
            if (dg > roof) break;
            ov.push_back(dg);
            if (dg < sqrt(roof)) {
                pthread_mutex_lock(&(mu));
                dt->er->low.push_back(dg);  
                pthread_mutex_unlock(&(mu));
            }
        }
        
        // pthread_mutex_lock(&(mu));
        for(int i=0; i<ov.size(); i++){
            dt->er->_elements[dt->ind].push_back(ov[i]);
        }
        // pthread_mutex_unlock(&(mu));
        
        pthread_exit(0);
        return NULL;
    }

    static void* _remove_non_prime(void* data){
        Thread_data*dt = (Thread_data*)data;
        pthread_mutex_t mu = dt->er->mu;
        vector<int>*num =  &(dt->er->num);        
        int ind = dt->ind;
        int roof =  dt->er->roof;
        int k = (*num)[ind];
        int BASIS = dt->er->BASIS;

        vector<int>*line = &(dt->er->_elements[ind]);

        for (auto i : dt->er->low){
            int cur = 0;
            while (true){
                unsigned long long dg = BASIS*cur + k;
                dg *= i;
                cur++;
                if (dg < i) continue;
                if (dg > roof) break;

                int reminder = (dg) % BASIS;

                // std::cout << dg << " " << i << std::endl;
                vector<int> rm_el = {   
                    dt->er->num_ind[reminder], 
                    int((dg)/30)
                };      

                // pthread_mutex_lock(&mu);
                // std::cout << cur << " " << i << "*" <<dg/i << " " << rm_el[0] << " " << rm_el[1] << std::endl;
                if (rm_el[1] >= 1) dt->er->_elements[rm_el[0]][rm_el[1]] = 0;
                // pthread_mutex_unlock(&mu);
            }
        }

/*         for (auto j : (*line)){
            for (auto l : dt->er->low){
                    long long t = j * l;

                if (t > dt->er->roof) continue;

                int reminder = t % dt->er->BASIS;
                int ind = num_ind[reminder];

                if (ind >= 0){
                    std::cout << dt->er->rm.size() << std::endl;
                    vector<int> rm_el = {ind, int(t/30)};             
                    pthread_mutex_lock(&mu);
                    dt->er->rm.push_back(rm_el);
                    pthread_mutex_unlock(&mu);
                } 
            }
        } */
        pthread_exit(0);
        return NULL;
    }

    Erathosven_class(){
        COUNT_NUM = 8;
        num = {1, 7, 11, 13, 17, 19, 23, 29};
        roof = 0;
        BASIS = 30;
        for (int i=0; i<BASIS; i++){ num_ind.push_back(-1); }
        for (int i=0; i<COUNT_NUM; i++){ num_ind[num[i]] = i; }

        pthread_mutex_init(&mu, NULL);
    }
};

vector<ull>erathosvenCircleFact(ull n){
    const int COUNT_NUM = 8;
    const int BASIS = 30;
    const int num[] = {1, 7, 11, 13, 17, 19, 23, 29};
    ull roof = (n/BASIS)*COUNT_NUM;
    int num_ind[BASIS];

    bool*sv = (bool*)malloc(sizeof(bool)*(roof));

    for (ull i=0; i<roof; i++) sv[i] = 1;
    for (int i=0; i<BASIS; i++){ num_ind[i] = -1; }
    for (int i=0; i<COUNT_NUM; i++){ 
        int ind = num[i];
        num_ind[ind] = i; 
    }
    for(ull i=1; i<roof; i++){
        if (sv[i] == 0) continue;;
        for(ull j=i; j<roof; j++){
            ull t = ((j/COUNT_NUM)*BASIS + num[j%COUNT_NUM]) * ((i/COUNT_NUM)*BASIS + num[i%COUNT_NUM]);
            
            if (t > n) break;

            int reminder = t % BASIS;
            ull ind = num_ind[reminder];
            // if ((t / BASIS)*8 + ind > roof) break;
            sv[(t / BASIS)*8 + ind] = 0;
        }
    }

    vector<ull> tmp; 
    for (ull i=0; i<roof; i++) {
        if (sv[i]!= 0) { 
            tmp.push_back((i/COUNT_NUM)*BASIS + num[i%COUNT_NUM]);
        }
    }
    free(sv);
    return tmp;
}

vector<ull>  Sundaram(ull n){
    ull k = (n)/2;
    bool*a = (bool*)malloc(sizeof(bool)*(ull(k+1)));
    vector<ull > tmp;
    for (ull i=0; i<(k); i++){ a[i]=(false); }

    for(ull i=1; i<k+1; i++){
        ull j=i;
        while((ull)(i+j+2*i*j) <= k){
            a[(ull)(i+j+2*i*j)] = true;
            j++;
        }
    }
    for (int i=0; i<(k+1); i++){
        if (a[i] == 0) { tmp.push_back(2*i + 1); }
    }
    free (a);
    return tmp;
}

vector<int> erathosven4(unsigned int n){
    const int COUNT_NUM = 8;
    const int BASIS = 30;
    const int num[] = {1, 7, 11, 13, 17, 19, 23, 29};
    int num_ind[BASIS];

    vector<vector<int>> sv;
    vector<vector<int>> rm;
    vector<int> low;

    for (int i=0; i<BASIS; i++){ num_ind[i] = -1; }
    for (int i=0; i<COUNT_NUM; i++){ 
        int ind = num[i];
        num_ind[ind] = i; 
    }
    for (int i=0; i<COUNT_NUM; i++){
        int t = num[i];
        sv.push_back({});

        if (t != 1) {
            sv[i].push_back(t);
            if (t < sqrt(n)) low.push_back(t);
        }
        else
            sv[i].push_back(0);
    }

    for (int j=0; j<COUNT_NUM; j++){
         sv.push_back({});
        // create_array(BASIS, num[j], n); 
    }

    // for(auto i: sv){   
    //     for(auto j: i){
    //         cout.width(4);
    //         cout << j << " ";
    //     }
    //     cout << endl;
    // }

    for (auto i=0; i<sv.size(); i++){
        for (auto j : sv[i]){
            for (auto l : low){
                long long t = j * l;

                if (t > n && t >0) break;

                int reminder = t % BASIS;
                int ind = num_ind[reminder];

                if (ind >= 0){
                    vector<int> rm_el = {ind, int(t/30)};
                    rm.push_back(rm_el);
                } 
            }
        }
    }
    /* for (auto i : rm){
        for (auto j : i)
            std::cout << j << " " << std::endl;
        std::cout << std::endl;
    } */
    for (auto i : rm){
        sv[i[0]][i[1]] = 0;
    }

    vector<int> tmp;

    for(auto i : sv)
        for(auto j : i)
            if (j != 0) tmp.push_back(j);        
    
    return tmp; 
}