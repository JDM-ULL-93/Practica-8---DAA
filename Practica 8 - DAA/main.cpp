// Practica 8 - DAA.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include "includes.h"



int main(int argc, char* argv[])
{
    std::ifstream input (argv[1], std::ifstream::in | std::ifstream::binary);
    Grafo* grafo = new Grafo(input);
    std::cout << *grafo << std::endl;
    std::cout << std::endl;
    BusquedaMD* busqueda = nullptr;
    busqueda = new BusquedaMDVorazConstructiva(grafo);
    busqueda->busquedaMejor();
    delete busqueda;
    busqueda = new BusquedaMDVorazAlternativa(grafo);
    busqueda->busquedaMejor();
    delete busqueda;
    busqueda = new BusquedaMDGRASP(grafo, 0.4f);
    busqueda->busquedaMejor();
    delete busqueda;
    busqueda = new BusquedaMDMultiArranque(grafo);
    busqueda->busquedaMejor();
    delete busqueda;
    busqueda = new BusquedaMDVNS(grafo);
    busqueda->busquedaMejor();
    delete busqueda;
    
    system("pause");
    
    //std::cout << "El programa ha finalizado y se cerrará en 5 segundos ..." << std::endl;
    //std::this_thread::sleep_for(std::chrono::seconds(5));
    return 0;
}

// Ejecutar programa: Ctrl + F5 o menú Depurar > Iniciar sin depurar
// Depurar programa: F5 o menú Depurar > Iniciar depuración

// Sugerencias para primeros pasos: 1. Use la ventana del Explorador de soluciones para agregar y administrar archivos
//   2. Use la ventana de Team Explorer para conectar con el control de código fuente
//   3. Use la ventana de salida para ver la salida de compilación y otros mensajes
//   4. Use la ventana Lista de errores para ver los errores
//   5. Vaya a Proyecto > Agregar nuevo elemento para crear nuevos archivos de código, o a Proyecto > Agregar elemento existente para agregar archivos de código existentes al proyecto
//   6. En el futuro, para volver a abrir este proyecto, vaya a Archivo > Abrir > Proyecto y seleccione el archivo .sln
