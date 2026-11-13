#include <iostream>
#include <fstream>
#include<windows.h>
#include<iomanip>
#include<list>
#include"proceso.h"
#include<stdlib.h>
#include<time.h>
#include<conio.h>
#include<ctime>
#include<random>
#include<chrono>
#include<functional>
#include"randomnum.h"
#include"marco.h"

using namespace std;

void gotoxy(int x, int y);
void pintar_escenario();
void print_all_lotes(list<list<Proceso>> _lista);
void lote_actual(list<Proceso> _lote, unsigned int &, unsigned int &  , unsigned int &_No_lote);
void setProceso(Marco *,list<Proceso> _lote, RandomNum &intDiceNum, RandomNum &intDiceOperador, RandomNum &intDiceTme, RandomNum &intDiceMem, unsigned int &_id, char *operaciones, unsigned int &_QUANTUM);
void mover_finalizado(Proceso, unsigned int&);
void mover_finalizado_error(Proceso, unsigned int&);
void remover_firstP();
void limpiar_ejecucion();
float getResultado(Proceso &);

void prueba(int,string);
void actualizarColaListos(list<Proceso>);
void cleanListos();
void print_all_process(list<Proceso> _lista, int &_pos);
void print_all_bloqueados(list<Proceso> &_bloqueados);
void BCP_terminados(list<Proceso> terminados, unsigned int & inc, unsigned int &_CONT);
void actualizarBloqueados(list<Proceso> bloq);
Proceso newProcess(RandomNum&, RandomNum&, RandomNum&, RandomNum&, unsigned int &_id, char *operaciones);
void printRemainingNumProcess(size_t & _tam);
void printGlobalCounter(unsigned int & _COUNTER);
void showExecutingProcess(unsigned int  _tme_p, unsigned int &_tt,unsigned int&_Counter);
void BCP(list<Proceso>& _listos,list<Proceso> &_bloqueados,list<Proceso> &_terminados, list<Proceso> &_suspendidos,Proceso &_actual, unsigned int & );
void BCP_bloquedos(list<Proceso> &_bloqueado,unsigned int &inc,unsigned int &);
void BCP_listos(list<Proceso> &_listos, unsigned int &inc, unsigned int&_CONT);
void print_header_BCP();
void add_horizontal_line(const unsigned int &_row, const unsigned int &_initial_pos_colum, const unsigned int &_final_pos_colum);
void add_vertical_line(const unsigned int &_colum, const unsigned int &_initial_pos_row, const unsigned int &_final_pos_row);
void BCP_Atual_process(Proceso &_process, unsigned int &_pos, unsigned int&);
void print_final_content(unsigned int &_fila);

// new
bool insertarProcesoMemoria(Marco *memoria,Proceso& _p);
bool elimProcesoMemoria(Marco *, Proceso & _p);

void printMemoria(Marco *);
void printMemoriaFrom(Marco *, int _pos);
void cleanMemoryScreen();
void updateMemoryScreen(Marco *);

bool saveProccessToFile(Proceso & _p);
void init_file();

// nuevo
void BCP_suspendidos(list<Proceso> &suspendidos, unsigned int &inc, unsigned int&_CONT);
void BCP_terminados_suspendidos(list<Proceso> &terminados,list<Proceso> &suspendidos, unsigned int &inc, unsigned int&_CONT);

int main()
{

    init_file();
    // memoria
    Marco memoria[45];

    // los primeros tres se reservan para el sistema operativo SO
    memoria[0].initFullMarco(0);
    memoria[1].initFullMarco(0);
    memoria[2].initFullMarco(0);


    // Genera numeros randon para las distintas necesidades
    RandomNum intDiceNum(0,1000);
    RandomNum intDiceOperando(0,4);                                                             // Objetos para obtener numeros random
    RandomNum intDiceTme(8,18);
    RandomNum intDiceMem(6,35);

    // lista de procesos
    list<Proceso> lista_procesos;                                                               // contien todos los procesos creados inicialmente                                                           // contiene n lotes
    // operaciones
    char operaciones[5] = {'+','-','*','/','%'};                                                // operaciones predefinidas

    string buffer;
    int _procesos = 0;                                                                          // numero de procesos totales iniciales
    unsigned id = 1, q = 0;                                                                            // variable que controla los ID's de los procesos


    cout << "\t\t .: INGRESE LOS SIGUIENTES VALORES :." << endl << endl << endl;
    // pide el numero de procesps que va a haber
    while( _procesos < 1 ){                                                                     // pide el numero inicial de procesos
        try{
            cout<<"No. Procesos: ";
            getline(cin,buffer);
            _procesos =stoi(buffer);

        }catch(...){
            cout<<"\t[ ERROR: DATO NO NUMERICO ]"<<endl;
            _procesos = 0;
        }
    }
    // pide el quentum del programa
    while( q < 1 or q >18 ){                                                                     // pide el numero inicial de procesos
        try{
            cout<<"Cuantum[1..18]: ";
            getline(cin,buffer);
            q = stoi(buffer);

        }catch(...){
            cout<<"\t[ ERROR: DATO NO NUMERICO ]"<<endl;

        }
    }



    for (int i = 0; i < _procesos; ++i) {
        // captura los datos de cada proceso
        Proceso p(newProcess(intDiceNum,intDiceOperando,intDiceTme, intDiceMem,id,operaciones));

        if(insertarProcesoMemoria(memoria, p)){
            cout << "Id: " << p.getId() << endl;
            lista_procesos.push_back(p);
        }
        else{
            cout << "[ WARNING: Un proceso no se pudo insertar: " << p.getId() << " ]" << endl;
            return 1;
        }

    }

    system("cls");


    // imprime el tamaño del quantum
    gotoxy(72,1);
    cout<<"+ QUANTUM: "<<q;

    //funcion principal del programa
    setProceso(memoria,lista_procesos,intDiceNum,intDiceOperando,intDiceTme, intDiceMem,id,operaciones,q);
    return 0;
}

///////////////////////////////////// ACTIVIDAD 15 ///////////////////////////////////
//////////////// FUNCIONES PARA GUARDAR Y LEER DE ARCHIVOS /////////////////////////////
void init_file(){
    ofstream ar("suspendidos.txt");
}


// funciones de archivod
bool saveProccessToFile(Proceso & _p){
    string nombre = "suspendidos.txt";
    fstream fs;
    fs.open(nombre, ios::app);
    char delim = ',';
    if(fs.is_open()){

        //////////////////  datos guardados:
        // Id, TME, Operacion, num1, num2, tiempo trascurrido (tt)
        //  was_executed, arrivalTime, response time, memorySize, marcosSuze, restoSize
        fs << _p.getId() << delim << _p.getTme() << delim
           << _p.getOperacion() << delim << _p.getNum1() << delim << _p.getNum2() << delim
           << _p.getTt() << delim << _p.getWas_execute() << delim << _p.getArrivalTime() << delim
           << _p.getResponseTime() << delim << _p.getMemorySize() <<delim
           << _p.getMarcosSize() << delim << _p.getRestoMarco() << endl;
        fs.close();
        return true;
    }
    return false;
}

Proceso getFirstSuspendedProcess(){
    Proceso p;
    fstream a("suspendidos.txt",ios::in);
    string buffer;

    unsigned int temp;
    float tempN;
    int val;
    if (a.is_open()){
        //id
        getline(a, buffer,',');
        temp = stoul(buffer);
        p.setId(temp);

        // TME
        getline(a, buffer,',');
        temp = stoul(buffer);
        p.setTme(temp);

        //operacion
        getline(a, buffer,',');
        p.setOperacion(buffer[0]);

        //num1
        getline(a,buffer,',');
        tempN = stof(buffer);
        p.setNum1(tempN);

        //num2
        getline(a,buffer,',');
        tempN = stof(buffer);
        p.setNum2(tempN);

        //tt
        getline(a,buffer,',');
        temp = stoul(buffer);
        p.setNum1(temp);

        //was executed
        getline(a,buffer,',');
        p.setWas_execute(true);

        //arrival time
        getline(a,buffer,',');
        temp = stoul(buffer);
        p.setArrivalTime(temp);

        //response time
        getline(a,buffer,',');
        temp = stoul(buffer);
        p.setResponseTime(temp);

        //memory size
        getline(a,buffer,',');
        temp = stoul(buffer);
        p.setMemorySize(temp);

        //marcos
        getline(a,buffer,',');
        val = stoi(buffer);
        p.setMarcosSize(val);

        //resto marcos
        getline(a,buffer,'\n');
        val = stoi(buffer);
        p.setRestoMarco(val);



        a.close();
    }
    return p;
}

bool existProcessSuspended(){
    fstream a("suspendidos.txt",ios::in);
    string buffer="";

    if (a.good()){
        //lee una linea
        getline(a,buffer);

        a.close();
    }

    return buffer == "" ? false:true;
}

bool removeFirstSuspendedProcess() {

    fstream archivo("suspendidos.txt",ios::in);
    fstream temp("temp.txt", ios::out);
    bool more_records = false;

    string buffer;
    // quitamos la primera line del archuvo
    // ya que es la que contiene el proceso extraido
    getline(archivo,buffer);

    if ( archivo.is_open()){

        while ( !archivo.eof() ){

            getline(archivo,buffer);
            if (archivo.eof()){
                break;
            }
            more_records = true;
            // guardamos en el archivo de respado
            temp  << buffer << endl;
        }

        // cierra archivos
        archivo.close();
        temp.close();

        // cambia el nombre del archivo
        remove("suspendidos.txt");
        rename("temp.txt","suspendidos.txt");

        return more_records;
    }
    return false;
}

