#include "stdio.h"   //Librerias estandar para flujo de datos y control de pantalla
#include "stdlib.h"
#include "string.h"
#include "curses.h"
#include "veterinaria.h"    //Libreria propias


void makeRegister(){     //Opcion 1
    
    
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
        scanf("%d",&opc);//pido la opcion deseada por el usuario

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







endwin(); //Termino el manejo de pantalla


    
}


int main(){
    
    MENU();
    return 0;

}