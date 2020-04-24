#include "BusquedaMD.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// BusquedaMD
BusquedaMD::BusquedaMD(const Grafo* grafo) //core
{
	this->_grafo = std::make_unique<const Grafo*>(grafo);
	
}

double BusquedaMD::md(const Grafo* grafo , const SolucionMD* const S ) const //core
{
	std::unordered_set<Arista, AristaHash, AristaComp> visitados;
	double sumatorio = 0;
	size_t numNodos = S->getListaVertices().size();
	for (int i = 0, id1 = (*S)[i]; i < numNodos; i++, id1 = (*S)[i])
		for (int j = 0, id2 = (*S)[j]; j < numNodos; j++, id2 = (*S)[j])
			if( visitados.insert(grafo->getVertices().at(id1)->getAristas().at(id2)).second )
				sumatorio += grafo->getVertices().at(id1)->getAristas().at(id2).coste;
			
	/*for (const Arista& arista : S->getListaAristas())//Mal
		sumatorio += arista.coste;*/
	visitados.clear();
	return (sumatorio / numNodos);
}

SolucionMD* BusquedaMD::busquedaMejor(SolucionMD* sol ) const
{
	const Grafo* grafo = *this->_grafo.get();
	SolucionMD* S = (sol == nullptr) ? primeraMejorSolucion(*this->_grafo.get()) : sol;
	SolucionMD* Sx = nullptr;
	do {
		Sx = algoritmo(grafo, S);
	} while(condicionParada(S,Sx));

	Sx->setScore(this->md(grafo, Sx));
	std::cout << "La dispersión media maxima es = " << Sx->getScore() << std::endl;
	return Sx;
}
/// END
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// BusquedaMDVorazConstructiva
SolucionMD* BusquedaMDVorazConstructiva::primeraMejorSolucion(const Grafo* grafo) const//Voraz
{
	return new SolucionMD(grafo->getMax().idNodo1, grafo->getMax().idNodo2);
}

SolucionMD* BusquedaMDVorazConstructiva::algoritmo(const Grafo* grafo, SolucionMD* S) const
{
	double solMejorUlt = md(grafo, S);
	SolucionMD* Sx = new SolucionMD(*S);
	for (const int& id : S->getListaVertices()) {
		const Grafo::Vertice* vertAct = grafo->getVertices().at(id);
		for (auto it_id_arist : vertAct->getAristas()) {
			if (!Sx->isPresent(it_id_arist.first)) { // Si no esta presente lo analizamos
				SolucionMD* temp = (*S + it_id_arist.first);
				double solActual = md(grafo, temp);
				if (solActual >= solMejorUlt) {
					solMejorUlt = solActual;
					delete Sx;
					Sx = temp;
				}
				else delete temp;
			}
		}		
	}
	return Sx;
}

const bool BusquedaMDVorazConstructiva::condicionParada(SolucionMD*& S, SolucionMD* Sx) const
{
	bool result = !(*S == *Sx);
	if (result) { //Actualizamos el original si resulta que son distintos
		delete S;
		S = Sx;
	}
	return result;
}
/// END
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// BusquedaMDVorazDestructiva

SolucionMD* BusquedaMDVorazAlternativa::primeraMejorSolucion(const Grafo* grafo) const
{
	SolucionMD* sol = new SolucionMD();
	for (auto& it_id_vert : grafo->getVertices())
		sol->addVertice(it_id_vert.first);
		
	return sol;
}

SolucionMD* BusquedaMDVorazAlternativa::algoritmo(const Grafo* grafo, SolucionMD* S) const
{
	double solMejorUlt = md(grafo, S);
	SolucionMD* Sx = new SolucionMD(*S);
	for (const int& id : S->getListaVertices()) {
		const Grafo::Vertice* vertAct = grafo->getVertices().at(id);
		for (auto it_id_arist : vertAct->getAristas()) {
			if (Sx->isPresent(it_id_arist.first)) { // Si no esta presente lo analizamos
				SolucionMD* temp = (*S - it_id_arist.first);
				double solActual = md(grafo, temp);
				if (solActual >= solMejorUlt) {
					solMejorUlt = solActual;
					delete Sx;
					Sx = temp;
				}
				else delete temp;
			}
		}
	}
	return Sx;
}

const bool BusquedaMDVorazAlternativa::condicionParada(SolucionMD*& S, SolucionMD* Sx) const
{
	bool result = !(*S == *Sx); 
	if (result) { //Actualizamos el original si resulta que son distintos
		delete S;
		S = Sx;
	}
	return result;
}
/// END
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// BusquedaMDGRASP
BusquedaMDGRASP::BusquedaMDGRASP(const Grafo* grafo)
{
	
}

SolucionMD* BusquedaMDGRASP::primeraMejorSolucion(const Grafo* grafo) const
{
	return nullptr;
}

SolucionMD* BusquedaMDGRASP::algoritmo(const Grafo* grafo, SolucionMD* S) const
{
	return nullptr;
}

const bool BusquedaMDGRASP::condicionParada(SolucionMD*& S, SolucionMD* Sx) const
{
	return false;
}

/// END
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////