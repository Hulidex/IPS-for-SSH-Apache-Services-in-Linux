#ifndef _FECHA_HPP_
#define _FECHA_HPP_

#include <iostream>
#include <string>
#include <cmath>
#include <time.h>


class fecha{
    private:
        unsigned dia;
        unsigned mes;
        unsigned anio;
        unsigned hora;
        unsigned minuto;
        unsigned segundo;

    
    public:
        fecha();
        fecha(std::string str, const std::string &formato);
        fecha (const fecha &fech);
        
        bool operator<(const fecha &fech ) const;
        fecha &operator=(const fecha &fech);
        unsigned long operator-(const fecha &fech) const;

    friend std::ostream &operator<<(std::ostream &out, const fecha & fech);


};

std::ostream &operator<<(std::ostream &out, const fecha & fech);
unsigned to_number(const std::string &str);

#endif