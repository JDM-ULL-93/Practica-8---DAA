
#include "includes.h"

#ifndef GRAFO_H
#define GRAFO_H 1


#ifndef VERBOSE
#define VERBOSE 0
#endif // !VERBOSE




class GrafoReadingException : std::exception {
public:
	GrafoReadingException(const char* message) : std::exception(message) {}
};


struct Arista {
public:
	explicit Arista() {} //Invalido
	explicit Arista(int idNodo1, int idNodo2, int coste) : idNodo1(idNodo1), idNodo2(idNodo2), coste(coste) {} //Nuevo
	explicit Arista(const Arista& other) : idNodo1(other.idNodo1), idNodo2(other.idNodo2), coste(other.coste) {}//Copia
	int idNodo1 = -1;
	int idNodo2 = -1;
	int coste = 0;

	friend const bool operator ==(const Arista& a1, const Arista& a2) { return ((a1.idNodo1 == a2.idNodo1 && a1.idNodo2 == a2.idNodo2) || (a1.idNodo1 == a2.idNodo2 && a1.idNodo2 == a2.idNodo1)); }
	friend const bool operator !=(const Arista& a1, const Arista& a2) { return (a1.idNodo1 != a2.idNodo1 || a1.idNodo2 != a2.idNodo2); }

	bool operator <(int otroCoste) { return coste < otroCoste; }
	bool operator >(int otroCoste) { return coste > otroCoste; }
};



class Grafo {
public:
	//////////////////////////////////////////////////////////////////////
	/// Vertice
	class Vertice {
	protected:
		int _id = -1;
		std::shared_ptr<const Grafo*> _grafo;
		std::map<const int,const Arista> _aristas; // id vertice_vecino -> coste_viaje
	public:
		~Vertice();
		explicit Vertice(const std::shared_ptr<const Grafo*>&, const int);
		explicit Vertice(const Vertice& other) = delete; //Prohibido copias de Vertice

		void addVecino(const int id, const Arista& arista);
		const std::map<const int, const Arista> getAristas() const { return this->_aristas; }
	};
	///
	//////////////////////////////////////////////////////////////////////
protected:
	size_t _numVertices = -1; //recordar size_t es unsigned
	std::map<const int, Vertice*> _vertices; // id_vertice -> Vertice

	//Pre-Analisís
	Arista _minimo = Arista(-1, -1, INT_MAX);
	Arista _maximo = Arista(-1, -1, -INT_MAX);

public:
	~Grafo();
	explicit Grafo(std::istream& input, bool debug = VERBOSE);
	explicit Grafo(const Grafo& other) = delete; //Prohibido copias de Grafo.

	const Arista& getMin() const { return this->_minimo; }
	const Arista& getMax() const { return this->_maximo; }
	const size_t& getNumVertices() const { return this->_numVertices; }
	const std::map<const int, Vertice*>& getVertices() const { return this->_vertices; }

	friend std::ostream& operator <<(std::ostream& out, const Grafo& grafo);
};



#endif