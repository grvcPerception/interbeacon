#include "cntronbase.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string.h>
#include <vector>

int chartohex(char * cnum) {
	int result = 0;
	int num[2];
	int i = 10;

	if ((cnum[i] >= 'A') && (cnum[i] <= 'F'))
		num[1] = (cnum[i] - 'A') + 10;
	else
		num[1] = (cnum[i] - '0');

	i = 11;

	if ((cnum[i] >= 'A') && (cnum[i] <= 'F'))
		num[0] = (cnum[i] - 'A') + 10;
	else
		num[0] = (cnum[i] - '0');

	result = num[1] * 16 + num[0];

	return result;
}

CNTronBase::CNTronBase(int baseId) {
	m_portHandler = 0;
	m_baseId = baseId;
}

CNTronBase::~CNTronBase(void) {
	finish();
}

int CNTronBase::getBaseId(void) const {
	return m_baseId;
}

// Initialize the serial port to the given values
int CNTronBase::init(const char *pDev) {
	struct termios my_termios;

	// Make sure port is closed 
	if (m_portHandler > 0)
		close(m_portHandler);

	// Open the port in read-write mode 
    m_portHandler = open(pDev, O_RDWR | O_NOCTTY);
        std::cout << m_portHandler << std::endl;
	if (m_portHandler < 0)
		return -5;
        
	/* Get the port attributes and flush all data on queues*/
	tcgetattr(m_portHandler, &my_termios);
	tcflush(m_portHandler, TCIOFLUSH);

	/* Setup the communication */
	my_termios.c_iflag &= ~(BRKINT | IGNPAR | PARMRK | INPCK | ISTRIP | IXON
			| INLCR | IGNCR | ICRNL);
	my_termios.c_iflag |= IGNBRK | IXOFF;
	my_termios.c_oflag &= ~(OPOST);
	my_termios.c_cflag |= CLOCAL | CREAD;
	my_termios.c_cflag &= ~PARENB;
	my_termios.c_cflag |= CS8;
	my_termios.c_cflag &= ~CSTOPB;
	my_termios.c_cflag &= ~CRTSCTS;
	my_termios.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL | ICANON | NOFLSH
			| TOSTOP | ISIG | IEXTEN);
	my_termios.c_cc[VMIN] = 1; //Each simple read call will be blocked until receive at least one byte

	//VTIME = Timeout. Is a character count ranging from 0 to 255 characters.
	//It is time measured in 0.1 second intervals, (0 to 25.5 seconds).
	//More information in http://www.unixwiz.net/techtips/termios-vmin-vtime.html

	my_termios.c_cc[VTIME] = 5;	//0 = No timeout for reading
	cfsetispeed(&my_termios, B115200);
	cfsetospeed(&my_termios, B115200);
	tcsetattr(m_portHandler, TCSANOW, &my_termios);

	/* Read base node ID */
	if (m_baseId == DEFAULT_ID)
		return readBaseID();

	/* Return 0 if no errors occurred */
	return 0;
}

// Close serial communication
void CNTronBase::finish(void) {
	/* Close the port */
	if (m_portHandler > 0) {
		close(m_portHandler);
		m_portHandler = 0;
	}
}

// Método que lee el ID de la base.
int CNTronBase::readBaseID() {
	std::string cmd = READNODEID;
	int result, size;
	char c;

	//Transformamos el objeto string 'cmd' en una cadena de caracteres char. 
	result = sendCommand(cmd.c_str(), cmd.size());
	if (result < 0)
	return -1;

	//std::cout << "[DEBUG] Reading base MAC ..." << std::endl;

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

	//std::cout << "[DEBUG] Base MAC: " << m_readBuff;

	//Nos quedamos con los primeros 2 bytes de la dirección MAC.
	//En este punto limitamos el rango de valor de ID a 0-255.

	char dirMAC[12];
	strncpy(dirMAC,m_readBuff,12);

	m_baseId=chartohex(dirMAC);

	return 0;
}

// Send a new data stream to the serial port
int CNTronBase::sendCommand(const char* command, int size) {
//	int i = 1;
	int dataWritten;

	//for (i = 0; i < size; i++) {
		dataWritten = write(m_portHandler, command, size);
		if (dataWritten < 0)
			return -6;
	//}
	return 0;
}

