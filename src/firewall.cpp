#include <firewall.hpp>


 firewall *firewall::instance;


void firewall::banear_ip(const std::string &ip)
{
    ban baneaso;
    std::string regla;


    if (mod == ssh){
        regla += "-A INPUT -p tcp --dport ";
        regla += std::to_string(sshPort); 
        regla += " -s "; 
        regla += ip;
        regla += " -j ";
        regla += accion;
    }
    else if (mod == http){
        regla += "-A INPUT -p tcp --dport ";
        regla += std::to_string(httpPort); 
        regla += " -s "; 
        regla += ip;
        regla += " -j ";
        regla += accion;
    }
    
    baneaso.ip = ip;
    baneaso.t_inicio = time(0);
    baneaso.regla = regla.substr(2,regla.size()-1);
    
    listaBaneados.push_back(baneaso);
    regla.insert(0,"sudo iptables ");
    system (regla.c_str());
    

}

void firewall::comprobarSiEliminarBaneo()
{
    std::vector<ban>::iterator it = listaBaneados.begin();
    std::string regla;

    while(it != listaBaneados.end()){
        if ((time(0) - it->t_inicio) > tiempo_baneado){

            regla += it->regla;
            regla.insert(0,"sudo iptables -D ");
            system(regla.c_str());
            listaBaneados.erase(it);
            regla.clear();
            
            it = listaBaneados.begin();
        }else ++it;
    }

}

void firewall::comprobarSiBanear()
{
    time_t now = time(0);
    fecha hoy;
    std::map<std::string,std::vector<fecha> >::iterator it;
    std::vector<fecha>::iterator vit;
    unsigned peticiones;
    bool baneada;


    hoy = fecha(std::string(ctime(&now)).substr(4,15), "ssh");//obtenemos la fecha actual


    for (it = sesiones.begin(); it != sesiones.end(); ++it){
        peticiones = 0;
        baneada = false;
        if (it->second.size() > max_peticiones){
            for(vit = it->second.begin(); (vit != it->second.end()) && (!baneada); ++vit){
                if ((hoy-*vit) <= intervalo)
                    ++peticiones;
                if (peticiones > max_peticiones){
                    banear_ip(it->first);
                    baneada = true;
                }
            }

            if (baneada)
                it->second.clear();
        }
    }
}

void firewall::manejadorSenial(int signum)
{
    std::cout << "\nSeñal " << signum << " recibida" << std::endl;


    if (firewall::instance->sesiones.size()>0)
        firewall::instance->clear();


    exit(0);
}

firewall::firewall()
{
    sesiones = std::map<std::string, std::vector<fecha> >();
    listaBaneados = std::vector<ban>();
    mod = ssh;
    accion = "DROP";
    intervalo =172801; //2 días y un segundo
    max_peticiones = 1000; 
    tiempo_baneado = 300; //5 minutos
    httpPort = 80;
    sshPort = 22;

    firewall::instance = this;
}


firewall::firewall(const std::string &ip, const std::vector<fecha> &fechas)
{
    sesiones.insert(std::pair<std::string,std::vector<fecha> >(ip, fechas));
    listaBaneados = std::vector<ban>();
    mod = ssh;
    accion = "DROP";
    intervalo =172801; //2 días y un segundo
    max_peticiones = 1000; 
    tiempo_baneado = 300; //5 minutos
    httpPort = 80;
    sshPort = 22;

    firewall::instance = this;
}


void firewall::clear()
{
    std::map<std::string, std::vector<fecha> >::iterator it;

    for (it = sesiones.begin(); it != sesiones.end(); ++it)
        it->second.clear();

    sesiones.clear();
    listaBaneados.clear();
    accion.clear();
}

firewall::~firewall()
{
    clear();
}

void firewall::add(const std::string &ip, const fecha &fech)
{
    std::map<std::string, std::vector<fecha> >::iterator it;


    it = sesiones.find(ip);
    if (it != sesiones.end()){
        it->second.push_back(fech);
        std::sort(it->second.begin(), it->second.end());
    }else{
        sesiones.insert(std::pair<std::string, std::vector<fecha> >(ip, std::vector<fecha>(1,fech)));
    }
}

void firewall::run()
{
    signal(SIGINT, firewall::manejadorSenial);

    while(true)
    {
        comprobarSiBanear();
        comprobarSiEliminarBaneo();
    }
}


void firewall::set_intervalo(unsigned segundos)
{
    if (segundos > 172801)
        this->intervalo = 172801;
    else this->intervalo = segundos;
}

void firewall::set_maxpeticiones(unsigned peticiones)
{
    this->max_peticiones = peticiones;
}

void firewall::set_tiempoBaneado(unsigned segundos)
{
    this->tiempo_baneado = segundos;
}

void firewall::set_sshPort(unsigned puerto)
{
    this->sshPort = puerto;
}

void firewall::set_httpPort(unsigned puerto)
{
    this->httpPort = puerto;
}

void firewall::set_mod(modoOperacion modo)
{
    this->mod = modo;
}

void firewall::set_accion(const std::string &accion)
{
    if (accion == "REJECT")
        this->accion = accion;
    else this->accion = "DROP";
}

unsigned firewall::get_intervalo()
{
    return this->intervalo;
}


unsigned firewall::get_maxpeticiones()
{
    return this->max_peticiones;
}

unsigned firewall::get_tiempoBaneado()
{
    return this->tiempo_baneado;
}

unsigned firewall::get_sshPort()
{
    return this->sshPort;
}

unsigned firewall::get_httpPort()
{
    return this->httpPort;
}

modoOperacion firewall::get_mod()
{
    return this->mod;
}

std::string firewall::get_accion()
{
    return this->accion;
}

std::ostream &operator<<(std::ostream &out, const firewall &ses)
{
    std::map<std::string, std::vector<fecha> >::const_iterator it;
    std::vector<fecha>::const_iterator vit;


    for (it = ses.sesiones.begin(); it != ses.sesiones.end(); ++it){
        out << it->first << ":\n";
        for (vit = it->second.begin(); vit != it->second.end(); ++vit){
            out << "\t" << *vit << "\n";
        }
    }

    if (ses.mod == ssh){
        out << "Modo operacion: ssh\n" << "Puerto ssh: " << ses.sshPort << std::endl;
    }
    else if (ses.mod == http){
        out << "Modo opeacion: http\n" << "Puerto http: " << ses.httpPort << std::endl;
    }

    out << "Intervalo (seg): " << ses.intervalo << std::endl;
    out << "Numero maximo de Peticiones: " << ses.max_peticiones << std::endl;
    out << "Tiempo duracion del baneo(seg): " << ses.tiempo_baneado << std::endl;




    return out;
}