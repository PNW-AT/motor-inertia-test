#include <Arduino.h>
#include <JMotor.h> //https://github.com/joshua-8/JMotor
#include <Wire.h>

JEncoderAS5048bI2C encoder = JEncoderAS5048bI2C(false, 1, 0x40);
JMotorDriverEsp32HBridge motor = JMotorDriverEsp32HBridge(0, A0, A1);

void setup()
{
    Serial.begin(115200);
    encoder.useCustomWire(Wire1);
    Wire1.begin();
    motor.enable();
}

void loop()
{
    encoder.run();
    Serial.println(encoder.getPos());
    delay(500);
}
