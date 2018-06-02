#ifndef AUTOR_H_
#define AUTOR_H_


class Autor
{
public:
	Autor(Cadena nombre, Cadena apellidos, Cadena direccion)noexcept: nombre_(nombre), apellidos_(apellidos), direccion_(direccion){}
	const Cadena nombre()const noexcept{return nombre_;}
	const Cadena apellidos()const noexcept{return apellidos_;}
	const Cadena direccion()const noexcept{return direccion_;}
private:
	const Cadena nombre_;
	const Cadena apellidos_;
	const Cadena direccion_;
};

#endif//AUTOR_H_