#include "wiring.h"


void readsensors(sensorpin){
    int sensorvalue  = analogRead(sensorpin)
    sensorvalue = sensorvalue*4.9 // voltage divider values to give actual value
}
