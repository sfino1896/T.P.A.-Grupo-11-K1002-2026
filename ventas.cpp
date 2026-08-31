using namespace std;

#include <cstring> 


const int CORRIMIENTO = 5;
const float TASA_COMISION = 0.10f;

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

int main(int argc, char const *argv[])
{

    return 0;
    
}
