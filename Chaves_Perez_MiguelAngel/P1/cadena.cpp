#include <iostream>
#include <iomanip>
#include "cadena.hpp"
#include <cstring>
#include <stdlib.h>


using namespace std;

// INTERNOS //

Cadena::Cadena(size_t tam, char caracter) : s_(new char[(tam+1)]), tam_(tam)
{
	for(size_t i =0;i<tam;++i)
		s_[i] = caracter;
	s_[tam_] = '\0';
}

Cadena::Cadena(const char* cadena, size_t n_primeros)
{
	if(cadena == NULL)
	{
		tam_ = 0;
		s_ = NULL;
	}
	else
	{

		if(n_primeros >= strlen(cadena))
			tam_ = strlen(cadena);
		else
			tam_ = n_primeros;
	
		s_ = new char[tam_ +1];
		for(size_t i = 0; i<tam_;i++)
			s_[i] = cadena[i];
		s_[tam_] = '\0';
	}

}

Cadena::Cadena(const Cadena& cadenaOriginal, size_t inicial,size_t longitud)
{
	if(inicial>cadenaOriginal.tam_ ||(inicial == cadenaOriginal.tam_ && inicial != 0 ))
		throw std::out_of_range("Indice inicio fuera del rango del tamanno de la cadena.");
	//if(longitud>=cadenaOriginal.tam_ || inicial + longitud >= cadenaOriginal.tam_)//Evitar la suma
	if(longitud >= cadenaOriginal.tam_ || inicial >= cadenaOriginal.tam_ - longitud)
	{
		tam_ = cadenaOriginal.tam_ - inicial;
		s_ = new char[tam_ +1];
	}
	else
	{
		s_ = new char[longitud+1];
		tam_ = longitud;
	}
		

	for(size_t i = 0; i < tam_; ++i)
		s_[i] = cadenaOriginal.s_[inicial + i];

	s_[tam_] = '\0';

}

Cadena::Cadena(Cadena&& cadenaOriginal) : s_(cadenaOriginal.s_), tam_(cadenaOriginal.tam_)
{
    cadenaOriginal.s_ = nullptr;
    cadenaOriginal.tam_ = 0;
}

Cadena& Cadena::operator =(const Cadena& cadenaOriginal)
{
	if(this != &cadenaOriginal)//evistamos la autoasignacion
	{
		if(tam_ != cadenaOriginal.tam_)
			delete[] s_;
		tam_ = cadenaOriginal.tam_;
		s_ = new char[tam_ +1];
		strcpy(s_,cadenaOriginal.s_);
	}

	return *this;

}

Cadena& Cadena::operator =(const char* cadenaOriginal)
{
	if(tam_ != strlen(cadenaOriginal))
	{
		delete[] s_;
		tam_ = strlen(cadenaOriginal);
		s_ = new char[tam_+1];
	}
	strcpy(s_,cadenaOriginal);
	//s_[tam_] = '\0';
	return *this;
}

Cadena& Cadena::operator =(Cadena&& cadenaOriginal)
{
	if(this != &cadenaOriginal)
	{
		delete[] s_;
		s_ = cadenaOriginal.s_;
		tam_ = cadenaOriginal.tam_;
		cadenaOriginal.s_ = nullptr;
		cadenaOriginal.tam_ = 0;
	}

	return *this;
	
}

const char* Cadena::c_str()const
{
	return s_;
}

size_t Cadena::length()const
{
	return tam_;
}

char Cadena::operator[](size_t indice)const noexcept
{
	return s_[indice];	
}

char Cadena::at(size_t indice)const
{
	if(indice<tam_)
		return s_[indice];
	else
		throw std::out_of_range("No esta dentro del rango");
}
	
char& Cadena::operator[](size_t indice) noexcept
{
	return s_[indice];
}

char& Cadena::at(size_t indice)
{
	if(indice<tam_)
		return s_[indice];
	else
		throw std::out_of_range("No esta dentro del rango");
}

