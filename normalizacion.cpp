#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstring>

using namespace std;

//  Structs tal cual la ficha tecnica

struct ComandaHistorica {
    char fecha[11];        // "DD-MM-AAAA"
    char nombreMozo[50];
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

struct Mozo {
    int idMozo;
    char nombre[50];
    char password[20];
    float totalComision;
};

struct Comanda {
    int idMozo;
    int codigoProducto;
    int cantidad;
    float comision;
};

//Constantes

const int MAX_HISTORICAS = 20000; // tope de ventas historicas que soportamos en memoria
const int MAX_MOZOS = 200;
const int MAX_PRODUCTOS = 500;
const int MAX_FECHAS = 100;

const int K = 5; // corrimiento para "encriptar" las claves (facil de deshacer: restando K)

// Encriptado de claves 
// Se puede deshacer restando K a cada caracter. Elegimos suma/resta y no division
// porque la division pierde informacion (varios valores de entrada dan el mismo
// resultado) y no se puede volver al valor original.

void encriptar(char destino[20], const char* origen) {
    int i = 0;
    for (; origen[i] != '\0' && i < 19; i++) {
        destino[i] = (char)(origen[i] + K);
    }
    destino[i] = '\0';
}

// Buscar o crear un mozo por nombre en el arreglo en memoria
// Busqueda lineal: en este punto los mozos todavia no tienen ningun orden
// particular (van apareciendo a medida que leemos el historico), asi que no
// hay forma de hacer algo mejor que recorrer.

int buscarOCrearMozo(Mozo mozos[], int &cantMozos, const char* nombre) {
    for (int i = 0; i < cantMozos; i++) {
        if (strcmp(mozos[i].nombre, nombre) == 0) {
            return i;
        }
    }

    // No existia: lo creamos
    Mozo nuevo;
    nuevo.idMozo = cantMozos + 1; // los ids arrancan en 1
    strncpy(nuevo.nombre, nombre, 50);
    nuevo.nombre[49] = '\0';
    nuevo.totalComision = 0.0f;

    // Clave inicial: el numero de mozo como texto, ya encriptada
    string claveInicial = to_string(nuevo.idMozo);
    encriptar(nuevo.password, claveInicial.c_str());

    mozos[cantMozos] = nuevo;
    cantMozos++;
    return cantMozos - 1;
}

// Busqueda binaria de un producto por codigo 
// El inventario esta ordenado por codigo (aunque con huecos), asi que
// conviene busqueda binaria en vez de lineal.

int buscarProductoPorCodigo(Producto productos[], int cantProductos, int codigo) {
    int izq = 0, der = cantProductos - 1;
    while (izq <= der) {
        int medio = (izq + der) / 2;
        if (productos[medio].codigo == codigo) {
            return medio;
        } else if (productos[medio].codigo < codigo) {
            izq = medio + 1;
        } else {
            der = medio - 1;
        }
    }
    return -1; // no deberia pasar con datos consistentes, pero por las dudas
}

// Ordenar un arreglo de Comanda por idMozo (seleccion) 
// El volumen por dia es chico (las ventas de una jornada), asi que un
// metodo simple como seleccion es mas que suficiente y facil de explicar.

void ordenarPorMozo(Comanda comandas[], int cant) {
    for (int i = 0; i < cant - 1; i++) {
        int menor = i;
        for (int j = i + 1; j < cant; j++) {
            if (comandas[j].idMozo < comandas[menor].idMozo) {
                menor = j;
            }
        }
        if (menor != i) {
            Comanda tmp = comandas[i];
            comandas[i] = comandas[menor];
            comandas[menor] = tmp;
        }
    }
}

int main() {

    //1) Leer todo el historico a memoria 
    FILE* fHist = fopen("comandas_historicas.dat", "rb");
    if (fHist == NULL) {
        cout << "No se pudo abrir comandas_historicas.dat" << endl;
        return 1;
    }

    ComandaHistorica* historicas = new ComandaHistorica[MAX_HISTORICAS];
    int cantHistoricas = 0;
    while (fread(&historicas[cantHistoricas], sizeof(ComandaHistorica), 1, fHist) == 1) {
        cantHistoricas++;
    }
    fclose(fHist);
    cout << "Ventas historicas leidas: " << cantHistoricas << endl;

    //2) Armar la lista de mozos con su comision acumulada
    Mozo mozos[MAX_MOZOS];
    int cantMozos = 0;

    for (int i = 0; i < cantHistoricas; i++) {
        int idx = buscarOCrearMozo(mozos, cantMozos, historicas[i].nombreMozo);
        mozos[idx].totalComision += historicas[i].comision;
    }

    FILE* fMozos = fopen("mozos.dat", "wb");
    fwrite(mozos, sizeof(Mozo), cantMozos, fMozos);
    fclose(fMozos);
    cout << "Mozos generados: " << cantMozos << " (mozos.dat)" << endl;

    //3) Separar las ventas por dia, con idMozo y ordenadas 
    char fechasProcesadas[MAX_FECHAS][11];
    int cantFechas = 0;

    for (int i = 0; i < cantHistoricas; i++) {

        // Si ya procesamos esta fecha, la saltamos
        bool yaProcesada = false;
        for (int f = 0; f < cantFechas; f++) {
            if (strcmp(fechasProcesadas[f], historicas[i].fecha) == 0) {
                yaProcesada = true;
                break;
            }
        }
        if (yaProcesada) continue;

        // Juntamos TODAS las ventas de esta fecha
        Comanda delDia[MAX_HISTORICAS];
        int cantDelDia = 0;
        for (int j = 0; j < cantHistoricas; j++) {
            if (strcmp(historicas[j].fecha, historicas[i].fecha) == 0) {
                int idxMozo = buscarOCrearMozo(mozos, cantMozos, historicas[j].nombreMozo);
                delDia[cantDelDia].idMozo = mozos[idxMozo].idMozo;
                delDia[cantDelDia].codigoProducto = historicas[j].codigoProducto;
                delDia[cantDelDia].cantidad = historicas[j].cantidad;
                delDia[cantDelDia].comision = historicas[j].comision;
                cantDelDia++;
            }
        }

        ordenarPorMozo(delDia, cantDelDia);

        // Nombre de archivo: comandas_DD-MM-AAAA.dat (la fecha ya viene en ese formato)
        string nombreArchivo = string("comandas_") + historicas[i].fecha + ".dat";

        FILE* fDia = fopen(nombreArchivo.c_str(), "wb");
        fwrite(delDia, sizeof(Comanda), cantDelDia, fDia);
        fclose(fDia);

        cout << "Planilla generada: " << nombreArchivo << " (" << cantDelDia << " ventas)" << endl;

        strcpy(fechasProcesadas[cantFechas], historicas[i].fecha);
        cantFechas++;
    }

    // 4) Actualizar el stock del inventario 
    // Lo leemos completo a memoria, descontamos lo vendido, y despues
    // reescribimos SOLO los registros que cambiaron con fseek + fwrite
    // (no recreamos el archivo de cero).

    FILE* fInv = fopen("inventario.dat", "rb");
    if (fInv == NULL) {
        cout << "No se pudo abrir inventario.dat" << endl;
        delete[] historicas;
        return 1;
    }

    Producto productos[MAX_PRODUCTOS];
    int cantProductos = 0;
    while (fread(&productos[cantProductos], sizeof(Producto), 1, fInv) == 1) {
        cantProductos++;
    }
    fclose(fInv);

    for (int i = 0; i < cantHistoricas; i++) {
        int idx = buscarProductoPorCodigo(productos, cantProductos, historicas[i].codigoProducto);
        if (idx != -1) {
            productos[idx].stockActual -= historicas[i].cantidad;
        }
    }

    // Reabrimos en modo lectura/escritura para actualizar registro por registro
    FILE* fInvUpd = fopen("inventario.dat", "r+b");
    for (int i = 0; i < cantProductos; i++) {
        fseek(fInvUpd, i * sizeof(Producto), SEEK_SET);
        fwrite(&productos[i], sizeof(Producto), 1, fInvUpd);
    }
    fclose(fInvUpd);

    cout << "Inventario actualizado (inventario.dat)" << endl;

    delete[] historicas;

    cout << "\nNormalizacion terminada con exito." << endl;
    return 0;
}
