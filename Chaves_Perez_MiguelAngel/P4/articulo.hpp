#ifndef ARTICULO_H_
#define ARTICULO_H_
#include "cadena.hpp"
#include "fecha.hpp" // 	QUITAR CARPETAS
#include <set>
#include "autor.hpp"


class Autor;

//Clase Abstracta
class Articulo
{
public:
	typedef std::set<Autor*> Autores;//He decidido crear una asociacion unidireccional de multiplicidad 1..* con la clase Autor
	class Autores_vacios{};
	Articulo(const Autores& autores,const Cadena& codRef, const Cadena& titulo, const Fecha& publicacion, double precio);
	//Observadores
	Cadena referencia()const noexcept;
	Cadena titulo()const noexcept;
	Fecha f_publi()const noexcept;
	double precio()const noexcept;
	double& precio()noexcept;
	const Autores& autores()const noexcept;
	virtual void impresion_especifica(std::ostream& os)const=0;//Virtual Puro
	virtual ~Articulo()=default;//Necesario decralar el destructor como Virtual, para posibles problemas

protected:
	Autores autores_;
	Cadena codRef_;
	Cadena titulo_;
	Fecha publicacion_;
	double precio_;
};

std::ostream& operator <<(std::ostream& os, const Articulo& ar);


class ArticuloAlmacenable : public Articulo
{
public:
	ArticuloAlmacenable(const Autores& autores,const Cadena& codRef, const Cadena& titulo, const Fecha& publicacion, double precio, size_t stock);
	size_t stock()const noexcept;
	size_t& stock()noexcept;
	~ArticuloAlmacenable(){}

protected:
	size_t stock_;
};

class Libro : public ArticuloAlmacenable
{
public:
	Libro(const Autores& autores,const Cadena& codRef, const Cadena& titulo, const Fecha& publicacion, double precio, size_t n_pagina,size_t stock);
	const size_t n_pag()const noexcept;
	virtual void impresion_especifica(std::ostream& os)const noexcept;
private:
	const size_t n_pag_;

};

class Cederron : public ArticuloAlmacenable
{
public:
	Cederron(const Autores& autores,const Cadena& codRef, const Cadena& titulo, const Fecha& publicacion, double precio, size_t tam,size_t stock);
	const size_t tam()const noexcept;
	virtual void impresion_especifica(std::ostream& os)const noexcept;
private:
	const size_t tam_;
};

class LibroDigital : public Articulo
{
public:
	LibroDigital(const Autores& autores,const Cadena& codRef, const Cadena& titulo, const Fecha& publicacion, double precio,Fecha f_expir);
	const Fecha& f_expir()const noexcept;
	virtual void impresion_especifica(std::ostream& os)const noexcept;
private:
	const Fecha f_expir_;
};

#endif//ARTICULO_H_