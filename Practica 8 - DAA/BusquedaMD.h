#include "includes.h"

#ifndef BUSQUEDAMD_H
#define BUSQUEDAMD_H 1

class SolucionMD {
protected:
	std::set<int> _vertices;
	std::vector<Arista> _solucion;
	double _mdm = -INFINITY;

	void addArista(const Arista& arista);
public:
	explicit SolucionMD(const Arista arista, const size_t reserve = 100); //Nuevo
	explicit SolucionMD(const SolucionMD& other); //Copia
	
	const std::vector<Arista>& getListaAristas() const { return this->_solucion; }
	const std::set<int>& getListaVertices() const { return this->_vertices; }
	const double getMDM() const { return this->_mdm; }

	void setMDM(double mdm) { this->_mdm = mdm; }

	const Arista& operator [](int indice) const;
	friend const bool operator ==(const SolucionMD& s1, const SolucionMD& s2);
	friend SolucionMD* operator +(const SolucionMD& s, const Arista& arist);
};


class BusquedaMD {
protected:
	std::unique_ptr<const Grafo*> _grafo;
public:
	explicit BusquedaMD(const Grafo* grafo);
	explicit BusquedaMD(const BusquedaMD& other) = delete;

	const bool aristaVisitada(const SolucionMD* S, const Arista& arista) const;
	double md(const Grafo* grafo , const SolucionMD* S) const;
	SolucionMD* busquedaMejor() const;

	virtual SolucionMD* primeraMejorSolucion(const Grafo* grafo) const = 0;
	virtual SolucionMD* algoritmo(const Grafo* grafo, SolucionMD* S) const = 0;
	virtual const bool  condicionParada(SolucionMD*& S,SolucionMD* Sx) const = 0;

};


class BusquedaMDVoraz : public BusquedaMD {
public:
	explicit BusquedaMDVoraz(const Grafo* grafo) : BusquedaMD(grafo){}
	explicit BusquedaMDVoraz(const BusquedaMDVoraz& other) = delete;

	virtual SolucionMD* primeraMejorSolucion(const Grafo* grafo) const override;
	virtual SolucionMD* algoritmo(const Grafo* grafo, SolucionMD* S) const override;
	virtual const bool  condicionParada(SolucionMD*& S, SolucionMD* Sx) const override;

};



#endif // !BUSQUEDAMD_H
