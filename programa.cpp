// 5 partidos -> 25 candidatos cada uno
// los votos q puso sofi
// ordenarnlos como dijo dami
//sistema de d'hont

#include <iostream>
#include <cmath>
#include <fstream>
#include <ostream>
#include <string>
using namespace std;

#define _TOPE_CANDIDATOS 25
#define _TOPE_LISTAS 5
#define _TOPE_VOTOS 300
struct voto {
    int numero;
    char genero;
    int edad;
};
struct lista {
    int numero;
    string nombre;
    string candidatos[_TOPE_CANDIDATOS];
};

typedef lista tListas[_TOPE_LISTAS];
typedef voto tVotos[_TOPE_VOTOS];

void cargarLista(tListas & listas);
void parsear(string cadenaAParsear, voto & votoParseado);
void cargarVotos(tVotos & votos);
/*void ordenarVotos();
void procesarVotos();
void mostrarVotos();*/

int main(){
    tListas listas;
    tVotos votos;
    
    cargarLista(listas);
    cargarVotos(votos);
    //ordenarVotos();
    //mostrarVotos();
    //procesarVotos();
}

void cargarLista(tListas & listas) {
    string nombreCandidato;
    lista lista;
    int i = 0, j = 0;
    ifstream archivoCandidatos("candidatos.txt");

    while(getline(archivoCandidatos, nombreCandidato)) {
        if (i == _TOPE_CANDIDATOS){
            lista.numero = j+1;
            lista.nombre = "Lista " + to_string(lista.numero);
            listas[j] = lista;
            i = 0;
            j++;
        }
        lista.candidatos[i] = nombreCandidato;
        i++;
    }

    archivoCandidatos.close();

    // for (int i = 0; i < _TOPE_LISTAS; i++)
    // {

    //     cout<<"Numero de lista: " << listas[i].numero<<endl;
    //     cout<<"Nombre de lista: " << listas[i].nombre<<endl;
    //     cout<<"Candidatos: "<<endl;
    //     cout<<"______________Titulares_______________"<<endl;
    //     for (int j = 0; j < _TOPE_CANDIDATOS; j++)
    //     {
    //         if(j == 13){
    //             cout<<"______________Suplentes_______________"<<endl;
    //         }
    //         cout<<j + 1<<") "<<listas[i].candidatos[j]<<endl;

    //     }
    //     cout<<"______________________________________"<<endl;
    //     cout<<endl;

    // }
}

void cargarVotos(tVotos & votos) {
    string cadena;
    voto voto;
    int i = 0;
    ifstream archivoVotos("votos.txt");

    while(getline(archivoVotos, cadena)) {
        parsear(cadena, voto);
        votos[i] = voto;
        // cout << votos[i].numero << endl;
        // cout << votos[i].genero << endl;
        // cout << votos[i].edad << endl;
        // cout <<"-----------------------"<<endl;

        i++;
    }
    archivoVotos.close();
}

void parsear(string cadenaAParsear, voto & votoParseado) {
    int primero = 0, ultimo = 0, k = 0;
    string palabra[20];

    while(ultimo < cadenaAParsear.size()){
        ultimo = cadenaAParsear.find(";", primero); //cuando no lo encuentra devuelve -1
        if (ultimo == -1) ultimo = cadenaAParsear.size();
        palabra[k] = cadenaAParsear.substr(primero, ultimo - primero) + '\0';
        k++;
        primero = ultimo + 1;
    }

     votoParseado.numero = stoi(palabra[0]);
     votoParseado.genero = palabra[1][0]; 
     votoParseado.edad = stoi(palabra[2]);
}