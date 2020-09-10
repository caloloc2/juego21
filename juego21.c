#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* DECLARACION DE VARIABLES, VECTORES Y ARREGLOS */
int opcion;
float saldo_actual=0;
float limites[2] = {100, 1000};
int jugador[10];
int cuprier[10];
int numero_cartas = 0;
float valor_apostado = 0 ;
float apuesta = 0;
int cambio_as = 0;
int valor_as = 1;
int primera_vez = 1;

/* DECLARACION DE FUNCIONES */
float saldo_tarjeta(void);
void reglas(void);
int aleatorio(int, int);
void modificar_saldo(float, int);
int suma_cartas(int);
void realizar_apuesta(void);
int barajar(int);
void mostrar_cartas(int);
void analiza_cartas(void);
void reinicia(void);
void nombres_cartas(int);

/* FUNCION PRINCIPAL */
int main(){	
    system("cls");
    
    printf("Te saluda Helsinki, te doy la bienvenida al casino\n");
    printf("Por el momento solo disponemos mesas para BlackJack\n\n");
    
    printf("Su mesa asignada es la %d\n", aleatorio(1, 10));
    printf("Su numero de tarjeta es %d\n\n", aleatorio(100, 200));

    while(opcion!=5){
    	printf("Selecciona una opcion:\n");
	    printf("1. Ver reglas del juego\n");
	    printf("2. Empezar el juego\n");
	    printf("3. Ver saldo de tarjeta\n");
	    printf("4. Cargar saldo a tarjeta\n");
	    printf("5. Salir\n");
    
    	printf("Su opcion: ");
    	scanf("%d", &opcion);    	
    	
    	switch(opcion){
			case 1: {
				system("cls");
				reglas();
				break;
			}
			case 2: {
				system("cls");
				printf("EMPIEZA EL JUEGO\n\n");
				int op2 = 0;
				int valida = 1;
				int juega = 1;
				int x;
				
				while(juega==1){
					if (saldo_tarjeta()>0){
						printf("Su saldo actual es de %f\n", saldo_tarjeta());
					
						realizar_apuesta(); /* Realiza primera apuesta */
						
						printf("Repartiendo cartas...\n\n");
						/* SE REPARTE DOS CARTAS AL JUGADOR Y CUPRIER */
						for (x=0; x<2; x++){
							primera_vez = 1; // variable bandera para saber que se reparte las cartas por primera vez
							jugador[x] = barajar(0);
							cuprier[x] = barajar(1);
							numero_cartas += 1;
						}
						
						mostrar_cartas(0);					
						
						if ((suma_cartas(0)<21) && (suma_cartas(1)<21)){
							while(valida==1){
								printf("1. Aumentar apuesta\n");
								printf("2. Otra carta\n");
								printf("3. No mas cartas\n");
								printf("Seleccione una opcion: ");
								
								scanf("%d", &op2);
								
								switch(op2){
									case 1: {								
										realizar_apuesta(); /* Realiza nueva apuesta */									
										break;
									};
									case 2: {
										printf("\nRepartiendo cartas al jugador...\n\n");
										primera_vez = 0; // variable bandera para saber que se reparte las cartas despues de la primera vez
										int nueva_carta = barajar(0);
										
										if (nueva_carta==7){
											int reemplazo=20;
											printf("Has obtenido un 7.\n");
											while(reemplazo>numero_cartas){
												printf("Escoja una carta a reemplazar: ");
												scanf("%d", &reemplazo);
												
												jugador[(reemplazo-1)] = nueva_carta;
											}
										}else{
											jugador[numero_cartas] = nueva_carta;
											numero_cartas += 1;
										}
											
										mostrar_cartas(0);										
										
										break;
									};
									case 3: {
										printf("\nRepartiendo cartas al cuprier...\n\n");
										primera_vez = 0; // variable bandera para saber que se reparte las cartas despues de la primera vez
										cuprier[numero_cartas] = barajar(1);
										numero_cartas += 1;
										
										mostrar_cartas(1);
										
										analiza_cartas();
																				
										printf("\nSu saldo actual es de %f\n\n\n", saldo_tarjeta());
										
										juega = 0;
										valida = 0;										
										reinicia();
										break;
									};
									default: {
										printf("Opcion invalida\n");
										break;
									}
								}	
							}	
						}else{						
							printf("Juego terminado\n");
							juega = 0;
						}
					}else{
						printf("No tiene saldo en su tarjeta. Por favor recarguela.\n\n\n");
						juega = 0;
					}
				}				
				break;
			}
			case 3: {	
				system("cls");			
				printf("\n\nSu saldo es de %f\n\n\n", saldo_tarjeta());
				break;	
			};
			case 4: {
				system("cls");
				float valor = 0;
				printf("\n\nValor a cargar: $ ");
				scanf("%f", &valor);
				modificar_saldo(valor, 0);
				printf("Su nuevo saldo es de %f\n\n\n", saldo_tarjeta());
				break;	
			};
			case 5: {
				if (saldo_tarjeta()>0){
					printf("\n\nSe le entrega $ %f USD.\n", saldo_tarjeta());
				}else{
					printf("\n\nNo ha ganado nada en el juego.\n");
				}
				printf("Adios.");
				break;
			};
			default: {
				printf("Opcion invalida\n");
				break;
			}
		}
	}
}

