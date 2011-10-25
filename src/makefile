target = net
libs = -lGL -lSDL -lGLU
compiler = gcc
sources = address.cpp\
	flowControl.cpp\
	main.cpp\
	packetDef.cpp\
	renderer.cpp\
	connection.cpp\
	game.cpp\
        net.cpp\
	packetqueue.cpp\
	socket.cpp\
	entity.cpp\
	packet.cpp\
	reliabilitySystem.cpp

objects=$(sources:.cpp=.o)

$(target):$(objects)
	$(compiler) -o $(target) $(objects) $(libs)

$(objects):
	$(compiler) -c $(sources) $(libs)

clean:
	rm *.o && rm $(target)
