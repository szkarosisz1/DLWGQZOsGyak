#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#define MSGKEY 654321L

struct msgbuf1 
{
	long mtype;
	char mtext[512];

} rcvbuf, *msgp;		     /* message buffer es pointere */

struct msqid_ds ds, *buf;	/* uzenetsorhoz asszocialt struktura es pointere*/



int main()
{
    int msgid;		        /* uzenetsor azonosito */
	key_t key;		        /* kulcs az uzenetsorhoz */
	int mtype, msgflg;	    /* tipus, flag */
	int rtn, msgsz;		    /* return es meret */
	
	key = MSGKEY;		    /* beallitom a kulcsot */
	msgflg = 00666 | IPC_CREAT | MSG_NOERROR;

	msgid = msgget( key, msgflg);    
 	if ( msgid == -1) 
    {
        perror("\n The msgget system call failed!");
        exit(-1);
    }
	printf("\n Az msgid: %d",msgid);

	msgp = &rcvbuf;		   /* uzenetfogado buffer cime */
	buf = &ds;		       /* uzenetsor jellemzo adataihoz */
	msgsz = 20;		       /* max hossz */
	mtype = 0;		       /* minden tipust varok */
	rtn = msgctl(msgid,IPC_STAT,buf);               /* uzenetsor adatokat lekerdezem */
	printf("\n Az uzenetek szama: %ld \n", buf->msg_qnum);
	
	while (buf->msg_qnum)       /* van-e uzenet?*/
    {		                     	                        
	    rtn = msgrcv(msgid,(struct msgbuf *)msgp, msgsz, mtype, msgflg);   /* veszem a kovetkezo uzenetet: */
	    printf("\n Az rtn: %d,  a vett uzenet: %s\n",rtn, msgp->mtext);
	    rtn = msgctl(msgid,IPC_STAT,buf);             /* uzenetsor adatokat lekerdezem, benne azt is, hany uzenet van meg */
    }                           

    exit(0);
}