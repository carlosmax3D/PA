#include <iostream>
#include <string>
#include <fstream>

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
void abrirArchivo(struct NODO** lista);
void guardarArchivo(struct NODO* lista);
void leerArchivoTellg(struct NODO** lista);

// funcion principal
int main(int argc, char** argv) {
	struct NODO* HEAD = NULL;
	leerArchivoTellg(&HEAD);
	imprimirLista(HEAD);
	agregarAlFinalDeLista(&HEAD, "Carlos", 50, 100); // Se pasa de parametro el apuntador que sera la base la lista y los valores de la estructura
	agregarAElInicioDeLista(&HEAD, "Edgar", 15, 45); // Se pasa de parametro el apuntador que sera la base la lista y los valores de la estructura
	agregarAlFinalDeLista(&HEAD, "Ana", 19, 45); // Se pasa de parametro el apuntador que sera la base la lista y los valores de la estructura
	agregarEnMedioDeLista(&HEAD, "Jose", 17, 50,"Edgar"); // Se pasa de parametro el apuntador que sera la base la lista y los valores de la estructura
	agregarEnMedioDeLista(&HEAD, "Jesus", 21, 40, "Ana"); // Se pasa de parametro el apuntador que sera la base la lista y los valores de la estructura
	guardarArchivo(HEAD);
	imprimirLista(HEAD);
}

void leerArchivoTellg(struct NODO** lista) {
	ifstream variableArchivo;
	variableArchivo.open("archivo.txt", ios::binary | ios::in);
	if (variableArchivo.is_open()) {
		// get length of file:
		variableArchivo.seekg(0, variableArchivo.end);
		int length = variableArchivo.tellg();
		variableArchivo.seekg(0, variableArchivo.beg);
		int bytes = 0;
		while (bytes <= length) {
			char nombre[255];
			int edad;
			float peso;
			variableArchivo.read(nombre, 255);
			bytes += 255;
			variableArchivo.read(reinterpret_cast<char*>(&(edad)), sizeof(edad));
			bytes += sizeof(edad);
			variableArchivo.read(reinterpret_cast<char*>(&(peso)), sizeof(peso));
			bytes += sizeof(peso);
			agregarAElInicioDeLista(lista, string(nombre), edad, peso);
		}
		variableArchivo.close();
	}
}

void guardarArchivo(struct NODO* lista) {
	// Variable para manejar archivos
	ofstream variableArchivo;
	// Abrir el archivo para
	variableArchivo.open("archivo.txt", ios::binary | ios::trunc);
	// Variable para limitar los caracteres en el archivo
	char text[255];
	// Variable para guardar un valor entero
	int valor;
	struct NODO* indice = lista;
	while (indice != NULL) {
		// Se convierte a un arreglo para limitar los caracteres
		strcpy_s(text,(indice->nombre).c_str());
		// Se escribe el arreglo  con la cantidad maxima de caracteres
		variableArchivo.write(text, 255); // Se guarda el nombre
		variableArchivo.write((char*)&(indice->edad), sizeof(indice->edad));
		variableArchivo.write((char*)&(indice->peso), sizeof(indice->peso));
		indice = indice->siguiente;
	}
	variableArchivo.close();
}

void abrirArchivo(struct NODO** lista) {
	// Variable para leer archivos
	ifstream variableArchivo;
	// Habilitamos que no lea nada si el archivo no existe
	variableArchivo.exceptions(ifstream::failbit);
	try {
		// Abrimos el archivo en modo de lectura
		variableArchivo.open("archivo.txt", ios::binary | ios::in);
		// Leer el archivo hasta encontrar el final
		while (!variableArchivo.eof()) {
			// Variable para leer cada cadena de texto de las variables
			char nombre[255];
			int edad;
			float peso;
			// Variable para leer las categorias
			variableArchivo.read(nombre, 255);
			variableArchivo.read(reinterpret_cast<char*>(&(edad)), sizeof(edad));
			variableArchivo.read(reinterpret_cast<char*>(&(peso)), sizeof(peso));
			agregarAElInicioDeLista(lista, string(nombre), edad, peso);
		}
	}
	catch (const ifstream::failure& error) {
		// Si ocurre un error al leer el archivo
	}
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
