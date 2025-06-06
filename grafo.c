//Este archivo contendra el TDA grafo y sus operaciones necesarias para el programa
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>    //para hilos
#include <unistd.h>     //para la funcion usleep
#include <stdbool.h>
#include <ctype.h>

//se pueden agregar mas campos a los TDA
struct Vertice;

typedef struct Arista{
    int peso;       //peso de la arista
    struct Vertice *destino;   //vertice al que conduce esa arista
    pthread_mutex_t mutex;
    struct Arista *next;        //siguiente arista incidente al vertice actual
    bool caida;         //indica si la conexion esta caida o no
    int peso_max;       //el peso maximo que soporta la arista (su ancho de banda)
    int latencia;
} Arista;

typedef struct Vertice{
    char name[5];      //nombre del vertice    (el nombre tendra que cambiar a algo mas descriptivo ó agregar un campo extra)
    Arista *lista_adyacencia;       //lista de las aristas adyacentes
    struct Vertice *next;      //siguiente vertice del grafo en general
} Vertice;

typedef struct Grafo{
    Vertice *lista_vertices;    //lista que contiene todos los vertices (es la cabeza)
    int numero_vertices;        //numero de vertices del grafo
} Grafo;

typedef struct Nodo_aux {   //estructura para dijkstra
    Vertice *vertice;     // referencia al vértice original
    int distancia;        // distancia mínima conocida
    int visitado;         // si ya fue visitado
    //Vertice *anterior;    // para reconstruir el camino
    struct Nodo_aux *back;  //para reconstruir el camino
} Nodo_aux;

typedef struct Nodo_camino {     //estructura para almacenar el camino
    Vertice *vertice; 
          //el vertice en el que estamos
    Arista *arista_adyacente_1;   //arista al siguiente vertice del camino
    Arista *arista_adyacente_2;
         //lo mismo que la anterior pero desde el vertice contrario
    struct Nodo_camino *next;        //siguiente nodo de camino
} Nodo_camino;

typedef struct Camino {
    Nodo_camino *head;
    int peso;
} Camino;


typedef struct Reconexion {     //estructura necesaria para empaquetar los argumentos de la funcion 'levantar_conexion'
    Grafo *grafo;
    char vertice_1[5];
    char vertice_2[5];
} Reconexion; 


typedef struct nombre_vertice {     //estructura para usar archivos
    char nombre[5];
}nombre_vertice;


//crea un grafo y devuelve un puntero a este
Grafo *crear_grafo();

//agrega un vertice al grafo
void agregar_vertice(Grafo *, char *);

//crea un vertice y devuelve un puntero a este
Vertice *crear_vertice(char *);

//busca un vertice dado su nombre y regresa un puntero a este
Vertice *buscar_vertice(Grafo *, char *);

//conecta 2 vertices del grafo con una arista
void agregar_arista(Grafo *, char *, char *, int, int, int);

//crea una arista dados dos extremos
void crear_arista(int, int, int, Vertice *, Vertice *);

//busca una arista e indica si existe o no
int existe_arista(Vertice *, Vertice *);

//imprime el grafo
void imprimir_grafo(Grafo *);

//dijkstra
int buscar_indice(Nodo_aux *, int n, Vertice *);

Camino *dijkstra(Grafo *, const char *, char *);

void imprimir_camino(Camino *);

//camino
Camino *crear_camino();

Camino *construir_camino(Nodo_aux *, int, int);    //continuar...

Arista *encontrar_arista(Vertice *, Vertice *);

//Hilos
void *ajustar_pesos(void *arg);

//prueba de hilos
void prueba_hilos(Grafo *);

//funcion que levanta la conexion dado el nombre de los vertices adyacentes
void *levantar_conexion(void *arg);

//grafo en base a la topologia elegida
Grafo *generar_topologia();

//funcion para imprimir camino en archivo secuencial
void imprimir_camino_archivo(Camino *, char *);

void leer_caminos_archivo(char *);


//funciones para el menu
void menu();

void limpiar_pantalla();

int validacion_equipos(char *);

void validar_entero(char *, int *);

void limpiar_buffer();

