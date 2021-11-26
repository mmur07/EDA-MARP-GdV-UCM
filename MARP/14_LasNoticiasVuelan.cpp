
/*@ <answer>
 *
 * Nombre y Apellidos:
 *
 *@ </answer> */

#include <iostream>
#include <fstream>
 //
 //  Grafo.h
 //
 //  Implementación de grafos no dirigidos
 //
 //  Facultad de Informática
 //  Universidad Complutense de Madrid
 //
 //  Copyright (c) 2020  Alberto Verdejo
 //

#ifndef GRAFO_H_
#define GRAFO_H_

#include <iostream>
#include <vector>
#include <iostream>
#include <stdexcept>

using Adys = std::vector<int>;  // lista de adyacentes a un vértice

class Grafo {
private:
    int _V;  // número de vértices
    int _A;  // número de aristas
    std::vector<Adys> _ady;  // vector de listas de adyacentes
public:

    /**
     * Crea un grafo con V vértices.
     */
    Grafo(int V) : _V(V), _A(0), _ady(_V) {}

    /**
     * Crea un grafo a partir de los datos en el flujo de entrada (si puede).
     * primer es el índice del primer vértice del grafo en el entrada.
     */
    Grafo(std::istream& flujo, int primer = 0) : _A(0) {
        flujo >> _V;
        if (!flujo) return;
        _ady.resize(_V);
        int E, v, w;
        flujo >> E;
        while (E--) {
            flujo >> v >> w;
            ponArista(v - primer, w - primer);
        }
    }

    /**
     * Devuelve el número de vértices del grafo.
     */
    int V() const { return _V; }

    /**
     * Devuelve el número de aristas del grafo.
     */
    int A() const { return _A; }

    /**
     * Añade la arista v-w al grafo.
     * @throws domain_error si algún vértice no existe
     */
    void ponArista(int v, int w) {
        if (v < 0 || v >= _V || w < 0 || w >= _V)
            throw std::domain_error("Vertice inexistente");
        ++_A;
        _ady[v].push_back(w);
        _ady[w].push_back(v);
    }

    /**
     * Devuelve la lista de adyacencia de v.
     * @throws domain_error si v no existe
     */
    Adys const& ady(int v) const {
        if (v < 0 || v >= _V)
            throw std::domain_error("Vertice inexistente");
        return _ady[v];
    }

    /**
     * Muestra el grafo en el stream de salida o (para depurar)
     */
    void print(std::ostream& o = std::cout) const {
        o << _V << " vértices, " << _A << " aristas\n";
        for (int v = 0; v < _V; ++v) {
            o << v << ": ";
            for (int w : _ady[v]) {
                o << w << " ";
            }
            o << "\n";
        }
    }
};

/**
 * Para mostrar grafos por la salida estándar
 */
inline std::ostream& operator<<(std::ostream& o, Grafo const& g) {
    g.print(o);
    return o;
}

#endif /* GRAFO_H_ */
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

class NoticiasVuelan {
private:
    std::vector<int> componenteNodo;
    std::vector<int> alcanceComponenteConexa;
    int componenteActual = 0;
public:
    NoticiasVuelan(const Grafo& g, int nGrupos) : componenteNodo(g.V(), -1) {
        for (int k = 0; k < g.V(); k++) {
            if (componenteNodo[k] == -1) {
                alcanceComponenteConexa.push_back(dfs(g, k));
                componenteActual++;
            }
        }
    }

    int dfs(const Grafo& g, int n) {
        int cont = 1;
        componenteNodo[n] = componenteActual;
        for (int a : g.ady(n)) {
            if (componenteNodo[a] == -1) {
                cont += dfs(g, a);
            }
        }

        return cont;
    }

    void imprime() {
        for (int k = 0; k < componenteNodo.size(); k++) {
            std::cout << alcanceComponenteConexa[componenteNodo[k]] << " ";
        }
        std::cout << "\n";
    }
};

bool resuelveCaso() {
    int nNodos, nGrupos;
    // leer los datos de la entrada
    std::cin >> nNodos >> nGrupos;
    if (!std::cin)  // fin de la entrada
        return false;
    Grafo g = Grafo(nNodos);
    for (int k = 0; k < nGrupos; k++) {
        int tamGrupo;
        std::cin >> tamGrupo;
        if (tamGrupo > 0) {
            int nodoIni;
            std::cin >> nodoIni;
            if (tamGrupo > 1) {
                int newNode;
                for (int i = 1; i < tamGrupo; i++) {
                    std::cin >> newNode;
                    g.ponArista(nodoIni - 1, newNode - 1);
                    nodoIni = newNode;
                }
            }
        }
    }

    NoticiasVuelan n(g, nGrupos);
    n.imprime();
    // resolver el caso posiblemente llamando a otras funciones

    // escribir la soluci�n

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
