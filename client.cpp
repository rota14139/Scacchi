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

using namespace std;

//per coordinare i due client senza farli comunicare utilizzo una booleana che quando assume il valore di 0, permette al "BIANCO" di muoversi. Ogni volta che i client ricevono un pacchetto M****, cambiano la booleana.


void stampaScacchiera();
void login();
void inserireMovimento();
void formattaMovimento();
void movimentoScacco();
void erroreScacco();

char numeroUtente;
char nomeUtente[16];
char nomeAvversario[16];
char buffer[256];
char movimento[6];
char scacchiera[8][8]={{'t','c','a','r','k','a','c','t'},{'p','p','p','p','p','p','p','p'},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{'P','P','P','P','P','P','P','P'},{'T','C','A','R','K','A','C','T'}};
bool turno;

int main(int argc, char *argv[])
{
	int sockfd,portno,n,i;
	char server_ip[100];
	char risposta;
	bool controlloFinePartita=false;
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
	cout<<"Attesa avversario...\n";
	n = read(sockfd,buffer,255);
	if(buffer[1]=='B')
	{
		numeroUtente='B';
		turno=true;
	}
	else
	{
		numeroUtente='N';
		turno=false;
	}
	login();
	strcpy(buffer,"I");
	strcat(buffer,nomeUtente);
	n = write(sockfd,buffer,strlen(buffer)+1);
	if (n < 0)
	{
		cout<<"errore di scrittura nel socket\n";
	}
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
	cout<<"Attesa avversario...\n";
	do
	{
		stampaScacchiera();
		if(turno)
		{
			inserireMovimento();
			n = write(sockfd,movimento,strlen(movimento)+1);
			turno=!turno;
		}
		else
		{
			turno=!turno;
		}
		n = read(sockfd,movimento,255);
		switch(movimento[0])
		{
			case 'M':
			{
				movimentoScacco();
			}break;
			case 'E':
			{
				erroreScacco();
				turno=!turno;
			}break;
			case 'D'://arrocco
			{
				movimentoScacco();
				n = read(sockfd,movimento,255);
				movimentoScacco();
			}break;
			case 'S':
			{
				movimentoScacco();
				cout<<"\nSCACCO\n";
			}break;
			case 'F':
			{
				controlloFinePartita=true;
				cout<<"Il vincitore e': "<< movimento[1]==numeroUtente ? nomeUtente : nomeAvversario << ", colore:"<<movimento[1]=='B' ? "bianco" : "nero" ;
				cout<<"\nPremere invio per continuare...\n";
				cin.ignore();
			}break;
			case 'P':
			{
				if(movimento[1]=='?')
				{
					char prom;
					cout<<"Scegli la promozione\nA = Alfiere\nC = Cavallo\nR = Regina\nT = Torre\n";
					do
					{
						cin>>prom;
						prom=toupper(prom);
					}while(prom!='A' && prom!='C' && prom!='R' && prom!='T');
					strcpy(movimento,"P");
					movimento[1]=prom;
					n = write(sockfd,movimento,strlen(movimento)+1);
				}
				else
				{
					if(numeroUtente=='B')
					{
						switch(toupper(movimento[1]))
						{
							case 'A':
								scacchiera[8-((int)(movimento[3]-48))][(int)(movimento[2]-65)]='♝';
								break;
							case 'C':
								scacchiera[8-((int)(movimento[3]-48))][(int)(movimento[2]-65)]='♞';
								break;
							case 'R':
								scacchiera[8-((int)(movimento[3]-48))][(int)(movimento[2]-65)]='♜';
								break;
							case 'T':
								scacchiera[8-((int)(movimento[3]-48))][(int)(movimento[2]-65)]='♚';
								break;
						}
					}
					else
					{
						switch(toupper(movimento[1]))
						{
							case 'A':
								scacchiera[8-((int)(movimento[3]-48))][(int)(movimento[2]-65)]='♗';
								break;
							case 'C':
								scacchiera[8-((int)(movimento[3]-48))][(int)(movimento[2]-65)]='♘';
								break;
							case 'R':
								scacchiera[8-((int)(movimento[3]-48))][(int)(movimento[2]-65)]='♖';
								break;
							case 'T':
								scacchiera[8-((int)(movimento[3]-48))][(int)(movimento[2]-65)]='♕';
								break;
						}
					}
					
				}
				
			}break;
		}

		if (n < 0)
		{
			cout<<"errore di lettura dal socket\n";
		}
	}while(!controlloFinePartita);

	close(sockfd);
	return 0;
	// invio al server il nome utente
	// ricevo dal server il nome dell'avversario

}

