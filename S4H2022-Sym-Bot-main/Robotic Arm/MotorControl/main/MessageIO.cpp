#include "MessageIO.h"

void MessageIO::addDevice(CommDevice* dev)
{
    devices.add(dev);
}

int MessageIO::getNbrDevice()
{
    return devices.size();
}

CommDevice*  MessageIO::getDevice(int index)
{
    return devices.get(index);
}


 StaticJsonDocument<MaxJsonSize> MessageIO::formatMessage(ControlMessage& msg)
 {
       StaticJsonDocument<MaxJsonSize> message;

        message["type"] = msg.getType();
        message["PLS"] = msg.getPayLoadSize();

        if(msg.getPayLoadSize() > 0 )
        {
            for(int x=0; x < msg.getPayLoadSize();x++)
                message["data"].add(msg.getPayload()[x]);                
        }
        return message;
 }

void MessageIO::sendMessage(int deviceIndex,ControlMessage& msg)
{
    CommDevice* dev = devices.get(deviceIndex);
    if(dev != NULL)
    {
        dev->sendJSon(formatMessage(msg));
    }
}
#include <String.h>

ControlMessage* MessageIO::parceMessage(StaticJsonDocument<MaxJsonSize> msg)
{
    if(msg.containsKey("error"))
        return NULL;
    int msgType = msg["type"].as<int>();
    int msgPayloadSize = msg["PLS"].as<int>();
    if(msgPayloadSize > 0)
    {           
        int* data = new int[msgPayloadSize];
        for(int x=0; x < msgPayloadSize; x++)
        {
            data[x] = msg["data"][x].as<int>();
        }
            return new ControlMessage(msgType,msgPayloadSize,data);
        }
        else
        {
            return new ControlMessage(msgType);
        }
}

ControlMessage* MessageIO::readMessage(int deviceIndex)
{
    CommDevice* dev = devices.get(deviceIndex);
    if(dev != NULL)
    {
        if(dev->isMessageAvailable())
        {
            return parceMessage(dev->ReadJSon());
        }
        
    }
    return NULL;
}
