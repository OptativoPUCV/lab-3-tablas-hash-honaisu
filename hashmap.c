#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
    Pair* nuevoDato = createPair(key, value) ;
    long posicion = hash(key, map->capacity) ;
    //---//
    if (map->size >= map->capacity * 0.7) enlarge(map) ;
    //---//
    while (map->buckets[posicion] != NULL) {
        if (is_equal(map->buckets[posicion]->key, key)) return ;
        posicion = (posicion + 1)%(map->capacity) ;
    }
    //---//
    map->buckets[posicion] = nuevoDato ;
    map->current = posicion ;
    map->size++ ;
}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)

    Pair** punteroPares = map->buckets ; // antiguo copia
    long capacidadAnterior = map->capacity ;
    //---//
    map->capacity *= 2 ; // Nueva Capacidad
    //---//
    Pair** nuevoBuckets = (Pair**) malloc(map->capacity * sizeof(Pair*)) ;
    if (nuevoBuckets == NULL) {
        map->capacity = capacidadAnterior ;
        return ;
    }
    //---//
    for (int i = 0 ; i < map->capacity ; i++) {
        nuevoBuckets[i] = NULL ;
    }
    //---//
    map->buckets = nuevoBuckets ;
    map->size = 0 ;
    for (int i = 0 ; i < capacidadAnterior ; i++) {
        if (punteroPares[i] != NULL && punteroPares[i]->key != NULL) {
            insertMap(map, punteroPares[i]->key, punteroPares[i]->value) ;
        }
        
    }
    free(punteroPares) ;
}


HashMap * createMap(long capacity) {
    HashMap* mapa = (HashMap*) malloc(sizeof(HashMap)) ;
    if (mapa == NULL) return NULL ;
    //---//
    Pair** pares = (Pair**) malloc(capacity * sizeof(Pair*)) ;
    if (pares == NULL) return NULL ;
    for (long i = 0 ; i < capacity ; i++) {
        pares[i] = NULL ;
    } 
    //---//
    mapa->buckets = pares ;
    mapa->capacity = capacity ;
    mapa->size = 0 ;
    mapa->current = -1 ;
    return mapa ;
}

void eraseMap(HashMap * map,  char * key) {    
    long posicion = hash(key, map->capacity) ;
    //---//
    while (map->buckets[posicion] != NULL) {
        if (is_equal(map->buckets[posicion]->key, key)) {
            map->buckets[posicion]->key = NULL;
            map->size-- ;
            return ;
        }
        posicion = (posicion + 1)%(map->capacity) ;
    }
    //---//
    return ;
}

Pair * searchMap(HashMap * map,  char * key) {   
    long posicion = hash(key, map->capacity) ;
    //---//
    while (map->buckets[posicion] != NULL) {
        if (map->buckets[posicion]->key == NULL) return NULL ;
        else if (is_equal(map->buckets[posicion]->key, key)) break ;
        posicion = (posicion + 1)%(map->capacity) ;
    }
    //---//
    map->current = posicion ;
    return map->buckets[posicion] ;
}

Pair * firstMap(HashMap * map) {
    for (long i = 0 ; i < map->capacity ; i++) {
        if (map->buckets[i] == NULL || map->buckets[i]->key == NULL) continue ;
        map->current = i ;
        return map->buckets[i] ;
    }
    return NULL;
}

Pair * nextMap(HashMap * map) {
    for (long i = map->current + 1 ; i < map->capacity ; i++) {
        if (map->buckets[i] == NULL || map->buckets[i]->key == NULL) continue ;
        map->current = i ;
        return map->buckets[i] ;
    }
    return NULL;
}
