#include <iostream>

using namespace std;

typedef struct nodo {
    string nombre;
    int edad;
    float peso;
    struct nodo *anterior;
    struct nodo *siguiente;
} NODO;

typedef NODO* APUNTADOR_NODO;
typedef APUNTADOR_NODO* APUNTADOR_DOBLE_NODO;

APUNTADOR_NODO agregarALista(APUNTADOR_DOBLE_NODO lista, APUNTADOR_NODO ultimo, string nombre, int edad, float peso);
APUNTADOR_NODO nuevoNodo(string nombre, int edad, float peso);
void liberarMemoria(APUNTADOR_DOBLE_NODO Lista);

int main(int argc, char **argv) {
    APUNTADOR_NODO Lista = NULL, ultimo = NULL;
    ultimo = agregarALista(&Lista, ultimo, "CarlosOrta", 50, 50);
    ultimo = agregarALista(&Lista, ultimo, "EdgarCepeda", 20, 45);
    ultimo = agregarALista(&Lista, ultimo, "AnaFernanda", 19, 40);

    APUNTADOR_NODO posicion = Lista;
    while (posicion != nullptr) {
        cout << "Elemento actual" << endl;
        cout << "Nombre: " << posicion->nombre << " Edad: " << posicion->edad << " Peso: " << posicion->peso << endl;
        cout << "Elemento Anterior " << endl;
        if (posicion->anterior != nullptr)
            cout << "Nombre: " << posicion->anterior->nombre << " Edad: " << posicion->anterior->edad << " Peso: " << posicion->anterior->peso << endl;
        else
            cout << "Nadie esta antes!" << endl;
        posicion = posicion->siguiente;
    }
    liberarMemoria(&Lista);
}

void liberarMemoria(APUNTADOR_DOBLE_NODO Lista) {
    if (*Lista == nullptr)
        return;
    APUNTADOR_NODO indice = *Lista;
    while (indice != nullptr) {
        APUNTADOR_NODO siguiente = indice->siguiente;
        delete indice;
        indice = siguiente;
    }
    *Lista = nullptr;
}

APUNTADOR_NODO agregarALista(APUNTADOR_DOBLE_NODO lista, APUNTADOR_NODO ultimo, string nombre, int edad, float peso) {
    APUNTADOR_NODO nuevo = NULL;
    if (*lista == nullptr) {
        *lista = nuevoNodo(nombre, edad, peso);
        nuevo = *lista;
    }
    else {
        nuevo = nuevoNodo(nombre, edad, peso);
        if (ultimo == nullptr) {
            APUNTADOR_NODO indice = *lista;
            while (indice->siguiente != nullptr) {
                indice = indice->siguiente;
            }
            indice->siguiente = ultimo;
            nuevo->anterior = indice;
        }
        else {
            ultimo->siguiente = nuevo;
            nuevo->anterior = ultimo;
        }
    }
    return nuevo;
}

APUNTADOR_NODO nuevoNodo(string nombre, int edad, float peso) {
    APUNTADOR_NODO nuevo = new NODO();
    nuevo->nombre = nombre;
    nuevo->edad = edad;
    nuevo->peso = peso;
    nuevo->siguiente = nullptr;
    nuevo->anterior = nullptr;
    return nuevo;
}
