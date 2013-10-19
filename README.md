rfm69rptr
=========
   Griff Hamlin Oct 19,2013.
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
   sketch uses the RFM69 library (https://github.com/LowPowerLab/RFM69) to 
   control the moteino RF transceivers.

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
