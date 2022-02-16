#include <iostream>
#include <string>

using namespace std;

//Declaramos la estructura de nuestros nodos
struct NODO {
	string nombre;
	int edad;
	float peso;
	struct NODO *siguiente; // un apuntador del tipo NODO para ligar la lista
};

// Definicion de funciones para hacer operaciones con la lista
struct NODO* nuevoNodo(string nombre, int edad, float peso);
struct NODO* agregarEnMedioDeLista(struct NODO** HEAD, string nombre, int edad, float peso, string buscarNombre);
struct NODO* agregarAlFinalDeLista(struct NODO** HEAD, string nombre, int edad, float peso);
struct NODO* agregarAElInicioDeLista(struct NODO** HEAD, string nombre, int edad, float peso);
void imprimirLista(struct NODO* lista);
struct NODO* modificarNodoDeLista(struct NODO** HEAD, string nombre, int edad, float peso, string buscarNombre);
void eliminarNodoMedioDeLista(struct NODO** HEAD, string buscarNombre);
void eliminarNodoInicioDeLista(struct NODO** HEAD);
void eliminarNodoAlFinalDeLista(struct NODO** HEAD);

// funcion principal
int main(int argc, char** argv) {
	struct NODO* HEAD = NULL;
	agregarAlFinalDeLista(&HEAD, "Carlos", 50, 100); // Se pasa de parametro el apuntador que sera la base la lista y los valores de la estructura
	cout << "primero \n";
	imprimirLista(HEAD); // Funcion para imprimir la lista
	agregarAElInicioDeLista(&HEAD, "Edgar", 15, 45); // Se pasa de parametro el apuntador que sera la base la lista y los valores de la estructura
	cout << "segundo \n";
	imprimirLista(HEAD);
	agregarAlFinalDeLista(&HEAD, "Ana", 19, 45); // Se pasa de parametro el apuntador que sera la base la lista y los valores de la estructura
	cout << "tercero \n";
	imprimirLista(HEAD);
	agregarEnMedioDeLista(&HEAD, "Jose", 17, 50,"Edgar"); // Se pasa de parametro el apuntador que sera la base la lista y los valores de la estructura
	cout << "cuarto \n";
	imprimirLista(HEAD);
	agregarEnMedioDeLista(&HEAD, "Jesus", 21, 40, "Ana"); // Se pasa de parametro el apuntador que sera la base la lista y los valores de la estructura
	cout << "quinto \n";
	imprimirLista(HEAD);
	modificarNodoDeLista(&HEAD, "Francisco", 21, 40, "Jesus"); // Se pasa de parametro el apuntador que sera la base la lista y los valores de la estructura
	cout << "sexto \n";
	imprimirLista(HEAD);
	eliminarNodoMedioDeLista(&HEAD, "Francisco"); // Se pasa de parametro el apuntador que sera la base la lista y los valores de la estructura
	cout << "septimo \n";
	imprimirLista(HEAD);
	eliminarNodoInicioDeLista(&HEAD);  // Se pasa de parametro el apuntador que sera la base la lista para poder trabajar con eso
	cout << "octavo \n";
	imprimirLista(HEAD);
	eliminarNodoAlFinalDeLista(&HEAD);  // Se pasa de parametro el apuntador que sera la base la lista para poder trabajar con eso
	cout << "noveno \n";
	imprimirLista(HEAD);
	return 0;
}

void imprimirLista(struct NODO* lista) {
	struct NODO* indice = lista;
	while (indice != NULL) {
		cout << "Nombre: " << indice->nombre << " edad: " << indice->edad << " peso: " << indice->peso << endl;
		indice = indice->siguiente;
	}
}

void eliminarNodoAlFinalDeLista(struct NODO** HEAD) {
	if (*HEAD == NULL) { // Si la lista esta vacia, no se hace nada
		return;
	}
	else {
		struct NODO* indice = *HEAD, *anterior = NULL; //inicio de la lista
		while (indice->siguiente != NULL) { // Buscamos el final de la lista, osea apuntador siguiente == NULL
			anterior = indice;
			indice = indice->siguiente;
		}
		anterior->siguiente = NULL;
		delete indice;
	}
}

void eliminarNodoInicioDeLista(struct NODO** HEAD) {
	if (*HEAD == NULL) {// Si la lista esta vacia, no se hace nada
		return;
	}
	else {
		struct NODO* nodo = *HEAD; //inicio de la lista
		*HEAD = nodo->siguiente; // reemplazamos por nodo siguiente de la lista
		delete nodo;
	}
}

