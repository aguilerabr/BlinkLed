#include "BlinkLed.h"

BlinkLed::BlinkLed(ESP32_WS2812_RMT* ws2812Strip, uint16_t numLeds, uint32_t color,
    uint16_t duration , uint16_t pause) : _ws2812Strip(ws2812Strip), _numLeds(numLeds),
    _defaultBlinkColor(color), _defaultBlinkDuration(duration), _defaultPauseDuration(pause),
    _isBlinking(false), _currentBlink(0), _lastBlinkTime(0), _ledState(false),
    _inPause(false), _isRGB(true) {}

BlinkLed::BlinkLed(uint8_t pin, uint16_t duration, uint16_t pause)
    : _pin(pin), _isRGB(true), _numLeds(1), _defaultBlinkColor(0xffffff),
      _defaultBlinkDuration(duration), _defaultPauseDuration(pause),
      _isBlinking(false), _currentBlink(0), _lastBlinkTime(0), _ledState(false), _inPause(false) {}

void BlinkLed::begin() {
    pinMode(_pin, OUTPUT);
    setColor(0);
}

void BlinkLed::setErrorPattern(uint8_t blinks) {
    setErrorPattern(blinks, _defaultBlinkColor, _defaultBlinkDuration, _defaultPauseDuration);
}

void BlinkLed::setErrorPattern(uint8_t blinks, uint32_t color, uint16_t duration, uint16_t pause) {
    _blinkCount = blinks;
    _blinkDuration = duration;
    _pauseDuration = pause;
    _isBlinking = true;
    _currentBlink = 0;
    _lastBlinkTime = millis();
    _ledState = false;
    _inPause = false;

    if (_isRGB) {
        _blinkColor = color;
    } else {
        _blinkColor = 1;
    }
}

void BlinkLed::clearPattern() {
    _isBlinking = false;
    _inPause = false;
}

void BlinkLed::setState(uint32_t color) {
    _isBlinking = false;
    setColor(color);
}

void BlinkLed::update() {
    if (_isBlinking) {
        blinkLogic();
    }
}

void BlinkLed::setColor(uint32_t color) {
    if (_ws2812Strip) {
        rgb_t cor = BlinkLed::rgbColor(color);
        _ws2812Strip->fillColor(cor.r, cor.g, cor.b); // Vermelho
        _ws2812Strip->show();
    } else {
        digitalWrite(_pin, color ? HIGH : LOW);
    }
}

uint32_t BlinkLed::rgb(uint8_t r, uint8_t g, uint8_t b) {
    return (uint32_t)r << 16 | (uint32_t)g << 8 | (uint32_t)b;
}

rgb_t BlinkLed::rgbColor(uint32_t color) {
    rgb_t result;
    result.r = (color & 0xff0000) >> 16;
    result.g = (color & 0xff00) >> 8;
    result.b = (color & 0xff);

    return result;
}

void BlinkLed::blinkLogic() {
    unsigned long now = millis();
    
    if (_inPause) {
        if (now - _lastBlinkTime >= _pauseDuration) {
            _inPause = false;
            _currentBlink = 0;
            _ledState = false;
            _lastBlinkTime = now;
        }
        return;
    }

    if (now - _lastBlinkTime >= _blinkDuration) {
        _lastBlinkTime = now;

        if (_ledState) {
            setColor(0);  // Desliga o LED
            _ledState = false;
            _currentBlink++;

            if (_currentBlink >= _blinkCount) {
                _inPause = true;
                _lastBlinkTime = now;
            }
        } else {
            setColor(_blinkColor);  // Liga o LED
            _ledState = true;
        }
    }
}
