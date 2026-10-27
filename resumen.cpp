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

    int total = 0; // total productos buffet
    comanda registro;
    cout << "============= REPORTES DE VENTAS =============" << endl;
    //------------------------------------------------------CORTE DE CONTROL
        bool registroActual = fread(&registro, sizeof(comanda), 1, f) == 1; // control de registro por leer
        while (registroActual) {
        	int mozoActual = registro.idMozo;
			int productos = 0;
			float comisionTotal = 0.0f;
			while(registroActual && registro.idMozo == mozoActual){			// controlo que sea el mismo mozo
				productos += registro.cantidad;  //ventas
            	comisionTotal += registro.comision;  //paga comision
				total += registro.cantidad;
				registroActual = fread(&registro, sizeof(comanda),1,f) == 1;	//siguiente registro
			}
            
  			//termino de leer el mozo actual
            cout << "Mozo ID: " << mozoActual << endl;
            cout << "Productos vendidos: " << productos << endl;
            cout << "Comision a pagar: $" << comisionTotal << endl;
            cout << "--------------------------------------------" << endl;
    }

  	fclose(f);
    cout << "==============================================" << endl;
    cout << "TOTAL DE PRODUCTOS VENDIDOS EN EL BUFFET ESA SEMANA: " <<total<< endl;
    return 0;	
}
