
#Variables
CXX=g++
FLAGS=-Wall --std=c++11 -Ofast --debug #-lfl
FXX=flex -o
LEXFILE=src/analizador.l
OBJS=$(wildcard obj/*)#Objetos
SRCS=$(wildcard src/*)#Archivos fuente
INCLUDES=$(wildcard include/*)
BINS=$(wildcard bin/*)#Archivos binarios


binaries: bin/testfecha bin/testfirewall bin/testanalizador#Genera todos los ejecutables

#EJECUTABLES
bin/testfecha: obj/fecha.o obj/testfecha.o
	$(CXX) $(FLAGS) $^ -o $@ 

bin/testfirewall: obj/firewall.o obj/fecha.o obj/testfirewall.o
	$(CXX) $(FLAGS) $^ -o $@

bin/testanalizador: obj/analizador.o obj/firewall.o obj/fecha.o
	$(CXX) $(FLAGS) $^ -o $@ -lfl

#OBJETOS
obj/fecha.o: src/fecha.cpp include/fecha.hpp
	$(CXX) $(FLAGS) -c $< -o $@ -Iinclude
obj/testfecha.o: src/testfecha.cpp
	$(CXX) $(FLAGS) -c $< -o $@ -Iinclude
obj/firewall.o: src/firewall.cpp include/firewall.hpp
	$(CXX) $(FLAGS) -c $< -o $@ -Iinclude
obj/testfirewall.o: src/testfirewall.cpp
	$(CXX) $(FLAGS) -c $< -o $@ -Iinclude
obj/analizador.o: src/analizador.c
	$(CXX) $(FLAGS) -c $< -o $@ -Iinclude 

#LEX
src/analizador.c: src/analizador.l
	$(FXX) $@ $<


#OTRAS REGLAS
clean:
	rm -rvf $(BINS) $(OBJS) $(wildcard src/analizador.c)