/* DEFINICION DE FUNCIONES */

float saldo_tarjeta(void){
	/* Devuelve el saldo actual de la tarjeta */
	return saldo_actual;
}

void reglas(void){
	/* Imprime las reglas del juego */
	printf("REGLAS DEL JUEGO\n\n");
	printf("1. Su puesta minima es de %f y maxima de %f\n", limites[0], limites[1]);
	printf("2. El juego consiste en sumar 21 con sus cartas\n");
	printf("3. El juego termina cu&&o cualquiera de los jugadores llega a 21 o se sobrepasa, si es mayor o menos a 21, gana la persona cuya suma de cartas se acerque mas a 21 sin pasarse.\n\n\n");
}

int aleatorio(int min, int max){
	/* Cambia el valor de la semilla de los randomicos */
	srand(time(NULL));
	/* Obtiene un valor aleatorio entre min y max, con base a su semilla */
	return min + rand() % (max + 1 - min);
}

int barajar(int quien){
	int carta_retorno = 0;
	/* Obtiene un aleatorio para el palo y carta */
	int r_palo = aleatorio(1, 4);
	int num_carta = aleatorio(1, 13);
	/* Espera 1 segundo en cada reparto, para que se actualice una nueva semilla */
	Sleep(1000);
	
	if (quien==0){ // Analiza el valor de la carta obtenida, para dar las opciones al jugador		
		if (num_carta==1){
			if (cambio_as==0){											
				int op3 = 0;
				printf("Has obtenido un AS.\n");
				printf("1. Cambiar valor a 1.\n");
				printf("2. Cambiar valor a 11.\n");
				
				while((op3!=1)&&(op3!=2)){
					printf("Selecciona una opcion: ");
					scanf("%d", &op3);
				}
				
				if (op3==1){
					valor_as = 1;
				}else{
					valor_as = 11;
				}
				
				cambio_as = 1;
			}			
			carta_retorno = valor_as;
		}else if (num_carta==7){
			if (primera_vez == 1){
				int op4 = 0;
				printf("Has obtenido un 7.\n");
				while ((op4!=1) && (op4!=2)){
					printf("1. Deseas obtener una nueva carta.\n");
					printf("2. Mantener la nueva carta.\n");
					printf("Seleccione una opcion: ");
					scanf("%d", &op4);
					
					if (op4==1){ // Obtiene una nueva carta aleatoria
						r_palo = aleatorio(1, 4);
						carta_retorno = aleatorio(1, 13);					
					}else if (op4==2){
						carta_retorno = num_carta;
					}
				}	
			}else{
				carta_retorno = num_carta;
			}
		}else{
			carta_retorno = num_carta;
		}	
	}else{ // Devuelve la carta para el cuprier
		carta_retorno = num_carta;
	}
	
	/* Retorna la carta */
	return carta_retorno;
}

