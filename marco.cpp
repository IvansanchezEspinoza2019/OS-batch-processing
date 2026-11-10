#include "marco.h"



using namespace std;

unsigned int Marco::id() const
{
    return _id;
}

void Marco::setId(unsigned int id)
{
    _id = id;
}

void Marco::deleteData()
{
    // 0 significa que está desocupado
    vector[0] = 0;
    vector[1] = 0;
    vector[2] = 0;
    vector[3] = 0;

    _id = 99;
}

Marco::Marco()
{
    // initializing the vector

    for ( int i = 0; i < size; ++i){
        vector[i] = 0;

    }
// el id 99 lo considero para marcos libres
    _id = 99;

}



void Marco::initFullMarco(unsigned id)
{
    // funcion que reserva todo un marco, para este caso 4
    // 1 indica que esa posicion esta ocupada
    vector[0] = 1;
    vector[1] = 1;
    vector[2] = 1;
    vector[3] = 1;

    _id = id;

}

void Marco::printMatrix()
{
    // showing the matrix

    for ( int i = 0; i < size; ++i){
        vector[i] = 0;
    }
}

bool Marco::isFreeSpace()
{
    // verifica si un marco está libre
    return vector[0] == 0 and vector[1] == 0 and vector[2] == 0 and vector[3] == 0 ? true : false;
}

bool Marco::insertarProceso(Proceso & _p)
{
    if ( !isFreeSpace() ){
        // si no hay espacio libre
        return false;
    }

    // 1 indica que esa posicion esta ocupada
    vector[0] = 1;
    vector[1] = 1;
    vector[2] = 1;
    vector[3] = 1;

    _id = _p.getId();

    return true;
}

int Marco::qtyFreeSpace()
{
    int cont = 0;
    if(vector[0]==0){
        cont++;
    }
    if(vector[1]==0){
        cont++;
    }
    if(vector[2]==0){
        cont++;
    }
    if(vector[3]==0){
        cont++;
    }

    return  cont;
}

int Marco::qtyOcuppiedSpace()
{
    int cont = 0;
    if(vector[0]==1){
        cont++;
    }
    if(vector[1]==1){
        cont++;
    }
    if(vector[2]==1){
        cont++;
    }
    if(vector[3]==1){
        cont++;
    }

    return  cont;
}

bool Marco::insertResto(int sixe_mem, Proceso &_p)
{
    if ( sixe_mem <= 0){
        return false;
    }
    for (int i = 0; i < sixe_mem;i++) {
        vector[i] = 1;
    }
    _id = _p.getId();
    return true;
}
