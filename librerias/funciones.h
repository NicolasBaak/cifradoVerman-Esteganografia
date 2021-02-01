#ifndef _FUNCIONES
#define _FUNCIONES
// Un desplazamiento en la imagen ya que en un principio esta la informacion del formato de la imagen
// por lo que es necesario desplazarse un poco mas para alterar la imagen y ocultar nuestra información.
#define sz_header_img 100

typedef struct {
  char nf[40];
  int long sz;
} Header;

/* Tipo nodo para arbol */
typedef struct _nodo
{
   char letra;             /* Letra a la que hace referencia el nodo */
   unsigned long int bits; /* Valor de la codificacion de la letra */
   int frecuencia;         /* veces que aparece la letra en el texto o las letras */
   _nodo *sig;             /* Puntero a siguiente nodo de una lista enlazada */
   char nbits;             /* Numero de bits de la codificacion */
   _nodo *cero;            /* Puntero a la rama cero de un arbol */
   _nodo *uno;             /* Puntero a la rama uno de un arbol */
} tipoNodo;                /* Nombre del tipo */

/* Nodo para construir una lista para la tabla de codigos */
typedef struct _tabla
{
   char letra;             /* Letra a la que hace referencia el nodo */
   unsigned long int bits; /* Valor de la codificacion de la letra */
   char nbits;             /* Numero de bits de la codificacion */
   _tabla *sig;            /* Siguiente elemento de la tabla */
} tipoTabla;               /* Nombre del tipo */

/* Variables globales */
tipoTabla *Tabla;

/* Prototipos */
void Cuenta(tipoNodo* &Lista, char c);
void Ordenar(tipoNodo* &Lista);
void InsertarOrden(tipoNodo* &Cabeza, tipoNodo *e);
void BorrarArbol(tipoNodo *n);
void CrearTabla(tipoNodo *n, int l, int v);
void InsertarTabla(char c, int l, int v);
tipoTabla *BuscaCaracter(tipoTabla *Tabla, char c);

void BorrarArbol(tipoNodo *n);
void compres(char* entrada, char* salida);
void decompres(char* entrada, char* salida);
void vernam(char *fn1, char *key, char *fn2);

void printBin(unsigned char b);
void oculta(char *archivoImg, char *archivoDat);
void recupera(char *archivoImg);
int menu();

#include "./funciones.cpp"

#endif