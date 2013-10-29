rfm69rptr
=========
   Griff Hamlin Oct 19,2013.
   See https://github.com/griff2/rfm69rptr.
   rfm69rptr is a moteino R3 sketch.  It "repeats" all incoming and outgoing
   data, similar to a "dump terminal" program such as HyperTerminal. In this
   sketch, data bytes received from the computer via the Serial connection are 
   transmitted using the moteino's wireless transmitter.  Data bytes received 
   from the moteino's wireless receiver is sent to the computer via the Serial 
   connection.  

   You can put this sketch in two moteinos connected to two computers each 
   running a "terminal" program to create a "wireless" serial connection 
   between the computers. What you type on one computer will be displayed 
   on the other computer. I've tested it using "terminal" programs minicom 
   (on Linux) and HyperTerminal and PuttyTel (on Windows).  

   Alternatively, you should be able attach a virtual network interface using 
   the SLIP protocol to the serial port connected to the modeino on each 
   computer.  Then your two computers are networked using IP, and you can use 
   ALL the networking software you have (ping, ssh, telnet, ftp, email, 
   a web browser, etc.) over the wireless link. Be aware the wireless 
   radio can only handle 61 bytes of data per transmission (64 byte hardware
   queue less 3 bytes overhead). You'll need to fix-up whatever SLIP driver
   you're using to reduce the maximum packet length to 61. Also, the wireless 
   connection is "simplex", using only one RF frequency, so there will be 
   some IP packet loss due to RF "collisions" which will slow down the 
   effective data rate. Also the IP packet overhead slows the effective rate.
  
   This sketch uses these libraries:
        RFM69 (https://github.com/LowPowerLab/RFM69) to control the 
                moteino RF transceivers.
        SPIFlash (https://github.com/LowPowerLab/SPIFlash)
        SPI -- see arduino.cc/en/Reference/SPI.

   My experiments show that the Serial.available() function on moteinos only 
   works if the "terminal" programs are set for software (Xon/Xoff) flow 
   control. An Arduino Uno evidently doesn't have this requirement. 
   SLIP software often uses hardware flow control over the serial device. 
   So, after attaching a virtual SLIP interface to the serial device, you
   need to enable software flow control on the device. In Linux, I  use
   command "stty -F /dev/ttyUSB0 -crtscts ixon ixoff" to enable software
   flow control, after I attach the virtual SLIP interface to the ttyUSB0
   serial device.
