#include <stdio.h>
#include <stdlib.h>

int main(){
	int i;
	char nome[45];
	
	
	for(i=81; i<= 254; i++){
		if(i < 10){
			sprintf(nome, "./crosssums puzzle_prof/Kakuro_00%d.cfg", i); 
		}
		else{
			if(i < 100){
				sprintf(nome, "./crosssums puzzle_prof/Kakuro_0%d.cfg", i); 
			}
			else{
				sprintf(nome, "./crosssums puzzle_prof/Kakuro_%d.cfg", i); 
			}
		}
		
		system(nome);
		printf(" este foi o Kakuro #%d :)", i);
		getchar();
	}
	
	
	
	exit(0);
}
