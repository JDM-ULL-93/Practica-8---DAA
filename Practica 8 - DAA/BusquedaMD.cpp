#include "BusquedaMD.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// SolucionMD
SolucionMD::SolucionMD(const Arista arista, const size_t reserve) 
{
	//std::cout << "Constructor new" << std::endl;
	_solucion.reserve(reserve);
	this->addArista(arista);
	this->addArista(Arista(arista.idNodo1, arista.idNodo1, 0));
}

SolucionMD::SolucionMD(const SolucionMD& other) 
{
	//std::cout << "Constructor copia" << std::endl;
	_solucion.reserve(other._solucion.size());
	_solucion.insert(_solucion.begin(),other._solucion.begin(), other._solucion.end());
	_vertices.insert(other._vertices.begin(), other._vertices.end());
}

const Arista& SolucionMD::operator[](int indice) const
{
	assert (indice >= 0 || indice < _solucion.size());
	return _solucion[indice];
}

const bool operator==(const SolucionMD& s1, const SolucionMD& s2)
{
	if (s1._solucion.size() != s2._solucion.size()) return false;
	for (int i = 0; i < s1._solucion.size(); i++) {
		bool encontrado = false;
		for (int j = 0; j < s2._solucion.size(); j++) {
			if (s1[i] == s2[j]) {
				encontrado = true;
				break;
			}
		}
		if (!encontrado)
			return false;
	}
	return true;
}

void SolucionMD::addArista(const Arista& arista)
{
	this->_solucion.push_back(arista);
	this->_vertices.insert(arista.idNodo1);
	this->_vertices.insert(arista.idNodo2);
}

SolucionMD* operator+(const SolucionMD& s, const Arista& arist)
{
	SolucionMD* result = new SolucionMD(s);
	result->addArista(arist);
	return result;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Apuntes:

//Representación de mi solución...

//Supongamos que tenemos la siguiente lista de vertices:
//	V = {1,2,3,4,5,6,7,8,9,10}
//La solución, descrita a mano es tal que:
//	Como es un grafo no dirigido y todos estan conectados, da igual donde empezemos. Empezaremos en 0 siempre
//	Del vector 0 al 2  -> md = 10/2 = 5 . Meto en S[0] = 2
//	<-- Error, Despues no miro solo desde el 2, miro tambien desde el 0.
//		Del vector 
//	Del vector 2 al 4  -> md = 10+10/3 = 6.67 . Meto en S[2] = 4
//	Del vector 4 al 5 -> md = 10+10+8/4 = 7 . Meto en S[4] = 5
//	Del vector 5 al 8 -> md = 10+10+8+10/5 = 7.6 . Meto en S[5] = 8
//	Del vector 8 al 7 -> md = 10+10+8+10+9/6 = 7.83... . Meto en S[8] = 7
//	Del vector 7 al 6 -> md = 10+10+8+10+9+10/7 = 8.143 . Meto en S[7] = 6
//	<-- Error, apartir de aqui corta al no poder encontrar una solución que incremente la media de dispersión -->
//	<-- Entonces, S = [2,-1,4,-1,5,8,-1,6,7,-1]
//	Del vector 6 al 3 -> md = 10+10+8+10+9+10+3/8 = 7.5 . Meto en S[6] = 3
//	Del vector 3 al 1 -> md = 10+10+8+10+9+10+3-1/9 = 6.5



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// BusquedaMD
BusquedaMD::BusquedaMD(const Grafo* grafo) //core
{
	this->_grafo = std::make_unique<const Grafo*>(grafo);
	
}
const bool BusquedaMD::aristaVisitada(const SolucionMD* const S, const Arista& arista) const
{
	for (const Arista& arista2 : S->getListaAristas())
		if (arista == arista2) return true;
	return false;
}


double BusquedaMD::md(const Grafo* grafo , const SolucionMD* const S ) const //core
{
	double sumatorio = 0;
	size_t numNodos = S->getListaAristas().size();
	for (const Arista& arista : S->getListaAristas())
		sumatorio += arista.coste;
	return (sumatorio / numNodos);
}

SolucionMD* BusquedaMD::busquedaMejor() const
{
	const Grafo* grafo = *this->_grafo.get();
	SolucionMD* S = primeraMejorSolucion(*this->_grafo.get());
	SolucionMD* Sx = nullptr;
	do {
		Sx = algoritmo(grafo, S);
	} while(condicionParada(S,Sx));

	Sx->setMDM(this->md(grafo, Sx));
	std::cout << "La dispersión media maxima es = " << Sx->getMDM() << std::endl;
	return Sx;
}

// Yo se que la primera mejor solución será aquella donde d(i,j) == maximo global del grafo
// La pregunta es, imaginando un escenario real con un grafo de miles de nodos, ¿debería buscar por todos o solo apartir de unos cuantos?
// Ok, da igual, como para cargar el grafo tengo que leer todos los nodos, calculo desde ahí el maximo y lo busco
// Podría hacerlo más optimo si, además del maximo, guardarse la arista que contiene ese coste en el proceso de lectura del fichero
// En cualquier caso, el problema de tratar con muchos datos se traslada a la carga del fichero.
SolucionMD* BusquedaMDVoraz::primeraMejorSolucion(const Grafo* grafo) const//Voraz
{
	return new SolucionMD(Arista(grafo->getMax()), grafo->getNumVertices());
}

SolucionMD* BusquedaMDVoraz::algoritmo(const Grafo* grafo, SolucionMD* S) const
{
	double solMejorActual = md(grafo, S);
	SolucionMD* Sx = new SolucionMD(*S);
	for (const int& id : S->getListaVertices()) {
		const Grafo::Vertice* vertAct = grafo->getVertices().at(id);
		for (auto& it_idVec_arist : vertAct->getAristas()){
			const Arista& arist = it_idVec_arist.second;
			if (!this->aristaVisitada(Sx, arist)) {
				SolucionMD* temp = (*S + arist);
				double solActual = md(grafo,temp);
				if(solActual >= solMejorActual){
					solMejorActual = solActual;
					delete Sx;
					Sx = temp;
				}
				else delete temp;
			}
		}
	}
	return Sx;
}

const bool BusquedaMDVoraz::condicionParada(SolucionMD*& S, SolucionMD* Sx) const
{
	bool result = !(*S == *Sx);
	if (result) {
		delete S;
		S = Sx;
	}
	return result;
}

