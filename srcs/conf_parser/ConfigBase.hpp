// confBase.hpp
#pragma once

#include <iostream>

namespace Config {
    class confBase {
    protected:
        confBase(const confBase &) {}

    public:
        confBase() {}

        virtual ~confBase() {}

        virtual confBase &operator=(const confBase &) { return *this; }

        virtual void print() const {
            std::cout << "" << std::endl;
        }
    };
}