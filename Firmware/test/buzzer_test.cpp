#include <Arduino.h>

//Definimos lo pines de salida del motor que se controla con un puente H
uint8_t pin1 = 2;
uint8_t pin2 = 16;
uint8_t ch0 = 0;
uint8_t ch1 = 1;


void setup()
{
    int freq = 30000; //La frecuencia de la señal PWM. Usamos 30kHz ya que está fuera de la frecuencia audible
    int res = 1023; //La resolución del PWM
    uint8_t res_bits = 10; //La misma resolución en bits

    //Elegimos los canales de PWM
    //Establece los pines como salida
    pinMode(pin1, OUTPUT);
    pinMode(pin2, OUTPUT);  

    //Establece la frecuencia y la resolución del PWM en los canales 0 y 1
    ledcSetup(ch0, freq, res_bits);
    ledcSetup(ch1, freq, res_bits);

    //Adjunt los pines pin1 y pin2 a los canales ch0 y ch1, respectivamente 
    ledcAttachPin(pin1, ch0);
    ledcAttachPin(pin2, ch1);

    //Pone el ciclo de trabajo en 0 para que los motores estén apagados
    ledcWrite(ch0, 0);
    ledcWrite(ch1, 0);

}

void loop()
{
    ledcWrite(ch0, 0);
    ledcWrite(ch1, 0);
    delay(1000);
    ledcWrite(ch0, 512);
    ledcWrite(ch1, 512);
    delay(1000);
    ledcWrite(ch0, 1023);
    ledcWrite(ch1, 1023);
    delay(1000);
}