
#include <vector>
#include <utility>
#include <memory>
#include <algorithm>
#include <cstdlib>
#include "primality_B.decl.h"
 

class Main : public CBase_Main {
    public:
        Main(CkArgMsg* arg) {
            if (arg->argc != 3) {
                CkAbort("Invalid number of args");
            }
            int K = std::atoi(arg->argv[1]);
            int M = std::atoi(arg->argv[2]);
            count = (K + M - 1) / M;   
            start_time = CkWallTimer();
            for (int start = 0; start < K; start += M) {
                int n = std::min(M, K - start);   
                std::vector<long> chunk(n);
                for (int j = 0; j < n; ++j) {
                    long num = rand();
                    chunk[j] = num;
                    results.push_back(std::make_pair(num, false));  
                } 
                CProxy_Check_Primality::ckNew(n, chunk.data(), static_cast<unsigned int>(start), thisProxy);
            }
        }
 
        void receive_results(unsigned int start_idx, int n, bool* res) {
            for (int i = 0; i < n; ++i) {
                results[start_idx + i].second = res[i];  
            }
            --count;
            if (count == 0) {
                double elapsed = CkWallTimer() - start_time;
                ckout << "Elapsed: " << elapsed << endl;
                for (const auto& [num, is_prime] : results) {
                    ckout << num << " is prime: " << is_prime << endl;
                }
                CkExit();
            }
        }
    private:
        int count;
        double start_time;
        std::vector<std::pair<long, bool>> results;
};
 
  

bool isPrime(const long number) {
    if (number <= 1) {
        return false;
    }
    for (long i = 2; i < number; ++i) {
        if (number % i == 0) {
            return false;
        }
    }
    return true;
}
 
 
class Check_Primality : public CBase_Check_Primality {
    public:
        Check_Primality(int n, long* numbers, unsigned int start, CProxy_Main main_proxy) {
            std::unique_ptr<bool[]> res(new bool[n]);
            for (int j = 0; j < n; ++j) {
                res[j] = isPrime(numbers[j]);
            }
            main_proxy.receive_results(start, n, res.get());
        }
};
 
#include "primality_B.def.h"
