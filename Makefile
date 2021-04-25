DIR_Config   = ./src/lib/Config
DIR_EPD      = ./src/lib/e-Paper
DIR_GUI      = ./src/lib/GUI
DIR_BMP      = ./src/lib/easybmp
DIR_OUTPUT   = ./output
DIR_OBJ      = ./output/object
DIR_BIN      = ./output/bin
DIR_WP43S    = ./src/wp43s
DIR_QT_INC   = /usr/include/qt4

OBJ_C = $(wildcard ${DIR_BMP}/*.cpp ${DIR_EPD}/*.cpp ${DIR_GUI}/*.cpp ${DIR_WP43S}/*.cpp )
OBJ_O = $(patsubst %.cpp,${DIR_OBJ}/%.o,$(notdir ${OBJ_C}))
RPI_DEV_C = $(wildcard $(DIR_OBJ)/dev_hardware_SPI.o $(DIR_OBJ)/RPI_sysfs_gpio.o $(DIR_OBJ)/DEV_Config.o )

DEBUG = -D DEBUG

# USELIB_RPI = USE_BCM2835_LIB
USELIB_RPI = USE_WIRINGPI_LIB
# USELIB_RPI = USE_DEV_LIB

LIB_RPI=-Wl,--gc-sections  -L/usr/lib/arm-linux-gnueabihf -lX11 -lXext -lXi
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
CC            = gcc
CXX           = g++
MSG = -std=c++14 -g -O -ffunction-sections -fdata-sections -Wall -lstdc++ -lpthread
CXXFLAGS += $(MSG)
OPENCV_FLAG = -I/usr/local/include/opencv4 -L/usr/local/lib -lopencv_aruco -lopencv_bgsegm -lopencv_bioinspired -lopencv_calib3d -lopencv_ccalib -lopencv_core -lopencv_datasets -lopencv_dnn_objdetect -lopencv_dnn -lopencv_dpm -lopencv_face -lopencv_features2d -lopencv_flann -lopencv_freetype -lopencv_fuzzy -lopencv_gapi -lopencv_hfs -lopencv_highgui -lopencv_imgcodecs -lopencv_img_hash -lopencv_imgproc -lopencv_line_descriptor -lopencv_ml -lopencv_objdetect -lopencv_optflow -lopencv_phase_unwrapping -lopencv_photo -lopencv_plot -lopencv_quality -lopencv_reg -lopencv_rgbd -lopencv_saliency -lopencv_shape -lopencv_stereo -lopencv_stitching -lopencv_structured_light -lopencv_superres -lopencv_surface_matching -lopencv_text -lopencv_tracking -lopencv_videoio -lopencv_video -lopencv_videostab -lopencv_xfeatures2d -lopencv_ximgproc -lopencv_xobjdetect -lopencv_xphoto

RPI_epd:${OBJ_O}
	echo $(@)
	$(CXX) $(CXXFLAGS) $(OPENCV_FLAG) -D RPI $(OBJ_O) $(RPI_DEV_C) -o $(TARGET) $(LIB_RPI) $(DEBUG) -lstdc++fs


$(shell mkdir -p $(DIR_OBJ))
$(shell mkdir -p $(DIR_BIN))


${DIR_OBJ}/%.o:$(DIR_WP43S)/%.cpp
	$(CXX) $(CXXFLAGS) -c  $< -o $@ $(OPENCV_FLAG) -I $(DIR_Config) -I $(DIR_GUI) -I $(DIR_EPD) -I $(DIR_BMP) $(DEBUG)
    
${DIR_OBJ}/%.o:$(DIR_EPD)/%.cpp
	$(CXX) $(CXXFLAGS) -c  $< -o $@ -I $(DIR_Config) $(DEBUG)
    
${DIR_OBJ}/%.o:$(DIR_GUI)/%.cpp
	$(CXX) $(CXXFLAGS) -c  $< -o $@ -I $(DIR_Config) $(DEBUG)

${DIR_OBJ}/%.o:${DIR_BMP}/%.cpp
	$(CXX) $(CXXFLAGS) -O3 -pipe -fomit-frame-pointer -funroll-all-loops -s -c  $< -o $@ -I $(DIR_Config) $(DEBUG)

RPI_DEV:
	$(CXX) $(CXXFLAGS) $(DEBUG_RPI) -c  $(DIR_Config)/dev_hardware_SPI.cpp -o $(DIR_OBJ)/dev_hardware_SPI.o $(LIB_RPI) $(DEBUG)
	$(CXX) $(CXXFLAGS) $(DEBUG_RPI) -c  $(DIR_Config)/RPI_sysfs_gpio.cpp -o $(DIR_OBJ)/RPI_sysfs_gpio.o $(LIB_RPI) $(DEBUG)
	$(CXX) $(CXXFLAGS) $(DEBUG_RPI) -c  $(DIR_Config)/DEV_Config.cpp -o $(DIR_OBJ)/DEV_Config.o $(LIB_RPI) $(DEBUG)

clean :
	rm -f $(DIR_OUTPUT)/*.*
	rm -f $(DIR_OBJ)/*.o
	rm -f $(TARGET)

