#ifndef PEDIDO_H_
#define PEDIDO_H_
#include "usuario.hpp"
#include "tarjeta.hpp"
#include "fecha.hpp"
#include "articulo.hpp"
#include "iomanip"

class Tarjeta;
class Usuario_Pedido;
class Pedido_Articulo;

class Pedido
{
public:
	
	//Clases para las Excepciones
	class Vacio
	{
	public:
		Vacio(const Usuario& us) : us_(&us){}
		const Usuario& usuario()const{return *us_;}
	private:
		const Usuario* us_;
	};
	class Impostor
	{
	public:
		Impostor(const Usuario& us) : us_(&us){}
		const Usuario& usuario()const{return *us_;}
	private:
		const Usuario* us_;
	};
	class SinStock
	{
	public:
		SinStock(const Articulo& ar) : ar_(&ar){}
		const Articulo& articulo()const{return *ar_;}
	private:
		const Articulo* ar_;
	};


	Pedido(Usuario_Pedido& up, Pedido_Articulo& pa, Usuario& us, const Tarjeta& ta, const Fecha& fecha=Fecha() );
	int numero()const;
	const Tarjeta* tarjeta()const;
	Fecha fecha()const;
	double total()const;
	static int n_total_pedidos();

private:
	
	static int n_pedidos;//Numero de pedido en general
	int num_;//Numero pedido de cada objeto
	const Tarjeta* tarjeta_;//Tarjeta con la cual paga los pedidos
	Fecha fecha_;//Fecha del pedido
	double total_;//Importe total del pedido
	
};

std::ostream& operator <<(std::ostream& os, const Pedido& pedido);

#endif//PEDIDO_H_