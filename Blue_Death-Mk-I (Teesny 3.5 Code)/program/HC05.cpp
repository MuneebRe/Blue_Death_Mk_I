#include <Arduino.h>

#include "HC05.h"

HC05::HC05()
{
  Serial2.begin(1382400);
  //Serial2->setTimeout(100);
}

void HC05::update(int shared_mem[])
{
      /*
      Serial2.readBytes(buffer_in,3);

      shared_mem[0] = buffer_in[0];
      shared_mem[1] = buffer_in[1];

      Serial.print(shared_mem[0]);
      Serial.print("   ");
      Serial.println(shared_mem[1]);
      */

      Serial2.readBytes(buffer_in,10);
      p = buffer_in;
      
      pi = (int*)p;
      shared_mem[0] = *pi;
      p += sizeof(int);

      pb = (bool*)p;
      shared_mem[1] = *pb;
      p += sizeof(bool);

      pb = (bool*)p;
      shared_mem[2] = *pb;
      p += sizeof(bool);
      
      pi = (int*)p;
      shared_mem[3] = abs(*pi);
      
      for(int i = 0; i< 4; i++)
      {
        Serial.print(shared_mem[i]); 
        Serial.print("\t"); 
      }
      Serial.println();
}

HC05::~HC05()
{
}
