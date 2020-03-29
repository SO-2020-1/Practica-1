#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"
#include "locale.h"
#include "math.h"
#include "veterinaria.h"
#define TamanoVeterinaria 1000000L



struct dogType Veterinaria[(2*TamanoVeterinaria)];

long HashFunction(char* nombre){
    long ascii=0;
    long long a=53;
    for(long long currentChar = 0 ; currentChar < strlen(nombre) ; currentChar++){
        unsigned long long _p = powl(a,currentChar);
        ascii += ((unsigned int)(nombre[currentChar]))*(unsigned long long)_p;
        ascii %= (unsigned long)2*TamanoVeterinaria;
    }
    
    return ascii;
}

void escribir(struct dogType *a){
    FILE *fp;
	fp = fopen ( "dataDogs.dat", "wb" );
    fwrite(a, sizeof(struct dogType),2*TamanoVeterinaria,fp);
    fclose ( fp );
 	return;
}


void veterinaria(){
   FILE *fpnombres;
    fpnombres = fopen("nombres.txt","r");
    bool colision = true;
    char nombre[32];
    for(long x = 0; x<(2*TamanoVeterinaria);x++){
        Veterinaria[x].initialized = 0;
        Veterinaria[x].next = -1;
        Veterinaria[x].prev = -1;
    }
    for(long i = 0; i < TamanoVeterinaria ; i++){
        colision = true;
        if(i%1700 == 0)fseek(fpnombres, 0, SEEK_SET);
        fgets(nombre,sizeof(nombre),fpnombres);
        long index = HashFunction(nombre);
        while(Veterinaria[index].next != -1){
            index = Veterinaria[index].next;
        }
        unsigned long previous = index;
        do{
            if(Veterinaria[index].initialized == 1){
            index = (index+1)%(2*TamanoVeterinaria);
            //printf("colision!!\n");
            }
            else colision=false;
        }while(colision);
        Veterinaria[index].initialized=1;
        if(previous != index){
            Veterinaria[index].prev = previous;
            Veterinaria[previous].next = index;
        }
        strcpy(Veterinaria[index].nombre, nombre);
        strcpy(Veterinaria[index].tipo, "perro");
        Veterinaria[index].edad=2*i % 14;
        strcpy(Veterinaria[index].raza, "Golden");
        Veterinaria[index].estatura = 3*i+i % 100;
        Veterinaria[index].peso = (2.3 * i + i % 50) + 10;
        if(i %2 == 0) strcpy(Veterinaria[index].sexo, "H");
        else strcpy(Veterinaria[index].sexo, "M");
        printf("%li\n",i);
        //printf("index: %li , %i \n",index,Veterinaria[index].initialized);
    }
    //for(long x = 0; x<(TamanoVeterinaria);x++)printf("%i",Veterinaria[x].initialized);
    
    escribir(Veterinaria);
    fclose(fpnombres);
    
    
}