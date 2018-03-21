/* 
 * File:   mobile.cpp
 * Author: ituser
 *
 * Created on 03 de septiembre de 2014, 11:00
 */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <time.h>
#include <ctime>

#include "cntronmod.h"

#include "ros/ros.h"
#include "ros/time.h"

#define MAP_SIZE 20

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

    int opcion=0;
    int resultado=0;
    
    /* ******************************************** */
    // Nodo robot

    int myID;
    float measurement; 

    resultado=nodoBase.readBaseID();
    switch(resultado){
      case -1:
        cout<<"Error al enviar el comando. [ SendCommand() devuelve -6 ].\n";
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
    
    vector<int> map;
    string line;
    ifstream mapfile;
    mapfile.open("map.txt");
    if (mapfile.is_open()) {
        int neighbor_id = 0;
        while (getline(mapfile, line)) {
            sscanf(line.c_str(),"%d",&neighbor_id);
            map.push_back(neighbor_id);
        }
    } else{
        cerr << "No puede abrir archivo de mapa\n";
        return -2;
    }
    mapfile.close();

    myID = nodoBase.getBaseId();
    time_t rawtime;
    struct tm * timeinfo;
    char logfile_name[40];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(logfile_name,40,"log_%Y_%m_%d_%H_%M.txt",timeinfo);
    ofstream logfile;
    logfile.open(logfile_name);

    /********** TIME **********/    
    ros::Time::init();
    ros::Time begin = ros::Time::now();
    
    cout << "RELOJ ROS: " << begin.toSec() << endl;
    usleep(1000000);
    cout << "RELOJ ROS: " << ros::Time::now().toSec() << endl;
    cout << "DIFERENCIA: " << ros::Time::now().toSec()-begin.toSec() << endl;
    /**************************/
    
    opcion = 1;
    if(logfile.is_open()){
        cout << "Log abierto. Midiendo...\n";
        do{

            for(int i=0;i<((int)map.size());i++){
                if(map[i] != myID){
                    measurement = nodoBase.measureTo(map[i]);
                    logfile << ros::Time::now().toSec()-begin.toSec() <<'\t'<< myID <<'\t'<< map[i] <<'\t'<< measurement <<'\n';
                }
            }

        } while(opcion!=0);
    } else{
        cerr << "No se pudo abrir el archivo de log\n";
        return -3;
    }

    /* ******************************************** */

    logfile.close();

    nodoBase.finish();

    return 0;
}

