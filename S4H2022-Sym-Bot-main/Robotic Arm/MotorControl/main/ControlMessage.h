
#ifndef CONTROLMESSAGE_H
#define CONTROLMESSAGE_H

class ControlMessage
{
    public:
        ControlMessage(int type,int payloadSize=0, int* payload=0);
        ~ControlMessage();
        int  getType()          {return type;};
        int  getPayLoadSize()   {return payloadSize;};
        int* getPayload()       {return payload;};

    private:
        int type;
        int payloadSize;
        int* payload;
};

#endif