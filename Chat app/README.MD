first compile the server and the client:

gcc server.c -o server.exe -lws2_32
gcc client.c -o client.exe -lws2_32

then run the server first :

./server.exe
The server will start and listen on port 8080.

then the client :

./client.exe
The client will connect to the server running on 127.0.0.1 (localhost).

can start chatting now! <3



