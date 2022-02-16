#include <iostream>

using namespace std;

struct nodo {
    string nombre;
    int edad;
    float peso;
    struct nodo *siguiente;
};

struct nodo* agregarALista(struct nodo** lista, string nombre, int edad, float peso);
struct nodo* nuevoNodo(string nombre, int edad, float peso);
void liberarMemoria(struct nodo** Lista);

int main() {
    struct nodo* Lista = NULL, *ultimo = NULL;
    ultimo = agregarALista(&Lista, "CarlosOrta", 50, 50);
    ultimo = agregarALista(&Lista, "EdgarCepeda", 20, 45);
    ultimo = agregarALista(&Lista, "AnaFernanda", 19, 40);

    struct nodo* posicion = Lista;
    while (posicion != nullptr) {
        cout << "Nombre: " << posicion->nombre << " Edad: " << posicion->edad << " Peso: " << posicion->peso << endl;
        posicion = posicion->siguiente;
    }
    liberarMemoria(&Lista);
}

void liberarMemoria(struct nodo** Lista) {
    if (*Lista == nullptr)
        return;
    struct nodo* indice = *Lista;
    while (indice != nullptr) {
        struct nodo* siguiente = indice->siguiente;
        delete indice;
        indice = siguiente;
    }
    *Lista = nullptr;
}

struct nodo* agregarALista(struct nodo **lista, string nombre, int edad, float peso) {
    struct nodo* ultimo = NULL;
    if (*lista == nullptr) {
        *lista = nuevoNodo(nombre, edad, peso);
        ultimo = *lista;
    }
    else {
        ultimo = nuevoNodo(nombre, edad, peso);
        struct nodo* indice = *lista;
        while (indice->siguiente != nullptr) {
            indice = indice->siguiente;
        }
        indice->siguiente = ultimo;
    }
    return ultimo;
}

struct nodo *nuevoNodo(string nombre, int edad, float peso) {
    struct nodo* nuevo = new struct nodo();
    nuevo->nombre = nombre;
    nuevo->edad = edad;
    nuevo->peso = peso;
    nuevo->siguiente = nullptr;
    return nuevo;
}
