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

/* DECLARACION DE FUNCIONES */
float saldo_tarjeta(void);
void reglas(void);
int aleatorio(int, int);
float obtener_saldo(void);
void modificar_saldo(float, int);
int suma_cartas(int);
void realizar_apuesta(void);
int barajar(void);
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
    	printf("Selecciona una opción:\n");
	    printf("1. Ver reglas del juego\n");
	    printf("2. Empezar el juego\n");
	    printf("3. Ver saldo de tarjeta\n");
	    printf("4. Cargar saldo a tarjeta\n");
	    printf("5. Salir\n");
    
    	printf("Su opción: ");
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
							jugador[x] = barajar();
							cuprier[x] = barajar();
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
										int nueva_carta = barajar();										
										
										if (nueva_carta==1){
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
												
											jugador[numero_cartas] = valor_as;
											numero_cartas += 1;
										}else if (nueva_carta==7){
											int reemplazo=20;
											printf("Has obtenido un 7.\n");
											while(reemplazo>numero_cartas){												
												printf("Escoja una carta a reemplazar: ");
												scanf("%d", &reemplazo);
												
												jugador[(reemplazo-1)] = nueva_carta;												
											}
										}else{
											jugador[numero_cartas] = barajar();
											numero_cartas += 1;	
										}
																				
										mostrar_cartas(0);										
										
										break;
									};
									case 3: {
										printf("\nRepartiendo cartas al cuprier...\n\n");
										cuprier[numero_cartas] = barajar();
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
	return saldo_actual;
}

void reglas(void){
	printf("REGLAS DEL JUEGO\n\n");
	printf("1. Su puesta minima es de %f y maxima de %f\n", limites[0], limites[1]);
	printf("2. El juego consiste en sumar 21 con sus cartas\n");
	printf("3. El juego termina cu&&o cualquiera de los jugadores llega a 21 o se sobrepasa, si es mayor o menos a 21, gana la persona cuya suma de cartas se acerque mas a 21 sin pasarse.\n\n\n");
}

int aleatorio(int min, int max){
	srand(time(NULL));
	return min + rand() % (max + 1 - min);
}

int barajar(void){	
	int r_palo = aleatorio(1, 4);
	int num_carta = aleatorio(1, 13);
	Sleep(1000);
	return num_carta;
}

float obtener_saldo(void){
	return saldo_actual;
}

void modificar_saldo(float valor, int op){
	if (op==0){
		saldo_actual += valor;
	}else{
		saldo_actual -= valor;
	}
}

int suma_cartas(int op){
	int total = 0;
	int x;
	for (x=0; x<10; x++){
		if (op==0){
			total += jugador[x];
		}else{
			total += cuprier[x];
		}
	}
	return total;
}

void nombres_cartas(int carta){
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
	printf("Cartas del jugador = [");
	for (x=0; x<numero_cartas; x++){
		nombres_cartas(jugador[x]);
		if (x<(numero_cartas-1)){
			printf(" - ");
		}
	}
	printf("]\n");
	printf("La suma de tus cartas es de %d\n\n", suma_cartas(0));
	
	if (op==1){
		printf("Cartas del cuprier = [");
		for (x=0; x<numero_cartas; x++){
			nombres_cartas(cuprier[x]);			
			if (x<(numero_cartas-1)){
				printf(" - ");
			}
		}
		printf("]\n");
		printf("La suma de las cartas del cuprier es de %d\n\n", suma_cartas(1));	
	}	
}

void realizar_apuesta(){
	printf("\nIngrese el valor de su apuesta: $ ");
	scanf("%f", &apuesta);
	
	if ((apuesta>=limites[0]) && (apuesta<=limites[1])){
		printf("Se ha descontado $ %f de su tarjeta\n\n", apuesta);
		modificar_saldo(apuesta, 1);
		valor_apostado += apuesta;
	}else if (apuesta<limites[0]){
		printf("Se ha descontado $ %f de su tarjeta\n\n", limites[0]);
		modificar_saldo(limites[0], 1);
		valor_apostado += limites[0];
	}else if (apuesta>limites[1]){
		printf("Se ha descontado $ %f de su tarjeta\n\n", limites[1]);
		modificar_saldo(limites[1], 1);
		valor_apostado += limites[1];
	}
}

void analiza_cartas(void){
	float total_ganado = valor_apostado * 2;
	int suma_jugador = suma_cartas(0);
	int suma_cuprier = suma_cartas(1);
		
	if (suma_jugador>suma_cuprier){
		if (suma_jugador<=21){
			printf("\nUsted ha ganado $ %f USD!!\n", total_ganado);
			modificar_saldo(total_ganado, 0);	
		}else{
			printf("\nCuprier ha ganado\n");
			printf("Usted ha perdido $ %f USD!!\n", valor_apostado);
		}		
	}else if (suma_jugador<suma_cuprier){
		if ((suma_cuprier==21) && (suma_jugador==21)){
			printf("\nExiste un empate\n");
			printf("Usted ha perdido $ %f USD!!\n", valor_apostado);
		}else if ((suma_cuprier<=21) && (suma_jugador<21)){
			printf("\nCuprier ha ganado\n");
			printf("Usted ha perdido $ %f USD!!\n", valor_apostado);
		}else if ((suma_cuprier>21) && (suma_jugador<=21)){			
			printf("\nUsted ha ganado $ %f USD!!\n", total_ganado);
			modificar_saldo(total_ganado, 0);
		}else if ((suma_cuprier>21) && (suma_jugador>21)){
			printf("\nNadie ha ganado\n");
			printf("Usted ha perdido $ %f USD!!\n", valor_apostado);
		}
	}		
}

void reinicia(void){
	numero_cartas = 0;
	valor_apostado = 0;
	int x;
	for (x=0; x<10; x++){		
		jugador[x] = 0;
		cuprier[x] = 0;
	}
}
