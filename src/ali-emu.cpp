#include <string>
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "tools.h"

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const AquaLightIR::ColorHandler BLACK = AquaLightIR::ColorHandler(0,0,0);
const AquaLightIR::ColorHandler WHITE = AquaLightIR::ColorHandler(250,250,250);
const AquaLightIR::ColorHandler DAYBREAK = AquaLightIR::ColorHandler(250,250,100);
const AquaLightIR::ColorHandler FULLSUN = AquaLightIR::ColorHandler(250,250,250);
const AquaLightIR::ColorHandler SUNSET = AquaLightIR::ColorHandler(250,150,0);
const AquaLightIR::ColorHandler MOONLIGHT = AquaLightIR::ColorHandler(25,25,100);

const ALLEGRO_COLOR getColor(const AquaLightIR::ColorHandler & color) {
    return al_map_rgb(color.get().get(AquaLightIR::RGBA::RED), color.get().get(AquaLightIR::RGBA::GREEN), color.get().get(AquaLightIR::RGBA::BLUE));
}

void drawContent(const AquaLightIR::ColorHandler & color, ALLEGRO_FONT * font){
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
    out <<  "Red: " << color.get().get(AquaLightIR::RGBA::RED) << "   green: " << color.get().get(AquaLightIR::RGBA::GREEN)  << "  blue: " <<  color.get().get(AquaLightIR::RGBA::BLUE) << std::endl;
    al_draw_text(font, getColor(WHITE), 10, height + 150, ALLEGRO_ALIGN_LEFT, out.str().c_str());

    al_flip_display();
}

void outColor(const AquaLightIR::ColorHandler & color){
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

    AquaLightIR::ColorHandler current = DAYBREAK;
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
