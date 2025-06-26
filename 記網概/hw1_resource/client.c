#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT 80  // Define the port number (80 for HTTP)
#define BUFFER_SIZE 4096 

int main(int argc, char* argv[]) {
    int socket_fd;
    struct sockaddr_in server_addr;
    char request[] = "GET / HTTP/1.1\r\nHost: motherfuckingwebsite.com\r\n\r\n";

    // Create a TCP socket.
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);        
    //domain(AF_INET->IPv4), type(SOCK_STREAM->TCP), protocol(0->Automatically select the appropriate protocol)
    

    // Set up the server's address.
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    struct hostent* h = gethostbyname("motherfuckingwebsite.com");
    memcpy(&server_addr.sin_addr, h->h_addr_list[0], h->h_length);

    // Connect to the server.
    connect(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    // Send the HTTP GET request.
    send(socket_fd, request, strlen(request), 0);       //0: no extra flag

    // Receive the server's response.
    // You might use while loop to receive packets.
    

    int received_bytes; 
    //Store the actual received data length returned by recv()
    //Determine whether there is still data to be read (while (received_bytes > 0))
    //1.If data is received, recv() returns received_bytes.
    //2.If the connection is closed, recv() returns 0.
    //3.If an error occurs, recv() returns -1.

    char buffer[BUFFER_SIZE];   //stores the received HTTP response
    while ((received_bytes = recv(socket_fd, buffer, BUFFER_SIZE - 1, 0)) > 0) {
        

        buffer[received_bytes] = '\0';  //Make sure the string ends
        //printf("%s", buffer);     //Show full HTTP response

        // Print out the words with label <h2></h2>.
        char *start = strstr(buffer, "<h2>");   //Find the `<h2>` tag
        while (start) {
            start += 4;     // Skip `<h2>`
            char *end = strstr(start, "</h2>"); 
            if (end) {
                *end = '\0';    // // Change the `<` in `</h2>` to `\0` to make it a legal end of the string
                printf("%s\n", start);
                start = strstr(end + 5, "<h2>");    //continue searching
            } else {
                break;
            }
        }
    }

    // Close the socket.
    close(socket_fd);

    return 0;
}
