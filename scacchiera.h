#ifndef SCACCHIERA_H
#define SCACCHIERA_H

#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include "pezzi.h"
#include <typeinfo>

using namespace std;

class scacchiera
{
private:
    pezzo * casella[8][8];
    char pos_re[2][3]={"E1","E8"};//posizione dei re per controllo scacco
public:
    scacchiera()//inizializzazione scacchiera
    {
        casella[0][0]= new torre(1);
        casella[0][1]= new cavallo(1);
        casella[0][2]= new alfiere(1);
        casella[0][3]= new regina(1);
        casella[0][4]= new re(1);
        casella[0][5]= new alfiere(1);
        casella[0][6]= new cavallo(1);
        casella[0][7]= new torre(1);
        for(int i =0;i<8;i++) //pedoni neri
            casella[1][i]= new pedone(1);

        for(int j =2;j<5;j++)   //caselle vuote
            for(int i =0;i<8;i++)
                casella[j][i]= NULL;

        for(int i =0;i<8;i++)//pedoni bianchi
            casella[6][i]= new pedone(0);
        casella[7][0]= new torre(0);
        casella[7][1]= new cavallo(0);
        casella[7][2]= new alfiere(0);
        casella[7][3]= new regina(0);
        casella[7][4]= new re(0);
        casella[7][5]= new alfiere(0);
        casella[7][6]= new cavallo(0);
        casella[7][7]= new torre(0);
    }
    int sposta(char mov[5],bool turno);
    int controlla_movimento(char mov[5],bool turno); //controlla se un movimento si possa fare
    bool scacco(char cella[3],bool colore); //controlla se una deter        minata cella è sotto scacco (utile per re e per arrocco)
    bool controllo_tragitto(char mov[5]); // bool controllo_caselle_in mezzo(char mov[5]);
    void movimento(char mov[5]); //muove un pezzo (senza controllo... quindi farlo prima)
    void promozione(char cella[3],char pezzo);
    bool controlla_vittoria(bool turno);
    char * getRe(int i);

