
%{
//bibliotecas
#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <firewall.hpp>
extern "C" int yylex();


//variables
firewall fire;
std::vector<std::string> matches;
fecha fech;
std::string ip, match;

%}

mes         Jan|Feb|Mar|Apr|May|Jun|Jul|Aug|Sep|Oct|Nov|Dec
numero      [0-9]+
texto       [a-zA-Z" :_;=/"\[\]\-\(\)0-9]*
hora        {numero}":"{numero}":"{numero}
ip          {numero}"."{numero}"."{numero}"."{numero}
fechassh    {mes}" "+{numero}" "{hora}
fechahttp   {numero}"/"{mes}"/"{numero}":"{hora}
sesionssh   {fechassh}{texto}"Failed password"{texto}"from "{ip}



%%

{sesionssh}     {match = std::string(yytext);
                 matches.push_back(match);
                 fech = fecha(match.substr(0,14), "ssh");
                 ip = match.substr(match.find("from")+5, match.size());
                 fire.add(ip, fech);

                 }
.               {}
%%

int MostrarInterfaz()
{
    int entrada;
    std::string str;
    int respuesta = 0;

    do{
        std::cout << "------------------------------------------------------------------------------------------" << std::endl;
        std::cout << "\t0.Establecer el modo de operación.[";
        if (fire.get_mod() == ssh)
            std::cout << "ssh]" << std::endl;
        else std::cout << "http]" << std::endl;

        std::cout << "\t1.Indicar otro fichero con los logs." << std::endl;
        std::cout << "\t2.Establecer puerto del protocolo analizado.[" << fire.getPort() << "]" << std::endl;
        std::cout << "\t3.Ajustar el intervalo de análisis.[" << fire.get_intervalo() << " seg]" << std::endl;
        std::cout << "\t4.Establecer el máximo número de peticiones en dicho intervalo.[" << fire.get_maxpeticiones() << "]" << std::endl;
        std::cout << "\t5.Establecer opción al banear.[" << fire.get_accion() << "]" << std::endl;
        std::cout << "\t6.Establecer el tiempo que una IP está baneada.[" << fire.get_tiempoBaneado() <<" seg]" << std::endl;
        std::cout << "\t7.Encender el firewall." << std::endl;
        std::cout << "\t8.Ayuda." << std::endl;
        std::cout << "\t9.Salir." << std::endl;
        std::cout << "------------------------------------------------------------------------------------------" << std::endl;
        std::cout << "Opción a introducir: ";

        
        std::getline(std::cin,str);
        entrada = std::stoi(str);
        str.clear();
            
        if (entrada < 0 || entrada > 9)
            std::cout << "Opción no valida." << std::endl;
            
    }while(entrada < 0 || entrada > 9);
    
    system("clear");

    switch (entrada){
        case 0:
            do{
                str.clear();
                std::cout << "Introduzca el modo de operación[ssh|http]: ";
                std::getline(std::cin, str);
            }while(str.compare("ssh") != 0 && str.compare("http") != 0);
            
            if (str.compare("ssh") == 0)
                fire.set_mod(ssh);
            else fire.set_mod(http);

            break;
        case 1:
            std::cout << "Introduzca la ruta donde se encuentra el fichero con los logs: ";
            std::getline(std::cin, str);
            
            yyin = fopen(str.c_str(), "r"); //Comunicamos a Lex que el fichero a leer tiene nombre argv[1] y lo abrimos en modo lectura exclusivamente.
            
            if (yyin == NULL){
                std::cerr << "Error al abrir el fichero: " << str << std::endl;
                exit(EXIT_FAILURE);
            }

            yyout = fopen("/dev/null", "w");
            if (yyout == NULL){
                std::cerr << "Error al abrir el fichero: /dev/null" << std::endl;
                exit(EXIT_FAILURE);
            }
            
            
            yylex();

            std::cout << fire << std::endl;

            break;
        case 2:
            std::cout << "Introduzca el numero del puerto del protocolo: ";
            std::getline(std::cin,str);
            entrada = std::stoi(str);
            str.clear();
            
            while(entrada <= 0){
                std::cout << "El puerto es un número natural positivo. Reintroduzca su valor: ";
                std::getline(std::cin,str);
                entrada = std::stoi(str);
                str.clear();
            }

            fire.setPort(entrada);
            break;
        case 3:
            std::cout << "Introduzca el intervalo de tiempo (seg) en el que se contabilizarán las peticiones: ";
            std::getline(std::cin,str);
            entrada = std::stoi(str);
            str.clear();
            
            while (entrada <= 0){
                std::cout << "El intervalo debe ser un numero natural positivo mayor que 0. Reintroduzca su valor: ";
                std::getline(std::cin,str);
                entrada = std::stoi(str);
                str.clear();
            }

            fire.set_intervalo(entrada);
            break;
        case 4:
            std::cout << "Introduce el número de peticiones que se contarán en el intervalo antes de banear una ip: ";
            std::getline(std::cin,str);
            entrada = std::stoi(str);
            str.clear();
            
            while (entrada <= 0){
                std::cout << "El numero de peticiones debe ser un numero natural positivo mayor que 0. Reintroduza su valor: ";
                std::getline(std::cin,str);
                entrada = std::stoi(str);
                str.clear();
            }

            fire.set_maxpeticiones(entrada);
            break;
        case 5:
            std::cout << "Introduzca el comportamiento el modo de comportamiento que tendrá el firewall al banear una ip.\n\"DROP\" para que tras el"
            "baneo, si la ip baneada sigue solicitando el servicio, el firewall no responda a sus peticiones con ningún mensaje.\nO \"REJECT\" si "
            "se quiere que la ip baneada, al realizar peticiones, el firewall responda con un mensaje de rechazo: ";
            std::getline(std::cin, str);

            while (str.compare("DROP") != 0 && str.compare("REJECT") != 0){
                str.clear();
                std::cout << "Opción no válida, introduzca una opción correcta[DROP|REJECT]: ";
                std::getline(std::cin, str);
            }

            fire.set_accion(str);
            str.clear();
            break;
        case 6:
            std::cout << "Introduzca el tiempo que una ip estará baneada: ";
            std::getline(std::cin,str);
            entrada = std::stoi(str);
            str.clear();
            
            while (entrada < 0){
                std::cout << "El valor introducido debe ser un número natural positivo. Reintroduzca el valor: ";
                std::getline(std::cin,str);
                entrada = std::stoi(str);
                str.clear();
            }

            fire.set_tiempoBaneado(entrada);
            break;
        case 7:
            system("clear");
            std::cout << "Pulsar Ctrl+C para detener el firewall." << std::endl;
            fire.run();
            respuesta = -2;
            break;
        case 8:
            std::cout << "Este programa está realizado por hulidex.\nEs una especie de firewall muy rudimentario que "
             "tiene como propósito proteger de ataques DOS (Denial-of-service attack) básicos a ciertos protocolos (ssh y http).\n"
            "Es imprescindible que antes de ajustar ningún parámetro en el firewall, se indique la ruta del fichero que contiene"
             "los logs del protocolo que se quiere proteger (Opción 1). El firewall por defecto tiene la siguiente configuración:\n"
            " Trabaja suponiendo que el protocolo a proteger es el ssh, que dicho protocolo está escuchando sobre el puerto 22 y si" 
             " capta peticiones de intento de sesión fallidos en dicho protocolo, tiene en cuenta aquellos intentos que se produjeron "
             " desde hace 172801 segundos (2 días), hasta el momento actual.\nSi en ese intervalo se han hecho más de 1000 peticiones, "
             " entonces banea la ip, aplicando la opción \"DROP\", es decir si dicha ip realiza más peticiones de acceso, el firewall"
             " del sistema operativo no le responde con ningún mensaje.\nEl tiempo que una ip está baneada es de 300 segundos (5 minutos)." 
             << std::endl;
             std::cout << "\nPulsa intro para continuar: ";
             std::getline(std::cin,str);
             str.clear();
            break;
        case 9:
            std::cout << "Adiós." << std::endl;
            respuesta = -1;
            break;
    }


    return respuesta;
}


