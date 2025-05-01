#ifndef Grafo_H
#define Grafo_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>    //para hilos
#include <unistd.h>     //para la funcion usleep

//se pueden agregar mas campos a los TDA
struct Vertice;

// **TDA GRAFO**
typedef struct Arista{
    int peso;       //peso de la arista
    struct Vertice *destino;   //vertice al que conduce esa arista
    pthread_mutex_t mutex;
    struct Arista *next;        //siguiente arista incidente al vertice actual
} Arista;

typedef struct Vertice{
    char name[4];      //nombre del vertice    (el nombre tendra que cambiar a algo mas descriptivo ó agregar un campo extra)
    Arista *lista_adyacencia;       //lista de las aristas adyacentes
    struct Vertice *next;      //siguiente vertice del grafo en general
} Vertice;

typedef struct Grafo{
    Vertice *lista_vertices;    //lista que contiene todos los vertices (es la cabeza)
    int numero_vertices;        //numero de vertices del grafo
} Grafo;



// **ESTRUCTURA PARA REALIZAR DIJKSTRA**
typedef struct Nodo_aux {   //estructura para dijkstra
    Vertice *vertice;     // referencia al vértice original
    int distancia;        // distancia mínima conocida
    int visitado;         // si ya fue visitado
    //Vertice *anterior;    // para reconstruir el camino
    struct Nodo_aux *back;  //para reconstruir el camino
} Nodo_aux;



// **ESTRUCTURAS PARA ALMACENAR EL CAMINO MAS CORTO**
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
void imprimir_grafo(Grafo *);   //ESTA PROBABLEMENTE NO VA

//dijkstra
int buscar_indice(Nodo_aux *, int n, Vertice *);

Camino *dijkstra(Grafo *, const char *, char *);

void imprimir_camino(Camino *);

//camino
Camino *crear_camino();

Camino *construir_camino(Nodo_aux *, int, int);    

Arista *encontrar_arista(Vertice *, Vertice *);

//Hilos
void *ajustar_pesos(void *arg);

//prueba de hilos
void prueba_hilos(Grafo *);

//grafo en base a la topologia elegida
Grafo *generar_topologia();

#endif