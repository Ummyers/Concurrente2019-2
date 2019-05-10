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
#include <unistd.h> //Para poder hacer uso de usleep()

#define CONSTANTE 2; //Constante que determina el valor inicial del contador del semáforo

/* Estructura que simula un semáforo
*  Int Contador Determina cuantos hilos permite pasar
*  Int Cola
*/
struct semaforo{
	int contador;
	int cola[50];
};

int cierto[50];
int numHilos;
struct semaforo sem1;

/*
* Método que fue auxiliar cuando se llevo acabo la implementación.
*/
void imprimeCola(){
	for (int i = 0; i < 10; ++i)
	{
		printf("%i, ",sem1.cola[i]);
	}
	printf("\n");
}
/* Dado la forma de trabajar de C se implementa este método. 
*/
void memoria_Arreglos(){
	for (int i = 0; i < 50; ++i)
	{
		cierto[i] = 0;
		sem1.cola[i] = 0;
	}
}

void imprimeCierto(){

	for (int i = 0; i < 10; ++i)
	{
		printf("%i, ",cierto[i]);
	}
	printf("\n");
}
/*
* Método que suma los primeros 1000 números.
*/
void sumaIterativa(){
	int suma = 0;
	for (int i=0;i<1000;i++){
		suma += i;
	}
}

//Método que se encarga de "encolar" al hilo que no pudo entrar a la sección especial
void encolar(int H){
	for (int i = 0; i < 50; ++i)
	{
		if(sem1.cola[i]=0){
			sem1.cola[i] = H;
			break;
		}
	}
}
/* Verifica si ya es el turno del Hilo
*  El que este en la posición 0 
*
*/
int verificaTurno(int H){
	if(sem1.cola[0]=H){
		return 1;
	}else{
		return 0;
	}
}

/* Método que simula un wait del semáforo
*	@param Hilo: Recibe el hilo que lo invoco
*/
void wait_mySemaphore(int Hilo){

	if(sem1.contador > 0){
		
		sem1.contador --;
		printf(" <--- Entro el hilo: %i, contador: %i \n", Hilo, sem1.contador);
	}else{

		encolar(Hilo);
		//Si no puede llevar acabo la sección especial hay que encolarlo. 
		//printf("ENCOLO %i\n",Hilo);
		//imprimeCola();

		while(1){
			usleep(1000);
			if((verificaTurno(Hilo)==1)&&(sem1.contador > 0)){
				sem1.contador--;
				printf(" <-- Entro el hilo: %i contador %i \n", Hilo, sem1.contador);
				
				//printf(" <-- Entro el hilo: %i, contador: %i \n", Hilo, sem1.contador);
				break;
			}
		}	
	}
}
/* Meétodo signal_mySemaphore que no se utiliza
* ya que la solución correcta es ponerlo en la zona 
* paralela. 
*
*/
void signal_mySemaphore(){

	//Recorre a lugar
	for (int i = 0; i < 50; ++i)
	{	
		sem1.cola[i] = sem1.cola[i+1];
	}

	sem1.contador ++;
}

int main(int argc, char** argv){

//Se verifica que se indicara el tamaño del arreglo y la semilla.
	if (argc < 2){
		printf("No especificaste todos los parametros.\n");
		exit(1);
	}
	
	numHilos = atoi(argv[1]);

	//Se lanzan los hilos que el usuario indico
	omp_set_num_threads(numHilos);

	memoria_Arreglos();
	//Se inicia el semáforo con el valor de CONSTANTE que es el número de hilos que entrara...
	sem1.contador = CONSTANTE;

	printf("El valor inicial del contador es: %i\n",sem1.contador);
	printf("---------------------------------------\n");

	int id_Hilo;

	#pragma omp parallel private (id_Hilo)
	{
		
		for(int i=0; i< 10; i++){

				id_Hilo = omp_get_thread_num();
				id_Hilo = id_Hilo+1;

					wait_mySemaphore(id_Hilo);
						//printf(" <--- Entro el hilo: %i, contador: %i \n", id_Hilo, sem1.contador);
						//printf("Se procede a sumar iterativamente...\n");
						sumaIterativa();
				
						//printf("---> Salio el hilo %i, contador %i \n", id_Hilo, sem1.contador+1);
						printf("---> Salio el hilo %i\n", id_Hilo);
						/* Parte de código que simula un signal. 
						*
						*/
						for (int i = 0; i < 50; ++i)
						{	
							//#pragma parallel atomic
							sem1.cola[i] = sem1.cola[i+1];
						}
						//#pragma parallel atomic
						sem1.contador ++;
						//printf("---> Salio el hilo %i, contador %i \n", id_Hilo, sem1.contador);
		}//END FOR
	}
}//END MAIN
