#include "proceso.h"

std::string Proceso::getNombre() const
{
    return nombre;
}

void Proceso::setNombre(const std::string &value)
{
    nombre = value;
}

unsigned int Proceso::getId() const
{
    return id;
}

void Proceso::setId(unsigned int value)
{
    id = value;
}

char Proceso::getOperacion() const
{
    return operacion;
}

void Proceso::setOperacion(char value)
{
    operacion = value;
}

unsigned int Proceso::getTme() const
{
    return tme;
}

void Proceso::setTme(unsigned int value)
{
    tme = value;
}

float Proceso::getNum1() const
{
    return num1;
}

void Proceso::setNum1(const float &value)
{
    num1 = value;
}

float Proceso::getNum2() const
{
    return num2;
}

void Proceso::setNum2(const float  &value)
{
    num2 = value;
}

float Proceso::getRes() const
{
    return res;
}

void Proceso::setRes(const float &value)
{
    res = value;
}

unsigned int Proceso::getTt() const
{
    return tt;
}

void Proceso::setTt(unsigned int value)
{
    tt = value;
}

unsigned int Proceso::getTtb() const
{
    return ttb;
}

void Proceso::setTtb(unsigned int value)
{
    ttb = value;
}

bool Proceso::getWas_execute() const
{
    return was_execute;
}

void Proceso::setWas_execute(bool value)
{
    was_execute = value;
}

unsigned int Proceso::getArrivalTime() const
{
    return arrivalTime;
}

void Proceso::setArrivalTime(unsigned int value)
{
    arrivalTime = value;
}

unsigned int Proceso::getResponseTime() const
{
    return responseTime;
}

void Proceso::setResponseTime(unsigned int value)
{
    responseTime = value;
}

unsigned int Proceso::getFinishedTime() const
{
    return finishedTime;
}

void Proceso::setFinishedTime(unsigned int value)
{
    finishedTime = value;
}

unsigned int Proceso::getServiceTime() const
{
    return serviceTime;
}

void Proceso::setServiceTime(unsigned int value)
{
    serviceTime = value;
}

bool Proceso::getSuccess() const
{
    return success;
}

void Proceso::setSuccess(bool value)
{
    success = value;
}

unsigned int Proceso::getWaintingTotalTime() const
{
    return waintingTotalTime;
}

void Proceso::setWaintingTotalTime(unsigned int value)
{
    waintingTotalTime = value;
}

bool Proceso::getFinished() const
{
    return finished;
}

void Proceso::setFinished(bool value)
{
    finished = value;
}

unsigned int Proceso::getMemorySize() const
{
    return memorySize;
}

void Proceso::setMemorySize(unsigned int value)
{
    memorySize = value;
}

 int Proceso::getMarcosSize() const
{
    return marcosSize;
}

void Proceso::setMarcosSize(int value)
{
    marcosSize = value;
}

int Proceso::getRestoMarco() const
{
    return restoMarco;
}

void Proceso::setRestoMarco(int value)
{
    restoMarco = value;
}

void Proceso::eraseData()
{
    //reinicia todos los valores de los atributos
    nombre="";
    id=0;
    tme=0;
    operacion='N';
    res = 0.0;
    num1=0.0;
    num2=0.0;
    tt = 0;
    ttb = 0;
    was_execute = false;
    arrivalTime = 0;
    finishedTime = 0;
    responseTime = 0;
    serviceTime = 0;
    success = false;
    waintingTotalTime = 0;
    finished = false;
    memorySize = 0;
    marcosSize = 0;
    restoMarco = 0;
    suspended = false;
}



bool Proceso::getSuspended() const
{
    return suspended;
}

void Proceso::setSuspended(bool value)
{
    suspended = value;
}

Proceso::Proceso()
{
    nombre="";
    id=0;
    tme=0;
    operacion='N';
    res = 0.0;
    num1=0.0;
    num2=0.0;
    tt = 0;
    ttb = 0;
    was_execute = false;
    arrivalTime = 0;
    finishedTime = 0;
    responseTime = 0;
    serviceTime = 0;
    success = false;
    waintingTotalTime = 0;
    finished = false;
    memorySize = 0;
    marcosSize = 0;
    restoMarco = 0;
    suspended = false;

}

Proceso::Proceso(const Proceso &_p)
{
    nombre = _p.getNombre();
    id = _p.getId();
    tme = _p.getTme();
    operacion = _p.getOperacion();
    res = _p.getRes();
    num1 = _p.getNum1();
    num2 = _p.getNum2();
    tt = _p.getTt();
    ttb = _p.getTtb();
    was_execute = _p.getWas_execute();
    arrivalTime = _p.getArrivalTime();
    finishedTime = _p.getFinishedTime();
    responseTime = _p.getResponseTime();
    serviceTime = _p.getServiceTime();
    success  = _p.getSuccess();
    waintingTotalTime = _p.getWaintingTotalTime();
    finished = _p.getFinished();
    memorySize = _p.getMemorySize();
    marcosSize = _p.getMarcosSize();
    restoMarco = _p.getRestoMarco();
    suspended = _p.getSuspended();
}
