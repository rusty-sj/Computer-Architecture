# Script adapted from https://hiltmon.com/blog/2013/07/03/a-simple-c-plus-plus-project-structure/
CC := /usr/local/classes/eecs/spring2021/cs472/public/gcc/bin/g++
STRIPUTIL = strip
SRCDIR = src
BUILDDIR = build
TARGET = bin/cache_sim

# Handle debug case
DEBUG ?= 1
ifeq ($(DEBUG), 1)
	CFLAGS += -g -Wall
else
	CFLAGS += -DNDEBUG -O3
endif
 
SRCEXT = cpp
SOURCES = $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS = $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
LDFLAGS += -Wl,-rpath,/usr/local/classes/eecs/spring2021/cs472/public/gcc/lib64
LIB += -pthread
INC += -I $(SRCDIR)

$(TARGET): $(OBJECTS)
	@echo "Linking..."
	@echo " $(CC) $^ -o $(TARGET) $(LIB) $(LDFLAGS)"; $(CC) $^ -o $(TARGET) $(LIB) $(LDFLAGS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo "Cleaning..."; 
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)

.PHONY: clean
