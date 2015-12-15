#include "events.h"

using namespace AquaLightIR;
using namespace Events;

unsigned int NEXT_ID = 1;

std::string AquaLightIR::Events::opName(const Op & op){
    switch (op){
        case UP:
            return "Up";
        case DOWN:
            return "Down";
        case SET:
            return "Set";
    }
    
    return "";
}

std::string AquaLightIR::Events::channelName(const Channel & channel){
    switch(channel){
        case RED:
            return "Red";
        case GREEN:
            return "Green";
        case BLUE:
            return "Blue";
        case ALPHA:
            return "Alpha";
    }
    
    return "";
}

Event::Event(const Op & op, const Channel & channel, int value):
op(op),
channel(channel),
value(value){
}

Event::Event(const Event & copy):
op(copy.op),
channel(copy.channel),
value(copy.value){
}

Event::~Event(){
}

const Event & Event::operator=(const Event & copy){
    this->op = copy.op;
    this->channel = copy.channel;
    this->value = value;
    
    return *this;
}

Notifier::Notifier(){
}

Notifier::Notifier(const Notifier & copy){
    copySubscribers(copy);
}

Notifier::~Notifier(){
    for (SubMap::iterator i = subscribers.begin(); i != subscribers.end(); i++){
        Listener * listener = i->second;
        listener->parent = 0;
    }
}

const Notifier & Notifier::operator=(const Notifier & copy){
    
    this->subscribers = copy.subscribers;
    for (SubMap::iterator i = subscribers.begin(); i != subscribers.end(); i++){
        Listener * listener = i->second;
        listener->parent = this;
    }
    
    return *this;
}

void Notifier::copySubscribers(const Notifier & copy){
    for (SubMap::iterator i = subscribers.begin(); i != subscribers.end(); i++){
        Listener * listener = i->second;
        listener->parent = this;
    }
}

void Notifier::notify(const Event & event){
    for (SubMap::iterator i = subscribers.begin(); i != subscribers.end(); i++){
        Listener * listener = i->second;
        listener->onEvent(event);
    }
}

void Notifier::subscribe(Listener * listener) {
    subscribers[listener->getID()] = listener;
}

void Notifier::unsubscribe(Listener * listener) {
    SubMap::iterator removable = subscribers.find(listener->getID());
    if (removable != subscribers.end()){
        subscribers.erase(removable);
    }
}


Listener::Listener():
id(NEXT_ID++),
parent(0){
}

Listener::Listener(Notifier * parent):
id(NEXT_ID++),
parent(parent){
    parent->subscribe(this);
}

Listener::~Listener(){
    if (parent != 0){
        parent->unsubscribe(this);
    }
}

void Listener::setParent(Notifier * parent) {
    this->parent = parent; 
    parent->subscribe(this);
}

Repeater::Repeater(){
}

Repeater::~Repeater(){
}

void Repeater::onEvent(const Event & event){
    notify(event);
}
