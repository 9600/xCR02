#include <RH_RF95.h>

#define LED_BUILTIN 16

#define CR02_FREQUENCY 868.0

uint8_t tempdata[30];

/*
  Slave Select mapped to D13/PD2
  Interrupt mapped to D14/PB2
*/
RH_RF95 CR02;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);

  if (!CR02.init()) {
    Serial.println("init failed");
  }
  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then
  // you can set transmitter powers from 5 to 23 dBm:
  // Failure to do that will result in extremely low transmit powers.

  //CR02.setModemConfig(CR02.Bw31_25Cr48Sf512);
  CR02.setFrequency(433.0);
  CR02.setTxPower(23, false);
}

void loop()
{
  if (CR02.available())
  {
    // Should be a message for us now
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    if (CR02.recv(buf, &len))
    {
      digitalWrite(LED_BUILTIN, HIGH);
      Serial.print("got request: ");
      Serial.println((char*)buf);
      Serial.print("RSSI: ");
      Serial.println(CR02.lastRssi(), DEC);

      // Send a reply
      sprintf(tempdata, "%s", "Hello Client");
      CR02.send(tempdata, sizeof(tempdata));
      CR02.waitPacketSent();
      Serial.println("Sent a reply");
      digitalWrite(LED_BUILTIN, LOW);
    }
    else
    {
      Serial.println("recv failed");
    }
  }
}


