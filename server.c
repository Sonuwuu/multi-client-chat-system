#include <stdio.h>      
// For printf(), snprintf()

#include <stdlib.h>     
// For setenv()

#include <string.h>     
// For strlen(), strcmp(), strncpy(), memset()

#include <unistd.h>     
// For read(), close(), gethostname()

#include <arpa/inet.h>  
// For sockaddr_in, htons(), inet_ntoa()

#include <sys/socket.h> 
// For socket functions

#include <sys/select.h> 
// For select() and fd_set

#include <time.h>       
// For timestamps

#include <netdb.h>      
// For gethostbyname()



// Server port
#define PORT 8080

// Maximum clients allowed
#define MAX_CLIENTS 10

// Maximum message size
#define BUFFER_SIZE 1024

// Maximum username size
#define NAME_LEN 50

// Extra message size
#define MSG_SIZE 1200



// Function to get current time
char *get_time() {

    static char time_str[10];

    time_t now = time(NULL);

    struct tm *t = localtime(&now);

    snprintf(time_str,
             sizeof(time_str),
             "%02d:%02d",
             t->tm_hour,
             t->tm_min);

    return time_str;
}



// Find username
int find_user(char names[MAX_CLIENTS][NAME_LEN],
              char *target) {

    for (int i = 0; i < MAX_CLIENTS; i++) {

        if (strcmp(names[i], target) == 0)
            return i;
    }

    return -1;
}



int main() {

    // Set Indian timezone
    setenv("TZ", "Asia/Kolkata", 1);
    tzset();



    // =================================================
    // AUTOMATIC IP DETECTION
    // =================================================

    char hostbuffer[256];

    struct hostent *host_entry;

    char *IPbuffer;



    // Get hostname
    gethostname(hostbuffer,
                sizeof(hostbuffer));



    // Get host information
    host_entry = gethostbyname(hostbuffer);



    // Convert into readable IP
    IPbuffer = inet_ntoa(*((struct in_addr*)
                           host_entry->h_addr_list[0]));




    // Main server socket
    int server_fd;

    // Stores client sockets
    int client_socket[MAX_CLIENTS];

    // Stores usernames
    char client_name[MAX_CLIENTS][NAME_LEN] = {0};



    // Structure storing IP + Port
    struct sockaddr_in address;

    // Used by select()
    fd_set readfds;

    // Address size
    int addrlen = sizeof(address);

    // Message buffer
    char buffer[BUFFER_SIZE];



    // Initially no clients connected
    for (int i = 0; i < MAX_CLIENTS; i++)
        client_socket[i] = 0;



    // CREATE SOCKET
    server_fd = socket(AF_INET,
                       SOCK_STREAM,
                       0);



    // Using IPv4
    address.sin_family = AF_INET;

    // Accept connections from any IP
    address.sin_addr.s_addr = INADDR_ANY;

    // Set port number
    address.sin_port = htons(PORT);



    // Bind socket with IP + Port
    bind(server_fd,
         (struct sockaddr *)&address,
         sizeof(address));



    // Start listening
    listen(server_fd, 5);



    // Show server IP
    printf(" Chat Server Started Successfully\n");

    printf("Server IP   : %s\n",
           IPbuffer);

    printf("Server Port : %d\n\n",
           PORT);

    printf("Waiting for clients...\n\n");



    // Server loop
    while (1) {

        // Clear fd_set
        FD_ZERO(&readfds);

        // Add server socket
        FD_SET(server_fd, &readfds);

        // Highest descriptor
        int max_sd = server_fd;



        // Add client sockets
        for (int i = 0; i < MAX_CLIENTS; i++) {

            int sd = client_socket[i];

            if (sd > 0)
                FD_SET(sd, &readfds);

            if (sd > max_sd)
                max_sd = sd;
        }



        // Wait for activity
        select(max_sd + 1,
               &readfds,
               NULL,
               NULL,
               NULL);

        // NEW CLIENT CONNECTION

        if (FD_ISSET(server_fd, &readfds)) {

            int new_socket = accept(server_fd,
                                    (struct sockaddr*)&address,
                                    (socklen_t*)&addrlen);



            // Store client socket
            for (int i = 0; i < MAX_CLIENTS; i++) {

                if (client_socket[i] == 0) {

                    client_socket[i] = new_socket;
                    break;
                }
            }
        }


        // HANDLE CLIENTS


        for (int i = 0; i < MAX_CLIENTS; i++) {

            int sd = client_socket[i];

            if (sd > 0 && FD_ISSET(sd, &readfds)) {

                int valread = read(sd,
                                   buffer,
                                   BUFFER_SIZE - 1);



                // CLIENT DISCONNECTED
                if (valread <= 0) {

                    if (strlen(client_name[i]) > 0) {

                        char msg[MSG_SIZE];

                        snprintf(msg,
                                 MSG_SIZE,
                                 "[%s] [INFO] %s disconnected\n",
                                 get_time(),
                                 client_name[i]);



                        // Show on server
                        printf("%s",
                               msg);



                        // Send to all clients
                        for (int j = 0; j < MAX_CLIENTS; j++) {

                            if (client_socket[j] != 0)
                                send(client_socket[j],
                                     msg,
                                     strlen(msg),
                                     0);
                        }
                    }



                    close(sd);

                    client_socket[i] = 0;

                    memset(client_name[i],
                           0,
                           NAME_LEN);

                    continue;
                }



                // Convert into proper string
                buffer[valread] = '\0';



                // FIRST MESSAGE = USERNAME
                if (strlen(client_name[i]) == 0) {

                    strncpy(client_name[i],
                            buffer,
                            NAME_LEN - 1);

                    client_name[i][NAME_LEN - 1] = '\0';

                    char msg[MSG_SIZE];

                    snprintf(msg,
                             MSG_SIZE,
                             "[%s] [INFO] %s joined the chat\n",
                             get_time(),
                             client_name[i]);



                    // Show on server
                    printf("%s",
                           msg);



                    // Send to all clients
                    for (int j = 0; j < MAX_CLIENTS; j++) {

                        if (client_socket[j] != 0)
                            send(client_socket[j],
                                 msg,
                                 strlen(msg),
                                 0);
                    }

                    continue;
                }


                // PRIVATE MESSAGE
                if (strncmp(buffer, "/msg ", 5) == 0) {

                    char target[NAME_LEN];
                    char text[BUFFER_SIZE];



                    // Extract username + message
                    sscanf(buffer + 5,
                           "%s %[^\n]",
                           target,
                           text);



                    // Find receiver
                    int idx = find_user(client_name,
                                        target);



                    // User exists
                    if (idx != -1 &&
                        client_socket[idx] != 0) {

                        char msg[MSG_SIZE];

                        snprintf(msg,
                                 MSG_SIZE,
                                 "[%s] (PM) %s -> %s: %s\n",
                                 get_time(),
                                 client_name[i],
                                 target,
                                 text);



                        // Send to receiver
                        send(client_socket[idx],
                             msg,
                             strlen(msg),
                             0);

                        // Send copy back to sender
                        send(sd,
                             msg,
                             strlen(msg),
                             0);
                    }
                }


                // NORMAL MESSAGE

                else {

                    char msg[MSG_SIZE];

                    snprintf(msg,
                             MSG_SIZE,
                             "[%s] %s: %s\n",
                             get_time(),
                             client_name[i],
                             buffer);



                    // Broadcast to all users
                    for (int j = 0; j < MAX_CLIENTS; j++) {

                        if (client_socket[j] != 0)
                            send(client_socket[j],
                                 msg,
                                 strlen(msg),
                                 0);
                    }
                }
            }
        }
    }

    return 0;
}
