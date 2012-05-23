BUILD		:=	build
SOURCES		:=	src
INCLUDES	:=	inc
OBJ 		:=	obj

TARGET 		:= 	honCheck


CFLAGS		:=	-I$(INCLUDES) $(shell xml2-config --cflags)
CXXFLAGS	:=	$(CFLAGS)

DOXYGEN		:=	doxygen
ECHO		:=	echo

LDFLAGS		:= 
LIBS		:=	$(shell xml2-config --libs)

CFILES		:=	$(wildcard $(SOURCES)/*.c)
CPPFILES	:=	$(wildcard $(SOURCES)/*.cpp)
HFILES		:=	$(wildcard $(INCLUDES)/*.h)

OFILES		:=	$(subst $(SOURCES),$(OBJ),$(CPPFILES:.cpp=.o)) $(subst $(SOURCES),$(OBJ),$(CFILES:.c=.o))


#---------------------------------------------------------------------------------
$(OBJ)/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ 	
#---------------------------------------------------------------------------------
$(OBJ)/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@


$(BUILD)/$(TARGET): $(OFILES)
	$(CXX) $(CXXFLAGS) -o $@ $(OFILES) $(LDFLAGS) $(LIBS)

clean:
	rm -f $(OFILES) $(BUILD)/$(TARGET)

doc: Doxyfile $(CFILES) $(CPPFILES) $(HFILES)
	$(ECHO) "generating documentation, it may take a time ..."
	$(DOXYGEN) -u $^
	$(DOXYGEN) $^
	$(ECHO) "-> html documentation in \`doc/html'"

loc: $(CFILES) $(CPPFILES) $(HFILES)
	cloc $(INCLUDES) $(SOURCES) --xsl loc.xsl --out doc/lines_of_code.xml
