//Este archivo contendra el TDA grafo y sus operaciones necesarias para el programa
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//se pueden agregar mas campos a los TDA
struct Vertice;

typedef struct Arista{
    int peso;       //peso de la arista
    struct Vertice *destino;   //vertice al que conduce esa arista
    struct Arista *next;        //siguiente arista incidente al vertice actual
} Arista;

typedef struct Vertice{
    char name[4];      //nombre del vertice    (el nombre tendra que cambiar a algo mas descriptivo ó agregar un campo extra)
    Arista *lista_adyacencia;       //lista de las aristas adyacentes
    struct Vertice *next;      //siguiente vertice del grafo en general
} Vertice;

typedef struct Grafo{
    Vertice *lista_vertices;    //lista que contiene todos los vertices (es la cabeza)
} Grafo;

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

int main() {
    Grafo *grafo = crear_grafo();

    agregar_vertice(grafo, "cp0");    //validar que no exista el vertice
    agregar_vertice(grafo, "sw0"); 
    agregar_vertice(grafo, "cp0");   
    agregar_vertice(grafo, "rt0");
    agregar_vertice(grafo, "cp1");

    agregar_arista(grafo, "cp0", "sw0", 10);
    agregar_arista(grafo, "cp0", "rt0", 5);
    agregar_arista(grafo, "cp0", "sw0", 10);
    agregar_arista(grafo, "sw0", "rt0", 15);
    agregar_arista(grafo, "cp1", "sw0", 20);
    
    imprimir_grafo(grafo);
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