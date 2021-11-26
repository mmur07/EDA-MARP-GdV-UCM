
/*@ <answer>
 *
 * Nombre y Apellidos:
 *
 *@ </answer> */

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;
//
//  Matriz.h
//
//  Implementaci�n de matrices (arrays bidimensionales)
//
//  Facultad de Inform�tica
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

    // operadores para poder utilizar notaci�n M[i][j]
    std::vector<Object> const& operator[](int f) const {
        return datos[f];
    }
    std::vector<Object>& operator[](int f) {
        return datos[f];
    }

    // m�todos que lanzan una excepci�n si la posici�n no existe
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
//  Implementaci�n de enteros con +infinito.
//
//  Facultad de Inform�tica
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

struct Direccion {
    int x;
    int y;
};

std::vector<Direccion> dir = { {-1, -1} ,{0, -1}, {1, -1} };

bool es_Valida(int i, int j, int N) {
    return (i >= 0 && j >= 0 && j < N);
}

int resolver(int i, int j, const Matriz<int>& m, Matriz<int> &resultado) {
    
    if (!es_Valida(i, j, m.numcols())) return -1;

    if (resultado[i][j] != -1) return resultado[i][j];

    if (i == 0) resultado[i][j] = m[i][j];

    else {
        int r = max(max(resolver(i - 1, j - 1, m, resultado), resolver(i - 1, j, m, resultado)), resolver(i - 1, j + 1, m, resultado));

        r += m[i][j];
        resultado[i][j] = r;
    }

    return resultado[i][j];
}

bool resuelveCaso() {
    int N;
    std::cin >> N;

    if (!std::cin)  // fin de la entrada
        return false;

    Matriz<int>m(N, N, -1);
    int aux;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++){
            std::cin >> aux;
            m[i][j] = aux;
        }
    }
    
    int maxSolucion = 0, newSolution, col = 0;
    Matriz<int>mat(N, N, -1);
    for (int k = 0; k < N; k++) {
        newSolution = resolver(N - 1, k, m, mat);
        if (newSolution > maxSolucion) {
            maxSolucion = newSolution;
            col = k + 1;
        }
    }

    std::cout << maxSolucion << " " << col << "\n";

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
