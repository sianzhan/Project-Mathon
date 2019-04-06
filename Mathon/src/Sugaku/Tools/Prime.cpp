#include "Tools.h"
#include "Integer.h"
#include <vector>

using namespace std;

namespace
{
    static vector<Integer> tablePrime = 
    {
        2, 3, 5, 7, 11, 13, 17, 19, 23,
        29, 31, 37, 41, 43, 47, 53, 59,
        61, 67, 71, 73, 79, 83, 89, 97,
        101, 103, 107, 109, 113, 127, 131,
        137, 139, 149, 151, 157, 163, 167,
        173, 179, 181, 191, 193, 197, 199
    };
    void genPrime(const Integer& z)
    {
        Integer target = sqrt(z);
        while(target > tablePrime.back())
        {
            Integer guess = tablePrime.back();
            bool match;
            do
            {
                guess += 2;
                match = 1;
                for(Integer &val : tablePrime)
                {
                    if(guess % val == 0) {match = 0; break;}
                }
            }while(!match);
            tablePrime.push_back(guess);
        }
    }
}

const bool isPrime(const Integer &z)
{
    if(z < 2) return false;
    genPrime(z);
    Integer max = sqrt(z);
    for(Integer &val : tablePrime)
    {
        if(val > max) return true;
        if(z % val == 0) return false;
    }
    return true;
}