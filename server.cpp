#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "scacchiera.h"
#include "pezzi.h"

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
/*
classe scacchiera ->collezione di->
astract classe pezzo ->sottoclassi->
    -re
    -regina
    -torre
    -alfiere
    -cavallo
    -pedone
*/

void login (int player1,int player2, char player_name[2][16]);
void toup(char stringa[]);
void comunica_movimento(char mov[5],int player1,int player2);
char richiedi_promozione(int player);
void comunica_promozione(char* pos ,char pezzo,int player1,int player2);
void comunica_errore(int err,int player);
void comunica_fine(bool turno,int player1,int player2,char * player_name);
void comunica_arrocco(char *mov ,int player1,int player2);
void comunica_scacco(char mov[5],int player1,int player2);

scacchiera scacchiera1;

int main(int argc, char **argv)
{
    int sockfd, player1,player2, portno, clilen,n;
	struct sockaddr_in serv_addr;
	struct sockaddr_in cli_addr;
	if (argc < 2)
	{
		cout<<"Indirizzo IP richiesto\n";
		return -1;
	}
	if (argc < 3)
	{
		cout<<"Numero porta richiesto\n";
		return -1;
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
    {
		cout<<"errore di apertura del socket\n";
        return -1;
    }
	memset(&serv_addr,0,sizeof(serv_addr));
	portno = atoi(argv[2]);
	serv_addr.sin_family = AF_INET;
	inet_aton(argv[1],&(serv_addr.sin_addr));
	serv_addr.sin_port = htons(portno);
	if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
    {
		cout<<"errore di collegamento\n";
        return -1;
    }
	listen(sockfd,2);
	clilen = sizeof(cli_addr);


	cout<<"-server- attessa player1\n";
	player1 = accept(sockfd,(struct sockaddr *) &cli_addr,(socklen_t *)&clilen);
	if (player1 == 0)
    {
		cout<<"errore su accept() player1\n";
        return -1;
    }
	cout<<"-server- player1 connesso correttamente\n-server- attessa player2\n";
	player2 = accept(sockfd,(struct sockaddr *) &cli_addr,(socklen_t *)&clilen);
    if (player2 == 0)
    {
		cout<<"errore su accept() player2\n";
        return -1;
    }
	cout<<"-server- player2 connesso correttamente\n";
    char buffer[20];
    char player_name[2][16];
    bool turno=false; //true = nero, false = bianco
	memset(buffer,0,20);

	login(player1,player2,player_name);

    do{
        if(read((turno ? player2 : player1),buffer,sizeof(buffer))<0)
           cout<<"client -> server: errore read player"<<(turno ? 2 : 1)<<endl;
        else
            cout<<"client -> server: lettura mossa player"<<(turno ? 2 : 1)<<" buffer letto: "<<buffer<<endl;
        toup(buffer);
        switch(buffer[0])
        {
            case 'M':
            {

                int err=scacchiera1.sposta(&buffer[1],turno);
                switch(err)
                {
                    case 0:
                    {
                        turno=!turno;
                        if(scacchiera1.controlla_vittoria(turno))
                            comunica_fine(turno,player1,player2,player_name[turno]);
                        else if (scacchiera1.scacco(scacchiera1.getRe(turno),turno))
                            comunica_scacco(&buffer[1],player1,player2);
                        else
                            comunica_movimento(&buffer[1],player1,player2);


                    }break;
                    case 11://arrocco
                    {
                        comunica_arrocco(&buffer[1],player1,player2);
                        turno=!turno;
                    }break;
                    case 12:
                    {
                        char  pezzo=richiedi_promozione(turno? player2:player1);
                        scacchiera1.promozione(&buffer[3],pezzo);
                        comunica_promozione(&buffer[1],pezzo,player1,player2);
                        turno=!turno;
                    }break;
                    default:
                        comunica_errore(err,turno ? player2 : player1);

                }
            }break;
            case 'R':
            {
                comunica_fine(turno,player1,player2,player_name[turno]);
            }break;
            default:
                    comunica_errore(1,turno ? player2 : player1);
        }
    }while(true);

}

void login (int player1,int player2,char player_name[2][16])
{
    char buffer[20];
	memset(buffer,0,20);

    if(write(player1,"NB",3)<0)
       cout<<"server -> client: errore write player1\n";
    else
        cout<<"server -> client: richiesta player_name1\n";

    if(read(player1,buffer,sizeof(buffer))<0)
       cout<<"client -> server: errore read player1\n";
    else
        cout<<"client -> server: lettura player_name1: "<<&buffer[1]<<endl;
    strcpy(player_name[0],&buffer[1]);

    if(write(player2,"NN",3)<0)
       cout<<"server -> client: errore write player2\n";
    else
        cout<<"server -> client: richiesta player_name2\n";

    if(read(player2,buffer,sizeof(buffer))<0)
       cout<<"client -> server: errore read player2\n";
    else
        cout<<"client -> server: lettura player_name2: "<<&buffer[1]<<endl;
    strcpy(player_name[1],&buffer[1]);

    strcpy(buffer,"I");
    strcat(buffer,player_name[1]);
    if(write(player1,buffer,strlen(buffer)+1)<0)
       cout<<"server -> client: errore write player1\n";
    else
        cout<<"server -> client: comunicazione player_name2 a player1\n";

    strcpy(buffer,"I");
    strcat(buffer,player_name[0]);
    if(write(player2,buffer,strlen(buffer)+1)<0)
       cout<<"server -> client: errore write player2\n";
    else
        cout<<"server -> client: comunicazione player_name1 a player2\n";
    cout<<"\n\n";
}
void toup(char stringa[])
{
    int i =-1;
    while(stringa[(++i)]!=0)
        stringa[i]=toupper(stringa[i]);
}

void comunica_movimento(char mov[5],int player1,int player2)
{
    char buffer[20];
    strcpy(buffer,"M");
    strcat(buffer,mov);
    if(write(player1,buffer,strlen(buffer)+1)<0)
       cout<<"server -> client: errore write player1\n";
    else
        cout<<"server -> client: comunicazione movimento a player1\n";

    if(write(player2,buffer,strlen(buffer)+1)<0)
       cout<<"server -> client: errore write player2\n";
    else
        cout<<"server -> client: comunicazione movimento a player2\n";

}

void comunica_errore(int err,int player)
{

    char buffer[20];
    buffer[0]='E';
    buffer[1]=err+'0';
    buffer[2]=0;
    if(write(player,buffer,strlen(buffer)+1)<0)
       cout<<"server -> client: errore write player\n";
    else
        cout<<"server -> client: comunicazione errore a player\n";

}

void comunica_fine (bool turno,int player1,int player2,char * player_name)
{
    char buffer[20];
    strcpy(buffer,"F");
    strcat(buffer,turno ? "B":"N");
    strcat(buffer,player_name);
    cout<<"-server- vincitore: "<<player_name<<", player: "<<(turno? "bianco" : "nero")<<endl;
    if(write(player1,buffer,strlen(buffer)+1)<0)
       cout<<"server -> client: errore write player1\n";
    else
        cout<<"server -> client: comunicazione fine partita a player1\n";

    if(write(player2,buffer,strlen(buffer)+1)<0)
       cout<<"server -> client: errore write player2\n";
    else
        cout<<"server -> client: comunicazione fine partita a player2\n";
}

char richiedi_promozione(int player)
{
    char buffer[20];
    strcpy(buffer,"P?");
    if(write(player,buffer,strlen(buffer)+1)<0)
       cout<<"server -> client: errore write player\n";
    else
        cout<<"server -> client: richiesta promozione\n";


    if(read(player,buffer,sizeof(buffer))<0)
       cout<<"client -> server: errore read player\n";
    else
        cout<<"client -> server: ricevo pezzo promozione\n";
    return buffer[1];
}

void comunica_promozione(char* movimento, char pezzo,int player1,int player2)
{
    char buffer[20];
    strcpy(buffer,"P");
    strcat(buffer,&pezzo);
    strcat(buffer, movimento);
    cout<<buffer<<endl;
    if(write(player1,buffer,strlen(buffer)+1)<0)
       cout<<"server -> client: errore write player1\n";
    else
        cout<<"server -> client: comunicazione promozionea player1\n";

    if(write(player2,buffer,strlen(buffer)+1)<0)
       cout<<"server -> client: errore write player2\n";
    else
        cout<<"server -> client: comunicazione fine partita a player2\n";
}

void comunica_arrocco(char *mov ,int player1,int player2)
{

    char buffer[20];
    strcpy(buffer,"D");
    strcat(buffer,mov);
    if(write(player1,buffer,strlen(buffer)+1)<0)
       cout<<"server -> client: errore write player1\n";
    else
        cout<<"server -> client: comunicazione movimento a player1\n";

    if(write(player2,buffer,strlen(buffer)+1)<0)
       cout<<"server -> client: errore write player2\n";
    else
        cout<<"server -> client: comunicazione movimento a player2\n";
    mov[0]= (mov[2]=='C' ? 'A' : 'H');
    mov[2]= (mov[2]=='C' ? 'D':'F');
    comunica_movimento(mov,player1,player2);

}
void comunica_scacco(char mov[5],int player1,int player2)
{
    char buffer[20];
    strcpy(buffer,"S");
    strcat(buffer,mov);
    if(write(player1,buffer,strlen(buffer)+1)<0)
       cout<<"server -> client: errore write player1\n";
    else
        cout<<"server -> client: comunicazione movimento a player1\n";

    if(write(player2,buffer,strlen(buffer)+1)<0)
       cout<<"server -> client: errore write player2\n";
    else
        cout<<"server -> client: comunicazione movimento a player2\n";

}
