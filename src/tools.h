#ifndef _aqua_light_ir_h
#define _aqua_light_ir_h

#include <map>

namespace AquaLightIR {

class RGBA {
public:
    RGBA();    
    RGBA(int red, int green, int blue);
    RGBA(int red, int green, int blue, int alpha);
    RGBA(const RGBA & copy);
    ~RGBA();

    const RGBA & operator=(const RGBA & copy);
    bool operator==(const RGBA & other);
    
    enum Op {
        UP,
        DOWN,
        SET
    };
    
    enum Channel {
        RED,
        GREEN,
        BLUE,
        ALPHA,
    };
    
    class EventListener {
    public:
        EventListener();
        EventListener(RGBA * parent);
        virtual ~EventListener();
        virtual void onChange(const RGBA::Op &, const RGBA::Channel &) = 0;
        inline int getID() const { return this->id; }
        void setParent(RGBA * parent);
    private:
        const int id;
        RGBA * parent;
        friend class RGBA;
    };
    
    int get(const Channel &) const;
    void send(const Op &, const Channel &, int modifier = 0);
    void towards(const RGBA & to);

private:
    int & lookup(const Channel &);
    void notify(const Op &, const Channel &);
    void subscribe(EventListener *);
    void unsubscribe(EventListener *);
    
    int clamp(int value);
    int red;
    int green;
    int blue;
    int alpha;
    
    std::map<int, EventListener *> subscribers;
};

class ColorHandler : public RGBA::EventListener {
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
    
    void onChange(const RGBA::Op &, const RGBA::Channel &);
    
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
