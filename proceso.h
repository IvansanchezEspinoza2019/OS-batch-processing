#ifndef PROCESO_H
#define PROCESO_H
#include<iostream>


class Proceso
{
    std::string nombre;
    unsigned int id;
    char operacion;

    float num1;
    float num2;
    float res;

    bool success;
    bool was_execute;
    bool finished;
    bool suspended;

    unsigned int tt;
    unsigned int tme;
    unsigned int ttb;
    unsigned int arrivalTime;
    unsigned int responseTime;
    unsigned int finishedTime;
    unsigned int serviceTime;
    unsigned int waintingTotalTime;

    unsigned int memorySize;
    int marcosSize;
    int restoMarco;

public:
    Proceso();
    Proceso(const Proceso&);
    std::string getNombre() const;
    void setNombre(const std::string &value);
    unsigned int getId() const;
    void setId(unsigned int value);
    char getOperacion() const;
    void setOperacion(char value);
    unsigned int getTme() const;
    void setTme(unsigned int value);


    friend std::ostream& operator << (std::ostream& o,Proceso &a){
        o<<"| "<<a.getNombre()<<"ID: ";
        o<<a.getId()<<", Oper: ";
        o<<a.getOperacion()<<", TME: ";
        o<<a.getTme()<<", NUM 1: ";
        o<<a.getNum1()<<", NUM 2: ";
        o<<a.getNum2()<<", Memory: ";
        o<<a.getMemorySize()<<"|"<<std::endl;


        return o;

}
    float getNum1() const;
    void setNum1(const float &value);
    float getNum2() const;
    void setNum2(const float &value);
    float getRes() const;
    void setRes(const float &value);

    unsigned int getTt() const;
    void setTt(unsigned int value);
    unsigned int getTtb() const;
    void setTtb(unsigned int value);
    bool getWas_execute() const;
    void setWas_execute(bool value);
    unsigned int getArrivalTime() const;
    void setArrivalTime(unsigned int value);

    unsigned int getResponseTime() const;
    void setResponseTime(unsigned int value);
    unsigned int getFinishedTime() const;
    void setFinishedTime(unsigned int value);
    unsigned int getServiceTime() const;
    void setServiceTime(unsigned int value);
    bool getSuccess() const;
    void setSuccess(bool value);
    unsigned int getWaintingTotalTime() const;
    void setWaintingTotalTime(unsigned int value);
    bool getFinished() const;
    void setFinished(bool value);
    unsigned int getMemorySize() const;
    void setMemorySize(unsigned int value);
    int getMarcosSize() const;
    void setMarcosSize(int value);
    int getRestoMarco() const;
    void setRestoMarco(int value);
    void eraseData();


    bool getSuspended() const;
    void setSuspended(bool value);
};

#endif // PROCESO_H
