#include <iostream>
//#include "randomnum.h"
#include "proceso.h"

using namespace std;



int main(){

	// Genera numeros randon para las distintas necesidades
	//RandomNum intDiceNum(0,1000);
  


    Proceso proceso;
    proceso.setId(1);
    proceso.setNombre("Proceso 1");

	std::cout<< "First process created: "<<proceso.getNombre()<<std::endl;
	return 0;
}