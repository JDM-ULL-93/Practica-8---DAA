#include "includes.h"

#ifndef SOLUCIONMD_H
#define SOLUCIONMD_H 1

class SolucionMD {
protected:
	std::set<int> _vertices;
	//std::vector<Arista> _solucion;
	double _score = -INFINITY;
public:
	explicit SolucionMD(){} //Nuevo vacio
	explicit SolucionMD(const int& vertice1, const int& vertice2); //Nuevo
	explicit SolucionMD(const SolucionMD& other); //Copia

	const std::set<int>& getListaVertices() const { return this->_vertices; }

	const bool addVertice(const int& idVertice);
	const bool removeVertice(const int& idVertice);

	const double getScore() const { return this->_score; }
	void setScore(const double& mdm) { this->_score = mdm; }

	const size_t size() const { return this->_vertices.size(); }
	const bool isPresent(const int& idVert) const { return (this->_vertices.find(idVert) != this->_vertices.end()); }

	const int operator [](const int& indice) const;
	friend const bool operator ==(const SolucionMD& s1, const SolucionMD& s2);
	friend SolucionMD* operator +(const SolucionMD& s, const int& idVert);
	friend SolucionMD* operator -(const SolucionMD& s, const int& idVert);
};
#endif // !SOLUCIONMD_H
