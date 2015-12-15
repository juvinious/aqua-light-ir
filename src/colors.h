#ifndef _aqua_light_ir_colors_h
#define _aqua_light_ir_colors_h

#include <map>
#include "events.h"

namespace AquaLightIR {

class RGBA : public Events::Notifier {
public:
    RGBA();    
    RGBA(int red, int green, int blue);
    RGBA(int red, int green, int blue, int alpha);
    RGBA(const RGBA & copy);
    ~RGBA();

    const RGBA & operator=(const RGBA & copy);
    bool operator==(const RGBA & other);
    
    int get(const Events::Channel &) const;
    void send(const Events::Op &, const Events::Channel &, int modifier = 0);
    void towards(const RGBA & to);

private:
    int & lookup(const Events::Channel &);
    
    int clamp(int value);
    int red;
    int green;
    int blue;
    int alpha;
};

class ColorHandler : public Events::Repeater {
public:
    ColorHandler();    
    ColorHandler(int red, int green, int blue);
    ColorHandler(int red, int green, int blue, int alpha);
    ColorHandler(const ColorHandler & copy);
    ~ColorHandler();
    const ColorHandler & operator=(const ColorHandler & copy);
    const RGBA & get() const;
    void print() const;
    void setTo(const ColorHandler & to);
    bool interpolate();
    
private:
    RGBA current;
    RGBA toColor;
};

class Fixture {
public:
    Fixture();
    ~Fixture();
    
private:
    ColorHandler red, green, blue, white;
};

}

#endif
