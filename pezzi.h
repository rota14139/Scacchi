#ifndef PEZZI_H
#define PEZZI_H

#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <math.h>

using namespace std;
class pezzo
{
protected:
    bool colore; //0 bianco ~ 1 nero
public:
    pezzo(bool colore)
    {
        this->colore =colore;
    }
    virtual int movimento(char mov[5])=0;
    bool get_colore()
    {
        return colore;
    }
    //funzioni test
    virtual void stampa ()=0;
};

class re : public pezzo
{
private:
    bool mosso; //serve per arrocco
public:
    re(bool colore) : pezzo(colore)
    {
        mosso=false;
    }
    int movimento(char mov[5])
    {
        if(abs(mov[1]-mov[3])<=1 &&abs(mov[0]-mov[2])<=1)// se si sposta di 1
        {
            mosso=true;
            return true;
        }
        return false;
        /*fare arrocco...
        var...Il Re non deve mai essere stato mosso prima durante la partita;
        var...La Torre coinvolta nell’arrocco non deve mai essere stata spostata prima durante la partita;
        Il Re al momento di effettuare l’arrocco non deve essere sotto scacco (cioè situato in una casa sottoposta al tiro avversario);
        Il Re durante il movimento dell’arrocco non deve attraversare case sotto scacco. Anche la casa d'arrivo non deve essere sotto scacco;
        Fra il Re e la Torre non ci devono essere altri pezzi, né amici né avversari.
        -----
        controllare se tra non c'è vicino un altro re (da fare altro caso particolare nel return)
        */
    }
    void stampa ()
    {
        cout<<(char)('K'+(32*colore));
    }
};
class regina : public pezzo
{
public:
    regina(bool colore) : pezzo(colore)
    {
    }
    int movimento(char mov[5])
    {
        if(abs(mov[1]-mov[3])==0 || abs(mov[0]-mov[2])==0||abs(mov[1]-mov[3])==abs(mov[0]-mov[2]))
            return true;
        return false;
    }
    void stampa ()
    {
        cout<<(char)('R'+(32*colore));
    }
};
class torre : public pezzo
{
private:
    bool mosso; //serve per arrocco
public:
    torre(bool colore) : pezzo(colore)
    {
        mosso=false;
    }
    int movimento(char mov[5])
    {
        if(abs(mov[1]-mov[3])==0 || abs(mov[0]-mov[2])==0)
        {
            mosso=true;
            return true;
        }
        return false;
    }
    void stampa ()
    {
        cout<<(char)('T'+(32*colore));
    }
};
class alfiere : public pezzo
{
public:
    alfiere(bool colore) : pezzo(colore)
    {
    }
    int movimento(char mov[5])
    {
        if(abs(mov[1]-mov[3])==abs(mov[0]-mov[2]))
            return true;
        return false;
    }
    void stampa ()
    {
        cout<<(char)('A'+(32*colore));
    }
};
class cavallo : public pezzo
{
public:
    cavallo(bool colore) : pezzo(colore)
    {
    }
    int movimento(char mov[5])
    {
        if((abs(mov[0]-mov[2])==1&&abs(mov[1]-mov[3])==2)||(abs(mov[0]-mov[2])==2&&abs(mov[1]-mov[3])==1))
            return 4;
        return false;
    }
    void stampa ()
    {
        cout<<(char)('C'+(32*colore));
    }
};
class pedone : public pezzo
{
public:
    pedone(bool colore) : pezzo(colore)
    {
    }
    int movimento(char mov[5])
    {
        if(mov[0]-mov[2]==0)        //se non si muove verticalmente
            if((mov[1]-mov[3]) < (colore ? (mov[1]=='7' ? 3 : 2 ) : 0) && (mov[1]-mov[3]) > (colore ? 0 :(mov[1]=='2' ? -3 : -2 ))) /*controlla se può spostarsi in avanti (di 1 o di 2)*/
                return 2;
        if(abs(mov[0]-mov[2])==1)   //se si muove in diagonale per mangiare
            if((mov[1]-mov[3]) == (colore ? 1 : -1))//se mangia in diagonale
               return 3;
        return false;
    }
    void stampa ()
    {
        cout<<(char)('P'+(32*colore));
    }
};

#endif
