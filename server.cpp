#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <arpa/inet.h>

using namespace std;

char scacchiera[8][8]={{'t','c','a','r','k','a','c','t'},{'p','p','p','p','p','p','p','p'},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{'P','P','P','P','P','P','P','P'},{'T','C','A','R','K','A','C','T'}};

bool scacco(char movimento[]); //controlla scacco

int main(int argc, char** argv)
{
    //collegamento
    int sockfd, newsockfd, portno, clilen,n;
	struct sockaddr_in serv_addr;
	struct sockaddr_in cli_addr;
	if (argc < 2)
	{
		cout<<"Indirizzo IP richiesto\n";
		return 0;
	}
	if (argc < 3)
	{
		cout<<"Numro porta richiesto\n";
		return 0;
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		cout<<"errore di apertura del socket\n";
	memset(&serv_addr,0,sizeof(serv_addr));
	portno = atoi(argv[2]);
	serv_addr.sin_family = AF_INET;
	inet_aton(argv[1],&(serv_addr.sin_addr));
	serv_addr.sin_port = htons(portno);

	if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
		cout<<"errore di collegamento\n";

	listen(sockfd,5);
	clilen = sizeof(cli_addr);

	newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr,(socklen_t *)&clilen);
	if (newsockfd == 0)
		cout<<"errore su accept()\n";
	memset(buffer,0,50);

    //comunicazione
    do{


    }while();
}

bool scacco()
{


}
 /*
 switch(scacchiera[i][j])
{
    case 'P':
        cout<<"♟";break;
    case 'C':
        cout<<"♞";break;
    case 'A':
        cout<<"♝";break;
    case 'T':
        cout<<"♜";break;
    case 'K':
        cout<<"♚";break;
    case 'R':
        cout<<"♛";break;
    case 'p':
        cout<<"♙";break;
    case 'c':
        cout<<"♘";break;
    case 'a':
        cout<<"♗";break;
    case 't':
        cout<<"♖";break;
    case 'k':
        cout<<"♔";break;
    case 'r':
        cout<<"♕";break;
}
*/
