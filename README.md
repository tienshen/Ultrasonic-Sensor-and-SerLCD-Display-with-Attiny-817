# Ultrasonic-Sensor-and-SerLCD-Display-with-Attiny-817

#### Parts: 
* SparkFun SerLCD 16x2 
* Ultrasonic Ranging Module HC - SR04 
* and ATtiny817 Xplained Pro

#### ATtiny sends a trigger to the ranging module.
#### ATtiny uses the event system to capture the echo response.
#### The echo signal gets calculated to produce the distance.
#### The distance gets displayed on the SerLCD via SPI or USART.
