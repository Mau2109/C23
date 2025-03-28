/* Implementacion basica del comando cat.
   @autor: Ricardo Ruiz Rodriguez
*/
#include <stdio.h>
#define TAM 150

int main(int argc, char *argv[]){
   FILE *archivoPtr;
   char cadena[TAM];
   
   if(argc != 2){
      printf("Uso: %s archivo\n", argv[0]);
   }else if((archivoPtr = fopen(argv[1], "r")) != NULL){
      while(fgets(cadena, TAM, archivoPtr) != NULL)
         fputs(cadena, stdout);
      fclose(archivoPtr);
   }else
      printf("Error al intentar abrir el archivo \"%s\"\n", argv[1]);
   
   return 0;
}
