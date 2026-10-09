/* que me diga, por cada mozo, cuántos productos 
vendió en la semana y cuánta plata de comisión le tengo que pagar. 
Y bien abajo, el total de productos que vendió el buffet en toda la semana. 
(pide un menu de reporte por pantalla)

	usar comandas_semana_sX-mm.dat reportes por semana (id mozo, int codigoproducto, int cantidad, float comision )
*/
#include <iostream>
#include <string>
#include <stdlib.h>
#include <cstdio>	//file
using namespace std;

struct comanda{
	int idMozo;
	int codigoproducto;
	int cantidad;
	float comision;
};
const int MAX_MOZOS = 20; //memoria fisica de mozos

int main (){
	cout <<"=========Resumen de la semana========="<<endl;
	cout <<"ingrese la semana que quiere cargar! (sx-mm)"<<endl;
	bool prueba1 = false; //primer chequeo contra error si la semana o mes que ingreso es invalida
	string semana, archivoverificacion;
	FILE* f = NULL;
	while (prueba1 == false){
		cout<<"semana: ";
		cin>>semana; //sx-mm
		archivoverificacion = "comandas_semana_"+semana+".dat";
		f = fopen(archivoverificacion.c_str(),"rb"); //solo lectura
		if (f==NULL){
			cout<<"el archivo no existe, asegurese de haber escrito correctamente el codigo!"<<endl<<"ejemplo s1-04 (primera semana de abril)"<<endl;
		}
		else {
			system("cls");
			cout<<"carga completa!"<<endl<<endl;
			prueba1 = true;
		}

	}
 	int mozoP[MAX_MOZOS] = {0};   // productos/platos
    float mozoC[MAX_MOZOS] = {0.0f};  // total a pagar

    int total = 0; // total productos buffet
    comanda registro;
    while (fread(&registro, sizeof(comanda), 1, f) == 1) {
        int mozoActual = registro.idMozo;

        if (mozoActual >= 0 && mozoActual < MAX_MOZOS) {
            mozoP[mozoActual] += registro.cantidad;  //ventas
            
            mozoC[mozoActual] += registro.comision;  //paga comision
        }

        total += registro.cantidad;
    }
	cin.ignore();
    fclose(f);
    cout << "============= REPORTES DE VENTAS =============" << endl;
    for (int i = 0; i < MAX_MOZOS; i++) {
        if (mozoP[i] > 0) {
            cout << "Mozo ID: " << i << endl;
            cout << "Productos vendidos: " << mozoP[i] << endl;
            cout << "Comision a pagar: $" << mozoC[i] << endl;
            cout << "--------------------------------------------" << endl;
            cin.get();
        }
    }


    cout << "==============================================" << endl;
    cout << "TOTAL DE PRODUCTOS VENDIDOS EN EL BUFFET ESA SEMANA: " <<total<< endl;
    return 0;	
}
