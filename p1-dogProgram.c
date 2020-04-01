#include "stdio.h"   //Librerias estandar para flujo de datos y control de pantalla
#include "stdlib.h"
#include "string.h"
#include "curses.h"
#include "veterinaria.h"    //Libreria propias


void makeRegister(){     //Opcion 1


	  
	struct dogType perro;
	char nombre[32];
	 printw("Ingrese el nombre del perro \n ");
	 refresh();
	 scanw("%32s",nombre);
	 strcpy(perro.nombre, nombre);
   
	char tipo[32];
	 printw("Ingrese el tipo del perro \n ");
	 refresh();
	 scanw("%32s",tipo);
	 
   strcpy( perro.tipo,tipo);
	int edad;
	 printw("Ingrese la edad del perro \n ");
	 refresh();
	 scanw("%d",&edad);
	perro.edad=edad;
	char raza[16];
	 printw("Ingrese la raza del perro \n");
	 refresh();
	 scanw("%16s",raza);
	strcpy(perro.raza,raza);
	long estatura;
	 printw("Ingrese la estatura del canchoso \n");
	 refresh();
	 scanw("%ld",&estatura);
	perro.estatura=estatura;
  float peso;
	 printw("Ingrese el peso del perro \n");
	 refresh();
	 scanw("%f",&peso);
	perro.peso=peso;

	 
	char sexo[2];
	 printw("Ingrese 'M' si es macho o 'F' si es hembra \n");
	 refresh();
	 scanw("%2s",sexo);
	 strcpy(perro.sexo,sexo);

	 //vamos a abrir el archivo en moco reemplazar 


long hashNumber=HashFunction(perro.nombre);
/*char nombrePrueba[32] ="Abanero";

long hashPropio=HashFunction("Hernán");
long hashPropio2=HashFunction("Poter");
printw(" %ld %s %ld", hashPropio," ", hashPropio2, " \n");
refresh();*/
//la estructura pesa    120 bytes
//vamos a esa posición en el archivo y le buscamos el dato initialized

	short initiliazed;
   struct  dogType perroCopia;
   
   
    
   
   FILE *fp = fopen("dataDogs.dat", "rb+");
	   fseek(fp, (sizeof(perro)*hashNumber), SEEK_SET);
   
   
   
   
	fread(&perroCopia, sizeof(perro), 1, fp);
	printw("%d %s",perroCopia.initialized ,perroCopia.nombre, " \n");
	 refresh();
	if(perroCopia.initialized==0){
		fseek(fp, 0, SEEK_SET);
		fseek(fp, (sizeof(perro)*hashNumber), SEEK_SET);
		perro.initialized=1;
 		fwrite(&perro, sizeof(perro), 1, fp);

	}else
	{
		while(perroCopia.next!=-1){
			long siguiente= perroCopia.next;
			fseek(fp, 0, SEEK_SET);
			fseek(fp, (sizeof(perro)*siguiente), SEEK_SET);
			fread(&perroCopia, sizeof(perro), 1, fp);
			if(perroCopia.next==-1){
				perro.prev=perroCopia.id;
				perro.next=-1;
				break;

			}


		}
		while(perroCopia.initialized!=0){
			fread(&perroCopia, sizeof(perro), 1, fp);
		
		}

		
		fseek(fp, (sizeof(perro)*perroCopia.), SEEK_SET);
		perro.initialized=1;
 		fwrite(&perro, sizeof(perro), 1, fp);





	}
	

   
	fclose(fp);
	
}

void showRegister(){    //Opcion 2
	
}

void deleteRegister(){  //Opcion 3

}

void seekRegister(){    //Opcion 4

}



void MENU(){            //Funcion menu ciclica que sera ejecutada en main()

	initscr();          //Inicio la capacidad de utilizar funciones de curses.h
	
	int opc = 0;        //Declaracion de variable para elegir opcion
	int aux = 0;        //Variable para salir del ciclo
	
	do{
		
		clear();        //limpio pantalla
	   
						//Opciones, se usa printw en vez de printf para manejo de pantalla

		printw("MENU\n\n");
		printw("1.Ingresar registro.\n");
		printw("2.Ver registro.\n");
		printw("3.Borrar registro.\n");
		printw("4.Buscar registro.\n");
		printw("5.SALIR.\n");
		refresh();      //Permite mostrar todos los printw
		scanw("%d",&opc);//pido la opcion deseada por el usuario

		//Segun la opcion se va por el caso correspondiente
		switch(opc){
			case 1:{
				makeRegister();
			   
				getch();
				clear();
				break;
			}
			case 2:{
				showRegister();
				printw("Presione cualquier tecla para continuar..\n");
				getch();
				clear();
				break;
			}
			case 3:{
				deleteRegister();
				printw("Presione cualquier tecla para continuar..\n");
				getch();
				clear();
				break;
			}
			case 4:{
				seekRegister();
				printw("Presione cualquier tecla para continuar..\n");
				getch();    
				clear();
				break;
			}
			case 5:{
				aux=-1;
			}
			

		};

	}while(aux!=-1);







endwin(); //Termino el manejo de pantalla


	
}


int main(){
	//veterinaria();
	MENU();
	return 0;

}
