/* Compresi�n de archivos usando el Algoritmo de Huffman: */
/* (C) Noviembre de 2000 Salvador Pozo Coronado           */
/* Descompresor */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funciones.h"

int main(int argc, char *argv[])
{
    char comprimido[] = "resultados/secreto/comprimido.txt";
    char descomprimido[] = "resultados/secreto/descomprimido.txt"; 
    char cifrado[] = "resultados/secreto/cifrado.txt"; 
    char descifrado[] = "resultados/descifrado/descifrado.txt";

   int seleccion = menu();
   char archivo[30];
   char imagen[30];
   char key[50];

   while(1){
      switch (seleccion)
      {
      case 1:
         
         printf("\n Ocultar archivo:\n");
         printf("nombre-archivo.txt: ");
         scanf("%s", archivo);
         printf("\nclave de cifrado: ");
         scanf("%s", key);
         printf("\nnombre-imagen.pgm: ");
         scanf("%s", imagen);
       

         vernam(archivo, key, cifrado);
         compres(cifrado, comprimido);
         oculta(imagen, comprimido);
         
         printf("Hemos escondido el archivo %s en la imagen %s\n\n\n", archivo, imagen);
         break;
      case 2:
         printf("\n Recuperar archivo:\n");
         printf("nombre-imagen.pgm: ");
         scanf("%s", imagen);
         recupera(imagen);
         printf("\nclave de cifrado: ");
         scanf("%s", key);
         decompres(comprimido, descomprimido);
         vernam(descomprimido, key, descifrado);


         printf("La imagen %s genero el archivo en /resultados/descifrado/%s\n\n\n", imagen, archivo);
         break;
      case 3:
         printf("\n\nGracias por usar nuestro programa :) \n");
         exit(0);
         break;
     default:
            printf ("\n\nSeleccione una opción valida \n\n\n");
            break;
      }
      seleccion = menu();
   }
   return 0;
}
