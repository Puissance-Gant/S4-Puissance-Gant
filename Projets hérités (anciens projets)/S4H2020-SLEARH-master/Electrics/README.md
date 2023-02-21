# Electrical Parts list

    Name	              Description	                                Quantity
    ------------------------------------------------------------------------
    Open CR     	      OpenCR1.0     	                              1
    Servo driver	      PCA9685 16 channel PWM servo Driver	         1
    Servo motors	      SG90 servo motor	                              12

# Electrical Connections:

  # OpenCR pins to Drive Pins
    - 5V   ------->    VCC
    - Ground  ---->    Ground
    - scl(pin15) ->    scl
    - sda(pin14) ->    sda
    
  # Drive pins to Servo Motors    
    - 0      --------> motor 0 (thumb bottom)
    - 1      --------> motor 1 (thumb top)
    - 2      --------> motor 2 (index bottom)
    - 3      --------> motor 3 (index top)
    - 4      --------> motor 4 (middle bottom)
    - 5      --------> motor 5 (middle top)
    - 6      --------> motor 6 (ring bottom)
    - 7      --------> motor 7 (ring top)
    - 8      --------> motor 8 (pinky bottom)
    - 9      --------> motor 9 (pinky top)
    - 10     --------> motor 10 (index base) 
    - 13     --------> motor 11 (thumb base) 

The 2A voltage source must be connected to the 2 pin terminal block of the drive.
The voltage source with the barrel jack must be connected to the female adapter on the OpenCR.
The 2 Dynamixel servos can be plugged into any of the TTL ports on the OpenCR.
