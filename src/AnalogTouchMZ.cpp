#include <AnalogTouchMZ.h>

void AnalogTouchMZ::initializeDevice() {
    pinMode(_xl, INPUT);
    pinMode(_xr, INPUT);
    pinMode(_yu, INPUT);
    pinMode(_yd, INPUT);
    _pos.x = 0;
    _pos.y = 0;
    _pressed = false;
    _scale_x = 4000 / (_width);
    _scale_y = 4000 / (_height);
    _rotation = 0;
}

int AnalogTouchMZ::x() {
    int x = _pos.x + _offset_x;
    int y = _pos.y + _offset_y;

    switch (_rotation) {
        case 0:
            return x;
        case 3:
            return _height - 1 - y;
        case 2:
            return _width - 1 - x;
        case 1:
            return y;
    }
    return 0;
}

int AnalogTouchMZ::y() {
    int x = _pos.x + _offset_x;
    int y = _pos.y + _offset_y;

    switch (_rotation) {
        case 0:
            return y;
        case 3:
            return x;
        case 2:
            return _height - 1 - y;
        case 1:
            return _width - 1 - x;
    }
    return 0;
}

bool AnalogTouchMZ::isPressed() {
    return _pressed;
}

int AnalogTouchMZ::getSample(uint8_t pin) {
    int samples[ANALOGTOUCHMZ_SMPSIZE];
    for (int i = 0; i < ANALOGTOUCHMZ_SMPSIZE; i++) {
        samples[i] = analogRead(pin);
    }
    int most = samples[0];
    int mostcount = 1;
    for(int pos = 0; pos < ANALOGTOUCHMZ_SMPSIZE; pos++) {
        int current = samples[pos];
        int currentcount = 0;
        for(int inner = pos + 1; inner < ANALOGTOUCHMZ_SMPSIZE; inner++) {
            if(samples[inner] == current) {
                currentcount++;
            }
        }
        if(currentcount > mostcount) {
            most = current;
            mostcount = currentcount;
        }
        // If we have less array slices left than the current
        // maximum count, then there is no room left to find
        // a bigger count.  We have finished early and we can
        // go home.
        if(ANALOGTOUCHMZ_SMPSIZE - pos < mostcount) {
            break;
        }
    }
    if (mostcount < (ANALOGTOUCHMZ_SMPSIZE/10)) {
        return 4000;
    }
    return most;
}

void AnalogTouchMZ::sample() {
    pinMode(_xl, INPUT);
    pinMode(_xr, OUTPUT);
    pinMode(_yu, OUTPUT);
    pinMode(_yd, INPUT);
    digitalWrite(_yu, LOW);
    digitalWrite(_xr, HIGH);
    int p = analogRead(_xl);

    if (p > 4000) {
        _pressed = false;
        return;
    }
    _pressed = true;

    pinMode(_xl, OUTPUT);
    pinMode(_xr, OUTPUT);
    pinMode(_yu, INPUT);
    pinMode(_yd, INPUT);
    digitalWrite(_xl, LOW);
    digitalWrite(_xr, HIGH);
    int x = getSample(_yu);

    pinMode(_xl, INPUT);
    pinMode(_xr, INPUT);
    pinMode(_yu, OUTPUT);
    pinMode(_yd, OUTPUT);
    digitalWrite(_yu, LOW);
    digitalWrite(_yd, HIGH);
    int y = getSample(_xl);

    _pos.x = x / _scale_x;
    _pos.y = y / _scale_y;
}

int AnalogTouchMZ::pressure() {
    return _pressed ? 100 : 0;
}

void AnalogTouchMZ::setRotation(int r) {
    _rotation = r % 4;
}
