#include <Arduino.h>
#include "BlinkLed.h"

#define LED_PIN 23    // Pino do LED comum ou do WS2812

// Para WS2812, descomente estas linhas:
// #define USE_NEOPIXEL
// #define NUM_LEDS 1
// BlinkLed led(LED_PIN, true, NUM_LEDS);  // LED RGB (WS2812)

BlinkLed blinkLed(LED_PIN, true, 3);  // LED RGB
BlinkLed blink2(LED_BUILTIN);

void setup() {
    Serial.begin(115200);
    blinkLed.begin();
    blink2.begin();
    blink2.setErrorPattern(3, 1);
    blinkLed.setErrorPattern(5, BlinkLed::rgb(127, 48, 129), 300);
}

void loop() {
    static unsigned long lastActionTime = 0;
    static int step = 0;

    blinkLed.update();  // Atualiza a lógica do LED sem bloquear
    blink2.update();

    // Muda o padrão a cada 3 segundos
    if (0) {//millis() - lastActionTime > 3000) {
        lastActionTime = millis();

        switch (step) {
            case 0:
                blinkLed.setErrorPattern(1, 0xFF0000, 300);  // 1 piscada vermelha
                break;
            case 1:
                blinkLed.setErrorPattern(2, 0x00FF00, 300);  // 2 piscadas verdes
                break;
            case 2:
                blinkLed.setState(0x0000FF);  // Azul sólido (estado normal)
                break;
            case 3:
                blinkLed.setErrorPattern(3, 0xFFFF00, 300);  // 3 piscadas amarelas
                break;
            case 4:
                blinkLed.setState(0x00FFFF);  // Ciano (outro estado fixo)
                break;
        }

        step = (step + 1) % 5;
    }
}
