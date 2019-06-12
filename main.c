#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include "raylib.h"

int sz = 0;
void* uicp (char *argv[]){
    char buff[10];
    int data;
    int i;
    int filec,filev;
    char buffer [500];
    struct stat f_stat;

    stat(argv[1], &f_stat);

    if(S_ISREG(f_stat.st_mode)){//checa se é arquivo ou pasta
      i=0;
      sz = 0;

      filec = open(argv[1], O_RDONLY); // Abre o arquivo que vai ser copiado
      filev = open(argv[2], O_WRONLY|O_CREAT|O_TRUNC, 0777); //Cria um arquivo e se ja existir apaga seu conteudo

      while(i < f_stat.st_size){ //Tamanho do arquivo
        data = read(filec, buff, 10);
        write(filev,buff, data);
        i++;
        sz =100* i/f_stat.st_size;
      }
      close(filec);
      close(filev);
    }

    else{

      DIR *d;
      d = opendir(argv[1]); // Tenta abrir a pasta
      if(d){ // Se a pasta existir
        struct dirent *dir;
        if (stat( argv[2],&f_stat) == -1) { // Cria a pasta destino
            mkdir( argv[2], 0777);
        }
        stat(argv[2], &f_stat);
        if(!S_ISREG(f_stat.st_mode)){ // checa se não é uma pasta
          while((dir = readdir(d))!= NULL){ // Le cada arquivo da pasta
            i=0;
            sz = 0;

            sprintf(buffer,"%s/%s",argv[1],dir->d_name);
            stat(buffer, &f_stat);

            if(S_ISREG(f_stat.st_mode)){ // checa se é arquivo ou pasta

              filec = open(buffer, O_RDONLY); // Abre o arquivo que vai ser copiado
              sprintf(buffer,"%s/%s",argv[2],dir->d_name);
              filev = open(buffer, O_WRONLY|O_CREAT|O_TRUNC, 0777); //Cria um arquivo e se ja existir apaga seu conteudo

              while(i < f_stat.st_size){ // Tamanho do arquivo
                data = read(filec, buff, 10);
                write(filev,buff, data);
                i++;
                sz = 100* i/f_stat.st_size;
              }

              close(filec);
              close(filev);
            }
          }
        }
          else{ // Se não é uma pasta é um arquivo
            printf("Erro, o caminho do destino é um arquivo e não uma pasta\n" );
            exit(-2);
          }

        closedir(d);
      }
    }
}

int main(int argc, char const *argv[]){

   pthread_t 	t1;
   char txt[200];
   char perc[200];

   int screenWidth = 800;
   int screenHeight = 450;
   InitWindow(screenWidth, screenHeight, "uicp");
   int boxPositionY = screenHeight/2;
   SetTargetFPS(60);

   sprintf(txt,"Copiando %s para %s",argv[1],argv[2]);

   pthread_create(&t1, NULL, uicp, argv); // Cria a thread

   while (!WindowShouldClose()){
     BeginDrawing();
     ClearBackground(RAYWHITE);
     DrawRectangle(200, 200, 400, 12, LIGHTGRAY);
     DrawRectangle(200, 200, (int)4*sz, 12, BLUE);
     DrawRectangleLines(200, 200, 400, 12, GRAY);
     DrawText(txt, 215, 250, 20, BLACK);
     sprintf(perc,"%d%% copiado",sz);
     DrawText(perc, 208, 280, 20, BLACK);
     EndDrawing();
    }
    pthread_join(t1, NULL); // Espera pela thread

    CloseWindow();
}
