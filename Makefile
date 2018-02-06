CC	= gcc
CFLAGS	= -pthread
LFLAGS	= -lwiringPi

EXEC	= smartHouse

SRC_DIR	= src
OBJ_DIR	= build

all:	$(EXEC)

$(EXEC): $(OBJ_DIR)/sensorsManager.o $(OBJ_DIR)/actuators.o $(OBJ_DIR)/networkInterface.o $(OBJ_DIR)/main.o
	$(CC) $(CFLAGS) $^ -o $(EXEC) $(LFLAGS)

$(EXEC_OLD): $(OBJ_DIR)/sensorsManager.o $(OBJ_DIR)/actuators.o $(OBJ_DIR)/networkInterface.o $(OBJ_DIR)/lightsManager.o $(OBJ_DIR)/main.o
	$(CC) $(CFLAGS) $^ -o $(EXEC) $(LFLAGS)

$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c
	@mkdir -p $(OBJ_DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(SRC_DIR)/%.h
	@mkdir -p $(OBJ_DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

server: $(OBJ_DIR)/server.o
	$(CC) $(CFLAGS) $^ -o smartHouseServer

client: $(OBJ_DIR)/client.o
	$(CC) $(CFLAGS) $^ -o smartHouseClient

photoresistor:
	gcc -o photoresistor src/photoresistor.c -lwiringPi

soundsensor:
	gcc -o soundsensor src/sound_sensor.c -lwiringPi

pcf8591:
	gcc -o pcf8591 src/pcf8591.c -lwiringPi


.PHONY: clean, mrproper
clean:
	rm -rf $(OBJ_DIR) *~

mrproper: clean
	rm -rf $(EXEC)
	rm -rf smartHouseClient
	rm -rf smartHouseServer
	rm -rf photoresistor
	rm -rf soundsensor
	rm -rf pcf8591
