#ifndef VETERINARIA_H_INCLUDED
#define VETERINARIA_H_INCLUDED

void Veterinaria();
//void escribir(struct dogType *a);
long HashFunction(char* nombre);

struct dogType
{
    long ID;
    char nombre[32];
    char tipo[32];
    long edad;
    char raza[16];
    long estatura;
    float peso;
    char sexo[1];
    int initialized;
};



#endif // VETERINARIA_H_INCLUDED