void modificar_saldo(float valor, int op){	
	if (op==0){ /* Si es cero entonces suma */
		saldo_actual += valor;
	}else{ /* Si es otro valor entonces resta */
		saldo_actual -= valor;
	}
}

int suma_cartas(int op){
	int total = 0;
	int x;	 
	for (x=0; x<10; x++){ /* Recorre las 10 posiciones de cada vector */
		if (op==0){ // Si es cero, entonces obtiene la suma del vector jugador
			if ((jugador[x]==11) || (jugador[x]==12) || (jugador[x]==13)){ // para las J, Q, y K con valores 11, 12 y 13... se suma solo 10
				total += 10;
			}else{
				total += jugador[x];	
			}
			
		}else{ // Si es otro valor, entonces obtiene la suma del vector cuprier
			if ((cuprier[x]==11) || (cuprier[x]==12) || (cuprier[x]==13)){ // para las J, Q, y K con valores 11, 12 y 13... se suma solo 10
				total += 10;
			}else{
				total += cuprier[x];
			}
			
		}
	}
	return total; // retorna la suma total
}

void nombres_cartas(int carta){
	// Cambia el nombre de las cartas, excepto las cartas numericas
	switch(carta){
		case 1: {
			printf("AS");
			break;
		};		
		case 11: {
			printf("J");
			break;
		};
		case 12: {
			printf("Q");
			break;
		};
		case 13: {
			printf("K");
			break;
		};
		default: {
			printf("%d", carta);
			break;
		}
	}
}

void mostrar_cartas(int op){	
	int x;
	/* Muesta las cartas del jugador */
	printf("Cartas del jugador = [");
	for (x=0; x<numero_cartas; x++){
		if (jugador[x]>0){
			nombres_cartas(jugador[x]);
			if (x<(numero_cartas-1)){
				printf(" - ");
			}
		}			
	}
	printf("]\n");
	/* Muesta la suma de las cartas del jugador */
	printf("La suma de tus cartas es de %d\n\n", suma_cartas(0));
	
	if (op==1){ // Si op es uno, entonces muestra las cartas del cuprier tambien
		/* Muesta las cartas del cuprier */
		printf("Cartas del cuprier = [");
		for (x=0; x<numero_cartas; x++){
			if (cuprier[x]>0){
				nombres_cartas(cuprier[x]);			
				if (x<(numero_cartas-1)){
					printf(" - ");
				}
			}				
		}
		printf("]\n");
		/* Muesta la suma de las cartas del cuprier */
		printf("La suma de las cartas del cuprier es de %d\n\n", suma_cartas(1));	
	}	
}

void realizar_apuesta(){	
	printf("\nIngrese el valor de su apuesta: $ ");
	scanf("%f", &apuesta); // obtiene el valor de la apuesta
	
	if ((apuesta>=limites[0]) && (apuesta<=limites[1])){ // Si esta dentro de los limites, descuenta el valor ingresado
		printf("Se ha descontado $ %f de su tarjeta\n\n", apuesta);
		modificar_saldo(apuesta, 1); // resta del saldo actual de la tarjeta
		valor_apostado += apuesta; // guarda el valor apostado
	}else if (apuesta<limites[0]){ // Si el valor de la apuesta es menor que el limite inferior, entonces descuenta el valor del limite inferior
		printf("Se ha descontado $ %f de su tarjeta\n\n", limites[0]);
		modificar_saldo(limites[0], 1); // resta del saldo actual de la tarjeta
		valor_apostado += limites[0]; // guarda el valor apostado
	}else if (apuesta>limites[1]){ // Si el valor de la apuesta es mayor que el limite superior, entonces descuenta el valor del limite superior
		printf("Se ha descontado $ %f de su tarjeta\n\n", limites[1]);
		modificar_saldo(limites[1], 1); // resta del saldo actual de la tarjeta
		valor_apostado += limites[1]; // guarda el valor apostado
	}
}

