#ifndef USBCOM_H
#define USBCOM_H

static int sendCommand(unsigned char *toSendBuffer, unsigned char *receivedBuffer);

char commands(char *cmd, char *resp, int num);
char command(char cmd);

char commandsBySerial(char *iSerial, char *cmd, char *resp, int num);
char commandBySerial(char *iSerial, char cmd);

int listDevices();

unsigned int getPID(char *iSerial);



#endif // USBCOM_H
