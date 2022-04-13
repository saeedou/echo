/**
  APPLICATION START
  Create a listen socket.
  Bind a listen socket.
  Wait for new packet.

  NEW CONNECTION
  Allocate a new connection struct
  Store IP address and source PORT
  Set connection buffer size to zero
  Add connection struct to global connection list

  NEW PACKET
  Read the packet.
  If error remove the connection struct from the global list and return.
  Save the new packet in the connection struct.

  EXCETION
  Remove the connection struct from the global list.

  READY TO WRITE
  Write from connection struct buffer into socket.
  If error remove the connection struct from the global list and return.
  Set connection buffer size to zero

  SELECT
  Loop over global connection list.
  Register listen fd for read.
  If connection buffer size > 0 then register it for write.
  Else register the file for read.
  Finally register the file for exception.
  Call select funcion.
  Loop over readfds and accept new connection or read from each socket.
  Loop over writefds and write in the connection struct buffer.
  */

#include <sys/socket.h>
#include <netinet/in.h>

#define BUFF_SIZE 1024

struct conn {
    int fd;
    struct sockaddr addr;
    char buffer[BUFF_SIZE];
    unsigned int bufflen;
};

int main() {
    return 0;
}
