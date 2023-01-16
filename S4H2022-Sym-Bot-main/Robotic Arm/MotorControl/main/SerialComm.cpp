#include "SerialComm.h"

SerialComm::SerialComm(Stream *serialPort)
{
    this->serialPort = serialPort;
}

bool SerialComm::isMessageAvailable()
{
    return serialPort->available() > 0;
}

StaticJsonDocument<MaxJsonSize> SerialComm::ReadJSon()
{
   
    StaticJsonDocument<MaxJsonSize> message;
    DeserializationError err = deserializeJson(message, *serialPort);
    if(err) 
    {
        message["error"] = "deserialisation";        
    }
    
    return message;
}


void SerialComm::sendJSon(StaticJsonDocument<MaxJsonSize> message)
{
    serializeJson(message, *serialPort);
    serialPort->print('\n');
}

/*
Stream SerialComm::getSerialPort()
{
    return *serialPort;   
}
*/