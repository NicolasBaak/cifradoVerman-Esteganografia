/* Compresi�n de archivos usando el Algoritmo de Huffman: */
/* (C) Noviembre de 2000 Salvador Pozo Coronado           */
/* Descompresor */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./librerias/funciones.h"

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
         //Obtener la información:
         printf("\n====  Ocultar archivo:  ====\n\n");
         printf("\nNombre del archivo a ocultar (nombre-archivo.txt):  ");
         scanf("%s", archivo);
         printf("\n¿Cual va ser la clave de cifrado? (la vas a necesitar para recuperar tu archivo): ");
         scanf("%s", key);
         printf("\n¿En que imagen se va ocultar la información? (nombre-imagen.pgm): ");
         scanf("%s", imagen);
         //Implementando los algoritmos: 
         vernam(archivo, key, cifrado);
         compres(cifrado, comprimido);
         oculta(imagen, comprimido);

         getchar();
         printf("\n--> Hemos escondido el archivo '%s' en la imagen '%s'\n\n\n", archivo, imagen);
         printf("pulsa cualquier tecla para continuar...");
         getchar();
         break;
      case 2:
         //Obtener la información:
         printf("\n====  Recuperar archivo:  ====\n\n");
         printf("¿Dónde esta la imagen con la información? (nombre-imagen.pgm): ");
         scanf("%s", imagen);
         printf("\n¿Cual es la clave de cifrado? (la necesitamos para descifrar el archivo): ");
         scanf("%s", key);
         //Implementando los algoritmos:
         recupera(imagen);
         decompres(comprimido, descomprimido);
         vernam(descomprimido, key, descifrado);

         getchar();
         printf("\n--> La imagen '%s' genero el archivo en la carpeta '/resultados/descifrado/%s'\n\n\n", imagen, archivo);
         printf("pulsa cualquier tecla para continuar...");
         getchar();
         break;
      case 3:
         printf("\n\n==> Gracias por usar nuestro programa :)\n\n");
         exit(0);
         break;
     default:
            printf ("\n\nSeleccione una opción valida! \n\n\n");
            break;
      }
      seleccion = menu();
   }
   return 0;
}
