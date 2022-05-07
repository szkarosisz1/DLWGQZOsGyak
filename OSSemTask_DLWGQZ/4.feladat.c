#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    // Mindkét processhez kell egy-egy file leírót deklarálni. Ezeken keresztül fognak kommunikálni egymással.
    // Két elemű tömbre van szükség, mert az egyikbe írunk (1), a másikból olvasunk (0)
    int pipeFileLeiroGyerek[2];
    int pipeFileLeiroSzulo[2];

    // Átmeneti tároló, amibe ír vagy amiből olvas az adott process          
    int bufferGyerek;              
    int bufferSzulo;         

    // Egyik pipe létrehozása a szülő processhez. Ha nem tudja létrehozni, akkor kilép.
    if (pipe(pipeFileLeiroGyerek) == -1)
    {
        perror("Egyik pipe hibas.");    
        exit(-1);
    }
    
    // Másik pipe létrehozása a gyerek processhez. Ha nem tudja létrehozni, akkor kilép.
    if (pipe(pipeFileLeiroSzulo) == -1)
    {
        perror("Masik pipe hibas.");   
        exit(-1);
    }

    // Gyerek process ID tárolásához
    // Létrehozunk egy gyerek processt, innentől kezdve lemásolódik a kód, két processz lesz.
    // Az egyik a szülő, ilyenkor a fork kimenete (childPid) egyenlő a létrehozott gyerek process ID-jával.
    // A másik a gyerek, ilyenkor a fork kimenete (childPid) egyenlő nullával.
            
    pid_t childPid = fork();

    if (childPid == -1)
    {
        perror("Elforkol");
        exit(-1);
    }

    // Le kell kérni az adott process ID-ját, mert még nem tudjuk
    int aktualisProcessId = getpid();

    printf("Ezt mindket process kiirja! A sajat PID-em: %d\n", aktualisProcessId);

    if (childPid == 0)  // Ez az ág csak a gyerek processnél fut le.   
    {
        printf("Gyerek: gyerek process vagyok, ID-m => %d\n", aktualisProcessId);
        
        // Megvárjuk, hogy a szülő process beírjon a gyerek process file leírójába egy int méretű adatot (szülő pid)
        read(pipeFileLeiroGyerek[0], &bufferGyerek, sizeof(int));
        
        int kapottSzuloPid = bufferGyerek;

        printf("Gyerek: szulo PID-je => %d\n", kapottSzuloPid);
        
        // Válaszol a szülő process file leírójába és megadja a saját process ID-ját. Ez után rögtön tovább megy.
        write(pipeFileLeiroSzulo[1], &aktualisProcessId, sizeof(int));

        printf("Gyerek: befejeztem\n");

        exit(0);

    }
    else  // Ez az ág csak a szülő processnél fut le.           
    {      
        printf("Szulo: szulo process vagyok, ID-m => %d\n", aktualisProcessId);

        write(pipeFileLeiroGyerek[1], &aktualisProcessId, sizeof(int)); 

        // Megvárjuk, hogy a gyerek process beírjon a szülő process file leírójába egy int méretű adatot (gyerek pid)
        read(pipeFileLeiroSzulo[0], &bufferSzulo, sizeof(int));
        
        int kapottGyerekPid =  bufferSzulo;

        printf("Szulo: gyerek PID-je => %d\n",kapottGyerekPid);

        printf("Szulo: varakozok...\n");

        // Megvárja, hogy a gyerek process befejeződjön. Ilyenkor a childPid a gyerek PID-je lesz
        wait(&kapottGyerekPid); // ugyanaz, mint wait(&childPid)

        printf("Szulo: Gyerek process befejezodott.\n");

        exit(0);

    }
    
    return 0;
}
