#include <iostream>
#include <cmath>
#include <fstream>
#include <ostream>
#include <string>
#include <string.h>
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
void traer(tVotos votos, voto & v, int cant, int & ptro, bool & fin);
void clasificarVotos(votoInvalido & votosInvalidos, tListas & listas, int v, int total);
void sacarPromedioVotosValidos(tListas & listas, votoInvalido & votosInvalidos);
void ordenarListas(tListas & listas);
void DHont(tListas & listas, votoInvalido votosInvalidos);
void ponderacionCantidadVotos(tListas & listas, int & cantListValidas);
void armarListaCantVotos(tListas listas, int cantListValidas, int votosPond[][3]);
void ordenarCantidadVotos(int votosPond[][3], int cantidad);
void asignarBancas(tListas & listas, int votosPond[][3]);
int buscarLista(int numLista, tListas listas);
void mostrarDatos(tListas listas, votoInvalido votosInvalidos);
void tablaDHont(tListas listas, votoInvalido & votosInvalidos, tVotos votos);
void imprimirVotosValidos(tListas listas);
void imprimirVotosInvalidos(tListas listas, votoInvalido & votosInvalidos);
int agregarMargen(int margen, int numero, int banca2Digitos = 0);
void verRangoEdad(voto v, int listaAnterior,  int & hasta18, int & hasta30, int & hasta50, int & masDe50);
void MostrarRangosEdad(voto v, int listaAnterior,  int & hasta18, int & hasta30, int & hasta50, int & masDe50);
void imprimirRangoEdades(tListas listas, tVotos votos);
void imprimirEncabezado();

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
    // mostrarDatos(listas, votosInvalidos);
    tablaDHont(listas, votosInvalidos, votos);
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

void conteoVotos (tVotos votos, tListas & listas, votoInvalido & votosInvalidos) {
    int listaAnterior, totalLista, total = 0, i = 0;
    bool fin;
    voto v;

    traer(votos, v, _TOPE_VOTOS, i, fin);
    while (!fin) {
        listaAnterior = v.numero;
        totalLista = 0;
        while ((!fin ) && listaAnterior == v.numero) {
            totalLista ++;
            traer(votos, v, _TOPE_VOTOS, i, fin);
        }
        total = total + totalLista;
        clasificarVotos(votosInvalidos, listas, listaAnterior, totalLista);
    }

}

