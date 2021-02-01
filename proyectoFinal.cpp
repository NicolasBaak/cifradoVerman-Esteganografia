#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./librerias/funciones.h"

int main(int argc, char *argv[])
{
   //Ruta de los archivos cifrados y comprimidos/descomprimidos
    char comprimido[]    = "resultados/secreto/comprimido.txt";
    char descomprimido[] = "resultados/secreto/descomprimido.txt"; 
    char cifrado[]       = "resultados/secreto/cifrado.txt"; 
    //Ruta del archivo cuando es descomprimido y descifrado
    char descifrado[]    = "resultados/descifrado/descifrado.txt";

   int seleccion = menu();
   char archivo[30];
   char imagen[30];
   char key[30];

   while(1){
      switch (seleccion)
      {
      case 1:

         //Obtener la información:
         printf("\n====  Ocultar archivo:  ====\n\n");
         printf("\nNombre del archivo a ocultar (nombre-archivo.txt):  ");
         scanf("%s", archivo);
         printf("\nEscribe una clave de cifrado (la vas a necesitar para recuperar tu archivo): ");
         scanf("%s", key);
         printf("\nNombre de la imagen donde ocultaremos el archivo (nombre-imagen.bmp): ");
         scanf("%s", imagen);

         //=== Implementando los algoritmos: === 

         //Primero usando el algoritmo de vernam, ciframos el archivo 
         vernam(archivo, key, cifrado);
         //Una vez cifrado, comprimimos el cifrado con el algoritmo de compresión Huffman
         compres(cifrado, comprimido);
         //Finalmente se oculta el archivo comprimido en la imagen usando el algotmo de esteganografía
         oculta(imagen, comprimido);
         getchar();
         printf("\n--> Hemos ocultado el archivo '%s' en la imagen '%s'\n\n", archivo, imagen);
         printf("pulsa cualquier tecla para continuar...");
         getchar();
         break;

      case 2:

         //Obtener la información:
         printf("\n====  Recuperar archivo:  ====\n\n");
         printf("\nNombre de la imagen donde esta oculto el archivo (nombre-imagen.bmp): ");
         scanf("%s", imagen);
         printf("\nEscribe la clave de cifrado para descifrar la imagen '%s': ", imagen);
         scanf("%s", key);

         //=== Implementando los algoritmos: ===
         
         //Primero usamos el algoritmo de recuperacion de esteganografía para obtener el archivo cifrado y comprimido
         recupera(imagen);
         //Usamos el algorimot de Huffman para descomprimir el archivo obtenido anteriormente, obtenemos un archivo cifrado
         decompres(comprimido, descomprimido);
         //Usando el algoritmo el verman junto a la clave de cifrado, desciframos el archivo descomprimido y se almacena en resultados/descifrados/
         vernam(descomprimido, key, descifrado);
         getchar();
         printf("\n--> La imagen '%s' genero el archivo en la carpeta '/resultados/descifrado/%s'\n\n", imagen, descifrado);
         printf("pulsa cualquier tecla para continuar...");
         getchar();
         break;

      case 3:
         //=== Fin del programa ===
         printf("\n\n==> Gracias por usar nuestro programa :)\n\n");
         exit(0);
         break;

     default:
         //En caso de no tener una opcion valida se muestra este mensaje.
         printf ("\n\nSeleccione una opción valida! \n\n\n");
         break;

      }
      seleccion = menu();
   }
   return 0;
}
