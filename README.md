# Ultrasonic-Sensor-and-SerLCD-Display-with-Attiny-817

Parts: SparkFun SerLCD 16x2, Ultrasonic Ranging Module HC - SR04, and ATtiny817 Xplained Pro. \r

ATtiny sends a 17 microsecond trigger to the ranging module and uses the event system to capture the echo response. \n
Then, the the echo signal gets calculated to produce the distance. \n
The distance gets displayed on the SerLCD via SPI or USART.
