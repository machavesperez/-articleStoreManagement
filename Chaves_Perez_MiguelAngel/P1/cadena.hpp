#ifndef Cadena_H_
#define Cadena_H_

#include <cstring>
#include <stdexcept>
#include <functional>


class Cadena
{
public:
	typedef char* iterator;
    typedef const char* const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
	explicit Cadena(size_t tam=0, char caracter = ' ');
	Cadena(const char* cadena, size_t n_primeros = npos);
	Cadena(const Cadena& cadenaOriginal, size_t inicial=0, size_t =npos);
	Cadena(Cadena&& cadenaOriginal);
	Cadena& operator =(const Cadena& cadenaOriginal);
	Cadena& operator =(const char* cadenaOriginal);
	Cadena& operator =(Cadena&& cadenaOriginal);
	const char* c_str()const;
	size_t length()const;	
	char operator [](size_t indice)const noexcept;
	char at(size_t indice)const; 
	char& operator [](size_t indice) noexcept;
	char& at(size_t indice);
	Cadena substr(size_t indice, size_t tamanno)const;
	~Cadena();
	static const size_t npos = -1;

	//FUNCIONES ITERADORES
	iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;
    reverse_iterator rbegin() noexcept;
    const_reverse_iterator rbegin() const noexcept;
    const_reverse_iterator crbegin() const noexcept;
    reverse_iterator rend() noexcept;
    const_reverse_iterator rend() const noexcept;
    const_reverse_iterator crend() const noexcept;
	

private:
	char* s_;
	size_t tam_;	

};

Cadena operator +(const Cadena& op1,const Cadena& op2);
Cadena& operator +=(Cadena& op1,const Cadena& op2);
bool operator ==(const Cadena& cad1, const Cadena& cad2);
bool operator !=(const Cadena& cad1, const Cadena& cad2);
bool operator >(const Cadena& cad1, const Cadena& cad2);
bool operator <(const Cadena& cad1, const Cadena& cad2);
bool operator >=(const Cadena& cad1, const Cadena& cad2);
bool operator <=(const Cadena& cad1, const Cadena& cad2);

std::istream& operator >>(std::istream& is, Cadena& cadena);
std::ostream& operator <<(std::ostream& os, const Cadena& cadena);

namespace std 
{ // Estaremos dentro del espacio de nombres std
	template <> // Es una especialización de una plantilla para Cadena
	struct hash<Cadena> 
	{ // Es una clase con solo un operador publico
		size_t operator() (const Cadena& cad) const // el operador función
		{
			hash<string> hs; // creamos un objeto hash de string
			const char* p = cad.c_str(); // obtenemos la cadena de la Cadena
			string s(p); // creamos un string desde una cadena
			size_t res = hs(s); // el hash del string. Como hs.operator()(s);
			return res; // devolvemos el hash del string
		}
	};
}

#endif

