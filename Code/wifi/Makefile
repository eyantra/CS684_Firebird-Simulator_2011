OBJ_DIR = ./objs
STAGE_DIR=../stage

all: map.so client
map.so: acc.o
	gcc -shared $(OBJ_DIR)/acc.o -o $(STAGE_DIR)/map.so

acc.o: acc.cc stage.hh
	gcc -c acc.cc -o $(OBJ_DIR)/acc.o

client: client.c client.h
	gcc client.c -o client

clean:
	rm $(OBJ_DIR)/*.o
	rm $(STAGE_DIR)/map.so
	rm client
run-sim:
	make map.so
	stage $(STAGE_DIR)/ACC.world
run-cli:
	make client
	chmod a+x client
	xterm "/mnt/win_e/mtp/stage/bharat/Stage-3.2.2-Source/worlds/ex/acc/wifi"
run-tcli:
	make client
	chmod a+x client
	./client &
run:
	make
	chmod a+x client
	xterm "/mnt/win_e/mtp/stage/bharat/Stage-3.2.2-Source/worlds/ex/acc/wifi" &
	#./client &
	stage $(STAGE_DIR)/ACC.world
