#include <iostream>
#include <fecha.hpp>
#include <unistd.h>
#include <ctime>
extern "C" unsigned int sleep(unsigned int seconds);


int main(const int argc, const char *argv[])
{
    fecha fech1("02/Jan/2018:21:03:00","http");
    fecha fech2("Jan  3 12:55:59", "ssh");
    fecha fech3(fech1);
    

    std::cout << "Fecha1: " << fech1 << "\nFecha2: " << fech2 << std::endl;
    if (fech2 < fech1)
        std::cout << "Fecha2 es menor que Fecha1" << std::endl;   
    else std::cout << "Fecha2 no es menor que Fecha1" << std::endl;

    fech1 = fech2;

    std::cout << "Fech1 es igual a Fech2: " << fech1 << std::endl << "Fech3 tiene el anterior Fech1: " << fech3 << std::endl;

    time_t now = time(0);
    fecha hoy;

    hoy = fecha(std::string(ctime(&now)).substr(4,15), "ssh");

    std::cout << "Hoy bueno: " <<  ctime(&now) << std::endl;


    //sleep(5);
    time_t notnow = time(0);

    std::cout << "Hoy: " << hoy << std::endl;

    std::cout << hoy - fech2 << std::endl;
    std::cout << "xd " << notnow - now << std::endl;


    return 0;
}