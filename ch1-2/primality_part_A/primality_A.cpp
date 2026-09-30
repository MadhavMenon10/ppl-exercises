#include <vector>
#include <utility>
#include <cstdlib>
#include "primality_A.decl.h"

class Main: public CBase_Main {
    public:
        Main(CkArgMsg* arg) {
            if (arg->argc != 2) {
                CkAbort("No K passed");
            }
            count = std::atoi(arg->argv[1]);
            int k = count;

            for (int i = 0; i < k; ++i) {
                int num = rand();
                results.push_back(std::make_pair(num, false));
                CProxy_Check_Primality check = CProxy_Check_Primality::ckNew(num, results.size() - 1, thisProxy);
            }
           

        }
        void receive_results(unsigned int idx, bool is_prime) {
            results[idx].second = is_prime;
            --count;
            if (count == 0) {
                for (const auto& [num, res] : results) {
                    ckout << num << " is prime: " << res << endl;
                }
                CkExit();
            }
        }
    private:
        int count;
        std::vector<std::pair<long, bool>> results;
};

bool isPrime(const long number) {
    if (number <= 1) {
        return false;
    } 
    if (number <= 3) {
        return true;
    }
    if (number % 2 == 0 || number % 3 == 0) {
        return false;
    }
    for(int i = 5; (i * i) <= number; i += 6) {
        if ((number % i == 0) || (number % (i + 2) == 0)) {
            return false;
        }
    }
    return true;
}

class Check_Primality : public CBase_Check_Primality {
    public:
        Check_Primality(long number, unsigned int idx, CProxy_Main main_proxy) {
            bool is_prime = isPrime(number);
            main_proxy.receive_results(idx, is_prime);
        }
    private:
};




#include "primality_A.def.h"
