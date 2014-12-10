#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){
	int i;
	char nome[45];
	char check[45];
	
	time_t inicio, fim;	
	
	time(&inicio);
	
	for(i=1; i<= 254; i++){
		if(i < 10){
			sprintf(nome, "./crosssums puzzle_prof/Kakuro_00%d.cfg", i); 
			sprintf(check, "chkcs puzzle_prof/Kakuro_00%d.sol; echo $?", i); 
		}
		else{
			if(i < 100){
				sprintf(nome, "./crosssums puzzle_prof/Kakuro_0%d.cfg", i);
				sprintf(check, "chkcs puzzle_prof/Kakuro_0%d.sol; echo $?", i); 
			}
			else{
				sprintf(nome, "./crosssums puzzle_prof/Kakuro_%d.cfg", i);
				sprintf(check, "chkcs puzzle_prof/Kakuro_%d.sol; echo $?", i); 
			}
		}
		
		system(nome);
		system(check);
		printf(" este foi o Kakuro #%d :)\n", i);
		/*getchar();*/
	}
	
	time(&fim);
	
	printf("acabou em aproximadamente %.0f segundos\n", difftime(fim, inicio));
	
	
	
	exit(0);
}
