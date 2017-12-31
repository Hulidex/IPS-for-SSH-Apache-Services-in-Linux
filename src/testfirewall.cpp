#include <iostream>
#include <firewall.hpp>


int main (const int argc, const char *argv[])
{
    firewall fire;


    fire.add("192.168.1.52",fecha("31/Dec/2017:17:03:59","http"));
    fire.add("192.168.1.52",fecha("31/Dec/2017:17:03:58","http"));
    fire.add("192.168.1.52",fecha("31/Dec/2017:17:03:57","http"));
    fire.add("192.168.1.52",fecha("31/Dec/2017:17:03:56","http"));
    fire.add("192.168.1.52",fecha("31/Dec/2017:17:03:56","http"));

    fire.add("192.168.1.78",fecha("31/Dec/2017:17:03:59","http"));
    fire.add("192.168.1.78",fecha("31/Dec/2017:17:03:58","http"));
    fire.add("192.168.1.78",fecha("31/Dec/2017:17:03:57","http"));
    /*fire.add("192.168.1.52",fecha("31/Sep/2017:17:03:59","http"));
    fire.add("192.168.1.52",fecha("31/Sep/2017:17:03:59","http"));
    fire.add("192.168.1.52",fecha("31/Sep/2017:17:03:59","http"));
    fire.add("192.168.1.52",fecha("31/Sep/2017:17:03:59","http"));
    fire.add("192.168.1.52",fecha("31/Sep/2017:17:03:59","http"));
    fire.add("192.168.1.56",fecha("31/Sep/2017:17:03:59","http"));
    fire.add("192.168.1.56",fecha("31/Oct/2017:17:03:59","http"));
    fire.add("192.168.1.2",fecha("31/Nov/2017:17:03:59","http")); 
    fire.add("192.168.1.43",fecha("Feb 28 17:03:59", "ssh"));
    fire.add("192.168.1.43",fecha("Feb 28 17:04:59", "ssh"));
    fire.add("192.168.1.43",fecha("Feb 28 17:05:59", "ssh"));
    fire.add("192.168.1.43",fecha("Feb 28 17:06:59", "ssh"));
    fire.add("192.168.1.43",fecha("Feb 28 17:06:59", "ssh"));
    fire.add("192.168.1.43",fecha("Dec 28 17:06:59", "ssh"));
    fire.add("192.168.1.43",fecha("Dec 28 17:06:59", "ssh"));
    fire.add("192.168.1.43",fecha("Dec 28 17:06:59", "ssh"));*/
    fire.set_intervalo(7200);//en 2 horas
    fire.set_maxpeticiones(2);//si se encuentran mas de 3 peticiones
    fire.set_tiempoBaneado(15);//se banea 15 segundos




    
    
    
    std::cout << fire << std::endl;

    fire.run();

    
    return 0;
}