int main() {
    
    //inicializacion de archivos
   FILE *archivo = fopen("caminos.txt", "w");

   if(!archivo)
        printf("Error al abrir archivo");
    
    fclose(archivo);

   FILE *archivo_2 = fopen("caminos_no_completados.txt", "w");
   if(!archivo)
        printf("Error al abrir archivo");
    
    fclose(archivo_2);

   menu();
   
   
    
    return 0;
}

Grafo *crear_grafo() {
    Grafo *nuevo_grafo = (Grafo*)malloc(sizeof(Grafo));     //se crea el grafo

    if(!nuevo_grafo) {   //validar que no sea NULL
        printf("\n\tError: no se asigno memoria correctamente");    //el mensaje podria estar en main porque igual se debe validar ahi
        return NULL;
    }

    //se inicializan valores
    nuevo_grafo->lista_vertices = NULL;
    nuevo_grafo->numero_vertices = 0;

    return nuevo_grafo;
}

void agregar_vertice(Grafo *grafo, char *nombre) {
    if(buscar_vertice(grafo, nombre)) {
        printf("\n\tError: el vertice ya existe");
        return;
    }
    Vertice *nuevo_vertice = crear_vertice(nombre);   //llamo a la funcion para crear vertice

    if(!nuevo_vertice)  //valido que no sea NULL
        return;

    //Insertar al inicio el nuevo vertice en la lista de vertices
    nuevo_vertice->next = grafo->lista_vertices;    
    grafo->lista_vertices = nuevo_vertice;
    grafo->numero_vertices++;
}

Vertice *crear_vertice(char *nombre) {
    Vertice *nuevo_vertice = (Vertice*)malloc(sizeof(Vertice));     //se crea el vertice

    if(!nuevo_vertice) { //validar que se creo correctamente
        printf("\n\tError: no se asigno memoria correctamente");    //podria quitar el mensaje
        return NULL;
    }

    //se inicializan los valores
    strncpy(nuevo_vertice->name, nombre, sizeof(nuevo_vertice->name)-1);
    nuevo_vertice->name[sizeof(nuevo_vertice->name)-1] = '\0';
    nuevo_vertice->lista_adyacencia = NULL;
    nuevo_vertice->next = NULL;

    return nuevo_vertice;
}

Vertice *buscar_vertice(Grafo *grafo, char *nombre) {
    Vertice *temp_list_vertices = grafo->lista_vertices;    //un puntero a la lista de los vertices para recorrerla

    while(temp_list_vertices && strcmp(temp_list_vertices->name, nombre) != 0) {//mientras no sea NULL y el nombre del vertice sea diferente al que buscamos
        temp_list_vertices = temp_list_vertices->next;  //avanzar hasta al siguiente vertice
    }

    if(!temp_list_vertices) //si es NULL quiere decir que llego al final, por lo tanto no lo encontro
        return NULL;

    return temp_list_vertices;  //regresa un puntero al vertice encontrado
}


void agregar_arista(Grafo *grafo, char *inicio, char *destino, int peso, int peso_maximo, int latencia) {
    //se buscan ambos vertices
    Vertice *vertice_inicial = buscar_vertice(grafo, inicio);
    Vertice *vertice_destino = buscar_vertice(grafo, destino);

    if(!vertice_inicial || !vertice_destino) {      //se verifica que existan los vertices
        printf("\n\tError: uno o ambos vertices no existen");
        return;
    }

    if(existe_arista(vertice_inicial, vertice_destino)) {      //se verifica que no exista la arista
        printf("\n\tError: ya existe arista incidente entre esos vertices");
        return;
    }

    //se crea una arista para cada vertice porque no es dirigida
    crear_arista(peso, peso_maximo, latencia, vertice_inicial, vertice_destino);   
    crear_arista(peso, peso_maximo, latencia, vertice_destino, vertice_inicial);
    
}

