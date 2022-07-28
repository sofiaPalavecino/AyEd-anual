#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

int main ()
{
    ofstream file;
    file.open ("Tabla_de_resultados.txt");
    streambuf* sbuf = cout.rdbuf();
    cout.rdbuf(file.rdbuf());
    
    cout << left << setw(7)     << "Listas"                       << left << setw(2) << "|" 
         << left << setw(18)    << "Cantidad de votos"            << left << setw(2) << "|"
         << left << setw(7)     << "Porcentaje de votos validos"  << left << setw(2) << "|"
         << left << setw(15)    << "Primera banca"                << left << setw(2) << "|"
         << left << setw(15)    << "Segunda banca"                << left << setw(2) << "|"
         << left << setw(15)    << "Tercera banca"                << left << setw(2) << "|"
         << left << setw(15)    << "Cuarta banca"                 << left << setw(2) << "|"
         << left << setw(15)    << "Quinta banca"                 << left << setw(2) << "|"
         << left << setw(15)    << "Sexta banca"                  << left << setw(2) << "|"
         << left << setw(15)    << "Séptima banca"                << left << setw(2) << "|"
         << left << setw(15)    << "Octava banca"                 << left << setw(2) << "|"
         << left << setw(15)    << "Novena banca"                 << left << setw(2) << "|"
         << left << setw(15)    << "Décima banca"                 << left << setw(2) << "|"
         << left << setw(15)    << "Onceava banca"                << left << setw(2) << "|"
         << left << setw(15)    << "Doceava banca"                << left << setw(2) << "|"
         << left << setw(15)    << "Treceava banca"               << endl;


    cout.rdbuf(sbuf);
    cout << "La tabla con los resultados de la elección fué ingresada en 'Tabla_de_resultados.txt'" << endl;
}