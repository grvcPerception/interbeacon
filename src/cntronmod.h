/* 
 * File:   cntronmod.h
 * Author: ituser
 *
 * Created on 12 de marzo de 2014, 11:04
 */

#ifndef CNTRONMOD_H
#define	CNTRONMOD_H

#include "cntronbase.h"

#ifdef RANGETO
    #undef RANGETO
#endif
#define RANGETO(ID) std::string("RangeTo 0000000000") + ID + "\r\n"


class CNTronMod : public CNTronBase {
public:
    CNTronMod(int baseId) : CNTronBase(baseId){};
    virtual ~CNTronMod();
    
    /**
     Take a range measurement to another beacon.
      
     \param beacon id from which take the measurement
     
     \return
     - >0: range measurement
     - -1: error taking the measurement
     - -5: error reading serial port
    **/
    float measureTo(int desId);
private:

};

#endif	/* CNTRONMOD_H */

