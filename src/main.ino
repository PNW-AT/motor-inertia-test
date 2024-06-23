#include <Arduino.h>
#include <JMotor.h> //https://github.com/joshua-8/JMotor
#include <Wire.h>

JEncoderAS5048bI2C encoder = JEncoderAS5048bI2C(false, 1, 0x40);
JMotorDriverEsp32HBridge motor = JMotorDriverEsp32HBridge(0, MOSI, MISO);

void setup()
{
    Serial.begin(115200);
    encoder.useCustomWire(Wire1);
    Wire1.begin();
    motor.enable();
}

void loop()
{
    float frictionConstant = analogRead(A0) / 4095.0 - .25;
    float inertiaConstant = analogRead(A1) / 4095.0 - .25;

    static unsigned long lastMicros = micros();
    float dt = (micros() - lastMicros) / 1000000.0;
    lastMicros = micros();

    encoder.run();

    float vel = encoder.getVel();
    static float lastEncoderVel = vel;
    float accReading = (vel - lastEncoderVel) / dt;

    float accReadingSmoothingFactor = 0.005;
    static float acc = 0;
    acc = accReading * accReadingSmoothingFactor + (1 - accReadingSmoothingFactor) * acc;

    lastEncoderVel = vel;

    float motorOut = 0;
    motorOut += vel * -frictionConstant;
    motorOut += acc * -inertiaConstant;

    motor.set(motorOut); // currently assuming voltage is linearly proportional to speed

    Serial.printf("%6.3f %8.3f %5.2f %9.4f %9.4f\n", vel, acc, motorOut, frictionConstant, inertiaConstant);
    delay(1);
}
