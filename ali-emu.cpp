#include <string>
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

const unsigned int STEP = 25;

class RGBA {
public:
    RGBA():
    red(0),
    green(0),
    blue(0),
    alpha(0){
    }
    RGBA(int red, int green, int blue):
    red(clamp(red)),
    green(clamp(green)),
    blue(clamp(blue)),
    alpha(250){
    }
    RGBA(int red, int green, int blue, int alpha):
    red(clamp(red)),
    green(clamp(green)),
    blue(clamp(blue)),
    alpha(clamp(alpha)){
    }
    RGBA(const RGBA & copy):
    red(copy.red),
    green(copy.green),
    blue(copy.blue),
    alpha(copy.alpha){
    }
    ~RGBA(){
    }

    const RGBA & operator=(const RGBA & copy){
        this->red = copy.red;
        this->green = copy.green;
        this->blue = copy.blue;
        this->alpha = copy.alpha;

        return *this;
    }

    bool operator==(const RGBA & other){
        return red == other.red &&
               green == other.green &&
               blue == other.blue &&
               alpha == other.alpha;
    }

    int getRed() const {
        return red;
    }

    void setRed(int value){
        red = clamp(value);
    }

    void upRed() {
        red = clamp(red + STEP);
    }

    void downRed() {
        red = clamp(red - STEP);
    }

    int getGreen() const {
        return green;
    }

    void setGreen(int value){
        green = clamp(value);
    }

    void upGreen() {
        green = clamp(green + STEP);
    }

    void downGreen() {
        green = clamp(green - STEP);
    }

    int getBlue() const {
        return blue;
    }

    void setBlue(int value){
        blue = clamp(value);
    }

    void upBlue() {
        blue = clamp(blue + STEP);
    }

    void downBlue() {
        blue = clamp(blue - STEP);
    }

    int getAlpha() const {
        return alpha;
    }

    void setAlpha(int value){
        alpha = clamp(value);
    }

    void upAlpha() {
        alpha = clamp(alpha + STEP);
    }

    void downAlpha() {
        alpha = clamp(alpha - 10);
    }

    void towards(const RGBA & to){
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

private:
    int clamp(int value){
        if (value < 0){
            return 0;
        } else if (value > 250){
            return 250;
        }
        return value;
    }

    int red;
    int green;
    int blue;
    int alpha;
};

class Color {
public:
    Color(){
    }
    Color(int red, int green, int blue):
    current(red, green, blue),
    toColor(red, green, blue){
    }
    Color(int red, int green, int blue, int alpha):
    current(red, green, blue, alpha),
    toColor(red, green, blue, alpha){
    }
    Color(const Color & copy):
    current(copy.current),
    toColor(copy.toColor){
    }
    ~Color(){
    }
    const Color & operator=(const Color & copy){
        this->current = copy.current;
        this->toColor = copy.toColor;

        return *this;
    }

    const RGBA & get() const {
        return current;
    }

    void print() const {
        std::cout <<  "Current red " << current.getRed() << "   green " << current.getGreen()  << "  blue " <<  current.getBlue() << std::endl;
        std::cout <<  "To Color red " << toColor.getRed() << "   green " << toColor.getGreen()  << "  blue " <<  toColor.getBlue() << std::endl;
    }
    void setTo(const Color & to){
        this->toColor = to.toColor;
    }
    bool interpolate(){
        current.towards(toColor);
        return current == toColor;
    }
private:
    RGBA current;
    RGBA toColor;
};

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const Color BLACK = Color(0,0,0);
const Color WHITE = Color(250,250,250);
const Color DAYBREAK = Color(250,250,100);
const Color FULLSUN = Color(250,250,250);
const Color SUNSET = Color(250,150,0);
const Color MOONLIGHT = Color(25,25,100);

const ALLEGRO_COLOR getColor(const Color & color) {
    return al_map_rgb(color.get().getRed(), color.get().getGreen(), color.get().getBlue());
}

void drawContent(const Color & color, ALLEGRO_FONT * font){
    al_clear_to_color(getColor(BLACK));
    const ALLEGRO_COLOR rgb = getColor(color);
    const int height = WINDOW_HEIGHT / 4;
    const int increment = WINDOW_WIDTH / 8;
    int ledPos = increment;
    //al_clear_to_color(al_map_rgb(color.red,color.green,color.blue));
    al_draw_filled_rectangle(0,0,WINDOW_WIDTH, height, rgb);

    al_draw_line(0,height + 10,WINDOW_WIDTH, height + 10, getColor(WHITE), 2);
    
    for (int i = 0; i < 7; i++){
        al_draw_filled_circle(ledPos, height + 30, 10, rgb);
        al_draw_filled_circle(ledPos, height + 60, 15, rgb);
        al_draw_filled_circle(ledPos, height + 90, 10, rgb);
        ledPos += increment;
    }

    al_draw_line(0,height + 110,WINDOW_WIDTH, height + 110, getColor(WHITE), 2);
    
    std::ostringstream out;
    out <<  "Red: " << color.get().getRed() << "   green: " << color.get().getGreen()  << "  blue: " <<  color.get().getBlue() << std::endl;
    al_draw_text(font, getColor(WHITE), 10, height + 150, ALLEGRO_ALIGN_LEFT, out.str().c_str());

    al_flip_display();
}

void outColor(const Color & color){
    //printf("Colors red %d   green %d   blue %d\n", color.red, color.green, color.blue);
    color.print();
}

int main(int argc, char **argv){

    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;

    if(!al_init() || !al_init_primitives_addon() || !al_init_font_addon() || !al_init_ttf_addon()) {
        fprintf(stderr, "failed to initialize allegro!\n");
        return -1;
    }

    if(!al_install_keyboard()) {
        fprintf(stderr, "failed to initialize the keyboard!\n");
        return -1;
    }

    display = al_create_display(WINDOW_WIDTH, WINDOW_HEIGHT);
    if(!display) {
        fprintf(stderr, "failed to create display!\n");
        return -1;
    }

    event_queue = al_create_event_queue();
    if(!event_queue) {
        fprintf(stderr, "failed to create event_queue!\n");
        al_destroy_display(display);
        return -1;
    }
    
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    ALLEGRO_FONT * font = al_load_ttf_font("railway.ttf", 25, 0);

    Color current = DAYBREAK;
    current.setTo(FULLSUN);
    
    int state = 0;
    
    drawContent(BLACK, font);

    al_flip_display();

    ALLEGRO_TIMEOUT timeout;
    al_init_timeout(&timeout, .2);
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

    bool quit = false;
    while (!quit){
        ALLEGRO_EVENT ev;
        al_wait_for_event_until(event_queue, &ev, &timeout);
        if(ev.type == ALLEGRO_EVENT_KEY_UP) {
             switch(ev.keyboard.keycode) {
                 case ALLEGRO_KEY_ESCAPE:
                     quit = true;
                     break;
             }
        }

        switch (state){
            case 0:
                if (current.interpolate()){
                    current.setTo(SUNSET);
                    state = 1;
                }
                break;
            case 1:
                if (current.interpolate()){
                    current.setTo(MOONLIGHT);
                    state = 2;
                }
                break;
            case 2:
                if (current.interpolate()){
                    current.setTo(DAYBREAK);
                    state = 3;
                }
                break;
            case 3:
                if (current.interpolate()){
                    current.setTo(FULLSUN);
                    state = 0;
                }
                break;
        }

        //outColor(current);

        drawContent(current, font);
    


        al_rest(.25);
    }

    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
}
