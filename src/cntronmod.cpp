/* 
 * File:   cntronmod.cpp
 * Author: ituser
 * 
 * Created on 12 de marzo de 2014, 11:04
 */

#include "cntronmod.h"
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <unistd.h>

CNTronMod::~CNTronMod() {
}

float CNTronMod::measureTo(int desId) {
   
    float range;
    char id[4];
    sprintf(id,"%2x",desId);

   /** short id2 = desId;
    sprintf(id,"%x",id2);**/

    if(id[0]<'0' || id[0]>'9')
        id[0] = '0';
    
    // Debugging
    //std::cout << "Midiendo a " << id << std::endl;

    std::string cmd = RANGETO(id);

    // Debugging
    //std::cout << "Comando enviado: " << cmd << std::endl;

    int result, size, error;
    char c;

    //std::cout << "\n>> comando: " << cmd << std::endl;
    
    result = sendCommand(cmd.c_str(), cmd.size());
    if (result < 0)
	return -1;

    // Read result
    c = 0;
    size = 0;
    while (c != '\n') {
            result = read(m_portHandler, &c, 1);
            if (result < 0) {
                    usleep(10000);
                    return -5;
            }

            if (result == 1) {
                    m_readBuff[size++] = c;
            }
    }
    m_readBuff[size] = '\0';

    sscanf(m_readBuff, "%d, %f, %*d", &error, &range);
    
     //std::cout << m_readBuff << std::endl;
    
    if(error!=0) // Error
        range = -2;

    return range;
}

