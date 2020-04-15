#include "stdio.h"   //Librerias estandar para flujo de datos y control de pantalla
#include "stdlib.h"
#include "string.h"
#include "ctype.h"
#include "curses.h"
#include <termios.h>
#include "veterinaria.h"    //Libreria propias


long numOfDogs = 0;

long getNumOfDogs(){	//Funcion para obtener numOfDogs de infoVet.dat
	
	FILE *fp;
	fp=fopen("infoVet.dat", "rb+");
	long numberOfDogs;
	fread(&numberOfDogs,sizeof(numberOfDogs),1,fp);
	fclose(fp);
	return numberOfDogs;

}

long sumarPerro(){		//Funcion para modificar infoVet y mantener numOfDogs entre instancias

	FILE *fp;
	fp=fopen("infoVet.dat", "rb+");
	
	long numberOfDogs;
	fread(&numberOfDogs,sizeof(numberOfDogs),1,fp);
	numberOfDogs++;
	fclose(fp);
	fp=fopen("infoVet.dat", "wb+");

	fwrite(&numberOfDogs, sizeof(numberOfDogs), 1, fp);
	fclose(fp);
	return numberOfDogs;

}

long borrarPerro(){		//Funcion para modificar infoVet y mantener numOfDogs entre instancias
	FILE *fp;
	fp=fopen("infoVet.dat", "rb+");
	
	long numberOfDogs;
	fread(&numberOfDogs,sizeof(numberOfDogs),1,fp);
	numberOfDogs--;
	fclose(fp);
	fp=fopen("infoVet.dat", "wb+");
	
	fwrite(&numberOfDogs, sizeof(numberOfDogs), 1, fp);
	fclose(fp);
	return numberOfDogs;
		
		

	
}

void makeRegister(){     //Opcion 1

	struct dogType *perro = malloc(sizeof(struct dogType));
	
	char nombre[32];	
	printw("Ingrese el nombre del perro: \n ");
	refresh();
	scanw("%32s",nombre);

	char *s = &(nombre[0]);				//Formateamos el nombre a Primer caracter mayuscula y el resto minuscula
  	while (*s) {
    	*s = tolower((unsigned char) *s);
    	s++;
  	}
	s = &(nombre[0]);
	*s = toupper((unsigned char) *s);

	strcpy( perro->nombre , nombre );
   
	char tipo[32];
	printw("Ingrese el tipo del perro: \n ");
	refresh();
	scanw("%32s",tipo);
	strcpy( perro->tipo  , tipo );
	
	int edad;
	printw("Ingrese la edad del perro: \n ");
	refresh();
	scanw("%d",&edad);
	perro->edad=edad;

	char raza[16];
	printw("Ingrese la raza del perro: \n");
	refresh();
	scanw("%16s",raza);
	strcpy(perro->raza,raza);

	long estatura;
	printw("Ingrese la estatura del perro: \n");
	refresh();
	scanw("%ld",&estatura);
	perro->estatura=estatura;
    
	float peso;
	printw("Ingrese el peso del perro: \n");
	refresh();
	scanw("%f",&peso);
	perro->peso=peso;

	char sexo[2];
	printw("Ingrese 'M' si es macho o 'F' si es hembra: \n");
	refresh();
	scanw("%2s",sexo);
	strcpy(perro->sexo,sexo);

	//Empezamos la busqueda del lugar en registro, si ya esta ocupada buscamos la siguiente y etc..
	
	FILE *fp = fopen("dataDogs.dat", "rb+");

	long hashNumber = HashFunction(perro->nombre);
	struct dogType *perroCopia = malloc(sizeof(struct dogType));
   
	fseek(fp, (sizeof(struct dogType)*hashNumber), SEEK_SET);
	fread(perroCopia, sizeof(struct dogType), 1, fp);

	perro->initialized = 1;

	//Caso 1: Si la posicion del hash esta vacia, guardamos el perro en esa posicion
	if(perroCopia->initialized==0){ 
			rewind(fp);
			fseek(fp, (sizeof(struct dogType)*hashNumber), SEEK_SET);
			perro->next = -1;
			perro->prev = -1;
			perro->id = hashNumber;
 			fwrite(perro, sizeof(struct dogType), 1, fp);
	}else{
		//Caso 2: la posicion del hash esta ocupada por un perro
		// ... entonces de igual forma se sigue la lista enlazada para saber donde unirlo
			while(perroCopia->next != -1){
				long siguiente= perroCopia->next;
				rewind(fp);
				fseek(fp, (sizeof(struct dogType)*siguiente), SEEK_SET);
				fread(perroCopia, sizeof(struct dogType), 1, fp);
			};
			perro->prev = perroCopia->id;
			perro->next = -1;
		//buscamos un lugar libre 
			while (!feof(fp)){
				fread(perroCopia,sizeof(struct dogType),1,fp);
				if(perroCopia->initialized == 0){
					rewind(fp);
					fseek(fp,sizeof(struct dogType)*perroCopia->id,SEEK_SET);
					break;
				}
			};
		//escribimos
			perro->id = perroCopia->id;
			fwrite(perro, sizeof(struct dogType), 1, fp);
		//por ultimo como la lista es doble enlazada tenemos que poner el next al anterior
			rewind(fp);
			fseek(fp,sizeof(struct dogType)*perro->prev,SEEK_SET);
			fread(perroCopia,sizeof(struct dogType),1,fp);
			perroCopia->next = perro->id;
			rewind(fp);
			fseek(fp,sizeof(struct dogType)*perro->prev,SEEK_SET);
			fwrite(perroCopia,sizeof(struct dogType),1,fp);
		

	}	
	//Verificamos la informacion del perro que ingresamos
	rewind(fp);
	fseek(fp,sizeof(struct dogType)*perro->id,SEEK_SET);
	fread(perro,sizeof(struct dogType),1,fp);
	printw("%s %s %s","El perro ingresado es",perro->nombre,"\n");
	printw("%s %d %s","Su id es:",perro->id,"\n");
	printw("%s %i %s","Su estado es:",perro->initialized,"\n");
	refresh();

	//Guardamos la informacion del nuevo perro en numOfDogs y liberamos memoria
	numOfDogs=sumarPerro();;
	free(perro);
	free(perroCopia);
	fclose(fp);

}

