cmake_minimum_required (VERSION 2.6)
project (net)

set(source 
	   src/main.cpp 
	   src/packetDef.cpp
	   src/net.cpp
	   src/connection.cpp
	   src/address.cpp
	   src/flowControl.cpp
	   src/entity.cpp
	   src/flowControl.cpp
	   src/packet.cpp
	   src/packetqueue.cpp
	   src/reliabilitySystem.cpp
	   src/renderer.cpp
	   src/socket.cpp
)
set(header
	 
	   src/header/packetDef.h
	   src/header/net.h
	   src/header/connection.h
	   src/header/address.h
	   src/header/flowControl.h
	   src/header/entity.hpp
	   src/header/flowControl.h
	   src/header/packet.h
	   src/header/packetqueue.h
	   src/header/reliabilitySystem.h
	   src/header/renderer.h
	   src/header/socket.h
)

find_package(SDL)
find_package(OpenGL)
find_package(GLUT)

link_libraries(${SDL_LIBRARY} ${OPENGL_LIBRARY} ${GLUT_LIBRARY})
add_executable(net ${source} ${header})

