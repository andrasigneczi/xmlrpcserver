MAKE=make
RM=rm
MKDIR=mkdir

# If no configuration is specified, "Debug" will be used
ifndef "CFG"
CFG=Debug
endif

#
# Configuration: Debug
#
ifeq "$(CFG)" "Debug"
OUTDIR=Debug
OUTFILE=$(OUTDIR)/XMLRPC_server
CFG_INC_C=
CFG_INC_CPP=-I./include
#CFG_INC_CPP=-I./include -DUSE_HEADER_FOR_RESPONSE
CFG_LIB=-lstdc++ -lm -pthread
CFG_OBJ=
COMMON_OBJ=$(OUTDIR)/main.o $(OUTDIR)/xml.o \
	$(OUTDIR)/httpheader.o $(OUTDIR)/charstream.o $(OUTDIR)/method.o \
	$(OUTDIR)/methodbuilder.o $(OUTDIR)/objserver.o $(OUTDIR)/rpcserver.o \
	$(OUTDIR)/rpcagent.o $(OUTDIR)/testserverobjects.o $(OUTDIR)/serversocket_lin.o \
	$(OUTDIR)/clientsocket_lin.o $(OUTDIR)/methodparameter.o
	
OBJ=$(COMMON_OBJ) $(CFG_OBJ)

COMPILEC=gcc -c  -Wall -g -o "$(OUTDIR)/$(*F).o" $(CFG_INC_C) "$<"
COMPILECPP=g++ -std=c++0x -Wall -g -c -pthread -o "$(OUTDIR)/$(*F).o" $(CFG_INC_CPP) "$<"
LINK=g++ -o "$(OUTFILE)" $(OBJ) $(CFG_LIB)

# Pattern rules

$(OUTDIR)/%.o : impl/%.cpp
	$(COMPILECPP)

$(OUTDIR)/%.o : impl/%.c
	$(COMPILEC)

# Build rules
all: prebuildcmds $(OUTFILE)

prebuildcmds:
	-mkdir -p Debug

$(OUTFILE): $(OUTDIR)  $(OBJ)
	$(LINK)

$(OUTDIR):
	$(MKDIR) -p "$(OUTDIR)"

# Rebuild this project
rebuild: cleanall all

# Clean this project
clean:
	$(RM) -f $(OUTFILE)
	$(RM) -f $(OBJ)

# Clean this project and all dependencies
cleanall: clean
endif

#
# Configuration: Release
#
ifeq "$(CFG)" "Release"
OUTDIR=Release
OUTFILE=$(OUTDIR)/XMLRPC_server
CFG_INC_C=
CFG_INC_CPP=-I./include
#CFG_INC_CPP=-I./include -DUSE_HEADER_FOR_RESPONSE
CFG_LIB=-lstdc++ -lm -pthread
CFG_OBJ=
COMMON_OBJ=$(OUTDIR)/main.o $(OUTDIR)/xml.o \
	$(OUTDIR)/httpheader.o $(OUTDIR)/charstream.o $(OUTDIR)/method.o \
	$(OUTDIR)/methodbuilder.o $(OUTDIR)/objserver.o $(OUTDIR)/rpcserver.o \
	$(OUTDIR)/rpcagent.o $(OUTDIR)/testserverobject.o $(OUTDIR)/serversocket_lin.o \
	$(OUTDIR)/clientsocket_lin.o
OBJ=$(COMMON_OBJ) $(CFG_OBJ)

COMPILEC=gcc -c  -Wall -g -o "$(OUTDIR)/$(*F).o" $(CFG_INC_C) "$<"
COMPILECPP=g++ -c -Wall -DNDEBUG  -O3 -std=c++0x -pthread -o "$(OUTDIR)/$(*F).o" $(CFG_INC_CPP) "$<"
LINK=g++ -O3 -o "$(OUTFILE)" $(OBJ) $(CFG_LIB)

# Pattern rules
$(OUTDIR)/%.o : impl/%.cpp
	$(COMPILECPP)

$(OUTDIR)/%.o : impl/%.c
	$(COMPILEC)

# Build rules
all: prebuildcmds $(OUTFILE)

prebuildcmds:
	-mkdir -p Release

$(OUTFILE): $(OUTDIR)  $(OBJ)
	$(LINK)
	strip --strip-unneeded "$(OUTFILE)"

$(OUTDIR):
	$(MKDIR) -p "$(OUTDIR)"

# Rebuild this project
rebuild: cleanall all

# Clean this project
clean:
	$(RM) -f $(OUTFILE)
	$(RM) -f $(OBJ)

# Clean this project and all dependencies
cleanall: clean
endif
