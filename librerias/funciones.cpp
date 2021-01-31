#include "funciones.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int menu(){
   int p = 0;
   int seleccion;
   while(p!=1){
      system("clear");
   printf("===> Bienvenido a nuestro programa para ocultar/mostrar informacion secreta <===\n\n");
   printf("1.- Ocultar información\n");
   printf("2.- Recuperar información\n");
   printf("3.- Salir del programa\n");
   printf("-Ingresa su numero: 1, 2 ó 3\nOpción: ");
   seleccion = getchar();
   if( isdigit(seleccion)){
      seleccion -=48;
      if(seleccion==1 || seleccion==2 || seleccion==3)
         p = 1;
   }else{
      printf("\n\nError: ¡¡Ingresa una opcion valida!!\n\n\n");
      
   }
   }

   return seleccion;
}

void vernam(char *fn1, char *key, char *fn2){
	FILE *f1,*f2;
	char ch;
	int numbytes;
	int keysize = strlen(key);
	unsigned int ct=0;
	
	f1 = fopen(fn1,"rb");
	if(f1==NULL){
		printf("El Archivo %s no Existe!\n", fn1);
		exit(0);
	}
	f2 = fopen(fn2,"wb");
	
	
	while ( (numbytes = fread(&ch, sizeof(char),1, f1)) > 0){
		//terminado
		ch = ch ^ key[ ct ];
		fwrite( &ch, sizeof(char),1, f2);
		if(ct<keysize)
			ct++;
		else
			ct=0;
}
	
	/* Cierre de archivos */
	fclose(f1);
	fclose(f2);
}

/* Funci�n recursiva para borrar un arbol */
void BorrarArbol(tipoNodo *n)
{
   if(n->cero) BorrarArbol(n->cero);
   if(n->uno)  BorrarArbol(n->uno);
   free(n);
}

/* Funcion para descomprimir un archivo */
void decompres(char* entrada, char* salida){
   tipoNodo *Arbol;        /* Arbol de codificaci�n */
   long int Longitud;      /* Longitud de fichero */
   int nElementos;         /* Elementos de �rbol */
   unsigned long int bits; /* Almacen de bits para decodificaci�n */
   FILE *fe, *fs;          /* Ficheros de entrada y salida */

   tipoNodo *p, *q;        /* Auxiliares */
   unsigned char a;
   int i, j;


   /* Crear un arbol con la informaci�n de la tabla */
   Arbol = (tipoNodo *)malloc(sizeof(tipoNodo)); /* un nodo nuevo */
   Arbol->letra = 0;
   Arbol->uno = Arbol->cero = NULL;
   fe = fopen(entrada, "rb");
   fread(&Longitud, sizeof(long int), 1, fe); /* Lee el n�mero de caracteres */
   fread(&nElementos, sizeof(int), 1, fe); /* Lee el n�mero de elementos */
   for(i = 0; i < nElementos; i++) /* Leer todos los elementos */
   {
      p = (tipoNodo *)malloc(sizeof(tipoNodo)); /* un nodo nuevo */
      fread(&p->letra, sizeof(char), 1, fe); /* Lee el car�cter */
      fread(&p->bits, sizeof(unsigned long int), 1, fe); /* Lee el c�digo */
      fread(&p->nbits, sizeof(char), 1, fe); /* Lee la longitud */
      p->cero = p->uno = NULL;
      /* Insertar el nodo en su lugar */
      j = 1 << (p->nbits-1);
      q = Arbol;
      while(j > 1)
      {
         if(p->bits & j) /* es un uno*/
            if(q->uno) q = q->uno;   /* Si el nodo existe, nos movemos a �l */
            else                     /* Si no existe, lo creamos */
            {
               q->uno = (tipoNodo *)malloc(sizeof(tipoNodo)); /* un nodo nuevo */
               q = q->uno;
               q->letra = 0;
               q->uno = q->cero = NULL;
            }
         else /* es un cero */
            if(q->cero) q = q->cero; /* Si el nodo existe, nos movemos a �l */
            else                     /* Si no existe, lo creamos */
            {
               q->cero = (tipoNodo *)malloc(sizeof(tipoNodo)); /* un nodo nuevo */
               q = q->cero;
               q->letra = 0;
               q->uno = q->cero = NULL;
            }
         j >>= 1;  /* Siguiente bit */
      }
      /* Ultimo Bit */
      if(p->bits & 1) /* es un uno*/
         q->uno = p;
      else            /* es un cero */
         q->cero = p;
   }
   /* Leer datos comprimidos y extraer al fichero de salida */
   bits = 0;
   fs = fopen(salida, "w");
   /* Lee los primeros cuatro bytes en la dobel palabra bits */
   fread(&a, sizeof(char), 1, fe);
   bits |= a;
   bits <<= 8;
   fread(&a, sizeof(char), 1, fe);
   bits |= a;
   bits <<= 8;
   fread(&a, sizeof(char), 1, fe);
   bits |= a;
   bits <<= 8;
   fread(&a, sizeof(char), 1, fe);
   bits |= a;
   j = 0; /* Cada 8 bits leemos otro byte */
   q = Arbol;
   /* Bucle */
   do {
      if(bits & 0x80000000) q = q->uno; else q = q->cero; /* Rama adecuada */
      bits <<= 1;           /* Siguiente bit */
      j++;
      if(8 == j)            /* Cada 8 bits */
      {
         i = fread(&a, sizeof(char), 1, fe); /* Leemos un byte desde el fichero */
         bits |= a;                    /* Y lo insertamos en bits */
         j = 0;                        /* No quedan huecos */
      }                                
      if(!q->uno && !q->cero)          /* Si el nodo es una letra */
      {
         putc(q->letra, fs);           /* La escribimos en el fich de salida */
         Longitud--;                   /* Actualizamos longitud que queda */
         q=Arbol;                      /* Volvemos a la raiz del �rbol */
      }
   } while(Longitud);                  /* Hasta que acabe el fichero */
   /* Procesar la cola */

   fclose(fs);                         /* Cerramos ficheros */
   fclose(fe);

   BorrarArbol(Arbol);                 /* Borramos el �rbol */
}

