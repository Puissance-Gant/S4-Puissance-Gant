#include <Arduino.h>
#include <ArduinoJson.h>
#include "CommDevice.h"

#ifndef SERIALCOM_H
#define SERIALCOM_H

class SerialComm : public CommDevice
{
    public:
        SerialComm(Stream* serialPort);
        StaticJsonDocument<MaxJsonSize> ReadJSon();
        void sendJSon(StaticJsonDocument<MaxJsonSize> message);
        bool isMessageAvailable();
    private:
        Stream* serialPort;
};

#endif