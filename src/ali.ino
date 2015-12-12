#include <IRremote.h>
#include <IRremoteInt.h>

static const uint32_t BTN_POWER = 0x807F00FF;
static const uint32_t BTN_MAX = 0x807FC23D;
static const uint32_t BTN_9_AM = 0x807F40BF;
static const uint32_t BTN_12_PM = 0x807FC03F;
static const uint32_t BTN_3_PM = 0x807F827D;
static const uint32_t BTN_6_PM = 0x807FA25D;
static const uint32_t BTN_9_PM = 0x807F926D;
static const uint32_t BTN_12_AM = 0x807FD02F;
static const uint32_t BTN_3_AM = 0x807F50AF;
static const uint32_t BTN_6_AM = 0x807F609F;
static const uint32_t BTN_24_7 = 0x807F02FD;
static const uint32_t BTN_DEMO = 0x807F12ED;
static const uint32_t BTN_CLOUDY = 0x807F708F;
static const uint32_t BTN_THUNDER = 0x807FF00F;
static const uint32_t BTN_SUNNY = 0x807FB24D;
static const uint32_t BTN_MOON = 0x807F32CD;
static const uint32_t BTN_WHITE = 0x807F48B7;
static const uint32_t BTN_WHITE_UP = 0x807F6897;
static const uint32_t BTN_WHITE_DOWN = 0x807F58A7;
static const uint32_t BTN_RED = 0x807FC837;
static const uint32_t BTN_RED_UP = 0x807FE817;
static const uint32_t BTN_RED_DOWN = 0x807FD827;
static const uint32_t BTN_GREEN = 0x807F8A75;
static const uint32_t BTN_GREEN_UP = 0x807FAA55;
static const uint32_t BTN_GREEN_DOWN = 0x807F9A65;
static const uint32_t BTN_BLUE = 0x807F0AF5;
static const uint32_t BTN_BLUE_UP = 0x807F2AD5;
static const uint32_t BTN_BLUE_DOWN = 0x807F1AE5;
static const uint32_t BTN_MEMORY_1 = 0x807F7887;
static const uint32_t BTN_MEMORY_2 = 0x807FF807;
static const uint32_t BTN_MEMORY_3 = 0x807FBA45;
static const uint32_t BTN_MEMORY_4 = 0x807F3AC5;
static const uint32_t BTN_NULL = 0xFFFFFFF;


static const unsigned short BTN_BIT_LENGTH = 32;
static const unsigned short BTN_INCREMENT = 10;
static const unsigned short BTN_MAX_LENGTH = 100;
static const unsigned short BTN_MIN_LENGTH = 0;


unsigned short clamp(unsigned short value){
  if (value > BTN_MAX_LENGTH){
    return BTN_MAX_LENGTH;
  }
  return BTN_MAX_LENGTH;
}

unsigned short get_interpolate_steps(unsigned short from, unsigned short to){
  
}

/**
 * Use this to retrieve values from controller.
 * Set the pin to whatever output you connect the receiver to
 * and then just update on loop. It will print the values in hex.
 * Example:
 *                      [ ( ) ]
 *                       | | |
 *                    Out Gnd Vcc
 *                     |   |   |
 *                Pin 11  Gnd  5V
 */
class Receiver {
public:
    Receiver(int pin):
    receiver(pin){
      receiver.enableIRIn();
    }
    ~Receiver(){
      
    }
    void update(){
      if (receiver.decode(&results)){
        if (Serial){
          Serial.println("--------------------------");
          Serial.println(results.value, HEX);
          Serial.print("Length in bits: ");
          Serial.print(results.bits);
          Serial.println("--------------------------");
          Serial.println("");
        }
        receiver.resume();
      }
    }
private:
    IRrecv receiver;
    decode_results results;
};

/**
 * Probably overkill to wrap this but it should help with clarity
 */
class Transmitter {
public:
    Transmitter(){
    }
    ~Transmitter(){
    }
    void send(const uint32_t value, const unsigned short bits){
        sender.sendNEC(value, bits);
    }
private:
    // Mega 2560 pin 9 | Uno pin 3 -> 100/150 ohm
    IRsend sender;
};

class Colors {
public:
    enum Type {
        WHITE=0,
        RED,
        GREEN,
        BLUE,
    };
    Colors():
    white(BTN_MAX_LENGTH),
    red(BTN_MAX_LENGTH),
    green(BTN_MAX_LENGTH),
    blue(BTN_MAX_LENGTH){
    }
    Colors(const Colors & copy):
    white(copy.white),
    red(copy.red),
    green(copy.green),
    blue(copy.blue){
      
    }
    Colors(unsigned short w, unsigned short r, unsigned short g, unsigned short b):
    white(w),
    red(r),
    green(g),
    blue(b){
    }
    ~Colors(){
      
    }
    const Colors & operator=(const Colors & copy){
      this->white = copy.white;
      this->red = copy.red;
      this->green = copy.green;
      this->blue = copy.blue;
      return *this;
    }
    unsigned short get(const Type & color){
        switch (color) {
          case WHITE:
            return white;
            break;
          case RED:
            return red;
            break;
          case GREEN:
            return green;
            break;
          case BLUE:
            return blue;
            break;
          default:
            break;
        }
        return 0;
    }
    void set(const Type & color, unsigned short v){
        static unsigned short value = clamp(v);
        switch (color) {
          case WHITE:
            white = value;
            break;
          case RED:
            red = value;
            break;
          case GREEN:
            green = value;
            break;
          case BLUE:
            blue = value;
            break;
          default:
            break;
        }
    }
    
    void up(const Type & color){
        switch (color) {
          case WHITE:
            white += white < BTN_MAX_LENGTH ? BTN_INCREMENT : 0;
            break;
          case RED:
            red  += red < BTN_MAX_LENGTH ? BTN_INCREMENT : 0;
            break;
          case GREEN:
            green += green < BTN_MAX_LENGTH ? BTN_INCREMENT : 0;
            break;
          case BLUE:
            blue += blue < BTN_MAX_LENGTH ? BTN_INCREMENT : 0;
            break;
          default:
            break;
        }
    }
    
    void down(const Type & color){
        switch (color) {
          case WHITE:
            white -= white > BTN_MIN_LENGTH ? BTN_INCREMENT : 0;
            break;
          case RED:
            red  -= red < BTN_MIN_LENGTH ? BTN_INCREMENT : 0;
            break;
          case GREEN:
            green -= green < BTN_MIN_LENGTH ? BTN_INCREMENT : 0;
            break;
          case BLUE:
            blue -= blue < BTN_MIN_LENGTH ? BTN_INCREMENT : 0;
            break;
          default:
            break;
        }
    }
private:
    unsigned short white;
    unsigned short red;
    unsigned short green;
    unsigned short blue;
};

class Fixture {
public:
    enum Mode {
        POWER_OFF = 1,
        CYCLE_24_7,
        DEMO,
        CLOUDY,
        SUNNY,
        MOON,
    };
    Fixture(){
    
    }
    ~Fixture(){
      
    }
private:
    // Mega 2560 pin 9 | Uno pin 3 -> 100/150 ohm
    Transmitter sender;
    Colors current;
    Colors daybreak;
    Colors highsun;
    Colors sunset;
    Colors moonlight;
};

//Receiver receiver(11);

void setup() {
  Serial.begin(9600);
}

void loop() {
  /*if (Serial.read() != -1){
    Serial.println("Sending..");
    sender.send(BTN_POWER, BTN_BIT_LENGTH);
  }*/
}

