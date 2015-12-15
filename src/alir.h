#ifndef _aqua_light_ir_h
#define _aqua_light_ir_h

namespace AquaLightIR {

namespace Config {    

void setIncrementStep(unsigned int value);
unsigned int getIncrementStep();
void setIncrementMinimum(int value);
int getIncrementMinimum();
void setIncrementMaximum(int value);
int getIncrementMaximum();

} // namespace Config

} // namespace AquaLightIR

#endif
