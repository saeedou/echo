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
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define BUFF_SIZE 1024
#define LISTEN_BACKLOG 4
#define PORT 8080
#define CONN_NUM 10

struct conn {
    int fd;
    struct sockaddr addr;
    char buffer[BUFF_SIZE];
    unsigned int bufflen; // TODO: what is the purpose and what is the difference with BUFF_SIZE?
};

void check_error(int , char *);
void write_packet(struct conn connection);
void read_packet(struct conn connection, struct conn connection_list *);

int main() {
    int i; // An index for loops
    int listen_socket;
    struct sockaddr_in server_addr;
    struct conn conn_list[CONN_NUM];
    struct conn no_connection;

    // Create a listen socket.
    check_error(listen_socket = socket(AF_INET, SOCK_STREAM, 0),
            "Socket creation failed");

    // Bind a listen socket.
    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    check_error(bind(listen_socket, (struct sockaddr *) &server_addr,
                sizeof(struct sockaddr)), "Binding failed");

    // Wait for new packet.
    check_error(listen(listen_socket, LISTEN_BACKLOG), "Listening failed");

    // Allocate a new connection struct
    struct conn client_conn;

    //  Store IP address and source PORT
    int client_addr_len = sizeof(client_conn.addr);
    check_error(client_conn.fd = accept(listen_socket, &client_conn.addr,
                &client_addr_len), "Accept failed");

    //  Set connection buffer size to zero TODO: size of the buffer or the buffer itself?
    memset(&client_conn.buffer, 0, BUFF_SIZE);

    //  Add connection struct to global connection list
    no_connection.fd = -1;
    for (i = 0; i < 10; i++) {
        conn_list[i] = no_connection;
    }

    // Read the packet.
    // If error remove the connection struct from the global list and return.
    // Save the new packet in the connection struct.
    read_packet(client_conn, &conn_list);

    //client_conn.bufflen = sizeof(client_conn.buffer);
    //if (read(client_conn.fd, &client_conn.buffer, client_conn.bufflen) < 0) {
    //    perror("Read failed");
    //    for (i=0;i<CONN_NUM;i++) {
    //        if (conn_list[i].fd == -1) {
    //            conn_list[i] = client_conn;
    //            break;
    //        }
    //    }
    //}

  // Write from connection struct buffer into socket.
  // If error remove the connection struct from the global list and return.
  // Set connection buffer size to zero
    write_packet(client_conn);
    return 0;
}

void check_error(int return_value, char message[]) {
    if (return_value < 0) {
        perror(message);
    }
}

void write_packet(struct conn connection) {
    if (write(connection.fd, &connection.buffer, connection.bufflen) < 0) {
            perror("Write failed");
        } else {
            memset(&connection.buffer, 0, BUFF_SIZE);
        }
}
void read_packet(struct conn connection, struct conn connection_list[]){
    connection.bufflen = sizeof(connection.buffer);
    if (read(connection.fd, &connection.buffer, connection.bufflen) < 0) {
        perror("Read failed");
        for (int i = 0; i < CONN_NUM ; i++) {
            if (connection_list[i].fd == -1) {
                connection_list[i] = connection;
                break;
            }
        }
    }
}
