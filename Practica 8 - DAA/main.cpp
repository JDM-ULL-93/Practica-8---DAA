// Practica 8 - DAA.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include "includes.h"

/*
struct Test {
    Test(const int valor) : valor(valor){}
    Test(const Test& other) : valor(other.valor) { std::cout << "En copia" << std::endl; }
    int valor = -1;
};

void test(Test*& a1) { //Un puntero pasado por referencia permite cambiar el propio puntero pasado desde fuera.
    std::cout << a1 << std::endl;
    a1->valor = 5;
}

void test2(Test& a1) { //No se llama a constructor copia
    std::cout << a1.valor << std::endl;
    a1.valor = 5;
}
*/

int main(int argc, char* argv[])
{
    //Test* a = new Test(10);
    //std::cout << a << std::endl;
    //test(a);
    //std::cout << a->valor << std::endl;
    Grafo* grafo = new Grafo(std::cin);
    std::cout << *grafo << std::endl;
    std::cout << std::endl;
    BusquedaMD* busqueda = new BusquedaMDVorazConstructiva(grafo);
    busqueda->busquedaMejor();
    delete busqueda;
    busqueda = new BusquedaMDVorazAlternativa(grafo);
    busqueda->busquedaMejor();
    std::cout << "El programa ha finalizado y se cerrará en 5 segundos ..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(5));
}

// Ejecutar programa: Ctrl + F5 o menú Depurar > Iniciar sin depurar
// Depurar programa: F5 o menú Depurar > Iniciar depuración

// Sugerencias para primeros pasos: 1. Use la ventana del Explorador de soluciones para agregar y administrar archivos
//   2. Use la ventana de Team Explorer para conectar con el control de código fuente
//   3. Use la ventana de salida para ver la salida de compilación y otros mensajes
//   4. Use la ventana Lista de errores para ver los errores
//   5. Vaya a Proyecto > Agregar nuevo elemento para crear nuevos archivos de código, o a Proyecto > Agregar elemento existente para agregar archivos de código existentes al proyecto
//   6. En el futuro, para volver a abrir este proyecto, vaya a Archivo > Abrir > Proyecto y seleccione el archivo .sln
