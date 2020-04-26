#include "BusquedaMD.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// BusquedaMD
BusquedaMD::BusquedaMD(const Grafo* grafo) //core
{
	this->_grafo = std::make_unique<const Grafo*>(grafo);
	_iteracion = 0;
	_bestIteracion = 0;
}

double BusquedaMD::funcionObjetivo(const Grafo* grafo , const SolucionMD* const S ) const //core
{
	std::unordered_set<Arista, AristaHash, AristaEqual> visitados;
	double sumatorio = 0;
	size_t numNodos = S->getListaVertices().size();
#pragma omp parallel for
	for (int i = 0; i < numNodos; i++) {
		int id1 = (*S)[i];
		for (int j = 0, id2 = (*S)[j]; j < numNodos; j++, id2 = (*S)[j]) {
			const auto aristas = grafo->getVertices().at(id1)->getAristas();
			const auto it = aristas.find(id2);
#pragma omp critical
			{
				if (it != aristas.end())
					if (visitados.insert(it->second).second)
						sumatorio += it->second.coste;
			}

		}
	}

	visitados.clear();
	return (sumatorio / numNodos);
}

SolucionMD* BusquedaMD::busquedaMejor(SolucionMD* sol ) 
{
	const Grafo* grafo = *this->_grafo.get();
	SolucionMD* S = (sol == nullptr) ? primeraMejorSolucion(*this->_grafo.get()) : sol;
	SolucionMD* Sx = nullptr;
	do {
		Sx = algoritmo(grafo, S);
		Sx->setScore(this->funcionObjetivo(grafo, Sx));
	} while(condicionParada(S,Sx));

	
	std::cout << "La dispersion media maxima es = " << S->getScore() << ". Encontrada en iteracion " << _bestIteracion << " (Total iteraciones = " << _iteracion << ")" << std::endl;
	return S;
}

SolucionMD* BusquedaMD::generarSolucionAleatoria(const Grafo* grafo)
{
	//Nuestra primera solucion sera una formada totalmente al azar
	srand(time(0));
	SolucionMD* S = new SolucionMD();
	S->addVertice(rand() % grafo->getNumVertices()); //Añadimos un vertice(de inicio) de los posibles de forma aleatoria
	size_t numIter = rand() % grafo->getNumVertices() + 2; //Aleatorio entre [2, grafo->getNumVertices() )
#pragma omp parallel for
	for (int i = numIter; i < numIter; i++) {
		int id = -1;
		while (S->isPresent(id = rand() % grafo->getNumVertices()));//Seleccionamos un vertice al azar que no este ya presente.
#pragma omp critical
		{
			S->addVertice(id); // Y se lo añadimos a nuestra solución actual.
		}
	}
	return S;
}
/// END
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////






















/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// BusquedaMDVorazConstructiva
SolucionMD* BusquedaMDVorazConstructiva::primeraMejorSolucion(const Grafo* grafo) //Voraz
{
	return new SolucionMD(grafo->getMax().idNodo1, grafo->getMax().idNodo2);
}

