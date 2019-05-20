/* Práctica de Recuperación: Computación Concurrente
*	Muñiz Patiño, Andrea Fernanda
*	No. de cuenta 311175318
* 	UNAM, Facultad de Ciencias.
* 	18/Mayo/2019
*	Problema Lectores - Escritores
*	<< Número de lectores >> << Número de escritores >>
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>
//Para el uso de semáforos
#include <semaphore.h>

#define CONSTANTE 4; //Constante que determina el número de iteraciones por hilo

//Declaración de los semáforos 

sem_t base; //Semáforo para el escritor
sem_t mutex; //Semáforo general

int contador = 0;	


/*
* Métodos que simulan las acciones de los dos tipos de usuario. 
*/

void leer(int id){
	printf("El hilo %i esta leyendo la base de datos..\n", id);
}

void escribir(int id){
	printf("-> El hilo %i escribiendo en la base de datos\n", id);
}

//Métodos que resuelven la sincronización

void lector(int id){
	//Obtenemos acceso exclusivo a la variable contador
	int idH = id;
	sem_wait(&mutex);
		contador++;
		if(contador ==1){
			sem_wait(&base);
		}
	sem_post(&mutex);
	//Leyendo la base de datos
	leer(idH);

	sem_wait(&mutex);
		contador--;
		if(contador == 0){
			sem_post(&base);
		}
	sem_post(&mutex);

}

void escritor(int id){
	sem_wait(&base);
		escribir(id);
	sem_post(&base);	

}


int main(int argc, char** argv){

if (argc < 3){
		printf("No especificaste todos los parametros.\n");
		exit(1);
	}


	int lectores = atoi(argv[1]);
	int escritores = atoi(argv[2]);

	//Se generan tantos hilos como lectores y escritores queramos.
	int numHilos = lectores + escritores; 
	
	int iteraciones = CONSTANTE;


	//------------------Inicialización de semáforos-------------------
	sem_init(&base, 1, 1);
	sem_init(&mutex, 1,1);


	/*---------------Usando OMP--------------------------------------*/

	//Se lanzan los hilos que el usuario indico
	omp_set_num_threads(numHilos);
	
	int id_Hilo;

	#pragma omp parallel private (id_Hilo)
	{
		for(int i =0; i<iteraciones; i++){		
				id_Hilo = omp_get_thread_num();

				/* Esta parte del código determina que papel juega el hilo
				*	Ya sea Lector o Escritor. 
				*/								
					if ((id_Hilo >= 0)&&(id_Hilo < lectores)){
						lector(id_Hilo);
					}else{
						escritor(id_Hilo);
					}
		}//END FOR

		//---Requisito de la práctica---
		printf("** El hilo %i ha iterado %i veces ** \n", id_Hilo, iteraciones);	
		//---Requisito de la práctica---

	}//END Parallel

	printf("Se termina la ejecución de todos los hilos\n");	

	sem_destroy(&base);
	sem_destroy(&mutex);

}//END MAIN