void eliminarNodoMedioDeLista(struct NODO** HEAD, string buscarNombre) {
	if (*HEAD == NULL) {// Si la lista esta vacia, no se hace nada
		return;
	}
	else {
		struct NODO* indice = *HEAD, * anterior = NULL;
		bool loEncontre = false;
		while (indice->siguiente != NULL) {// Buscamos el final de la lista, osea apuntador siguiente == NULL
			if (indice->nombre.compare(buscarNombre) == 0) {
				loEncontre = true;
				break;
			}
			anterior = indice;
			indice = indice->siguiente;
		}
		if (loEncontre == true || indice->nombre.compare(buscarNombre) == 0) {
			anterior->siguiente = indice->siguiente;
			delete indice;
		}
	}
}

struct NODO* modificarNodoDeLista(struct NODO** HEAD, string nombre, int edad, float peso, string buscarNombre) {
	struct NODO* nodo = NULL;
	if (*HEAD == NULL) {// Si la lista esta vacia, no se hace nada
		return NULL;
	}
	else {
		struct NODO* indice = *HEAD, * anterior = NULL;
		bool loEncontre = false;
		while (indice->siguiente != NULL) { // Buscamos el final de la lista, osea apuntador siguiente == NULL
			if (indice->nombre.compare(buscarNombre) == 0) {
				loEncontre = true;
				break;
			}
			anterior = indice;
			indice = indice->siguiente;
		}
		if (loEncontre == true || indice->nombre.compare(buscarNombre) == 0) {
			nodo = indice;
			nodo->nombre = nombre;
			nodo->peso = peso;
			nodo->edad = edad;
		}
	}
	return nodo;
}

struct NODO* agregarAlFinalDeLista(struct NODO** HEAD, string nombre, int edad, float peso) {
	struct NODO* nuevo = NULL;
	if (*HEAD == NULL) {// Si la lista esta vacia, no se hace nada
		nuevo = nuevoNodo(nombre, edad, peso);
		*HEAD = nuevo;
	}
	else {
		struct NODO* indice = *HEAD;
		bool loEncontre = false;
		while (indice->siguiente != NULL) { // Buscamos el final de la lista, osea apuntador siguiente == NULL
			indice = indice->siguiente;
		}
		nuevo = nuevoNodo(nombre, edad, peso);
		indice->siguiente = nuevo;
		nuevo->siguiente = NULL;
	}
	return nuevo;
}

struct NODO* agregarEnMedioDeLista(struct NODO** HEAD, string nombre, int edad, float peso, string buscarNombre) {
	struct NODO* nuevo = NULL;
	if (*HEAD == NULL) {// Si la lista esta vacia, no se hace nada
		nuevo = nuevoNodo(nombre, edad, peso);
		*HEAD = nuevo;
	}
	else {
		struct NODO* indice = *HEAD, *anterior = NULL;
		bool loEncontre = false;
		while (indice->siguiente != NULL) { // Buscamos el final de la lista, osea apuntador siguiente == NULL
			if (indice->nombre.compare(buscarNombre) == 0) {
				loEncontre = true;
				break;
			}
			anterior = indice;
			indice = indice->siguiente;
		}
		if (loEncontre == true || indice->nombre.compare(buscarNombre) == 0) {
			if (anterior == NULL) {
				agregarAElInicioDeLista(HEAD, nombre, edad, peso);
			}
			else {
				nuevo = nuevoNodo(nombre, edad, peso);
				anterior->siguiente = nuevo;
				nuevo->siguiente = indice;

			}
		}
	}
	return nuevo;
}

struct NODO* agregarAElInicioDeLista(struct NODO** HEAD, string nombre, int edad, float peso) {
	struct NODO* nuevo = NULL;
	if (*HEAD == NULL) { // Si la lista esta vacia, no se hace nada
		nuevo = nuevoNodo(nombre, edad, peso);
		*HEAD = nuevo;
	}
	else {
		nuevo = nuevoNodo(nombre, edad, peso);
		nuevo->siguiente = *HEAD;
		*HEAD = nuevo;
	}
	return nuevo;
}

struct NODO* nuevoNodo(string nombre, int edad, float peso) {
	struct NODO* nuevo = new struct NODO; // Se le pide al sistema operativo memoria en el heap para nuestra estructura
	nuevo->nombre = nombre;
	nuevo->edad = edad;
	nuevo->peso = peso;
	nuevo->siguiente = NULL;
	return nuevo;
}
