
// Nombre y Apellidos

// Comentario general sobre la soluci�n,
// explicando c�mo se resuelve el problema

#include <iostream>
#include <fstream>
#include <queue>

//
//  DigrafoValorado.h
//
//  Implementaci�n de grafos dirigidos valorados
//
//  Facultad de Inform�tica
//  Universidad Complutense de Madrid
//
//  Copyright (c) 2020 Alberto Verdejo
//

#ifndef DIGRAFOVALORADO_H_
#define DIGRAFOVALORADO_H_

#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

template <typename Valor>
class AristaDirigida {
public:
    AristaDirigida() : pimpl(nullptr) {}
    AristaDirigida(int v, int w, Valor valor) : pimpl(std::make_shared<AristaDirigida_impl>(v, w, valor)) {}
    int desde() const { return pimpl->v; }
    int hasta() const { return pimpl->w; }
    Valor valor() const { return pimpl->valor; }

    void print(std::ostream& o = std::cout) const {
        o << "(" << pimpl->v << ", " << pimpl->w << ", " << pimpl->valor << ")";
    }

    bool operator<(AristaDirigida<Valor> const& b) const {
        return valor() < b.valor();
    }
    bool operator>(AristaDirigida<Valor> const& b) const {
        return b.valor() < valor();
    }
private:
    struct AristaDirigida_impl {
        int v, w;
        Valor valor;
        AristaDirigida_impl(int v, int w, Valor valor) : v(v), w(w), valor(valor) {}
    };
    std::shared_ptr<AristaDirigida_impl> pimpl;
};

template <typename Valor>
inline std::ostream& operator<<(std::ostream& o, AristaDirigida<Valor> const& ar) {
    ar.print(o);
    return o;
}



template <typename Valor>
using AdysDirVal = std::vector<AristaDirigida<Valor>>;  // lista de adyacentes a un v�rtice

template <typename Valor>
class DigrafoValorado {
public:

    /**
     * Crea un grafo con V v�rtices.
     */
    DigrafoValorado(int v) : _V(v), _A(0), _ady(_V) {}

    /**
     * Crea un grafo dirigido y valorado a partir de los datos en el flujo de entrada (si puede).
     * primer es el �ndice del primer v�rtice del grafo en el entrada.
     */
    DigrafoValorado(std::istream& flujo, int primer = 0) : _A(0) {
        flujo >> _V;
        if (!flujo) return;
        _ady.resize(_V);
        int E, v, w;
        Valor c;
        flujo >> E;
        while (E--) {
            flujo >> v >> w >> c;
            ponArista({ v - primer, w - primer, c });
        }
    }

    /**
     * Devuelve el n�mero de v�rtices del grafo.
     */
    int V() const { return _V; }

    /**
     * Devuelve el n�mero de aristas del grafo.
     */
    int A() const { return _A; }

    /**
     * A�ade la arista dirigida v-w al grafo.
     * @throws invalid_argument si alg�n v�rtice no existe
     */
    void ponArista(AristaDirigida<Valor> arista) {
        int v = arista.desde(), w = arista.hasta();
        if (v < 0 || v >= _V || w < 0 || w >= _V)
            throw std::invalid_argument("Vertice inexistente");
        ++_A;
        _ady[v].push_back(arista);
    }

    /**
     * Comprueba si hay arista de v a w.
     */
    bool hayArista(int v, int w) const {
        for (auto a : _ady[v])
            if (a.hasta() == w) return true;
        return false;
    }

    /**
     * Devuelve la lista de adyacencia de v.
     * @throws invalid_argument si v no existe
     */
    AdysDirVal<Valor> const& ady(int v) const {
        if (v < 0 || v >= _V)
            throw std::invalid_argument("Vertice inexistente");
        return _ady[v];
    }

    /**
     * Devuelve el grafo dirigido inverso.
     */
    DigrafoValorado<Valor> inverso() const {
        DigrafoValorado<Valor> inv(_V);
        for (auto v = 0; v < _V; ++v) {
            for (auto a : _ady[v]) {
                inv.ponArista({ a.hasta(), a.desde(), a.valor() });
            }
        }
        return inv;
    }

