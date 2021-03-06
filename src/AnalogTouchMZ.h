#ifndef _ANALOGTOUCHMZ_H
#define _ANALOGTOUCHMZ_H

#include <Cariad.h>

#if !defined(max)
#define max(A,B) ((A) > (B) ? (A) : (B))
#endif

class AnalogTouchMZ : public Touch {
    private:
        static const uint8_t ANALOGTOUCHMZ_SMPSIZE = 50;

    public: 
        AnalogTouchMZ(uint8_t xl, uint8_t xr, uint8_t yu, uint8_t yd, int w, int h) : Touch(w, h), 
            _xl(xl), _xr(xr), _yu(yu), _yd(yd) {}

        void sample();
        int getSample(uint8_t);
        int x();
        int y();
        int rawX() { return x(); };
        int rawY() { return y(); };

        void offsetX(int ox) { _offset_x = ox; }
        void offsetY(int oy) { _offset_y = oy; }
        void scaleX(float sx) { _scale_x = sx; }
        void scaleY(float sy) { _scale_y = sy; }

        bool isPressed();

        void initializeDevice();

        int pressure();

        void setRotation(int r);

    private:
        struct coord _pos;
        bool _pressed;
        uint8_t _xl;
        uint8_t _xr;
        uint8_t _yu;
        uint8_t _yd;
        float _scale_x;
        float _scale_y;
        int _offset_x;
        int _offset_y;
        int _pressure;
        int _rotation;
};

#endif

