#include "articulo.hpp"
#include <iomanip>


////////Clase Articulo/////////

Articulo::Articulo(const Autores& autores,const Cadena& codRef, const Cadena& titulo, const Fecha& publicacion, double precio): 
autores_(autores),codRef_(codRef), titulo_(titulo), publicacion_(publicacion),precio_(precio)
{
	if(autores.empty())
		throw Autores_vacios();
}

Cadena Articulo::referencia()const noexcept
{
	return codRef_;
}

Cadena Articulo::titulo()const noexcept
{
	return titulo_;
}
Fecha Articulo::f_publi()const noexcept
{
	return publicacion_;
}
double Articulo::precio()const noexcept
{
	return precio_;
}

double& Articulo::precio() noexcept
{
	return precio_;
}

//Metodo Externo

const Articulo::Autores& Articulo::autores()const noexcept{return autores_;}

std::ostream& operator<<(std::ostream& os, const Articulo& ar)
{
	os << "["<<ar.referencia().c_str()<<"] \""<<ar.titulo().c_str()<<"\", de ";

	Articulo::Autores::const_iterator j = ar.autores().end();
	j--;
	for(Articulo::Autores::const_iterator i = ar.autores().begin(); i != ar.autores().end();)
	{
		os<<(*i)->apellidos();
		if((i++) != j)
			os<<", ";
	}
	os<<". "<<ar.f_publi().anno()<<". "<< std::fixed << std::setprecision(2)<<ar.precio()<<" €\n";
	ar.impresion_especifica(os);

	return os;
}

////////////ArticuloAlmacenable////////////////


ArticuloAlmacenable::ArticuloAlmacenable(const Autores& autores,const Cadena& codRef, const Cadena& titulo, const Fecha& publicacion, double precio, size_t stock) : 
Articulo(autores,codRef,titulo,publicacion,precio), stock_(stock){}

size_t ArticuloAlmacenable::stock()const noexcept
{
	return stock_;
}
size_t& ArticuloAlmacenable::stock() noexcept
{
	return stock_;
}

//////////Libro////////////////

Libro::Libro(const Autores& autores,const Cadena& codRef, const Cadena& titulo, const Fecha& publicacion, double precio, size_t n_pagina,size_t stock) :
ArticuloAlmacenable(autores,codRef,titulo,publicacion,precio,stock),n_pag_(n_pagina){}

const size_t Libro::n_pag()const noexcept
{
	return n_pag_;
}

void Libro::impresion_especifica(std::ostream& os)const noexcept
{
	os<<"\t"<<n_pag_<<" págs., "<<stock_<<" unidades.";
}


//////////Cederron/////////////

Cederron::Cederron(const Autores& autores,const Cadena& codRef, const Cadena& titulo, const Fecha& publicacion, double precio, size_t tam,size_t stock) :
ArticuloAlmacenable(autores,codRef,titulo,publicacion,precio,stock),tam_(tam){}

const size_t Cederron::tam()const noexcept
{
	return tam_;
}

void Cederron::impresion_especifica(std::ostream& os)const noexcept
{
	os<<"\t"<<tam_<<" MB, "<<stock_<<" unidades.";
}

/////////LibroDigital///////////////
LibroDigital::LibroDigital(const Autores& autores,const Cadena& codRef, const Cadena& titulo, const Fecha& publicacion, double precio,Fecha f_expir) :
Articulo(autores,codRef,titulo,publicacion,precio),f_expir_(f_expir){}

const Fecha& LibroDigital::f_expir()const noexcept
{
	return f_expir_;
}

void LibroDigital::impresion_especifica(std::ostream& os)const noexcept
{
	os<<"\tA la venta hasta el "<<f_expir_<<".";	
}











