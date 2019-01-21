#include "libs.h"
#include "main.h"
#include "mode40.h"

char mode80_title[] = HK_KA HK_RA HK_HU HK_RU;

static int cycle = 0;
static int mode = 0;
static int deg = 0;
static int r = 0, g = 0, b = 0;
void do_mode80(UI_DATA *ud){
  /*if(tmv_flag==TRUE)*/{
    if(cycle == 0){
      if(deg == 16){
	deg = 0;
	mode = (mode + 1) % 3;
      }else{
	deg++;
      }
      if(mode == 0){
	b = 15 - deg;
	r = deg;
      }else if(mode == 1){
	r = 15 - deg;
	g = deg;
      }else if(mode == 2){
	g = 15 - deg;
	b = deg;
      }
    }

    if(r > cycle) ENABLE_LED_RED();
    else DISABLE_LED_RED();
    if(g > cycle) ENABLE_LED_GREEN();
    else DISABLE_LED_GREEN();
    if(b > cycle) ENABLE_LED_BLUE();
    else DISABLE_LED_BLUE();
    
    cycle = (cycle + 1) & 0xf;
    tmv_flag = FALSE;
  }
}
