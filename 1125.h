#include "esphome.h"

class UartReadLineSensor : public Component, public UARTDevice, public Sensor{
public:
  UartReadLineSensor(UARTComponent *parent) : UARTDevice(parent) {}
  Sensor *distance_sensor = new Sensor();

  void setup() override {
    // nothing to do here
  }

  int readline(int readch, char *buffer, int len)
  {
    static int pos = 0;
    int rpos;

    if (readch > 0) {
      switch (readch) {
        case '\n': // Ignore new-lines
          break;
        case '\r': // Return on CR
          rpos = pos;
          pos = 0;  // Reset position index ready for next time
          return rpos;
        default:
          if (pos < len-1) {
            buffer[pos++] = readch;
            buffer[pos] = 0;
          }
      }
    }
    // No end of line has been found, so return -1.
    return -1;
  }

  void loop() override {
    const int max_line_length = 80;
    static char buffer[max_line_length];
        static uint8_t mov_flag = 0;
        static int uartdata = 0;
        static char u16_dis[7] = {0};
        static char u16_dis_1[6] = {0};
        static char u16_dis_2[5] = {0};
        static int i,j;
        static float intdis = 0;
        static float f32rmax;
        static char chartemp[4] = {0};
        static char chartemp5[5] = {0};
        static char chartemp2[2] = {0};
        static char chartemp3[3] = {0};
        static uint8_t chartemp1 = 0;
       
    while (available()) {
                uartdata = readline(read(), buffer, max_line_length);
                if(uartdata > 0) {
                        //publish_state(buffer);
                        if(buffer[0] == 0x6d && buffer[1] == 0x6f && buffer[2] == 0x76)
                        {
                                //publish_state(to_string(uartdata));
                                if (uartdata < 16)
                                {
                                        j = 0;
                                        if (uartdata == 13)
                                        {
                                                for(i = 9; i < uartdata; i++)
                                                {
                                                        u16_dis_2[j] = buffer[i];
                                                        j++;
                                                }
                                                //intdis = atof(u16_dis_2);
                                                distance_sensor->publish_state(atof(u16_dis_2)*100);
                                        }
                                        if(uartdata == 14)
                                        {
                                                for(i = 9; i < uartdata; i++)
                                                {
                                                        u16_dis_1[j] = buffer[i];
                                                        j++;
                                                }
                                                distance_sensor->publish_state(atof(u16_dis_1)*100);
                                        }
                                        if(uartdata == 15)
                                        {
                                                for(i = 9; i < uartdata; i++)
                                                {
                                                        u16_dis[j] = buffer[i];
                                                        j++;
                                                }
                                                distance_sensor->publish_state(atof(u16_dis)*100);
                                        }
                                }
                                if(mov_flag != 0)
                                {
                                        mov_flag = 0;
                                        id(mmWave).publish_state("运动");
                                }
                        }
                       
                        else if(buffer[0] == 0x6f && buffer[1] == 0x63 && buffer[2] == 0x63)
                        {
                                if (uartdata < 16)
                                {
                                        j = 0;
                                        if (uartdata == 13)
                                        {
                                                for(i = 9; i < uartdata; i++)
                                                {
                                                        u16_dis_2[j] = buffer[i];
                                                        j++;
                                                }
                                                distance_sensor->publish_state(atof(u16_dis_2)*100);
                                        }
                                        if(uartdata == 14)
                                        {
                                                for(i = 9; i < uartdata; i++)
                                                {
                                                        u16_dis_1[j] = buffer[i];
                                                        j++;
                                                }
                                                distance_sensor->publish_state(atof(u16_dis_1)*100);       
                                        }
                                        if(uartdata == 15)
                                        {
                                                for(i = 9; i < uartdata; i++)
                                                {
                                                        u16_dis[j] = buffer[i];
                                                        j++;
                                                }
                                                distance_sensor->publish_state(atof(u16_dis)*100);
                                        }
                                }
                                if(mov_flag != 1)
                                {
                                        mov_flag = 1;
                                        id(mmWave).publish_state("静止");
                                }
                               
                        }
                        else if(buffer[0] == 0x6e && buffer[1] == 0x75 && buffer[2] == 0x6C)
                        {
                                if(mov_flag != 2)
                                {
                                        mov_flag = 2;
                                        id(mmWave).publish_state("无人");
                                        distance_sensor->publish_state(0);
                                }
                        }
                        else if(buffer[0] == 0x72 && buffer[1] == 0x6d && buffer[2] == 0x61 && buffer[3] == 0x78 && buffer[4] == 0x20 && buffer[5] == 0x69 && buffer[6] == 0x73 && buffer[7] == 0x20 )
                        {
                                j = 0;
                                if(uartdata == 12)
                                {
                                        for(i = 8; i < uartdata; i++)
                                        {
                                                chartemp[j] = buffer[i];
                                                j++;
                                        }
                                        id(rmax).publish_state(atof(chartemp)*100);
                                       
                                }
                                if(uartdata == 13)
                                {
                                        for(i = 8; i < uartdata; i++)
                                        {
                                                chartemp5[j] = buffer[i];
                                                j++;
                                        }
                                        id(rmax).publish_state(atof(chartemp5)*100);
                                }                               
                        }
                        else if(buffer[0] == 0x6d && buffer[1] == 0x74 && buffer[2] == 0x68 && buffer[3] == 0x31 && buffer[4] == 0x20 && buffer[5] == 0x69 && buffer[6] == 0x73 && buffer[7] == 0x20 )
                        {
                                j = 0;
                                if(uartdata == 10)
                                {
                                        for(i = 8; i < uartdata; i++)
                                        {
                                                chartemp2[j] = buffer[i];
                                                j++;
                                        }
                                        id(mth1).publish_state(atoi(chartemp2));                                       
                                }
                                if(uartdata == 11)
                                {
                                        for(i = 8; i < uartdata; i++)
                                        {
                                                chartemp3[j] = buffer[i];
                                                j++;
                                        }
                                        id(mth1).publish_state(atoi(chartemp3));                                       
                                }
                                if(uartdata == 12)
                                {
                                        for(i = 8; i < uartdata; i++)
                                        {
                                                chartemp[j] = buffer[i];
                                                j++;
                                        }
                                        id(mth1).publish_state(atoi(chartemp));                                       
                                }
                        }
                        else if(buffer[0] == 0x6d && buffer[1] == 0x74 && buffer[2] == 0x68 && buffer[3] == 0x32 && buffer[4] == 0x20 && buffer[5] == 0x69 && buffer[6] == 0x73 && buffer[7] == 0x20 )
                        {
                                j = 0;
                                if(uartdata == 10)
                                {
                                        for(i = 8; i < uartdata; i++)
                                        {
                                                chartemp2[j] = buffer[i];
                                                j++;
                                        }
                                        id(mth2).publish_state(atoi(chartemp2));                                       
                                }
                                if(uartdata == 11)
                                {
                                        for(i = 8; i < uartdata; i++)
                                        {
                                                chartemp3[j] = buffer[i];
                                                j++;
                                        }
                                        id(mth2).publish_state(atoi(chartemp3));                                       
                                }
                                if(uartdata == 12)
                                {
                                        for(i = 8; i < uartdata; i++)
                                        {
                                                chartemp[j] = buffer[i];
                                                j++;
                                        }
                                        id(mth2).publish_state(atoi(chartemp));                                       
                                }
                        }
                        else if(buffer[0] == 0x6d && buffer[1] == 0x74 && buffer[2] == 0x68 && buffer[3] == 0x33 && buffer[4] == 0x20 && buffer[5] == 0x69 && buffer[6] == 0x73 && buffer[7] == 0x20 )
                        {
                                j = 0;
                                if(uartdata == 10)
                                {
                                        for(i = 8; i < uartdata; i++)
                                        {
                                                chartemp2[j] = buffer[i];
                                                j++;
                                        }
                                        id(mth3).publish_state(atoi(chartemp2));                                       
                                }
                                if(uartdata == 11)
                                {
                                        for(i = 8; i < uartdata; i++)
                                        {
                                                chartemp3[j] = buffer[i];
                                                j++;
                                        }
                                        id(mth3).publish_state(atoi(chartemp3));                                       
                                }
                                if(uartdata == 12)
                                {
                                        for(i = 8; i < uartdata; i++)
                                        {
                                                chartemp[j] = buffer[i];
                                                j++;
                                        }
                                        id(mth3).publish_state(atoi(chartemp));                                       
                                }
                        }
                        else if(buffer[0] == 0x74 && buffer[1] == 0x6f && buffer[2] == 0x6e && buffer[3] == 0x73 && buffer[4] == 0x20 && buffer[5] == 0x69 && buffer[6] == 0x73 && buffer[7] == 0x20 )
                        {
                                j = 0;
                                if(uartdata == 9)
                                {
                                        chartemp1 = buffer[8]-48;
                                        id(tons).publish_state(chartemp1);                                       
                                }
                                if(uartdata == 10)
                                {
                                        for(i = 8; i < uartdata; i++)
                                        {
                                                chartemp2[j] = buffer[i];
                                                j++;
                                        }
                                        id(tons).publish_state(atoi(chartemp2));                                       
                                }
                                if(uartdata == 11)
                                {
                                        for(i = 8; i < uartdata; i++)
                                        {
                                                chartemp3[j] = buffer[i];
                                                j++;
                                        }
                                        id(tons).publish_state(atoi(chartemp3));                                       
                                }                               
                        }
                        else if(buffer[0] == 0x75 && buffer[1] == 0x74 && buffer[2] == 0x6f && buffer[3] == 0x6e && buffer[4] == 0x73 && buffer[5] == 0x20 && buffer[6] == 0x69 && buffer[7] == 0x73 && buffer[8] == 0x20 )
                        {
                                j = 0;
                                if(uartdata == 11)
                                {
                                        for(i = 9; i < uartdata; i++)
                                        {
                                                chartemp2[j] = buffer[i];
                                                j++;
                                        }
                                        id(utons).publish_state(atoi(chartemp2));                                       
                                }
                                if(uartdata == 12)
                                {
                                        for(i = 9; i < uartdata; i++)
                                        {
                                                chartemp3[j] = buffer[i];
                                                j++;
                                        }
                                        id(utons).publish_state(atoi(chartemp3));                                       
                                }       
                                if(uartdata == 13)
                                {
                                        for(i = 9; i < uartdata; i++)
                                        {
                                                chartemp[j] = buffer[i];
                                                j++;
                                        }
                                        id(utons).publish_state(atoi(chartemp));                                       
                                }                               
                        }
                        memset(chartemp,0,4);
                        memset(chartemp3,0,3);
                        memset(chartemp2,0,2);
                        memset(chartemp5,0,2);
                }
    }
  }
};
