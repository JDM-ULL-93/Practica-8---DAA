#include "SolucionMD.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// SolucionMD
SolucionMD::SolucionMD(const int& idVertice1, const int& idVertice2)
{
	//std::cout << "Constructor new" << std::endl;
	//_solucion.reserve(reserve);
	//this->addArista(arista);
	_vertices.insert(idVertice1); 
	_vertices.insert(idVertice2);
}

SolucionMD::SolucionMD(const SolucionMD& other)
{
	//std::cout << "Constructor copia" << std::endl;
	//_solucion.reserve(other._solucion.size());
	//_solucion.insert(_solucion.begin(),other._solucion.begin(), other._solucion.end());
	_vertices.insert(other._vertices.begin(), other._vertices.end());
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