Cadena Cadena::substr(size_t indice, size_t tamanno)const
{
	if(indice>=tam_)
		throw std::out_of_range("Indice superior al tamanno de la cadena");
	if(tamanno >= tam_ -indice)
		throw std::out_of_range("Indice + tamanno superior al tamanno de la cadena");

	char* auxiliar = new char[tamanno +1];

	std::strncpy(auxiliar,s_+indice,tamanno);
	
	/*Cadena auxiliar(tamanno);
	
	for(size_t i=0; i < tamanno;i++)
	{
		auxiliar.s_[i]=s_[indice + i];
	}

	auxiliar.s_[auxiliar.tam_] = '\0';*/

	auxiliar[tamanno] = '\0';

	return auxiliar;
}

Cadena::~Cadena()
{
	delete[] s_;
}

// EXTERNOS //

Cadena operator +(const Cadena& op1, const Cadena& op2)
{
	
	char* original = new char[op1.length() + op2.length() + 1];
	strcpy(original,op1.c_str());
	strcat(original,op2.c_str());
	//Cadena *Cadena_resultado = new Cadena(original);
	Cadena nueva_resul(original);
	delete[] original;

	return nueva_resul;
}

Cadena& operator +=(Cadena& op1, const Cadena& op2)
{
	op1 = op1 + op2;
	return op1;
}

bool operator ==(const Cadena& cad1, const Cadena& cad2)
{
	return (cad1.length()==cad2.length() && !strcmp(cad1.c_str(),cad2.c_str()));
}

bool operator <(const Cadena& cad1, const Cadena& cad2)
{
	return (strcmp(cad1.c_str(),cad2.c_str())<0);
}
bool operator >(const Cadena& cad1, const Cadena& cad2)
{
	return (strcmp(cad1.c_str(),cad2.c_str())>0);
}
bool operator <=(const Cadena& cad1, const Cadena& cad2)
{
	return (cad1 < cad2 || cad1 == cad2); 
}
bool operator >=(const Cadena& cad1, const Cadena& cad2)
{
	return (cad1 > cad2 || cad1== cad2);
}
bool operator !=(const Cadena& cad1, const Cadena& cad2)
{
	return !(cad1==cad2);
}

std::istream& operator >>(std::istream& is, Cadena& cad)
{
	char cadena[32]="";
  
    while(is.peek()==' ' && is.peek() != EOF)
    {
	is.get();
    }
     
    if(is.peek() == EOF)
    {
	cad = cadena;
    }
    else
    {
	is >> cadena;
        cad = cadena;
    } 
    
    return is;
}

std::ostream& operator <<(std::ostream& os, const Cadena& cadena)
{
	os << cadena.c_str();
	return os;
}

// ITERADORES //

Cadena::iterator Cadena::begin() noexcept{return s_;}

Cadena::const_iterator Cadena::begin() const noexcept{return s_;}

Cadena::const_iterator Cadena::cbegin() const noexcept{return s_;}

Cadena::iterator Cadena::end() noexcept{return (s_+tam_);}

Cadena::const_iterator Cadena::end() const noexcept{return (s_+tam_);}

Cadena::const_iterator Cadena::cend() const noexcept{return (s_+tam_);}

Cadena::reverse_iterator Cadena::rbegin() noexcept{return reverse_iterator(end());}

Cadena::const_reverse_iterator Cadena::rbegin() const noexcept{return const_reverse_iterator(cend());}

Cadena::const_reverse_iterator Cadena::crbegin() const noexcept{return const_reverse_iterator(cend());}

Cadena::reverse_iterator Cadena::rend() noexcept{return reverse_iterator(begin());}

Cadena::const_reverse_iterator Cadena::rend() const noexcept{return const_reverse_iterator(cbegin());}

Cadena::const_reverse_iterator Cadena::crend() const noexcept{return const_reverse_iterator(cbegin());}



