#include "pedido-articulo.hpp"
#include <iomanip>

///LineaPedido///

LineaPedido::LineaPedido(double PrecioVenta, unsigned cantidad) : precioVenta(PrecioVenta), cantidad_(cantidad){}

double LineaPedido::precio_venta()const{return precioVenta;}

unsigned LineaPedido::cantidad()const{return cantidad_;}

std::ostream& operator <<(std::ostream& os, const LineaPedido& lp)
{
	os << std::fixed << std::setprecision(2) << lp.precio_venta()<< " €\t" << lp.cantidad();
	return os;

}

///Pedido///

void Pedido_Articulo::pedir(Pedido& pedido, Articulo& articulo, double precio, unsigned cantidad)
{
	LineaPedido lp(precio,cantidad);
	pedArt_[&pedido].insert(std::make_pair(&articulo,lp));
	artPed_[&articulo].insert(std::make_pair(&pedido,lp));
}

void Pedido_Articulo::pedir(Articulo& articulo,Pedido& pedido, double precio, unsigned cantidad)
{
	pedir(pedido,articulo,precio,cantidad);
}

Pedido_Articulo::ItemsPedido& Pedido_Articulo::detalle(Pedido& pedido)
{
	return pedArt_[&pedido];
}

Pedido_Articulo::Pedidos& Pedido_Articulo::ventas(Articulo& articulo)
{
	return artPed_[&articulo];
}

void Pedido_Articulo::mostrarDetallePedidos(std::ostream& os){
	double total=0.0;
	double ventas = 0.0;
	for(auto i: pedArt_){
		os << std::endl << "Pedido num. " << (i.first)->numero() << std::endl;
		os << "Cliente: " << (((i.first)->tarjeta())->titular())->nombre() << "\t\tFecha: " << (i.first)->fecha() << std::endl
		   << "==============================================================================" << std::endl
		   << "  PVP\t Cantidad\t Artículo" << std::endl
		   << "==============================================================================" << std::endl;
		for( Pedido_Articulo::ItemsPedido::const_iterator j = (i.second).begin(); j!=(i.second).end(); j++) {
				os << (j->second).precio_venta() <<" €\t" << (j->second).cantidad() << "\t\t[" << (j->first)->referencia() 
				   <<"] \"" << (j->first)->titulo() << "\"" << std::endl;
				total += ((j->second).precio_venta())*((j->second).cantidad());
				ventas += ((j->second).precio_venta())*((j->second).cantidad());
		}
		os << "==============================================================================" << std::endl
		   << "Total: " << ventas << " €" << std::endl << std::endl;
		ventas = 0.0;
	}
	os << "\nTOTAL VENTAS \t" << std::fixed << std::setprecision(2) 
	   << total << " €"<< std::endl;
}

void Pedido_Articulo::mostrarVentasArticulos(std::ostream& os){
	double ventas = 0;
	unsigned int cantidad = 0;
	for(auto i: artPed_){
		os << std::endl << " Ventas de " << "[" << (i.first)->referencia() << "]" << " \"" << (i.first)->titulo() << "\""
		<< " [Pedidos: " << (i.second).size() << "]" << std::endl 
		<< "==============================================================================" << std::endl
		<<  "PVP\tCantidad\tFecha de venta" << std::endl
		<< "==============================================================================" << std::endl
		;
		for(Pedido_Articulo::Pedidos::const_iterator j = (i.second).begin(); j!= (i.second).end(); j++){
			os << (j->second).precio_venta() << " €\t" << (j->second).cantidad() << "\t\t" << (j->first)->fecha() << std::endl;
			ventas += ( (j->second).precio_venta())*((j->second).cantidad());
			cantidad += (j->second).cantidad();
		}
		os << "==============================================================================" << std::endl
		<< ventas << " €\t" << cantidad << std::endl << std::endl;
		ventas = cantidad = 0;
	}
}

std::ostream& operator <<(std::ostream& os, const Pedido_Articulo::ItemsPedido& itemspedido)
{
	double importeTotal=0;
	unsigned ejemplares=0;;

	for(auto i: itemspedido)
	{
		os<< "Articulo: "<<*(i.first)<<"\tCantidad: "<<(i.second).cantidad()<<"\tPrecio_venta: "<<(i.second).precio_venta()<<"€"<<std::endl;
		importeTotal += ((i.second).precio_venta() * (i.second).cantidad());
		ejemplares += (i.second).cantidad();
	}

	os<<"Importe_Total: "<<importeTotal<<"€"<<"\tNumero_Ejemplares: "<<ejemplares<<std::endl;

	return os;
}

std::ostream& operator <<(std::ostream& os, const Pedido_Articulo::Pedidos& pedidos)
{
	
		double total=0.0;
	os << "\tCant.\tPrecio\tFecha" << std::endl
	   << "==============================================================================" << std::endl;
	for(auto i: pedidos){
		os << "\t" << (i.second).cantidad() << "\t" << std::fixed << std::setprecision(2)
		   << (i.second).precio_venta() << " €\t" << (i.first)->fecha() << std::endl;
		total += ( (i.second).cantidad() * (i.second).precio_venta() );
	}
	return os << "\tTotal: " << total <<" €";

}