/* Funcion para comprimir un archivo */
void compres(char* entrada, char* salida){
    tipoNodo *Lista;       /* Lista de letras y frecuencias */
   tipoNodo *Arbol;       /* Arbol de letras y frecuencias */

   FILE *fe, *fs;         /* Ficheros de entrada y salida */
   char c;                /* variables auxiliares */
   tipoNodo *p;
   tipoTabla *t;
   int nElementos;        /* N�mero de elementos en tabla */
   long int Longitud = 0; /* Longitud del fichero original */

   unsigned long int dWORD; /* Doble palabra usada durante la codificaci�n */
   int nBits;               /* N�mero de bits usados de dWORD */

   Lista = NULL;
   /* Fase 1: contar frecuencias */
   fe = fopen(entrada, "r");
   while((c = fgetc(fe)) != EOF)
   {
      Longitud++;       /* Actualiza la cuenta de la longitud del fichero */
      Cuenta(Lista, c); /* Actualiza la lista de frecuencias */
   }
   fclose(fe);

   /* Ordenar la lista de menor a mayor */
   Ordenar(Lista);

   /* Crear el arbol */
   Arbol = Lista;
   while(Arbol && Arbol->sig) /* Mientras existan al menos dos elementos en la lista */
   {
      p = (tipoNodo *)malloc(sizeof(tipoNodo)); /* Un nuevo �rbol */
      p->letra = 0;                             /* No corresponde a ninguna letra */
      p->uno = Arbol;                           /* Rama uno */
      Arbol = Arbol->sig;                       /* Siguiente nodo en */
      p->cero = Arbol;                          /* Rama cero */
      Arbol = Arbol->sig;                       /* Siguiente nodo */
      p->frecuencia = p->uno->frecuencia +
                      p->cero->frecuencia;      /* Suma de frecuencias */
      InsertarOrden(Arbol, p);                  /* Inserta en nuevo nodo */
   }                                            /* orden de frecuencia */

   /* Construir la tabla de c�digos binarios */
   Tabla = NULL;
   CrearTabla(Arbol, 0, 0);

   /* Crear fichero comprimido */
   fs = fopen(salida, "wb");
   /* Escribir la longitud del fichero */
   fwrite(&Longitud, sizeof(long int), 1, fs);
   /* Cuenta el n�mero de elementos de tabla */
   nElementos = 0;
   t = Tabla;
   while(t)
   {
      nElementos++;
      t = t->sig;
   }
   /* Escribir el n�mero de elemenos de tabla */
   fwrite(&nElementos, sizeof(int), 1, fs);
   /* Escribir tabla en fichero */
   t = Tabla;
   while(t)
   {
      fwrite(&t->letra, sizeof(char), 1, fs);
      fwrite(&t->bits, sizeof(unsigned long int), 1, fs);
      fwrite(&t->nbits, sizeof(char), 1, fs);
      t = t->sig;
   }

   /* Codificaci�n del fichero de entrada */
   fe = fopen(entrada, "r");
   dWORD = 0; /* Valor inicial. */
   nBits = 0; /* Ning�n bit */
   while((c = fgetc(fe)) != EOF)
   {
      /* Busca c en tabla: */
      t = BuscaCaracter(Tabla, c);
      /* Si nBits + t->nbits > 32, sacar un byte */
      while(nBits + t->nbits > 32)
      {
         c = dWORD >> (nBits-8);           /* Extrae los 8 bits de mayor peso */
         fwrite(&c, sizeof(char), 1, fs);  /* Y lo escribe en el fichero */
         nBits -= 8;                       /* Ya tenemos hueco para 8 bits m�s */
      }
      dWORD <<= t->nbits; /* Hacemos sitio para el nuevo caracter */
      dWORD |= t->bits;   /* Insertamos el nuevo caracter */
      nBits += t->nbits;  /* Actualizamos la cuenta de bits */
   }
   /* Extraer los cuatro bytes que quedan en dWORD*/
   while(nBits>0)
   {
      if(nBits>=8) c = dWORD >> (nBits-8);
      else c = dWORD << (8-nBits);
      fwrite(&c, sizeof(char), 1, fs);
      nBits -= 8;
   }

   fclose(fe);  /* Cierra los ficheros */
   fclose(fs);

   /* Borrar Arbol */
   BorrarArbol(Arbol);

   /* Borrar Tabla */
   while(Tabla)
   {
      t = Tabla;
      Tabla = t->sig;
      free(t);
   }
}

