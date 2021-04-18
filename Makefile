DIR_Config   = ./src/lib/Config
DIR_EPD      = ./src/lib/e-Paper
DIR_GUI      = ./src/lib/GUI
DIR_OUTPUT   = ./output
DIR_OBJ      = ./output/object
DIR_BIN      = ./output/bin
DIR_WP43S    = ./src/wp43s

OBJ_C = $(wildcard ${DIR_EPD}/*.cpp ${DIR_GUI}/*.cpp ${DIR_WP43S}/*.cpp )
OBJ_O = $(patsubst %.cpp,${DIR_OBJ}/%.o,$(notdir ${OBJ_C}))
RPI_DEV_C = $(wildcard $(DIR_OBJ)/dev_hardware_SPI.o $(DIR_OBJ)/RPI_sysfs_gpio.o $(DIR_OBJ)/DEV_Config.o )

DEBUG = -D DEBUG

# USELIB_RPI = USE_BCM2835_LIB
USELIB_RPI = USE_WIRINGPI_LIB
# USELIB_RPI = USE_DEV_LIB

LIB_RPI=-Wl,--gc-sections
ifeq ($(USELIB_RPI), USE_BCM2835_LIB)
    LIB_RPI += -lbcm2835 -lm 
else ifeq ($(USELIB_RPI), USE_WIRINGPI_LIB)
    LIB_RPI += -lwiringPi -lm 
else ifeq ($(USELIB_RPI), USE_DEV_LIB)
    LIB_RPI += -lm 
endif
DEBUG_RPI = -D $(USELIB_RPI) -D RPI

.PHONY : RPI clean

RPI:RPI_DEV RPI_epd

TARGET = output/bin/epd
CC = g++
MSG = -g -O -ffunction-sections -fdata-sections -Wall
CFLAGS += $(MSG)

RPI_epd:${OBJ_O}
	echo $(@)
	$(CC) $(CFLAGS) -D RPI $(OBJ_O) $(RPI_DEV_C) -o $(TARGET) $(LIB_RPI) $(DEBUG)


$(shell mkdir -p $(DIR_OBJ))
$(shell mkdir -p $(DIR_BIN))

${DIR_OBJ}/%.o:$(DIR_WP43S)/%.cpp
	$(CC) $(CFLAGS) -c  $< -o $@ -I $(DIR_Config) -I $(DIR_GUI) -I $(DIR_EPD) $(DEBUG)
    
${DIR_OBJ}/%.o:$(DIR_EPD)/%.cpp
	$(CC) $(CFLAGS) -c  $< -o $@ -I $(DIR_Config) $(DEBUG)
    
${DIR_OBJ}/%.o:$(DIR_GUI)/%.cpp
	$(CC) $(CFLAGS) -c  $< -o $@ -I $(DIR_Config) $(DEBUG)

RPI_DEV:
	$(CC) $(CFLAGS) $(DEBUG_RPI) -c  $(DIR_Config)/dev_hardware_SPI.cpp -o $(DIR_OBJ)/dev_hardware_SPI.o $(LIB_RPI) $(DEBUG)
	$(CC) $(CFLAGS) $(DEBUG_RPI) -c  $(DIR_Config)/RPI_sysfs_gpio.cpp -o $(DIR_OBJ)/RPI_sysfs_gpio.o $(LIB_RPI) $(DEBUG)
	$(CC) $(CFLAGS) $(DEBUG_RPI) -c  $(DIR_Config)/DEV_Config.cpp -o $(DIR_OBJ)/DEV_Config.o $(LIB_RPI) $(DEBUG)

clean :
	rm -f $(DIR_OUTPUT/*.*)
	rm -f $(DIR_OBJ/*.*)
	rm -f $(TARGET) 

