#include "Vector.h"
#include "CommDevice.h"
#include "ControlMessage.h"

#ifndef MESSAGEIO_H
#define MESSAGEIO_H

class MessageIO
{
    public:
        void addDevice(CommDevice* dev);
        int getNbrDevice();
        void sendMessage(int deviceIndex, ControlMessage& msg);
        ControlMessage* readMessage(int deviceIndex);
        ControlMessage* parceMessage(StaticJsonDocument<MaxJsonSize> msg);
        StaticJsonDocument<MaxJsonSize> formatMessage(ControlMessage& msg);
        CommDevice* getDevice(int index);
    private:
        Vector<CommDevice*> devices;
};

#endif
