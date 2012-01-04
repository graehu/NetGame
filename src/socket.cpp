#include "header/socket.h"
using namespace net;

socket::socket()
{
	m_socket = 0;
}

socket::~socket()
{
	closeSock();
}

bool socket::openSock(unsigned short port)
{
	assert(!IsOpen());

	// create socket
	m_socket = ::socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
	if (m_socket <= 0)
	{
		#if PLATFORM == PLATFORM_WINDOWS
		int error = WSAGetLastError();
		printf( "failed to create socket, error(%i)\n", error );
		#else
		printf( "failed to create socket\n");
		#endif

		m_socket = 0;
		return false;
	}
	// bind to port
	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons((unsigned short)port);

	if (bind(m_socket, (const sockaddr*) &address, sizeof(sockaddr_in)) < 0)
	{
		printf( "failed to bind socket\n" );
		closeSock();
		return false;
	}
	// set non-blocking io
	#if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX

		int nonBlocking = 1;
		if ( fcntl(m_socket, F_SETFL, O_NONBLOCK, nonBlocking ) == -1 )
		{
			printf( "failed to set non-blocking socket\n" );
			closeSock();
			return false;
		}

	#elif PLATFORM == PLATFORM_WINDOWS

		DWORD nonBlocking = 1;
		if ( ioctlsocket( socket, FIONBIO, &nonBlocking ) != 0 )
		{
			printf( "failed to set non-blocking socket\n" );
			closeSock();
			return false;
		}
	#endif

	return true;
}

void socket::closeSock()
{
	if (m_socket != 0)
	{
		#if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
		close(m_socket);
		#elif PLATFORM == PLATFORM_WINDOWS
		closesocket(socket);
		#endif
		m_socket = 0;
	}
}

bool socket::IsOpen() const
{
	return m_socket != 0;
}

bool socket::send(const address & destination, const void * data, int size)
{
	assert(data);
	assert(size > 0);

	if (m_socket == 0)
		return false;

	assert(destination.getAddress() != 0);
	assert(destination.getPort() != 0);

	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(destination.getAddress());
	address.sin_port = htons((unsigned short) destination.getPort());
	int sent_bytes = sendto(m_socket, (const char*)data, size, 0, (sockaddr*)&address, sizeof(sockaddr_in));
	return sent_bytes == size;
}

int socket::receive(address & sender, void * data, int size)
{
	assert(data);
	assert(size > 0);

	if (m_socket == 0)
		return false;

	#if PLATFORM == PLATFORM_WINDOWS
		typedef int socklen_t;
	#endif

	sockaddr_in from;
	socklen_t fromLength = sizeof(from);
	int received_bytes = recvfrom(m_socket, (char*)data, size, 0, (sockaddr*)&from, &fromLength);
	if ( received_bytes <= 0 )
		return 0;

	unsigned int nAddress = ntohl(from.sin_addr.s_addr);
	unsigned short nPort = ntohs(from.sin_port);
	sender = address(nAddress, nPort);
	return received_bytes;
}

