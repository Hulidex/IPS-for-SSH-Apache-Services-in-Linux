#include <fecha.hpp>

unsigned to_number(const std::string &str)
{
    unsigned num = 0;
    unsigned size = str.size();

    for (unsigned i = 0; i < size; ++i){
        num += (str[i] - '0')*pow(10,size-(i+1));
    }


    return num;
}

fecha::fecha(){
    dia = mes = anio = hora = minuto = segundo = 0;
}

fecha::fecha(std::string str, const std::string &formato)
{
    std::string aux;
    unsigned pos;


    if (formato.compare("http") == 0){
        for (int i = 0; i < 6; ++i){
            switch (i){
                case 0://Extraemos dia
                    pos = str.find('/');
                    aux = str.substr(0, pos);
                    dia = to_number(aux);
                    str.erase(0, pos+1);
                    break;
                case 1://Extraemos mes
                    pos = str.find('/');
                    aux = str.substr(0, pos);
                    if (aux.compare("Jan") == 0)
                        mes = 1;
                    else if (aux.compare("Feb") == 0)
                        mes = 2;
                    else if (aux.compare("Mar") == 0)
                        mes = 3;
                    else if (aux.compare("Apr") == 0)
                        mes = 4;
                    else if (aux.compare("May") == 0)
                        mes = 5;
                    else if (aux.compare("Jun") == 0)
                        mes = 6;
                    else if (aux.compare("Jul") == 0)
                        mes = 7;
                    else if (aux.compare("Aug") == 0)
                        mes = 8;
                    else if (aux.compare("Sep") == 0)
                        mes = 9;
                    else if (aux.compare("Oct") == 0)
                        mes = 10;
                    else if (aux.compare("Nov") == 0)
                        mes = 11;
                    else if (aux.compare("Dec") == 0)
                        mes = 12;
                    else mes = 0;
                    str.erase(0, pos+1);                                                                                 
                    break;
                case 2://Extraemos Anio
                    pos = str.find(':');
                    aux = str.substr(0, pos);
                    anio = to_number(aux);
                    str.erase(0, pos+1);
                    break;
                case 3://Extraemos hora
                    pos = str.find(':');
                    aux = str.substr(0, pos);
                    hora = to_number(aux);
                    str.erase(0, pos+1);
                    break;
                case 4://Extraemos minuto
                    pos = str.find(':');
                    aux = str.substr(0, pos);
                    minuto = to_number(aux);
                    str.erase(0, pos+1);
                    break;
                case 5://Extraemos segundo
                    aux = str.substr(0, str.size());
                    segundo = to_number(aux);
                    break;
            }
        }
    }else if (formato.compare("ssh") == 0){
        for(int i = 0; i < 5; ++i){
            switch (i){
                case 0://Extraemos mes
                    pos = str.find(' ');
                    aux = str.substr(0, pos);
                    if (aux.compare("Jan") == 0)
                        mes = 1;
                    else if (aux.compare("Feb") == 0)
                        mes = 2;
                    else if (aux.compare("Mar") == 0)
                        mes = 3;
                    else if (aux.compare("Apr") == 0)
                        mes = 4;
                    else if (aux.compare("May") == 0)
                        mes = 5;
                    else if (aux.compare("Jun") == 0)
                        mes = 6;
                    else if (aux.compare("Jul") == 0)
                        mes = 7;
                    else if (aux.compare("Aug") == 0)
                        mes = 8;
                    else if (aux.compare("Sep") == 0)
                        mes = 9;
                    else if (aux.compare("Oct") == 0)
                        mes = 10;
                    else if (aux.compare("Nov") == 0)
                        mes = 11;
                    else if (aux.compare("Dec") == 0)
                        mes = 12;
                    else mes = 0;
                    str.erase(0, pos+1);                                                                                 
                    break;
                case 1://Extraemos dia
                    pos = str.find(' ');
                    aux = str.substr(0, pos);
                    dia = to_number(aux);
                    str.erase(0, pos+1);
                    break;
                case 2://Extraemos hora
                    pos = str.find(':');
                    aux = str.substr(0, pos);
                    hora = to_number(aux);
                    str.erase(0, pos+1);
                    break;
                case 3://Extraemos minuto
                    pos = str.find(':');
                    aux = str.substr(0, pos);
                    minuto = to_number(aux);
                    str.erase(0, pos+1);
                    break;
                case 4://Extraemos segundo
                    aux = str.substr(0, str.size());
                    segundo = to_number(aux);
                    break;
            }
        }
        time_t Atime = time(NULL);
        anio = localtime(&Atime)->tm_year + 1900; //Como desconcemos el año ponemos el actual
    }else {
        std::cerr << "Formato no compatible, el objeto no se construlló correctamente" << std::endl;
    }
}


fecha::fecha (const fecha &fech)
{
    *this = fech; 
}

fecha &fecha::operator=(const fecha &fech)
{
    if (this != &fech){
        this->dia = fech.dia;
        this->minuto = fech.minuto;
        this->segundo = fech.segundo;
        this->hora = fech.hora;
        this->mes = fech.mes;
        this->anio = fech.anio;
    }

    return *this;
}



bool fecha::operator<(const fecha &fech )const
{
    bool menor = false;


    if (this->anio < fech.anio)
        menor = true;
    else if (this->anio == fech.anio){
        if (this->mes < fech.mes)
            menor = true;
        else if (this->mes == fech.mes){
            if (this->dia < fech.dia)
                menor = true;
            else if (this->dia == fech.dia){
                if (this->hora < fech.hora)
                    menor = true;
                else if (this->hora == fech.hora){
                    if (this->minuto < fech.minuto)
                        menor = true;
                    else if (this->minuto == fech.minuto){
                        if (this->segundo < fech.segundo)
                            menor = true;  
                    }
                }
            }
        } 
    }



    return menor;
}

unsigned long fecha::operator-(const fecha &fech) const
{
    unsigned  long seg1, seg2;

    seg1 = anio*365*24*60*60 + mes*31*24*60*60 + dia*24*60*60 + hora*60*60 + minuto*60 + segundo;
    seg2 = fech.anio*365*24*60*60 + fech.mes*31*24*60*60 + fech.dia*24*60*60 + fech.hora*60*60 + fech.minuto*60 + fech.segundo;


    return seg1 - seg2;
}


std::ostream &operator<<(std::ostream &out, const fecha & fech)
{
    if(fech.dia < 10)
        out << "0";
    
    out << fech.dia << "/";
    
    if(fech.mes < 10)
        out << "0";
    out << fech.mes << "/" << fech.anio;

    if (fech.hora < 10)
        out << "0";
    out << " " << fech.hora << ":";

    if (fech.minuto < 10)
        out << "0";
    out << fech.minuto << ":";

    if (fech.segundo < 10)
        out << "0";
    out << fech.segundo;
    
    
    return out;
}