    //funzioni per fase di test
    void stampa_scacchiera();
};
void scacchiera::stampa_scacchiera()
{
    for(int i = 0;i<8;i++)
        {
            if(i==0)
                cout<<"   A   B   C   D   E   F   G   H\n\n";
            for(int j =0;j<8;j++)
            {
                if(j==0)
                    cout<<8-i<<"| ";
                if(casella[i][j]!=NULL)
                    casella[i][j]->stampa();
                else
                    cout<<"*";
                cout<<"   ";
            }
            cout<<" "<<8-i;
            if(i==7)
                cout<<"\n   -------------------------------\n   A   B   C   D   E   F   G   H";
            else
                cout<<endl<<" |"<<endl;
        }
}
char * scacchiera::getRe(int i)
{
    return pos_re[i];
}
int scacchiera::sposta(char mov[5],bool turno)
{
    int err=controlla_movimento(mov,turno);
    switch(err)//0 = normale >=10 mosse speciali
    /*  0 normale
        1~9 errori
        10 re muove
        11 arrocco
        12 promozione */
    {
    case 0:
        {
            pezzo *mangiato = casella[8-(mov[3]-'0')][(mov[2]-'A')];
            movimento(mov);
            if (scacco(pos_re[casella[8-(mov[3]-'0')][(mov[2]-'A')]->get_colore()],casella[8-(mov[3]-'0')][(mov[2]-'A')]->get_colore()))
            {
                char sup[5]={mov[2],mov[3],mov[0],mov[1],0};
                movimento(sup);
                casella[8-(mov[3]-'0')][(mov[2]-'A')]=mangiato;
                return 8;
            }
            if(typeid(*casella[8-(mov[3]-'0')][(mov[2]-'A')])==typeid(pedone))                      //se è un pedone...
                if((mov[3])== (casella[8-(mov[3]-'0')][(mov[2]-'A')]->get_colore() ? '1' : '8'))    //controlla se si può promuovere
                    return 12;
            return 0;
        }
    case 10: //re muove
        {
            char pos[3]={mov[2],mov[3],0};
            pezzo *mangiato = casella[8-(pos[1]-'0')][(pos[0]-'A')];
            movimento(mov);
            if(scacco(pos,casella[8-(mov[3]-'0')][(mov[2]-'A')]->get_colore()))
            {
                char sup[5]={mov[2],mov[3],mov[0],mov[1],0};
                movimento(sup);
                casella[8-(pos[1]-'0')][(pos[0]-'A')]=mangiato;
                return 7;
            }
            strcpy(pos_re[casella[8-(pos[1]-'0')][(pos[0]-'A')]->get_colore()],pos);
            return 0;
        }
    case 11: //arrocco
        {
            int colore= casella[8-(mov[1]-'0')][mov[0]-'A']->get_colore();
            if(casella[8-(mov[1]-'0')][mov[0]=='C' ? 0 : 7]!=NULL)
                if(typeid(*casella[8-(mov[1]-'0')][mov[0]=='C' ? 0 : 7])==typeid(torre))//se è una torre
                    if(!casella[8-(mov[1]-'0')][mov[0]=='C' ? 0 : 7]->getMosso()); //se la torre non si è ancora mossa
                    {
                        char mov_torre[5]={mov[2]=='C' ? 'A' : 'H',mov[1],mov[2]=='C' ? 'D':'F',mov[1],0};
                        if(controllo_tragitto(mov_torre))
                        {
                            char sup[5];
                            strcpy(sup,mov);

                            if(sup[0]>sup[2])
                                sup [2]--;
                            else if(sup[0]<sup[2])
                                sup[2]++;
                            while(sup[0]!=sup[2])
                            {
                                char ctrl_scacco[3]={sup[0],sup[1],0};
                                if(casella[8-(ctrl_scacco[1]-'0')][ctrl_scacco[0]-'A']!=NULL)
                                    if(scacco(ctrl_scacco,colore))
                                        return 6;
                                if(sup[0]<sup[2])
                                    sup [0]++;
                                else if(sup[0]>sup[2])
                                    sup[0]--;
                            }
                            movimento(mov);
                            movimento(mov_torre);
                            char new_pos_re[3]={mov[3],mov[2],0};
                            strcpy(pos_re[casella[8-(mov[3]-'0')][(mov[2]-'A')]->get_colore()],new_pos_re);
                            return 11;
                        }
                    }
            return 6;
        }
    default:
        return err;
    }
}
bool scacchiera::scacco(char cella[3],bool colore)
{

    char mov[5];
    mov[2]=cella[0];
    mov[3]=cella[1];
    mov[4]=0;
    for(int j =0;j<8;j++)
        for(int i =0;i<8;i++)
        {
            mov[0]=i+'A';
            mov[1]=(7-j)+'1';
            if(casella[j][i]!=NULL)
                if(casella[8-(mov[1]-'0')][(mov[0]-'A')]->get_colore()!=colore)
                    if((abs(mov[1]-mov[3])==0 || abs(mov[0]-mov[2])==0||abs(mov[1]-mov[3])==abs(mov[0]-mov[2]))||((abs(mov[0]-mov[2])==1&&abs(mov[1]-mov[3])==2)||(abs(mov[0]-mov[2])==2&&abs(mov[1]-mov[3])==1)))
                    {
                        switch( casella[8-(mov[1]-'0')][(mov[0]-'A')]->movimento(mov))//controllo se il movimento del pezzo è legale
                            {
                            case 1: //normale
                                if(controllo_tragitto(mov))//controllo caselle_in mezzo
                                    return true;
                                break;
                            case 3: //pedone mangia
                            case 4://cavallo che non deve controllare se ci sono pezzi in mezzo
                            case 5://re muove
                                return true;
                            }
                    }
        }
    return false;
}
int scacchiera::controlla_movimento(char mov[5],bool turno)
{
    if(strlen(mov)==4)
    {
        if(mov[0]>='A'&&mov[0]<='H'&&mov[1]>='1'&&mov[1]<='8'&&mov[2]>='A'&&mov[2]<='H'&&mov[3]>='1'&&mov[3]<='8')// se le posizioni rientrano nella scacchiera
        {
            if(mov[1]!=mov[3] || mov[0]!=mov[2])// se le celle sono uguali
            {
                if(casella[8-(mov[1]-'0')][(mov[0]-'A')]!=NULL) //se la casella di partenza non è vuota
                {
                    if(casella[8-(mov[1]-'0')][(mov[0]-'A')]->get_colore()==turno)
                    {
                        if(casella[8-(mov[3]-'0')][(mov[2]-'A')]!=NULL)   //se la casella di arrivo non è vuota
                        {
                            if(casella[8-(mov[1]-'0')][(mov[0]-'A')]->get_colore()!=casella[8-(mov[3]-'0')][(mov[2]-'A')]->get_colore())//se nella cella di arrivo non c'è un pezzo dello stesso colore
                                switch( casella[8-(mov[1]-'0')][(mov[0]-'A')]->movimento(mov))//controllo se il movimento del pezzo è legale
                                {
                                case 1: //normale
                                    return controllo_tragitto(mov)? 0 : 6;//controllo caselle_in mezzo;
                                case 0://mossa sbagliata
                                case 6://arrocco
                                case 2://perdone muove avanti
                                    return 6;  //false perche' non puo' muoversi avanti visto che c'e' un pezzo nemico
                                case 3: //pedone mangia return true visto che nella cella nemica c'è per forza un pezzo nemico
                                case 4://cavallo che non deve controllare se ci sono pezzi in mezzo
                                    return 0;
                                case 5://re muove
                                    return 10;
                                }
                                return 5;//alleato nella cella di arrivo
                        }
                        else
                            switch( casella[8-(mov[1]-'0')][(mov[0]-'A')]->movimento(mov))//controllo se il movimento del pezzo è legale
                                {
                                case 1: //normale
                                case 2://perdone muove avanti
                                    return controllo_tragitto(mov)? 0 : 6;    //funzione perchè la casella davanti e' vuota ma se si muove di 2 potrebbe esserci un pezzo in mezzo
                                case 0://mossa sbagliata
                                case 3: //mangia pedone
                                    return 6; //nella casella dove potrebbe mangiare non c'è nessun pezzo nemico
                                case 4://cavallo che non deve controllare se ci sono pezzi in mezzo
                                    return 0;
                                case 5://re muove
                                    return 10;
                                case 6://arrocco
                                    return controllo_tragitto(mov)? 11 : 6;
                                }
                    }//non puoi muovere un pezzo avversario
                    return 9;
                }//casella di partenza  vuota
                return 4;
            }//casella di partenza uguale a casella di arrivo
            return 3;
        }//posizioni illegali
        return 2;
    }//errore formattazione
    return 1;
}
void scacchiera::movimento(char mov[5])//finita
{
    casella[8-(mov[3]-'0')][(mov[2]-'A')]=casella[8-(mov[1]-'0')][(mov[0]-'A')];
    casella[8-(mov[1]-'0')][(mov[0]-'A')]=NULL;
}
bool scacchiera::controllo_tragitto(char mov[5])
{
    char sup[5];
    strcpy(sup,mov);
    if(sup[0]>sup[2])
        sup [2]++;
    else if(sup[0]<sup[2])
        sup[2]--;

    if(sup[1]>sup[3])
        sup [3]++;
    else if(sup[1]<sup[3])
        sup[3]--;
    while(sup[0]!=sup[2]||sup[1]!=sup[3])
    {
        if(sup[0]<sup[2])
            sup [0]++;
        else if(sup[0]>sup[2])
            sup[0]--;

        if(sup[1]<sup[3])
            sup [1]++;
        else if(sup[1]>sup[3])
            sup[1]--;
        if(casella[8-(sup[1]-'0')][(sup[0]-'A')]!=NULL)
            return false;
    }
    return true;
}

void scacchiera::promozione(char cella[3],char pezzo)
{
    switch(pezzo)
    {
    case 'C':
        casella[8-(cella[1]-'0')][(cella[0]-'A')]= new cavallo(cella[1]=='1'? 1 : 0 );
    break;
    case 'A':
        casella[8-(cella[1]-'0')][(cella[0]-'A')]= new alfiere(cella[1]=='1'? 1 : 0 );
    break;
    case 'R':
        casella[8-(cella[1]-'0')][(cella[0]-'A')]= new regina(cella[1]=='1'? 1 : 0 );
    break;
    case 'T':
        casella[8-(cella[1]-'0')][(cella[0]-'A')]= new torre(cella[1]=='1'? 1 : 0 );
    break;
    }
}

bool scacchiera::controlla_vittoria(bool turno)
{
    return (typeid(*casella[8-(pos_re[turno][1]-'0')][pos_re[turno][0]-'A']))!=typeid(re);
}
#endif
