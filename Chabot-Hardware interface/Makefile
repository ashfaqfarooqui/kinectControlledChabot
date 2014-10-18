PROJECT = libchabot
CFLAGS = -gdwarf-2 -Wall -g -I/opt/unixroot/usr/local/include
OBJS = Runner.o Message.o SerialManager.o

$(PROJECT): $(OBJS)
	$(AR) cr $@.a $^
	rm -rf ./dist
	mkdir -p ./dist/include
	mkdir -p ./dist/lib
	cp Runner.h SerialManager.h Message.h dist/include/
	cp $@.a dist/lib/

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)
	
%.o: %.cpp
	$(CXX) -c -o $@ $< $(CFLAGS)

default: $(TARGETS)

clean:
	rm *.o $(PROJECT).a
	rm -rf ./dist
