#include "time.h"

// 注意: sec, min: [-59, 59], h: [-23, 23]

void time_add_seconds(TIME *time, int sec) {
  sec += time->seconds;
  if(sec < 0) {
    time_add_minutes(time, sec / 60);
    sec += 60;
  } else if(sec >= 60) {
    time_add_minutes(time, sec / 60);
    sec -= 60;
  }
  time->seconds = sec;
}

void time_add_minutes(TIME *time, int min) {
  min += time->minutes;
  if(min < 0) {
    time_add_hours(time, min / 60);
    min += 60;
  } else if(min >= 60) {
    time_add_hours(time, min / 60);
    min -= 60;
  }
  time->minutes = min;
}

void time_add_hours(TIME *time, int h) {
  h += 24;
  time->hours = (time->hours + h) % 24;
}

void time_to_string(TIME *time, char *temp) {
  temp[0] = '0' + (time->seconds % 10);
  temp[1] = '0' + (time->seconds / 10);
  temp[2] = ':';
  temp[3] = '0' + (time->minutes % 10);
  temp[4] = '0' + (time->minutes / 10);
  temp[5] = ':';
  temp[6] = '0' + (time->hours % 10);
  temp[7] = '0' + (time->hours / 10);
  temp[8] = '\0';
}