void crear_arista(int peso, int peso_maximo, int latencia, Vertice *vertice_inicial, Vertice *vertice_destino) {
    Arista *nueva_arista = (Arista*)malloc(sizeof(Arista)); //se crea la arista

    if(!nueva_arista) {  //validar que no sea NULL
        printf("\n\tError: no se asigno memoria correctamente");
        return;
    }

    //inicializar valores
    nueva_arista->peso = peso;
    nueva_arista->next = vertice_inicial->lista_adyacencia;
    vertice_inicial->lista_adyacencia = nueva_arista;
    nueva_arista->destino = vertice_destino;
    pthread_mutex_init(&nueva_arista->mutex, NULL);     //inicializar mutex (destruir al final)
    nueva_arista->caida = false;    //nuevos campos inicializados
    nueva_arista->peso_max = peso_maximo;
    nueva_arista->latencia = latencia;
}

int existe_arista(Vertice *vertice_1, Vertice *vertice_2) {
    Arista *temp_list_adyacencia = vertice_1->lista_adyacencia;

    while(temp_list_adyacencia && temp_list_adyacencia->destino->name != vertice_2->name) { 
        temp_list_adyacencia = temp_list_adyacencia->next;  //avanza al siguiente
    }

    if(!temp_list_adyacencia)   //es NULL por lo tanto no existe la arista
        return 0;

    return 1;
}

void imprimir_grafo(Grafo *grafo) {
    Vertice *temp_list_vertice = grafo->lista_vertices;

    while(temp_list_vertice) {
        printf("\n %s --> ", temp_list_vertice->name);
        Arista *temp_list_adyacencia = temp_list_vertice->lista_adyacencia;

        while(temp_list_adyacencia) {
            printf(" %s <--%d--> ", temp_list_adyacencia->destino->name, temp_list_adyacencia->peso);
            temp_list_adyacencia = temp_list_adyacencia->next;
        }
        temp_list_vertice = temp_list_vertice->next;
    }
}

int buscar_indice(Nodo_aux *nodos, int n, Vertice *v) {
    for (int i = 0; i < n; i++) {
        if (nodos[i].vertice == v)
            return i;
    }
    return -1;
}


