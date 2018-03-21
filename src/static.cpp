/* 
 * File:   static.cpp
 * Author: ituser
 *
 * Created on 03 de septiembre de 2014, 11:00
 */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <time.h>
#include <vector>
#include <math.h>

#include "cntronmod.h"

#define MAP_SIZE 4
#define SENSING_RANGE 7

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

    //char dev[20] = "/dev/ttyUSB0";

    CNTronMod nodoBase = CNTronMod(1);

    int a = nodoBase.init("/dev/ttyUSB0");

    if (a != 0) {
        cout << "Error de Inicialización\n";
        return a;
    }

    int opcion = 0;
    int resultado = 0;

    /* ******************************************** */
    // Static beacon - Interbeacon measurements

    int myID;
    float measurement;

    resultado = nodoBase.readBaseID();
    switch (resultado) {
        case -1:
            cout << "Error al enviar el comando. [ SendCommand() devuelve -6 ].\n";
            break;
        case -5:
            cout << "Error al encontrarse el buffer vacío. [ read() devuelve 0 ].\n";
            break;
        case 0:
            cout << "Lectura del ID de la base realizada con éxito.\n";
            cout << "El ID de la base es " << nodoBase.getBaseId() << "\n";
            break;
        default:
            cout << "Error desconocido\n";
    }

    myID = nodoBase.getBaseId();

    /********************* Find neighbors ************************/
    vector<int> neighbors;

    /* @TODO Cálculo de vecinos
     * Leer de map.txt
     * id x y z
     * neighbors.push_back(id);
     */
    int neighbor_id[MAP_SIZE];
    float neighbor_x[MAP_SIZE], neighbor_y[MAP_SIZE], neighbor_z[MAP_SIZE];
    float myX, myY, myZ;
    string line;
    ifstream mapfile;
    mapfile.open("map.txt");
    if (mapfile.is_open()) {
        int i = 0;
        while (getline(mapfile, line)) {
            sscanf(line.c_str(),"%d %f %f %f",&neighbor_id[i],&neighbor_x[i],&neighbor_y[i],&neighbor_z[i]);
            if(neighbor_id[i]==myID){
                myX = neighbor_x[i];
                myY = neighbor_y[i];
                myZ = neighbor_z[i];
            }
            i++;
        }
        
        float distance;
        for(i=0;i<MAP_SIZE;i++){
            if(neighbor_id[i]!=myID){
                distance = sqrt(pow(neighbor_x[i]-myX,2)+pow(neighbor_y[i]-myY,2)+pow(neighbor_z[i]-myZ,2));
                if(distance < SENSING_RANGE){
                    neighbors.push_back(neighbor_id[i]);
                }
            }
        }
    }
    else {
        cerr << "No se ha podido leer el archivo map.txt";
        return -1;
    }
    mapfile.close();
    
    /*********************************************************/
    
    time_t rawtime;
    struct tm * timeinfo;
    char logfile_name[40];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(logfile_name, 40, "log_%Y_%m_%d_%H_%M.txt", timeinfo);
    ofstream logfile;
    logfile.open(logfile_name);

    do {

        for (int i = 1; i < ((int) neighbors.size()); i++) {
            measurement = nodoBase.measureTo(neighbors[i]);
            logfile << time(NULL) << '\t' << myID << '\t' << neighbors[i] << '\t' << measurement << '\n';
        }

    } while (opcion != 0);

    /* ******************************************** */

    logfile.close();

    nodoBase.finish();

    return 0;
}

