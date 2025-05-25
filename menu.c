//archivo para el menu
#include <stdio.h>
#include <string.h>

void menu();

//funcion que valida que el equipo dado exista
int validacion_equipos(char *);

int main()
{
    return 0;
}

void menu()
{
    int opc;
    do
    {
        printf("\n\tMENU");
        printf("\n\t1-Enivar informacion"); 
        printf("\n\t2-Levantar conexion");      //levanta una conexion dados dos vertices
        printf("\n\t3-Caminos completados");    //imprime todos los caminos completados
        printf("\n\t4-Caminos no completados");     //imprime todos los caminos no completados    
        printf("\n\t5-Salir");
        scanf("%d", &opc);  //validar entrada

        switch(opc)
        {
            case 1:
                char vertice_inicio[5], vertice_destino[5];
                int peso_formato;

                printf("\n\tIngresa el nombre del equipo de inicio: ");
                fgets(vertice_inicio, 5, stdin);
                vertice_inicio[strcspn(vertice_inicio, "\n")] = '\0'; //quita el salto de linea

                printf("\n\tIngresa el nombre del equipo destino: ");
                fgets(vertice_destino, 5, stdin);
                vertice_destino[strcspn(vertice_destino, "\n")] = '\0'; //quita el salto de linea

                if(!validacion_equipos(vertice_inicio) || !validacion_equipos(vertice_destino))
                    printf("\n\tError: el nombre del equipo debe ser PC##");    //a donde deberia ir a partir de aqui??
                
                //verificar que los vertices existan en el grafo

                printf("\n\tQue deseas enviar?");
                printf("\n\t1-Mensaje");
                printf("\n\t2-Archivo");
                printf("\n\t3-Video");
                scanf("%d", &peso_formato);     //validar entrada

                switch(peso_formato)    //se asigna el peso (en ancho de banda que va a ocupar)
                {
                    case 1:
                        peso_formato = 5;
                        break;

                    case 2:
                        peso_formato = 10;
                        break;

                    case 3:
                        peso_formato = 15;
                        break;

                    default:
                        break;
                }
                //aqui mando a llamar a las funciones necesarias
                break;
            
            case 2:
                char vertice_1[5], vertice_2[5];
                printf("\n\tIngresa el nombre del equipo 1: ");
                fgets(vertice_1, 5, stdin);
                vertice_1[strcspn(vertice_1, "\n")] = '\0'; //quita el salto de linea

                printf("\n\tIngresa el nombre del equipo 2: ");
                fgets(vertice_2, 5, stdin);
                vertice_2[strcspn(vertice_2, "\n")] = '\0'; //quita el salto de linea

                if(!validacion_equipos(vertice_1) || !validacion_equipos(vertice_2))
                    printf("\n\tError: el nombre del equipo debe ser PC##");

                //verificar que el los vertices existan en el grafo
                //llamar la funcion correspondiente
                break;
            
            case 3:
                printf("\n\tCaminos completados hasta el momento:");
                //llamar a la funcion correspondiente (archivos)
                break;

            case 4:
                printf("\n\tCaminos no completados por caidas de conexion:");
                //llamar la funcion correspondiente (archivos)
                break;

            case 5:
                printf("\n\tFin del programa");
                break;
                
            default:
                break;
        }

    }while(opc != 7);
    
    


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