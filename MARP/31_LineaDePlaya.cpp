
/*@ <answer>
 *
 * Nombre y Apellidos:
 *
 *@ </answer> */

#include <iostream>
#include <fstream>
#include <queue>
using namespace std;

/*@ <answer>

 Escribe aqu� un comentario general sobre la soluci�n, explicando c�mo
 se resuelve el problema y cu�l es el coste de la soluci�n, en funci�n
 del tama�o del problema.

 @ </answer> */


 // ================================================================
 // Escribe el c�digo completo de tu soluci�n aqu� debajo
 // ================================================================
 //@ <answer>

struct Edificio {
    int inicio;
    int final;
    bool operator< (const Edificio& other) const{
        return final< other.final;
    }
    bool operator> (const Edificio& other) const {
        return final> other.final;
    }
};

int numConexiones(std::priority_queue<Edificio, std::vector<Edificio>, std::greater<Edificio>> &cola) {
    int nConexiones = 0;

    while (!cola.empty()) {
        nConexiones++;
        Edificio edificioInicial = cola.top();
        cola.pop();
        while (!cola.empty() && cola.top().inicio < edificioInicial.final) {
            cola.pop();
        }
    }

    return nConexiones;
}

bool resuelveCaso() {

    // leer los datos de la entrada
    int nEdificios;
    std::cin >> nEdificios;
    if (nEdificios == 0)  // fin de la entrada
        return false;
    std::priority_queue<Edificio, std::vector<Edificio>, std::greater<Edificio>> cola;
    Edificio nEdi;
    int ini, fin;
    for (int k = 0; k < nEdificios; k++) {
        std::cin >> ini >> fin;
        nEdi.inicio = ini;
        nEdi.final = fin;
        cola.push(nEdi);
    }
    
    std::cout << numConexiones(cola) << "\n";

    return true;
}

//@ </answer>
//  Lo que se escriba dejado de esta l�nea ya no forma parte de la soluci�n.

int main() {
    // ajustes para que cin extraiga directamente de un fichero
#ifndef DOMJUDGE
    std::ifstream in("casos.txt");
    auto cinbuf = std::cin.rdbuf(in.rdbuf());
#endif

    while (resuelveCaso());

    // para dejar todo como estaba al principio
#ifndef DOMJUDGE
    std::cin.rdbuf(cinbuf);
    system("PAUSE");
#endif
    return 0;
}
