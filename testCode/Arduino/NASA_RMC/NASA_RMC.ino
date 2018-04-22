#include <Servo.h>
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

Servo leftDrive;
Servo rightDrive;
Servo unloader;
Servo digger;

int leftDrivePin = 2;
int rightDrivePin = 3;
int unloaderPin = 4;
int diggerPin = 5;

//Encoder leftEnc(2, 3);


byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 2, 177);

unsigned int localPort = 8888;      // local port to listen on



// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,EthernetUDP Udp;



void setup() {
  leftDrive.attach(leftDrivePin);
  rightDrive.attach(rightDrivePin);
  unloader.attach(unloaderPin);
  digger.attach(diggerPin);

  Ethernet.begin(mac, ip);
  Udp.begin(localPort);

  Serial.begin(9600);
}


void loop() {
  HandleEthernet();
  /*for (int i = 0; i < 100; i++) {
    writeMotors(i, i, i, i); // left, right, unloader, digger
    delay(60);
  }
  writeMotors(100,100,100,100);
  delay(3000);
  for (int i = 100; i > -100; i--) {
    writeMotors(i, i, i, i); // left, right, unloader, digger
    delay(60);
  }
  writeMotors(-100,-100,-100,-100);
  delay(3000);
  for (int i = -100; i < 0; i++) {
    writeMotors(i, i, i, i); // left, right, unloader, digger
    delay(60);
  }*/
}

void writeMotors(int left, int right, int unload, int dig) {
  leftDrive.writeMicroseconds(map(left, -100, 100, 1000, 2000));
  rightDrive.writeMicroseconds(map(right, -100, 100, 1000, 2000));
  unloader.writeMicroseconds(map(unload, -100, 100, 1000, 2000));
  digger.writeMicroseconds(map(dig, -100, 100, 1000, 2000));

}

void HandleEthernet() {
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remote = Udp.remoteIP();
    for (int i = 0; i < 4; i++)
    {
      Serial.print(remote[i], DEC);
      if (i < 3)
      {
        Serial.print(".");
      }
    }
    Serial.print(", port ");
    Serial.println(Udp.remotePort());

    // read the packet into packetBufffer
    Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    Serial.println("Contents:");
    Serial.println(packetBuffer);
    int motorValues[4];
    parseMotorPacket(packetBuffer,motorValues);
    for(int i=0;i<4;i++) {
      Serial.println(motorValues[i]);
    }
  }
}

bool validMotorPacket(String msg) {
  return msg[0] == '$' && msg[13] == '&';
}

void parseMotorPacket(String msg, int* vals) {
  if (validMotorPacket(msg)) {
    Serial.println("Good");
    for (int i=0;i<4;i++) {
      vals[i]=msg.substring(3*i+1,3*i+4).toInt();
    }
  }
  else {
    for(int i=0;i<4;i++) {
      vals[i]=100;
    }
  }
}

