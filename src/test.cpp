/* 
 * File:   main.cpp
 * Author: ituser
 *
 * Created on 12 de marzo de 2014, 11:00
 */

#include <cstdlib>
#include <iostream>
#include <stdio.h>

#include "cntronmod.h"

using namespace std;
/*
 * 
 */
int main(int argc, char** argv) {

    //char dev[20] = "/dev/ttyUSB0";
    
    CNTronMod nodoBase= CNTronMod(1);

    int a = nodoBase.init("/dev/ttyUSB0");

    if(a!=0){
	cout<<"Error de Inicialización\n";
        return a;
    }

    cout << ' ' << a << endl;

    int opcion=0;
    int resultado=0;
    int desID;
    
    /* ***** Test menu ***** */
	
    do{
        cout<<"\nMenu: escoja la operaión a realizar.\n";
        cout<<"[1] - Lectura del ID de la BASE.\n";
        cout<<"[2] - Medir al nodo N\n";
        cout<<"[3] - Tomar 10 medidas del nodo N\n";
        cout<<"[0] - Para finalizar.\n";
        cin >>opcion;

        switch (opcion){
        case 0:
          cout<<"Exit application\n";
          break;
        case 1:
          resultado=nodoBase.readBaseID();
          switch(resultado){
              case -1:
                cout<<"Error al enviar el comando.           [ SendCommand() devuelve -6 ].\n";
                break;
              case -5:
                cout<<"Error al encontrarse el buffer vacío. [ read() devuelve 0 ].\n";
                break;
              case 0:
                cout<<"Lectura del ID de la base realizada con éxito.\n";
                cout<<"El ID de la base es "<< nodoBase.getBaseId()<<"\n";
                break;
              default:
                cout<<"Error desconocido\n";
          }
          break;
        case 2:
            cout<<"Introduzca el ID del nodo: ";
            cin >> desID;
            cout << "Medida: " << nodoBase.measureTo(desID) << endl;
            break;
        case 3:
            cout<<"Introduzca el ID del nodo: ";
            cin >> desID;
            for(int i=0;i<10;i++)
                cout << "Medida: " << nodoBase.measureTo(desID) << endl;
            break;
        default:
          cout<<"Seleccione otra opción\n";
        }
    } while(opcion!=0);
	
    nodoBase.finish();

    return 0;
}

