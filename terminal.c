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

//funcion que se encarga de validar la entrada
char *leer_entrada();

void main_terminal()
{
    Comandos lista_comandos[num_comandos] = {
    {"help", "Muestra todos comandos y su descripcion"}, 
    {"send message from '' to ''", "Envia un mensaje del equipo i al equipo j"},
    {"send file from '' to ''", "Envia un archivo del equipo i al equipo j"},
    {"send image from '' to ''", "Envia una imagen del equipo i al equipo j"},
    {"saludo", "Muestra un saludo"},{"salir", "Sale de la terminal"}};

    while(1)
    {   
        printf(">> ");
        char *aux = NULL;
        aux = leer_entrada();

        if(aux) {
            if(strcmp(aux, "send") == 0) {
                //aqui van las 3 opciones de send
                //necesito otra funcion para determinar lo que se envia y en base a eso los nombres de los equipos
                printf("xdxd");
            }
            else if(strcmp(aux, "help") == 0) {
                printf("%-35s%s\n\n", "Comando", "Descripcion");
                for(int i = 0; i < num_comandos; i++) {
                    printf("%-35s%s\n", lista_comandos[i].comando, lista_comandos[i].descripcion);
                    //printf("%s", lista_comandos[i].comando);
                    //printf("\t\t\t\t\t\t%s\n", lista_comandos[i].descripcion);
                }
            }
            else if(strcmp(aux, "saludo") == 0)
                printf("\n\tHola colega!!\n");
            else if(strcmp(aux, "salir") == 0) {
                printf("\n\tSaliendo...\n");
                break;
            } 
            else if(aux[0] != '\0')     //esta es la ultima condicion
                printf("\n\tError: no existe comando: '%s'\n", aux);   
            
            free(aux);
        }

        
    }
}

//aun hay algunos problemas
//ejemplo:     **comando correcto** sdf      <--se ejecuta el comando
//ejemplo:     **comando correcto**\tdsf     <--no se ejecuta el comando
char *leer_entrada()     //el comando se podria leer dentro de esta funcion
{
    //debo crear un bloque de memoria
    char *entrada = (char*)malloc(sizeof(char) * 50);
    if(!entrada) {
        printf("\n\tError al asignar memoria");
        return NULL;
    }
        
    //lee la entrada
    fgets(entrada, 50, stdin);
    entrada[strcspn(entrada, "\n")] = '\0'; //strcspn regresa la cantidad de caracteres que van desde aux[0] a "\n" (debe pasarse como cadena)

    //avanza e ignora los espacios y tabulaciones a la izquierda
    int i = 0;
    while(entrada[i] == ' ' || entrada[i] == '\t') {
        i++;
    }

    if(entrada[i] == '\0') { //la cadena unicamente contiene espacios o tabulaciones
        entrada[0] = '\0';  
        return entrada;
    }

    char *comando = (char*)malloc(sizeof(char) * 50);   //se crea nuevo espacio en memoria para la cadena
    if(!comando) {
        printf("\n\tError al asignar memoria");
        return NULL;
    }
    int j = 0;

    while(entrada[i] != ' ' && entrada[i] != '\0' && entrada[i] != '\t') {  //se detiene cuando encuentra un espacio, caracter nulo o tabulacion
        comando[j] = entrada[i];
        j++;
        i++;
    }
    //pero a la derecha puede haber mas 
    int pos_espacio = j;
    while(entrada[i] != '\0') {
        if(comando[j - 1] != ' ' && entrada[i] == ' ') {    //pero que pasa si ya no hay nada mas que solo espacios??
            comando[j] = ' ';
            pos_espacio = j;    //guardo la posicion en que esta el espacio
            j++;
            
        }
        
        i++;
        while(entrada[i] != ' ' && entrada[i] != '\0' && entrada[i] != '\t') {
            comando[j] = entrada[i];
            i++;
            j++;
            pos_espacio = j;
        }
    }
    comando[pos_espacio] = '\0';
    free(entrada);
    return comando;
    
}

int main()
{
    main_terminal();

    return 0;
}