/* Actualiza la cuenta de frecuencia del car�cter c */
/* El puntero a Lista se pasa por referencia, ya que debe poder cambiar */
/* Ya sea por que la lista est� vac�a, o porque el nuevo elemento sea el */
/* primero */
void Cuenta(tipoNodo* &Lista, char c)
{
   tipoNodo *p, *a, *q;

   if(!Lista)  /* Si la lista est� vac�a, el nuevo nodo ser� Lista */
   {
      Lista = (tipoNodo *)malloc(sizeof(tipoNodo)); /* Un nodo nuevo */
      Lista->letra = c;                             /* Para c */
      Lista->frecuencia = 1;                        /* en su 1� aparici�n */
      Lista->sig = Lista->cero = Lista->uno = NULL;
   }
   else
   {
      /* Buscar el caracter en la lista (ordenada por letra) */
      p = Lista;
      a = NULL;
      while(p && p->letra < c)
      {
         a = p;      /* Guardamos el elemento actual para insertar */
         p = p->sig; /* Avanzamos al siguiente */
      }
      /* Dos casos: */
      /* 1) La letra es c se encontr� */
      if(p && p->letra == c) p->frecuencia++; /* Actualizar frecuencia */
      else
      /* 2) La letra c no se encontr�*/
      {
         /* Insertar un elemento nuevo */
         q = (tipoNodo *)malloc(sizeof(tipoNodo));
         q->letra = c;
         q->frecuencia = 1;
         q->cero = q->uno = NULL;
         q->sig = p;        /* Insertar entre los nodos p */
         if(a) a->sig = q;  /* y a */
         else Lista = q;    /* Si a es NULL el nuevo es el primero */
      }
   }
}

/* Ordena Lista de menor a mayor por frecuencias */
/* De nuevo pasamos el puntero a lista por referencia */
void Ordenar(tipoNodo* &Lista)
{
   tipoNodo *Lista2, *a;

   if(!Lista) return; /* Lista vacia */
   Lista2 = Lista;
   Lista = NULL;
   while(Lista2)
   {
      a = Lista2;              /* Toma los elementos de Lista2 */
      Lista2 = a->sig;
      InsertarOrden(Lista, a); /* Y los inserta por orden en Lista */
   }
}

