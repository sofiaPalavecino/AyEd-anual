#include <iostream>
#include <cmath>
#include <fstream>
#include <ostream>
#include <string>
#include <string.h>
using namespace std;

#define _TOPE_LISTAS 5
#define _TOPE_CANDIDATOS 25
#define _TOPE_VOTOS 300
struct voto {
    int numero;
    char genero;
    int edad;
};
struct votoInvalido{
    int votoNulo = 0;
    int votoBlanco = 0;
};
struct lista {
    int numero;
    int cantidadVotos;
    float porcentajeVotosValidos;
    string nombre;
    string candidatos[_TOPE_CANDIDATOS];
};

typedef lista tListas[_TOPE_LISTAS];
typedef voto tVotos[_TOPE_VOTOS];

void cargarLista(tListas & listas);
void cargarVotos(tVotos & votos);
void parsear(string cadenaAParsear, voto & votoParseado);
void ordenarVotos(tVotos votos, int desde, int hasta);
int colocar(tVotos votos, int desde, int hasta);
void conteoVotos(tVotos votos, tListas & listas,votoInvalido & votosInvalidos);
void sacarPromedioVotosValidos(tListas & listas, votoInvalido & votosInvalidos);
void mostrarVotos(tVotos votos);
void mostrarDatos(tListas listas, votoInvalido votosInvalidos);
void traer(tVotos votos, voto & v, int cant, int & ptro, bool & fin);
// void procesarVotos();

int main(){
    tListas listas;
    tVotos votos;
    votoInvalido votosInvalidos;
    
    cargarLista(listas);
    cargarVotos(votos);
    ordenarVotos(votos, 0, _TOPE_VOTOS-1);
    //mostrarDatos(listas, votosInvalidos);
    conteoVotos(votos, listas, votosInvalidos);
}

void cargarLista(tListas & listas) {
    string nombreCandidato;
    lista lista;
    int i = 0, j = 0;
    ifstream archivoCandidatos("candidatos.txt");

    while(getline(archivoCandidatos, nombreCandidato)) {
        if (i == _TOPE_CANDIDATOS){
            lista.numero = j + 1;
            lista.nombre = "Lista " + to_string(lista.numero);
            lista.cantidadVotos = 0;
            lista.porcentajeVotosValidos = 0;
            listas[j] = lista;
            i = 0;
            j++;
        }
        lista.candidatos[i] = nombreCandidato;
        i++;
    }

    archivoCandidatos.close(); 
}

void cargarVotos(tVotos & votos) {
    string cadena;
    voto voto;
    int i = 0;
    ifstream archivoVotos("votos.txt");

    while(getline(archivoVotos, cadena)) {
        parsear(cadena, voto);
        votos[i] = voto;
        i++;
    }

    archivoVotos.close();
}

void parsear(string cadenaAParsear, voto & votoParseado) {
    int primero = 0, ultimo = 0, k = 0;
    string palabra[20];

    while(ultimo < cadenaAParsear.size()){
        ultimo = cadenaAParsear.find(";", primero); // cuando no lo encuentra devuelve -1
        if (ultimo == -1) ultimo = cadenaAParsear.size();
        palabra[k] = cadenaAParsear.substr(primero, ultimo - primero) + '\0';
        k++;
        primero = ultimo + 1;
    }

    votoParseado.numero = stoi(palabra[0]);
    votoParseado.genero = palabra[1][0]; 
    votoParseado.edad = stoi(palabra[2]);
}

void ordenarVotos(tVotos votos, int desde, int hasta){
    int pivote;

    if(desde < hasta){
        pivote = colocar(votos, desde, hasta);
        ordenarVotos(votos, desde, pivote - 1);
        ordenarVotos(votos, pivote + 1, hasta);
    }
}

int colocar(tVotos votos, int desde, int hasta){
    int i, pivote = desde;
    voto temp, valor_pivote = votos[pivote];

    for (i = desde + 1; i <= hasta; i++){
        if (votos[i].numero < valor_pivote.numero){
            pivote++;
            temp = votos[i];
            votos[i] = votos[pivote];
            votos[pivote] = temp;
        }
    }

    temp = votos[desde];
    votos[desde] = votos[pivote];
    votos[pivote] = temp;
    return pivote;
}

