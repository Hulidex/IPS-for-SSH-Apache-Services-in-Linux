#ifndef _FIREWALL_HPP_
#define _FIREWALL_HPP_


#include <string> 
#include <vector>
#include <algorithm>
#include <map>
#include <iostream>
#include <ctime>
#include <csignal>
#include <fecha.hpp>


struct ban
{
    std::string ip;
    time_t t_inicio;
    std::string regla;
};


class firewall{
    private:
        static firewall *instance;
        std::map<std::string,std::vector<fecha> > sesiones;
        std::vector<ban> listaBaneados;
        std::string accion;
        unsigned intervalo;
        unsigned max_peticiones;
        unsigned tiempo_baneado;
        unsigned Port;
        static bool parado;

        
        void banear_ip(const std::string &str);
        void comprobarSiBanear();
        void comprobarSiEliminarBaneo();
        static void manejadorSenial(int signum);

    public:
        firewall();
        firewall(const std::string &ip, const std::vector<fecha> &fechas);
        ~firewall();
        void add(const std::string &ip, const fecha &fech);
        void run();
        void clear();
        

        //OPERACIONES DE MODIFICACION
        void set_intervalo(unsigned segundos);
        void set_maxpeticiones(unsigned peticiones);
        void set_tiempoBaneado(unsigned segundos);
        void setPort(unsigned puerto);
        void set_accion(const std::string &accion);


        //OPERACIONES DE CONSULTA
        unsigned get_intervalo();
        unsigned get_maxpeticiones();
        unsigned get_tiempoBaneado();
        unsigned getPort();
        std::string get_accion();



    friend std::ostream &operator<<(std::ostream &out, const firewall &ses);
};

std::ostream &operator<<(std::ostream &out, const firewall &ses);


#endif