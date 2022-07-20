// 5 partidos -> 25 candidatos cada uno
// los votos q puso sofi
// ordenarnlos como dijo dami
//sistema de d'hont

#include<iostream>
#include<cmath>
#include<fstream>
#include<ostream>
using namespace std;

#define _TOPE_LARGO_NOMBRE 100
#define _TOPE_CANDIDATOS 25
#define _TOPE_LISTAS 5

typedef char tNombre[_TOPE_LARGO_NOMBRE];

struct lista {
    int numero;
    string nombre;
    string candidatos[_TOPE_CANDIDATOS];
};

typedef lista tLista[_TOPE_LISTAS];

struct voto {
    int numero;
    char genero;
    int edad;
};

void cargarVotos();
void cargarLista(tLista & listas);
/*void ordenarVotos();
void procesarVotos();
void mostrarVotos();*/

int main(){
    tLista listas;
    
    cargarLista(listas);
    //cargarVotos("votos.csv");
    //ordenarVotos();
    //mostrarVotos();
    //procesarVotos();

    for (int i = 0; i < _TOPE_LISTAS; i++)
    {
        cout<<listas[i].numero<<endl;
        cout<<listas[i].nombre<<endl;
        for (int j = 0; j < _TOPE_CANDIDATOS; j++)
        {
            cout<<listas[i].candidatos[j]<<endl;
        }
        cout<<"_____________________________"<<endl;
        
    }
    
}

void cargarLista(tLista & listas) {

    string nombreCandidato;
    lista lista;
    int i,j = 0;
    ifstream fe("candidatos.txt");

    while(getline(fe, nombreCandidato)) {
        if(i<_TOPE_CANDIDATOS-1){
            lista.candidatos[i] = nombreCandidato;
        }
        else{
            lista.numero = j+1;
            lista.nombre = "Lista " + to_string(j+1);
            listas[j] = lista;
            i = -1;
            j++;
        }
        i++;
    }
      
    fe.close();
}

/*void  cargaVotos(char * ori) {
  tRPersona per;
  string cadena;
  // Abre un entrada de entrada cadena de caracteres
  ifstream fe(ori); 
 
  cout<<"Procesa personas desde el archivo de texto "<<ori<<" a "<<endl;
  cout<<"El archivo origen es un texto que tiene nombre;edad;categoria;salario "<<endl;
  cout<<"separado por el simbolo ; sin encabezado"<<endl;
  cout<<"============================================================="<<endl;
  printf("%-30s %5s %5s %5s\n", "Nombre", "Edad", "Cat.", "Salario" );
  printf("%-30s %5s %5s %5s\n", "=============================", "====", "====", "=======" );
  while(getline(fe, cadena)) { // detecta la marca de fin de archivo
	  parsear(cadena, per ) ; // separa los campos, los devuelve en el registro de acuerdo al tipo de datos

      printf("%-30s %5d %5s %5d\n", per.nbe, per.edad, per.categoria, per.salario );

   }
   fe.close();
}*/
