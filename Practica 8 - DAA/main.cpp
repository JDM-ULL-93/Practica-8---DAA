// Practica 8 - DAA.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include "includes.h"



int main(int argc, char* argv[])
{
    if (argc >= 2) {
        std::ifstream input(argv[1], std::ifstream::in | std::ifstream::binary);
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
    }
    else {
        BusquedaMD* busqueda = nullptr;
        Grafo* grafo = nullptr;
        const size_t size = 3;
        const size_t numAlgoritmos = 5;
        const size_t numRep = 5;
        
        std::chrono::steady_clock::time_point t_start;
        std::chrono::steady_clock::time_point t_end;
        double elapsed_time_ms;

        const char* inputs[size] = { "max-mean-div-10.txt", "max-mean-div-15.txt","max-mean-div-20.txt" };
        
        
        for (int i = 0; i < size;i++) {
            std::ifstream input(inputs[i], std::ifstream::in | std::ifstream::binary);
            grafo = new Grafo(input);
            std::cout << "PROBLEMA ID : " << i << std::endl;
            for (int j = 0; j < numAlgoritmos; j++) {

                for (int rep = 1; rep <= 5; rep++) {
                    std::cout << "\tRepeticion " << rep << std::endl;
                    switch (j) {
                    case 0:
                        std::cout << "\t\tAlgoritmo voraz constructivo" << std::endl;
                        busqueda = new BusquedaMDVorazConstructiva(grafo);
                        t_start = std::chrono::high_resolution_clock::now();
                        busqueda->busquedaMejor();
                        t_end = std::chrono::high_resolution_clock::now();
                        elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end - t_start).count();
                        std::cout << "CPU : " << elapsed_time_ms << " ms" << std::endl;
                        break;
                    case 1:
                        std::cout << "\t\tAlgoritmo voraz alternativo" << std::endl;
                        busqueda = new BusquedaMDVorazAlternativa(grafo);
                        t_start = std::chrono::high_resolution_clock::now();
                        busqueda->busquedaMejor();
                        t_end = std::chrono::high_resolution_clock::now();
                        elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end - t_start).count();
                        std::cout << "CPU : " << elapsed_time_ms << " ms" << std::endl;
                        break;
                    case 2:
                        std::cout << "\t\tAlgoritmo GRASP" << std::endl;
                        busqueda = new BusquedaMDGRASP(grafo,0.2f*rep);
                        t_start = std::chrono::high_resolution_clock::now();
                        busqueda->busquedaMejor();
                        t_end = std::chrono::high_resolution_clock::now();
                        elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end - t_start).count();
                        std::cout << "CPU : " << elapsed_time_ms << " ms" << std::endl;
                        break;
                    case 3:
                        std::cout << "\t\tAlgoritmo MultiArranque" << std::endl;
                        busqueda = new BusquedaMDMultiArranque(grafo);
                        t_start = std::chrono::high_resolution_clock::now();
                        busqueda->busquedaMejor();
                        t_end = std::chrono::high_resolution_clock::now();
                        elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end - t_start).count();
                        std::cout << "CPU : " << elapsed_time_ms << " ms" << std::endl;
                        break;
                    case 4:
                        std::cout << "\t\tAlgoritmo VNS" << std::endl;
                        busqueda = new BusquedaMDVNS(grafo, 5*rep);
                        t_start = std::chrono::high_resolution_clock::now();
                        busqueda->busquedaMejor();
                        t_end = std::chrono::high_resolution_clock::now();
                        elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end - t_start).count();
                        std::cout << "CPU : " << elapsed_time_ms << " ms" << std::endl;
                        break;
                    }
                    std::cout << "------------------------------------------" << std::endl;
                }
               
                delete busqueda;
            }
            delete grafo;
        }

    }
  
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