/* Inserta el elemento e en la Lista ordenado por frecuencia de menor a mayor */
/* El puntero a Cabeza se pasa por referencia */
void InsertarOrden(tipoNodo* &Cabeza, tipoNodo *e)
{
   tipoNodo *p, *a;

   if(!Cabeza) /* Si Cabeza en NULL, e es el primer elemento */
   {
      Cabeza = e;
      Cabeza->sig = NULL;
   }
   else
   {
       /* Buscar el caracter en la lista (ordenada por frecuencia) */
       p = Cabeza;
       a = NULL;
       while(p && p->frecuencia < e->frecuencia)
       {
          a = p;      /* Guardamos el elemento actual para insertar */
          p = p->sig; /* Avanzamos al siguiente */
       }
       /* Insertar el elemento */
       e->sig = p;
       if(a) a->sig = e;   /* Insertar entre a y p */
       else Cabeza = e;    /* el nuevo es el primero */
    }
}

/* Funci�n recursiva para crear Tabla */
/* Recorre el �rbol cuya raiz es n y le asigna el c�digo v de l bits */
void CrearTabla(tipoNodo *n, int l, int v)
{
   if(n->uno)  CrearTabla(n->uno, l+1, (v<<1)|1);
   if(n->cero) CrearTabla(n->cero, l+1, v<<1);
   if(!n->uno && !n->cero) InsertarTabla(n->letra, l, v);
}

/* Insertar un elemento en la tabla */
void InsertarTabla(char c, int l, int v)
{
   tipoTabla *t, *p, *a;

   t = (tipoTabla *)malloc(sizeof(tipoTabla)); /* Crea un elemento de tabla */
   t->letra = c;                               /* Y lo inicializa */
   t->bits = v;
   t->nbits = l;

   if(!Tabla)         /* Si tabla es NULL, entonces el elemento t es el 1� */
   {
      Tabla = t;
      Tabla->sig = NULL;
   }
   else
   {
       /* Buscar el caracter en la lista (ordenada por frecuencia) */
       p = Tabla;
       a = NULL;
       while(p && p->letra < t->letra)
       {
          a = p;      /* Guardamos el elemento actual para insertar */
          p = p->sig; /* Avanzamos al siguiente */
       }
       /* Insertar el elemento */
       t->sig = p;
       if(a) a->sig = t;  /* Insertar entre a y p */
       else Tabla = t;    /* el nuevo es el primero */
    }
}

/* Buscar un caracter en la tabla, devuelve un puntero al elemento de la tabla */
tipoTabla *BuscaCaracter(tipoTabla *Tabla, char c)
{
   tipoTabla *t;

   t = Tabla;
   while(t && t->letra != c) t = t->sig;
   return t;
}

/* Estega.c */
void printBin(unsigned char b){
  int i;
  for (i=7; i>=0; i--) { // muestra en binario
	short bit = ((b >> i) & 1);
	printf("%d", bit);
  } 
}

