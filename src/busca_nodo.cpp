#include <cstdlib>
#include <iostream>
#include <stdio.h>

#include <time.h>
#include "cntronmod.h"
#include "ros/ros.h"
#include "interbeacon/measure.h"

using namespace std;
/*
 * 
 */
#define NMAX_NODOS 24

int main(int argc, char** argv) {

    ros::init(argc, argv, "busca_nodo");
    ros::NodeHandle n;
    ros::Publisher chatter_pub = n.advertise<interbeacon::measure>("distances", 1);

    interbeacon::measure vector_dist;
    float dist;
    CNTronMod nodoBase= CNTronMod(1);

	ros::Time ahora, despues;

    int a = nodoBase.init("/dev/ttyUSB0");

    if(a!=0){
	cout<<"Error de Inicialización\n";
        return a;
    }

    int resultado=0;
    

    
    /* ***** Test menu ***** */
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
	

    if(resultado == 0)
	{
		while(ros::ok())
		{
			vector_dist.id_destiny.clear();
			vector_dist.measurement.clear();
			//usleep(20);
			//ahora = ros::Time::now();			
			for(int id=1; id<NMAX_NODOS; id++)
			{
				dist = nodoBase.measureTo(id);
	  			if(dist > 0)
				{
					vector_dist.id_destiny.push_back(id);
					vector_dist.measurement.push_back(dist);
				}
			}
			/**despues = ros::Time::now();
			if(despues.sec != ahora.sec)
			{
			cout << "El tiempo en la medicion es " << (despues.nsec)*pow(10,-6) + (pow(10,3) - ahora.nsec*pow(10,-6)) << " milisegundos" << endl;
			}
			else
			{
			cout << "El tiempo en la medicion es " << (despues.nsec-ahora.nsec)*pow(10,-6) << " milisegundos" << endl;
			}
			cout<<"La longitud del vector es "<<vector_dist.id_destiny.size()<<endl;**/
            //if(vector_dist.id_destiny.size() > 3)
                chatter_pub.publish(vector_dist);
		}		
	}



	if(nodoBase.getBaseId() != 255)
		cout << "El nodo que estamos usando no es el nodo BASE, es el nodo de ID " << nodoBase.getBaseId() << endl;


    cout << "Programa terminado" << endl;
    nodoBase.finish();

    return 0;
}


