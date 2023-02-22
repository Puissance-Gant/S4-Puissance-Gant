/*******************************************************************************
* Copyright 2016 ROBOTIS CO., LTD.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

/* Authors: Taehun Lim (Darby) 
 * Modify
 */

#include <DynamixelWorkbench.h>

#if defined(__OPENCM904__)
  #define DEVICE_NAME "3" //Dynamixel on Serial3(USART3)  <-OpenCM 485EXP
#elif defined(__OPENCR__)
  #define DEVICE_NAME ""
#endif   

#define SERIAL_BAUD      57600
const String DXL_BAUD = "57600";

#define STRING_BUF_NUM 64
String cmd[STRING_BUF_NUM];
uint8_t len = 0;

DynamixelWorkbench dxl_wb;
uint8_t get_id[16];
uint8_t scan_cnt = 0;
uint8_t ping_cnt = 0;

bool isAvailableID(uint8_t id);
uint8_t split(String data, char separator, String* temp);
void printInst();

void setup() 
{
    Serial.begin(SERIAL_BAUD);
    while(!Serial); // Open a Serial Monitor  

    printInst();
}

void loop() 
{
  const char *log = NULL;
  bool cmdSucceedec = false;

  if (Serial.available())
  {
      String read_string = Serial.readStringUntil('\n');
      Serial.println("[CMD] : " + String(read_string));

      read_string.trim();

      len = split(read_string, ' ', cmd);

      if (cmd[0] == "help")
      {
        printInst();
      }
      else if (cmd[0] == "begin")
      {
        if (cmd[1] == '\0')
          cmd[1] = DXL_BAUD;

        uint32_t baud = cmd[1].toInt();
        cmdSucceedec = dxl_wb.init(DEVICE_NAME, baud);
        if (cmdSucceedec == false)
        {
          Serial.println(log);
          Serial.println("Failed to init");
        }
        else
        {
          Serial.print("Succeed to init : ");
          Serial.println(baud);  
        }
      }
      else if (cmd[0] == "end")
      {        
        return;
      }
      else if (cmd[0] == "scan")
      { 
          if (cmd[1] == '\0')
              cmd[1] = String("253");

          uint8_t range = cmd[1].toInt();
          cmdSucceedec = dxl_wb.scan(get_id, &scan_cnt, range);
          if (cmdSucceedec == false)
          {
              Serial.println(log);
              Serial.println("Failed to scan");
          }
          else
          {
              Serial.print("Find ");
              Serial.print(scan_cnt);
              Serial.println(" Dynamixels");

              for (int cnt = 0; cnt < scan_cnt; cnt++)
              {
                  Serial.print("id : ");
                  Serial.print(get_id[cnt]);
                  Serial.print(" model name : ");
                  Serial.println(dxl_wb.getModelName(get_id[cnt]));
              }
          }  
      }
      else if (cmd[0] == "ping")
      {
          if (cmd[1] == '\0')
            cmd[1] = String("1");

          uint8_t id       = cmd[1].toInt();
          // get_id[ping_cnt] = cmd[1].toInt();
          uint16_t model_number = 0;

          cmdSucceedec = dxl_wb.ping(id, &model_number, &log);
          // cmdSucceedec = dxl_wb.ping(get_id[ping_cnt], &model_number, &log);
          if (cmdSucceedec == false)
          {
              Serial.println(log);
              Serial.println("Failed to ping");
          }
          else
          {
              // ping_cnt++;

              Serial.println("Succeed to ping");
              Serial.print("id : ");
              Serial.print(id);
              // Serial.print(get_id[ping_cnt]);
              Serial.print(" model_number : ");
              Serial.println(model_number);
          }
      }
      else if (cmd[0] == "position")
      {
          int32_t data     = 0;

          if(cmd[1] != '\0')
          {
              uint8_t id = cmd[1].toInt();
              if(isAvailableID(cmd[1].toInt()))
              {
                  cmdSucceedec = dxl_wb.itemRead(id, "Present_Position", &data, &log);
                  if (cmdSucceedec == false)
                  {
                      Serial.println(log);
                      Serial.println("Failed to read");
                      return;
                  }
                  else
                  {
                      Serial.println(log);
                      Serial.print("read position : ");
                      Serial.println(data);
                  }
              }
          }
          else
          {
              for (size_t dxl_cnt = 0; dxl_cnt < (scan_cnt + ping_cnt); ++dxl_cnt)
              {
                  Serial.println("============ Present Position ============");
                  cmdSucceedec = dxl_wb.itemRead(get_id[dxl_cnt], "Present_Position", &data, &log);
                  if (cmdSucceedec == false)
                  {
                      Serial.print("Failed to read : ");
                      Serial.println(log);
                      return;
                  }
                  else
                  {
                      Serial.print(get_id[dxl_cnt]);
                      Serial.print(" : ");
                      Serial.println(data);
                  }
                  delay(10);
              }
          }
      }
      else if (isAvailableID(cmd[1].toInt()))
      {
          if (cmd[0] == "control_table")
          {
              uint8_t id = cmd[1].toInt();

              const ControlItem *control_item =  dxl_wb.getControlTable(id);
              uint8_t the_number_of_control_item = dxl_wb.getTheNumberOfControlItem(id);

              uint16_t last_register_addr = control_item[the_number_of_control_item-1].address;
              uint16_t last_register_addr_length = control_item[the_number_of_control_item-1].data_length;

              uint32_t getAllRegisteredData[last_register_addr+last_register_addr_length];

              if (control_item != NULL)
              {
                  cmdSucceedec = dxl_wb.readRegister(id, (uint16_t)0, last_register_addr+last_register_addr_length, getAllRegisteredData, &log);
                  if (cmdSucceedec == false)
                  {
                    Serial.println(log);
                    return;
                  }
                  else
                  {
                      for (int index = 0; index < the_number_of_control_item; index++)
                      {
                          uint32_t data = 0;

                          if (dxl_wb.getProtocolVersion() == 2.0f)
                          {
                            data = getAllRegisteredData[control_item[index].address];
                            Serial.print("\t");
                            Serial.print(control_item[index].item_name);
                            Serial.print(" : ");
                            Serial.println(data);
                          }
                          else if (dxl_wb.getProtocolVersion() == 1.0f)
                          {
                              switch (control_item[index].data_length)
                              {
                                  case BYTE:
                                    data = getAllRegisteredData[control_item[index].address];
                                    Serial.print("\t");
                                    Serial.print(control_item[index].item_name);
                                    Serial.print(" : ");
                                    Serial.println(data);
                                    break;

                                  case WORD:
                                    data = DXL_MAKEWORD(getAllRegisteredData[control_item[index].address], getAllRegisteredData[control_item[index].address+1]);
                                    Serial.print("\t");
                                    Serial.print(control_item[index].item_name);
                                    Serial.print(" : ");
                                    Serial.println(data);
                                    break;

                                  case DWORD:
                                    data = DXL_MAKEDWORD(DXL_MAKEWORD(getAllRegisteredData[control_item[index].address],   getAllRegisteredData[control_item[index].address+1]),
                                                          DXL_MAKEWORD(getAllRegisteredData[control_item[index].address+2], getAllRegisteredData[control_item[index].address+3]));
                                    Serial.print("\t");
                                    Serial.print(control_item[index].item_name);
                                    Serial.print(" : ");
                                    Serial.println(data);
                                    break;

                                  default:
                                    data = getAllRegisteredData[control_item[index].address];
                                    break;
                              } 
                          }
                      }
                  }
              }
          }
          else if (cmd[0] == "id")
          {
              uint8_t id     = cmd[1].toInt();
              uint8_t new_id = cmd[2].toInt();

              cmdSucceedec = dxl_wb.changeID(id, new_id, &log);
              if (cmdSucceedec == false)
              {
                  Serial.println(log);
                  return;
              }
              else
              {
                  Serial.println(log);
              }
          }
          else if (cmd[0] == "baud")
          {
              uint8_t  id       = cmd[1].toInt();
              uint32_t  new_baud  = cmd[2].toInt();

              cmdSucceedec = dxl_wb.changeBaudrate(id, new_baud, &log);
              if (cmdSucceedec == false)
              {
                  Serial.println(log);
                  return ;
              }
              else
              {
                  cmdSucceedec = dxl_wb.setBaudrate(new_baud, &log);
                  Serial.println(log);
              }
          }
          else if (cmd[0] == "torque")
          {
            uint8_t id       = cmd[1].toInt();
            uint8_t onoff    = cmd[2].toInt();

            cmdSucceedec = dxl_wb.torque(id, onoff, &log);
            if (cmdSucceedec == false)
            {
                Serial.println(log);
                return;
            }
            else
            {
                Serial.println(log);
            }
          }
          else if (cmd[0] == "led")
          {
            uint8_t id       = cmd[1].toInt();
            uint8_t onoff    = cmd[2].toInt();

            cmdSucceedec = dxl_wb.led(id, onoff, &log);
            if (cmdSucceedec == false)
            {
                Serial.println(log);
                return;
            }
            else
            {
                Serial.println(log);
            }
          }
          else if (cmd[0] == "counter_clockwise")
          {
            uint8_t id       = cmd[1].toInt();

            cmdSucceedec = dxl_wb.setNormalDirection(id, &log);
            if (cmdSucceedec == false)
            {
                Serial.println(log);
                return;
            }
            else
            {
                Serial.println(log);
            }
          }
          else if (cmd[0] == "clockwise")
          {
            uint8_t id       = cmd[1].toInt();

            cmdSucceedec = dxl_wb.setReverseDirection(id, &log);
            if (cmdSucceedec == false)
            {
                Serial.println(log);
                return;
            }
            else
            {
                Serial.println(log);
            }
          }
          else if (cmd[0] == "joint")
          {
              uint8_t id    = cmd[1].toInt();
              uint16_t goal = cmd[2].toInt();

              cmdSucceedec = dxl_wb.jointMode(id, 64, 0, &log);
              if (cmdSucceedec == false)
              {
                  Serial.println(log);
                  return;
              }
              else
              {
                  Serial.println(log);
              }

              if(len != 2) 
              {
                  cmdSucceedec = dxl_wb.goalPosition(id, (int32_t)goal, &log);
                  if (cmdSucceedec == false)
                  {
                      Serial.println(log);
                      return;
                  }
                  else
                  {
                      Serial.println(log);
                  }
              }
          }
          else if (cmd[0] == "extend")
          {
              uint8_t id    = cmd[1].toInt();
              int32_t goal  = cmd[2].toInt();

              cmdSucceedec = dxl_wb.setExtendedPositionControlMode(id, &log);
              if (cmdSucceedec == false)
              {
                  Serial.println(log);
                  return;
              }
              else
              {
                  Serial.println(log);
              }
              if(len != 2) 
              {
                  cmdSucceedec = dxl_wb.goalPosition(id, (int32_t)goal, &log);
                  if (cmdSucceedec == false)
                  {
                      Serial.println(log);
                      return;
                  }
                  else
                  {
                      Serial.println(log);
                  }
              }
          }
          else if (cmd[0] == "wheel")
          {
              uint8_t id    = cmd[1].toInt();
              int32_t goal  = cmd[2].toInt();

              cmdSucceedec = dxl_wb.wheelMode(id, 0, &log);
              if (cmdSucceedec == false)
              {
                  Serial.println(log);
                  return;
              }
              else
              {
                  Serial.println(log);
              }

              if(len != 2) 
              {
                  cmdSucceedec = dxl_wb.goalVelocity(id, (int32_t)goal, &log);
                  if (cmdSucceedec == false)
                  {
                      Serial.println(log);
                      return;
                  }
                  else
                  {
                      Serial.println(log);
                  }
              }
          }
          else if (cmd[0] == "write")
          {
              uint8_t id = cmd[1].toInt();      
              uint32_t value = cmd[3].toInt(); 

              cmdSucceedec = dxl_wb.writeRegister(id, cmd[2].c_str(), value, &log);
              if (cmdSucceedec == false)
              {
                  Serial.println(log);
                  Serial.println("Failed to write");
                  return;
              }
              else
              {
                  Serial.println(log);
              }
          }
          else if (cmd[0] == "read")
          {
              uint8_t id = cmd[1].toInt();

              int32_t data = 0;
              
              cmdSucceedec = dxl_wb.readRegister(id, cmd[2].c_str(), &data, &log);
              if (cmdSucceedec == false)
              {
                  Serial.println(log);
                  Serial.println("Failed to read");
                  return;
              }
              else
              {
                  Serial.println(log);
                  Serial.print("read data : ");
                  Serial.println(data);
              }
          }
          else if (cmd[0] == "reboot")
          {
              uint8_t id = cmd[1].toInt();

              cmdSucceedec = dxl_wb.reboot(id, &log);
              if (cmdSucceedec == false)
              {
                  Serial.println(log);
                  Serial.println("Failed to reboot");
                  return;
              }
              else
              {
                  Serial.println(log);
              }
          }
          else if (cmd[0] == "reset")
          {
              uint8_t id = cmd[1].toInt();

              cmdSucceedec = dxl_wb.reset(id, &log);
              if (cmdSucceedec == false)
              {
                  Serial.println(log);
                  Serial.println("Failed to reset");
                  return;
              }
              else
              {
                  Serial.println(log);
              }
          }
          else 
          {
              Serial.println("Wrong command");
          }
      }
      else 
      {
            Serial.println("Please check ID");
      }
  }
}

