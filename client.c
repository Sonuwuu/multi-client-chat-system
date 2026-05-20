#include <stdio.h>
// For printf(), fgets()

#include <stdlib.h>
// For standard functions

#include <string.h>
// For strlen(), strcmp(), strstr(), strcspn()

#include <unistd.h>
// For read(), close()

#include <arpa/inet.h>
// For socket related functions

#include <sys/select.h>
// For select() and fd_set



#define PORT 8080
// Same port as server

#define BUFFER_SIZE 1024
// Maximum message size

#define NAME_LEN 50
// Maximum username size



// Terminal colors
#define RESET "\033[0m"

#define YELLOW "\033[1;33m"

#define MAGENTA "\033[0;35m"



int main() {

    int sock;
    // Client socket

    struct sockaddr_in serv_addr;
    // Stores server info



    char buffer[BUFFER_SIZE];
    // Incoming messages

    char name[NAME_LEN];
    // Username

    char server_ip[50];
    // Server IP entered by user



    // Ask user for server IP
    printf("Enter Server IP: ");

    fgets(server_ip, sizeof(server_ip), stdin);

    // Remove newline
    server_ip[strcspn(server_ip, "\n")] = '\0';



    // Create socket
    //
    // AF_INET     -> IPv4
    // SOCK_STREAM -> TCP
    sock = socket(AF_INET, SOCK_STREAM, 0);



    // Using IPv4
    serv_addr.sin_family = AF_INET;

    // Server port
    serv_addr.sin_port = htons(PORT);



    // Convert IP string into binary format
    inet_pton(AF_INET, server_ip, &serv_addr.sin_addr);



    // Connect to server
    if (connect(sock,
                (struct sockaddr*)&serv_addr,
                sizeof(serv_addr)) < 0) {

        printf("\nConnection Failed!\n");

        return 1;
    }



    // Ask username
    printf("Enter Name: ");

    fgets(name, NAME_LEN, stdin);

    // Remove newline
    name[strcspn(name, "\n")] = '\0';



    // Send username to server
    //
    // Server treats first message as username
    send(sock, name, strlen(name), 0);



    fd_set readfds;
    // Used by select()



    // Client loop
    while (1) {

        // Clear fd_set
        FD_ZERO(&readfds);

        // Add keyboard input
        FD_SET(0, &readfds);

        // Add socket
        FD_SET(sock, &readfds);



        // Wait for activity
        //
        // 1. User types message
        // 2. Server sends message
        select(sock + 1, &readfds, NULL, NULL, NULL);



        // USER INPUT
        if (FD_ISSET(0, &readfds)) {

            char msg[BUFFER_SIZE];



            // Read message
            fgets(msg, BUFFER_SIZE, stdin);

            // Remove newline
            msg[strcspn(msg, "\n")] = '\0';



            // Exit command
            if (strcmp(msg, "/exit") == 0)
                break;



            // Move cursor one line up
            //
            // Prevent duplicate typed line
            printf("\033[A");

            // Clear line
            printf("\r\033[K");

            fflush(stdout);



            // Send message to server
            send(sock, msg, strlen(msg), 0);
        }



        // RECEIVE MESSAGE
        if (FD_ISSET(sock, &readfds)) {

            // Read incoming message
            int valread = read(sock, buffer, BUFFER_SIZE - 1);



            // Server disconnected
            if (valread <= 0)
                break;



            // Convert bytes into proper string
            buffer[valread] = '\0';



            // PRIVATE MESSAGE
            if (strstr(buffer, "(PM)")) {

                char sender[NAME_LEN];



                // Extract sender name
                //
                // Example:
                // [16:30] sonu -> rahul: hello
                sscanf(buffer,
                       "[%*[^]]] (PM) %49s",
                       sender);



                // Remove trailing space/newline if any
                sender[strcspn(sender, " ")] = '\0';



                // If sender is current user
                if (strcmp(sender, name) == 0) {

                    char temp[BUFFER_SIZE];
                    char final_msg[BUFFER_SIZE];

                    strcpy(temp, buffer);

                    char *pos = strstr(temp, sender);

                    if (pos != NULL) {

                        int index = pos - temp;

                        strncpy(final_msg, temp, index);

                        final_msg[index] = '\0';

                        strcat(final_msg, "YOU");

                        strcat(final_msg,
                               pos + strlen(sender));

                        printf(MAGENTA "%s" RESET "\n",
                               final_msg);
                    }
                }

                // Normal PM from others
                else {

                    printf(MAGENTA "%s" RESET "\n",
                           buffer);
                }
            }



            // NORMAL MESSAGE
            else {

                char sender[NAME_LEN];



                // Extract sender name
                //
                // Example:
                // [16:30] sonu: hello
                if (sscanf(buffer,
                           "[%*[^]]] %49[^:]:",
                           sender) == 1) {

                    // Exact username match
                    if (strcmp(sender, name) == 0) {

                        char temp[BUFFER_SIZE];
                        char final_msg[BUFFER_SIZE];

                        strcpy(temp, buffer);

                        char *pos = strstr(temp, sender);

                        if (pos != NULL) {

                            int index = pos - temp;

                            strncpy(final_msg,
                                    temp,
                                    index);

                            final_msg[index] = '\0';

                            strcat(final_msg, "YOU");

                            strcat(final_msg,
                                   pos + strlen(sender));

                            printf(YELLOW "%s" RESET "\n",
                                   final_msg);
                        }
                    }

                    // Message from others
                    else {

                        printf("%s\n",
                               buffer);
                    }
                }

                else {

                    printf("%s\n",
                           buffer);
                }
            }
        }
    }



    // Close socket
    close(sock);

    return 0;
}
