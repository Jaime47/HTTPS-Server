/**
  * @author Jaime Pons Garrido
  * @author Federico Perez Fernandez
  * @file verbs.c
  * @date 11 Mar 2021
  * @brief
  */
/* --- Defines --- */
#ifndef _my_lock
#include <unistd.h>
#include <sys/mman.h>
#include <pthread.h>
#include <fcntl.h>
#define _my_lock

/**
  * @brief Inicializa un semaforo.
  *
  * @param 
  *
  * @return 
  */
void my_lock_init();
/**
  * @brief Hace que el semaforo inicializado en my_lock_init espere.
  *
  * @param 
  *
  * @return 
  */
void my_lock_wait();
/**
  * @brief Libera el semaforo inicializado en my_lock_init
  *
  * @param 
  *
  * @return 
  */
void my_lock_release();

#endif