#include "stdio.h"   //Librerias estandar para flujo de datos y control de pantalla
#include "stdlib.h"
#include "string.h"
#include "curses.h"
#include "veterinaria.h"    //Libreria propias

long numOfDogs = TamanoVeterinaria;

void makeRegister(){     //Opcion 1

	struct dogType perro;
	char nombre[32];	
	printw("Ingrese el nombre del perro: \n ");
	refresh();
	scanw("%32s",nombre);
	strcpy( perro.nombre , nombre );
   
	char tipo[32];
	printw("Ingrese el tipo del perro: \n ");
	refresh();
	scanw("%32s",tipo);
	strcpy( perro.tipo  , tipo );
	
	int edad;
	printw("Ingrese la edad del perro: \n ");
	refresh();
	scanw("%d",&edad);
	perro.edad=edad;

	char raza[16];
	printw("Ingrese la raza del perro: \n");
	refresh();
	scanw("%16s",raza);
	strcpy(perro.raza,raza);

	long estatura;
	printw("Ingrese la estatura del perro: \n");
	refresh();
	scanw("%ld",&estatura);
	perro.estatura=estatura;
    
	float peso;
	printw("Ingrese el peso del perro: \n");
	refresh();
	scanw("%f",&peso);
	perro.peso=peso;

	char sexo[2];
	printw("Ingrese 'M' si es macho o 'F' si es hembra: \n");
	refresh();
	scanw("%2s",sexo);
	strcpy(perro.sexo,sexo);

	//Empezamos la busqueda del lugar en registro, si ya esta ocupada buscamos la siguiente y etc..
	
	FILE *fp = fopen("dataDogs.dat", "rb+");
	long hashNumber = HashFunction(perro.nombre);
	struct dogType perroCopia;
   
	fseek(fp, (sizeof(perro)*hashNumber), SEEK_SET);
	fread(&perroCopia, sizeof(perro), 1, fp);

	perro.initialized = 1;

	//Caso 1: Si la posicion del hash esta vacia, guardamos el perro en esa posicion
	if(perroCopia.initialized==0){ 
			rewind(fp);
			fseek(fp, (sizeof(perro)*hashNumber), SEEK_SET);
			perro.next = -1;
			perro.prev = -1;
			perro.id = hashNumber;
 			fwrite(&perro, sizeof(perro), 1, fp);
	}else{
		//Caso 2: la posicion del hash esta ocupada por un perro
		// ... entonces de igual forma se sigue la lista enlazada para saber donde unirlo
			while(perroCopia.next != -1){
				long siguiente= perroCopia.next;
				rewind(fp);
				fseek(fp, (sizeof(perro)*siguiente), SEEK_SET);
				fread(&perroCopia, sizeof(perro), 1, fp);
			};
			perro.prev = perroCopia.id;
			perro.next = -1;
		//buscamos un lugar libre 
			while (!feof(fp)){
				fread(&perroCopia,sizeof(perro),1,fp);
				if(perroCopia.initialized == 0){
					rewind(fp);
					fseek(fp,sizeof(perro)*perroCopia.id,SEEK_SET);
					break;
				}
			};
		//escribimos
			perro.id = perroCopia.id;
			fwrite(&perro, sizeof(perro), 1, fp);
		//por ultimo como la lista es doble enlazada tenemos que poner el next al anterior
			rewind(fp);
			fseek(fp,sizeof(perro)*perro.prev,SEEK_SET);
			fread(&perroCopia,sizeof(perroCopia),1,fp);
			perroCopia.next = perro.id;
			rewind(fp);
			fseek(fp,sizeof(perro)*perro.prev,SEEK_SET);
			fwrite(&perroCopia,sizeof(perroCopia),1,fp);
		

	}	
		rewind(fp);
		fseek(fp,sizeof(perro)*perro.id,SEEK_SET);
		fread(&perro,sizeof(perro),1,fp);
		printw("%s %s %s","El perro ingresado es",perro.nombre,"\n");
		printw("%s %d %s","Su id es:",perro.id,"\n");
		printw("%s %i %s","Su estado es:",perro.initialized,"\n");
		refresh();
		numOfDogs++;
 
	fclose(fp);

}

void showRegister(){    //Opcion 2

	//Se muestra la cantidad de perros y se solicita el id a ver
	long id;
	struct dogType dog;
	printw("%s %d %s","Cantidad de perros registrados:\n", numOfDogs, "\n");
	printw("%s","Ingrese No. ID a ver:");
	scanw("%ld",&id);
	refresh();

	//Se busca en el archivo el struct correspondiente al id
	FILE *fp = fopen("dataDogs.dat", "rb+");
	rewind(fp);
	fseek(fp,(sizeof(dog)*id),SEEK_SET);
	fread(&dog,sizeof(dog),1,fp);

	//Se valida si el struct es un perro valido
	if(dog.initialized==0){
		printw("%s","El id no es valido\n");
		refresh();
		return;
	}

	//Se muestra la informacion del perro ingresado
	printw("%s %s %s","\nEl perro ingresado es",dog.nombre,"\n");
	printw("%s %ld %s","ID ",dog.id,"\n");
	printw("%s %d %s","Edad ",dog.edad,"\n");
	printw("%s %d %s","Estatura ",dog.estatura," cm \n");
	printw("%s %f %s","Peso ",dog.peso," Kg \n");
	printw("%s %s %s","Raza ",dog.raza,"\n");
	printw("%s %s %s","Sexo ",dog.sexo,"\n\n");
	printw("%s %i %s","Su estado es:",dog.initialized,"\n");
	refresh();
	

	//Se pregunta si se quiere ver la historia medica
	
	int verificador = 0;
	do{
		printw("%s","Desea ver la historia medica del perro?[S/N]:");
		refresh();
		char respuesta[1];
		scanw("%s",respuesta);
		//Se comparada la entrada del usuario para verificar que sea valida.
		if( strcmp(respuesta,"S")==0 || strcmp(respuesta,"N")==0 ){
			if(strcmp(respuesta,"S")==0){
				//Para abrir o crear usamos nano, asi que queremos construir una cadena dirFile...
				//de la manera "nano HM/id.txt para almacenar las Historias Medicas"
				char dirFile_1[] = "nano HM/";
				char dirFile_2[] = ".txt";
				char file[21];
				sprintf(file,"%ld",id);
				char * dirFile ;
				//nos aseguramos de tener memoria para la construccion del string dirFile 
				if((dirFile = malloc(strlen(dirFile_1)+strlen(file)+strlen(dirFile_2)+1)) != NULL){
    				dirFile[0] = '\0';   // ensures the memory is an empty string
    				strcat(dirFile,dirFile_1);
    				strcat(dirFile,file);
					strcat(dirFile,dirFile_2);
				}
				//Se abre o crea el archivo en la carpeta HM con el nombre del id del perro
				system(dirFile);
			}
			
			system("clear");

			verificador = -1;
		}
	}while (verificador==0);
	
	fclose(fp);
	
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
		clear();
		//Segun la opcion se va por el caso correspondiente
		switch(opc){
			case 1:{
				makeRegister();
			   	printw("Presione cualquier tecla para continuar..\n");
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

	system("clear");

	endwin(); //Termino el manejo de pantalla
	
}


int main(){
	
	MENU();
	return 0;

}