Proceso getNextSuspendeProccessInfo(){
    Proceso p;
    fstream ar("suspendidos.txt");
    string buffer;
    unsigned int temp;
    float tempN;
    int val;

    if (ar.is_open()){
        //id
        getline(ar, buffer,',');
        temp = stoul(buffer);
        p.setId(temp);

        // TME
        getline(ar, buffer,',');
        temp = stoul(buffer);

        //operacion
        getline(ar, buffer,',');


        //num1
        getline(ar,buffer,',');
        tempN = stof(buffer);


        //num2
        getline(ar,buffer,',');
        tempN = stof(buffer);


        //tt
        getline(ar,buffer,',');
        temp = stoul(buffer);


        //was executed
        getline(ar,buffer,',');


        //arrival time
        getline(ar,buffer,',');
        temp = stoul(buffer);


        //response time
        getline(ar,buffer,',');
        temp = stoul(buffer);


        //memory size
        getline(ar,buffer,',');
        temp = stoul(buffer);
        p.setMemorySize(temp);

        //marcos
        getline(ar,buffer,',');
        val = stoi(buffer);
        p.setMarcosSize(val);

        //resto marcos
        getline(ar,buffer,'\n');
        val = stoi(buffer);
        p.setRestoMarco(val);



        ar.close();
    }
    return p;
}

///////////////////////// FIN ACTIVIDAD 15  //////////////////////////////////////////////


void add_horizontal_line(const unsigned int &_row, const unsigned int &_initial_pos_colum, const unsigned int &_final_pos_colum){


    for (unsigned var = _initial_pos_colum; var <= _final_pos_colum; ++var) {
        gotoxy(var,_row);
        cout<<"-";
    }
}


void add_vertical_line(const unsigned int &_colum, const unsigned int &_initial_pos_row, const unsigned int &_final_pos_row){
    for (unsigned var = _initial_pos_row; var <= _final_pos_row; ++var) {
        gotoxy(_colum,var);
        cout<<"|";
    }
}


void print_all_process(list<Proceso> _lista, int &_pos){
    gotoxy(1, _pos);

    for (list<Proceso>::iterator ptr = _lista.begin(); ptr !=  _lista.end(); ptr++) {
        cout<<*ptr<<endl;
        _pos ++;
    }
}

Proceso newProcess(RandomNum & intDiceNum, RandomNum & intDiceOperador, RandomNum & intDiceTme, RandomNum & intDiceMem, unsigned int& _id, char* operaciones){
    unsigned int i_operador=0;
    Proceso proceso;
    proceso.setMemorySize(intDiceMem.choiceRandom());





    proceso.setId(_id);

    proceso.setTme(intDiceTme.choiceRandom());                       //TME
    proceso.setNum1(float(intDiceNum.choiceRandom()));               // NUMERO 1


    do{

        i_operador = intDiceOperador.choiceRandom();
        proceso.setNum2(float(intDiceNum.choiceRandom()));       // NUMERO 2

    }while((operaciones[i_operador]== '/' or operaciones[i_operador]=='%') and proceso.getNum2()==0);
    proceso.setOperacion(operaciones[i_operador]);     //Operacion

    _id++;

    return proceso;
}

float getResultado(Proceso &_p){

    switch (_p.getOperacion()) {
    case '+':
        return (_p.getNum1() + _p.getNum2());
        break;
    case '-':
        return (_p.getNum1() - _p.getNum2());
        break;
    case '*':
        return (_p.getNum1() * _p.getNum2());
        break;
    case '/':
        return (_p.getNum1() / _p.getNum2());
        break;
    case '%':
        return float(int(_p.getNum1()) % int(_p.getNum2()));
        break;
    default:
        return 0.0;
        break;
    }


}

void cleanListos(){
    gotoxy(3,10);
    cout<<setw(23)<<" ";
    gotoxy(3,11);
    cout<<setw(23)<<" ";
    gotoxy(3,12);
    cout<<setw(23)<<" ";
    gotoxy(3,13);
    cout<<setw(23)<<" ";
    gotoxy(3,14);
    cout<<setw(23)<<" ";
    gotoxy(3,15);
    cout<<setw(23)<<" ";
    gotoxy(3,16);
    cout<<setw(23)<<" ";
    gotoxy(3,17);
    cout<<setw(23)<<" ";
    gotoxy(3,18);
    cout<<setw(23)<<" ";
    gotoxy(3,19);
    cout<<setw(23)<<" ";
    gotoxy(3,20);
    cout<<setw(23)<<" ";
}

void actualizarColaListos(list<Proceso> _lote){

    int auto_size=0;
    auto_size = 10;
    for (list<Proceso>::iterator ptr = _lote.begin();  ptr!= _lote.end(); ptr++) {

        gotoxy(3,auto_size);

        cout<<ptr->getId();

        gotoxy(11,auto_size);

        cout<<ptr->getTme();

        gotoxy(19,auto_size);

        cout<<ptr->getTt();

        auto_size+=1;

    }

}
void remover_firstP(unsigned int _pos){

    gotoxy(3,_pos);
    cout<<setw(48)<<" ";
}

void mover_finalizado(Proceso p, unsigned int & _pos_f){

    gotoxy(110,_pos_f);
    cout<<setw(51)<<" ";

    gotoxy(110,_pos_f+1);
    cout<<setw(51)<<" ";

    gotoxy(110,_pos_f);
    cout<<p.getId();

    gotoxy(120,_pos_f);
    cout<<p.getNum1()<<" "<<p.getOperacion()<<" "<<p.getNum2();

    gotoxy(147,_pos_f);
    cout<<p.getRes();

}

void mover_finalizado_error(Proceso p, unsigned int & _pos_f){

    gotoxy(110,_pos_f);
    cout<<setw(51)<<" ";

    gotoxy(110,_pos_f+1);
    cout<<setw(51)<<" ";

    gotoxy(110,_pos_f);
    cout<<p.getId();

    gotoxy(120,_pos_f);
    cout<<p.getNum1()<<" "<<p.getOperacion()<<" "<<p.getNum2();

    gotoxy(147,_pos_f);
    cout<<"ERROR";
}

void prueba(int pos,string msg){
    gotoxy(55,pos);
    cout<<msg;
}

void print_bloquedo(Proceso _bloqueado, unsigned int pos){
    // limpia
    gotoxy(60,pos);
    cout<<setw(40)<<"     ";

    //imprime
    gotoxy(60,pos);
    cout<<_bloqueado.getId();
    gotoxy(70,pos);
    cout<<_bloqueado.getTtb();
    gotoxy(80,pos);
    cout<<10 - _bloqueado.getTtb();

}

void process_bloqueados(list<Proceso> _bloqueados){

    unsigned int pos= 31;
    for (list<Proceso>::iterator var = _bloqueados.begin(); var != _bloqueados.end(); ++var) {
        var->setTtb(var->getTtb()+1);
        print_bloquedo(*var,pos);
        pos++;


    }

}
void print_all_bloqueados(list<Proceso> &_bloqueados){
    unsigned int pos = 32;
    int val;
    for (list<Proceso>::iterator var = _bloqueados.begin(); var != _bloqueados.end(); ++var) {

        var->setTtb(var->getTtb()+1);
        //var->setWaintingTotalTime(var->getWaintingTotalTime()+1);


        gotoxy(3,pos);
        cout<<setw(20)<<"  ";
        //imprime
        gotoxy(3,pos);
        cout<<var->getId();
        gotoxy(13,pos);
        cout<<var->getTtb();
        gotoxy(23,pos);
        val = 10 - int(var->getTtb());
        cout<<val;

        pos++;
    }
}
void delete_all_bloqueados(){
    //imprime
    gotoxy(3,32);
    cout<<setw(28)<<"   ";
    gotoxy(3,33);
    cout<<setw(28)<<"   ";
    gotoxy(3,34);
    cout<<setw(28)<<"   ";
    gotoxy(3,35);
    cout<<setw(28)<<"   ";
    gotoxy(3,36);
    cout<<setw(28)<<"   ";
    gotoxy(3,37);
    cout<<setw(28)<<"   ";
    gotoxy(3,38);
    cout<<setw(28)<<"   ";
    gotoxy(3,39);
    cout<<setw(28)<<"   ";
    gotoxy(3,40);
    cout<<setw(28)<<"   ";
    gotoxy(3,41);
    cout<<setw(28)<<"   ";

}
void actualizarBloqueados(list<Proceso> bloq){
    unsigned int pos = 32;
    int val = 0;
    for (list<Proceso>::iterator var = bloq.begin(); var != bloq.end(); ++var) {

        gotoxy(3,pos);
        cout<<setw(28)<<"     ";

        //imprime
        gotoxy(3,pos);
        cout<<var->getId();
        gotoxy(13,pos);
        cout<<var->getTtb();
        gotoxy(23,pos);
        val = 10 - int(var->getTtb());
        cout<<val;

        pos++;

    }
}
void printRemainingNumProcess(size_t & _tam){
    gotoxy(25,1);
    cout<<setw(15)<<"  ";
    gotoxy(25,1);
    cout<<_tam;
}

void printGlobalCounter(unsigned int & _Counter){
    gotoxy(25,25);
    cout<<setw(17)<<"   ";
    gotoxy(25,25);
    cout<<_Counter;
}

void showExecutingProcess(unsigned int  _tme_p, unsigned int &_tt,unsigned int&_Counter){
    gotoxy(61,12);
    cout<<"      ";
    gotoxy(61,12);
    cout<<_tt;

    printGlobalCounter(_Counter);

    gotoxy(61,13);
    cout<<"      ";
    gotoxy(61,13);
    cout<<_tme_p-_tt;
}



