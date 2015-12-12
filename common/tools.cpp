#include "tools.h"

#include <iostream>
#include <stdio.h>
#include <sstream>

const unsigned int STEP = 25;

RGBA::RGBA():
red(0),
green(0),
blue(0),
alpha(0){
}

RGBA::RGBA(int red, int green, int blue):
red(clamp(red)),
green(clamp(green)),
blue(clamp(blue)),
alpha(250){
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
alpha(copy.alpha){
}

RGBA::~RGBA(){
}

const RGBA & RGBA::operator=(const RGBA & copy){
    this->red = copy.red;
    this->green = copy.green;
    this->blue = copy.blue;
    this->alpha = copy.alpha;

    return *this;
}

bool RGBA::operator==(const RGBA & other){
    return red == other.red &&
           green == other.green &&
           blue == other.blue &&
           alpha == other.alpha;
}

int RGBA::getRed() const {
    return red;
}

void RGBA::setRed(int value){
    red = clamp(value);
}

void RGBA::upRed() {
    red = clamp(red + STEP);
}

void RGBA::downRed() {
    red = clamp(red - STEP);
}

int RGBA::getGreen() const {
    return green;
}

void RGBA::setGreen(int value){
    green = clamp(value);
}

void RGBA::upGreen() {
    green = clamp(green + STEP);
}

void RGBA::downGreen() {
    green = clamp(green - STEP);
}

int RGBA::getBlue() const {
    return blue;
}

void RGBA::setBlue(int value){
    blue = clamp(value);
}

void RGBA::upBlue() {
    blue = clamp(blue + STEP);
}

void RGBA::downBlue() {
    blue = clamp(blue - STEP);
}

int RGBA::getAlpha() const {
    return alpha;
}

void RGBA::setAlpha(int value){
    alpha = clamp(value);
}

void RGBA::upAlpha() {
    alpha = clamp(alpha + STEP);
}

void RGBA::downAlpha() {
    alpha = clamp(alpha - 10);
}

void RGBA::towards(const RGBA & to){
    if (red < to.red){
        upRed();
    } else if (red > to.red){
        downRed();
    }
    if (green < to.green){
        upGreen();
    } else if (green > to.green){
        downGreen();
    }
    if (blue < to.blue){
        upBlue();
    } else if (blue > to.blue){
        downBlue();
    }
    if (alpha < to.alpha){
        upAlpha();
    } else if (alpha > to.alpha){
        downAlpha();
    }
}


int RGBA::clamp(int value){
    if (value < 0){
        return 0;
    } else if (value > 250){
        return 250;
    }
    return value;
}

Color::Color(){
}

Color::Color(int red, int green, int blue):
current(red, green, blue),
toColor(red, green, blue){
}

Color::Color(int red, int green, int blue, int alpha):
current(red, green, blue, alpha),
toColor(red, green, blue, alpha){
}

Color::Color(const Color & copy):
current(copy.current),
toColor(copy.toColor){
}

Color::~Color(){
}

const Color & Color::operator=(const Color & copy){
    this->current = copy.current;
    this->toColor = copy.toColor;

    return *this;
}

const RGBA & Color::get() const {
    return current;
}

void Color::print() const {
    std::cout <<  "Current red " << current.getRed() << "   green " << current.getGreen()  << "  blue " <<  current.getBlue() << std::endl;
    std::cout <<  "To Color red " << toColor.getRed() << "   green " << toColor.getGreen()  << "  blue " <<  toColor.getBlue() << std::endl;
}
void Color::setTo(const Color & to){
    this->toColor = to.toColor;
}

bool Color::interpolate(){
    current.towards(toColor);
    return current == toColor;
}

