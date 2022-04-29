#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    //Ket darab valozot hoztam letre, hogy krealni tudjam a ket csovezeteket
    int pipefd[2];
    int pipefdMasik[2];

    pid_t cpid;           //A gyerek pidje lesz benne
    int buf;              //Letrehoztam egy buffert
    int bufMasik;         //Masik buffer a kuldendo szovegnek

    if (pipe(pipefd) == -1)
    {
        perror("pipe");    //Csovezeteket krealunk, amiben a ket fd lesz eltarolva
        exit(-1);
    }
    
    if (pipe(pipefdMasik) == -1)
    {
        perror("pipe");   //Csovezeteket krealunk, amiben a ket fd lesz eltarolva
        exit(-1);
    }

    cpid = fork();
    if (cpid == -1)
    {
        perror("Elforkol");
        exit(-1);
    }

    if (cpid == 0)      //A cpid gyerek lesz
    {
        printf("%d: gyerek vagyok\n",getpid()); //A gyerek csak olvasni fog
        while(read(pipefd[0], &buf, 1) > 0)
            printf("%d",buf);

        //Itt becsukom az olvaso veget
        int pid = getpid();
        write(pipefdMasik[1], &pid, sizeof(int));

        exit(0); // kilepek

    }
    else               //Ilyenkor a cpid szulo lesz
    {
        printf("%d: szulo vagyok\n",getpid());
        //Itt az olvaso veget fogjuk bezarni, mert irni fogunk

        int pid = getpid();
        write(pipefd[1], &pid, sizeof(int)); // Anyagot ontunk a csobe

        //Addig olvasunk a cso kimeneterol, amig meg lehet (1 byte-onkent)
        //Lecsukom a csatorna fedelet, a masik oldalon EOF lesz
        while (read(pipefdMasik[0], &bufMasik, 1) > 0)  
            printf("%d",bufMasik);
        
        wait(NULL);  //Gyerekre varunk

        exit(0);
    }
    
    return 0;
}