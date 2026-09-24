#include <stdio.h>
#include <sys/socket.h> //AF_INET SOCK_STREAM 
#include <netinet/in.h>
#include <unistd.h> // close()
#include <string.h> // strlen
int main(){
   int serverSocket = socket(AF_INET, SOCK_STREAM, 0);  //Endpunkt wird erstellt

   struct sockaddr_in serverAddress;    // Socket Adresse von dem Server
   serverAddress.sin_family = AF_INET;  // IPv4 "Family"
   serverAddress.sin_port = htons(8080); // Port wird zugeschrieben
   serverAddress.sin_addr.s_addr = INADDR_ANY;

   bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)); // Socket wird mit der ServerAdresse verbunden
   printf("bind successfull");

   listen(serverSocket, 5); // maximale Nummer an Requests in einer Schlange 
   printf("Server listening on Port: %d\n", 8080);

   char response[]=
   "HTTP/1.1 200 OK \r\n"
   "Content-Type: text/plain\r\n"
   "Content-Length: 12\r\n"
   "\r\n"
   "Hello World\n";

   while(1) {
	   int clientSocket = accept(serverSocket, NULL, NULL); //Verbindung zwischen Server und Client annehmen
	   write(clientSocket, response, strlen(response));	//Antwort
   }

   close(serverSocket);
   return 0;
}
