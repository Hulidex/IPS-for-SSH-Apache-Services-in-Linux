
%{
//bibliotecas
#include <iostream>
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
fechassh    {mes}" "{numero}" "{hora}
fechahttp   {numero}"/"{mes}"/"{numero}":"{hora}
sesionssh   {fechassh}{texto}"Failed password"{texto}"from "{ip}



%%

{sesionssh}     {match = std::string(yytext);
                 matches.push_back(match);
                 fech = fecha(match.substr(0,14), "ssh");
                 ip = match.substr(match.find("from")+5, match.size());
                 fire.add(ip, fech);

                 }
%%




int main (int argc, const char *argv[])
{
    if (argc  == 2){
        yyin = fopen(argv[1], "r"); //Comunicamos a Lex que el fichero a leer tiene nombre argv[1] y lo abrimos en modo lectura exclusivamente.
        if (yyin == NULL){
            fprintf(stderr, "Error al abrir el fichero: %s", argv[1]);
            exit(EXIT_FAILURE);
        }
    }else yyin = stdin;

    std::vector<std::string>::const_iterator it;

    yylex();

    for (it = matches.begin(); it!= matches.end(); ++it)
       std::cout << *it << std::endl;

    std::cout << matches.size() << std::endl << fire << std::endl;

    return 0;
}