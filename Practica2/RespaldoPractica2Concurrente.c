/* Práctica 02: Computación Concurrente
*	Muñiz Patiño, Andrea Fernanda
*	No. de cuenta 311175318
* 	UNAM, Facultad de Ciencias.
* 	2/Abril/2019
*	Semáforos
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>
//Para el uso de semáforos
#include <semaphore.h>


//Se declara el semáforo
sem_t entran;


void sumaIterativa();

/*
* Método que suma los primeros 1000 números.
*/
void sumaIterativa(){
	int suma = 0;

	for (int i=0;i<1000;i++){
		suma += i;
	}

}

int main(int argc, char** argv){

//Se verifica que se indicara el tamaño del arreglo y la semilla.
	if (argc < 2){
		printf("No especificaste todos los parametros.\n");
		exit(1);
	}
	
	int numHilos = atoi(argv[1]);

	int CONSTANTE = 2; //Constante que determina el valor inicial del contador del semáforo

	//Se lanzan los hilos que el usuario indico
	omp_set_num_threads(numHilos);

	/* Primer parametro: Hace referencia al semáforo que se inicializa
	*  Segundo parametro: Indica si otros procesos pueden visualizarlo
	*  Tercer parametro: Es el contador del semáforo
	*/

	sem_init(&entran, 1, CONSTANTE);
	int id_Hilo;
	#pragma omp parallel
	#pragma omp for
		
	
		for(int i =0; i<10; i++){

				sem_wait(&entran);
					#pragma omp critical
					{
						id_Hilo = omp_get_thread_num();
						printf("Entro el hilo: %i\n", id_Hilo);
						//Falta poner la variable
						//printf("El valor del contador es: %i \n", CONSTANTE);
						//printf("Se procede a sumar iterativamente...\n");
						sumaIterativa();
					}
				sem_post(&entran);
			
			id_Hilo = omp_get_thread_num();
			printf("El hilo %i ha salido\n", id_Hilo );
		}
		
}//END MAIN
