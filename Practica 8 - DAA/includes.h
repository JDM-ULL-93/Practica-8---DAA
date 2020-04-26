#ifndef INCLUDES_H
#define INCLUDES_H 1

/*** EXTERNAL */
//STD
#include <iostream> //Para acceder a std:cin y std::cout 
#include <fstream> //Para acceder a clase ifstrea
#include <memory> //Para acceder a smart pointer (shared_pointer, unique_pointer, ...)
#include <map> //Para acceder a la clase std::map
#include <vector> //Para acceder a la clase std::vector
#include <set> //Para aceder a la clase std::set
#include <unordered_set> //Para acceder a la clase std::unordered_set
#include <chrono> //Diversas funciones de tiempo (como time, std::chrono::seconds, std::chrono::milliseconds, ...)
//#include <assert.h> //Para los asserts (comprobaciones en tiempo de ejecución)

#include <omp.h> //Para acceder a las directivas de OpenMP para programación paralela (añadir flag /openmp a compilador para habilitar soporte)


/*** INTERNAL */
#include "Grafo.h"
#include "SolucionMD.h"
#include "BusquedaMD.h"



#endif // !INCLUDES_H
