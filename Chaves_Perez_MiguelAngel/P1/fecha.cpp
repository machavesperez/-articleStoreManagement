#include "fecha.hpp"
#include <iostream>
#include <ctime>
#include <stdio.h>

using namespace std;

// INTERNOS //

Fecha::Fecha(int dia, int mes, int anno) : dia_(dia), mes_(mes), anno_(anno)
{
	time_t tiempo_calendario = time(0);
	tm* tiempo_descompuesto = localtime(&tiempo_calendario);

	if(dia == 0)
	{
		dia_ = tiempo_descompuesto->tm_mday;
	}
	if(mes == 0)
	{
		mes_ = tiempo_descompuesto->tm_mon +1;
	}
	if(anno == 0)
	{
		anno_ = tiempo_descompuesto->tm_year + 1900;
	}

	if(!valida())
		throw Invalida("Fecha invalida en el constructor");
}

Fecha::Fecha(const char* cadenaFecha)
{
	if(!(sscanf(cadenaFecha,"%d/%d/%d",&dia_, &mes_, &anno_)==3))
		throw Invalida("Fallo en el constructor");

	time_t tiempo_calendario = time(0);
	tm* tiempo_descompuesto = localtime(&tiempo_calendario);

	if(dia_ == 0)
	{
		dia_ = tiempo_descompuesto->tm_mday;
	}
	if(mes_ == 0)
	{
		mes_ = tiempo_descompuesto->tm_mon +1;
	}
	if(anno_ == 0)
	{
		anno_ = tiempo_descompuesto->tm_year + 1900;
	}

	if(!valida())
		throw Invalida("Fallo en el constructor");

}

int Fecha::dia()const
{
	return dia_;
}

int Fecha::mes()const
{
	return mes_;
}

int Fecha::anno()const
{
	return anno_;
}


Fecha& Fecha::operator+=(int numDia)
{
	tm Fecha_auxiliar={0,0,0,dia_+numDia,mes_-1,anno_-1900,0,0,0};
	if(mktime(&Fecha_auxiliar) == static_cast<time_t>(-1)) throw Invalida("error en mktime en en el operador+=");
	dia_ = Fecha_auxiliar.tm_mday;
	mes_ = Fecha_auxiliar.tm_mon+1;
	anno_ = Fecha_auxiliar.tm_year+1900;

	if(!(valida()))
		throw Invalida("Fecha fuera de rango en +=");
			
	return *this;
}

Fecha& Fecha::operator-=(int numDia)
{
	return *this += (-numDia);
}

Fecha Fecha::operator++(int)
{
	Fecha temporal (*this);
	*this += 1;
	return temporal;
}

Fecha& Fecha::operator++()
{
	return *this +=1;
}

Fecha Fecha::operator--(int)
{
	Fecha temporal(*this);
	*this += -1;

	return temporal;
}

Fecha& Fecha::operator--()
{
	return *this += -1;
}

const char* Fecha::cadena() const
{

	tm fecha_struct = {0,0,0,dia_,mes_-1,anno_-1900,0,0,0 };
	static char fecha_caracteres[sizeof("miércoles 20 de septiembre de 2000")];
	static const char* const nombre_dia[] = {"domingo", "lunes", "martes", "miércoles", "jueves", "viernes", "sábado"};	//mie'rcoles
	static const char* const nombre_mes[] = {"enero", "febrero", "marzo", "abril", "mayo", "junio", "julio", "agosto", "septiembre", "octubre", "noviembre", "diciembre"};

	std::mktime(&fecha_struct);

	sprintf(fecha_caracteres,"%s %d de %s de %d",nombre_dia[fecha_struct.tm_wday],dia_,nombre_mes[fecha_struct.tm_mon],anno_);

	return fecha_caracteres;
}

bool Fecha::valida()const
{
	bool result = true;
    		
    //Mes verdadero

	if(!((mes_>=1)&&(mes_<=12)))
		result=false;
		
	//Día verdadero

	if((dia_>=1)||(dia_<=31))
	{

		if((mes_==4||mes_==6||mes_==9||mes_==11)&&(dia_==31))
			result=false;

		if((mes_==2)&&(dia_==30||dia_==31))
			result = false;

		if(!(anno_ % 4 == 0 && (anno_ % 400 == 0 || anno_ % 100 !=0))&&(dia_==29)&&mes_==2)			
			result = false;
			
	}
	else
		result = false;

	// Año verdadero

	if((anno_ > AnnoMaximo) || (anno_< AnnoMinimo))
		result = false;

	return result;
}

// EXTERNOS //

Fecha Fecha::operator+(int numDia) const
{
	Fecha temporal(*this);

	return temporal += numDia; 
}

Fecha Fecha::operator-(int numDia) const
{
	return Fecha(*this) += -numDia;
}


