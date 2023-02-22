#include <ArduinoJson.h>



#ifndef COMDEVICE_H
#define COMDEVICE_H

#define MaxJsonSize 256

class CommDevice
{ 
    public:
        virtual StaticJsonDocument<MaxJsonSize> ReadJSon()=0;
        virtual void sendJSon(StaticJsonDocument<MaxJsonSize> message)=0;
        virtual bool isMessageAvailable()=0;
};


#endif