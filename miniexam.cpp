// // Allowed functions: write, close, select, socket, accept, listen, send, recv, bind, strstr, malloc, realloc, free, calloc, bzero, atoi, sprintf, strlen, exit, strcpy, strcat, memset

// #include <errno.h>
// #include <string.h>
// #include <unistd.h>
// #include <netdb.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <stdlib.h>
// #include <stdio.h>
// #include <unistd.h>
// #include <sys/socket.h>
// #include <sys/select.h>

// int main() {
// 	int sockfd, connfd, len;
// 	struct sockaddr_in servaddr; 

// 	// socket create and verification 
// 	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
// 	if (sockfd == -1) { 
// 		exit(0); 
// 	} 
// 	bzero(&servaddr, sizeof(servaddr)); 

// 	// assign IP, PORT 
// 	servaddr.sin_family = AF_INET; 
// 	servaddr.sin_addr.s_addr = htonl(2130706433); //127.0.0.1
// 	servaddr.sin_port = htons(8081); 
  
// 	// Binding newly created socket to given IP and verification 
// 	if ((bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) { 
// 		printf("socket bind failed...\n");
//         exit(0); 
// 	} 
// 	if (listen(sockfd, 10) != 0) {
// 		exit(0); 
// 	}



//     fd_set readfds;
//     fd_set tmp;

//     bzero(&readfds, sizeof(readfds));
//     FD_ZERO(&readfds);
//     int client_socket[3000];

//     for (int i = 0; i < 3000; i++) {
//         client_socket[i] = 0;
//     }
//     client_socket[0] = sockfd;

//     int num_fds = 1;
//     FD_SET(sockfd, &readfds);
//     while (1) {
//         tmp = readfds;
//         select(num_fds + 1, &readfds, NULL, NULL, 0);
//         for (size_t i = 0; i < num_fds; i++)
//         {
//             // kayn event
//             if (FD_ISSET(client_socket[i], &tmp)) {
//                 // event server
//                 printf("event server\n");
//                 if (client_socket[i] == sockfd) {
//                     num_fds++;
//                     int acc = accept(sockfd, NULL, NULL);
//                     client_socket[num_fds - 1] = acc;
//                     FD_SET(acc, &readfds);
//                     send(acc, "Welcome to the server!\n", 23, 0);
//                 }
//                 // event client
//                 else {
//                     char buff[1024];
//                     bzero(buff, sizeof(buff));
//                     int rec = recv(client_socket[i], buff, sizeof(buff), 0);
//                     //  new message
//                     if (rec > 0) {
//                     }
//                     // client disconnected
//                     else {
//                         num_fds--;
//                     }
//                 }
//             }
//         }
        
//     }
// }
