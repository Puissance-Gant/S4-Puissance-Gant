

def BytesToInt(BytesArray):
    nbr = 0
    nbr |= BytesArray[0] << 24
    nbr |= BytesArray[1] << 16
    nbr |= BytesArray[2] << 8
    nbr |= BytesArray[3]
    return nbr

def IntToBytes(nbr):
    buffer = bytearray(4)
    buffer[0] = (nbr & 0xFF000000) >> 24
    buffer[1] = (nbr & 0x00FF0000) >> 16
    buffer[2] = (nbr & 0x0000FF00) >> 8
    buffer[3] = (nbr & 0x000000FF)
    return buffer
