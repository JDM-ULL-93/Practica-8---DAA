#include "SolucionMD.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// SolucionMD
SolucionMD::SolucionMD(const int& idVertice1, const int& idVertice2)
{
	_vertices.insert(idVertice1); 
	_vertices.insert(idVertice2);
}

SolucionMD::SolucionMD(const SolucionMD& other)
{
	_vertices.insert(other._vertices.begin(), other._vertices.end());
	_score = other._score;
}

const int SolucionMD::operator[](const int& indice) const
{
	//assert(indice >= 0 && indice < _vertices.size());
	if (indice < 0 || indice >= _vertices.size()) return -1;
	return *std::next(_vertices.begin(), indice);
}

const bool operator==(const SolucionMD& s1, const SolucionMD& s2)
{
	if (s1.size() != s2.size()) return false;
	for (int i = 0; i < s1.size(); i++)
		if (s1[i] != s2[i]) return false;
	return true;
}

const bool SolucionMD::addVertice(const int& idVertice)
{
	_score = -INFINITY;
	return this->_vertices.insert(idVertice).second; //true si era nuevo id en la lista
}
SolucionMD* operator+(const SolucionMD& s, const int& idVert)
{
	SolucionMD* newSol = new SolucionMD(s);
	if (!newSol->addVertice(idVert)) throw std::exception("El nodo a introducir ya existe en el original");
	return newSol;
}

const bool SolucionMD::removeVertice(const int& idVertice)
{
	_score = -INFINITY;
	return this->_vertices.erase(idVertice); // false si se han eliminado 0 elementos
}
SolucionMD* operator-(const SolucionMD& s,const int& idVert)
{
	SolucionMD* newSol = new SolucionMD(s);
	if (!newSol->removeVertice(idVert)) throw std::exception("El nodo a eliminar no existia previamente en el original");
	return newSol;
}

/// END
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////