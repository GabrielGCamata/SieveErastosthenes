#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#define MAX_THREAD 8 // NAO PODE PASSAR DE 8, SE PASSAR AS THREADS SEGUINTES NAO VAO TER UTILIDADES. SE DIMINUIR NAO INFLUENCIA EM NADA APENAS OS NUMEROS PRIMOS PARA TESTE VAO DIMINUIR
#define MAX_NUM 10000 //QUANTIDADE MAXIMA DE ARGUMENTOS QUE PODE SER PASSADO

typedef struct thread_data{
	int id;	//IDENTIFICADOR DA THREAD
	int divisor; //NUMERO PRIMO QUE VAI SER O DIVISOR
	int quantidade; // QUANTIDADE DE NUMEROS MULTIPLOS DO NUMERO PRIMO
	int multiplos[MAX_NUM];//BUFFER COM OS NUMEROS MULTIPLOS DO DIVISOR
} t_thread_data;	//tipo para armazenar os numeros multiplos dos primos

typedef struct numeros{
	int quantidade;//QUANTIDADE DE NUMEROS PASSADA PELO O USUARIO
	int	qnt_numeros[MAX_NUM];//BUFFER COM OS NUMEROS GERADOS
}t_quantidade_numeros; 	//tipo para armazenar os numeros gerados

t_quantidade_numeros vetor_numeros;

void * gera_vetor(void *arg){ //funcao que gera o vetor com os numeros de acordo com o que usuario entrou
	int *valor = (int *)(arg);
	int i;
	if(*valor > 0){
		for(i=0; i < *valor; i++)
			vetor_numeros.qnt_numeros[i] = i+2;
		printf("Thread principal\n");
		vetor_numeros.quantidade = *valor;
	}
	else{
		printf("Passou um argumento errado");
		exit(0);
	}
}

void * sieve_erastosthenes(void *arg){ //A função atribui os multiplos dos números primos, isto é marcam os números que nao sao primos
	t_thread_data *thread_dados = (t_thread_data*)(arg);
	int i,j;
	printf("%d\n",thread_dados->id);
	if(thread_dados->id == 1){
		for(i = 0,j=0; i < vetor_numeros.quantidade; i++){
			if((vetor_numeros.qnt_numeros[i] % 2)==0){
				thread_dados->multiplos[j] = vetor_numeros.qnt_numeros[i];
				j++;
			}
		}
		thread_dados->divisor = 2; 
		thread_dados->quantidade = j; 
	}
	if(thread_dados->id == 2){
		for(i = 0,j=0; i < vetor_numeros.quantidade; i++){
			if((vetor_numeros.qnt_numeros[i] % 3)==0){
				thread_dados->multiplos[j] = vetor_numeros.qnt_numeros[i];
				j++;
			}
		}
		thread_dados->divisor = 3;
		thread_dados->quantidade =j;
	}
	if(thread_dados->id == 3){
		for(i = 0,j=0; i < vetor_numeros.quantidade; i++){
			if((vetor_numeros.qnt_numeros[i] % 5)==0){
				thread_dados->multiplos[j] = vetor_numeros.qnt_numeros[i];
				j++;
			}
		}
		thread_dados->divisor = 5;
		thread_dados->quantidade = j;
	}
	if(thread_dados->id == 4){
		for(i = 0,j=0; i < vetor_numeros.quantidade; i++){
			if((vetor_numeros.qnt_numeros[i] % 7)==0){
				thread_dados->multiplos[j] = vetor_numeros.qnt_numeros[i];
				j++;
				vetor_numeros.qnt_numeros[i] =1;
			}
		}
		thread_dados->divisor = 7;
		thread_dados->quantidade =j;
	}
	if(thread_dados->id == 5){
		for(i = 0,j=0; i < vetor_numeros.quantidade; i++){
			if((vetor_numeros.qnt_numeros[i] % 11)==0){
				thread_dados->multiplos[j] = vetor_numeros.qnt_numeros[i];
				j++;
			}
		}
		thread_dados->divisor = 11;
		thread_dados->quantidade = j;
	}
	if(thread_dados->id == 6){
		for(i = 0,j=0; i < vetor_numeros.quantidade; i++){
			if((vetor_numeros.qnt_numeros[i] % 13)==0){
				thread_dados->multiplos[j] = vetor_numeros.qnt_numeros[i];
				j++;
			}
		}
		thread_dados->divisor = 13;
		thread_dados->quantidade =j;
	}
	if(thread_dados->id == 7){
		for(i = 0,j=0; i < vetor_numeros.quantidade; i++){
			if((vetor_numeros.qnt_numeros[i] % 17)==0){
				thread_dados->multiplos[j] = vetor_numeros.qnt_numeros[i];
				j++;
			}
		}
		thread_dados->divisor = 17;
		thread_dados->quantidade = j;
	}
	if(thread_dados->id == 8){
		for(i = 0,j=0; i < vetor_numeros.quantidade; i++){
			if((vetor_numeros.qnt_numeros[i] % 19)==0){
				thread_dados->multiplos[j] = vetor_numeros.qnt_numeros[i];
				j++;
			}
		}
		thread_dados->divisor = 19;
		thread_dados->quantidade =j;
	}
}



