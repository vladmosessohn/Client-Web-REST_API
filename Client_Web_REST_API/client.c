#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"
#include "parson.h"

int main(int argc, char *argv[]) {
    char *message, *response, *info[1], *cookies[1];
    int flag = 0, sockfd;
    char command[100];
    while (1){
    	scanf("%s",command);

    	if(strcmp(command, "register") == 0) {
            sockfd = open_connection("3.8.116.10", 8080, AF_INET, SOCK_STREAM, 0);
            char my_username[BUFLEN];
            char my_password[BUFLEN];

            printf("username=");
            scanf("%s", my_username);
            printf("password=");
            scanf("%s", my_password);

			JSON_Value *val = json_value_init_object();
      		JSON_Object *obj = json_value_get_object(val);
      		json_object_set_string(obj, "username", my_username);
      		json_object_set_string(obj, "password", my_password);
      		info[0] = json_serialize_to_string_pretty(val);

            message = compute_post_request("3.8.116.10", "/api/v1/tema/auth/register", "application/json", info, 1, NULL, 0);
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);

            int res = 0;
            for(int i = 0; i < strlen(response); i++)
            	if(response[i] == 'e' && response[i + 1] == 'r' && response[i + 2] == 'r' && response[i + 3] == 'o' && response[i + 4] == 'r') {
            		res = 1;
            	}

            if (res > 0) {
                printf("The username is taken, please choose another one.\n"); // ce vrei !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            } else {
                printf("You have successfully registered!\n"); // ce vrei !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            }
            close_connection(sockfd);
    	}

    	if (strcmp(command,"login") == 0){
    		char *cooki;
    		if (flag != 1){
	            sockfd = open_connection("3.8.116.10", 8080, AF_INET, SOCK_STREAM, 0);
	            char username[BUFLEN];
	            char password[BUFLEN];

	            printf("username=");
	            scanf("%s", username);
	            printf("password=");
	            scanf("%s", password);

    			JSON_Value *val = json_value_init_object();
	      		JSON_Object *obj = json_value_get_object(val);
	      		json_object_set_string(obj, "username", username);
	      		json_object_set_string(obj, "password", password);
	      		info[0] = json_serialize_to_string_pretty(val);

	            message = compute_post_request("3.8.116.10", "/api/v1/tema/auth/login", "application/json", info, 1, NULL, 0);
	            send_to_server(sockfd, message);
	            response = receive_from_server(sockfd);

	            for(int i = 0; i < strlen(response); i++) {
	            	if(response[i] == 'c' && response[i + 1] == 'o' && response[i + 2] == 'n' && response[i + 3] == 'n' && response[i + 4] == 'e' && response[i+5] == 'c' && 
	            		response[i + 6] == 't'){
	            		// preluam cookie-ul
	            		cooki = response + i;
	            	}
	            }
	            char *tok = strtok(cooki,";");
	            // cookie-ul se afla in tok
	            cookies[0] = tok;
	            char *ptr = strstr(response, "HTTP/1.1 200 OK");
	            if (ptr != NULL){
	            	printf("Login ok.\n");
	            	flag = 1;
	            } else {
	            	printf("Login failed.\n");
	            }
       		} else {
       			printf("You are already logged in.\n");
       		}
    	}

    	if (strcmp(command,"logout") == 0){
    		
    		sockfd = open_connection("3.8.116.10", 8080, AF_INET, SOCK_STREAM, 0);
    		if(sockfd < 0) {
    			printf("Problema la deschiderea socketului.\n");
    		}
    		message = compute_get_request("3.8.116.10", "/api/v1/tema/auth/logout",NULL,cookies,1);
    		send_to_server(sockfd,message);
    		response = receive_from_server(sockfd);
    		char *find = strstr(response, "HTTP/1.1 200 OK");
    		if (cookies != NULL && find != NULL){
    			flag = 0;
    			printf("Clientul a efectuat logout-ul cu succes.\n");
    		} else {
    			printf("Clientul nu este logat.\n");
    		}
    	}

    	if(strcmp(command, "exit") == 0) {
    		printf("Clientul s-a deconectat.\n");
    		close(sockfd);
    		return 0;
    	}
        
    
    
    }
    return 0;
}