void traer(tVotos votos, voto & v, int cant, int & ptro, bool & fin) {
    if (ptro<cant) {
        v = votos[ptro];
        ptro++;
        fin = false ;
    } else fin = true;
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

void sacarPromedioVotosValidos(tListas & listas, votoInvalido & votosInvalidos){
    float votosValidos = _TOPE_VOTOS - votosInvalidos.votoNulo - votosInvalidos.votoBlanco;

    for (int i = 0; i < _TOPE_LISTAS; i++){
        listas[i].porcentajeVotosValidos = listas[i].cantidadVotos*100/votosValidos;
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

void armarListaCantVotos(tListas listas, int cantListValidas, int votosPond[][3]){
    int x = 0;
    
    for (int i = 0; i < cantListValidas; i++){
        for (int j = 0; j < _TOPE_BANCAS; j++){
            votosPond[x][0] = listas[i].votosPonderados[j];
            votosPond[x][1] = listas[i].numero;
            votosPond[x][2] = listas[i].porcentajeVotosValidos;
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
            if(votosPond[j][0] > votosPond[i][0] || (votosPond[j][0] == votosPond[i][0] && votosPond[j][2] > votosPond[i][2])) {
                cantAux = votosPond[i][0];
                listaAux = votosPond[i][1];
                porcentajeAux = votosPond[i][2];
                votosPond[i][0] = votosPond[j][0];
                votosPond[i][1] = votosPond[j][1];
                votosPond[i][2] = votosPond[j][2];
                votosPond[j][0] = cantAux;
                votosPond[j][1] = listaAux;
                votosPond[j][2] = porcentajeAux;
            }
        }
    }


}

void asignarBancas(tListas & listas, int votosPond[][3]){
    cout<<endl;
    for (int i = 0; i < _TOPE_BANCAS; i++){
        // cout<<"Asignar bancas"<<endl;
        // cout<< "Voto numero "<<i<<": "<<"ponderacion del voto: "<<votosPond[i][0]<<", numero de lista: "<<votosPond[i][1]<<endl;
        // cout<<buscarLista(votosPond[i][1], listas)<<endl;
        listas[buscarLista(votosPond[i][1], listas)].bancasObtenidas++;
    }
}

int buscarLista(int numLista, tListas listas){
    int i = 0;
    
    while (i < _TOPE_LISTAS && numLista != listas[i].numero){
        i ++;
    }

    if (i > _TOPE_LISTAS || listas[i].numero != numLista) {
        i = -1;
    }

    return i;
}

void mostrarDatos(tListas listas, votoInvalido votosInvalidos){

    cout<<"Votos en Blanco: "<<votosInvalidos.votoBlanco<<endl;
    cout<<"Votos en Nulos: "<<votosInvalidos.votoNulo<<endl;
    
    for (int i = 0; i < _TOPE_LISTAS; i++){
        cout<<endl;
        cout<<"Numero de la lista: " << listas[i].numero<<endl;
        cout<<"Nombre de la lista: " << listas[i].nombre<<endl;
        cout<<"Cantidad de votos de la lista: " << listas[i].cantidadVotos<<endl;
        cout<<"Porcentaje dentro de los votos validos: " << listas[i].porcentajeVotosValidos<<"%"<<endl;
        cout<<"Ponderacion: ";

        for (int j = 0; j < _TOPE_BANCAS; j++)
        {
            if(j + 1 == _TOPE_BANCAS){
                cout<< listas[i].votosPonderados[j];
            }
            else{
                cout<< listas[i].votosPonderados[j]<<", ";
            }
        }

        cout<<endl;
        cout<<"Obtiene "<< listas[i].bancasObtenidas<<" bancas."<<endl;
        cout<<"Candidatos que obtuvieron banca: ";

        for (int k = 0; k < listas[i].bancasObtenidas; k++){
            if(k + 1 == listas[i].bancasObtenidas){
                cout<< listas[i].candidatos[k];
            }
            else{
                cout<< listas[i].candidatos[k]<<", ";
            }
        }
        cout<<endl;

    }
}

void tablaDHont(tListas listas, votoInvalido & votosInvalidos, tVotos votos){
    ofstream archivoTexto("Tabla_de_resultados.txt");
    streambuf* sbuf = cout.rdbuf();
    cout.rdbuf(archivoTexto.rdbuf());

    cout << "|" << " Listas " << setw(9) << "|" << " Cantidad de votos " << "|" << " Porcentaje de votos validos " << "|";

    for (int i = 0; i < _TOPE_BANCAS; i++){
        cout << " " << i + 1 << "° banca " << "|";
    }

    cout << " Bancas que ganan cada lista " << setw(11) << "|" << " Nombre de candidatos en las bancas " << endl;

    imprimirVotosValidos(listas);
    imprimirVotosInvalidos(listas, votosInvalidos);
    imprimirRangoEdades(listas, votos);

    cout.rdbuf(sbuf);
    cout << "La tabla con los resultados de la eleccion fue ingresada en 'Tabla_de_resultados.txt'" << endl;
};

void imprimirVotosValidos(tListas listas){
    for (int  i = 0; i < _TOPE_LISTAS; i++){

        // listas, cantidad de votos, % de votos
        cout << "|" << " Lista " << listas[i].numero << setw(agregarMargen(9, listas[i].numero, 0)) << "|";
        cout << " " << listas[i].cantidadVotos << setw(agregarMargen(18, listas[i].cantidadVotos, 0)) << "|"
        << " " << listas[i].porcentajeVotosValidos << "%" << setw(21) << "|";

        // bancas
        for (int j = 0; j < _TOPE_BANCAS; j++){
            if(listas[j].bancasObtenidas == 0 and j < _TOPE_LISTAS){
                cout << " " << setw(10) << "|";
            }
            else{
                cout << " " << listas[i].votosPonderados[j] << setw(agregarMargen(9, listas[i].votosPonderados[j], j)) << "|";
            }
        }

        // ganan
        if(listas[i].bancasObtenidas == 0){
            cout << " Obtuvo menos del 3%, queda DESCARTADA " << "|";
        }else{
            cout << " Obtiene " << listas[i].bancasObtenidas;

            if(listas[i].bancasObtenidas == 1){
                cout << " banca " << setw(23) << "|";
            }else cout << " bancas " << setw(agregarMargen(22, listas[i].bancasObtenidas, 0)) << "|";
        }

        // candidatos
        for (int k = 0; k < listas[i].bancasObtenidas; k++){
            if(k + 1 == listas[i].bancasObtenidas){
                cout << " " << listas[i].candidatos[k]<<endl;
            }else cout << " " << listas[i].candidatos[k] << ",";
        }
    }
}

void imprimirVotosInvalidos(tListas listas, votoInvalido & votosInvalidos){
    for (int i = 0; i < 2; i++){

        if(i == 0){
            cout << "|" << " Voto en blanco " << "|" << " " << votosInvalidos.votoBlanco << setw(agregarMargen(18, votosInvalidos.votoBlanco, 0)) << "|";
        }else{
            cout << "|" << " Voto Nulo " << setw(6) << "|" << " " << votosInvalidos.votoNulo << setw(agregarMargen(18, votosInvalidos.votoNulo, 0)) << "|";
            
        }

        // % votos validos
        cout << setw(30) << "|";

        // bancas
        for (int j = 0; j < _TOPE_BANCAS ; j++){
            cout << setw(agregarMargen(11, 0, j)) << "|";
        }
        // ganan
        cout << setw(40) << "|" <<endl;
    }
}

int agregarMargen(int margen, int numero, int banca2Digitos){

    if(banca2Digitos > 8){
        margen += 1;
    }

    if (numero < 10){
        return margen; // si entra aca tiene 1 caracter
    }
    else if (numero < 100){
        return margen - 1; // si entra aca tiene 2 caracter
    }
    else if (numero < 1000){
        return margen - 2; // si entra aca tiene 3 caracter
    }
    
}

void imprimirRangoEdades(tListas listas, tVotos votos){
    imprimirEncabezado();
    int listaAnterior, i = 0, hasta18 = 0, hasta30 = 0, hasta50 = 0, masDe50 = 0;
    bool fin;
    voto v;
    
    traer(votos, v, _TOPE_VOTOS, i, fin);

    while (!fin) {
        listaAnterior = v.numero;
        while ((!fin ) && listaAnterior == v.numero) {
            verRangoEdad(v, listaAnterior, hasta18, hasta30, hasta50, masDe50);
            traer(votos, v, _TOPE_VOTOS, i, fin);
        }
        MostrarRangosEdad(v, listaAnterior,hasta18, hasta30, hasta50, masDe50);
    }
}

void imprimirEncabezado(){
    cout << endl << endl;
    cout<<"| Numero de la lista |";
    cout<<" Cantidad de Votantes de hasta 18 años |";
    cout<<" Cantidad de Votantes de hasta 30 años |";
    cout<<" Cantidad de Votantes de hasta 50 años |";
    cout<<" Cantidad de Votantes mayores a 50 años"<<endl;
}

void verRangoEdad(voto v, int listaAnterior, int & hasta18, int & hasta30, int & hasta50, int & masDe50){
    if(listaAnterior > 0 && listaAnterior < _TOPE_LISTAS){
        if (v.edad < 18) {
            hasta18++;
        } else if (v.edad < 30) {
            hasta30++;
        } else if (v.edad < 50) {
            hasta50++;
        } else {
            masDe50++;
        }
    }

}

void MostrarRangosEdad(voto v, int listaAnterior, int & hasta18, int & hasta30, int & hasta50, int & masDe50){
    if(listaAnterior > 0 && listaAnterior < _TOPE_LISTAS){
        cout<<"| Lista "<<listaAnterior ;
        cout << setw(14)<<"| " << hasta18;
        cout << setw(agregarMargen(39, hasta18, 0))<<"| " << hasta30;
        cout << setw(agregarMargen(39, hasta30, 0))<<"| " << hasta50;
        cout << setw(agregarMargen(39, hasta50, 0))<<"| " << masDe50 << endl;
        hasta18 = 0;
        hasta30 = 0;
        hasta50 = 0;
        masDe50 = 0;
    }
}