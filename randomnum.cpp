#include "randomnum.h"
using namespace std;


RandomNum::RandomNum(const int& _min, const int&_max) :generator(chrono::system_clock::now().time_since_epoch().count()),intDistributionNum(_min,_max)
{

    rnd = bind(intDistributionNum,generator);


}

int RandomNum::choiceRandom() const
{
    return rnd();
}
