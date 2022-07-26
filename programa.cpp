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
#define _TOPE_BANCAS 13
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
    int bancasObtenidas;
    int votosPonderados[_TOPE_BANCAS];
};

typedef lista tListas[_TOPE_LISTAS];
typedef voto tVotos[_TOPE_VOTOS];
typedef int tPondVotos[_TOPE_LISTAS][_TOPE_BANCAS];

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
void DHont(tListas & listas, votoInvalido votosInvalidos);
void ponderacionCantidadVotos(tListas & listas);
void ordenarListas(tListas & listas);
void buscarElMayor(tListas & listas, int & tope, int & numLista);

int main(){
    tListas listas;
    tVotos votos;
    votoInvalido votosInvalidos;
    tPondVotos cantVotDiv;
    
    cargarLista(listas);
    cargarVotos(votos);
    ordenarVotos(votos, 0, _TOPE_VOTOS-1);
    conteoVotos(votos, listas, votosInvalidos);
    //mostrarDatos(listas, votosInvalidos);
    ordenarListas(listas);
    DHont(listas,votosInvalidos);
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

void ordenarListas(tListas & listas){
    lista aux;
    for(int i = 0; i < _TOPE_LISTAS ; i++){
        for(int j = i + 1 ; j < _TOPE_LISTAS ; j++){
            if(listas[i].cantidadVotos < listas[j].cantidadVotos) {
                aux=listas[i];
                listas[i]=listas[j];
                listas[j]=aux;
            }
        }
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
    sacarPromedioVotosValidos(listas, votosInvalidos);
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

        cout<<"Número de la lista: " << listas[i].numero<<endl;
        //cout<<"Nombre de la lista: " << listas[i].nombre<<endl;
        //cout<<"Cantidad de votos de la lista: " << listas[i].cantidadVotos<<endl;
        cout<<"Porcentaje dentro de los votos válidos: " << listas[i].porcentajeVotosValidos<<"%"<<endl;
        cout<<"Ponderacion"<<endl;
        for (int j = 0; j < _TOPE_BANCAS; j++)
        {
            cout<<listas[i].votosPonderados[j]<<" ";
        }
        cout<<endl;
        cout<<"Bancas" << listas[i].bancasObtenidas<<endl;
        /*cout<<"Candidatos: "<<endl;
        cout<<"______________Titulares_______________"<<endl;
        for (int j = 0; j < _TOPE_CANDIDATOS; j++)
        {
            if(j == 13){
                cout<<"______________Suplentes_______________"<<endl;
            }
            cout<<j + 1<<") "<<listas[i].candidatos[j]<<endl;

        }
        cout<<"______________________________________"<<endl;
        cout<<endl;*/

    }
}

void DHont(tListas & listas, votoInvalido votosInvalidos){
    ponderacionCantidadVotos(listas);
    mostrarDatos(listas, votosInvalidos);
    int i = 1;
    int tope = _TOPE_VOTOS;
    int numLista = 0;
    while (i<=_TOPE_BANCAS)
    {
        buscarElMayor(listas, tope, numLista);
        i++;
    }
    mostrarDatos(listas, votosInvalidos);
}

void ponderacionCantidadVotos(tListas & listas){
    for (int i = 0; i < _TOPE_LISTAS; i++){
        if(listas[i].porcentajeVotosValidos >= 3.0){
            for (int j = 0; j < _TOPE_BANCAS; j++){
                listas[i].votosPonderados[j] = listas[i].cantidadVotos/(j+1);
            }
        } else listas[i].bancasObtenidas = 0;
    }
}

void buscarElMayor(tListas & listas, int & tope, int & numLista){
    int mayor = 0;
    int ponVoto = 0;
    int posicion = 0;
    //lista con todos los valores y numero de lista al que pertenecen :), ordenarla de menor a mayor y guardarse los primeros 13 valores
    for (int i = 0; i < _TOPE_LISTAS; i++)
    {
        for (int j = 0; j < _TOPE_BANCAS; j++)
        {
            if(listas[i].votosPonderados[j]>mayor && listas[i].votosPonderados[j]<=tope && listas[i].porcentajeVotosValidos >= 3.0){
                mayor = listas[i].votosPonderados[j];
                numLista = listas[i].numero;
                posicion = i;
                ponVoto = j;
            } 
        }
        
    }

    tope = mayor;
    cout<<mayor<<endl;
    cout<<numLista<<endl;

    for (int i = 0; i < _TOPE_BANCAS; i++)
    {
        if(listas[i].numero==numLista){
            listas[i].bancasObtenidas++;
            i = _TOPE_BANCAS;
        }
    }
    listas[posicion].votosPonderados[ponVoto] = 0;
}