#ifndef USUARIO_PEDIDO_H_
#define USUARIO_PEDIDO_H_
#include "pedido.hpp"
#include "usuario.hpp"



class Usuario_Pedido
{
public:
	typedef std::set<Pedido*> Pedidos;
	typedef std::map<Usuario*,Pedidos> UsuarioPedido;
	typedef std::map<Pedido*,Usuario*> PedidoUsuario;

	//Metodos Asociativos

	void asocia(Usuario& usuario, Pedido& pedido)
	{
		usped_[&usuario].insert(&pedido);
		pedus_[&pedido]= &usuario;
	}
	void asocia(Pedido& pedido, Usuario& usuario)
	{
		asocia(usuario,pedido);
	}
	Pedidos& pedidos(Usuario& usuario){return usped_[&usuario];}
	Usuario* cliente(Pedido& pedido){return pedus_[&pedido];}

private:
	UsuarioPedido usped_;
	PedidoUsuario pedus_;

};

#endif//USUARIO_PEDIDO_H_