/*void conteoVotos(tVotos votos, votoInvalido & votosInvalidos, tListas & listas){
    votosInvalidos.votoNulo = 0;
    votosInvalidos.votoBlanco = 0;

    for (int i = 0; i < _TOPE_VOTOS; i++){
        if(votos[i].numero < 0 or votos[i].numero > _TOPE_LISTAS){
            votosInvalidos.votoNulo += 1;
        }
        else if(votos[i].numero == 0){
            votosInvalidos.votoBlanco += 1;
        }
        else{
            for (int j = 0; j < _TOPE_LISTAS; j++){
                if(votos[i].numero == listas[j].numero){
                    listas[j].cantidadVotos += 1;
                    j = _TOPE_LISTAS;
                }
            }
        }
    }

    sacarPromedioVotosValidos(listas, votosInvalidos);
}*/

void clasificarVotos(votoInvalido & votosInvalidos, tListas & listas, int v, int total){
    if(v < 0 || v > _TOPE_LISTAS){
        votosInvalidos.votoNulo = total;
    }
    else if(v == 0){
        votosInvalidos.votoBlanco = total;
    }
    else{
        for (int j = 0; j < _TOPE_LISTAS; j++){
            if(v == listas[j].numero){
                listas[j].cantidadVotos = total;
                j = _TOPE_LISTAS;
            }
        }
    }
}

void conteoVotos (tVotos votos, tListas & listas, votoInvalido & votosInvalidos) {
    int listaAnterior;
    int i = 0;
    voto v;
    bool fin;
    int total = 0;
    int totalLista;
    traer(votos, v, _TOPE_VOTOS, i, fin);
    while (!fin) {
        listaAnterior = v.numero;
        totalLista = 0;
        cout<<"Lista "<<listaAnterior<<endl;
        while ((!fin ) && listaAnterior==v.numero) {
            totalLista ++;
            traer(votos, v, _TOPE_VOTOS, i, fin);
        }
        cout<<"Total listas "<<totalLista<<endl;
        total = total + totalLista;
        clasificarVotos(votosInvalidos, listas, listaAnterior, totalLista);
    }
    cout<<"total ingreso de las listas es "<<total<<endl;
    for (int j = 0; j < _TOPE_LISTAS; j++){
        cout<<listas[j].numero<<endl;
        cout<<listas[j].cantidadVotos<<endl;
    }
}

void traer(tVotos votos, voto & v, int cant, int & ptro, bool & fin) {
    if (ptro<cant) {
        v = votos[ptro];
        ptro++;
        fin = false ;
    } else fin = true;
}

void sacarPromedioVotosValidos(tListas & listas, votoInvalido & votosInvalidos){
    float votosValidos = _TOPE_VOTOS - votosInvalidos.votoNulo - votosInvalidos.votoBlanco;

    for (int i = 0; i < _TOPE_LISTAS; i++){
        listas[i].porcentajeVotosValidos = listas[i].cantidadVotos*100/votosValidos;
    }
}

void mostrarVotos(tVotos votos){
    for (int i = 0; i < _TOPE_VOTOS; i++){   
        if(i + 1 == _TOPE_VOTOS){
            cout<< votos[i].numero;
        }
        else{
            cout<< votos[i].numero<<", ";
        }
    }
    cout<<endl;
    cout<<endl;
}

void mostrarDatos(tListas listas, votoInvalido votosInvalidos){

    cout<<"______________________________________"<<endl;
    cout<<endl;
    cout<<"Votos en Blanco: "<<votosInvalidos.votoBlanco<<endl;
    cout<<"Votos en Nulos: "<<votosInvalidos.votoNulo<<endl;
    cout<<"______________________________________"<<endl;
    cout<<endl;

    for (int i = 0; i < _TOPE_LISTAS; i++){

        cout<<"Numero de la lista: " << listas[i].numero<<endl;
        cout<<"Nombre de la lista: " << listas[i].nombre<<endl;
        cout<<"Cantidad de votos de la lista: " << listas[i].cantidadVotos<<endl;
        cout<<"Cantidad de votos validos: " << listas[i].porcentajeVotosValidos<<"%"<<endl;
        cout<<"Candidatos: "<<endl;
        cout<<"______________Titulares_______________"<<endl;
        for (int j = 0; j < _TOPE_CANDIDATOS; j++)
        {
            if(j == 13){
                cout<<"______________Suplentes_______________"<<endl;
            }
            cout<<j + 1<<") "<<listas[i].candidatos[j]<<endl;

        }
        cout<<"______________________________________"<<endl;
        cout<<endl;

    }
}