Camino* dijkstra(Grafo *grafo, const char *origen_name, char *destino_name) {   //puedo detener el algoritmo en el momento que se encuentra el camino que buscamos

    // Paso 1: contar vértices
    int n = grafo->numero_vertices;
    Vertice *v = grafo->lista_vertices;

    // Paso 2: crear arreglo auxiliar
    Nodo_aux *nodos = malloc(sizeof(Nodo_aux) * n);
    v = grafo->lista_vertices;
    for (int i = 0; i < n; i++) {
        nodos[i].vertice = v;           //es el vertice
        nodos[i].distancia = INT_MAX;       //distancia minima se inicializa en 'infinito'
        nodos[i].visitado = 0;              //no ha sido visitado
        //nodos[i].anterior = NULL;           //aun no hay una referencia a un nodo anterior
        nodos[i].back = NULL;   //NEW FEATURE
        v = v->next;                    //se avanza al siguiente vertice
    }

    // Paso 3: encontrar vértice origen
    int origen_idx = -1;
    for (int i = 0; i < n; i++) {       //se busca el vertice origen en el arreglo nodo de tipo Nodo_aux
        if (strcmp(nodos[i].vertice->name, origen_name) == 0) {     //cuando se encuentra
            origen_idx = i;                 //se guarda la el indice
            nodos[i].distancia = 0;         //su distancia minima es 0
            break;                          //se rompe el ciclo
        }
    }

    if (origen_idx == -1) {             //no se encontro el vertice (esto lo puedo cambiar) //voy a asegurar de que los vertices ya existan en este punto
        printf("Vertice origen no encontrado.\n");
        free(nodos);
        //return;
    }

    // Paso 4: algoritmo principal
    for (int i = 0; i < n; i++) {
        // seleccionar el vértice no visitado con menor distancia
        int u = -1;
        for (int j = 0; j < n; j++) {       //el primero va a ser el vertice inicial
            if (!nodos[j].visitado && (u == -1 || nodos[j].distancia < nodos[u].distancia)) {
                u = j;
            }
        }

        if (nodos[u].distancia == INT_MAX) 
            break; // no quedan accesibles   (representa un vertice no conexo)

        nodos[u].visitado = 1;      //se marca como visitado

        // explorar vecinos
        Arista *arista = nodos[u].vertice->lista_adyacencia;    //lista de adyacencia
        while (arista) {
            if(!arista->caida) { //si la conexion no esta caida
                Vertice *vecino = arista->destino;      //vertice adyacente al vertice u
                int v_idx = buscar_indice(nodos, n, vecino);        //busca el indice del vertice vecino en el array nodos de tipo Nodo_aux
                if (v_idx != -1 && !nodos[v_idx].visitado) {    //si se encuentra y no ha sido visitado
                    int nueva_dist = nodos[u].distancia + arista->peso;//nueva distancia = distancia al nodo u + peso de la arista al vertice vecino

                    if (nueva_dist < nodos[v_idx].distancia) {  //si nueva distancia menor a la distancia para llegar al vertice en nodos
                        nodos[v_idx].distancia = nueva_dist;    //actualizar la distancia para llegar al vertice en el array nodos
                        //nodos[v_idx].anterior = nodos[u].vertice;   //actualiza puntero al vertice anterior (del cual llego)
                        nodos[v_idx].back = &nodos[u];      //NEW FEATURE   
                    }
                }
            }
            arista = arista->next;      //se avanza al siguiente
        }
        if(strcmp(nodos[u].vertice->name, destino_name) == 0) {  //se detiene el algoritmo si ya se encontro el camino mas corto a el vertice destino
            // Ejemplo: mostrar resultados
            printf("Distancias desde %s:\n", origen_name);  //esto se va a quitar
            for (int i = 0; i < n; i++) {
                printf("A %s: %d\n", nodos[i].vertice->name,
                nodos[i].distancia == INT_MAX ? -1 : nodos[i].distancia);
            }

            int vd_idx = buscar_indice(nodos, n, buscar_vertice(grafo, destino_name));  
            Camino *nuevo_camino = construir_camino(nodos, vd_idx, n);
            free(nodos);    //se libera el arreglo
            return nuevo_camino;        //se regresa el camino generado
        }
    }  

    
    //tengo que pensar en como saber si se puede generar un camino al vertice_destino
    //int vd_idx = buscar_indice(nodos, n, buscar_vertice(grafo, destino_name));
    //Camino *nuevo_camino = construir_camino(nodos, vd_idx, n);

    free(nodos);        //en algun momento debo liberarlo
    return NULL;    //no se genero ningun camino
}

Camino *crear_camino()
{
    Camino *nuevo_camino = (Camino*)malloc(sizeof(Camino));

    if(!nuevo_camino) 
        return NULL;

    nuevo_camino->head = NULL;
    return nuevo_camino;
}

Camino *construir_camino(Nodo_aux *nodos, int vf_idx, int n)   
{
    Camino *nuevo_camino = crear_camino();
    Nodo_camino *v_camino = (Nodo_camino*)malloc(sizeof(Nodo_camino));
    if(!v_camino || !nuevo_camino) {
        printf("\n\tError: no se asigno memoria correctamente");
        return NULL;
    }
    v_camino->vertice = nodos[vf_idx].vertice;
    v_camino->next = NULL;

    v_camino->arista_adyacente_1 = NULL;
    v_camino->arista_adyacente_2 = NULL;

    nuevo_camino->head = v_camino;

    //int vb_idx = vf_idx;
    while(nodos[vf_idx].back != NULL) 
    {
        int vb_idx = buscar_indice(nodos, n, nodos[vb_idx].back->vertice);

        Nodo_camino *vertice_camino = (Nodo_camino*)malloc(sizeof(Nodo_camino));
        if(!vertice_camino) {
            printf("\n\tError: no se asigno memoria correctamente");
            free(nuevo_camino);
            return NULL;
        }

        vertice_camino->vertice = nodos[vb_idx].vertice;

        vertice_camino->arista_adyacente_1 = encontrar_arista(nodos[vb_idx].vertice, nodos[vf_idx].vertice);  //Ojo aqui
        vertice_camino->arista_adyacente_2 = encontrar_arista(nodos[vf_idx].vertice, nodos[vb_idx].vertice);

        vertice_camino->next = nuevo_camino->head;
        nuevo_camino->head = vertice_camino;
        
        vf_idx = vb_idx;
    }
    return nuevo_camino;
}

