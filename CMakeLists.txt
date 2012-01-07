cmake_minimum_required (VERSION 2.6)
project (netlib)

set(source 
	   src/main.cpp 
	   src/packetDef.cpp
	   src/defRegister.cpp
	   src/net.cpp
	   src/connection.cpp
	   src/address.cpp
	   src/flowControl.cpp
	   src/packet.cpp
	   src/packetqueue.cpp
	   src/stats.cpp
	   src/renderer.cpp
	   src/socket.cpp
	   src/entity.cpp
	   src/netEntity.cpp
	   src/dataUtils.cpp
	   src/eventHandler.cpp
	   src/entityHandler.cpp
	   src/readPacket.cpp
	   src/writePacket.cpp
	   src/netManager.cpp
)
set(header
	 
	   src/header/packetDef.h
	   src/header/net.h
	   src/header/connection.h
	   src/header/address.h
	   src/header/flowControl.h
	   src/header/packet.h
	   src/header/packetqueue.h
	   src/header/stats.h
	   src/header/renderer.h
	   src/header/socket.h
	   src/header/entity.h
	   src/header/netEntity.h
	   src/header/dataUtils.h
	   src/header/eventHandler.h
	   src/header/entityHandler.h
	   src/header/netHandler.h
	   src/header/readPacket.h
	   src/header/writePacket.h
	   src/header/netManager.h
	   
)

find_package(SDL)
find_package(OpenGL)
find_package(GLUT)

link_libraries(${SDL_LIBRARY} ${OPENGL_LIBRARY} ${GLUT_LIBRARY})
add_executable(net ${source} ${header})


