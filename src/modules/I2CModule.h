#pragma once

#include <Wire.h>

class I2CModule {
    public:
        I2CModule();
        bool initialize();
    protected:
    private:
};

extern I2CModule *i2cModule;