void oculta(char* archivoImg, char* archivoDat){
	unsigned char B, M = 1; 
	unsigned char c = 0;
	char sn='n', nnombre[50];
	
	FILE *out, *orig;
	Header cabeza;
	
	short cont=0, cont2, cont1;
	size_t result;
	long lSizeOut, lSizeOrig, i=0, max=0;
	unsigned char * bufferOrig, * bufferOut;
	
	
	out=fopen(archivoImg, "rb+"); // la imagen
	orig=fopen(archivoDat, "rb"); //el archivo a ocultar
	
	if(out==NULL){ 
		printf("error al leer el archivo  %s !", archivoImg);
		exit(1);
	}
	
	if(orig==NULL){ 
		printf("error al leer el archivo  %s !", archivoDat);
		exit(1);
	}
	
	// tamanio del archivo de imagen
	fseek (out , 0 , SEEK_END);
	lSizeOut = ftell (out);
	rewind (out);
	
	
	// tamanio del archivo de origen
	fseek (orig , 0 , SEEK_END);
	lSizeOrig = ftell (orig);
	rewind (orig);
	
	
	if(lSizeOrig*8+16+sizeof(Header)*8 >= lSizeOut ){
		printf("Se necesita un archivo de imagen mas grande!\n");
		exit(1);
	}
	
	
	// memoria para  el archivo de origen:
	bufferOrig = (unsigned char*) malloc (sizeof(char)*lSizeOrig + sizeof(Header));
	if (bufferOrig == NULL) {fputs ("error de memoria orig",stderr); exit (2);}
	
	// memoria para el archivo de salida
	bufferOut = (unsigned char*) malloc (sizeof(char)*lSizeOut );
	if (bufferOut == NULL) {fputs ("error de memoria out",stderr); exit (2);}
	
	// copia el archivo origen texto al buffer:
	result = fread (&bufferOrig[sizeof(Header)],1,lSizeOrig,orig);
	if (result != lSizeOrig) {fputs ("error de lectura orig ",stderr); exit (3);}	
	
	
	// copia el archivo out imagen al buffer:
	result = fread (bufferOut,1,lSizeOut,out);
	if (result != lSizeOut) {fputs ("error de lectura out ",stderr); exit (3);}
	
	
	strcpy(cabeza.nf, archivoDat);
	cabeza.sz = lSizeOrig;
	memcpy(bufferOrig, &cabeza, sizeof(Header));
	
	cont=0;  
	
	
	//brincar encabezado del formato de imagen (sz_header_img)
	
	long k = sz_header_img;
	
	for(i=0; i<(lSizeOrig+sizeof(Header) ); i++){
		
		B = bufferOrig[i];
		/*
		if(sn!='s') {
			printf("\nBytes originales en la imagen\n");
			printBin(bufferOut[k]);  printf("\n");
			printBin(bufferOut[k+1]); printf("\n");
			printBin(bufferOut[k+2]); printf("\n");
			printBin(bufferOut[k+3]); printf("\n");
			printBin(bufferOut[k+4]); printf("\n");
			printBin(bufferOut[k+5]); printf("\n");
			printBin(bufferOut[k+6]); printf("\n");
			printBin(bufferOut[k+7]); printf("\n");
		}
		*/
		
		// Proceso de codificacion - esteganografia 
		bufferOut[k] =  (B >> 7) | (bufferOut[k] & 254); //borrar antes el primer bit   
		bufferOut[k+1] =  ((B >> 6) & 1) | (bufferOut[k+1] & 254);
		bufferOut[k+2] =  ((B >> 5) & 1) | (bufferOut[k+2] & 254);	
		bufferOut[k+3] =  ((B >> 4) & 1) | (bufferOut[k+3] & 254);
		bufferOut[k+4] =  ((B >> 3) & 1) | (bufferOut[k+4] & 254);
		bufferOut[k+5] =  ((B >> 2) & 1) | (bufferOut[k+5] & 254);
		bufferOut[k+6] =  ((B >> 1) & 1) | (bufferOut[k+6] & 254);
		bufferOut[k+7] =  (B & 1) | (bufferOut[k+7] & 254);
		
      /*
		if(sn!='s') {
			printf("\nCaracter: %c   binario: ", B); printBin(B);
			printf("\n\n");
			printf("Codificacion en 8 bytes (bit menos significativo): \n");
			printBin(bufferOut[k]);  printf("\n");
			printBin(bufferOut[k+1]); printf("\n");
			printBin(bufferOut[k+2]); printf("\n");
			printBin(bufferOut[k+3]); printf("\n");
			printBin(bufferOut[k+4]); printf("\n");
			printBin(bufferOut[k+5]); printf("\n");
			printBin(bufferOut[k+6]); printf("\n");
			printBin(bufferOut[k+7]);
			printf("\n\n");
			printf("Codificar todo sin mostrar s=si, otra_tecla=no: ");
			sn=getchar();
		}
		*/
		k=k+8;
		
	}
	
	
	//guardar imagen modificada
	rewind (out);
	result = fwrite (bufferOut,1,lSizeOut,out);
	//printf("\nArchivo %s actualizado con datos ocultos!\n", archivoImg);
	
	free(bufferOrig);
	free(bufferOut);
	
	fclose(orig);
	fclose(out);
	
}

