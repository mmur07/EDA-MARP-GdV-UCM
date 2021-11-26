
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

bool resuelveCaso() {

    int nGrupos;
    std::cin >> nGrupos;
    if (nGrupos == 0)
        return false;
    priority_queue<int, vector<int>, greater<int>> cola;
    int aux;
    for (int k = 0; k < nGrupos; k++) {
        std::cin >> aux;
        cola.push(aux);
    }
    
    int gorrasNecesarias = 0;

    while (cola.size() > 1) {
        int equipo1 = cola.top();
        cola.pop();

        int equipo2 = cola.top();
        cola.pop();

        cola.push(equipo1 + equipo2);
        gorrasNecesarias += equipo1 + equipo2;
    }
    std::cout << gorrasNecesarias << "\n";
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
