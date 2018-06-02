#include "tarjeta.hpp"
#include <cctype> //para el isdigit
#include <algorithm> //para lambda []
#include "cadena.hpp"

//LUHN
bool luhn (const Cadena& numero, size_t n);

//NUMERO

Numero::Numero (const Cadena& num)
{


	Cadena cadena_aux(num);
  
	auto i = std::remove_if(cadena_aux.begin(),cadena_aux.end(), [](char c){return isspace(c);});//Borra los espacios en blanco 
	*i = '\0';
	auto f = std::find_if(cadena_aux.begin(),i,[](char a){return !isdigit(a);});//compruebo que son dígitos.
	if(i != f)
	    throw Incorrecto(DIGITOS);

	Cadena cadenaFinal(cadena_aux.c_str());


	
	if( cadenaFinal.length()<13 || cadenaFinal.length()>19) 
	    throw Incorrecto(LONGITUD);
	if(!luhn(cadenaFinal, cadenaFinal.length())) 
	    throw Incorrecto(NO_VALIDO);
	
	numero_ = cadenaFinal;
}

Numero::operator const char*() const
{
	return numero_.c_str();
}

bool operator <(const Numero& num1, const Numero& num2)//Operador < para tener un orden
{
	return (strcmp(num1,num2)<0);
}
//TARJETA

//Metodos Internos//

Tarjeta::Tarjeta(Tipo tipo,const Numero& numero, Usuario& use, const Fecha& caducidad) : tipo_(tipo),numero_(numero), titular_(&use)
{
	Fecha fechaHoy;
	if(caducidad<fechaHoy)
		throw Caducada(caducidad);
	else
	{
		caducidad_=caducidad;
		titular_facial_ = use.nombre() + " " +use.apellidos();
		for(size_t i=0;i<titular_facial_.length();++i)
			titular_facial_[i] = toupper(titular_facial_[i]);
		use.es_titular_de(*this);	
	}
}

Tarjeta::Tipo Tarjeta::tipo() const {return tipo_;}

Numero Tarjeta::numero() const {return numero_;}

Fecha Tarjeta::caducidad() const {return caducidad_;}

Cadena Tarjeta::titular_facial() const{return titular_facial_;}

const Usuario* Tarjeta::titular() const{return titular_;}

void Tarjeta::anula_titular()
{
	titular_=nullptr;
}

Tarjeta::~Tarjeta()
{
	if(titular_)
    //eliminamos el const de titular en este caso
	const_cast<Usuario*>(titular_) -> no_es_titular_de( *this );
}

//Metodos Internos

std::ostream& operator << (std::ostream& os, const Tarjeta& tarjeta){
    os <<tarjeta.tipo() << tarjeta.numero() << std::endl << tarjeta.titular_facial() << std::endl;
    int i = tarjeta.caducidad().anno()%100;
    os << "Caduca: " << ( tarjeta.caducidad().mes()<10 ? "0" : "" )
       << tarjeta.caducidad().mes() << ( i<10 ? "/0" : "/" ) << i << std::endl;
    return os;
}

std::ostream& operator <<(std::ostream& os, Tarjeta::Tipo t)
{
	switch(t)
	{
		case 1: os<<"VISA";
		break;
		case 2: os<<"Mastercard";
		break;
		case 3: os<<"Maestro";
		break;
		case 4: os<<"JCB";
		break;
		case 5: os<<"AmericanExpress";
	}

	return os;
}

bool operator < (const Tarjeta& t1, const Tarjeta& t2)
{
    return ( t1.numero() < t2.numero() );
}