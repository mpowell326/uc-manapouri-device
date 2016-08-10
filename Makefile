CC = g++ # This is the main compiler (use g++ for C++)
SRCDIR = src_main
BUILDDIR = bin
TARGET = $(BUILDDIR)/nemo
 
SOURCES = $(shell find $(SRCDIR) -type f -name *.c)
OBJECTS = $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.c=.o))
CFLAGS = -g -Wall
INC = -I ./ -I lib/wiringBone/library/ -I lib/wiringBone/

all: $(TARGET)
	@echo Program has been compiled

$(TARGET): $(OBJECTS)
	$(CC) $^ -o lib/wiringBone/BUILD/wiringBone.so $(CFLAGS) $(INC) -o $(TARGET) 

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " Cleaning..."; 
	@echo " $(RM) -r $(BUILDDIR)/*.o $(TARGET)"; $(RM) -r $(BUILDDIR)/*.o $(TARGET)

.PHONY: clean
