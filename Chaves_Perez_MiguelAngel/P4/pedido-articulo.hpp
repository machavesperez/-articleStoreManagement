#ifndef PEDIDO_ARTICULO_H_
#define PEDIDO_ARTICULO_H_
#include "pedido.hpp"
#include "articulo.hpp"
#include <map>


class LineaPedido
{
public:
	explicit LineaPedido(double PrecioVenta, unsigned cantidad=1);
	double precio_venta()const;
	unsigned cantidad()const;

private:
	double precioVenta;
	unsigned cantidad_;
};

std::ostream& operator <<(std::ostream& os, const LineaPedido& lp);


class Pedido_Articulo
{
public:
	class OrdenaPedidos
	{
	public:
		bool operator()(const Pedido* pedido1, const Pedido* pedido2)const
		{
			return pedido1->numero() < pedido2->numero();
		}
	};
	class OrdenaArticulos
	{
	public:
		bool operator()(const Articulo* articulo1, const Articulo* articulo2)const
		{
			return articulo1->referencia() < articulo2->referencia();
		}
	};

	typedef std::map<Articulo*,LineaPedido, OrdenaArticulos> ItemsPedido;
	typedef std::map<Pedido*,ItemsPedido, OrdenaPedidos> PedidoArticulo;
	typedef std::map<Pedido*, LineaPedido, OrdenaPedidos> Pedidos;
	typedef std::map<Articulo*,Pedidos,OrdenaArticulos> ArticuloPedido;	

	void pedir(Pedido& pedido, Articulo& articulo, double precio, unsigned cantidad=1);
	void pedir(Articulo& articulo, Pedido& pedido, double precio, unsigned cantidad=1);
	ItemsPedido& detalle(Pedido&);
	Pedidos& ventas(Articulo&);
	void mostrarDetallePedidos(std::ostream& os);
	void mostrarVentasArticulos(std::ostream& os);

private:
	PedidoArticulo pedArt_;
	ArticuloPedido artPed_;
};

std::ostream& operator<<(std::ostream& os, const Pedido_Articulo::ItemsPedido& itemspedido);
std::ostream& operator <<(std::ostream& os, const Pedido_Articulo::Pedidos& pedidos);


#endif//PEDIDO_ARTICULO_H_