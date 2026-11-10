#ifndef MARCO_H
#define MARCO_H
#include<iostream>
#include"proceso.h"
class Marco
{
private:


    static const int size = 4;
    int vector[size];
    unsigned int _id;



public:
    Marco();

    void initFullMarco(unsigned _id);
    void printMatrix();
    bool isFreeSpace();
    bool insertarProceso(Proceso &);
    int qtyFreeSpace();
    int qtyOcuppiedSpace();
    bool insertResto(int sixe_mem, Proceso&);

    unsigned int id() const;
    void setId(unsigned int id);

    void deleteData();
};

#endif // MARCO_H