Arista *encontrar_arista(Vertice *inicio, Vertice *destino) {//no necesito el grafo realmente

    Arista *temporal_arista = inicio->lista_adyacencia;

    if(!destino)
        return NULL;

    while(temporal_arista && temporal_arista->destino != destino) {
        temporal_arista = temporal_arista->next;
    }

    if(!temporal_arista)
        return NULL;

    return temporal_arista;
}

void *ajustar_pesos(void *arg) {
    Camino *camino = (Camino *)arg; //casting
    Nodo_camino *temporal = camino->head;
    
    //int peso_total = 0;

    //imprimir_camino_archivo(camino);
    while(temporal && temporal->arista_adyacente_1) { //temporal->caida != true     ??
        Arista *a1 = temporal->arista_adyacente_1;
        Arista *a2 = temporal->arista_adyacente_2;

        //aqui podria haber un if para que no entre si la conexion esta caida
        if(a1->caida) { //si la conexion esta caida rompe el ciclo directamente
            imprimir_camino_archivo(camino, "caminos_no_completados.txt");
            free(camino);
            return NULL;  //aqui es un return
        }
        //se estandariza el orden de bloqueo de los mutex para evitar deadlocks
        if(a1 < a2) {
            pthread_mutex_lock(&a1->mutex);
            pthread_mutex_lock(&a2->mutex);
        }
        else {
            pthread_mutex_lock(&a2->mutex);
            pthread_mutex_lock(&a1->mutex);
        }
        temporal->arista_adyacente_1->peso += camino->peso;
        temporal->arista_adyacente_2->peso += camino->peso;

        //aqui tendria que hacer algo por si se supera el limite
        if(temporal->arista_adyacente_1->peso > temporal->arista_adyacente_1->peso_max) {   //si se supera al peso en la suma anterior ó en otro hilo
            printf("\n\tConexion %s - %s caida", temporal->vertice->name, temporal->next->vertice->name);
            
            temporal->arista_adyacente_1->peso -= camino->peso;
            temporal->arista_adyacente_2->peso -= camino->peso;
            
            temporal->arista_adyacente_1->caida = true; //ahora estan caidas
            temporal->arista_adyacente_2->caida = true;
            pthread_mutex_unlock(&a1->mutex);   //se desbloquean los mutex
            pthread_mutex_unlock(&a2->mutex);

            imprimir_camino_archivo(camino, "caminos_no_completados.txt");
            free(camino);

            return NULL;  //ya no continua el ciclo       aqui es un return
            //debe haber algo que diferencie si se completo el camino o no (para mostrarlo)
        }

        pthread_mutex_unlock(&a1->mutex);   //se desbloquean los mutex
        pthread_mutex_unlock(&a2->mutex);

        sleep(temporal->arista_adyacente_1->latencia);  //se pone a dormir al hilo (funciona en segundos)

        if(a1 < a2) {   //podria ser una funcion??
            pthread_mutex_lock(&a1->mutex);
            pthread_mutex_lock(&a2->mutex);
        }
        else {
            pthread_mutex_lock(&a2->mutex);
            pthread_mutex_lock(&a1->mutex);
        }
        temporal->arista_adyacente_1->peso -= 10;
        temporal->arista_adyacente_2->peso -= 10;
        pthread_mutex_unlock(&a1->mutex);   //se desbloquean los mutex
        pthread_mutex_unlock(&a2->mutex);


        //peso_total += temporal->arista_adyacente_1->peso;
        temporal = temporal->next;
    }
    //imprimir_camino_archivo(camino);
    //si llega hasta aqui es que no hubo problemas con ninguna conexion del camino
    //antes guardar el camino en una estructura o archivo
    //antes de liberar el camino lo imprimo en el archivo
    
    imprimir_camino_archivo(camino, "caminos.txt"); //se imprime el camino en caminos porque se completo el recorrido
    free(camino);   //se libera el camino

    return NULL;
}

