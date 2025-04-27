//aqui va todo el codigo que involucra a la terminal
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define num_comandos 6

//estructura que alberga los comandos y su descripcion
typedef struct Comandos {
    char comando[70];
    char descripcion[70];
}Comandos;

//funcion que se encarga de llamar a todas las demas
void main_terminal();

void argumento_invalido(char *, char *);

void main_terminal()
{
    Comandos lista_comandos[num_comandos] = {
    {"help", "Muestra todos comandos y su descripcion"}, 
    {"send message from '' to ''", "Envia un mensaje del equipo i al equipo j"},
    {"send file from '' to ''", "Envia un archivo del equipo i al equipo j"},
    {"send image from '' to ''", "Envia una imagen del equipo i al equipo j"},
    {"saludo", "Muestra un saludo"},{"salir", "Sale de la terminal"}};

    char aux[70];
    while(1)
    {   
        printf(">> ");
        //char *aux = NULL;
        //aux = leer_entrada();
        fgets(aux, sizeof(aux), stdin);
        aux[strcspn(aux, "\n")] = '\0'; //elimina salto de linea

        char *comando = strtok(aux, " \t");     //dos delimitadores
        char *argumento = strtok(NULL, " \t");
        if(comando) {

            if(strcmp(comando, "send") == 0) {
                if(argumento) {
                    if(strcmp(argumento, "message") == 0) {
                        printf("messagexdddd");
                    }
                    else if(strcmp(argumento, "file") == 0) {
                        printf("filexd\n");
                    }
                    else if(strcmp(argumento, "image") == 0) {
                        printf("imagexddd\n");
                    }
                    else
                        argumento_invalido(comando, argumento);
                }
                else {
                    printf("\tError: el comando send deberia tener un argumento\n");
                }
                //aqui van las 3 opciones de send
                //necesito otra funcion para determinar lo que se envia y en base a eso los nombres de los equipos
            }
            else if(strcmp(comando, "help") == 0) {
                if(argumento) 
                    argumento_invalido(comando, argumento);
                else {
                    printf("%-35s%s\n\n", "Comando", "Descripcion");
                    for(int i = 0; i < num_comandos; i++) {
                        printf("%-35s%s\n", lista_comandos[i].comando, lista_comandos[i].descripcion);
                    }
                }
            }
            else if(strcmp(comando, "saludo") == 0)
                printf("\n\tHola colega!!\n");
            else if(strcmp(comando, "salir") == 0) {
                printf("\n\tSaliendo...\n");
                break;
            } 
            else    //esta es la ultima condicion
                printf("\n\tError: no existe comando: '%s'\n", comando); 
        
        }

        
    }
}


void argumento_invalido(char *comando, char *argumento) {
    printf("\tError: el comando %s no tiene el argumento %s\n", comando, argumento);

}

int main()
{
    main_terminal();

    return 0;
}