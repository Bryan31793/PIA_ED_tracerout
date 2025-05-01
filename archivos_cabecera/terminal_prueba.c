//aqui va todo el codigo que involucra a la terminal
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h> //isdigit
#include "Grafo.h"

#define num_comandos 6

//estructura que alberga los comandos y su descripcion
typedef struct Comandos {
    char comando[70];
    char descripcion[70];
}Comandos;

//funcion que se encarga de llamar a todas las demas
void main_terminal();

void argumento_invalido(char *, char *);

void enviar(char *, int);

int validacion_equipos(char *);


void main_terminal()
{
    Grafo *grafo = generar_topologia();
    
    Comandos lista_comandos[num_comandos] = {
    {"help", "Muestra todos comandos y su descripcion"}, 
    {"send message from '' to ''", "Envia un mensaje del equipo i al equipo j"},    //para message deberia limpiar la cadena de una forma diferente
    {"send file from '' to ''", "Envia un archivo del equipo i al equipo j"},   //entre file y from podria haber algo como tarea.txt
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
            else if(strcmp(comando, "saludo") == 0)     //este no va
                printf("\n\tHola colega!!\n");
            else if(strcmp(comando, "salir") == 0) {        //este probablemente tampoco
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


void enviar(char *token, int recursos) {    //los recursos dependen de lo que se envia (file/message/image)
    //para este punto ya se debio haber consumido el from
    char *vertice_1 = strtok(NULL, " \t");
    if(!vertice_1) {    //si se ingreso send image from
        printf("\n\tError: el comando from deberia tener un argumento");
        return;
    }

    token = strtok(NULL, " \t");
    if(!token) {        //si se ingreso send file from v1
        printf("\n\tError: comando incompleto");
        return;
    }

    char *vertice_2 = strtok(NULL, " \t");
    if(!vertice_1) {    //si se ingreso send message from v1 to
        printf("\n\tError: el comando to deberia tener un argumento");
        return;
    }

    token = strtok(NULL, " \t");
    if(token) {     //si se ingreso send file from v1 to v2 sfdgas
        printf("\n\tError: %s no es parte del comando", token);
        return;
    }

    //tambien validar que los vertices sean equipos
    if(!validacion_equipos(vertice_1) || !validacion_equipos(vertice_2))   //no cumplen
        return;

    //aqui se valida que existan los vertices con la funcion de grafo
    //Vertice *vertice_origen = buscar_vertice(grafo, vertice_1);
    //Vertice *vertice_destion = buscar_vertice(grafo, vertice_2);

    //despues enviarlo a dijstra y almacenar el camino
    
    //luego mandarlo a la funcion ajustar_pesos (crear estructura para empaquetar los parametros de la funcoin ajustar_pesos)

    
    
    
    
}

int validacion_equipos(char *vertice) {
    //los vertices debe seguir el formato PC##

    if(strlen(vertice) != 4) //se valida que la longitud sea correcta 
        return 0;

    if(vertice[0] != 'P' && vertice[1] != 'C')  //los primeros dos caracteres deben ser P y C
        return 0;
    
    if(!isdigit(vertice[2]) || !isdigit(vertice[3]))   //los siguientes dos digitos deben ser numeros
        return 0;

    return 1;
    
}

int main()
{
    main_terminal();

    return 0;
}