void prueba_hilos(Grafo *grafo)
{
    int opc;
    
    printf("\n\t**Prueba de hilos**");
    do
    {
        printf("\n\tCamino de pc00 a pc08:\n");
        Camino *camino = dijkstra(grafo, "pc00", "pc08");
        if(!camino)
            printf("\n\tError: conexiones caidas, no hay forma de transmitir los datos");
        else {
            imprimir_camino(camino);    //esto va dentro del thread
            pthread_t thread;
            
            pthread_create(&thread, NULL, ajustar_pesos, camino);
            pthread_detach(thread);
        }
        printf("\n\tSalir?(1-si/2-no): ");
        scanf("%d", &opc);
    }while(opc == 2);
    
}

void imprimir_camino(Camino *camino)
{
    Nodo_camino *temporal = camino->head;

    //printf("\n\tCamino:\n");
    while(temporal)
    {
        printf(" %s --- ", temporal->vertice->name);
        temporal = temporal->next;
    }
}

//que pasa si restablezco la misma conexion 2 veces (podria usar una cola de prioridad)
void *levantar_conexion(void *arg) {    //Deberia haber un mutex aqui??
    Reconexion *reconexion = (Reconexion *)arg; //casting
    Vertice *vertice_1 = buscar_vertice(reconexion->grafo, reconexion->vertice_1);
    Vertice *vertice_2 = buscar_vertice(reconexion->grafo, reconexion->vertice_2);

    if(!vertice_1 || !vertice_2) {
        return NULL;
    }

    Arista *conexion_1 = encontrar_arista(vertice_1, vertice_2);
    Arista *conexion_2 = encontrar_arista(vertice_2, vertice_1);
    if(!conexion_1) {
        return NULL;
    }

    if(conexion_1->caida) {   //caso en que la conexion no esta caida
        return NULL;
    }

    //tengo que hacer que se tome un tiempo antes de que se levante la conexion 
    //probablemente tenga que ser una formula para obtener el tiempo necesario para que el peso de la conexion quede en su base
    //para eso creo que tendira que almacenar su peso original
    //si almaceno su peso original ya no es necesario obtener una formula
    sleep(70);  //este sleep es para que le de tiempo a los hilos de restar lo que sumaron al peso de la arista

    if(conexion_1) {    //esta condicion es necesaria porque el usuario puede ejecutar esta funcion 2 veces seguidas
        conexion_1->caida = false;
        conexion_2->caida = false;
        return NULL;
    }
    return NULL;
}

Grafo *generar_topologia()
{
    Grafo *grafo = crear_grafo();

    //routers
    agregar_vertice(grafo, "rt00");    
    agregar_vertice(grafo, "rt01"); 
    agregar_vertice(grafo, "rt02");
    agregar_vertice(grafo, "rt03");
    agregar_vertice(grafo, "rt04");

    //switches
    agregar_vertice(grafo, "sw00");
    agregar_vertice(grafo, "sw01");
    agregar_vertice(grafo, "sw02");

    //computadoras
    agregar_vertice(grafo, "pc00");
    agregar_vertice(grafo, "pc01");
    agregar_vertice(grafo, "pc02");
    agregar_vertice(grafo, "pc03");
    agregar_vertice(grafo, "pc04");
    agregar_vertice(grafo, "pc05");
    agregar_vertice(grafo, "pc06");
    agregar_vertice(grafo, "pc07");
    agregar_vertice(grafo, "pc08");

    //routers a routers
    agregar_arista(grafo, "rt00", "rt01", 10, 100, 20);
    agregar_arista(grafo, "rt00", "rt02", 10, 100, 20);
    agregar_arista(grafo, "rt00", "rt03", 10, 100, 20);
    agregar_arista(grafo, "rt00", "rt04", 10, 100, 20);
    agregar_arista(grafo, "rt01", "rt02", 10, 100, 20);
    agregar_arista(grafo, "rt01", "rt03", 10, 100, 20);
    agregar_arista(grafo, "rt01", "rt04", 10, 100, 20);
    agregar_arista(grafo, "rt02", "rt03", 10, 100, 20);
    agregar_arista(grafo, "rt02", "rt04", 10, 100, 20);
    agregar_arista(grafo, "rt03", "rt04", 10, 100, 20);

    //routers a switches
    agregar_arista(grafo, "rt00", "sw00", 10, 100, 0);
    agregar_arista(grafo, "rt01", "sw01", 10, 100, 0);
    agregar_arista(grafo, "rt02", "sw02", 10, 100, 0);

    //switches a computadoras
    agregar_arista(grafo, "sw00", "pc00", 10, 100, 0);
    agregar_arista(grafo, "sw00", "pc01", 10, 100, 0);
    agregar_arista(grafo, "sw00", "pc02", 10, 100, 0);
    agregar_arista(grafo, "sw01", "pc03", 10, 100, 0);
    agregar_arista(grafo, "sw01", "pc04", 10, 100, 0);
    agregar_arista(grafo, "sw01", "pc05", 10, 100, 0);
    agregar_arista(grafo, "sw02", "pc06", 10, 100, 0);
    agregar_arista(grafo, "sw02", "pc07", 10, 100, 0);
    agregar_arista(grafo, "sw02", "pc08", 10, 100, 0);
    return grafo;
    
}

