#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/msg.h>
#include <errno.h>
#include <string.h>
#include <semaphore.h>

#include <sys/msg.h>

#include "song.h"

int main(){
    int running = 1;
    int msgid;
    struct song song;
    long int song_index = 0;

    msgid = msgget((key_t)1234, 0666 | IPC_CREAT);

    if (msgid == -1) {
        fprintf(stderr, "msgget failed with error: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    while(running) {
        // REGIAO CRÍTICA!!
        if (msgrcv(msgid, (void *)&song, BUFSIZ, song_index, 0) == -1) {
            fprintf(stderr, "msgrcv failed with error: %d\n", errno);
            exit(EXIT_FAILURE);
        }
        printf("Musica: \n Nome: %s, Autor: %s,  Duracao: 00:0%i, Gênero: %s\n",  song.nome, song.autor, song.duracao, song.genero);
        // FIM DA REGIÃO CRÍTICA
        sleep(song.duracao);
        printf("finalizou Musica: %s\n\n", song.nome);
    }

    if (msgctl(msgid, IPC_RMID, 0) == -1) {
        fprintf(stderr, "msgctl(IPC_RMID) failed\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
