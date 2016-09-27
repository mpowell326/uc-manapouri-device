CC = g++ # This is the main compiler (use g++ for C++)
SRCDIR = src_main
BUILDDIR = bin
TARGET = $(BUILDDIR)/nemo

SOURCES = $(shell find $(SRCDIR) -type f -name *.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.cpp=.o))
CFLAGS = -g -Wall
INC = -I ./ -I ./lib -I lib/wiringBone/library/ -I lib/wiringBone/ -I lib/Adafruit_Sensor2/ -I lib/Adafruit_BNO055-master/ -I lib/Lux_TSL2561_Sensor/src/

all: $(TARGET)
	@echo Program has been compiled

$(TARGET): $(OBJECTS) $(BUILDDIR)/BNO055.o $(BUILDDIR)/SparkFunTSL2561.o
	$(CC) $^ $(CFLAGS) $(INC) -o $(TARGET) lib/wiringBone/BUILD/wiringBone.so -lpthread

$(BUILDDIR)/lux.o: lib/Lux_TSL2561_Sensor/src/SparkFunTSL2561.cpp
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $(BUILDDIR)/SparkFunTSL2561.o $<

$(BUILDDIR)/BNO055.o: lib/Adafruit_BNO055-master/Adafruit_BNO055.cpp
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $(BUILDDIR)/BNO055.o $<

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " Cleaning..."; 
	@echo " $(RM) -r $(BUILDDIR)/*.o $(TARGET)"; $(RM) -r $(BUILDDIR)/*.o $(TARGET)

.PHONY: clean