void analiza_cartas(void){
	float total_ganado = valor_apostado * 2; // en caso de ganar, se entregara el doble de la apuesta
	float total_ganado_21 = valor_apostado * 3; // en caso de ganar con 21, se entregara el triple de la apuesta
	int suma_jugador = suma_cartas(0); // obtiene la suma de las cartas para el jugador
	int suma_cuprier = suma_cartas(1); // obtiene la suma de las cartas para el cuprier
		
	if (suma_jugador>suma_cuprier){ // Verifica que las cartas del jugador sean mayor que las del cuprier
		if (suma_jugador<=21){ // Verifica que no sobrepase 21
			if (suma_jugador==21){ // En caso de que gane el jugador con 21, se duplica el valor ganado
				printf("\nUsted ha ganado con 21, recibe $ %f USD!!\n", total_ganado_21);
				modificar_saldo(total_ganado_21, 0); // acredita el valor apostado x 2	
			}else if (suma_jugador<21){ // si gana con menos de 21, entonces solamente recibe el valor apostado x 2
				printf("\nUsted ha ganado $ %f USD!!\n", total_ganado);
				modificar_saldo(total_ganado, 0); // acredita el valor apostado x 2	
			}			
		}else{ // Se sobrepaso de 21
			printf("\nCuprier ha ganado\n");
			printf("Usted ha perdido $ %f USD!!\n", valor_apostado);
		}		
	}else if (suma_jugador<suma_cuprier){ // En caso de que las cartas del cuprier sean mayores que las del jugador
		if ((suma_cuprier==21) && (suma_jugador==21)){ // En caso de que los dos tengan 21
			printf("\nExiste un empate\n");
			printf("Se devuelve su valor apostado $ %f USD!!\n", valor_apostado);
			modificar_saldo(valor_apostado, 0);
		}else if ((suma_cuprier<=21) && (suma_jugador<21)){ // En caso de que el cuprier tenga 21 o menos, y el jugador menos de 21
			printf("\nCuprier ha ganado\n");
			printf("Usted ha perdido $ %f USD!!\n", valor_apostado);
		}else if ((suma_cuprier>21) && (suma_jugador<=21)){ // En caso de que el cuprier tenga mas de 21, y el jugador menos de 21
			if (suma_jugador==21){ // En caso de que gane el jugador con 21, se duplica el valor ganado
				printf("\nUsted ha ganado con 21, recibe $ %f USD!!\n", total_ganado_21);
				modificar_saldo(total_ganado_21, 0); // acredita el valor apostado x 2	
			}else if (suma_jugador<21){ // si gana con menos de 21, entonces solamente recibe el valor apostado x 2
				printf("\nUsted ha ganado $ %f USD!!\n", total_ganado);
				modificar_saldo(total_ganado, 0); // acredita el valor apostado x 2	
			}			
		}else if ((suma_cuprier>21) && (suma_jugador>21)){ // En caso de que los dos tengas mas de 21
			if ((suma_cuprier>21) && (suma_cuprier<suma_jugador)){ // Si la suma de cartas del cuprier se acerca a 21, gana cuprier
				printf("\nCuprier ha ganado\n");
				printf("Usted ha perdido $ %f USD!!\n", valor_apostado);
			}else if ((suma_jugador>21) && (suma_jugador<suma_cuprier)){ // Si la suma de cartas del jugador se acerca a 21, gana jugador
				printf("\nUsted ha ganado $ %f USD!!\n", total_ganado);
				modificar_saldo(total_ganado, 0); // acredita el valor apostado x 2
			}else if (suma_cuprier=suma_jugador){ // Si son iguales, entonces empate
				printf("\nExiste un empate\n");
				printf("Se devuelve su valor apostado $ %f USD!!\n", valor_apostado);
				modificar_saldo(valor_apostado, 0);
			}			
		}
	}		
}

void reinicia(void){
	/* Reinicio de variables y vectores para volver a jugar */
	numero_cartas = 0;
	valor_apostado = 0;
	primera_vez = 0;
	int x;
	for (x=0; x<10; x++){		
		jugador[x] = 0;
		cuprier[x] = 0;
	}
}
