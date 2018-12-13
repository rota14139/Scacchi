#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <unistd.h>
#include <arpa/inet.h>
#define clear() printf("\033[H\033[J") //corrisponde al system("cls"), viene richiamato tramite la funzione "clear()" 

using namespace std;



void stampaScacchiera();
void login();
void inserireMovimento();
void formattaMovimento();
void movimentoScacco();
void erroreScacco();
void scaccoScacco();
void finePartita();

char nomeUtente[16];
char nomeAvversario[16];
char buffer[256];
char movimento[6];

int main(int argc, char *argv[])
{
	int sockfd,portno,n,i;
	char server_ip[100];
	char scacchiera[8][8]={{'t','c','a','r','k','a','c','t'},{'p','p','p','p','p','p','p','p'},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{'P','P','P','P','P','P','P','P'},{'T','C','A','R','K','A','C','T'}};
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
	for(i=1;buffer[i]!='\0';i++)
	{
		nomeAvversario[i-1]=buffer[i];
	}
	nomeAvversario[i-1]='\0';
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
		switch(buffer[0])
		{
			case 'M':
			{
				movimentoScacco();
			}break;
			case 'E':
			{
				erroreScacco();
				inserireMovimento();
				n = write(sockfd,movimento,strlen(movimento));
			}break;
			case 'D':
			{
				//arroccoScacco();
			}
			case 'S':
			{
				//scaccoScacco();
			}break;
			case 'F':
			{
				//finePartita();
			}break;
			default:
			{
				inserireMovimento();
				n = write(sockfd,movimento,strlen(movimento));
			}break;
		}
		if (n < 0)
		{
			cout<<"errore di lettura dal socket\n";
		}
		clear();
		stampaScacchiera();
		cin.get();
		clear();		
	}while(strcmp(movimento,"RESA"));
	
	close(sockfd);
	return 0;
	// invio al server il nome utente
	// ricevo dal server il nome dell'avversario

}

void stampaScacchiera()
{
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

void inserireMovimento()
{
	char sup[3];
	cout<<"\nInserisci le coordinate della pedina che vuoi spostare <lettera e numero>: ";
	do
	{
		cin.getline(sup,3);
	}while(sup[0]<32 || sup[0]>57);
	strcpy(movimento,"M");
	strcat(movimento,sup);
	cout<<"\nInserisci le coordinate della nuova posizione della pedina <lettera e numero>: ";
	do
	{
		cin.getline(sup,3);
	}while(sup[0]>32 && sup[0]<57);
	strcat(movimento,sup);
	formattaMovimento();
}

void formattaMovimento()
{
	for(int i=0;movimento[i]!='\0';i++)
	{
		movimento[i]=toupper(movimento[i]);
	}
}

void movimentoScacco()
{
	char sup;
	sup=scacchiera[(int)(movimento[1]-65)][(int)(movimento[2]-48)];
	scacchiera[(int)(movimento[1]-65)][(int)(movimento[2]-48)]=scacchiera[(int)(movimento[3]-65)][(int)(movimento[4]-48)];
	scacchiera[(int)(movimento[1]-65)][(int)(movimento[2]-48)]=sup;
}

void erroreScacco()
{
	switch(buffer[1])
	{
		case '1':
		{
			cout<<"\nERRORE FORMATTAZIONE (LUNGHEZZA)\n";
		}break;
		case '2':
		{
			cout<<"\nPOSIZIONI ILLEGALI (INESISTENTI, FUORI SCACCHIERA)\n";
		}break;
		case '3':
		{
			cout<<"\nCASELLA DI ARRIVO UGUALE A CASELLA DI PARTENZA\n";
		}break;
		case '4':
		{
			cout<<"\nNESSUN PEZZO NELLA CELLA DI PARTENZA\n";
		}break;
		case '5':
		{
			cout<<"\nALLEATO NELLA CELLA DI ARRIVO\n";
		}break;
		case '6':
		{
			cout<<"\nMOVIMENTO SBAGLIATO\n";
		}break;	
	}
}
