COPTS = \
		-Wall $(MOREDEF) -mtune=native -O3 \
		-std=c++11
		
LDOPTS = \
		-lm \
		-pthread \
		-lthrift \
		-lcryptopp \
		-lstdc++

OBJECTS = 

OBJECTSIDL = \
		src/gen-cpp/BallotBox.o \
		src/gen-cpp/RegOffice.o \
		src/gen-cpp/SimpleEvote_constants.o \
		src/gen-cpp/SimpleEvote_types.o

PROGRAM = \
		rOffice \
		bBox \
		voter \
		schoenenborn

# 4. create programms
all: $(PROGRAM)


gen-idl:
	thrift --gen cpp src/SimpleEvote.thrift
	mv gen-cpp/* src/gen-cpp/
	rm -rf gen-cpp

# 3. create thrift code and objects
idl: gen-idl $(OBJECTSIDL)

schoenenborn: \
	$(OBJECTS) \
	$(OBJECTSIDL) \
	src/Tools.hpp \
	schoenenborn.cpp
		g++ $(COPTS) $(OBJECTS) $(OBJECTSIDL) \
		  schoenenborn.cpp -o schoenenborn $(LDOPTS)

voter: \
	$(OBJECTS) \
	$(OBJECTSIDL) \
	src/Tools.hpp \
	voter.cpp
		g++ $(COPTS) $(OBJECTS) $(OBJECTSIDL) \
		  voter.cpp -o voter $(LDOPTS)

bBox: \
	$(OBJECTS) \
	$(OBJECTSIDL) \
	src/Tools.hpp \
	bBox.cpp
		g++ $(COPTS) $(OBJECTS) $(OBJECTSIDL) \
		  bBox.cpp -o bBox $(LDOPTS)

rOffice: \
	$(OBJECTS) \
	$(OBJECTSIDL) \
	src/Tools.hpp \
	rOffice.cpp
		g++ $(COPTS) $(OBJECTS) $(OBJECTSIDL) \
		  rOffice.cpp -o rOffice $(LDOPTS)

%.o: %.cpp
	g++ $(COPTS) -o $@ -c $*.cpp

# 1. clean thrift objects and generated code
clean-idl:
	rm -f src/gen-cpp/*.o

# 2. clean programm objects and programms
clean:
	rm -f src/gen-cpp/*.o
	rm -f src/*.o
	rm -f *.o
	rm -f $(PROGRAM)
