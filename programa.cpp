#include <iostream>
#include <cmath>
#include <fstream>
#include <ostream>
#include <string>
#include <string.h>
#include <fstream>
#include <iomanip>
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
    int bancasObtenidas = 0;
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
void ponderacionCantidadVotos(tListas & listas, int & cantListValidas);
void ordenarListas(tListas & listas);
void ordenarCantidadVotos(int votosPond[][3], int cantidad);
void armarListaCantVotos(tListas & listas, int cantListValidas, int votosPond[][3]);
void asignarBancas(tListas & listas, int votosPond[][3]);
int buscarLista(int numLista, tListas & listas);
void tablaDHont(tListas listas);
void imprimirDatosEleccion(tListas listas);

int main(){
    tListas listas;
    tVotos votos;
    votoInvalido votosInvalidos;
    tPondVotos cantVotDiv;
    
    cargarLista(listas);
    cargarVotos(votos);
    ordenarVotos(votos, 0, _TOPE_VOTOS-1);
    conteoVotos(votos, listas, votosInvalidos);
    ordenarListas(listas);
    DHont(listas,votosInvalidos);
    //mostrarDatos(listas, votosInvalidos);
    tablaDHont(listas);
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
        cout<<"Nombre de la lista: " << listas[i].nombre<<endl;
        cout<<"Cantidad de votos de la lista: " << listas[i].cantidadVotos<<endl;
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
    int cantListValidas = 0;
    ponderacionCantidadVotos(listas, cantListValidas);
    int cantidad = _TOPE_BANCAS * cantListValidas;
    int votosPond[cantidad][3];
    armarListaCantVotos(listas, cantListValidas, votosPond);
    ordenarCantidadVotos(votosPond, cantidad);
    asignarBancas(listas, votosPond);

}

void ponderacionCantidadVotos(tListas & listas, int & cantListValidas){
    for (int i = 0; i < _TOPE_LISTAS; i++){
        if(listas[i].porcentajeVotosValidos >= 3.0){
            cantListValidas++;
            for (int j = 0; j < _TOPE_BANCAS; j++){
                listas[i].votosPonderados[j] = listas[i].cantidadVotos/(j+1);
            }
        } else listas[i].bancasObtenidas = 0;
    }
}

void armarListaCantVotos(tListas & listas, int cantListValidas, int votosPond[][3]){
    int x = 0;
    
    for (int i = 0; i < cantListValidas; i++){
        for (int j = 0; j < _TOPE_BANCAS; j++){
            votosPond[x][0]=listas[i].votosPonderados[j];
            votosPond[x][1]=listas[i].numero;
            votosPond[x][2]=listas[i].porcentajeVotosValidos;
            x++;
        }
    }
}

void ordenarCantidadVotos(int votosPond[][3], int cantidad){
    int cantAux, listaAux;
    float porcentajeAux;
    
    for(int i = 0; i < cantidad ; i++){
        for(int j = i + 1 ; j < cantidad; j++){
            // si el voto dividio por x es mayor que el siguiente o es igual pero el porcentaje es mayor que el siguiente
            if(votosPond[j][0] > votosPond[i][0]  || (votosPond[i][0] == votosPond[j][0] && votosPond[i][2] < votosPond[j][2])) {
                cantAux=votosPond[i][0];
                listaAux=votosPond[i][1];
                porcentajeAux=votosPond[i][2];
                votosPond[i][0]=votosPond[j][0];
                votosPond[i][1]=votosPond[j][1];
                votosPond[i][2]=votosPond[j][2];
                votosPond[j][0]=cantAux;
                votosPond[j][1]=listaAux;
                votosPond[j][2]=porcentajeAux;
            }
        }
    }


}

void asignarBancas(tListas & listas, int votosPond[][3]){
    for (int i = 0; i < _TOPE_BANCAS; i++){
        cout<<"asignarbancas"<<endl;
        cout<< "Voto numero "<<i<<" :" <<votosPond[i][0]<<"," <<votosPond[i][1]<<endl;
        cout<<buscarLista(votosPond[i][1],listas)<<endl;
        //cout<<listas[buscarLista(votosPond[i][1],listas)].bancasObtenidas<<endl;
        listas[buscarLista(votosPond[i][1],listas)].bancasObtenidas++;
    }
}

int buscarLista(int numLista, tListas & listas){
    int i = 0;
    
    while (i<_TOPE_LISTAS && numLista!=listas[i].numero){
        i = i+1;
    }
    if (i>_TOPE_LISTAS || listas[i].numero!=numLista) {
        i = -1;
    }

    return i;
}

void tablaDHont(tListas listas){
    ofstream archivoTexto;
    archivoTexto.open ("Tabla_de_resultados.txt");
    streambuf* sbuf = cout.rdbuf();
    cout.rdbuf(archivoTexto.rdbuf());
    
    cout << left << setw(7)  << "Listas"                      << left << setw(2) << "|" 
         << left << setw(18) << "Cantidad de votos"           << left << setw(2) << "|"
         << left << setw(7)  << "Porcentaje de votos validos" << left << setw(2) << "|"
         << left << setw(7)  << "01° banca"                   << left << setw(2) << "|"
         << left << setw(7)  << "02° banca"                   << left << setw(2) << "|"
         << left << setw(7)  << "03° banca"                   << left << setw(2) << "|"
         << left << setw(7)  << "04° banca"                   << left << setw(2) << "|"
         << left << setw(7)  << "05° banca"                   << left << setw(2) << "|"
         << left << setw(7)  << "06° banca"                   << left << setw(2) << "|"
         << left << setw(7)  << "07° banca"                   << left << setw(2) << "|"
         << left << setw(7)  << "08° banca"                   << left << setw(2) << "|"
         << left << setw(7)  << "09° banca"                   << left << setw(2) << "|"
         << left << setw(7)  << "10° banca"                   << left << setw(2) << "|"
         << left << setw(7)  << "11° banca"                   << left << setw(2) << "|"
         << left << setw(7)  << "12° banca"                   << left << setw(2) << "|"
         << left << setw(7)  << "13° banca"                   << left << setw(2) << "|"
         << left << setw(37) << "Ganan"                       << left << setw(2) << "|"
         <<endl;

    imprimirDatosEleccion(listas);

    cout.rdbuf(sbuf);
    cout << "La tabla con los resultados de la elección fué ingresada en 'Tabla_de_resultados.txt'" << endl;
}

void imprimirDatosEleccion(tListas listas){
    for (int  i = 0; i < _TOPE_LISTAS; i++){
        cout << left << setw(7)  << listas[i].numero                 << left << setw(2) << "|"
             << left << setw(18) << listas[i].cantidadVotos          << left << setw(2) << "|"
             << listas[i].porcentajeVotosValidos << left << setw(20) << "%" << left << setw(2) << "|" << left << setw(9);
        for (int j = 0; j < _TOPE_BANCAS; j++){
            cout << listas[i].votosPonderados[j] << left << setw(2) << "|" << left << setw(9);
        }

        if(listas[i].bancasObtenidas == 0){
            cout << left << setw(7) << "Obtuvo menos del 3%, queda DESCARTADA" << left << setw(2) << "|"; 
        }else{
            cout << left << setw(7) << "Obtiene " << listas[i].bancasObtenidas << left << setw(28);
            if(listas[i].bancasObtenidas == 1){
                cout << " banca" << left << setw(2) << "|"; 
            }else cout << " bancas" << left << setw(2) << "|";
        }
        
        cout<<endl;
    }
}