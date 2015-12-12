#include "tools.h"

#include <iostream>
#include <stdio.h>
#include <sstream>

using namespace AquaLightIR; 

const unsigned int STEP = 25;
unsigned int NEXT_ID = 1;

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
alpha(copy.alpha),
subscribers(copy.subscribers){
    for (std::map<int, EventListener *>::iterator i = subscribers.begin(); i != subscribers.end(); i++){
        EventListener * listener = i->second;
        listener->parent = this;
    }
}

RGBA::~RGBA(){
    for (std::map<int, EventListener *>::iterator i = subscribers.begin(); i != subscribers.end(); i++){
        EventListener * listener = i->second;
        listener->parent = NULL;
    }
}

RGBA::EventListener::EventListener():
id(NEXT_ID++),
parent(NULL){
}

RGBA::EventListener::EventListener(RGBA * parent):
id(NEXT_ID++),
parent(parent){
    parent->subscribe(this);
}

RGBA::EventListener::~EventListener(){
    if (parent != NULL){
        parent->unsubscribe(this);
    }
}

void RGBA::EventListener::setParent(RGBA * parent) {
    this->parent = parent; 
    parent->subscribe(this);
}

const RGBA & RGBA::operator=(const RGBA & copy){
    this->red = copy.red;
    this->green = copy.green;
    this->blue = copy.blue;
    this->alpha = copy.alpha;
    this->subscribers = copy.subscribers;
    
    for (std::map<int, EventListener *>::iterator i = subscribers.begin(); i != subscribers.end(); i++){
        EventListener * listener = i->second;
        listener->parent = this;
    }

    return *this;
}

bool RGBA::operator==(const RGBA & other){
    return red == other.red &&
           green == other.green &&
           blue == other.blue &&
           alpha == other.alpha;
}

int RGBA::get(const Channel & channel) const {
    switch (channel){
        case RED:
            return red;
        case GREEN:
            return green;
        case BLUE:
            return blue;
        case ALPHA:
            return alpha;
    }
    
    return 0;
}

void RGBA::send(const Op & op, const Channel & channel, int modifier) {
    int & value = lookup(channel);
    switch (op){
        case UP:
            value = clamp(value + STEP);
            notify(op, channel);
            break;
        case DOWN:
            value = clamp(value - STEP);
            notify(op, channel);
            break;
        case SET: {
            const int set = clamp(modifier);
            if (set % STEP == 0){
                while (value != modifier){
                    if (value < modifier){
                        send(UP, channel);
                    } else {
                        send(UP, channel);
                    }
                }
            }
            break;
        }
    }
}

void RGBA::towards(const RGBA & to){
    if (red < to.red){
        send(UP, RED);
    } else if (red > to.red){
        send(DOWN, RED);
    }
    if (green < to.green){
        send(UP, GREEN);
    } else if (green > to.green){
        send(DOWN, GREEN);
    }
    if (blue < to.blue){
        send(UP, BLUE);
    } else if (blue > to.blue){
        send(DOWN, BLUE);
    }
    if (alpha < to.alpha){
        send(UP, ALPHA);
    } else if (alpha > to.alpha){
        send(DOWN, ALPHA);
    }
}

int & RGBA::lookup(const Channel & channel) {
    switch (channel) {
        case RED:
            return red;
        case GREEN:
            return green;
        case BLUE:
            return blue;
        case ALPHA:
            return alpha;
    }
    
    return red;
}

int RGBA::clamp(int value){
    if (value < 0){
        return 0;
    } else if (value > 250){
        return 250;
    }
    return value;
}

void RGBA::notify(const Op & op, const Channel & channel){
    for (std::map<int, EventListener *>::iterator i = subscribers.begin(); i != subscribers.end(); i++){
        EventListener * listener = i->second;
        listener->onChange(op, channel);
    }
}

void RGBA::subscribe(EventListener * listener) {
    subscribers[listener->getID()] = listener;
}

void RGBA::unsubscribe(EventListener * listener) {
    std::map<int, EventListener *>::iterator removable = subscribers.find(listener->getID());
    if (removable != subscribers.end()){
        subscribers.erase(removable);
    }
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
    std::cout <<  "Current red " << current.get(RGBA::RED) << "   green " << current.get(RGBA::GREEN)  << "  blue " <<  current.get(RGBA::BLUE) << std::endl;
    std::cout <<  "To Color red " << toColor.get(RGBA::RED) << "   green " << toColor.get(RGBA::GREEN)  << "  blue " <<  toColor.get(RGBA::BLUE) << std::endl;
}
void ColorHandler::setTo(const ColorHandler & to){
    this->toColor = to.toColor;
}

bool ColorHandler::interpolate(){
    current.towards(toColor);
    return current == toColor;
}

void ColorHandler::onChange(const RGBA::Op & op, const RGBA::Channel & channel){
    //std::cout << "CHANGED!" << std::endl;
}
