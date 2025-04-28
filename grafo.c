//Este archivo contendra el TDA grafo y sus operaciones necesarias para el programa
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>    //para hilos
#include <unistd.h>     //para la funcion usleep

//se pueden agregar mas campos a los TDA
struct Vertice;

typedef struct Arista{
    int peso;       //peso de la arista
    struct Vertice *destino;   //vertice al que conduce esa arista
    pthread_mutex_t mutex;
    struct Arista *next;        //siguiente arista incidente al vertice actual
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
} Camino;


//crea un grafo y devuelve un puntero a este
Grafo *crear_grafo();

//agrega un vertice al grafo
void agregar_vertice(Grafo *, char *);

//crea un vertice y devuelve un puntero a este
Vertice *crear_vertice(char *);

//busca un vertice dado su nombre y regresa un puntero a este
Vertice *buscar_vertice(Grafo *, char *);

//conecta 2 vertices del grafo con una arista
void agregar_arista(Grafo *, char *, char *, int);

//crea una arista dados dos extremos
void crear_arista(int, Vertice *, Vertice *);

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

//grafo en base a la topologia elegida
Grafo *generar_topologia();

int main() {
    //Grafo *grafo = crear_grafo();

    /*
    agregar_vertice(grafo, "cp0");    
    agregar_vertice(grafo, "sw0"); 
 //   agregar_vertice(grafo, "cp0");   
    agregar_vertice(grafo, "rt0");
    agregar_vertice(grafo, "cp1");
    agregar_vertice(grafo, "rt1");

    agregar_arista(grafo, "cp0", "sw0", 10);
    agregar_arista(grafo, "cp0", "rt0", 5);
   // agregar_arista(grafo, "cp0", "sw0", 10);
    agregar_arista(grafo, "sw0", "rt0", 15);
    agregar_arista(grafo, "cp1", "sw0", 20);
    agregar_arista(grafo, "rt0", "rt1", 25);
    
    imprimir_grafo(grafo);

    printf("\n\n");
    Camino *camino = dijkstra(grafo, "sw0", "rt1");
    Nodo_camino *temporal = camino->head;
    
    int suma_peso = 0;
    printf("\n\tCamino de sw0 a rt0:\n\n");
    while(temporal)
    {
        printf(" %s --- ", temporal->vertice->name);

        if(temporal->arista_adyacente_1 != NULL)
            suma_peso += temporal->arista_adyacente_1->peso;

        temporal = temporal->next;
    }

    printf("\n\tEl peso del camino es: %d", suma_peso);

    
    pthread_t threads[2];

    for(int i = 0; i < 2; i++) {
        pthread_create(&threads[i], NULL, ajustar_pesos, camino);
    } 

    printf("\n\tHola como estas?");
    
    for(int i = 0; i < 2; i++) {
        pthread_detach(threads[i]);
    }*/
   Grafo *grafo = generar_topologia();
   
   prueba_hilos(grafo);
    
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


void agregar_arista(Grafo *grafo, char *inicio, char *destino, int peso) {
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
    crear_arista(peso, vertice_inicial, vertice_destino);   
    crear_arista(peso, vertice_destino, vertice_inicial);
    
}

void crear_arista(int peso, Vertice *vertice_inicial, Vertice *vertice_destino) {
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
            arista = arista->next;      //se avanza al siguiente
        }
        if(strcmp(nodos[u].vertice->name, destino_name) == 0)   //se detiene el algoritmo si ya se encontro el camino mas corto a el vertice destino
            break;
    }  

    // Ejemplo: mostrar resultados
    printf("Distancias desde %s:\n", origen_name);  //esto se va a quitar
    for (int i = 0; i < n; i++) {
        printf("A %s: %d\n", nodos[i].vertice->name,
               nodos[i].distancia == INT_MAX ? -1 : nodos[i].distancia);
    }
    int vd_idx = buscar_indice(nodos, n, buscar_vertice(grafo, destino_name));
    Camino *nuevo_camino = construir_camino(nodos, vd_idx, n);

    free(nodos);        //en algun momento debo liberarlo
    return nuevo_camino;
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

    while(temporal && temporal->arista_adyacente_1) {
        Arista *a1 = temporal->arista_adyacente_1;
        Arista *a2 = temporal->arista_adyacente_2;

        //se estandariza el orden de bloqueo de los mutex para evitar deadlocks
        if(a1 < a2) {
            pthread_mutex_lock(&a1->mutex);
            pthread_mutex_lock(&a2->mutex);
        }
        else {
            pthread_mutex_lock(&a2->mutex);
            pthread_mutex_lock(&a1->mutex);
        }
        temporal->arista_adyacente_1->peso += 10;
        temporal->arista_adyacente_2->peso += 10;
        pthread_mutex_unlock(&a1->mutex);   //se desbloquean los mutex
        pthread_mutex_unlock(&a2->mutex);

        //peso_total += temporal->arista_adyacente_1->peso;
        temporal = temporal->next;
    }
    //printf("\n\tPeso del camino luego del ajuste: %d", peso_total);
    sleep(20);  //funciona en segundos

    
    temporal = camino->head;
    //peso_total = 0;
    while(temporal && temporal->arista_adyacente_1) {
        Arista *a1 = temporal->arista_adyacente_1;
        Arista *a2 = temporal->arista_adyacente_2;

        //se estandariza el orden de bloqueo de los mutex para evitar deadlocks
        if(a1 < a2) {
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
    //printf("\n\tPeso del camino luego de transcurrir el tiempo: %d", peso_total);
    free(camino);   //se libera el camino

    return NULL;
}

void prueba_hilos(Grafo *grafo)
{
    int opc;
    
    printf("\n\t**Prueba de hilos**");
    do
    {
        printf("\n\tCamino de rt0 a rt4:\n");
        Camino *camino = dijkstra(grafo, "rt00", "rt04");
        imprimir_camino(camino);
        pthread_t thread;
        
        pthread_create(&thread, NULL, ajustar_pesos, camino);
        pthread_detach(thread);

        printf("\n\tSalir?(1-si/2-no): ");
        scanf("%d", &opc);
    }while(opc == 2);
    
}

void imprimir_camino(Camino *camino)
{
    Nodo_camino *temporal = camino->head;

    printf("\n\tCamino:\n");
    while(temporal)
    {
        printf(" %s --- ", temporal->vertice->name);
        temporal = temporal->next;
    }
}

Grafo *generar_topologia()
{
    Grafo *grafo = crear_grafo();

    agregar_vertice(grafo, "rt00");    
    agregar_vertice(grafo, "rt01"); 
 //   agregar_vertice(grafo, "cp0");   
    agregar_vertice(grafo, "rt02");
    agregar_vertice(grafo, "rt03");
    agregar_vertice(grafo, "rt04");

    agregar_arista(grafo, "rt00", "rt01", 10);
    agregar_arista(grafo, "rt00", "rt02", 10);
    agregar_arista(grafo, "rt00", "rt03", 10);
    agregar_arista(grafo, "rt00", "rt04", 10);
    agregar_arista(grafo, "rt01", "rt02", 10);
    agregar_arista(grafo, "rt01", "rt03", 10);
    agregar_arista(grafo, "rt01", "rt04", 10);
    agregar_arista(grafo, "rt02", "rt03", 10);
    agregar_arista(grafo, "rt02", "rt04", 10);
    agregar_arista(grafo, "rt03", "rt04", 10);

    return grafo;
    
}