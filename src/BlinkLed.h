#ifndef BLINK_LED_H
#define BLINK_LED_H

#define USE_NEOPIXEL

#include <Arduino.h>
#include <ESP32_WS2812_RMT.h>

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} rgb_t;


class BlinkLed {
public:
    BlinkLed(uint8_t pin, bool isRGB = false, uint16_t numLeds = 1, uint32_t color = 0xffffff,
        uint16_t duration = 500, uint16_t pause = 1000);
    void begin();
    
    void setErrorPattern(uint8_t blinks, uint32_t color, uint16_t duration = 500, uint16_t pause = 1000);
    void setErrorPattern(uint8_t blinks);
    void setState(uint32_t color);
    void clearPattern();  // Para a piscada
    void update();  // Deve ser chamado no loop() para evitar bloqueios
    static uint32_t rgb(uint8_t r, uint8_t g, uint8_t b);
    static rgb_t rgbColor(uint32_t rgbColor);

private:
    void setColor(uint32_t color);
    void blinkLogic();

    uint8_t _pin;
    bool _isRGB;
    uint16_t _numLeds;

    uint32_t _defaultBlinkColor;
    uint16_t _defaultBlinkDuration;
    uint16_t _defaultPauseDuration;  // Pausa entre ciclos de piscada
    
    uint8_t _blinkCount;
    uint32_t _blinkColor;
    uint16_t _blinkDuration;
    uint16_t _pauseDuration;  // Pausa entre ciclos de piscada
    
    bool _isBlinking;
    uint8_t _currentBlink;
    unsigned long _lastBlinkTime;
    bool _ledState;
    bool _inPause;  // Flag para indicar se está no tempo de pausa
    
    ESP32_WS2812_RMT* _strip;
};

#endif