void extendFinalizadosContent(unsigned int &_pos_f){
    gotoxy(109,_pos_f);
    cout<<"|";

    gotoxy(161,_pos_f);
    cout<<"+";

    gotoxy(161,_pos_f);
    cout<<"|";

    gotoxy(109,_pos_f+1);
    cout<<"+";

    add_horizontal_line(_pos_f+1,110,160);
    gotoxy(161,_pos_f+1);
    cout<<"+";

}


void actualizarColaFinalizados(list<Proceso> &_terminados){

    int _pos_f =10;

    for (list<Proceso>::iterator ptr = _terminados.begin();  ptr != _terminados.end(); ptr++) {
        gotoxy(110,_pos_f);
        cout<<setw(51)<<" ";

        gotoxy(110,_pos_f+1);
        cout<<setw(51)<<" ";

        gotoxy(110,_pos_f);
        cout<<ptr->getId();

        gotoxy(120,_pos_f);
        cout<<ptr->getNum1()<<" "<<ptr->getOperacion()<<" "<<ptr->getNum2();

        gotoxy(147,_pos_f);
        if(ptr->getSuccess()){
            cout<<ptr->getRes();
        }
        else{
            cout<<"ERROR";
        }
        _pos_f++;
    }


    add_vertical_line(109,10,_pos_f);

    gotoxy(109,_pos_f+1);
    cout<<"+";

    add_horizontal_line(_pos_f+1,110,160);

    gotoxy(161,_pos_f+1);
    cout<<"+";


    add_vertical_line(161,10,_pos_f);

}

void update_scenario(list<Proceso> listos,list<Proceso> bloqueados,list<Proceso> terminados,unsigned int &_cont,unsigned int &q){
    system("cls");
    gotoxy(72,1);
    cout<<"+ QUANTUM: "<<q;

    pintar_escenario();
    printGlobalCounter(_cont);

    actualizarBloqueados(bloqueados);
    actualizarColaListos(listos);
    actualizarColaFinalizados(terminados);
}

void updateWaitingTimeListos(list<Proceso> &_listos){
    for (list<Proceso>::iterator ptr = _listos.begin(); ptr != _listos.end(); ptr++) {
        ptr->setWaintingTotalTime(ptr->getWaintingTotalTime()+1);
    }
}

void update_executing_process(Proceso p_actual){
    gotoxy(61,9);
    cout<<p_actual.getId();

    gotoxy(61,10);
    cout<<p_actual.getNum1()<<" "<<p_actual.getOperacion()<<" "<<p_actual.getNum2();

    gotoxy(61,11);
    cout<<p_actual.getTme();
}

void cleanNextProcess() {
    gotoxy(1,3);
    cout << setw(90) << "  ";
}
void pintarNextProccess(Proceso & _p){
    gotoxy(1, 3);
    cout << "Proceso SUSPENDIDO:\t\t[Id: " << _p.getId() << "] [ Espacio en memoria: " << _p.getMemorySize() << " ]";
}

void imprimirNextProccessMsg(){
    gotoxy(1,3);
    cout << "Proceso SUSPENDIDO:" << endl;
}

void cleanNextNewProcess() {
    gotoxy(1,4);
    cout << setw(90) << "  ";
}
void pintarNextNewProccess(Proceso & _p){
    gotoxy(1, 4);
    cout << "Proceso NUEVO:\t\t[Id: " << _p.getId() << "] [ Espacio en memoria: " << _p.getMemorySize() << " ]";
}
void imprimirNextNewProccessMsg(){
    gotoxy(1,4);
    cout << "Proceso NUEVO:" << endl;
}