int main (int argc, const char *argv[])
{
    std::string str;
    int salida;

    if (argc == 2){
        yyin = fopen(argv[1], "r");
        if (yyin == NULL){
            std::cerr << "Error al abrir el fichero: " << argv[1] << std::endl;
            exit(EXIT_FAILURE); 
        }
    }else{
        std::cout << "Introduzca la ruta donde se encuentra el fichero con los logs: ";
        std::getline(std::cin, str);

        yyin = fopen(str.c_str(), "r"); //Comunicamos a Lex que el fichero a leer tiene nombre argv[1] y lo abrimos en modo lectura exclusivamente.
        str.clear();

        if (yyin == NULL){
            std::cerr << "Error al abrir el fichero: " << str << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    yyout = fopen("/dev/null", "w");
    if (yyout == NULL){
        std::cerr << "Error al abrir el fichero: /dev/null" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    
    yylex();

    std::cout << fire << std::endl;

    salida = MostrarInterfaz();
    while (salida != -1){
        if (salida == -2){
            std::cout << "¿Quieres volver a analizar el fichero con los logs, pero con otras opciones?[Si/No]: " << std::endl;
            std::getline(std::cin, str);
            if(str.compare("Si") != 0 && str.compare("si") != 0){
                salida = -1;
                std::cout << "Adiós." << std::endl;            
            }else{
                yylex();
                salida = MostrarInterfaz();
            }

        }else salida = MostrarInterfaz();
    }


    return 0;
}
