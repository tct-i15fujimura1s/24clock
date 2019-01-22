#include "libs.h"
#include "main.h"
#include "mode40.h"

char mode80_title[] = HK_KA HK_RA HK_HU HK_RU;

static int cycle = 0;
static int mode = 0;
static int d = 0;
static int r = 0, g = 0, b = 0;
void do_mode80(UI_DATA *ud){
  if(ud->sw == KEY_LONG_C){
    ud->mode = MODE_0;
    return;
  }
  
  if(tmv_flag==TRUE){
    if(cycle == 0){
      if(d == 4){
	d = 0;
	mode = (mode + 1) % 3;
      }else{
	d++;
      }
      if(mode == 0){
	b = 3 - d;
	r = d;
	g = 0;
      }else if(mode == 1){
	r = 3 - d;
	g = d;
	b = 0;
      }else if(mode == 2){
	g = 3 - d;
	b = d;
	r = 0;
      }
    }

#define led(c, color) \
    if(cycle < (c)) ENABLE_LED_##color(); \
    else DISABLE_LED_##color();

    led(r, RED);
    led(g, GREEN);
    led(b, BLUE);

    cycle = (cycle + 1) & 3;
    
    tmv_flag = FALSE;
  }
}