void printDataCommingProccess(Proceso &suspendido, list<Proceso> &nuevos){
    /////// ACTUALIZA LOS DATOS DE LOS PROCESOS QUE VANA A ENTRAR A MEMORIA PRINCIPAL   /////
    // actualiza el procesos suspendido
    if ( suspendido.getSuspended()){
        // si  esta suspendido, es decir,
        //está en memoria secundaria entonces hay que mostrar sus datos
        cleanNextProcess();
        pintarNextProccess(suspendido);
    }
    else{
        // si no hay un proceso suspendido
        cleanNextProcess();
        imprimirNextProccessMsg();
    }

    // actualiza los procesos nuevos
    if (!nuevos.empty()){

        cleanNextNewProcess();
        pintarNextNewProccess(nuevos.front());
    }
    else{
        // si no hay un proceso suspendido
        cleanNextNewProcess();
        imprimirNextNewProccessMsg();
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////
}
void setProceso(Marco* memoria,list<Proceso> nuevos, RandomNum & intDiceNum, RandomNum & intDiceOperador, RandomNum & intDiceTme, RandomNum & intDiceMem,unsigned int& _id, char* operaciones, unsigned int &_QUANTUM){

    unsigned int var_=0,_pos_f=10,_cont=0,initial_pos_finalized=5, _quantum=1;                  //'_cont' contador global del programa
    char tecla;                                             // 'tecla' utilizada utilizada por kbhit()
    bool end = false;                                       // sirve para controlar el proceso en ejecucion
    int limit = 0;                                          // sirve para controlar los procesos en memoria principal
    size_t remaining_p = 0;

    ////////// listas de los diferentes estados
    list<Proceso> listos;
    list<Proceso> bloqueados;
    list<Proceso> terminados;
    list<Proceso> suspendidos;
    //////////

    Proceso suspendido;                                     // para controlar el proceso suspendido

    pintar_escenario();                                     // imprime todas las etiquetas en pantalla
    imprimirNextProccessMsg();
    printMemoria(memoria);


    ////////////  ingresa 5 o menos procesos a memoria principal
    while (!nuevos.empty()) {

        listos.push_back(nuevos.front());                   // los agrega a la cola de listos
        limit++;

        nuevos.pop_front();
    }
    ///////////

    actualizarColaListos(listos);                          // imprime en pantalla los procesos de listos
    remaining_p = nuevos.size();
    printRemainingNumProcess(remaining_p);                 // imprime la cantidad de  procesos restantes

    Sleep(2000);
    if(kbhit()){
        tecla = getch();

        if(tecla == 'p'){                                 // PAUSA
            while(getch() != 99){

            }
        }
        if(tecla == 'n'){                                 // NUEVO PROCESO
            Proceso proceso(newProcess(intDiceNum,intDiceOperador,intDiceTme,intDiceMem,_id,operaciones));  // Crea un proceso nuevo
            if ( nuevos.empty()){
                // si la lista de nuevos esta vacia,
                // quiere decir que el proceso que se agregue
                // pretende entrar a memoria principal,
                // por lo que es necesario comprobar si el proceso cabe


                if(insertarProcesoMemoria(memoria,proceso)){
                    // Si existe espacio en memoria ///

                    proceso.setArrivalTime(_cont);               // Tiempo de llegada
                    nuevos.push_back(proceso);                  // lo encola en los proceso nuevos
                    remaining_p++;
                    printRemainingNumProcess(remaining_p);      // imprime los procesos restantes


                    listos.push_back(nuevos.front());       // encola en listos un proceso de la cola de nuevos
                    nuevos.pop_front();
                    remaining_p--;
                    actualizarColaListos(listos);           // actualiza en pantalla los procesos listos
                    limit++;
                    printRemainingNumProcess(remaining_p);

                    //actualiza el estatus de la memoria
                    updateMemoryScreen(memoria);

                }
                else{
                    // si no existe espacio en memoria
                    nuevos.push_back(proceso);
                    // aumenta el contador de nuevos
                    remaining_p++;
                    printRemainingNumProcess(remaining_p);  // actualiza contador de procesos nuevos

                }
            }
            else{
                // si no esta vacía quiere decir que no es necesario comprobar
                // si el proceso cabe en memoria

                // si no existe espacio en memoria
                nuevos.push_back(proceso);
                // aumenta el contador de nuevos
                remaining_p++;
                printRemainingNumProcess(remaining_p);  // actualiza contador de procesos nuevos
            }
            printDataCommingProccess(suspendido, nuevos);
        }
        if(tecla == 'b'){
            system("cls");

            // imprime  BCP de listos
            print_header_BCP();
            BCP_listos(listos,initial_pos_finalized,_cont);
            print_final_content(initial_pos_finalized);

            while(getch() != 99){

            }
            update_scenario(listos,bloqueados,terminados,_cont,_QUANTUM);
            printRemainingNumProcess(remaining_p);
            updateMemoryScreen(memoria);

            printDataCommingProccess(suspendido, nuevos);

        }
        if( tecla == 't'){
            system("cls");

            printMemoriaFrom(memoria,1);
            cout << endl << "Presione <C> para continuar..." << endl;

            while ( getch() != 'c'){

            }
            update_scenario(listos,bloqueados,terminados,_cont,_QUANTUM);
            printRemainingNumProcess(remaining_p);
            updateMemoryScreen(memoria);

            printDataCommingProccess(suspendido, nuevos);
        }
    }

    while(!listos.empty() or !bloqueados.empty() or !suspendidos.empty()){      // mientras haya procesos en listos y en bloqueados

        if ( listos.empty() and bloqueados.empty() and !suspendidos.empty()){
///////////////////////////////// SOLO SUSPENDIDOS /////////////////////////////////////////////////////
            // si no hay procesos en listos ni en
            // bloqueados pero si hay almenos un  proceso suspendido
            // entonces, el progr/ama no terminará
            _cont++;
            printGlobalCounter(_cont);                  // actualiza el contador global
            Sleep(800);
            if(kbhit()){
                tecla = getch();

                if(tecla == 'p'){
                    while(getch() != 99){

                    }
                }
                if(tecla == 'n'){
                    // genera un nuevo proceso
                    Proceso proceso(newProcess(intDiceNum,intDiceOperador,intDiceTme, intDiceMem, _id,operaciones));

                    if ( nuevos.empty()){

                        if ( insertarProcesoMemoria(memoria, proceso)){

                            proceso.setArrivalTime(_cont); // asigna el tiempo de llegada

                            nuevos.push_back(proceso);    // lo agrega a la cola de listos
                            remaining_p++;
                            printRemainingNumProcess(remaining_p);  // actualiza contador de procesos nuevos


                            listos.push_back(nuevos.front());   // el nuevo proceso lo coloca en la cola de listos
                            nuevos.pop_front();
                            remaining_p--;
                            actualizarColaListos(listos);
                            limit++;
                            printRemainingNumProcess(remaining_p);

                            // actualiza el estado de la memoria
                            updateMemoryScreen(memoria);

                        }
                        else{
                            nuevos.push_back(proceso);

                            remaining_p++;
                            printRemainingNumProcess(remaining_p);  // actualiza contador de procesos nuevos
                        }
                    }
                    else{
                        nuevos.push_back(proceso);

                        remaining_p++;
                        printRemainingNumProcess(remaining_p);  // actualiza contador de procesos nuevos
                    }

                    printDataCommingProccess(suspendido, nuevos);


                }

                if(tecla == 'b'){
                    system("cls");
                    print_header_BCP();
                    BCP_listos(listos,initial_pos_finalized,_cont);           // BCP de listos
                    BCP_bloquedos(bloqueados,initial_pos_finalized,_cont);    // BCP de bloqueados
                    BCP_terminados(terminados,initial_pos_finalized,_cont);   // BCP de terminados
                    BCP_suspendidos(suspendidos,initial_pos_finalized,_cont);

                    print_final_content(initial_pos_finalized);         // imprime la forma del contenedor del BCP
                    cout<<endl<<endl;

                    while(getch() != 99){

                    }
                    update_scenario(listos,bloqueados,terminados,_cont,_QUANTUM);  // regresa a la pantalla antes de mostrar el BCP
                    printRemainingNumProcess(remaining_p);
                    updateMemoryScreen(memoria);

                    printDataCommingProccess(suspendido, nuevos);

                }
                if( tecla == 't'){
                    system("cls");

                    printMemoriaFrom(memoria,1);
                    cout << endl << "Presione <c> para continuar..." << endl;

                    while ( getch() != 'c'){

                    }
                    update_scenario(listos,bloqueados,terminados,_cont,_QUANTUM);
                    printRemainingNumProcess(remaining_p);
                    updateMemoryScreen(memoria);

                    printDataCommingProccess(suspendido, nuevos);
                }
                if ( tecla == 'r'){
                    if (existProcessSuspended()){
                        // si existe
                        // obtiene el primer registro del archivo
                        Proceso proceso(getFirstSuspendedProcess());

                        if(insertarProcesoMemoria(memoria, proceso)){
                            // si el proceso suspendido cabe en memoria

                            // actuaiza el status de la memoria
                            updateMemoryScreen(memoria);

                            //// quita de suspendidos, ya que acaba de regresar
                            if(!suspendidos.empty()){

                                Proceso a(suspendidos.front());
                                suspendidos.pop_front();


                                // removerlo del archivo
                                if ( removeFirstSuspendedProcess() ){
                                    // remueve el proceso del archivo que acaba de entrar a memoria
                                    // si retorna verdadero significa que hay mas registros
                                    // en memoria secundaria

                                    // obtiene  la info del siguiente proceso suspendido que etrara a memoria
                                    Proceso pros(getNextSuspendeProccessInfo());



                                    // actualizar los datos
                                    suspendido.setSuspended(true);
                                    suspendido.setId(pros.getId());
                                    suspendido.setMemorySize(pros.getMemorySize());

                                }
                                else{
                                    // significa que no hay mas procesos suspendidos
                                    suspendido.setSuspended(false);
                                }

                                // actualiza el procesos suspendido
                                if ( suspendido.getSuspended()){
                                    // si  esta suspendido en memoria secundaria entonces hay que mostrar sus datos
                                    cleanNextProcess();
                                    pintarNextProccess(suspendido);
                                }
                                else{
                                    // si no hay un proceso suspendido
                                    cleanNextProcess();
                                    imprimirNextProccessMsg();
                                }

                                //// limpia y actualiza la cola de bloqueados
                                bloqueados.push_back(a);
                                delete_all_bloqueados();

                                if(bloqueados.front().getTtb() == 10){        // si ya acabo su tiempo
                                    Proceso temp(bloqueados.front());

                                    temp.setTtb(0);
                                    listos.push_back(temp);
                                    bloqueados.pop_front();
                                    actualizarColaListos(listos);
                                    // limpia la pantalla de bloqueados
                                    actualizarBloqueados(bloqueados);

                                }
                                else{

                                    // sino, incrementa su contador en una unidad a cada procceso de bloqueados
                                    // limpia la pantalla de bloqueados

                                    print_all_bloqueados(bloqueados);
                                    printGlobalCounter(_cont);
                                    _cont++;

                                    if(bloqueados.front().getTtb() == 10){        // si ya acabo su tiempo
                                        Proceso temp(bloqueados.front());

                                        temp.setTtb(0);
                                        listos.push_back(temp);
                                        bloqueados.pop_front();
                                        actualizarColaListos(listos);
                                        // limpia la pantalla de bloqueados
                                        delete_all_bloqueados();
                                        actualizarBloqueados(bloqueados);

                                    }

                                }
                                ///// fin limpiar blloqueados /////////////
                            }

                        }

                    }
                }
            }

///////////////////////////////// FIN SOLO SUSPENDIDOS /////////////////////////////////////////////
        }
        else if(listos.empty()){                             // si listos  esta vacio
//////////////////////////////// SI NO HAY PROCESOS PARA EJECUCION //////////////////////////////////
            printDataCommingProccess(suspendido, nuevos);

            printGlobalCounter(_cont);                  // actualiza el contador global

            if(!bloqueados.empty()){
                // si la lista de bloquedos no esta vacia



                if(bloqueados.front().getTtb() == 10){
                    Proceso temp(bloqueados.front());  // obtiene el proceso que ya cumplio su cuota de espeera en bloqueado

                    temp.setTtb(0);                    // inicializa variable temporal de espera de bloqueado
                    listos.push_back(temp);
                    bloqueados.pop_front();
                    delete_all_bloqueados();           // limpia escenario de bloqueados

                    actualizarBloqueados(bloqueados);  // actualiza la informacion en pantalla
                    actualizarColaListos(listos);      // actualiza la informacion de listos
                }
                else{
                    printGlobalCounter(_cont);
                    print_all_bloqueados(bloqueados);  // actualiza todos los procesos de bloqueados e incrementa en uno su tiempo de espera
                    printGlobalCounter(_cont);
                    _cont++;
                }

            }
            Sleep(800);
            if(kbhit()){
                tecla = getch();

                if(tecla == 'p'){
                    while(getch() != 99){

                    }
                }
                if(tecla == 'n'){
                    // genera un nuevo proceso
                    Proceso proceso(newProcess(intDiceNum,intDiceOperador,intDiceTme, intDiceMem, _id,operaciones));

                    if ( nuevos.empty()){

                        if ( insertarProcesoMemoria(memoria, proceso)){

                            proceso.setArrivalTime(_cont); // asigna el tiempo de llegada

                            nuevos.push_back(proceso);    // lo agrega a la cola de listos
                            remaining_p++;
                            printRemainingNumProcess(remaining_p);  // actualiza contador de procesos nuevos


                            listos.push_back(nuevos.front());   // el nuevo proceso lo coloca en la cola de listos
                            nuevos.pop_front();
                            remaining_p--;
                            actualizarColaListos(listos);
                            limit++;
                            printRemainingNumProcess(remaining_p);

                            // actualiza el estado de la memoria
                            updateMemoryScreen(memoria);

                        }
                        else{
                            nuevos.push_back(proceso);

                            remaining_p++;
                            printRemainingNumProcess(remaining_p);  // actualiza contador de procesos nuevos
                        }
                    }
                    else{
                        nuevos.push_back(proceso);

                        remaining_p++;
                        printRemainingNumProcess(remaining_p);  // actualiza contador de procesos nuevos
                    }

                    printDataCommingProccess(suspendido, nuevos);


                }

                if(tecla == 'b'){
                    system("cls");
                    print_header_BCP();
                    BCP_listos(listos,initial_pos_finalized,_cont);           // BCP de listos
                    BCP_bloquedos(bloqueados,initial_pos_finalized,_cont);    // BCP de bloqueados
                    BCP_terminados(terminados,initial_pos_finalized,_cont);   // BCP de terminados
                    BCP_suspendidos(suspendidos,initial_pos_finalized,_cont);

                    print_final_content(initial_pos_finalized);         // imprime la forma del contenedor del BCP
                    cout<<endl<<endl;

                    while(getch() != 99){

                    }
                    update_scenario(listos,bloqueados,terminados,_cont,_QUANTUM);  // regresa a la pantalla antes de mostrar el BCP
                    printRemainingNumProcess(remaining_p);
                    updateMemoryScreen(memoria);

                    printDataCommingProccess(suspendido, nuevos);

                }
                if( tecla == 't'){
                    system("cls");

                    printMemoriaFrom(memoria,1);
                    cout << endl << "Presione <c> para continuar..." << endl;

                    while ( getch() != 'c'){

                    }
                    update_scenario(listos,bloqueados,terminados,_cont,_QUANTUM);
                    printRemainingNumProcess(remaining_p);
                    updateMemoryScreen(memoria);

                    printDataCommingProccess(suspendido, nuevos);
                }
                if (tecla == 's'){
                    // suspendidos
                    if(!bloqueados.empty()){

                        // si bloqueados no esta vacio

                        // obtiene el primer elemento de bloqueados
                        Proceso susp(bloqueados.front());
                        if( elimProcesoMemoria(memoria, susp)){
                            // si el proceso se elimina correctamente de memoria principal

                            // actuaiza el status de la memoria
                            updateMemoryScreen(memoria);
                            if (saveProccessToFile(susp)) {
                                // if the proccess was saved to file successfully
                                bloqueados.pop_front();

                                // añadir a suspendidos
                                suspendidos.push_back(susp);

                                // limpia la pantalla de bloqueados
                                delete_all_bloqueados();
                                //// limpia y actualiza la cola de bloqueados

                                if(bloqueados.front().getTtb() == 10){        // si ya acabo su tiempo
                                    Proceso temp(bloqueados.front());

                                    temp.setTtb(0);
                                    listos.push_back(temp);
                                    bloqueados.pop_front();
                                    actualizarColaListos(listos);
                                    // limpia la pantalla de bloqueados
                                    delete_all_bloqueados();
                                    actualizarBloqueados(bloqueados);

                                }
                                else{

                                    // sino, incrementa su contador en una unidad a cada procceso de bloqueados
                                    // limpia la pantalla de bloqueados

                                    print_all_bloqueados(bloqueados);
                                    _cont++;
                                    if(bloqueados.front().getTtb() == 10){        // si ya acabo su tiempo
                                        Proceso temp(bloqueados.front());

                                        temp.setTtb(0);
                                        listos.push_back(temp);
                                        bloqueados.pop_front();
                                        actualizarColaListos(listos);
                                        // limpia la pantalla de bloqueados
                                        delete_all_bloqueados();
                                        actualizarBloqueados(bloqueados);

                                    }

                                }
                                ///// fin limpiar blloqueados /////////////





                                if (! suspendido.getSuspended()){
                                    // si no hay ningun proceso suspendido

                                    // save the new data to the suspended proccess
                                    suspendido.setId(susp.getId());
                                    suspendido.setMemorySize(susp.getMemorySize());
                                    suspendido.setSuspended(true);

                                    // imprime en pantalla los datos del nuevo proceso supendido
                                    cleanNextProcess();
                                    pintarNextProccess(suspendido);
                                }

                            }

                        }
                    }
                }
                if ( tecla == 'r'){
                    if (existProcessSuspended()){
                        // si existe
                        // obtiene el primer registro del archivo
                        Proceso proceso(getFirstSuspendedProcess());

                        if(insertarProcesoMemoria(memoria, proceso)){
                            // si el proceso suspendido cabe en memoria

                            // actuaiza el status de la memoria
                            updateMemoryScreen(memoria);

                            //// quita de suspendidos, ya que acaba de regresar
                            if(!suspendidos.empty()){

                                Proceso a(suspendidos.front());
                                suspendidos.pop_front();


                                // removerlo del archivo
                                if ( removeFirstSuspendedProcess() ){
                                    // remueve el proceso del archivo que acaba de entrar a memoria
                                    // si retorna verdadero significa que hay mas registros
                                    // en memoria secundaria

                                    // obtiene  la info del siguiente proceso suspendido que etrara a memoria
                                    Proceso pros(getNextSuspendeProccessInfo());



                                    // actualizar los datos
                                    suspendido.setSuspended(true);
                                    suspendido.setId(pros.getId());
                                    suspendido.setMemorySize(pros.getMemorySize());

                                }
                                else{
                                    // significa que no hay mas procesos suspendidos
                                    suspendido.setSuspended(false);
                                }

                                // actualiza el procesos suspendido
                                if ( suspendido.getSuspended()){
                                    // si  esta suspendido en memoria secundaria entonces hay que mostrar sus datos
                                    cleanNextProcess();
                                    pintarNextProccess(suspendido);
                                }
                                else{
                                    // si no hay un proceso suspendido
                                    cleanNextProcess();
                                    imprimirNextProccessMsg();
                                }

                                //// limpia y actualiza la cola de bloqueados
                                bloqueados.push_back(a);
                                delete_all_bloqueados();

                                if(bloqueados.front().getTtb() == 10){        // si ya acabo su tiempo
                                    Proceso temp(bloqueados.front());

                                    temp.setTtb(0);
                                    listos.push_back(temp);
                                    bloqueados.pop_front();
                                    actualizarColaListos(listos);
                                    // limpia la pantalla de bloqueados
                                    actualizarBloqueados(bloqueados);

                                }
                                else{

                                    // sino, incrementa su contador en una unidad a cada procceso de bloqueados
                                    // limpia la pantalla de bloqueados

                                    print_all_bloqueados(bloqueados);
                                    printGlobalCounter(_cont);
                                    _cont++;

                                    if(bloqueados.front().getTtb() == 10){        // si ya acabo su tiempo
                                        Proceso temp(bloqueados.front());

                                        temp.setTtb(0);
                                        listos.push_back(temp);
                                        bloqueados.pop_front();
                                        actualizarColaListos(listos);
                                        // limpia la pantalla de bloqueados
                                        delete_all_bloqueados();
                                        actualizarBloqueados(bloqueados);

                                    }

                                }
                                ///// fin limpiar blloqueados /////////////
                            }

                        }

                    }
                }
            }
//////////////////////////////// FIN NO HAY PROCESOS PARA EJECUCION //////////////////////////////////
        }else{   // si hay procesos listos
//////////////////////////////// SI HAY PROCESOS LISOS PARA EJECUTARSE //////////////////////////////
            printDataCommingProccess(suspendido, nuevos);
            Proceso p_actual(listos.front());
            listos.pop_front();

            // si el proceso n fue ejecutado
            if(!p_actual.getWas_execute()){
                p_actual.setResponseTime(_cont);  //coloca el contador global
                p_actual.setWas_execute(true);
            }

            printRemainingNumProcess(remaining_p);

            cleanListos();                       // limplia el escenario de procesos listos
            actualizarColaListos(listos);        // actualiza con la informacion de listos
            update_executing_process(p_actual);  // muestra los estaticos del proceso a ejecutarse

            p_actual.setRes(getResultado(p_actual));    // procesa la operacion y se lo asigna al proceso

            var_ = p_actual.getTt();                    // el proceso de ejecucion continuara justo donde se quedó
            _quantum=1;

            while(!end){

                while( (var_ < p_actual.getTme()) and !kbhit() and (_quantum <= _QUANTUM)){    // mientras no se haya completado el TME y no se haya tcleado algo

                    var_++;
                    _cont++;
                    showExecutingProcess(p_actual.getTme(),var_,_cont); // muestra los valores cambiantes del proceso: TT, contador global, TR

                    _quantum++;

                    //// bloqueados
                    ///

                    if(!bloqueados.empty()){                            // si bloqueados no esta vacio
                        if(bloqueados.front().getTtb() == 10){           // si ya se acabo su tiempo de espera en bloqueados
                            Proceso temp(bloqueados.front());

                            temp.setTtb(0);                             // reinicia su contador temporal de espera de bloqueado

                            listos.push_back(temp);                     // lo agrega a la cola de listos
                            bloqueados.pop_front();
                            delete_all_bloqueados();                    // limpia pantalla de bloqueados
                            actualizarColaListos(listos);               // actualiza listos
                        }
                        print_all_bloqueados(bloqueados);               // imcrementa en una la estancia de los procesos en bloqueado e imprime
                    }
                    //updateWaitingTimeListos(listos);                    // incrementa el tiempo de espera a todos los procesos listos en una unidad

                    Sleep(800);
                }



                if(kbhit()){

                    tecla = getch();   //obtiene tecla
                    if(tecla == 'p'){

                        while(getch() != 99){

                        }

                    }
                    if(tecla == 'e'){   // ERROR

                        if (elimProcesoMemoria(memoria, p_actual)){
                            // si se eliminan las pai¡ginas del proceso de la memoria


                            // actuaiza el status de la memoria
                            updateMemoryScreen(memoria);

                            limpiar_ejecucion();

                            // actualiza tiempo de fializacion y de servicio
                            p_actual.setFinishedTime(_cont);
                            p_actual.setServiceTime(var_);
                            p_actual.setFinished(true);
                            p_actual.setSuccess(false);

                            mover_finalizado_error(p_actual,_pos_f);  // imprime en pantalla el proceso finalizado
                            _pos_f++; //pos finalizado

                            extendFinalizadosContent(_pos_f);         // funcio que imprime el contorno de finalizados
                            end = true;                               // cambia el estado de la bandera a finalizado = verdad

                            // AGREGAR un nuevo proceso a memoria principal
                            if (!nuevos.empty()){
                                // si no esta vacía

                                Proceso temp(nuevos.front());

                                if ( insertarProcesoMemoria(memoria, temp)){
                                    // si cabe en memoria principal

                                    temp.setArrivalTime(_cont);
                                    listos.push_back(temp);
                                    nuevos.pop_front();
                                    remaining_p--;              // disminuye la cantidad de procesos nuevos

                                    // actuaiza el status de la memoria
                                    updateMemoryScreen(memoria);

                                }
                            }
                            terminados.push_back(p_actual);
                            printRemainingNumProcess(remaining_p);

                            printDataCommingProccess(suspendido, nuevos);

                        }

                    }
                    if(tecla == 'n'){
                        Proceso proceso(newProcess(intDiceNum,intDiceOperador,intDiceTme, intDiceMem,_id,operaciones));
                        if ( nuevos.empty()){

                            if( insertarProcesoMemoria(memoria, proceso)){

                                proceso.setArrivalTime(_cont);
                                nuevos.push_back(proceso);
                                remaining_p++;
                                printRemainingNumProcess(remaining_p);


                                listos.push_back(nuevos.front());
                                nuevos.pop_front();
                                remaining_p--;
                                actualizarColaListos(listos);
                                limit++;

                                // actuaiza el status de la memoria
                                updateMemoryScreen(memoria);

                            }
                            else{
                                nuevos.push_back(proceso);
                                remaining_p++;
                            }
                        }
                        else{
                            nuevos.push_back(proceso);
                            remaining_p++;
                        }
                        printRemainingNumProcess(remaining_p);

                        printDataCommingProccess(suspendido, nuevos);
                    }
                    if(tecla == 'i'){  // INTERRUPCION

                        limpiar_ejecucion();

                        ////// BLOQUEADOS
                        ///

                        if(var_== p_actual.getTt() and !bloqueados.empty()){ // si ya termino su TME y bloqueados no esta vacio
                            if(bloqueados.front().getTtb() == 10){        // si ya acabo su tiempo
                                Proceso temp(bloqueados.front());

                                temp.setTtb(0);
                                listos.push_back(temp);
                                bloqueados.pop_front();
                                delete_all_bloqueados();
                                actualizarColaListos(listos);
                            }
                            else{
                                // sino, incrementa su contador en una unidad a cada procceso de bloqueados

                                print_all_bloqueados(bloqueados);
                                _cont++;
                            }

                            Sleep(800);
                        }

                        printGlobalCounter(_cont);
                        p_actual.setTt(var_);           // actualiza el tiempo transcurrido del proceso
                        bloqueados.push_back(p_actual);


                        end = true;
                    }
                    if(tecla == 'b'){  // BCP
                        system("cls");

                        p_actual.setTt(var_);           // actualiza el tiempo transcurrido del proceso

                        BCP(listos,bloqueados,terminados,suspendidos,p_actual,_cont); //imprime el BCP de todas las listas
                        cout<<endl<<endl;



                        while(getch() != 99){

                        }

                        update_scenario(listos,bloqueados,terminados,_cont,_QUANTUM);        // actualiza escenario justo antes de mmostrar BCP
                        update_executing_process(p_actual);                         // muestra el BCP del proceso actual
                        printRemainingNumProcess(remaining_p);
                        updateMemoryScreen(memoria);

                        printDataCommingProccess(suspendido, nuevos);


                    }
                    if( tecla == 't'){
                        system("cls");

                        printMemoriaFrom(memoria,1);
                        cout << endl << "Presione <C> para continuar..." << endl;

                        while ( getch() != 'c'){

                        }
                        update_scenario(listos,bloqueados,terminados,_cont,_QUANTUM);
                        update_executing_process(p_actual);
                        printRemainingNumProcess(remaining_p);
                        updateMemoryScreen(memoria);

                        printDataCommingProccess(suspendido, nuevos);
                    }
                    if (tecla == 's'){
                        // suspendidos
                        if(!bloqueados.empty()){

                            // si bloqueados no esta vacio

                            // obtiene el primer elemento de bloqueados
                            Proceso susp(bloqueados.front());
                            if( elimProcesoMemoria(memoria, susp)){
                                // si el proceso se elimina correctamente de memoria principal

                                // actuaiza el status de la memoria
                                updateMemoryScreen(memoria);
                                if (saveProccessToFile(susp)) {
                                    // if the proccess was saved to file successfully
                                    bloqueados.pop_front();

                                    // añadir a suspendidos
                                    suspendidos.push_back(susp);

                                    // limpia la pantalla de bloqueados
                                    delete_all_bloqueados();
                                    //// limpia y actualiza la cola de bloqueados

                                    if(bloqueados.front().getTtb() == 10){        // si ya acabo su tiempo
                                        Proceso temp(bloqueados.front());

                                        temp.setTtb(0);
                                        listos.push_back(temp);
                                        bloqueados.pop_front();
                                        actualizarColaListos(listos);
                                        // limpia la pantalla de bloqueados
                                        delete_all_bloqueados();
                                        actualizarBloqueados(bloqueados);

                                    }
                                    else{

                                        // sino, incrementa su contador en una unidad a cada procceso de bloqueados
                                        // limpia la pantalla de bloqueados

                                        print_all_bloqueados(bloqueados);
                                        _cont++;
                                        if(bloqueados.front().getTtb() == 10){        // si ya acabo su tiempo
                                            Proceso temp(bloqueados.front());

                                            temp.setTtb(0);
                                            listos.push_back(temp);
                                            bloqueados.pop_front();
                                            actualizarColaListos(listos);
                                            // limpia la pantalla de bloqueados
                                            delete_all_bloqueados();
                                            actualizarBloqueados(bloqueados);

                                        }

                                    }
                                    ///// fin limpiar blloqueados /////////////


                                    if (! suspendido.getSuspended()){
                                        // si no hay ningun proceso suspendido

                                        // save the new data to the suspended proccess
                                        suspendido.setId(susp.getId());
                                        suspendido.setMemorySize(susp.getMemorySize());
                                        suspendido.setSuspended(true);

                                        // imprime en pantalla los datos del nuevo proceso supendido
                                        cleanNextProcess();
                                        pintarNextProccess(suspendido);
                                    }

                                }

                            }
                        }
                    }
                    if ( tecla == 'r'){
                        if (existProcessSuspended()){
                            // si existe
                            // obtiene el primer registro del archivo
                            Proceso proceso(getFirstSuspendedProcess());

                            if(insertarProcesoMemoria(memoria, proceso)){
                                // si el proceso suspendido cabe en memoria

                                // actuaiza el status de la memoria
                                updateMemoryScreen(memoria);

                                //// quita de suspendidos, ya que acaba de regresar
                                if(!suspendidos.empty()){

                                    Proceso a(suspendidos.front());
                                    suspendidos.pop_front();



                                    // removerlo del archivo
                                    if ( removeFirstSuspendedProcess() ){
                                        // remueve el proceso del archivo que acaba de entrar a memoria
                                        // si retorna verdadero significa que hay mas registros
                                        // en memoria secundaria

                                        // obtiene  la info del siguiente proceso suspendido que etrara a memoria
                                        Proceso pros(getNextSuspendeProccessInfo());



                                        // actualizar los datos
                                        suspendido.setSuspended(true);
                                        suspendido.setId(pros.getId());
                                        suspendido.setMemorySize(pros.getMemorySize());

                                    }
                                    else{
                                        // significa que no hay mas procesos suspendidos
                                        suspendido.setSuspended(false);
                                    }

                                    // actualiza el procesos suspendido
                                    if ( suspendido.getSuspended()){
                                        // si  esta suspendido en memoria secundaria entonces hay que mostrar sus datos
                                        cleanNextProcess();
                                        pintarNextProccess(suspendido);
                                    }
                                    else{
                                        // si no hay un proceso suspendido
                                        cleanNextProcess();
                                        imprimirNextProccessMsg();
                                    }

                                    //// limpia y actualiza la cola de bloqueados
                                    bloqueados.push_back(a);
                                    delete_all_bloqueados();

                                    if(bloqueados.front().getTtb() == 10){        // si ya acabo su tiempo
                                        Proceso temp(bloqueados.front());

                                        temp.setTtb(0);
                                        listos.push_back(temp);
                                        bloqueados.pop_front();
                                        actualizarColaListos(listos);
                                        // limpia la pantalla de bloqueados
                                        actualizarBloqueados(bloqueados);

                                    }
                                    else{

                                        // sino, incrementa su contador en una unidad a cada procceso de bloqueados
                                        // limpia la pantalla de bloqueados

                                        print_all_bloqueados(bloqueados);
                                        printGlobalCounter(_cont);
                                        _cont++;

                                        if(bloqueados.front().getTtb() == 10){        // si ya acabo su tiempo
                                            Proceso temp(bloqueados.front());

                                            temp.setTtb(0);
                                            listos.push_back(temp);
                                            bloqueados.pop_front();
                                            actualizarColaListos(listos);
                                            // limpia la pantalla de bloqueados
                                            delete_all_bloqueados();
                                            actualizarBloqueados(bloqueados);

                                        }

                                    }
                                    ///// fin limpiar blloqueados /////////////
                                }

                            }

                        }
                    }
                }else if(var_ == p_actual.getTme()){
                    // TERMINACION CON EXITO
                    if ( elimProcesoMemoria(memoria,p_actual)){
                        //si se eliminan las pagina del proceso exitosamente

                        // actuaiza el status de la memoria
                        updateMemoryScreen(memoria);

                        limpiar_ejecucion();


                        // actualiza tiempo de finalizacion y de servicio del procesp
                        p_actual.setFinishedTime(_cont);
                        p_actual.setServiceTime(var_);
                        p_actual.setFinished(true);
                        p_actual.setSuccess(true);

                        mover_finalizado(p_actual,_pos_f);
                        _pos_f++; //pos finalizado

                        extendFinalizadosContent(_pos_f);
                        end = true;

                        // AGREGAR un nuevo proceso a memoria principal
                        if (!nuevos.empty()){
                            // si no esta vacía

                            Proceso temp(nuevos.front());

                            if ( insertarProcesoMemoria(memoria, temp)){
                                // si cabe en memoria principal

                                temp.setArrivalTime(_cont);
                                listos.push_back(temp);
                                nuevos.pop_front();
                                remaining_p--;              // disminuye la cantidad de procesos nuevos

                                // actuaiza el status de la memoria
                                updateMemoryScreen(memoria);
                            }
                        }
                        terminados.push_back(p_actual);
                        printRemainingNumProcess(remaining_p);

                        printDataCommingProccess(suspendido, nuevos);
                    }

                }
                else{
                    // Aqui se turna ROUND ROBIN
                    limpiar_ejecucion();
                    p_actual.setTt(var_);
                    listos.push_back(p_actual);
                    actualizarColaListos(listos);


                    end = true;
                }
//////////////////////////////// FIN HAY PROCESOS LISOS PARA EJECUTARSE //////////////////////////////
            }
            end = false;
            var_ = 0;                                 // inicializa variables de control

        }
        initial_pos_finalized = 5;
    }

    gotoxy(1,45);
    system("pause");
    system("cls");

    print_header_BCP();
    initial_pos_finalized = 5;
    BCP_terminados_suspendidos(terminados, suspendidos,initial_pos_finalized,_cont);
    print_final_content(initial_pos_finalized);

    int a = int(initial_pos_finalized);

    a = a + 4;
    gotoxy(65,a);
    cout << ".: Memoria :.";
    a = a + 2;
    printMemoriaFrom(memoria,a);


    cout<<endl<<endl;

}

void print_final_content(unsigned int &_fila){

    add_vertical_line(1,4,_fila);
    add_vertical_line(165,4,_fila);
    gotoxy(1,_fila+1);
    cout<<"+";
    gotoxy(165,_fila+1);
    cout<<"+";
    add_horizontal_line(_fila+1,2,164);
}

void print_header_BCP(){

    gotoxy(1,1);
    cout<<setw(80)<<" .: BCP :.\n";

    gotoxy(1,3);
    cout<<"| ID";

    gotoxy(10,3);
    cout<<"| Llegada";

    gotoxy(22,3);
    cout<<"| Respuesta";

    gotoxy(40,3);
    cout<<"| Espera";

    gotoxy(53,3);
    cout<<"| Servicio";

    gotoxy(70,3);
    cout<<"| TME";

    gotoxy(80,3);
    cout<<"| Retorno";

    gotoxy(95,3);
    cout<<"| Finalizacion";

    gotoxy(118,3);
    cout<<"| Operacion";

    gotoxy(138,3);
    cout<<"| Resultado";

    gotoxy(158,3);
    cout<<"|Estado";

    add_horizontal_line(4,2,164);
    gotoxy(1,4);
    cout<<"+";
    add_horizontal_line(2,2,164);
    gotoxy(1,2);
    cout<<"+";

    gotoxy(165,2);
    cout<<"+";

    gotoxy(165,4);
    cout<<"+";

    gotoxy(165,3);
    cout<<"|";
}


void print_BCP_process(Proceso &_actual, unsigned int & inc,unsigned int & _CONT){
    unsigned int retorno = 0, finalizacion = 0;

    gotoxy(2,inc);
    cout<<_actual.getId();

    gotoxy(11,inc);
    cout<<_actual.getArrivalTime();

    // si ya ha sido ejecutado el proceso
    if(_actual.getWas_execute()){
        gotoxy(23,inc);
        cout<<_actual.getResponseTime() - _actual.getArrivalTime();
    }
    else{
        // si no ha sido ejecutado
        gotoxy(23,inc);
        cout<<"-";
    }

    gotoxy(71,inc);
    cout<<_actual.getTme();

    gotoxy(119,inc);
    cout<<_actual.getNum1()<<" "<<_actual.getOperacion()<<" "<<_actual.getNum2();


    // si ya ha finalzado
    if(_actual.getFinished()){

        retorno = _actual.getFinishedTime()-_actual.getArrivalTime();

        //espera
        gotoxy(41,inc);
        cout<<retorno - _actual.getServiceTime();

        //servicio
        gotoxy(54,inc);
        cout<<_actual.getServiceTime();


        //retorno
        gotoxy(81,inc);
        cout<<retorno;

        //finalizacion
        gotoxy(96,inc);
        cout<<_actual.getFinishedTime();

        //resultado
        gotoxy(139,inc);
        if(_actual.getSuccess()){
            cout<<_actual.getRes();

        }else{
            cout<<"ERROR";
        }

    }else{
        // si no ha fializado
        gotoxy(41,inc);

        finalizacion = _CONT;
        retorno = finalizacion - _actual.getArrivalTime();
        _actual.setWaintingTotalTime(retorno - _actual.getTt());

        cout<<_actual.getWaintingTotalTime();


        gotoxy(54,inc);
        cout<<_actual.getTt();

        gotoxy(81,inc);
        cout<<"-";

        gotoxy(96,inc);
        cout<<"-";

        gotoxy(139,inc);
        cout<<"-";
    }

}
void BCP_terminados_suspendidos(list<Proceso> &terminados, list<Proceso> &suspendidos, unsigned int &inc, unsigned int&_CONT){
    BCP_terminados(terminados, inc, _CONT);
    BCP_suspendidos(suspendidos,inc,_CONT);

}
void BCP_suspendidos(list<Proceso> &suspendidos, unsigned int &inc, unsigned int&_CONT){

    for (list<Proceso>::iterator  _actual = suspendidos.begin(); _actual != suspendidos.end(); ++_actual) {
        print_BCP_process(*_actual,inc,_CONT);
        gotoxy(160,inc);
        cout<<"S";
        inc++;
    }

}

void BCP_listos(list<Proceso> &_listos, unsigned int &inc, unsigned int&_CONT){

    for (list<Proceso>::iterator  _actual = _listos.begin(); _actual != _listos.end(); ++_actual) {
        print_BCP_process(*_actual,inc,_CONT);
        gotoxy(160,inc);
        cout<<"L";
        inc++;
    }

}

void BCP_bloquedos(list<Proceso> &_bloqueado, unsigned int &inc, unsigned int & _CONT){


    for (list<Proceso>::iterator  _actual = _bloqueado.begin(); _actual != _bloqueado.end(); ++_actual) {
        print_BCP_process(*_actual,inc, _CONT);
        gotoxy(160,inc);
        cout<<"B";
        inc++;
    }

}

void BCP_Atual_process(Proceso & _actual,unsigned int & inc, unsigned int & _CONT){
    print_BCP_process(_actual,inc, _CONT);
    gotoxy(160,inc);
    cout<<"E";
    inc++;
}

void BCP_terminados(list<Proceso> terminados,unsigned int & inc, unsigned int & _CONT){

    for (list<Proceso>::iterator var = terminados.begin(); var != terminados.end(); ++var) {

        print_BCP_process(*var,inc,_CONT);
        gotoxy(160,inc);
        cout<<"T";
        inc++;
    }

}
void BCP(list<Proceso> &_listos, list<Proceso> &_bloqueados, list<Proceso> &_terminados, list<Proceso> &_suspendidos, Proceso &_actual, unsigned int & _CONT){

    unsigned int inc = 5;


    // imprime el enxabezado del BCP
    print_header_BCP();

    // procesos en cola de espera listos para ejecutarse
    BCP_listos(_listos,inc,_CONT);


    // Proceso en ejecucion
    BCP_Atual_process(_actual,inc, _CONT);


    // prcesos bloqueados
    BCP_bloquedos(_bloqueados,inc, _CONT);

    // terminados
    BCP_terminados(_terminados,inc, _CONT);

    // suspendidos
    BCP_suspendidos(_suspendidos,inc, _CONT);

    print_final_content(inc);

}


void gotoxy(int x,int y){
    HANDLE hcon;
    hcon = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD dwPos;
    dwPos.X = x;
    dwPos.Y= y;
    SetConsoleCursorPosition(hcon,dwPos);
}

void pintar_escenario(){
    imprimirNextProccessMsg();

    // gotoxy(columna,fila)
    gotoxy(2,1);
    cout<<"# PROCESOS NUEVOS ";

    gotoxy(2,5);
    cout<<"+";
    gotoxy(2,6);
    cout<<"|"<<setw(15)<<"LISTOS ";
    add_horizontal_line(5,3,25);
    gotoxy(26,5);
    cout<<"+";
    gotoxy(2,8);
    cout<<" ID";
    gotoxy(10,8);
    cout<<"|  TME";
    gotoxy(18,8);
    cout<<"|  TT";
    add_horizontal_line(7,3,25);
    add_horizontal_line(9,3,25);


    //vertical
    add_vertical_line(2,6,20);
    gotoxy(2,21);
    cout<<"+";
    add_horizontal_line(21,3,25);
    gotoxy(26,21);
    cout<<"+";
    add_vertical_line(26,6,20);


    ////////////////////ejecucion
    gotoxy(44,6);
    cout<<"|"<<setw(35)<<"PROCESO EN EJECUCION ";
    gotoxy(44,5);
    cout<<"+";
    add_horizontal_line(5,45,90);
    add_horizontal_line(7,45,90);

    add_vertical_line(44,7,14);
    gotoxy(44,15);
    cout<<"+";

    add_horizontal_line(15,45,90);
    gotoxy(91,15);
    cout<<"+";
    add_vertical_line(91,6,14);
    gotoxy(91,5);
    cout<<"+";


    gotoxy(45,9);
    cout<<"Id: ";

    gotoxy(45,10);
    cout<<"Operacion: ";

    gotoxy(45,11);
    cout<<"TME: ";

    gotoxy(45,12);
    cout<<"TT: ";

    gotoxy(45,13);
    cout<<"TR:";


    /////////////////////// bloquedos
    ///
    //44 original


    /*
     * gotoxy(2,27);
    cout<<"+";
    add_horizontal_line(27,45,72);
    gotoxy(44,28);
    cout<<"|"<<setw(22)<<"PROCESO BLOQUEADOS ";

    add_horizontal_line(29,45,72);
    gotoxy(73,27);
    cout<<"+";

    gotoxy(45,30);
    cout<<"Id";
    gotoxy(55,30);
    cout<<"| TT";
    gotoxy(65,30);
    cout<<"| TR";

    add_vertical_line(44,29,38);
    gotoxy(44,39);
    cout<<"+";
    add_horizontal_line(39,45,72);
    gotoxy(73,39);
    cout<<"+";

    add_vertical_line(73,28,38);
    add_horizontal_line(31,45,72);

     * */
    gotoxy(2,27);
    cout<<"+";
    add_horizontal_line(27,3,30);
    gotoxy(2,28);
    cout<<"|"<<setw(22)<<"PROCESO BLOQUEADOS ";

    add_horizontal_line(29,3,30);
    gotoxy(31,27);
    cout<<"+";

    gotoxy(3,30);
    cout<<"Id";
    gotoxy(13,30);
    cout<<"| TT";
    gotoxy(23,30);
    cout<<"| TR";

    add_vertical_line(2,29,42);
    gotoxy(2,43);
    cout<<"+";
    add_horizontal_line(43,3,30);
    gotoxy(31,43);
    cout<<"+";

    add_vertical_line(31,28,42);
    add_horizontal_line(31,3,30);
    //add_horizontal_line(45,3,30);




    //////////////////////// finalizacion

    add_horizontal_line(5,110,160);
    gotoxy(110,6);
    cout<<setw(35)<<"PROCESOS FINALIZADOS ";
    gotoxy(109,5);
    cout<<"+";
    add_horizontal_line(7,110,160);
    gotoxy(161,5);
    cout<<"+";

    gotoxy(110,8);
    cout<<"Id";

    gotoxy(118,8);
    cout<<"|"<<setw(20)<<"Operacion";

    gotoxy(145,8);
    cout<<"|"<<setw(15)<<"Resultado";

    gotoxy(110,9);
    add_horizontal_line(9,110,160);

    add_vertical_line(109,6,10);
    gotoxy(109,11);
    cout<<"+";

    // add_horizontal_line(11,110,160);
    gotoxy(161,11);
    cout<<"+";

    add_vertical_line(161,6,10);

    add_horizontal_line(11,110,160);

    gotoxy(11,25);
    cout<<"+ CONTADOR: ";
}


void limpiar_ejecucion(){
    gotoxy(61,9);
    cout<<setw(30)<<"  ";

    gotoxy(61,10);
    cout<<setw(30)<<"  ";

    gotoxy(61,11);
    cout<<"   ";

    gotoxy(61,12);
    cout<<setw(30)<<"   ";

    gotoxy(61,13);
    cout<<setw(30)<<"   ";

    gotoxy(61,14);
    cout<<setw(30)<<"   ";

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////// PAGINACIN SIMPLE ////////////////////////////////////////////////////

bool verifyMemorySpace(Marco* memoria, Proceso & _p){
    // variables
    int cociente = 0, cont= 0, residuo = 0;
    //obtiene la cantidad de marcos completos
    cociente = _p.getMemorySize() / 4;
    // verifica si se va a ocupar otro marco
    residuo = _p.getMemorySize() % 4;

    //cout << "Id: " << _p.getId() << endl;
    //cout << "Memory size: " << _p.getMemorySize() << endl;
    // cout << "Cociente: " << cociente << endl;
    //cout << "Residuo: " << residuo << endl;
    //cout << "-----------------------" << endl;

    // esta comprobacion es necesaria porque si hay
    // residuo significa que ocuprá otro marco

    // verifica si hay espacio libre en memoria, considerando que debe estra todo el marco dispoible
    for ( int i = 0; i < 45; i++) {
        if (memoria[i].isFreeSpace()){
            cont++;
        }
    }

    // añade el tamaño de marcos que necesita el proceso
    _p.setMarcosSize(cociente);
    _p.setRestoMarco(residuo);

    if ( residuo == 0 ){
        // si no tiene residuos
        if(cont >= cociente){
            return true;
        }
    }
    else{
        // si tiene almenos uno de residuo,
        // entonces necesitará otro marco obligatorio
        if(cont > cociente){
            return true;
        }
    }

    return false;
}


bool insertarProcesoMemoria(Marco *memoria, Proceso & _p){
    int cont = 0;
    bool bandera = false;

    if ( verifyMemorySpace(memoria, _p) ){
        // si hay espacio vacio en memoria

        for (int i = 0 ; i < 45; i++) {

            if ( memoria[i].isFreeSpace()){
                // si la memoria en la posicion i tiene espacio libre

                if ( cont == _p.getMarcosSize()){
                    // si ya se alcanzo los marcos enteros
                    bandera = true;

                }
                if ( bandera == true ){
                    // si ya se insertaron los ,arcos completos, es hora de insertar solo el resto
                    if ( _p.getRestoMarco()  <= 0 ){
                        return true;

                    }

                    return memoria[i].insertResto(_p.getRestoMarco(), _p ) ? true : false;

                }
                else{
                    // si aún no se han insertado todos los marcos necesarios
                    if ( memoria[i].insertarProceso(_p) ){
                        cont++;
                    }
                }
            }
        }

    }
    return false;
}

void cleanMemoryScreen(){
    int pos = 19, lim = 25;
    gotoxy(36,pos);
    for (int j = 0; j < lim; j++) {
        gotoxy(36,pos);
        cout << setw(70) <<" ";
        pos++;
    }


}

void updateMemoryScreen(Marco * memoria){
    cleanMemoryScreen();
    printMemoria(memoria);
}
void printMemoria(Marco * memoria){
    int i = 0, pos = 19, ref = 19;
    gotoxy(37,pos);
    cout << setw(8) << "Marco" << setw(10) << "Espacio" << setw(10) <<"Proceso";
    pos = pos + 2;

    for (; i < 23; i++) {
        if ( i < 3){
            gotoxy(37,pos);
            cout << setw(8) << i << setw(7) << memoria[i].qtyOcuppiedSpace() << " / 4" << setw(10) << "SO";
            pos++;
        }
        else if (memoria[i].id() == 99){
            gotoxy(37,pos);
            cout << setw(8) << i << setw(7) << memoria[i].qtyOcuppiedSpace() << " / 4" << setw(10) << "Libre" ;
            pos++;

        }
        else {
            gotoxy(37,pos);
            cout << setw(8) << i << setw(7) << memoria[i].qtyOcuppiedSpace() << " / 4" << setw(10) << memoria[i].id();
            pos++;

        }
    }
    pos = ref;

    add_vertical_line(75,pos,pos+24);

    gotoxy(77,pos);
    cout << setw(8) << "Marco" << setw(10) << "Espacio" << setw(10) <<"Proceso";

    pos = pos + 2;
    gotoxy(77, pos);
    for (; i < 45; i++) {
        if (memoria[i].id() == 99){
            gotoxy(77,pos);
            cout << setw(8) << i << setw(7) << memoria[i].qtyOcuppiedSpace() << " / 4" << setw(10) << "Libre";
            pos++;

        }
        else {
            gotoxy(77,pos);
            cout << setw(8) << i << setw(7) << memoria[i].qtyOcuppiedSpace() << " / 4" << setw(10) << memoria[i].id();
            pos++;

        }
    }
    add_horizontal_line(ref+1,37,106);

    //cout << "Presione la tecla 'c' para continuar" << endl;
}

void printMemoriaFrom(Marco* memoria, int  _pos){

    int i=0;
    gotoxy(1,_pos);
    cout << setw(8) << "Marco" << setw(20) << "Espacio" << setw(20) <<"Proceso" << endl << endl;
    for (; i < 25; i++) {
        if ( i < 3){
            cout << setw(8) << i << setw(15) << memoria[i].qtyOcuppiedSpace() << " / 4" << setw(18) << "SO" << endl;
        }
        else if (memoria[i].id() == 99){
            cout << setw(8) << i << setw(15) << memoria[i].qtyOcuppiedSpace() << " / 4" << setw(18) << "Libre" << endl;

        }
        else {
            cout << setw(8) << i << setw(15) << memoria[i].qtyOcuppiedSpace() << " / 4" << setw(18) << memoria[i].id() << endl;

        }
    }

    gotoxy(80,_pos);
    cout << setw(8) << "Marco" << setw(20) << "Espacio" << setw(20) <<"Proceso";

    _pos = _pos + 2;
    gotoxy(80, _pos);
    for (; i < 45; i++) {

        if (memoria[i].id() == 99){
            gotoxy(80,_pos);
            cout << setw(8) << i << setw(15) << memoria[i].qtyOcuppiedSpace() << " / 4" << setw(18) << "Libre";
            _pos++;

        }
        else {
            gotoxy(80,_pos);
            cout << setw(8) << i << setw(15) << memoria[i].qtyOcuppiedSpace() << " / 4" << setw(18) << memoria[i].id();
            _pos++;

        }
    }

    gotoxy(1,_pos+10);
    cout<<endl;

}

bool elimProcesoMemoria(Marco* memoria, Proceso & _p){

    for (int i = 0; i < 45; i++) {
        if ( memoria[i].id() == _p.getId()){
            // si la posicion corresponde con el id del proceso
            memoria[i].deleteData();
        }
    }
    return true;
}
//////////////////////////////// FIN PAGINACION SIMPLE   ///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