    /**
     * Muestra el grafo en el stream de salida o
     */
    void print(std::ostream& o = std::cout) const {
        o << _V << " v�rtices, " << _A << " aristas\n";
        for (auto v = 0; v < _V; ++v) {
            o << v << ": ";
            for (auto a : _ady[v]) {
                o << a << " ";
            }
            o << "\n";
        }
    }

private:
    int _V;   // n�mero de v�rtices
    int _A;   // n�mero de aristas
    std::vector<AdysDirVal<Valor>> _ady;   // vector de listas de adyacentes

};

/**
 * Para mostrar grafos por la salida est�ndar.
 */
template <typename Valor>
inline std::ostream& operator<<(std::ostream& o, const DigrafoValorado<Valor>& g) {
    g.print(o);
    return o;
}


#endif

//
//  IndexPQ.h
//
//  Implementaci�n de colas con prioridad mediante mont�culos.
//  Los elementos son enteros de 0 a N-1 que tienen asociada una prioridad
//  dentro de la cola. La prioridad de un elemento puede ser modificada
//  con coste en O(log N).
//
//  Facultad de Inform�tica
//  Universidad Complutense de Madrid
//
//  Copyright (c) 2020 Alberto Verdejo
//

#ifndef INDEXPQ_H_
#define INDEXPQ_H_

#include <iostream>
#include <stdexcept>
#include <vector>


// T es el tipo de las prioridades
// Comparator dice cu�ndo un valor de tipo T es m�s prioritario que otro
template <typename T = int, typename Comparator = std::less<T>>
class IndexPQ {
public:
    // registro para las parejas < elem, prioridad >
    struct Par {
        int elem;
        T prioridad;
    };

private:
    // vector que contiene los datos (pares < elem, prio >)
    std::vector<Par> array;     // primer elemento en la posici�n 1

    // vector que contiene las posiciones en array de los elementos
    std::vector<int> posiciones;   // un 0 indica que el elemento no est�

    /* Objeto funci�n que sabe comparar prioridades.
     antes(a,b) es cierto si a es m�s prioritario que b */
    Comparator antes;

public:
    IndexPQ(int N, Comparator c = Comparator()) : array(1), posiciones(N, 0), antes(c) {}

    IndexPQ(IndexPQ<T, Comparator> const&) = default;

    IndexPQ<T, Comparator>& operator=(IndexPQ<T, Comparator> const&) = default;

    ~IndexPQ() = default;

    // e debe ser uno de los posibles elementos
    void push(int e, T const& p) {
        if (posiciones.at(e) != 0)
            throw std::invalid_argument("No se pueden insertar elementos repetidos.");
        else {
            array.push_back({ e, p });
            posiciones[e] = size();
            flotar(size());
        }
    }

    void update(int e, T const& p) {
        int i = posiciones.at(e);
        if (i == 0) // el elemento e se inserta por primera vez
            push(e, p);
        else {
            array[i].prioridad = p;
            if (i != 1 && antes(array[i].prioridad, array[i / 2].prioridad))
                flotar(i);
            else // puede hacer falta hundir a e
                hundir(i);
        }
    }

    int size() const {
        return int(array.size()) - 1;
    }

    bool empty() const {
        return size() == 0;
    }

    Par const& top() const {
        if (size() == 0)
            throw std::domain_error("No se puede consultar el primero de una cola vacia");
        else return array[1];
    }

    void pop() {
        if (size() == 0) throw std::domain_error("No se puede eliminar el primero de una cola vac�a.");
        else {
            posiciones[array[1].elem] = 0; // para indicar que no est�
            if (size() > 1) {
                array[1] = std::move(array.back());
                posiciones[array[1].elem] = 1;
                array.pop_back();
                hundir(1);
            }
            else
                array.pop_back();
        }
    }

private:

    void flotar(int i) {
        Par parmov = std::move(array[i]);
        int hueco = i;
        while (hueco != 1 && antes(parmov.prioridad, array[hueco / 2].prioridad)) {
            array[hueco] = std::move(array[hueco / 2]); posiciones[array[hueco].elem] = hueco;
            hueco /= 2;
        }
        array[hueco] = std::move(parmov); posiciones[array[hueco].elem] = hueco;
    }

