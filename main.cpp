#include "mbed.h"
#include "QEI.h"
#include <math.h>

// PwmOut motor_left_pwm(D6);
// DigitalOut motor_left_bipolar(D9);
// DigitalOut motor_left_direction(D7);
// PwmOut motor_right_pwm(D5);
// DigitalOut motor_right_bipolar(D8);
// DigitalOut motor_right_direction(D4);

QEI encoder_l(D14, D15, NC, 256);
QEI encoder_r(D10, D11, NC, 256);

Serial pc(USBTX, USBRX);



class Motor {

protected:
    PwmOut pwm;
    DigitalOut direction;
    DigitalOut bipolar;
    float speed;

public:
    Motor(PinName _pwm, PinName _direction, PinName _bipolar) : pwm(_pwm), direction(_direction), bipolar(_bipolar) {}

    virtual float set_speed(float s) {
        if (s > -1.0 && s < 1.0) {
            speed = s;
        }

        return speed;
    }

    float get_speed() {
        return speed;
    }
};

class Unipolar : public Motor {
    public:
    Unipolar(PinName _pwm, PinName _direction, PinName _bipolar) : Motor(_pwm, _direction, _bipolar) {
        pwm.write(0.0);
        pwm.period_us(50);
        direction.write(0);
        bipolar.write(0);

    }

    float set_speed(float s) {
        if (s > -1.0 && s < 1.0) {
            speed = s;
        }

        if (s < 0) {
            direction.write(1);
        } else if (s > 0) {
            direction.write(0);
        }
        pwm.write(abs(s) * 100.0);

        return speed;
    }
};


// class Wheel {
// private:
//     QEI encoder;
//     Motor* wheel;

// public:
//     Wheel(QEI _encoder, Motor * _wheel) : encoder(_encoder), wheel(_wheel) {}

//     void setrpm

// }


int main() {
    pc.printf("HELLO");

    Unipolar left(D6, D7, D9);
    Unipolar right(D5, D4, D8);

    encoder_l.reset();
    encoder_r.reset();
    
    while(1) {

        int lpulses = encoder_l.getPulses();
        int rpulses = encoder_r.getPulses();

        pc.printf("l: %i      r: %i\n", lpulses, rpulses);
        
        left.set_speed(0.5);
        right.set_speed(0.5);

        wait(0.5);

        left.set_speed(0.0);
        right.set_speed(0.0);

        wait(0.5);
    }
}
