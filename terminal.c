//aqui va todo el codigo que involucra a la terminal
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


//funcion que se encarga de llamar a todas las demas
void main_terminal();

//funcion que se encarga de validar la entrada
char *leer_entrada();

void main_terminal()
{

    while(1)
    {   
        printf(">> ");
        char *aux = NULL;
        aux = leer_entrada();

        if(aux) {
            if(strcmp(aux, "saludo") == 0)
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
char *leer_entrada()     //el comando se podria leer dentro de esta funcion
{
    //debo crear un bloque de memoria
    char *entrada = (char*)malloc(sizeof(char) * 50);
    if(!entrada) {
        printf("\n\tError al asignar memoria");
        return NULL;
    }
        

    fgets(entrada, 50, stdin);
    entrada[strcspn(entrada, "\n")] = '\0'; //strcspn regresa la cantidad de caracteres que van desde aux[0] a "\n" (debe pasarse como cadena)

    int i = 0;
    while(entrada[i] == ' ' || entrada[i] == '\t') {
        i++;
    }

    if(entrada[i] == '\0') { //la cadena unicamente contiene espacios o tabulaciones
        entrada[0] = '\0';  
        return entrada;
    }

    char *comando = (char*)malloc(sizeof(char) * 50);
    if(!comando) {
        printf("\n\tError al asignar memoria");
        return NULL;
    }
    int j = 0;

    while(entrada[i] != ' ' && entrada[i] != '\0' && entrada[i] != '\t') {
        comando[j] = entrada[i];
        j++;
        i++;
    }
    free(entrada);
    comando[j] = '\0';
    return comando;
    
}

int main()
{
    main_terminal();

    return 0;
}