void imprimir_camino_archivo(Camino *camino, char *nombre_archivo) {
    FILE *archivo = fopen(nombre_archivo, "a");  // modo append (agrega al final)
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo");
        return;
    }

    Nodo_camino *temporal = camino->head;
    while (temporal) {
        fprintf(archivo, "%s", temporal->vertice->name);
        if (temporal->next != NULL) {
            fprintf(archivo, " --- ");  // separador entre vértices
        }
        temporal = temporal->next;
    }

    fprintf(archivo, "\n");  // termina la línea del camino
    fclose(archivo);         // muy importante cerrar el archivo
}

void menu()
{
    int opc;
    Grafo *grafo = generar_topologia();
    char vertice_inicio[5], vertice_destino[5], aux[15];
    Vertice *vertice_1, *vertice_2;
    Reconexion *conexion_caida = malloc(sizeof(Reconexion));
    

    do
    {
        printf("\n\tMENU");
        printf("\n\t1-Enivar informacion"); 
        printf("\n\t2-Levantar conexion");      //levanta una conexion dados dos vertices
        printf("\n\t3-Caminos completados");    //imprime todos los caminos completados
        printf("\n\t4-Caminos no completados");     //imprime todos los caminos no completados    
        printf("\n\t5-Salir");
        printf("\nSelecciona una opcion: ");
        validar_entero(aux, &opc);

        switch(opc)
        {
            case 1:
                
                int peso_formato;

                printf("\n\tIngresa el nombre del equipo de inicio: ");
                fgets(vertice_inicio, 5, stdin);
                vertice_inicio[strcspn(vertice_inicio, "\n")] = '\0'; //quita el salto de linea

                getchar();
                printf("\n\tIngresa el nombre del equipo destino: ");
                fgets(vertice_destino, 5, stdin);
                vertice_destino[strcspn(vertice_destino, "\n")] = '\0'; //quita el salto de linea

                if(!validacion_equipos(vertice_inicio) || !validacion_equipos(vertice_destino))
                {
                    printf("\n\tError: el nombre del equipo debe ser PC##");    //a donde deberia ir a partir de aqui??
                    break;
                }
                    
                
                vertice_1 = buscar_vertice(grafo, vertice_inicio);
                vertice_2 = buscar_vertice(grafo, vertice_destino);
                if(!vertice_1 || !vertice_2)
                {
                    printf("\n\tError: no se encontro uno o ambos equipos");
                    printf("%s\n", vertice_inicio);
                    printf("%s", vertice_destino);
                    break;
                }

                printf("\n\tQue deseas enviar?");   //esto todavia no funciona
                printf("\n\t1-Mensaje");
                printf("\n\t2-Archivo");
                printf("\n\t3-Video");
                printf("\n\tSelecciona una opcion: ");
                getchar();
                validar_entero(aux, &peso_formato);

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

                Camino *camino = dijkstra(grafo, vertice_inicio, vertice_destino);
                camino->peso = peso_formato;
                if(!camino)
                    printf("\n\tError: conexiones caidas, no hay forma de transmitir los datos");
                else {
                    imprimir_camino(camino);    //esto va dentro del thread
                    pthread_t thread;
                    
                    pthread_create(&thread, NULL, ajustar_pesos, camino);
                    pthread_detach(thread);
                }
                
                break;
            
            case 2:
                
                printf("\n\tIngresa el nombre del equipo 1: ");
                fgets(vertice_inicio, 5, stdin);
                vertice_inicio[strcspn(vertice_inicio, "\n")] = '\0'; //quita el salto de linea

                getchar();
                printf("\n\tIngresa el nombre del equipo 2: ");
                fgets(vertice_destino, 5, stdin);
                vertice_destino[strcspn(vertice_destino, "\n")] = '\0'; //quita el salto de linea

                vertice_1 = buscar_vertice(grafo, vertice_inicio);
                vertice_2 = buscar_vertice(grafo, vertice_destino);
                if(!vertice_1 || !vertice_2)
                {
                    printf("\n\tError: no se encontro uno o ambos equipos");
                    break;
                }
                Arista *arista = encontrar_arista(vertice_1, vertice_2);

                if(!arista)
                {
                    printf("\n\tError: no existe conexion entre esos equipos");
                    break;
                }
                if(!arista->caida)
                {
                    printf("\n\tError: la conexion entre esos equipos no esta caida");
                    break;
                }
                conexion_caida->grafo = grafo;
                strcpy(vertice_inicio, conexion_caida->vertice_1);
                strcpy(vertice_destino, conexion_caida->vertice_2);

                printf("\n\tRestableciendo conexion...");
                pthread_t thread;
                    
                pthread_create(&thread, NULL, levantar_conexion, conexion_caida);
                pthread_detach(thread);

                break;
            
            case 3:
                printf("\n\tCaminos completados hasta el momento:\n");
                leer_caminos_archivo("caminos.txt");
                break;

            case 4:
                printf("\n\tCaminos no completados por caidas de conexion:\n");
                leer_caminos_archivo("caminos_no_completados.txt");
                break;

            case 5:
                printf("\n\tFin del programa");
                break;
                
            default:
                printf("\n\tError: no existe esa opcion");
                break;

        }
        printf("\n\t--- Presiona enter para continuar ---");
        limpiar_buffer();

    }while(opc != 5);
    
    
    free(conexion_caida);

}

