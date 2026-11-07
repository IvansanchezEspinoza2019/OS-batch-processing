#ifndef RANDOMNUM_H
#define RANDOMNUM_H

#include<time.h>
#include<conio.h>
#include<ctime>
#include<random>
#include<chrono>
#include<functional>
#include<iostream>
#include<stdlib.h>

class RandomNum
{
private:


    std::default_random_engine generator;
    std::uniform_int_distribution<int> intDistributionNum;
    std::function<int()> rnd;


public:
    RandomNum(const int &_min, const int &_max);
    int choiceRandom() const;



};

#endif // RANDOMNUM_H