    void hundir(int i) {
        Par parmov = std::move(array[i]);
        int hueco = i;
        int hijo = 2 * hueco; // hijo izquierdo, si existe
        while (hijo <= size()) {
            // cambiar al hijo derecho de i si existe y va antes que el izquierdo
            if (hijo < size() && antes(array[hijo + 1].prioridad, array[hijo].prioridad))
                ++hijo;
            // flotar el hijo si va antes que el elemento hundi�ndose
            if (antes(array[hijo].prioridad, parmov.prioridad)) {
                array[hueco] = std::move(array[hijo]); posiciones[array[hueco].elem] = hueco;
                hueco = hijo; hijo = 2 * hueco;
            }
            else break;
        }
        array[hueco] = std::move(parmov); posiciones[array[hueco].elem] = hueco;
    }

};

#endif /* INDEXPQ_H_ */

class minimoEsfuerzo {
private:
    const int INF = std::numeric_limits<int>::max();
    int origen_;
    std::vector<int> dist;
    std::vector<AristaDirigida<int>> ult;
    IndexPQ<int> pq;
    int esfuerzo = 0;

    void relajar(AristaDirigida<int> a) {
        int v = a.desde(); int w = a.hasta();
        if (dist[w] > dist[v] + a.valor()) {
            dist[w] = dist[v] + a.valor();
            ult[w] = a;
            pq.update(w, dist[w]);
        }
    }

public:
    minimoEsfuerzo(const DigrafoValorado<int>& g, int origen) : dist(g.V(), INF), ult(g.V()), pq(g.V()), origen_(origen) {
        dist[origen] = 0;
        pq.push(origen, 0);
        while (!pq.empty()) {
            int v = pq.top().elem; pq.pop();
            for (auto a : g.ady(v)) {
                relajar(a);
            }
        }
    }
    int distancia(int objetivo){
        int dist = 0;
        for (auto a = ult[objetivo]; a.desde() != origen_; a = ult[a.desde()]) {
            if (a.valor() == INF) return -1;
            else dist += a.valor();
        }
        return dist;
    }
};


// funci�n que resuelve el problema
// comentario sobre el coste, O(f(N)), donde N es ...
void resolver(const DigrafoValorado<int> &g, int origen, std::vector<int> objetivos) {
    minimoEsfuerzo ida = minimoEsfuerzo(g, origen);
    int esfuerzo = 0;
    int esfuerzoAuxIda;
    int esfuerzoAuxVuelta;
    for (int k = 0; k < objetivos.size(); k++) {
        esfuerzoAuxIda = ida.distancia(objetivos[k]);

        minimoEsfuerzo vuelta = minimoEsfuerzo(g, objetivos[k]);
        esfuerzoAuxVuelta = vuelta.distancia(origen);

        if (esfuerzoAuxIda == -1 || esfuerzoAuxVuelta == -1) {
            std::cout << "Imposible\n";
            return;
        }
        else {
            esfuerzo += (esfuerzoAuxIda + esfuerzoAuxVuelta);
        }
    }
    std::cout << esfuerzo << "\n";
}

// resuelve un caso de prueba, leyendo de la entrada la
// configuraci�n, y escribiendo la respuesta
bool resuelveCaso() {
    int nNodos, nAristas;
    // leer los datos de la entrada
    std::cin >> nNodos;
    if (!std::cin)  // fin de la entrada
        return false;
    std::cin >> nAristas;
    DigrafoValorado<int> g(nNodos);
    int aux1, aux2, val;
    for (int k = 0; k < nAristas; k++) {
        std::cin >> aux1 >> aux2 >> val;
        AristaDirigida<int> a = AristaDirigida<int>(aux1 - 1, aux2 - 1, val);
        g.ponArista(a);
    }
    int origen, nConsultas;

    std::cin >> origen >> nConsultas;
    std::vector<int> objetivos;
    for (int k = 0; k < nConsultas; k++) {
        std::cin >> aux1;
        objetivos.push_back(aux1 - 1);
    }

    resolver(g, origen - 1, objetivos);

    // escribir sol

    return true;
}

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
