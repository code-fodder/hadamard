# Tools and flags
CC = g++
CFLAGS = -Wall -Werror -std=c++11 -I.
AR = ar
AR_FLAGS = rcs

# Output file
OUT_DIR = out
OUT_FILE_BASE = hadamard
EXEC = $(OUT_FILE_BASE).out

# List of object files
OBJDIR = obj
_OBJ = $(OUT_FILE_BASE).o
OBJ = $(patsubst %,$(OBJDIR)/%,$(_OBJ))
# No header files yet - but put them here if/when there are
DEPS=$(OUT_FILE_BASE).h

.phony: shared static clean all create_dirs

$(OBJDIR)/%.o: %.cpp $(DEPS) create_dirs
	$(CC) -c -fPIC -o $@ $< $(CFLAGS)
	
$(OUT_DIR)/$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

create_dirs:
	mkdir -p $(OBJDIR)
	mkdir -p $(OUT_DIR)	

# Create a shared library (.so)
# Link using 'gcc -L. <codefile.cpp> -l<name> -o <execfilename>'
# -L. is a library path to the current folder
# -l<name> looks for 'lib<name>.a'
# Also here -fPIC means use Position Independant Code
shared: create_dirs
	$(CC) -fPIC -o $(OBJDIR)/$(OUT_FILE_BASE)-shared.o $(OUT_FILE_BASE).cpp $(CFLAGS) -c
	$(CC) -shared -Wl,-soname,lib$(OUT_FILE_BASE).so.1 -o $(OUT_DIR)/lib$(OUT_FILE_BASE).so.1.0 $(OBJDIR)/$(OUT_FILE_BASE)-shared.o
	ln -sf lib$(OUT_FILE_BASE).so.1.0 $(OUT_DIR)/lib$(OUT_FILE_BASE).so
	ln -sf lib$(OUT_FILE_BASE).so.1.0 $(OUT_DIR)/lib$(OUT_FILE_BASE).so.1

# Create a static library (.a)
# Link using 'gcc -static <codefile.cpp> -L. -l<name> -o <execfilename>'
# Basically the same as for shared but the -'static' must be used
# -L. is a library path to the current folder
# -l<name> looks for 'lib<name>.a'
static: create_dirs $(OBJ)
	$(AR) $(AR_FLAGS) $(OUT_DIR)/lib$(OUT_FILE_BASE).a $(OBJDIR)/$(OUT_FILE_BASE).o

clean:
	rm -fr $(OBJDIR) $(OUT_DIR)

all: $(OUT_DIR)/$(EXEC) shared static

# Installs the lib files in /usr/local/lib
# Installs the header files in /usr/local/include/<projname>
# Note: to include the project in your source use '#include <projname/headerfile.h>'
install:
	# Copy lib files - keeping the symbolic links (-P)
	cp -P $(OUT_DIR)/lib$(OUT_FILE_BASE)* /usr/local/lib
	# Copy the header to the includes dir
	mkdir -p /usr/local/include/$(OUT_FILE_BASE)
	cp $(OUT_FILE_BASE).h /usr/local/include/$(OUT_FILE_BASE)