SolucionMD* BusquedaMDVorazConstructiva::algoritmo(const Grafo* grafo, const SolucionMD* const S) 
{
	double solMejorUlt = funcionObjetivo(grafo, S);
	SolucionMD* Sx = new SolucionMD(*S);
	for (const int& id : S->getListaVertices()) {
		const Grafo::Vertice* vertAct = grafo->getVertices().at(id);
		for (auto it_id_arist : vertAct->getAristas()) {
			if (!Sx->isPresent(it_id_arist.first)) { // Si no esta presente lo analizamos
				SolucionMD* temp = (*S + it_id_arist.first);
				double solActual = funcionObjetivo(grafo, temp);
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

const bool BusquedaMDVorazConstructiva::condicionParada(SolucionMD*& S, SolucionMD* Sx) 
{
	_iteracion++;
	bool result = !(*S == *Sx);
	if (result) { //Actualizamos el original si resulta que son distintos
		delete S;
		S = Sx;
		_bestIteracion = _iteracion;
	}
	return result;
}
/// END
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////













/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// BusquedaMDVorazAlternativa

SolucionMD* BusquedaMDVorazAlternativa::primeraMejorSolucion(const Grafo* grafo) 
{
	SolucionMD* sol = new SolucionMD();
	for (auto& it_id_vert : grafo->getVertices())
		sol->addVertice(it_id_vert.first);

	return sol;
}

SolucionMD* BusquedaMDVorazAlternativa::algoritmo(const Grafo* grafo, const SolucionMD* const S) 
{
	double solMejorUlt = funcionObjetivo(grafo, S);
	SolucionMD* Sx = new SolucionMD(*S);
	for (const int& id : S->getListaVertices()) {
		const Grafo::Vertice* vertAct = grafo->getVertices().at(id);
		for (auto it_id_arist : vertAct->getAristas()) {
			if (Sx->isPresent(it_id_arist.first)) { // Si no esta presente lo analizamos
				SolucionMD* temp = (*S - it_id_arist.first);
				double solActual = funcionObjetivo(grafo, temp);
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

const bool BusquedaMDVorazAlternativa::condicionParada(SolucionMD*& S, SolucionMD* Sx) 
{
	_iteracion++;
	bool result = !(*S == *Sx); 
	if (result) { //Actualizamos el original si resulta que son distintos
		delete S;
		S = Sx;
		_bestIteracion = _iteracion;
	}
	return result;
}
/// END
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////










/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// BusquedaEntornoLocal

SolucionMD* BusquedaEntornoLocal::operator()()
{
	bool mejora;
	SolucionMD* result = new SolucionMD(*_solucion);
	do {
		mejora = false;
		// Movimiento -> Sacar vertice con menor afinidad
		// Obtenemos el vertice con menor afinidad (aquel que sacarlo produce la menor diferencia con el resultado actual)
		const std::set<int>& sol = result->getListaVertices();
		double actualMD = _busqueda->funcionObjetivo(_grafo, result);
		double min = INFINITY;
		int minId = -1;
#pragma omp parallel for
		for (int i = 0; i < sol.size(); i++) {
			SolucionMD* temp = (*result - (*result)[i]);
			double diff = (actualMD - _busqueda->funcionObjetivo(_grafo, temp));
			delete temp;
#pragma omp critical
			{
				if (diff < min) {
					minId = (*_solucion)[i];
					min = diff;
				}
			}
			
		}
		SolucionMD* test = new SolucionMD(*result);
		test->removeVertice(minId);
		//Movimiento -> Añadir un vertice que mejore la solución inicial 
		//Ahora buscar por aquellos vertices que no se habían probado anteriormente, buscando uno que devuelva una mejor solucion.
		for (const auto& id_vert : _grafo->getVertices()) {
			if (!result->isPresent(id_vert.first)) {
				SolucionMD* temp = (*test + id_vert.first);
				double ahoraMD = _busqueda->funcionObjetivo(_grafo, temp);
				if (ahoraMD > actualMD) { 
					delete result;
					result = temp;
					actualMD = ahoraMD;
					mejora = true;
					//break;
				}
				else delete temp;
			}
		}
	} while (mejora);
	
	return result;
}

/// END
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////










/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// BusquedaMDGRASP

SolucionMD* BusquedaMDGRASP::primeraMejorSolucion(const Grafo* grafo) //Creamos LRC
{
	_countItSinMejora = 0;
	return new SolucionMD();
}

SolucionMD* BusquedaMDGRASP::algoritmo(const Grafo* grafo, const SolucionMD* const S) 
{
	SolucionMD* Sx = new SolucionMD(); //Importante no modificar el original que más tarde será usado para comparar en condición de parada
	Sx = faseConstructiva(grafo, Sx);
	Sx = BusquedaEntornoLocal(grafo, Sx, this)();
	return Sx;
}


SolucionMD* BusquedaMDGRASP::faseConstructiva(const Grafo* grafo, SolucionMD* S)
{
	srand(time(0)+ grafo->getNumAristas());
	S->addVertice(rand() % grafo->getNumVertices()); //Añadimos un vertice, de los posibles, de forma aleatoria
	size_t numIter = (rand() % grafo->getNumVertices()) + 2; //Aleatorio entre [2, grafo->getNumVertices() )
#pragma omp parallel for
	for (int i = numIter; i < numIter; i++) {
		int* LRC =  construirLRC(grafo, S); //Lista de vertices ordenados de mayor a menor afinidad (De Mayor a Menor resultado en función objetivo)
#pragma omp critical
		{
			S->addVertice(LRC[rand() % _lrcSize]); //Seleccionamos un vertice al azar de los presentes en LRC y los añadimos a la solución
		}
		delete[] LRC;
	}
	return S;
}



int* BusquedaMDGRASP::construirLRC(const Grafo* grafo, SolucionMD* S)
{
	double* afinidades = new double[grafo->getNumVertices()](); //Cada indice I del array corresponde con el vertice de id I. El contenido con el valor md(S + I)
#pragma omp parallel for
	for (int i = 0; i < grafo->getNumVertices(); i++) {
		if (!S->isPresent(i)) afinidades[i] = funcionObjetivo(grafo, *S + i);
		else afinidades[i] = -INFINITY;
	}
	
	
	int* result = new int[_lrcSize]();
#pragma omp parallel for
	for (int i = 0; i < _lrcSize; i++) { //Ahora nos dedicamos a meter en orden de mayor a menor, los lrcSize vertices con mayor afinidad del total
		double max = -INFINITY;
		int idMax = -1;
		for (const auto& id_vert : grafo->getVertices()) {
			if (afinidades[id_vert.first] > max) {
				max = afinidades[id_vert.first];
				idMax = id_vert.first;
			}
		}
		result[i] = idMax;
		afinidades[idMax] = -INFINITY; //Nos aseguramos de que no volverá a ser nominado.
	}
	delete[] afinidades;
	return result;
}

const bool BusquedaMDGRASP::condicionParada(SolucionMD*& S, SolucionMD* Sx)
{
	_iteracion++;
	bool result = !(*S == *Sx);
	if (result) { //Actualizamos el original si resulta que son distintos y es peor que el actual
		if (Sx->getScore() > S->getScore()) { //Mejora
			delete S;
			S = Sx;
			_countItSinMejora = 0;
			_bestIteracion = _iteracion;
		}
		else {
			_countItSinMejora += 1;
			delete Sx;
		}
	}
	else _countItSinMejora += 1;
	return (_countItSinMejora < _itSinMejora);
}
/// END
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////








/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// BusquedaMDMultiArranque

SolucionMD* BusquedaMDMultiArranque::primeraMejorSolucion(const Grafo* grafo)
{
	_countItSinMejora = 0;
	
	return generarSolucionAleatoria(grafo);
}

SolucionMD* BusquedaMDMultiArranque::algoritmo(const Grafo* grafo, const SolucionMD* const S)
{
	return BusquedaEntornoLocal(grafo, this->generarSolucionAleatoria(grafo), this)();
}

const bool BusquedaMDMultiArranque::condicionParada(SolucionMD*& S, SolucionMD* Sx)
{
	_iteracion++;
	bool result = !(*S == *Sx);
	if (result) { //Actualizamos el original si resulta que son distintos y es peor que el actual
		if (Sx->getScore() > S->getScore()) {
			delete S;
			S = Sx;
			_countItSinMejora = 0;
			_bestIteracion = _iteracion;
		}
		else {
			_countItSinMejora += 1;
			delete Sx;
		}
	}
	else _countItSinMejora += 1;
	return (_countItSinMejora < _itSinMejora);
}
/// END
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////














/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// BusquedaMDVNS
std::vector<SolucionMD*>* BusquedaMDVNS::generarEntornos(const Grafo* grafo, const SolucionMD* S)
{
	std::vector<SolucionMD*>* result = new std::vector<SolucionMD*>();
	result->reserve(S->getListaVertices().size());

	srand(time(0));
	for (const auto& id_vert : grafo->getVertices()) {
		if (result->size() == _maxEntornosSize) break;
		if (!S->isPresent(id_vert.first))
			for (const int& id : S->getListaVertices()) {
				if (result->size() == _maxEntornosSize) break;
				result->push_back((*(*S - id) + id_vert.first)); //Creamos una solución alternativa donde vamos intercambiando un vertice presente por otro no presente
			}
	}	
	return result;
}

SolucionMD* BusquedaMDVNS::primeraMejorSolucion(const Grafo* grafo)
{
	//Nuestra primera solucion sera una formada totalmente al azar
	srand(time(0) + grafo->getNumAristas());
	SolucionMD* S = new SolucionMD();
	S->addVertice(rand() % grafo->getNumVertices()); //Añadimos un vertice(de inicio) de los posibles de forma aleatoria
	size_t numIter = rand() % (grafo->getNumVertices()-1) + 2; //Aleatorio entre [2, grafo->getNumVertices()-1 )
#pragma omp parallel for
	for (int i = numIter; i < numIter; i++) {
		int id = -1;
		while (S->isPresent(id = rand() % grafo->getNumVertices()));//Seleccionamos un vertice al azar que no este ya presente.
		S->addVertice(id); // Y se lo añadimos a nuestra solución actual.
	}

	this->_entornos = generarEntornos(grafo, S);

	return S;
}

SolucionMD* BusquedaMDVNS::agitarEntorno(const Grafo* grafo, const SolucionMD* const entorno)
{
	srand(time(0));
	SolucionMD* result = new SolucionMD(*entorno);
	//Seleccionamos , de forma aleatoria, vertices distintos.
	int id1 = rand() % grafo->getNumVertices();
	int id2; while (id1 == (id2 = rand() % grafo->getNumVertices()));

	//Metemos/Sacamos uno y metemos/sacamos el otro
	if (result->isPresent(id1)) {
		if (result->isPresent(id2))
			result->removeVertice(id1);
		else {
			result->removeVertice(id1);
			result->addVertice(id2);
		}
	}
	else {
		if (result->isPresent(id2)) {
			result->removeVertice(id2);
			result->addVertice(id1);
		}
		else
			result->addVertice(id2);
	}
	return result;
}


SolucionMD* BusquedaMDVNS::algoritmo(const Grafo* grafo, const SolucionMD* const S)
{
	SolucionMD* Sx = new SolucionMD(*S);
	for (int i = 0; i < _entornos->size();_iteracion++) {
		SolucionMD* temp = BusquedaEntornoLocal(grafo, agitarEntorno(grafo,(*_entornos)[i]), this)();
		if (funcionObjetivo(grafo, temp) > funcionObjetivo(grafo, Sx)) {
			delete Sx;
			Sx = temp;
			i = 0;
		}
		else {
			delete temp;
			i++;
		}
	}
	return Sx;
}


const bool BusquedaMDVNS::condicionParada(SolucionMD*& S, SolucionMD* Sx)
{
	_iteracion++;
	bool result = !(*S == *Sx);
	if (result) { //Actualizamos el original si resulta que son distintos y es peor que el actual
		_countItSinMejora = 0;
		_bestIteracion = _iteracion;
		delete S;
		S = Sx;
	}
	else _countItSinMejora += 1;
	return (_countItSinMejora < _itSinMejora);
}
/// END
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////