uint8_t split(String data, char separator, String* temp)
{
    int cnt = 0;
    int get_index = 0;

    String copy = data;
    
    while(true)
    {
        get_index = copy.indexOf(separator);

        if(-1 != get_index)
        {
            temp[cnt] = copy.substring(0, get_index);
            copy = copy.substring(get_index + 1);
        }
        else
        {
            temp[cnt++] = copy.substring(0, copy.length());
            break;
        }
        ++cnt;
    }

    return cnt;
}

bool isAvailableID(uint8_t id)
{
    for (int dxl_cnt = 0; dxl_cnt < (scan_cnt + ping_cnt); ++dxl_cnt)
    {
        if (get_id[dxl_cnt] == id)
            return true;
    }

    return false;
}

void printInst(void)
{
    Serial.print("-------------------------------------\n");
    Serial.print("Set begin before scan or ping\n");
    Serial.print("-------------------------------------\n");
    Serial.print("help\n");
    Serial.print("begin  [BAUD]\n");
    Serial.print("scan   [RANGE]\n");
    // Serial.print("ping   [ID]\n");
    Serial.print("control_table (ID)\n");
    Serial.print("*id    (ID) (NEW_ID)\n");
    Serial.print("*baud  (ID) (NEW_BAUD)\n");
    Serial.print("torque (ID) (0|1)\n");
    Serial.print("led    (ID) (0|1)\n");
    Serial.print("counter_clockwise (ID)\n");
    Serial.print("clockwise (ID)\n");
    Serial.print("position [ID]\n");
    Serial.print("joint  (ID) [GOAL_POSITION]\n");
    Serial.print("extend  (ID) [GOAL_POSITION]\n");
    Serial.print("wheel  (ID) [GOAL_VELOCITY]\n");
    Serial.print("*write (ID) (ADDRESS_NAME) (DATA)\n");
    Serial.print("read   (ID) (ADDRESS_NAME)\n");
    Serial.print("reboot (ID) \n");
    Serial.print("*reset (ID) \n");
    Serial.print("end\n");
    Serial.print("-------------------------------------\n");
    Serial.print("Press Enter Key\n");
}
