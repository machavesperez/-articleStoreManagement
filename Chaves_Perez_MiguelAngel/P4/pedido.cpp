#include "pedido.hpp"
#include "usuario-pedido.hpp"
#include "pedido-articulo.hpp"

int Pedido::n_pedidos = 0;

Pedido::Pedido(Usuario_Pedido& up, Pedido_Articulo& pa, Usuario& us, const Tarjeta& tar, const Fecha& fecha): 
num_(n_pedidos+1),tarjeta_(&tar), fecha_(fecha),total_(0.0)
{
	if(us.compra().empty())
		throw Vacio(us);
	if(&us != tar.titular())
		throw Impostor(us);
	if(tar.caducidad() < fecha)
		throw Tarjeta::Caducada(tar.caducidad());

	auto carro = us.compra();//Copia de carrito para no inhabilitar el iterador al eliminar un articulo

	for(Articulos::const_iterator i =carro.begin(); i != carro.end();++i)
	{
		if(ArticuloAlmacenable* artAlm = dynamic_cast<ArticuloAlmacenable*>(i->first))
		{
			if((i->second) > artAlm->stock())
			{
				Articulo* pa = i->first;
				const_cast<Articulos&>(us.compra()).clear();
				//for(Articulos::const_iterator j = us.compra().begin(); j != us.compra().end();++j)
					//us.compra(*(i->first),0);
				throw SinStock(*pa);
			}

		}
		//Comprobamos que el libro digital sigue en venta
		else if( LibroDigital* pLibDig = dynamic_cast<LibroDigital*> (i->first) )
		{

			if( pLibDig->f_expir() < Fecha())
			{
				Articulo* articulo = i->first;			
				us.compra( *articulo,0);	
			}
		}		
	}
	//caso en el que tuviesemos únicamente libros digitales pasados de fecha.
    if( us.compra().empty())
		throw Vacio( us );

	up.asocia(us,*this);

	auto carrito (us.compra());

	for(Articulos::const_iterator i = carrito.begin(); i != carrito.end(); ++i)
	{
		pa.pedir(*this,*(i->first),(i->first)->precio(),i->second);
		total_ += ( (i->first)->precio() * i->second );
		us.compra(*(i->first),0);
		if(ArticuloAlmacenable* artAlm = dynamic_cast<ArticuloAlmacenable*>(i->first))
			artAlm->stock() -= i->second;
	}

	++n_pedidos;
} 

///Metodos Internos////

int Pedido::numero()const{return num_;}

const Tarjeta* Pedido::tarjeta()const{return tarjeta_;}

Fecha Pedido::fecha()const{return fecha_;}

double Pedido::total()const{return total_;}

int Pedido::n_total_pedidos(){return n_pedidos;}

///Metodos Externos///

std::ostream& operator <<(std::ostream& os, const Pedido& pedido)
{
	return os << std::endl << "\tNúm. pedido:\t" << pedido.numero() << std::endl << "\tFecha:\t\t"
	      << pedido.fecha() << std::endl << "\tPagado con:\t" << pedido.tarjeta()->numero()<<"Mastercard"
	      << std::endl << "\tImporte:\t" << std::fixed << std::setprecision(2)
	      << pedido.total() << " €";
}


