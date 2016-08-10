/* Hello World program */

#include <stdio.h>
#include "Wiring.h"

void setup(){
	init_motors();

}


main(void)
{
	int pot_var = 0;
    printf("Hello World\n");
    

    while(1){
    	pot_var = analogRead(adcPin)
    }


    return 0;
}
