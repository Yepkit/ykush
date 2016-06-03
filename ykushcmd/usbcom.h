#ifndef USBCOM_H
#define USBCOM_H

static int sendCommand(unsigned char *toSendBuffer, unsigned char *receivedBuffer);

char command(char cmd);

char commandBySerial(char *iSerial, char cmd);

int listDevices();


#endif // USBCOM_H
