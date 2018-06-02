#ifndef TARJETA_H_
#define TARJETA_H_
#include "cadena.hpp"
#include "fecha.hpp"
#include "usuario.hpp"
#include "articulo.hpp"

class Usuario;

class Numero
{
public:
	Numero(const Cadena& numero);
	operator const char*()const;

	//Clase para la Excepcion
	enum Razon {LONGITUD, DIGITOS, NO_VALIDO};
	class Incorrecto
	{
	public:
		Incorrecto(Razon razon) : razon_(razon){}		
		Razon razon()const{return razon_;}
	private:
		Razon razon_;
	};
	
private:
	Cadena numero_;

};

bool operator <(const Numero& num1, const Numero& num2);

class Tarjeta
{
public:
	enum Tipo {VISA=1,Mastercard,Maestro,JCB,AmericanExpress};
	//Clase para la Excepcion
	class Caducada
	{
	public:
		Caducada(Fecha f) : cad_(f){}
		Fecha cuando()const{return cad_;}
	private:
		Fecha cad_;
	};
	Tarjeta(Tipo tipo,const Numero& numero,Usuario& use, const Fecha& f_cad);
	Tarjeta(const Tarjeta& tar)=delete;//Se prohiben
	Tarjeta& operator =(const Tarjeta& tar)=delete;//Se prohiben
	Tipo tipo()const;
	Numero numero()const;
	Fecha caducidad()const;
	Cadena titular_facial()const;
	const Usuario* titular()const;
	void anula_titular();
	~Tarjeta();

private:
	Tipo tipo_;
	Numero numero_;
	const Usuario* titular_;
	Fecha caducidad_;
	Cadena titular_facial_;

};

bool operator < (const Tarjeta& t1, const Tarjeta& t2);
std::ostream& operator <<(std::ostream& os, const Tarjeta& tarjetaOriginal);
std::ostream& operator <<(std::ostream& os, Tarjeta::Tipo t);

#endif//TARJETA_H_