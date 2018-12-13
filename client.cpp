#include <iostream>
#include <stdlib.h>
using namespace std;



void stampaScacchiera();
void login();
void movimento();

char nomeUtente[16];
char nomeAvversario[16];
char buffer[256];

int main(int argc, char *argv[])
{
	int sockfd, portno, n;
	char server_ip[100];
	struct sockaddr_in serv_addr;
	struct hostent *server;
	
	
	if (argc < 3) 
	{
	   cout<<"impostare indirizzo  host porta \n";
	   exit(0);
	}
	portno = atoi(argv[2]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);    
	if (sockfd < 0)
	{
		cout<<"errore apertura socket\n";
	}
	strncpy(server_ip,argv[1],20);
	if (server_ip == NULL) {
		cout<<"errore non trovo host\n";
		exit(0);
	}
	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(server_ip);

	serv_addr.sin_port = htons(portno);
	if (connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) < 0) 
	{
		cout<<"errore di connessione\n";
		exit(-1);
	}
	
	//login al server
	memset(buffer,0,256);
	login();
	strcpy(buffer,"I");
	strcat(buffer,nomeUtente);
	n = write(sockfd,buffer,strlen(nomeUtente));
	if (n < 0)
	{
		cout<<"errore di scrittura nel socket\n";
	}
	cout<<"Attesa avversario...\n";
	n = read(sockfd,buffer,255);
	for(int i=1;buffer[i]!='\0';i++)
	{
		nomeAvversario=buffer[i];
	}
	if (n < 0)
	{
		cout<<"errore di lettura dal socket\n";
	}
	//fine login server
	
	do
	{
		cout<<"Attesa avversario...\n";
		n = read(sockfd,buffer,255);
		stampaScacchiera();
		if (n < 0)
		{
			cout<<"errore di lettura dal socket\n";
		}		
	}while(strcmp(movimento,"resa"));
	
	close(sockfd); 
	return 0;
	
	
	// invio al server il nome utente
	// ricevo dal server il nome dell'avversario
	
	
}

void stampaScacchiera()
{
	char scacchiera[8][8]={{'t','c','a','r','k','a','c','t'},{'p','p','p','p','p','p','p','p'},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{'P','P','P','P','P','P','P','P'},{'T','C','A','R','K','A','C','T'}};
	cout<<"\n\n";
	for(int i =0;i<8;i++)
	{
		if(i==0)
			cout<<"    A   B   C   D   E   F   G   H\n  ┌───┬───┬───┬───┬───┬───┬───┬───┐\n";
		else
			cout<<"  ├───┼───┼───┼───┼───┼───┼───┼───┤\n";
		for(int j =0;j<8;j++)
		{
			if(j==0)
				cout<<8-i<<" │";
			cout<<" ";
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
				default:
					cout<<" ";
			}
			cout<<" │";
		}
		cout<<" "<<8-i;	
		if(i==7)
			cout<<"\n  └───┴───┴───┴───┴───┴───┴───┴───┘\n    A   B   C   D   E   F   G   H\n";
		cout<<endl;
	}
    //♔♕♖♗♘♙
    //♚♛♜♝♞♟
    //﻿—|─│┌┐└┘├├┤┬┼═║┴
}

void login()
{
	cout<<"Inserisci il tuo nome utente: ";
	cin.getline(nomeUtente,16);
}