typedef struct s_primos{ //ESTRUTURA QUE O ESPAÇO DE MEMORIA COMPARTILHADO VAI TER
	int numero; // NUMERO PASSADO PELA THREAD PRINCIPAL
	int primo;  // FLAG QUE VAI DIZER SE O NUMERO É PRIMO OU NÃO
	int vetor_divisiveis; // SE NAO FOR PRIMO, IRA ARMAZENAR O PRIMO QUE É DIVISIVEL
	int qnt_divisor; 
	int sieve; // ARMAZENA A SIEVE QUE O NUMERO ESTA
}t_primos; 

t_primos primos[MAX_NUM];//ESPAÇO DE MEMORIA OMPATILHADO QUE VAI ARMAZENAR O RESULTADO

void verifica_primos(t_thread_data *thread_dado){ //VAI ESCREVER NO ESPAÇO DE MEMORIA COMPARTILHADO TODOS OS NÚMEROS
	int i,j,cout=0,flag=0,cout2;				  //FALANDO SE É PRIMO OU NAO

	for ( i = 0; i <= vetor_numeros.quantidade; i++){

		primos[i].numero = vetor_numeros.qnt_numeros[i];
		primos[i].primo = 0;

		for (j= 0; j < MAX_THREAD ;j++){

			for(cout2 = 0; cout2 <= thread_dado[j].quantidade; cout2++){

					if(primos[i].numero == thread_dado[j].multiplos[cout2] && flag == 0){

						if(!(primos[i].numero == thread_dado[j].divisor)){
							primos[i].vetor_divisiveis = thread_dado[j].divisor;
							primos[i].sieve = thread_dado[j].id;
							flag++;
							primos[i].primo = 1;
							primos[i].qnt_divisor = flag;
						}

					}

			}

		}

		flag = 0;
	}

	for(j =0; j < vetor_numeros.quantidade; j++){ // IMPRIME NA TELA O RESULTADO
		printf("\n%d ",primos[j].numero);
		if(primos[j].primo == 1){
			printf("divided by ");
			printf(" %d ",primos[j].vetor_divisiveis);
			printf("at sieve ");
			printf("%d ", primos[j].sieve-1);
		}
		else printf("is prime");
	}
}

int main(int argc, char **argv){
	pthread_t thread_principal;
	pthread_t thread_filtro[MAX_THREAD];
	int a1;
	t_thread_data thread_dados[MAX_THREAD];
	
	if(argc != 3){//verifica a quantidade de argumentos
		printf("Quantidade de argumentos errada");
		exit(0);
	}
	
	a1 = atoi(argv[1]) -1;//converte a string para int
	pthread_create(&thread_principal, NULL, gera_vetor, (void *)(&a1));//cria a thread principal com o vetor de numeros
	pthread_join(thread_principal,NULL);//faz com que a funcao execute a thread antes de encerrar

	/*int aux1;
	for(aux1 =0; aux1 < a1; aux1++)
		printf("%d\n",vetor_numeros.qnt_numeros[aux1]);
	*/
	printf("\n\n\n\n");
	
	int aux;
	for(aux = 0; aux < MAX_THREAD; aux++){//cria as 8 threads
		thread_dados[aux].id = aux+1;
		pthread_create(&thread_filtro[aux],NULL,sieve_erastosthenes,(void *)(&thread_dados[aux]));
	}
	for(aux = 0; aux < MAX_THREAD; aux++){
		pthread_join(thread_filtro[aux],NULL);//faz com que a funcao execute a thread antes de encerrar e em ordem, de acordo com o incremento
	}
	
	/*int aux_teste1, aux_teste2;	//IMPRIME OS MULTIPLOS DOS NUMEROS PRIMOS
	for(aux_teste2 = 0; aux_teste2 < MAX_THREAD; aux_teste2++){
		printf("\n\n\nmultiplos de %d\n",thread_dados[aux_teste2].divisor);
		printf("quantidade %d\n",thread_dados[aux_teste2].quantidade);
		for(aux_teste1 = 0; aux_teste1 < thread_dados[aux_teste2].quantidade; aux_teste1++){
			printf("%d ", thread_dados[aux_teste2].multiplos[aux_teste1]);
		}
	}*/

	verifica_primos(thread_dados); // PASSA O RESULTADO DAS THREADS PARA UM ESPAÇO COMPARTILHADO E IMPRIME O RESULTADO NA TELA

	exit(0);
}
