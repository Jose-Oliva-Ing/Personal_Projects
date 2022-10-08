//#include "BluetoothSerial.h"
#include "RF24.h"

/*#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif*/

//BluetoothSerial SerialBT;
RF24 myRadio (7, 8); 

byte addresses[][6] = {"0"};
int escrito = 0;
int recibido = 0;

void setup() {
  Serial.begin(9600);
  delay(100);
  //SerialBT.begin("Radio");
  myRadio.begin();  
  myRadio.setChannel(115); 
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate(RF24_250KBPS) ; 
  myRadio.openWritingPipe(addresses[0]);
  myRadio.openReadingPipe(1, addresses[0]);
  myRadio.startListening();
}

void loop() {
  if (Serial.available() > 0) {
    myRadio.stopListening();
    escrito = Serial.read();
    myRadio.write(&escrito, sizeof(escrito));
    Serial.write(Serial.read());
    myRadio.startListening();
  }
  delay(50);
  if (myRadio.available()) {
    while (myRadio.available()) {
      myRadio.read(&recibido, sizeof(recibido));
      Serial.write(recibido);
    }
  }
}
