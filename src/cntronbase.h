#ifndef __CNTRONBASE_H__
#define __CNTRONBASE_H__

#include <termios.h>
#include <string>
#include <time.h>
#include <vector>
#include <map>

#define DEFAULT_ID -1

#define RANGETO(ID) std::string("RangeTo ") + ID + "\r\n"
#define GETBEACONS(AGE) std::string("GetNodeIDList ") + #AGE + "\r\n"
#define READNODEID std::string("ReadNodeIDAdd\r\n")

//!Nanotron range data
struct CNTronRange {
  int emitterId;		// Unique emitter node ID
  int beaconId;		// Unique receiver node ID
  int time;             // Age of the range.
  float range;		// Estimated range in meters
};

class CNTronBase {
public:

	//!Default constructor
	CNTronBase(int baseId = DEFAULT_ID);

	//!Default destructor
	~CNTronBase(void);

	/**
	 Intialize the serial port to the given values.
	 The function open the serial port in read-write mode

	 \param pDev Port of the serial device (e.g. '/dev/ttyUSB0')
	 
	 \return
	 -  0: success
	 - -1: error while open the device
	 - -2: erroneous parity
	 - -3: erroneous dataBits
	 - -4: erroneous stopBits
	 */
	int init(const char *pDev);

	//!Finish the serial communication (closes serial port)
	void finish(void);

	/**
	 * Returns the base node unique identifier.
	 **/
	int getBaseId() const;
        /**
	 * Reads the base node ID (ID of node connected to serial port)
	 *
	 * return= -1; Error sending command. [ SendCommand() devuelve -6 ].
	 * return= -5; Empty serial buffer. [ read() devuelve 0 ].
	 * return= 0; Success.
	 **/
	int readBaseID();

protected:
	
	/**
	 Sends a stream of data to the configured serial port.
	 
	 \param command Data stream
	 \param size Length of the data stream.
	 
	 \return 0 if command was sent successfully -6 if any error. 
	**/
	int sendCommand(const char* command, int size);

	//!Serial port handler
	int m_portHandler;

	//!Reading buffer
	char m_readBuff[256];

	//! Base Id
	int m_baseId;
};

#endif

