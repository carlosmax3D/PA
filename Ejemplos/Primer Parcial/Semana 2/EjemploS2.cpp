#include <iostream>

using namespace std;

void veinteAniosDespues(int **edad); void regresaADiezAnios(int **edad);
int matrix(int **numeros);

int main(int argc, char **argv){
  int numeros[10][20] = {10,101, 10,10};
  int *p = (int *)&numeros[0];
  matrix(&p);
  cout << endl << "Cambie el primer valor!! " << numeros[0][0] << endl;
  cout << "Cambie el cuarto valor!! " << numeros[0][4] << endl;
  int *pointer = NULL;
  int edad = 10;
  pointer = &edad;
  veinteAniosDespues(&pointer);
  cout << *pointer << " - " << edad << endl;
  regresaADiezAnios(&pointer);
  cout << *pointer << " - " << edad;
  return 0;
}
void regresaADiezAnios(int **edad){
  *edad = new int;
  **edad = 10;
}
void veinteAniosDespues(int **edad){
  int *pointer = (*edad);
  *pointer = *pointer + 5;
  edad[0][0]= edad[0][0] + 5;
  *(*edad) = *(*edad) + 10;
}

int matrix(int **numeros){
    for (int i = 0; i < 1; i++)
        for (int j = 0; j < 6; j++)
            cout << numeros[i][j] << " - ";
    numeros[0][0] = 20;
    numeros[0][4] = 2000;
    return 0;
}
 