void showRegister(){    //Opcion 2

	//Se muestra la cantidad de perros y se solicita el id a ver
	long id;
	struct dogType *dog = malloc(sizeof(struct dogType));
	printw("%s %d %s","Cantidad de perros registrados:\n", numOfDogs, "\n");
	printw("%s","Ingrese No. ID a ver:");
	scanw("%ld",&id);
	refresh();

	//Se busca en el archivo el struct correspondiente al id
	FILE *fp = fopen("dataDogs.dat", "rb+");
	rewind(fp);
	fseek(fp,(sizeof(struct  dogType)*id),SEEK_SET);
	fread(dog,sizeof(struct dogType),1,fp);

	//Se valida si el struct es un perro valido
	if(dog->initialized==0){
		printw("%s","El id no es valido\n");
		refresh();
		return;
	}

	//Se muestra la informacion del perro ingresado
	printw("%s %s %s","\nEl perro ingresado es",dog->nombre,"\n");
	printw("%s %ld %s","ID ",dog->id,"\n");
	printw("%s %s %s","Tipo ",dog->tipo,"\n");
	printw("%s %d %s","Edad ",dog->edad,"\n");
	printw("%s %d %s","Estatura ",dog->estatura," cm \n");
	printw("%s %f %s","Peso ",dog->peso," Kg \n");
	printw("%s %s %s","Raza ",dog->raza,"\n");
	printw("%s %s %s","Sexo ",dog->sexo,"\n\n");
	printw("%s %i %s","Su estado es:",dog->initialized,"\n");
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
	free(dog);
	
}

