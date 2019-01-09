all:main.mot


main.mot: main
	h8300-elf-objcopy -O srec main main.mot

#ライブラリを作成し，一気に実行可能ファイルを作る
main:main.c subdirs
	h8300-elf-gcc -O2 -Wall -nostdlib -mh -mn \
         -Xlinker -T h8-3664.x -L./libs -I./libs \
	-o main main.c  ./libs/crt0.o -lutil -lc -lgcc

subdirs:
	(cd libs;make all)

# H8マイコンボードにプログラムをロードする
install: main.mot 
	h8write -3664 main.mot /dev/cu.usbserial-*

./h8write.fast:h8write.fast.c
	gcc -Wall h8write.fast.c -o h8write.fast  2>/dev/null
# H8マイコンボードにプログラムをロードする
install-fast: main.mot ./h8write.fast
	./h8write.fast -3664 main.mot /dev/cu.usbserial-*

# 印刷用PSファイルの作成
createps:main.ps
main.ps:main.c
	a2ps-j -t -h -n -L \
	"%month %mday %year %hour:%min (`whoami` : `pwd | sed 's/\/.*\///'`)" \
	 main.c > main.ps

# PSファイルの画面表示
display:main.ps
	gv main.ps

# PSファイルの印刷
printout:main.ps
	lpr main.ps

# 各オブジェクトを削除
clean:
	rm -f *.revas *.objdump *.nm *.hexdump *.lst *.o *~ 
	rm -f main main.s main.mot main.ps main.pdf main.log
	rm -f h8write.fast
	(cd libs;make clean)
