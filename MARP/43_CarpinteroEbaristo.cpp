
/*@ <answer>
 *
 * Nombre y Apellidos:
 *
 *@ </answer> */

#include <iostream>
#include <fstream>
#include <limits>

using namespace std;
//
//  Matriz.h
//
//  Implementación de matrices (arrays bidimensionales)
//
//  Facultad de Informática
//  Universidad Complutense de Madrid
//
//  Copyright (c) 2020  Alberto Verdejo
//

#ifndef MATRIZ_H
#define MATRIZ_H

#include <vector>
#include <ostream>

template <typename Object>
class Matriz {
public:
    // crea una matriz con fils filas y cols columnas,
    // con todas sus celdas inicializadas al valor e
    Matriz(int fils = 0, int cols = 0, Object e = Object()) : datos(fils, std::vector<Object>(cols, e)) {}

    // operadores para poder utilizar notación M[i][j]
    std::vector<Object> const& operator[](int f) const {
        return datos[f];
    }
    std::vector<Object>& operator[](int f) {
        return datos[f];
    }

    // métodos que lanzan una excepción si la posición no existe
    Object const& at(int f, int c) const {
        return datos.at(f).at(c);
    }
    Object& at(int f, int c) {
        return datos.at(f).at(c);
    }

    int numfils() const { return datos.size(); }
    int numcols() const { return numfils() > 0 ? datos[0].size() : 0; }

    bool posCorrecta(int f, int c) const {
        return 0 <= f && f < numfils() && 0 <= c && c < numcols();
    }

private:
    std::vector<std::vector<Object>> datos;
};

template <typename Object>
inline std::ostream& operator<<(std::ostream& out, Matriz<Object> const& m) {
    for (int i = 0; i < m.numfils(); ++i) {
        for (int j = 0; j < m.numcols(); ++j) {
            out << m[i][j] << ' ';
        }
        out << '\n';
    }
    return out;
}

#endif

//
//  EnterosInf.h
//
//  Implementación de enteros con +infinito.
//
//  Facultad de Informática
//  Universidad Complutense de Madrid
//
//  Copyright (c) 2020  Alberto Verdejo
//

#ifndef ENTEROSINF_H_
#define ENTEROSINF_H_

#include <limits>
#include <iostream>

class EntInf {
    int num;
public:
    static const int _intInf = 1000000000;

    EntInf(int n = 0) : num(n) {}

    EntInf operator+(EntInf const& b) const {
        if (num == _intInf || b.num == _intInf || num >= (_intInf - b.num))
            return _intInf;
        else return num + b.num;
    }

    bool operator==(EntInf const& b) const {
        return num == b.num;
    }

    bool operator!=(EntInf const& b) const {
        return !(*this == b);
    }

    bool operator<(EntInf const& b) const {
        if (num == _intInf) return false;
        else if (b.num == _intInf) return true;
        else return num < b.num;
    }

    bool operator>(EntInf const& b) const {
        return b < *this;
    }

    void print(std::ostream& out = std::cout) const {
        if (num == _intInf) out << "+Inf";
        else out << num;
    }
};

const EntInf Infinito(EntInf::_intInf);

inline std::ostream& operator<<(std::ostream& out, EntInf const& e) {
    e.print(out);
    return out;
}

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
#include <vector>



int mejor_Resultado(const std::vector<int>& cortes, Matriz<int> &resultados, int i, int j) {
    if (resultados[i][j] != -1) return resultados[i][j];
    
    int marcaI = cortes[i];
    int marcaJ = cortes[j];

    int longitud = marcaJ - marcaI;

    if (longitud <= 0) resultados[i][j] = 0;

    else {
        size_t sigMarcaI = i + 1;
        if (sigMarcaI == cortes.size() || cortes[sigMarcaI] >= marcaJ) resultados[i][j] = 0;
        else {
            int min = std::numeric_limits<int>().max();
            while (sigMarcaI < cortes.size() && cortes[sigMarcaI] < marcaJ) {
                min = std::min(min, mejor_Resultado(cortes, resultados, i, sigMarcaI) + mejor_Resultado(cortes, resultados, sigMarcaI, j));
                sigMarcaI++;
            }

            resultados[i][j] = 2 * longitud + min;
        }
    }

    return resultados[i][j];
}

bool resuelveCaso() {

    // leer los datos de la entrada
    int longTablero, nCortes;
    std::cin >> longTablero >> nCortes;
    
    if (longTablero == 0 && nCortes == 0)
        return false;
    std::vector<int> vecCortes;
    vecCortes.push_back(0);
    int aux;
    for (int k = 0; k < nCortes; k++) {
        std::cin >> aux;
        vecCortes.push_back(aux);
    }
    vecCortes.push_back(longTablero);
    Matriz<int> resultado(nCortes + 2, nCortes + 2, -1);
    std::cout << mejor_Resultado(vecCortes, resultado, 0, nCortes + 1) << "\n";
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
