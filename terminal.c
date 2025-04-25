//aqui va todo el codigo que involucra a la terminal
#include <stdio.h>
#include <string.h>

//funcion que se encarga de llamar a todas las demas
void main_terminal();

void main_terminal()
{
    char aux[50];

    while(1)
    {   
        printf(">> ");

        fgets(aux, sizeof(aux), stdin);
        aux[strcspn(aux, "\n")] = '\0'; //strcspn regresa la cantidad de caracteres que no contienen "\n" (debe pasarse como cadena)

        if(strcmp(aux, "saludo") == 0)
            printf("\n\tHola colega!!");
        else if(strcmp(aux, "salir") == 0) {
            printf("\n\tSaliendo...");
            break;
        }    
        

        //printf("\n");   //podria ir junto con el primer printf
    }
}

int main()
{
    main_terminal();

    return 0;
}