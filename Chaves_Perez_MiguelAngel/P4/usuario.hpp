#ifndef USUARIO_H_
#define USUARIO_H_
#include "cadena.hpp"
#include "articulo.hpp"
#include "tarjeta.hpp"
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>

class Numero;
class Tarjeta;
class Articulo;

typedef std::unordered_map<Articulo*, unsigned int> Articulos;

class Clave
{
public:
	enum Razon {CORTA, ERROR_CRYPT};
	Clave(const char* clave);
	Cadena clave()const;
	bool verifica(const char* clave)const;
	class Incorrecta
	{
	public:	
		Incorrecta(Razon razon) : razon_(razon){}
		Razon razon()const{return razon_;}
	private:
		Razon razon_;		
	};
	
private:
	static bool isSRandSet_;
	Cadena password_;
	
};

class Usuario
{
public:
	class Id_duplicado
	{
	public:
		Id_duplicado(Cadena idd) : idd_(idd){}
		Cadena idd()const{return idd_;}
	private:
		Cadena idd_;
	};
	typedef std::map<Numero,Tarjeta*>Tarjetas;
	
	Usuario(const Cadena& id, const Cadena& nombre, const Cadena& apellidos, const Cadena& direccion, const Clave& clave);
	
	Usuario(const Usuario& us) = delete;//Prohibicion
	Usuario& operator =(const Usuario& user) = delete;//Prohibicion
	
	Cadena id()const;
	Cadena nombre()const;
	Cadena apellidos()const;
	Cadena direccion()const;
	const Tarjetas& tarjetas()const;
	//Asociaciones
	void es_titular_de(Tarjeta& tarjeta);
	void no_es_titular_de(Tarjeta& tarjeta);

	~Usuario();
	void compra(Articulo& articulo, unsigned int cantidad=1);
	const Articulos& compra()const;
	size_t n_articulos()const;
	friend std::ostream& operator<<(std::ostream& os, const Usuario& usuario);
	

private:
	Cadena id_;
	Cadena nombre_;
	Cadena apellidos_;
	Cadena direccion_;
	Clave clave_;
	Tarjetas tarjetas_;
	Articulos carrito_;
	static std::unordered_set<Cadena> setIden_;//Para identificar si hay o no usuarios repetidos


};


void mostrar_carro(std::ostream& os,const Usuario& usuario);

#endif