void deleteRegister(){  //Opcion 3

	//Se muestra la cantidad de perros y se solicita el id a ver
	long id;
	struct dogType *dog = malloc(sizeof(struct dogType));
	printw("%s %d %s","Cantidad de perros registrados:\n", numOfDogs, "\n");
	printw("%s","Ingrese No. ID del perro que desea borrar:");
	scanw("%ld",&id);
	refresh();

	//Se busca y lee en el archivo el struct correspondiente al id
	FILE *fp = fopen("dataDogs.dat", "rb+");
	rewind(fp);
	fseek(fp,(sizeof(struct dogType)*id),SEEK_SET);
	fread(dog,sizeof(struct dogType),1,fp);

	//Se valida si el struct es un perro valido
	if(dog->initialized==0){
		printw("%s","El id no es valido\n");
		refresh();
		return;
	}

	struct dogType *dogAux = malloc(sizeof(struct dogType));
	
	if(dog->prev != -1 && dog->next != -1){ //Caso 1: Borramos cualquier nodo en medio de la LL
		
		//Leemos el nodo anterior y cambiamos su referencia al next
		fseek(fp,(sizeof(struct dogType)*dog->prev),SEEK_SET);
		fread(dogAux,sizeof(struct dogType),1,fp);
		dogAux->next = dog->next;
		//Reescribimos
		fseek(fp,(sizeof(struct dogType)*dog->prev),SEEK_SET);
		fwrite(dogAux,sizeof(struct dogType),1,fp);

		//Leemos el nodo posterior y cambiamos su referencia al prev
		fseek(fp,(sizeof(struct dogType)*dog->next),SEEK_SET);
		fread(dogAux,sizeof(struct dogType),1,fp);
		dogAux->prev = dog->prev;
		//Reescribimos
		fseek(fp,(sizeof(struct dogType)*dog->next),SEEK_SET);
		fwrite(dogAux,sizeof(struct dogType),1,fp);

		//Escribimos la struct de forma para que se se elimine
		strcpy(dogAux->nombre,"eliminado");
		dogAux->id = dog->id;
		dogAux->initialized = 0;
		dogAux->prev = -1;
		dogAux->next = -1;
		//Eliminamos el nodo
		fseek(fp,(sizeof(struct dogType)*dog->id),SEEK_SET);
		fwrite(dogAux,sizeof(struct dogType),1,fp);


	}else if(dog->prev == -1 && dog->next != -1){//Caso 2: Borramos head de la LL
		
		//Leemos el nodo posterior y cambiamos su referencia al prev por -1 y el id por el del head
		fseek(fp,(sizeof(struct dogType)*dog->next),SEEK_SET);
		fread(dogAux,sizeof(struct dogType),1,fp);
		dogAux->prev = -1;
		//guardamos el id del nodo en idEliminated para poder borrarlo despues
		long idEliminated = dogAux->id;
		dogAux->id = dog->id;
		//Reescribimos el nodo posterior
		fseek(fp,(sizeof(struct dogType)*dog->id),SEEK_SET);
		fwrite(dogAux,sizeof(struct dogType),1,fp);

		//Escribimos la struct de forma para que se se elimine
		dogAux->id = idEliminated;
		strcpy(dogAux->nombre,"eliminado");
		dogAux->initialized = 0;
		dogAux->prev = -1;
		dogAux->next = -1;
		//Eliminamos el nodo 
		fseek(fp,(sizeof(struct dogType)*idEliminated),SEEK_SET);
		fwrite(dogAux,sizeof(struct dogType),1,fp);

		//Leemos la nueva head
		fseek(fp,(sizeof(struct dogType)*dog->id),SEEK_SET);
		fread(dogAux,sizeof(struct dogType),1,fp);
		

		//Si tiene una referencia a next cambiamos la referencia de este nodo a prev al id actual.
		if(dogAux->next !=-1){
			//Leemos el next del nuevo head
			fseek(fp,(sizeof(struct dogType)*dogAux->next),SEEK_SET);
			fread(dogAux,sizeof(struct dogType),1,fp);
			//Cambiamos la referencia
			dogAux->prev = dog->id;
			//Reescribimos
			fseek(fp,(sizeof(struct dogType)*dogAux->id),SEEK_SET);
			fwrite(dogAux,sizeof(struct dogType),1,fp);
		}
	

	} else if(dog->next == -1 && dog->prev != -1){ //Caso 3: Borramos Tail de la LL
		
		//Leemos el nodo previo al tail
		fseek(fp,(sizeof(struct dogType)*dog->prev),SEEK_SET);
		fread(dogAux,sizeof(struct dogType),1,fp);
		//Le asignamos su referencia a next por -1
		dogAux->next = -1;
		//Reescribimos
		fseek(fp,(sizeof(struct dogType)*dog->prev),SEEK_SET);
		fwrite(dogAux,sizeof(struct dogType),1,fp);

		//Escribimos la struct de forma para que se se elimine
		dogAux->id = dog->id;
		strcpy(dogAux->nombre,"eliminado");
		dogAux->initialized = 0;
		dogAux->prev = -1;
		dogAux->next = -1;
		//Eliminamos la tail
		fseek(fp,(sizeof(struct dogType)*dog->id),SEEK_SET);
		fwrite(dogAux,sizeof(struct dogType),1,fp);


	}else{	//Borramos la unica instancia del nombre en el archivo
		
		//Buscamos la instancia en el archivo
		fseek(fp,(sizeof(struct dogType)*dog->id),SEEK_SET);
		fread(dogAux,sizeof(struct dogType),1,fp);
		//Escribimos la struct de forma para que se se elimine
		dogAux->id = dog->id;
		strcpy(dogAux->nombre,"eliminado");
		dogAux->initialized = 0;
		dogAux->prev = -1;
		dogAux->next = -1;
		//Eliminamos el registro
		fseek(fp,(sizeof(struct dogType)*dog->id),SEEK_SET);
		fwrite(dogAux,sizeof(struct dogType),1,fp);

	}
	
	numOfDogs=borrarPerro();
	fclose(fp);
	free(dog);
	free(dogAux);

}

