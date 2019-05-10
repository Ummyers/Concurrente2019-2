/* Práctica 03: Computación Concurrente
*	Muñiz Patiño, Andrea Fernanda
*	No. de cuenta 311175318
* 	UNAM, Facultad de Ciencias.
* 	23/Abril/2019
*	Problema Productor Consumidor
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>
//Para el uso de semáforos
#include <semaphore.h>

#define CONSTANTE 2; //Constante que determina el número de iteraciones a relizar el procedimiento

#define TAM 100; //Es el tamaño del buffer


//Declaración de semáforos a utilizar.
sem_t mutex;
//El semáforo mutex controla el acceso a la sección critica. Justificación en el PDF
sem_t vacias;
sem_t llenas; 

int tam_buffer=0;

void producir(){
	tam_buffer = tam_buffer + 1;
}

void consumir(){
	tam_buffer = tam_buffer - 1;
}

void consumidor(int id){
	//int value;
		//sem_getvalue(&llenas, &value);
		sem_wait(&llenas);
			sem_wait(&mutex);
				//sem_getvalue(&llenas, &value);
				printf("El hilo %i logró consumir\n",id);
				consumir();
				sem_post(&mutex);
		sem_post(&vacias);
}

void productor(int id){
	int valor;
		//sem_getvalue(&vacias, &valor);
	
		sem_wait(&vacias);
		sem_wait(&mutex);
	
			sem_getvalue(&vacias, &valor);

			//---Requisito de la práctica---
			printf("El hilo %i logró producir\n",id);
			producir();	
			//---Requisito de la práctica---
			
		sem_post(&mutex);
		sem_post(&llenas);

}

int main(int argc, char** argv){

//Se verifica que indiquen los parametros necesarios.
	if (argc < 3){
		printf("No especificaste todos los parametros.\n");
		exit(1);
	}
	
	// Como primer parámetro se recibira el número de productores
	// Como segundo parámetro se recibira el número de consumidores
	int productores = atoi(argv[1]);
	int consumidores = atoi(argv[2]);

	//Se generan tantos hilos como productores y consumidores tengamos.
	int numHilos = productores + consumidores;
	int tam_bufferSEM = TAM;
	int iteraciones = CONSTANTE;
	
	printf("Iteraciones solicitadas por hilo: %i \n", iteraciones);

	printf("Tenemos %i productores y %i consumidores.\nEl tamaño del buffer es: %i \n", productores, consumidores,tam_bufferSEM);
	printf("-----------------------------------\n");

	/* ----------------INICIALIZANDO LOS SEMÁFOROS--------------------
	*  Primer parametro: Hace referencia al semáforo que se inicializa
	*  Segundo parametro: Indica si otros procesos pueden visualizarlo
	*  Tercer parametro: Es el contador del semáforo
	*/
	sem_init(&mutex, 1, 1);
	//Inicialmente el buffer esta vació 
	sem_init(&vacias, 1, tam_bufferSEM);
	sem_init(&llenas, 1, 0);

	/*---------------Usando OMP--------------------------------------*/

	//Se lanzan los hilos que el usuario indico
	omp_set_num_threads(numHilos);
	
	int id_Hilo;

	#pragma omp parallel private (id_Hilo)
	{
		for(int i =0; i<iteraciones; i++){		
				id_Hilo = omp_get_thread_num();

				/* Esta parte del código determina que papel juega el hilo
				*  Es decir Consumidor o productor
				*/								
					if ((id_Hilo >= 0)&&(id_Hilo < consumidores)){
						consumidor(id_Hilo);
					}else{
						productor(id_Hilo);
					}

		}//END FOR

		//---Requisito de la práctica---
		printf("--El hilo %i ha iterado %i veces -- \n", id_Hilo, iteraciones);	
		//---Requisito de la práctica---

	}//END Parallel
	
	printf("Al final de la ejecución de todos los hilos el buffer tiene: %i elementos\n",tam_buffer);
	
	sem_destroy(&mutex);
	sem_destroy(&vacias);
	sem_destroy(&llenas);

		
}//END MAIN