long int operator -(const Fecha& fecha1, const Fecha& fecha2)//DUDAAA
{
	/*long int diaTotal = 0;
	int aux=0;
		
	Fecha mayor,menor;
	if(fecha1 > fecha2)
	{
		mayor = fecha1;
		menor = fecha2;	
	}
	else if(fecha1 == fecha2)
		return 0;
	else
	{
		menor = fecha1;
		mayor = fecha2;
	}

	if(mayor.anno() != menor.anno())
		if(mayor.mes() > menor.mes() || (mayor.mes() == menor.mes() && mayor.dia() > menor.dia()))
			diaTotal = (((mayor.anno()) - menor.anno())*365); //Diferencia de dias entre los dos años
		else
			diaTotal = (((mayor.anno()-1) - menor.anno())*365); //Diferencia de dias entre los dos años
			
	//Vamos a ver si hay algun anno bisiesto		
	for(int i=menor.anno(); i < mayor.anno(); i++)
	{
		if(i % 4 == 0 && (i % 400 == 0 || i % 100 !=0))
			aux++;
	}
	diaTotal = diaTotal + aux;

	int diasMes = 0;
	for(int i = 12; i > menor.mes(); i--)
	{
		if(i==4||i==6||i==9||i==11)
			diasMes=diasMes + 30;
		else if(i == 2)
		{
			if(menor.anno() % 4 == 0 && (menor.anno() % 400 == 0 || menor.anno() % 100 !=0))
					diasMes=diasMes + 29;
			else
					diasMes=diasMes + 28;
		}
		else
				diasMes=diasMes + 31;
	}
	diaTotal = diaTotal + diasMes;
		
	diasMes = 0;
	for(int i = 1; i < mayor.mes(); ++i)
	{
		if(i==4||i==6||i==9||i==11)
			diasMes=diasMes + 30;
		else if(i == 2)
		{
			if(mayor.anno() % 4 == 0 && (mayor.anno() % 400 == 0 || mayor.anno() % 100 !=0))
				diasMes=diasMes + 29;
			else
				diasMes=diasMes + 28;
		}
		else
			diasMes=diasMes + 31;
	}
	diaTotal = diaTotal + diasMes;
	int diasDia = 0;
	if(menor.mes()==4||menor.mes()==6||menor.mes()==9||menor.mes()==11)
		diasDia = 30 - menor.mes();
	else if(menor.mes() == 2)
	{
		if(menor.anno() % 4 == 0 && (menor.anno() % 400 == 0 || menor.anno() % 100 !=0))	
			diasDia = 29 - menor.dia();
		else
			diasDia = 28 - menor.dia();
	}
	else
		diasDia = 31 - menor.dia();
	diaTotal = diaTotal + diasDia;

	diaTotal = diaTotal + (mayor.dia());
	
	if(fecha1 > fecha2)
		diaTotal = diaTotal;
	else
		diaTotal = diaTotal * -1;
	return diaTotal;*/

	tm fecha_struct1 = {0,0,0,fecha1.dia(),fecha1.mes()-1,fecha1.anno()-1900,0,0,0 };
	tm fecha_struct2 = {0,0,0,fecha2.dia(),fecha2.mes()-1,fecha2.anno()-1900,0,0,0 };
	long int segundosF1 = std::mktime(&fecha_struct1);
	long int segundosF2 = std::mktime(&fecha_struct2);

	return std::difftime(segundosF1,segundosF2)/86400;


}

bool operator <(const Fecha& fecha1, const Fecha& fecha2)
{
	bool menorQue = false;

	if(fecha1.anno() < fecha2.anno())	
		menorQue = true;
	
	else
	{
		if(fecha1.anno()==fecha2.anno())
		{
			if(fecha1.mes() < fecha2.mes())
				menorQue = true;
				
			else
			{
				if(fecha1.mes() == fecha2.mes())
				{
					if(fecha1.dia() < fecha2.dia())
							menorQue = true;
						
				}

			}
		}
	}

	return menorQue;
}

bool operator <=(const Fecha& fecha1, const Fecha& fecha2)
{
	return !(fecha2<fecha1);
}
bool operator ==(const Fecha& fecha1, const Fecha& fecha2)
{
	return (fecha1.anno() == fecha2.anno() and fecha1.mes() == fecha2.mes() and fecha1.dia() == fecha2.dia());
}

bool operator >(const Fecha& fecha1, const Fecha& fecha2)
{
	return fecha2<fecha1;
}

bool operator >=(const Fecha& fecha1, const Fecha& fecha2)
{
	return !(fecha1<fecha2);
}

bool operator !=(const Fecha& fecha1, const Fecha& fecha2)
{
	return !(fecha1 == fecha2);
}

std::istream& operator >>(std::istream& is, Fecha& fecha)
{
	char cadena[11];
    
    is.width(11); //esto limita la lectura a 10 caracteres.
    is>>cadena;
    fecha=Fecha(cadena);  //estoy haciendo una llamada anónima al constructor, es mas eficiente que crear una variable Fecha.
    return is;
}

std::ostream& operator <<(std::ostream& os, const Fecha& fecha)
{
	return os<<fecha.cadena();
}


