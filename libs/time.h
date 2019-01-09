#ifndef TIME_H
#define TIME_H

typedef struct {
  int seconds;
  int minutes;
  int hours;
} TIME;

extern void time_add_seconds(TIME *, int);
extern void time_add_minutes(TIME *, int);
extern void time_add_hours(TIME *, int);
extern const char *time_to_string(TIME *);

#endif
