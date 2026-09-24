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
   //printf("bind successfull\n");

   listen(serverSocket, 5); // maximale Nummer an Requests in einer Schlange 
   //printf("Server listening on Port: %d\n", 8080);

   while(1) {
	   int clientSocket = accept(serverSocket, NULL, NULL); //Verbindung zwischen Server und Client annehmen
	   
	   char recBuf[1024] = {0};	//HTTP Request Platzhalter
	   recv(clientSocket, recBuf, 1024, 0); // Request wird "recieved"
	   //printf("%s\n", recBuf);

	   char *position = recBuf + 5; //Startpunkt des Paths
	   //printf("%s\n", position);
	   char *path = strtok(position, " ");	//Path wird extrahiert
	   //printf("%s\n", path);

	   const char *file;	// Compare HTTP Request
	   if (strcmp(path, "index.html") == 0) {
		   file = "./index.html";
	   } else {
		   file = "./notFound.html";
	   }

	   FILE *html = fopen(file, "r");	//Öffne und Lese aus der html
	
	   char buf[1024] = {0};	// Platzhalter
	   size_t read = 0;			// Anzahl an übertragenen Bytes

	   char *header	=		// HTTP RESPONSE
	   "HTTP/1.1 200 OK\r\n"
	   "Content-Type: text/html\r\n\r\n";
	   
	   send(clientSocket,header,strlen(header),0);	//Sende an den Client den HTTP Response
	   
	   while((read = fread(buf, sizeof(buf[0]), 1024, html)) > 0){ // Sende schrittweise die HTML
		   send(clientSocket, buf, read, 0);
		   }
	   
	   fclose(html);	// File wird wieder geschlossen
	   close(clientSocket);
	   //printf("disconnected client\n");
   }

   close(serverSocket);
   return 0;
}
