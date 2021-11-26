
/*@ <answer>
 *
 * Nombre y Apellidos:
 *
 *@ </answer> */

#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
using namespace std;

#include <vector>

//
//  ConjuntosDisjuntos.h
//
//  Implementación de estructuras de partición o conjuntos disjuntos
//  con unión por tamaño y compresión de caminos
//
//  Facultad de Informática
//  Universidad Complutense de Madrid
//
//  Copyright (c) 2020  Alberto Verdejo
//

#ifndef CONJUNTOSDISJUNTOS_H_
#define CONJUNTOSDISJUNTOS_H_

#include <vector>
#include <iostream>

class ConjuntosDisjuntos {
protected:
    int ncjtos;  // número de conjuntos disjuntos
    mutable std::vector<int> p;  // enlace al padre
    std::vector<int> tam;  // tamaño de los árboles
public:

    // crea una estructura de partición con los elementos 0..N-1,
    // cada uno en un conjunto, partición unitaria
    ConjuntosDisjuntos(int N) : ncjtos(N), p(N), tam(N, 1) {
        for (int i = 0; i < N; ++i)
            p[i] = i;
    }

    //  devuelve el representante del conjunto que contiene a a
    int buscar(int a) const {
        if (p.at(a) == a) // es una raíz
            return a;
        else
            return p[a] = buscar(p[a]);
    }

    // unir los conjuntos que contengan a a y b
    void unir(int a, int b) {
        int i = buscar(a);
        int j = buscar(b);
        if (i == j) return;
        if (tam[i] > tam[j]) { // i es la raíz del árbol más grande
            tam[i] += tam[j]; p[j] = i;
        }
        else {
            tam[j] += tam[i]; p[i] = j;
        }
        --ncjtos;
    }

    // devuelve si a y b están en el mismo conjunto
    bool unidos(int a, int b) const {
        return buscar(a) == buscar(b);
    }

    // devuelve el número de elementos en el conjunto de a
    int cardinal(int a) const {
        return tam[buscar(a)];
    }

    // devuelve el número de conjuntos disjuntos
    int num_cjtos() const { return ncjtos; }

};

#endif

/*@ <answer>

 Escribe aqu� un comentario general sobre la soluci�n, explicando c�mo
 se resuelve el problema y cu�l es el coste de la soluci�n, en funci�n
 del tama�o del problema.

 @ </answer> */


 // ================================================================
 // Escribe el c�digo completo de tu soluci�n aqu� debajo
 // ================================================================
 //@ <answer>

using Matriz = std::vector<std::vector<bool>>;

struct Direccion {
    int x;
    int y;
};

std::vector<Direccion> dirs = { {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1} };

class MapaPetroleros {
private:
    Matriz mapa;
    ConjuntosDisjuntos cjt;
    int mayor = 0;
    int ancho = 0;
    int alto = 0;
    bool valida(int x, int y) {
        return (x < mapa.size() && x >= 0 && y < mapa[0].size() && y >= 0);
    }
    
    int posicionMatriz(int x, int y, int anchura) {
        return x * anchura + y;
    }

public:
    MapaPetroleros(int altura, int anchura) : mapa(altura, std::vector<bool>(anchura, false)), cjt(altura * anchura), alto(altura), ancho(anchura) {}
    void construyeMapa() {
        for (int x = 0; x < alto; x++) {
            for (int y = 0; y < ancho; y++) {
                char aux = std::cin.get();
                if (aux == '#') {
                    if (mayor == 0) mayor = 1;
                    mapa[x][y] = true; //Visitamos la marca
                    for (Direccion dir : dirs) {
                        int adyX = x + dir.x;
                        int adyY = y + dir.y;
                        if (valida(adyX, adyY) && mapa[adyX][adyY]) {
                            int posOriginal = posicionMatriz(x, y, ancho);
                            int posAdy = posicionMatriz(adyX, adyY, ancho);
                            if (!cjt.unidos(posOriginal, posAdy)) {
                                cjt.unir(posOriginal, posAdy);
                                mayor = std::max(mayor, cjt.cardinal(posAdy));
                            }
                        }
                    }
                }
            }
            std::cin.get();
        }
    }
    

    void addMarca(int x, int y) {
        mapa[x][y] = true;
        if (mayor == 0) mayor = 1;
        for (Direccion dir : dirs) {
            int adyX = x + dir.x;
            int adyY = y + dir.y;
            if (valida(adyX, adyY) && mapa[adyX][adyY]) {
                int posOriginal = posicionMatriz(x, y, ancho);
                int posAdy = posicionMatriz(adyX, adyY, ancho);
                if (!cjt.unidos(posOriginal, posAdy)) {
                    cjt.unir(posOriginal, posAdy);
                    mayor = std::max(mayor, cjt.cardinal(posAdy));
                }
            }
        }
    }

    int getMayor() {
        return mayor;
    }
};

bool resuelveCaso() {

    // leer los datos de la entrada
    int altura, anchura;
    std::cin >> altura >> anchura;
    if (!std::cin)  // fin de la entrada
        return false;
    MapaPetroleros m = MapaPetroleros(altura, anchura);
    std::cin.get();
    m.construyeMapa();
    std::cout << m.getMayor() << " ";
    int nAdicionales;
    std::cin >> nAdicionales;
    for (int k = 0; k < nAdicionales; k++) {
        int posX, posY;
        std::cin >> posX >> posY;
        m.addMarca(posX - 1, posY - 1);
        std::cout << m.getMayor() << " ";
    }
    std::cout << "\n";
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