void stampaScacchiera()
{
	cout<<"\n\n\n\n\n";
	if(numeroUtente=='B')
	{
		for(int i=0;i<8;i++)
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
					switch(scacchiera[i][j]){case 'P':cout<<"♟";break;case 'C':cout<<"♞";break;case 'A':cout<<"♝";break;case 'T':cout<<"♜";break;case 'K':cout<<"♚";break;case 'R':cout<<"♛";break;case 'p':cout<<"♙";break;case 'c':cout<<"♘";break;case 'a':cout<<"♗";break;case 't':cout<<"♖";break;case 'k':cout<<"♔";break;case 'r':cout<<"♕";break;default:cout<<" ";}
				cout<<" │";
			}
			cout<<" "<<8-i;
			if(i==7)
				cout<<"\n  └───┴───┴───┴───┴───┴───┴───┴───┘\n    A   B   C   D   E   F   G   H\n";
			cout<<endl;
		}
	}
	else
	{
		for(int i=7;i>=0;i--)
		{
			if(i==7)
				cout<<"    A   B   C   D   E   F   G   H\n  ┌───┬───┬───┬───┬───┬───┬───┬───┐\n";
			else
				cout<<"  ├───┼───┼───┼───┼───┼───┼───┼───┤\n";
			for(int j=0;j<8;j++)
			{
				if(j==0)
					cout<<8-i<<" │";
				cout<<" ";
					switch(scacchiera[i][j]){case 'P':cout<<"♟";break;case 'C':cout<<"♞";break;case 'A':cout<<"♝";break;case 'T':cout<<"♜";break;case 'K':cout<<"♚";break;case 'R':cout<<"♛";break;case 'p':cout<<"♙";break;case 'c':cout<<"♘";break;case 'a':cout<<"♗";break;case 't':cout<<"♖";break;case 'k':cout<<"♔";break;case 'r':cout<<"♕";break;default:cout<<" ";}
				cout<<" │";
			}
			cout<<" "<<8-i;
			if(i==0)
				cout<<"\n  └───┴───┴───┴───┴───┴───┴───┴───┘\n    A   B   C   D   E   F   G   H\n";
			cout<<endl;
		}
	}
    //♔♕♖♗♘♙
    //♚♛♜♝♞♟
    //﻿—|─│┌┐└┘├├┤┬┼═║┴
}

void login()
{
	cout<<"Inserisci il tuo nome utente: ";
	cin.getline(nomeUtente,16);
	fflush(stdin);
}

void inserireMovimento()
{
	char sup[3];
	cout<<"\nInserisci le coordinate della pedina che vuoi spostare <lettera e numero>, R per arrenderti: ";
	cin.getline (sup,'\n');
	fflush(stdin);
	if(toupper(sup[0])!='R')
	{
		strcpy(movimento,"M");
		strcat(movimento,sup);
		cout<<"\nInserisci le coordinate della nuova posizione della pedina <lettera e numero>: ";
		cin.getline (sup,'\n');
		fflush(stdin);
		strcat(movimento,sup);
	}
	else
	{
		strcpy(movimento,"RESA");
	}
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
	scacchiera[8-((int)(movimento[4]-48))][(int)(movimento[3]-65)]=scacchiera[8-((int)(movimento[2]-48))][(int)(movimento[1]-65)];
	scacchiera[8-((int)(movimento[2]-48))][(int)(movimento[1]-65)]=0;
}

void erroreScacco()
{
	switch(movimento[1])
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
		case '7':
		{
			cout<<"\nIMPOSSIBILE MUOVERE RE IN UNA CELLA SOTTO SCACCO\n";
		}break;
		case '8':
		{
			cout<<"\nNON PUOI MUOVERE SE IL TUO RE È SOTTO SCACCO/ NON PUOI MANDARE SOTTO SCACCO IL TUO RE MUOVENDO\n";
		}break;
		case '9':
		{
			cout<<"\nNON PUOI MUOVERE UN PEZZO AVVERSARIO\n";
		}break;
	}
	cout<<"Premere invio per continuare...\n";
	cin.ignore();
}
