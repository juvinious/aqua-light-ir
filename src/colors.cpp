#include "colors.h"
#include "alir.h"

#include <iostream>
#include <stdio.h>
#include <sstream>

using namespace AquaLightIR;

RGBA::RGBA():
red(Config::getIncrementMinimum()),
green(Config::getIncrementMinimum()),
blue(Config::getIncrementMinimum()),
alpha(Config::getIncrementMinimum()){
}

RGBA::RGBA(int red, int green, int blue):
red(clamp(red)),
green(clamp(green)),
blue(clamp(blue)),
alpha(Config::getIncrementMaximum()){
}

RGBA::RGBA(int red, int green, int blue, int alpha):
red(clamp(red)),
green(clamp(green)),
blue(clamp(blue)),
alpha(clamp(alpha)){
}

RGBA::RGBA(const RGBA & copy):
red(copy.red),
green(copy.green),
blue(copy.blue),
alpha(copy.alpha),
Notifier(copy) {
}

RGBA::~RGBA(){
}

const RGBA & RGBA::operator=(const RGBA & copy){
    this->red = copy.red;
    this->green = copy.green;
    this->blue = copy.blue;
    this->alpha = copy.alpha;
    
    Notifier::operator=(copy);

    return *this;
}

bool RGBA::operator==(const RGBA & other){
    return red == other.red &&
           green == other.green &&
           blue == other.blue &&
           alpha == other.alpha;
}

int RGBA::get(const Events::Channel & channel) const {
    switch (channel){
        case Events::RED:
            return red;
        case Events::GREEN:
            return green;
        case Events::BLUE:
            return blue;
        case Events::ALPHA:
            return alpha;
    }
    
    return 0;
}

void RGBA::send(const Events::Op & op, const Events::Channel & channel, int modifier) {
    int & value = lookup(channel);
    switch (op){
        case Events::UP:
            value = clamp(value + Config::getIncrementStep());
            notify(Events::Event(op, channel, value));
            break;
        case Events::DOWN:
            value = clamp(value - Config::getIncrementStep());
            notify(Events::Event(op, channel, value));
            break;
        case Events::SET: {
            const int set = clamp(modifier);
            if (set % Config::getIncrementStep() == 0){
                while (value != modifier){
                    if (value < modifier){
                        send(Events::UP, channel);
                    } else {
                        send(Events::DOWN, channel);
                    }
                }
            }
            break;
        }
    }
}

void RGBA::towards(const RGBA & to){
    if (red < to.red){
        send(Events::UP, Events::RED);
    } else if (red > to.red){
        send(Events::DOWN, Events::RED);
    }
    if (green < to.green){
        send(Events::UP, Events::GREEN);
    } else if (green > to.green){
        send(Events::DOWN, Events::GREEN);
    }
    if (blue < to.blue){
        send(Events::UP, Events::BLUE);
    } else if (blue > to.blue){
        send(Events::DOWN, Events::BLUE);
    }
    if (alpha < to.alpha){
        send(Events::UP, Events::ALPHA);
    } else if (alpha > to.alpha){
        send(Events::DOWN, Events::ALPHA);
    }
}

int & RGBA::lookup(const Events::Channel & channel) {
    switch (channel) {
        case Events::RED:
            return red;
        case Events::GREEN:
            return green;
        case Events::BLUE:
            return blue;
        case Events::ALPHA:
            return alpha;
    }
    
    return red;
}

int RGBA::clamp(int value){
    if (value < Config::getIncrementMinimum()){
        return Config::getIncrementMinimum();
    } else if (value > Config::getIncrementMaximum()){
        return Config::getIncrementMaximum();
    }
    return value;
}

ColorHandler::ColorHandler(){
    setParent(&current);
}

ColorHandler::ColorHandler(int red, int green, int blue):
current(red, green, blue),
toColor(red, green, blue){
    setParent(&current);
}

ColorHandler::ColorHandler(int red, int green, int blue, int alpha):
current(red, green, blue, alpha),
toColor(red, green, blue, alpha){
    setParent(&current);
}

ColorHandler::ColorHandler(const ColorHandler & copy):
current(copy.current),
toColor(copy.toColor){
    setParent(&current);
}

ColorHandler::~ColorHandler(){
}

const ColorHandler & ColorHandler::operator=(const ColorHandler & copy){
    this->current = copy.current;
    this->toColor = copy.toColor;
    this->setParent(&current);

    return *this;
}

const RGBA & ColorHandler::get() const {
    return current;
}

void ColorHandler::print() const {
    std::cout <<  "Current red " << current.get(Events::RED) << "   green " << current.get(Events::GREEN)  << "  blue " <<  current.get(Events::BLUE) << std::endl;
    std::cout <<  "To Color red " << toColor.get(Events::RED) << "   green " << toColor.get(Events::GREEN)  << "  blue " <<  toColor.get(Events::BLUE) << std::endl;
}
void ColorHandler::setTo(const ColorHandler & to){
    this->toColor = to.toColor;
}

bool ColorHandler::interpolate(){
    current.towards(toColor);
    return current == toColor;
}
