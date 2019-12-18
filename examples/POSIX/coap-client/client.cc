#include "client.h"
#include "clientConnection.h"

int main(int argc, char ** argv)
{
	try {

	    clientConnection conn("127.0.0.1", 5684);
	    conn.fill_buffer("HELLO SERVER", sizeof("HELLO SERVER"));
	    conn.send();
	    conn.receive();  
	    std::cout << conn.get_buffer() << std::endl;
	    conn.fill_buffer("ACK", sizeof("ACK") - sizeof(char));
	    conn.send();
	}
	catch(...) {
		std::cerr << "There was caught some exception" << std::endl;
		exit(EXIT_FAILURE);
	}

    exit(EXIT_SUCCESS);
}