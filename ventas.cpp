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
