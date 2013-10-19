#include <RFM69.h>
#include <SPI.h>
#include <SPIFlash.h>

/* Griff Hamlin Oct 19,2013.
   rfm69rptr is a moteino R3 sketch.  It "repeats" all incoming and outgoing
   data, similar to a "dump terminal" program such as HyperTerminal. In this
   sketch, data bytes received from the computer via the Serial connection are 
   transmitted using the moteino's wireless transmitter.  Data bytes received 
   from the moteino's wireless receiver is sent to the computer via the Serial 
   connection.  You can put this sketch in two moteinos connected to two
   computers each running a "terminal" program to create a "wireless" serial
   connection between the computers. What you type on one computer
   will be displayed on the other computer. I've tested it using
   "terminal" programs minicom (on Linux) and PuttyTel (on Windows).  This 
   sketch uses these libraries:
        RFM69 (https://github.com/LowPowerLab/RFM69) to control the 
                moteino RF transceivers.
        SPIFlash (https://github.com/LowPowerLab/SPIFlash)
        SPI -- see arduino.cc/en/Reference/SPI.

   My experiments show that the Serial.available() function on moteinos only 
   works if the "terminal" programs are set for software (Xon/Xoff) flow 
   control. An Arduino Uno evidently doesn't have this requirement. This is 
   unfortunate since this "repeater" sketch was written to provide  a
   TCP/IP wireless link using SLIP. Evidently the SLIP software on my Linux 
   computers uses hardware flow control. I tested the SLIP configuration by 
   removing the two moteinos and wiring together the two FTDI boards connected 
   to two computers. I crossed the TxD,RxD and the CTS/DTR connections. 
   Without the CTS/DTR connections the SLIP configuration wouldn't work. With 
   these connections, I was able to use all networking software (ssh, telnet, 
   ping, etc..) between the two computers.
*/
#define NODEID      1
#define NETWORKID   100
#define FREQUENCY   RF69_433MHZ //Match this with the version of your Moteino! (others: RF69_915MHZ, RF69_868MHZ)
#define KEY         "thisIsEncryptKey" //has to be same 16 characters/bytes on all nodes, not more not less!
#define LED         9
#define SERIAL_BAUD 115200
#define ACK_TIME    30  // # of ms to wait for an ack

RFM69 radio;
SPIFlash flash(8, 0xEF30); //EF40 for 16mbit windbond chip
bool promiscuousMode = false; //set to 'true' to sniff all packets on the same network
byte theNodeID=0;

void setup() {
  Serial.begin(SERIAL_BAUD);
  while (!Serial);  // wait for serial to be ready
  delay(10);
  radio.initialize(FREQUENCY,NODEID,NETWORKID);
  radio.setHighPower(); //must uncomment for RFM69HW!
  radio.encrypt(KEY);
  radio.promiscuous(promiscuousMode);
  char buff[50];
  //sprintf(buff, "\nListening at %d Mhz...", FREQUENCY==RF69_433MHZ ? 433 : FREQUENCY==RF69_868MHZ ? 868 : 915);
  flash.initialize();
}

void loop() {
  char inbuf[80];  
  byte nser;
  int ch;
  int n;
  bool ok;
  // see if we got some serial input.
  n=Serial.available();         // n=number bytes avail in 64 byte FIFO
  if (n>0) {
    //get serial input
    nser=Serial.readBytes(inbuf,n);
    // xmit these nser bytes and get an ACK. No retries.
    ok=radio.sendWithRetry(theNodeID, (void *)inbuf, nser, 0); 
    //if (!ok) {
    //  Serial.print(nser); Serial.println(" bytes NOT sent ok");
    //}
    Blink(LED,3);
  }  // end some serial data available

  // see if we received any packet data.
  if (radio.receiveDone()) {
    for (byte i = 0; i < radio.DATALEN; i++)
      Serial.print((char)radio.DATA[i]);
    
    // When a node requests an ACK, respond to the ACK
    if (radio.ACK_REQUESTED)
    {
      byte theNodeID = radio.SENDERID;
      radio.sendACK();
    }
    Blink(LED,3);
  }  // end receiveDone 
}  // end loop()

void Blink(byte PIN, int DELAY_MS)
{
  pinMode(PIN, OUTPUT);
  digitalWrite(PIN,HIGH);
  delay(DELAY_MS);
  digitalWrite(PIN,LOW);
}
