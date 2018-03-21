#include <ros/ros.h>
#include <cstdlib>
#include <iostream>
#include <stdio.h>

#include <time.h>
#include "cntronmod.h"
#include <range_msgs/P2PRange.h>

using namespace std;


int main(int argc, char** argv) {

    ros::init(argc, argv, "euroc_range");
    ros::NodeHandle n("~");
    ros::Publisher chatter_pub = n.advertise<range_msgs::P2PRange>("/ranges", 1);

    range_msgs::P2PRange node_dist;

    float dist;

    int selfId;

    CNTronMod nodoBase = CNTronMod(1);

    std::string beaconPort;

    int idMax;

    if(!n.getParam("interbeaconPort", beaconPort))
    {
        ROS_ERROR("No port given to 'interbeaconPort'");
        ROS_WARN("Trying to connect to /dev/ttyUSB0");
        beaconPort = "/dev/ttyUSB0";
    }

    if(!n.getParam("idMax", idMax))
    {
        ROS_WARN("idMax not specified! Using default value [25]");
        idMax = 25;
    }

    int a = nodoBase.init(beaconPort.c_str());

    if(a!=0){
        cout<<"Error de Inicialización\n";
        return a;
    }

    int resultado=0, seq = 0;
    

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
        selfId = nodoBase.getBaseId();
        break;
    default:
        cout<<"Error desconocido\n";
    }

    node_dist.source_id = selfId;
    node_dist.source_type = range_msgs::P2PRange::BASE;
    if(resultado == 0)
    {
        while(ros::ok())
        {

            for(int id=1; id<=idMax; id++)
            {
                if(id != selfId)
                {
                    dist = nodoBase.measureTo(id);
                    usleep(12000);
                    if(dist > 0)
                    {
                        node_dist.destination_id = id;
                        node_dist.destination_type = range_msgs::P2PRange::ANCHOR;
                        node_dist.range = dist;
                        node_dist.variance = 2.25;
                        node_dist.header.seq = seq++;
                        node_dist.header.frame_id = "/nanotron";
                        node_dist.header.stamp = ros::Time::now();
                        chatter_pub.publish(node_dist);

                        ros::spinOnce();
                    }
                }
            }
        }
    }


    cout << "Ranging program finished" << endl;
    nodoBase.finish();

    return 0;
}


