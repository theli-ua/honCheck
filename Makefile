BUILD		:=	build
SOURCES		:=	src
INCLUDES	:=	inc
OBJ 		:=	obj

TARGET 		:= 	honCheck
OMPTARGET 	:= 	honCheck-openmp


CFLAGS		:=	-I$(INCLUDES) $(shell xml2-config --cflags) -O3 -DGL_GLEXT_PROTOTYPES
CFLAGS		+= $(shell pkg-config minizip --cflags)
CXXFLAGS	:=	$(CFLAGS)

DOXYGEN		:=	doxygen
ECHO		:=	echo

LDFLAGS		:= 
LIBS		:=	$(shell xml2-config --libs) $(shell pkg-config --libs minizip)
LIBS		+=	-lGL

CFILES		:=	$(wildcard $(SOURCES)/*.c)
CPPFILES	:=	$(wildcard $(SOURCES)/*.cpp)
HFILES		:=	$(wildcard $(INCLUDES)/*.h)

OFILES		:=	$(subst $(SOURCES),$(OBJ),$(CPPFILES:.cpp=.o)) $(subst $(SOURCES),$(OBJ),$(CFILES:.c=.o))
OMPFILES	:=	$(subst $(SOURCES),$(OBJ),$(CPPFILES:.cpp=.o.mp)) $(subst $(SOURCES),$(OBJ),$(CFILES:.c=.o.mp))

all: $(BUILD)/$(TARGET) $(BUILD)/$(OMPTARGET)
#---------------------------------------------------------------------------------
$(OBJ)/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ 	
#---------------------------------------------------------------------------------
$(OBJ)/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

#---------------------------------------------------------------------------------
$(OBJ)/%.o.mp: src/%.cpp
	$(CXX) $(CXXFLAGS) -fopenmp -c $< -o $@ 	
#---------------------------------------------------------------------------------
$(OBJ)/%.mp.o: src/%.c
	$(CC) $(CFLAGS) -fopenmp -c $< -o $@

$(BUILD)/$(TARGET): $(OFILES)
	$(CXX) $(CXXFLAGS) -o $@ $(OFILES) $(LDFLAGS) $(LIBS)
$(BUILD)/$(OMPTARGET): $(OMPFILES)
	$(CXX) $(CXXFLAGS) -fopenmp -o $@ $(OMPFILES) $(LDFLAGS) $(LIBS)

clean:
	rm -f $(OFILES) $(BUILD)/$(TARGET) $(OMPFILES) $(BUILD)/$(OMPTARGET)

doc/html/index.html: Doxyfile $(CFILES) $(CPPFILES) $(HFILES)
	$(ECHO) "generating documentation, it may take a time ..."
	$(DOXYGEN) -u $^
	$(DOXYGEN) $^
	$(ECHO) "-> html documentation in \`doc/html'"

doc: doc/html/index.html

loc: $(CFILES) $(CPPFILES) $(HFILES)
	cloc $(INCLUDES) $(SOURCES) --xsl loc.xsl --out doc/lines_of_code.xml --exclude-list-file=reused_code
