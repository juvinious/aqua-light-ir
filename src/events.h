#ifndef _aqua_light_ir_events_h
#define _aqua_light_ir_events_h

#include <string>
#include <map>

namespace AquaLightIR {
namespace Events {
    
class Listener;

enum Op {
    UP,
    DOWN,
    SET,
};

enum Channel {
    RED,
    GREEN,
    BLUE,
    ALPHA,
};

std::string opName(const Op &);
std::string channelName(const Channel &);

class Event {
public:
    Event(const Op &, const Channel &, int value);
    Event(const Event &);
    ~Event();
    
    const Event & operator=(const Event &);
    Op op;
    Channel channel;
    int value;
};

class Notifier {
public:
    Notifier();
    Notifier(const Notifier &);
    virtual ~Notifier();
    const Notifier & operator=(const Notifier &);
    void notify(const Event &);
    void subscribe(Listener *);
    void unsubscribe(Listener *);
    
    typedef std::map<int, Listener *> SubMap;

protected:
    SubMap subscribers;
private:
    void copySubscribers(const Notifier &);
};

class Listener {
public:
    Listener();
    Listener(Notifier *);
    virtual ~Listener();
    virtual void onEvent(const Event &) = 0;
    inline int getID() const { return this->id; }
    void setParent(Notifier *);
private:
    const int id;
    Notifier * parent;
    friend class Notifier;
};

class Repeater : public Notifier, public Listener {
public:
    Repeater();
    virtual ~Repeater();
    
    void onEvent(const Event &);
};

} // namespace Events
} // namespace AquaLightIR

#endif
