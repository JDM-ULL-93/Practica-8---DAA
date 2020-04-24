#include "includes.h"

#ifndef BUSQUEDAMD_H
#define BUSQUEDAMD_H 1


class BusquedaMD {
protected:
	std::unique_ptr<const Grafo*> _grafo;
	explicit BusquedaMD() {};
public:
	explicit BusquedaMD(const Grafo* grafo);
	explicit BusquedaMD(const BusquedaMD& other) = delete;

	double md(const Grafo* grafo , const SolucionMD* S) const;
	SolucionMD* busquedaMejor(SolucionMD* sol = nullptr) const;

	virtual SolucionMD* primeraMejorSolucion(const Grafo* grafo) const = 0;
	virtual SolucionMD* algoritmo(const Grafo* grafo, SolucionMD* S) const = 0;
	virtual const bool  condicionParada(SolucionMD*& S,SolucionMD* Sx) const = 0;

};

class BusquedaMDVorazConstructiva : public BusquedaMD {
public:
	explicit BusquedaMDVorazConstructiva(const Grafo* grafo) : BusquedaMD(grafo){}
	explicit BusquedaMDVorazConstructiva(const BusquedaMDVorazConstructiva& other) = delete;

	virtual SolucionMD* primeraMejorSolucion(const Grafo* grafo) const override;
	virtual SolucionMD* algoritmo(const Grafo* grafo, SolucionMD* S) const override;
	virtual const bool  condicionParada(SolucionMD*& S, SolucionMD* Sx) const override;
};

class BusquedaMDVorazAlternativa : public BusquedaMD {
public:
	explicit BusquedaMDVorazAlternativa(const Grafo* grafo) : BusquedaMD(grafo) {}
	explicit BusquedaMDVorazAlternativa(const BusquedaMDVorazAlternativa& other) = delete;

	virtual SolucionMD* primeraMejorSolucion(const Grafo* grafo) const override;
	virtual SolucionMD* algoritmo(const Grafo* grafo, SolucionMD* S) const override;
	virtual const bool  condicionParada(SolucionMD*& S, SolucionMD* Sx) const override;
};


class BusquedaMDGRASP : public BusquedaMD {
public:
	explicit BusquedaMDGRASP(const Grafo* grafo);
	explicit BusquedaMDGRASP(const BusquedaMDGRASP& other) = delete;

	virtual SolucionMD* primeraMejorSolucion(const Grafo* grafo) const override;
	virtual SolucionMD* algoritmo(const Grafo* grafo, SolucionMD* S) const override;
	virtual const bool  condicionParada(SolucionMD*& S, SolucionMD* Sx) const override;
};


#endif // !BUSQUEDAMD_H
