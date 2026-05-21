# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall

# Output file names
SERVER = server
CLIENT = client

# Source files
SERVER_SRC = server.c
CLIENT_SRC = client.c



# Default target
all: $(SERVER) $(CLIENT)



# Build server
$(SERVER): $(SERVER_SRC)
	$(CC) $(CFLAGS) $(SERVER_SRC) -o $(SERVER)



# Build client
$(CLIENT): $(CLIENT_SRC)
	$(CC) $(CFLAGS) $(CLIENT_SRC) -o $(CLIENT)



# Remove compiled files
clean:
	rm -f $(SERVER) $(CLIENT)



# Run server
run-server:
	./$(SERVER)



# Run client
run-client:
	./$(CLIENT)
