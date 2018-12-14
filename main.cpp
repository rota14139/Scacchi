#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "scacchiera.h"
#include "pezzi.h"

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

void toup(char stringa[])
{
    int i =-1;
    while(stringa[(++i)]!=0)
        stringa[i]=toupper(stringa[i]);
}
/*
ogni volta che muovi un pezzo devi controllare entrambi i re...
se il re di quello che ha mosso è sotto scacco allora la mossa è da richiedere
se il re di quello che non ha messo è sotto scacco bisogna avvisare
*/
//char scacchiera[8][8]={{'t','c','a','r','k','a','c','t'},{'p','p','p','p','p','p','p','p'},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{'P','P','P','P','P','P','P','P'},{'T','C','A','R','K','A','C','T'}};
scacchiera scacchiera1;
int main()
{
    char mov[5];
    do{
        scacchiera1.stampa_scacchiera();
        cout<<endl<<endl;
        cin>>mov;
        toup(mov);
        int err =scacchiera1.sposta(mov,0);
        if(err==0)
            cout<<"operazione a buon termine\n";
        else
            cout<<"error "<<err<<endl;

        if(scacchiera1.scacco_matto(scacchiera1.getRe(1),1))
            cout<<"scacco matto su nero\n";
        if(scacchiera1.scacco_matto(scacchiera1.getRe(0),0))
            cout<<"scacco matto su bianco\n";

        if(scacchiera1.scacco(scacchiera1.getRe(1),1))
            cout<<"scacco su nero\n";
        if(scacchiera1.scacco(scacchiera1.getRe(0),0))
            cout<<"scacco su bianco\n";
    }while(true);

}
