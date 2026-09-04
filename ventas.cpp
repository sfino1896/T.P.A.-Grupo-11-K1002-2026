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

struct Producto {
    int codigo;
    char descripcion[50];
    float precio;
    int stockActual;
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
    FILE* f = fopen(nombreArchivo, "rb");

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


long buscarProducto(char nombreArchivo[], int codigo, Producto &p)
{
    FILE* f = fopen(nombreArchivo, "rb");

    fseek(f, 0, SEEK_END);
    long n = ftell(f) / sizeof(Producto);
    long primero = 0, ultimo = n - 1, pos = -1;

    while(primero <= ultimo && pos == -1)
    {
        long medio = (primero + ultimo) / 2;
        fseek(f, medio * sizeof(Producto), SEEK_SET);
        fread(&p, sizeof(Producto), 1, f);
        if(p.codigo == codigo) pos = medio;
        else if(codigo > p.codigo) primero = medio + 1;
        else ultimo = medio - 1;
    }
    fclose(f);
    return pos;
}

bool hayStock(char nombreArchivo[], long pos, Producto &p, int cantidad)
{
    FILE* f = fopen(nombreArchivo, "rb");
    fseek(f, pos * sizeof(Producto), SEEK_SET);
    fread(&p, sizeof(Producto), 1, f);
    return (p.stockActual <= cantidad) ? false : true;
}

int main(int argc, char const *argv[])
{
    char fecha[20];
    char nombreArchivo[50];
    cout << "Ingrese la fecha (DD-MM-AAAA): " << endl;
    cin >> fecha;

    crearNombreArchivo(fecha, nombreArchivo);

    FILE* archivo = fopen(nombreArchivo, "ab+");

    int idMozo;
    cout << "Ingrese id del Mozo: " << endl;
    cin >> idMozo;

    Mozo m;

    int posicionMozo = buscarMozo("mozos.dat", idMozo, m);

    while(posicionMozo == -1)
    {
        cout << "No se encontró un mozo con esa ID, intente nuevamente: " << endl;
        cin >> idMozo;
        posicionMozo = buscarMozo("mozos.dat", idMozo, m);
    }

    char clave[20];
    cout << "Ingrese la clave: " << endl;
    cin >> clave[20];

    if(!encriptarYValidar(clave, 20, m))
    {
        cout << "Clave incorrecta" << endl;
    }
    else
    {
        Producto p;
        int codigoProducto;
        cout << "Ingrese un producto" << endl;
        cin >> codigoProducto;
        int posicionProducto = buscarProducto("inventario.dat", codigoProducto, p);
        while(posicionProducto == -1)
        {
            cout << "No se encontró el producto, intente nuevamente: " << endl;
            cin >> codigoProducto;
            posicionProducto = buscarProducto("inventario.dat", codigoProducto, p); 
        }
        int cantidad;
        cout << "Ingrese cantidad: " << endl;
        cin >> cantidad;
        if(hayStock("inventario.dat", posicionProducto, p, cantidad))
        {
            
            float comision = calcularComision(p.precio, cantidad);
            Comanda c;
            c.idMozo = idMozo;
            c.codigoProducto = codigoProducto;
            c.cantidad = cantidad;
            c.comision = comision;

            //Guarda la comanda al final del archivo del día
            fseek(archivo, 0, SEEK_END);
            fwrite(&c, sizeof(Comanda), 1, archivo);

            //Actualiza el stock del producto
            p.stockActual -= cantidad;

            FILE* inventario = fopen("inventario.dat", "wb+");
            fseek(inventario, posicionProducto * sizeof(Producto), SEEK_SET);
            fwrite(&p, sizeof(Producto), 1, archivo);

        }

    }


    return 0;
    
}
