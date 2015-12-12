#ifndef _aqua_light_ir_h
#define _aqua_light_ir_h

class RGBA {
public:
    RGBA();    
    RGBA(int red, int green, int blue);
    RGBA(int red, int green, int blue, int alpha);
    RGBA(const RGBA & copy);
    ~RGBA();

    const RGBA & operator=(const RGBA & copy);
    bool operator==(const RGBA & other);
    int getRed() const;
    void setRed(int value);
    void upRed();    
    void downRed();
    int getGreen() const;
    void setGreen(int value);    
    void upGreen();
    void downGreen();
    int getBlue() const;
    void setBlue(int value);
    void upBlue();
    void downBlue();
    int getAlpha() const;
    void setAlpha(int value);
    void upAlpha();
    void downAlpha();
    void towards(const RGBA & to);

private:
    int clamp(int value);
    int red;
    int green;
    int blue;
    int alpha;
};

class Color {
public:
    Color();    
    Color(int red, int green, int blue);
    Color(int red, int green, int blue, int alpha);
    Color(const Color & copy);
    ~Color();
    const Color & operator=(const Color & copy);
    const RGBA & get() const;
    void print() const;
    void setTo(const Color & to);
    bool interpolate();
private:
    RGBA current;
    RGBA toColor;
};


#endif
