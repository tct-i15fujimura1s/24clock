#include "libs.h"
#include "main.h"
#include "mode20.h"

char mode20_title[] = HK_TA HK_I HK_MA HK_ONBIKI;

static char* time_data_arr[]={"1","2"."3","4","5","6","7","8","9","10","15","30","60"};
volatile int time_arr[]={60,120,180,240,300,360,420,480,540,600,900,1800,3600};
//static char temp[2] = {0,0};
static int i = 0;
static int j = 1;
volatile int waittime = 0;
static int tempo=120;

void do_mode20(UI_DATA* ud){


lcd_clear();
lcd_putstr(0,0,"->");
//temp[0] = time_data_arr[i];
lcd_putstr(2,0,time_data_arr[i]);
lcd_putstr(4,0,"mintimer");
//temp[0] = time_data_arr[j];
lcd_putstr(2,1,time_data_arr[j]);
lcd_putstr(4,1,"mintimer");


 switch(ud->sw){

 case KEY_SHORT_U:
   i=i+1;
   j=j+1;
   if(i>=sizeof(time_data_arr)){
     i=0;
   }
   if(j>=sizeof(time_data_arr)){
     j=0;
   }
   break;
   
 case KEY_SHORT_D:
   i=i-1;
   j=j-1;
   if(i<0){
     i=sizeof(time_data_arr)-1;
   }
   if(j<0){
     j=sizeof(time_data_arr)-1;
   }
   break;
   
 case KEY_LONG_R:
   waittime = time_arr[i];
   ud->mode=MODE_21;/*次は，モード変更*/
   break;
   
 case KEY_LONG_C: /* 中央キーの長押し */
   lcd_cursor(FALSE);
   lcd_blink(FALSE);
   ud->mode=MODE_0; /* メニューに移動 */
   break;
   
   
 default:
   break;
   
   
}
}
	
void do_mode21(UI_DATA* ud){
  tempo_compare = 3735 / tempo; /* 1000/(tempo * 16 / 60) を展開 */
  //temp[0]=time_data_arr[i];
  lcd_clear();
  lcd_putstr(0,0,time_data_arr[i]);
  lcd_putstr(2,0,"mintimer");
  lcd_putudec(0,1,4,waittime);
  lcd_putstr(4,1,"secwait...");


  if(sec_flag==TRUE) {
    waittime -= 1;
    if(waittime==0){
      snd_play("C C C ");
      lcd_cursor(FALSE);
      lcd_blink(FALSE);
      ud->mode=MODE_0; /* メニューに移動 */
    }
    sec_flag=FALSE;
  }
  switch(ud->sw){
  case KEY_LONG_C: /* 中央キーの長押し */
    lcd_cursor(FALSE);
    lcd_blink(FALSE);
    ud->mode=MODE_0; /* メニューに移動 */
    break;
    
  case KEY_LONG_L:
    ud->mode=MODE_20;/*次は，モード変更*/
    break;
    
  default:
    break;

  }
  
}