void seekRegister(WINDOW *w){    //Opcion 4
	system("clear");
	//Preguntamos el nombre a buscar
	printw("%s", "Por favor ingrese el nombre del perro a buscar:");
	refresh();
	char nombre[32];
	scanw("%s",&nombre);
	
	//Formateamos el nombre a Primer caracter mayuscula y el resto minuscula
	char *s = &(nombre[0]);
  	while (*s) {
    	*s = tolower((unsigned char) *s);
    	s++;
  	}
	s = &(nombre[0]);
	*s = toupper((unsigned char) *s);

	//Buscamos el hash del nombre para tener la LL donde esten las ocurrencias
	long identifier = HashFunction(nombre);

	struct dogType *dog = malloc(sizeof(struct dogType));
	
	//Leemos la estructura head en el archivo correspondiente al hash
	FILE *fp = fopen("dataDogs.dat", "rb+");
	rewind(fp);
	fseek(fp,(sizeof(struct dogType)*identifier),SEEK_SET);
	fread(dog,sizeof(struct dogType),1,fp);
	
	//Imprimimos todos los nodos que tengan el mismo nombre en la LL
	int counter=0;
	
	char name[32];
	strcpy(name,nombre);
	endwin();

	//imprimimos por consola en vez de ncurses para mostrar todos los perros sin complicaciones

	do{

		if (strcmp(name,dog->nombre)==0){
			printf("%s %s %ld\n",dog->nombre," ID: ",dog->id);
			//refresh();
			counter+=1;
		}

		long siguiente = dog->next;
		
		if(siguiente != -1){
			rewind(fp);
			fseek(fp, (sizeof(struct dogType)*siguiente), SEEK_SET);
			fread(dog, sizeof(struct dogType), 1, fp);
			
			if(strcmp(name,dog->nombre)==0 && dog->next == -1){
				printf("%s %s %ld\n",dog->nombre," ID: ",dog->id);
				//refresh();
				counter+=1;
			}
		}
	}while(dog->next != -1);

	
	//Como imprimimos por consola en vez de getch, utilizamos getchar modificando sus parametros con la libreria termios

	printf("%s","Presione cualquier tecla para continuar..");

	struct termios info;
	tcgetattr(0, &info);          /* get current terminal attirbutes; 0 is the file descriptor for stdin */
	info.c_lflag &= ~ICANON;      /* disable canonical mode */
	info.c_cc[VMIN] = 1;          /* wait until at least one keystroke available */
	info.c_cc[VTIME] = 0;         /* no timeout */
	tcsetattr(0, TCSANOW, &info); /* set immediately */

	getchar();
	system("clear");
	//Liberamos memoria
	free(dog);
	return;

}


void MENU(){            //Funcion menu ciclica que sera ejecutada en main()


	
	WINDOW *w =initscr();
	          //Inicio la capacidad de utilizar funciones de curses.h
	
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
				seekRegister(w);
				
				break;
			}
			case 5:{
				aux=-1;
			}
			

		};

	}while(aux!=-1);

	

	endwin(); //Termino el manejo de pantalla

	system("clear");
}


int main(){
	
	numOfDogs= getNumOfDogs();
	MENU();
	return 0;

}