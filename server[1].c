#include<stdio.h>
#include<string.h>    
#include<stdlib.h>    
#include<pthread.h>
#include<math.h>
#include<time.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>    


void *client_thread(void *); //client handling thread function

int fibo(int number); // Function to calculate Fibonacci number calculation

int ran(int high, int low); //random number generation function
 
//main function
int main(int argc , char *argv[])
{
    int socket_tcp , client_sock , c;
    struct sockaddr_in server , client;
    //create socket
    socket_tcp = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_tcp == -1)
    {
        printf("Error in creating socket\n");
    }
    puts("Socket sucessfully  created\n");
   

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY; //localhost
    server.sin_port = htons( 36000 );
   
    //socket binding
    if( bind(socket_tcp,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        printf("Socket binding failed\n");
        return 1;
    }
    puts("Socket binding completed\n");
     
    //Listening, can listen up to 5 clients
    listen(socket_tcp , 5);
     
    puts("Accepting connections from clients\n");
    c = sizeof(struct sockaddr_in);
   
    pthread_t thread_number;

    //accept connection from client
    while( (client_sock = accept(socket_tcp, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        puts("Connection recieved from client\n");
         
        //create new thread to handle connection with client
        if( pthread_create( &thread_number , NULL ,  client_thread , (void*) &client_sock) < 0)
        {
            printf("Problem in creating thread\n");
            return 1;
        }
        else {
            puts("Thread Successfully Created for connection");
        }
    }
     
    if (client_sock < 0)
    {
        printf("Connection Failed\n");
        return 1;
    }
     
    return 0;
}

//Thread function to handle request
void *client_thread(void *socket_tcp)
{
    int connected = *(int*)socket_tcp;
    char send_data [4096] , recv_data[4096];
    int bytes_recieved, sin_size;
    printf("Server is listening.\n");
    // File for creating command history
    FILE *fp;
    while((bytes_recieved = recv(connected,recv_data,1024,0))>0) {
        printf("\nserver is listening\n");
        recv_data[bytes_recieved] = '\0';
        puts(recv_data);
        //if exit command recieved from client
        if (strcmp(recv_data , "exit") == 0 || strcmp(recv_data , "Exit") == 0) {
            fp = fopen("record.txt", "a+");
            fputs(recv_data,fp);
            fputs("\n",fp);
            fclose(fp);
            close(connected);
            break;
        }
        //if help command recieved from client
        else if (strcmp(recv_data, "help")==0) {
            fp = fopen("record.txt", "a+");
            fputs(recv_data,fp);
            fputs("\n",fp);
            char buffer[1024] = "You can use Following Commands : fibonacci 10 , random 5 17, sort 1 5 7 10, exit, help, show history";
            send(connected, buffer,strlen(buffer), 0);
        }
        //if fibonacci command recieved from client
        else if (strstr(recv_data, "fibonacci") != NULL) {
            fp = fopen("record.txt", "a+");
            fputs(recv_data,fp);
            fputs("\n",fp);
            //tokenize recieved command and seperate arguments
            char * delimeter;
            int array[10], i = 0;
            delimeter = strtok (recv_data," ");
            while (delimeter != NULL) {
                array[i] = atoi(delimeter);
                delimeter = strtok (NULL, " ");
                i++;
            }
            //call fibonacci function to calculate result
            int result = fibo(array[1]);
            char buffer[1024];
            snprintf(buffer, 10, "%d", result);
            fputs(buffer, fp);
            fputs("\n",fp);
            fclose(fp);
            send(connected, buffer,strlen(buffer), 0);
            }
            //if random command recieved from client
        else if (strstr(recv_data, "random") != NULL) {
            fp = fopen("record.txt", "a+");
            fputs(recv_data,fp);
            fputs("\n",fp);
            //tokenize recieved command and seperate arguments
            char * delimeter;
            int array[10], i = 0;
            delimeter = strtok (recv_data," ");
            while (delimeter != NULL) {
                    array[i] = atoi(delimeter);
                    delimeter = strtok (NULL, " ");
                    //printf("%d\n", array[i]);
                    i++;
            }
            //call random number calculation function to calculate result
            int result = ran(array[1],array[2]+1);
            char buffer[1024];
            snprintf(buffer, 10, "%d", result);
            fputs(buffer, fp);
            fputs("\n",fp);
            fclose(fp);
            send(connected, buffer,strlen(buffer), 0);
        }
        //if sort command recieved from client
        else if (strstr(recv_data, "sort") != NULL) {
            fp = fopen("record.txt", "a+");
            fputs(recv_data,fp);
            fputs("\n",fp);
            //tokenize recieved command and seperate arguments
            char * delimeter;
            int array[10], i = 0;
            int*  result_array;
            delimeter = strtok (recv_data," ");
            while (delimeter != NULL)
            {
                    array[i] = atoi(delimeter);
                    delimeter = strtok (NULL, " ");
                    //printf("%d\n", array[i]);
                    i++;
            }
            //call simple sorting function to sort array

            for(int j =0;j<10; j++){
                char buffer[1024];
                char delime[2] = ",\t";
                snprintf(buffer, 10, "%d", array[j]);
                fputs(buffer, fp);
                send(connected, buffer,strlen(buffer), 0);
                fputs(delime, fp);
                send(connected, delime,strlen(delime), 0);
            }
            fputs("\n",fp);
            fclose(fp);
        }
        //if show history command recieved from client

        else if (strcmp(recv_data, "show history")==0) {
            fp = fopen("record.txt", "a+");
            fputs(recv_data,fp);
            fputs("\n",fp);
            fclose(fp);
            fp = fopen("record.txt", "r");
            char line[256];
            while (fgets(line, sizeof(line), fp)) {
                send(connected, line, strlen(line), 0);
            }
            fclose(fp);
        }
        else {
            char buffer[1024] = "Invalid Command";
            send(connected, buffer,strlen(buffer), 0);
        }
        fflush(stdout);
    }
    return NULL;
}

//function to calcuate fibonacci series number
int fibo(int n)
{
    int a = 0, b = 1, c, i;
    if (n == 0)
        return a;
    for (i = 2; i <= n; i++) {
        c = a + b;
        a = b;
        b = c;
    }
    return b;
}

//function to calculate random number between two number range, inclusive those numbers
int ran(int low, int high) {
    srand(time(NULL));
    int randomNumber = rand() % (high - low + 1) + low;
    printf("Resulted random number = %d " , randomNumber);
    return randomNumber;
}
