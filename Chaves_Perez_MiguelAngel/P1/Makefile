

CXX = g++
CXXFLAGS = -g -Wall -std=c++11 -I.. -pedantic
OBJ_AUTO=fecha.o cadena.o test-P1-auto.o
BIN_AUTO=test-P1-auto
OBJ_CONSOLA=fecha.o cadena.o test-P1-consola.o
BIN_CONSOLA=test-P1-consola


all: $(BIN_AUTO) $(BIN_CONSOLA)

$(BIN_AUTO): $(OBJ_AUTO)
	$(CXX) $(LDFFLAGS) -o $(BIN_AUTO) $(OBJ_AUTO)

$(BIN_CONSOLA): $(OBJ_CONSOLA)
	$(CXX) $(LDFFLAGS) -o $(BIN_CONSOLA) $(OBJ_CONSOLA)

clean:
	$(RM) $(OBJ_AUTO) $(BIN_AUTO) $(OBJ_CONSOLA) $(BIN_CONSOLA) fecha_check cadena_check *.o


test-P0-auto.o: fecha.hpp cadena.hpp
fecha.o: fecha.hpp
cadena.o: cadena.hpp
test-P0-consola.o: cadena.cpp fecha.cpp


checks: check_fecha check_cadena cadena.cpp fecha.cpp

check_fecha: fecha_check fecha.cpp fecha.hpp
	./fecha_check  fecha.cpp  -- -std=c++11

check_cadena: cadena_check cadena.cpp cadena.hpp
	./cadena_check cadena.cpp -- -std=c++11

fecha_check: 
	$(MAKE) -f Make_check.mk $@

cadena_check: 
	$(MAKE) -f Make_check.mk $@





