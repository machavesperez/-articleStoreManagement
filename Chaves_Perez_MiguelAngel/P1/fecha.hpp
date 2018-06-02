#ifndef Fecha_H_
#define Fecha_H_

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <ctime>



class Fecha
{
public:
	class Invalida
	{
	public:
		Invalida(const char* error): error_(error){}
		const char* por_que()const {return error_;}
	private:
		const char* error_;
	};
	explicit Fecha(int dia=0, int mes=0, int anno=0);
	Fecha(const char* cadenaFecha);
	static const int AnnoMinimo = 1902;
	static const int AnnoMaximo = 2037;
	
	int dia()const;
	int mes()const;
	int anno()const;
	
	Fecha operator ++(int);//POST INC 
	Fecha& operator ++();//PRE INC
	Fecha operator --(int);// POST DEC 
	Fecha& operator --();// PRE INC
	Fecha& operator +=(int numDia);
	Fecha& operator -=(int numDia);
	Fecha operator +(int numDia) const;
	Fecha operator -(int numDia) const;
	const char* cadena()const;
private:
	int dia_;
	int mes_;
	int anno_;
	bool valida()const;
	
};


long int operator -(const Fecha& fecha1, const Fecha& fecha2);
bool operator <(const Fecha& fecha1, const Fecha& fecha2);
bool operator <=(const Fecha& fecha1, const Fecha& fecha2);
bool operator >=(const Fecha& fecha1, const Fecha& fecha2);
bool operator ==(const Fecha& fecha1, const Fecha& fecha2);
bool operator >(const Fecha& fecha1, const Fecha& fecha2);
bool operator !=(const Fecha& fecha1, const Fecha& fecha2);

std::istream& operator>>(std::istream& is, Fecha& fecha);
std::ostream& operator<<(std::ostream& os, const Fecha& fecha);

#endif