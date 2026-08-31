using namespace std;

#include <cstring> 
#include <iostream>

const int CORRIMIENTO = 5;
const float TASA_COMISION = 0.10f;

struct ComandaHistorica
{
    char fecha[11];
    char nombreMozo[50];
    int codigoProducto;
};

struct Mozo
{
    int idMozo; 
    char nombre[50]; 
    char password[20]; 
    float totalComision;
};

struct Comanda
{
    int idMozo;
    int codigoProducto;
    int cantidad;
    float comision;
};


float calcularComision(float precio, int cantidad)
{
    return precio * cantidad * TASA_COMISION;
}

bool encriptarYValidar(char clave[], int len, Mozo mozo)
{

    char claveEncriptada[len];

    int i = 0;
    while (i <= len)
    {
       claveEncriptada[i] = clave[i] + CORRIMIENTO;
       i++;
    }

    return strcmp(claveEncriptada, mozo.password) == 0; 
       
}


void crearNombreArchivo(char fecha[], char nombreArchivo[])
{
    strcpy(nombreArchivo, "comandas_");
    strcat(nombreArchivo, fecha);
    strcat(nombreArchivo, ".dat");   
}

long buscarMozo(char nombreArchivo[], int idBuscada, Mozo &m)
{
    FILE* f = fopen(nombreArchivo, "rb+");

    fseek(f, 0, SEEK_END);
    long n = ftell(f) / sizeof(Mozo);
    long primero = 0, ultimo = n - 1, pos = -1;

    while(primero <= ultimo && pos == -1)
    {
        long medio = (primero + ultimo) / 2;
        fseek(f, medio * sizeof(Mozo), SEEK_SET);
        fread(&m, sizeof(Mozo), 1, f);
        if(m.idMozo == idBuscada) pos = medio;
        else if(idBuscada > m.idMozo) primero = medio + 1;
        else ultimo = medio - 1; 
    }

    fclose(f);
    return pos;

}

int main(int argc, char const *argv[])
{
    char fecha[20];
    char nombreArchivo[50];
    cout << "Ingrese la fecha (DD-MM-AAAA): ";
    cin >> fecha;

    crearNombreArchivo(fecha, nombreArchivo);

    FILE* archivo = fopen(nombreArchivo, "ab+");

    return 0;
    
}