void limpiar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int validacion_equipos(char *vertice) {
    //los vertices debe seguir el formato PC##

    if(strlen(vertice) != 4) //se valida que la longitud sea correcta 
        return 0;

    if(vertice[0] != 'p' && vertice[1] != 'c')  //los primeros dos caracteres deben ser P y C
        return 0;
    
    if(!isdigit(vertice[2]) || !isdigit(vertice[3]))   //los siguientes dos digitos deben ser numeros
        return 0;

    return 1;
    
}

void leer_caminos_archivo(char *nombre_archivo) {
    FILE *archivo = fopen(nombre_archivo, "r");  // modo lectura
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo\n");
        return;
    }

    char linea[256];  // asume que cada línea tiene como máximo 255 caracteres
    printf("Contenido del archivo:\n");

    while (fgets(linea, sizeof(linea), archivo)) {
        printf("%s", linea);  // ya incluye el salto de línea
    }

    fclose(archivo);  // muy importante cerrar el archivo
}

void limpiar_pantalla() 
{
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void validar_entero(char *aux, int *num) {
    int y, p, i;

    do {
        //printf(": ");
        fgets(aux, 50, stdin);
        aux[strcspn(aux, "\n")] = 0; // Eliminar el salto de línea

        y = strlen(aux);
        p = 1;  // Se asume válido hasta que se pruebe lo contrario

        if (y == 0) {
            p = 0; // Evita que se acepten cadenas vacías
        } else {
            for (i = 0; i < y; i++) {
                if (!isdigit(aux[i])) {  // Si encuentra un caracter no numérico
                    p = 0;
                    break;
                }
            }
        }

        if (p == 0) {
            printf("\n\n Error, ingresa un entero\n\n");
        }
    } while (p == 0);

    *num = atoi(aux);
    //printf("\n\n\n\t Valor leido: %d\n", *num);
}