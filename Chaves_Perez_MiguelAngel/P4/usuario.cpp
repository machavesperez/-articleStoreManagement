#include "usuario.hpp"
#include <unistd.h> //para el crypt
#include <utility>
#include <unordered_set>
#include <iomanip>


using namespace std;

bool Clave::isSRandSet_ = false;//Bool para generalizar una sola vez los numeros aleatorio

Clave::Clave(const char* clave)
{
	const char* codificacion_ = {"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdef./ghijklmnopqrstuvwxyz0123456789"};
	char sal_[2];

	if( strlen(clave)<5 )
		throw Incorrecta(CORTA);

    //Condicion para generalizar una sola vez los numeros aleatorios
	if(!isSRandSet_)
	{
		isSRandSet_ = true;
		srand(time(0));
	}

    sal_[0] = codificacion_[rand()%64];
    sal_[1] = codificacion_[rand()%64];
    
    const char* password_encriptado = crypt(clave, sal_);
    if( password_encriptado == nullptr )
		throw Incorrecta(ERROR_CRYPT);
    password_= password_encriptado;
}

Cadena Clave::clave()const
{
	return password_;
}
bool Clave::verifica(const char* clave)const
{
	return Cadena (crypt(clave,password_.c_str())) == password_;
}

std::unordered_set<Cadena> Usuario::setIden_;

//USUARIO

//Metodo Internos

Usuario::Usuario(const Cadena& id, const Cadena& nombre, const Cadena& apellidos, const Cadena& direccion, const Clave& clave) : 
id_(id), nombre_(nombre), apellidos_(apellidos), direccion_(direccion), clave_(clave)
{
	if(setIden_.insert(id).second==false)
		throw Id_duplicado(id);
}
Cadena Usuario::id() const {return id_;}

Cadena Usuario::nombre() const{return nombre_;}

Cadena Usuario::apellidos() const{return apellidos_;}

Cadena Usuario::direccion() const{return direccion_;}

const Usuario::Tarjetas& Usuario::tarjetas()const
{
	return tarjetas_;
}

//Metodos de la Asociacion

void Usuario::es_titular_de(Tarjeta& tarjeta)
{
	if(tarjeta.titular()==this)
		tarjetas_.insert(make_pair(tarjeta.numero(),&tarjeta));
}

void Usuario::no_es_titular_de(Tarjeta& tarjeta)
{
	tarjetas_.erase(tarjeta.numero());
	tarjeta.anula_titular();
}


Usuario::~Usuario()
{
	for(Usuario::Tarjetas::const_iterator i = tarjetas_.begin();i !=tarjetas_.end();i++)
	{
		(i->second)->anula_titular();
		//tarjetas_.erase(i);
	}
	tarjetas_.clear();
	setIden_.erase(id_);
}

void Usuario::compra(Articulo& articulo, unsigned int cantidad)
{
	if(cantidad==0)
		carrito_.erase(&articulo);
	else
		carrito_[&articulo] = cantidad;


}
const Articulos& Usuario::compra()const
{
	return carrito_;
}

size_t Usuario::n_articulos()const
{
	return carrito_.size();
}

std::ostream& operator<<(std::ostream& os,const Usuario& usuario)
{
	 os << usuario.id() << " [" << usuario.clave_.clave() << "]" <<usuario.nombre()
	<< " " << usuario.apellidos() << std::endl << usuario.direccion() << std::endl 
	<< "Tarjetas: " << std::endl;
    for( Usuario::Tarjetas::const_iterator i = usuario.tarjetas().begin();
	 i != usuario.tarjetas().end() ; i++)
	    os << *(i->second) << std::endl;
    return os;
}

//Metodos Externos

void mostrar_carro(std::ostream& os, const Usuario& usuario)
{
	os << "Carrito de compra de " << usuario.id() << " [Artículos: "
       << usuario.n_articulos() << "]" << std::endl << "Cant. Artículo" << std::endl
       << "============================================================" << std::endl;
    for(Articulos::const_iterator i = usuario.compra().begin() ; i != usuario.compra().end() ; i++ )
    {
		os << " \t" << i->second << " \t" << "[" << (i->first)->referencia().c_str() << "] \"" << (i->first)->titulo().c_str()
		   << "\", " << (i->first)->f_publi().anno() << ". " << std::fixed << std::setprecision(2) << (i->first)->precio()
		   << " €" << std::endl;
    }
}