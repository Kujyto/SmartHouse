CC	= gcc
CFLAGS	= -pthread
LFLAGS	= -lwiringPi

EXEC	= smartHouse

SRC_DIR	= src
OBJ_DIR	= build

all:	$(EXEC)

$(EXEC): $(OBJ_DIR)/sensorsManager.o $(OBJ_DIR)/actuators.o $(OBJ_DIR)/networkInterface.o $(OBJ_DIR)/main.o
	$(CC) $(CFLAGS) $^ -o $(EXEC) $(LFLAGS)

$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c
	@mkdir -p $(OBJ_DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(SRC_DIR)/%.h
	@mkdir -p $(OBJ_DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

server: $(OBJ_DIR)/server.o
	$(CC) $(CFLAGS) $^ -o smartHouseServer

.PHONY: clean, mrproper
clean:
	rm -rf $(OBJ_DIR) *~

mrproper: clean
	rm -rf $(EXEC)
	rm -rf smartHouseServer