void recupera(char *archivoImg){
	unsigned char B, M = 1; 
	unsigned char c = 0;
	char sn='n', nnombre[50];
	
	FILE *out, *nuevo;
	Header cabeza;
	
	short cont=0, cont2, cont1;
	size_t result;
	long lSizeOut, lSizeOrig, i=0, max=0;
	unsigned char buffer1[sizeof(Header)], * bufferOut, *buffer2;
	
	out=fopen(archivoImg, "rb"); // la imagen
	
	if(out==NULL){ 
		printf("error al leer el archivo  %s !", archivoImg);
		exit(1);
	}
		
	// tamanio del archivo de imagen
	fseek (out , 0 , SEEK_END);
	lSizeOut = ftell (out);
	rewind (out);
	
	
	// memoria para cargar el archivo de imagen
	bufferOut = (unsigned char*) malloc (sizeof(char)*lSizeOut );
	if (bufferOut == NULL) {fputs ("error de memoria out",stderr); exit (2);}
	
	// copia el archivo out imagen al buffer:
	result = fread (bufferOut,1,lSizeOut,out);
	if (result != lSizeOut) {fputs ("error de lectura out ",stderr); exit (3);}
	
	
	cont=0;  
	
	//printf("\nProceso de recuperacion de archivo\n");

	//brincar encabezado del formato de imagen (sz_header_img)
	
	long k = sz_header_img;

	unsigned char dato;
	k = sz_header_img;
	
	//printf("\nRecuperando nombre y longitud del archivo oculto\n");
	//leer y decodificar el encabezado
	for(i=0; i<(sizeof(Header) ); i++){
		dato=0;
		dato = bufferOut[k]&1;
		dato<<=1;
		dato = dato | (bufferOut[k+1]&1);
		dato<<=1;
		dato = dato | (bufferOut[k+2]&1);
		dato<<=1;
		dato = dato | (bufferOut[k+3]&1);
		dato<<=1;
		dato = dato | (bufferOut[k+4]&1);
		dato<<=1;
		dato = dato | (bufferOut[k+5]&1);
		dato<<=1;
		dato = dato | (bufferOut[k+6]&1);
		dato<<=1;
		dato = dato | (bufferOut[k+7]&1);
		buffer1[i]=dato;
		k=k+8;
	}
	
	Header h2;
	memcpy(&h2, buffer1, sizeof(Header));

	//printf("\nNombre: %s   bytes: %ld \n", h2.nf, h2.sz);
	
	buffer2 = (unsigned char*) malloc (sizeof(char)*h2.sz );
	if (buffer2 == NULL) {fputs ("error de memoria buffer2 ",stderr); exit (2);}
	
	
	//printf("\nRecuperando datos del archivo oculto\n");
	//leer y decodificar el contenido del archivo oculto
	for(i=0 ; i<(h2.sz); i++){
		dato=0;
		dato = bufferOut[k]&1;
		dato<<=1;
		dato = dato | (bufferOut[k+1]&1);
		dato<<=1;
		dato = dato | (bufferOut[k+2]&1);
		dato<<=1;
		dato = dato | (bufferOut[k+3]&1);
		dato<<=1;
		dato = dato | (bufferOut[k+4]&1);
		dato<<=1;
		dato = dato | (bufferOut[k+5]&1);
		dato<<=1;
		dato = dato | (bufferOut[k+6]&1);
		dato<<=1;
		dato = dato | (bufferOut[k+7]&1);
		buffer2[i]=dato;
		// printf("%c",buffer2[i]);
		k=k+8;
	}
	
	//guardar el archivo recuperado
	strcpy(nnombre, h2.nf);
	strcat(nnombre,"_bak");
	nuevo=fopen(nnombre, "wb"); //el archivo recuperado
	
	if(out==NULL){ 
		printf("error al leer el archivo  %s !", nnombre );
		exit(1);
	}
	
	result = fwrite (buffer2,1,h2.sz,nuevo);
	//printf("\nSe genero archivo: %s \n", nnombre);
	
	free(buffer2);
	free(bufferOut);
	
	fclose(out);
	fclose(nuevo);
	
	//printf("\nArchivo oculto %s recuperado con exito!\n",h2.nf);
	
}
