#include "includes.h"

#ifndef BUSQUEDAMD_H
#define BUSQUEDAMD_H 1



#pragma warning( push )
#pragma warning( disable : 4244 )
#pragma warning( disable : 4267 )


class BusquedaMD {
protected:
	size_t _iteracion = 0;
	size_t _bestIteracion = 0;

	std::unique_ptr<const Grafo*> _grafo;

	SolucionMD* generarSolucionAleatoria(const Grafo*);
public:
	explicit BusquedaMD(const Grafo* grafo);
	explicit BusquedaMD(const BusquedaMD& other) = delete;

	double funcionObjetivo(const Grafo* grafo , const SolucionMD* const S) const; //md
	SolucionMD* busquedaMejor(SolucionMD* sol = nullptr) ;

	virtual SolucionMD* primeraMejorSolucion(const Grafo* grafo)  = 0;
	virtual SolucionMD* algoritmo(const Grafo* grafo, const SolucionMD* const S)  = 0;
	virtual const bool  condicionParada(SolucionMD*& S,SolucionMD* Sx)  = 0;

};

class BusquedaMDVorazConstructiva : public BusquedaMD {
public:
	explicit BusquedaMDVorazConstructiva(const Grafo* grafo) : BusquedaMD(grafo){}
	explicit BusquedaMDVorazConstructiva(const BusquedaMDVorazConstructiva& other) = delete;

	virtual SolucionMD* primeraMejorSolucion(const Grafo* grafo)  override;
	virtual SolucionMD* algoritmo(const Grafo* grafo, const SolucionMD* const S)  override;
	virtual const bool  condicionParada(SolucionMD*& S, SolucionMD* Sx)  override;
};

class BusquedaMDVorazAlternativa : public BusquedaMD {
public:
	explicit BusquedaMDVorazAlternativa(const Grafo* grafo) : BusquedaMD(grafo) {}
	explicit BusquedaMDVorazAlternativa(const BusquedaMDVorazAlternativa& other) = delete;

	virtual SolucionMD* primeraMejorSolucion(const Grafo* grafo)  override;
	virtual SolucionMD* algoritmo(const Grafo* grafo, const SolucionMD* const S)  override;
	virtual const bool  condicionParada(SolucionMD*& S, SolucionMD* Sx)  override;
};


class BusquedaEntornoLocal {
	const Grafo* _grafo;
	const SolucionMD* _solucion;
	const BusquedaMD* _busqueda;
public:
	BusquedaEntornoLocal() = delete;
	BusquedaEntornoLocal(const BusquedaEntornoLocal&) = delete;
	BusquedaEntornoLocal(const Grafo* grafo, const SolucionMD* solucion, const BusquedaMD* busqueda) : _grafo(grafo), _solucion(solucion), _busqueda(busqueda) {}

	SolucionMD* operator()();
};


class BusquedaMDGRASP : public BusquedaMD {
protected:
	const size_t _itSinMejora = 0;
	size_t _countItSinMejora = 0;
	size_t _lrcSize = 0;
	float _lrcDelta = 0;

	virtual SolucionMD* faseConstructiva(const Grafo*, SolucionMD* S);
	virtual int* construirLRC(const Grafo*, SolucionMD* S);

public:
	explicit BusquedaMDGRASP(const Grafo* grafo, const float delta, const size_t itSinMejora = 500) : BusquedaMD(grafo), _lrcDelta(delta) , _itSinMejora(itSinMejora)
	{ 
		if (_lrcDelta < 0 || _lrcDelta > 1) throw std::exception("LRCDelta debe ser un valor entre 0 y 1");
		else _lrcSize = round(grafo->getNumVertices() * _lrcDelta);
	}; //Descartado Preprocesamiento.
	explicit BusquedaMDGRASP(const BusquedaMDGRASP& other) = delete;

	virtual SolucionMD* primeraMejorSolucion(const Grafo* grafo) override;
	virtual SolucionMD* algoritmo(const Grafo* grafo, const SolucionMD* const S)  override;
	virtual const bool  condicionParada(SolucionMD*& S, SolucionMD* Sx)  override;
};


class BusquedaMDMultiArranque : public BusquedaMD {
protected:
	const size_t _itSinMejora = 0;
	size_t _countItSinMejora = 0;
public:
	explicit BusquedaMDMultiArranque(const Grafo* grafo, const size_t itSinMejora = 500) : BusquedaMD(grafo), _itSinMejora(itSinMejora) {}
	explicit BusquedaMDMultiArranque(const BusquedaMDMultiArranque& other) = delete;

	virtual SolucionMD* primeraMejorSolucion(const Grafo* grafo)  override;
	virtual SolucionMD* algoritmo(const Grafo* grafo, const SolucionMD* const S)  override;
	virtual const bool  condicionParada(SolucionMD*& S, SolucionMD* Sx)  override;
};


class BusquedaMDVNS : public BusquedaMD {
protected:
	const size_t _itSinMejora = 0;
	size_t _countItSinMejora = 0;

	std::vector<SolucionMD*>* _entornos = nullptr;
	size_t _maxEntornosSize = UINT_MAX;

	std::vector<SolucionMD*>* generarEntornos(const Grafo* grafo, const SolucionMD* S);
	SolucionMD* agitarEntorno(const Grafo* grafo, const SolucionMD* const entorno);
public:
	~BusquedaMDVNS() { this->_entornos->clear(); delete this->_entornos; }
	explicit BusquedaMDVNS(const Grafo* grafo, const size_t itSinMejora = 50, const size_t maxEntornosSize = UINT_MAX) :
		BusquedaMD(grafo), _itSinMejora(itSinMejora), _maxEntornosSize(maxEntornosSize){}
	explicit BusquedaMDVNS(const BusquedaMDVNS& other) = delete;

	virtual SolucionMD* primeraMejorSolucion(const Grafo* grafo)  override;
	virtual SolucionMD* algoritmo(const Grafo* grafo, const SolucionMD* const S)  override;
	virtual const bool  condicionParada(SolucionMD*& S, SolucionMD* Sx)  override;
};

#endif // !BUSQUEDAMD_H
