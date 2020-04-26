#include "Grafo.h"

Grafo::~Grafo()
{
    this->_vertices.clear();
    this->_numVertices = UINT_MAX;
}

Grafo::Grafo(std::istream& input, bool debug) // throw(GrafoReadingException)
{
    input.seekg(0, input.end);
    if (input.tellg()) { //Hay contenido
        input.seekg(0, input.beg);
        input >> this->_numVertices;
        if (debug) std::cout << "|V| = " << this->_numVertices << std::endl;

        for (int i = 0; i < this->_numVertices; i++) 
            this->_vertices.emplace(i , new Vertice(std::make_shared<const Grafo*>(this), i));

        int id1 = 0;
        int id2 = id1 + 1;
        try {
            for (int id1 = 0; id1 < this->_numVertices - 1; id1++) {
                for (int id2 = id1 + 1; id2 < this->_numVertices; id2++) {
                    if (input.eof()) throw std::out_of_range("Se ha alcanzado el final del fichero antes de lo esperado. Compruebe que el formato y el numero de aristas definidos son correctos");
                    int coste = 0;
                    input >> coste;
                    if (_maximo < coste) {
                        _maximo.idNodo1 = id1;
                        _maximo.idNodo2 = id2;
                        _maximo.coste = coste;
                    }
                    if ( _minimo > coste) {
                        _minimo.idNodo1 = id1 ;
                        _minimo.idNodo2 = id2;
                        _minimo.coste = coste;
                    }
                    _numAristas++;
                    this->_vertices[id1]->addVecino(id2, Arista(id1,id2,coste));
                    this->_vertices[id2]->addVecino(id1, Arista(id2, id1, coste));
                    if (debug) std::cout << "d(" << id1 << "," << id2 << ") = d(" << id2 << "," << id1 << ") = " << coste << std::endl;
                }
            }
        }
        catch (std::out_of_range& ex) {
            std::cerr << ex.what() << std::endl;
        }

        std::cout << "[Se ha cargado el grafo]" << std::endl;
        input.clear();
    }
    else
        throw GrafoReadingException("Input vacio. No se ha podido cargar ningún Grafo");
}

/*Grafo::Grafo(const std::map<const int, Vertice*>& vertices, const Arista& max, const Arista& min)
{
    this->_numVertices = vertices.size();
    for (const auto it_id_vertice : vertices)
        this->_vertices.emplace(it_id_vertice.first, new Vertice(std::make_shared<const Grafo*>(this), *it_id_vertice.second));
}*/

/*Grafo::Vertice::Vertice(const std::shared_ptr<const Grafo*>& grafo,const Vertice& other)
{
    this->_id = other._id;
    this->_grafo = grafo;
    this->_aristas.insert(other._aristas.begin(), other._aristas.end());
}*/

  
Grafo::Vertice::~Vertice()
{
    this->_aristas.clear();
    this->_id = -2;
}

Grafo::Vertice::Vertice(const std::shared_ptr<const Grafo*>& grafo, const int id)
{
    this->_grafo = grafo;
    this->_id = id;
    //this->_aristas.emplace(this->_id, Arista(id,id,0));
}



void Grafo::Vertice::addVecino(const int id, const Arista& arista)
{
    this->_aristas.emplace(id, arista);
}

std::ostream& operator<<(std::ostream& out, const Grafo& grafo)
{
    for (auto& it_id_vert : grafo.getVertices()) {
        for(auto& it_idVec_arist : it_id_vert.second->getAristas())
            std::cout << "d(" << it_idVec_arist.second.idNodo1 << "," << it_idVec_arist.second.idNodo2 << ") = " << it_idVec_arist.second.coste << std::endl;
        out << "-------------------------" << std::endl;
    }
    return out;
}