CC = gccc

SRCS1=wait.s lcd.c util.c lcd_util.c matrix.c key.c timer.c ir.c speaker.c
OBJS1=wait.o lcd.o util.o lcd_util.o matrix.o key.o timer.o ir.o speaker.o

SRCS2=serial.c ADconv.c eeprom.c watchdog.c led.c
OBJS2=serial.o ADconv.o eeprom.o watchdog.o led.o

SRCS3=time.c mode0.c mode10.c mode20.c mode30.c mode40.c mode50.c mode60.c mode70.c
OBJS3=time.o mode0.o mode10.o mode20.o mode30.o mode40.o mode50.o mode60.o mode70.o

SRCS=$(SRCS1) $(SRCS2) $(SRCS3)
OBJS=$(OBJS1) $(OBJS2) $(OBJS3)

CFLAGS= -O2 -Wall -mh -mn 

all:crt0.o libutil.a

libutil.a:$(OBJS)
	h8300-elf-ar r libutil.a $(OBJS)
	h8300-elf-ranlib libutil.a
.c.o:
	h8300-elf-gcc $(CFLAGS) -c $<
.s.o:
	h8300-elf-gcc $(CFLAGS) -c $<

depend:$(SRCS)
	($(LANGSET) h8300-elf-gcc -M $(CFLAGS) $(SRCS) > depend)

clean:
	rm -f *~ *.o lib*.a depend

#手動で，以下にdependファイルを挿入した
#クロスコンパイルでなければ，makedependコマンドが
#bsd makeかgnu makeかが分かっていれば，.includeかincludeが
#使えるのだけどなぁ

lcd.o: lcd.c 3664.h wait.h libsCore.h
util.o: util.c 3664.h libsCore.h matrix.h lcd.h
lcd_util.o: lcd_util.c lcd_util.h lcd.h
matrix.o: matrix.c matrix.h libsCore.h 3664.h
key.o: key.c key.h libsCore.h 3664.h lcd_util.h
timer.o: timer.c timer.h libsCore.h 3664.h
ir.o: ir.c ir.h libsCore.h 3664.h
speaker.o: speaker.c speaker.h libsCore.h 3664.h
serial.o: serial.c serial.h libsCore.h 3664.h
ADconv.o: ADconv.c ADconv.h libsCore.h 3664.h
eeprom.o: eeprom.c eeprom.h libsCore.h 3664.h
watchdog.o: watchdog.c watchdog.h libsCore.h 3664.h
led.o: led.c led.h libsCore.h 3664.h
time.o: time.c time.h
mode0.o: mode0.c mode0.h libs.h main.h
mode10.o: mode10.c mode10.h libs.h main.h
mode20.o: mode20.c mode20.h libs.h main.h
mode30.o: mode30.c mode30.h libs.h main.h
mode40.o: mode40.c mode40.h libs.h main.h
mode50.o: mode50.c mode50.h libs.h main.h
mode60.o: mode60.c mode60.h libs.h main.h
mode70.o: mode70.c mode70.h libs.h main.h
