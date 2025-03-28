/* Cambia un caracter por otro dentro de un archivo.
   @autor: Ricardo Ruiz Rodriguez
*/
#include <stdio.h>
#include <stdlib.h>

void cambiaOcurrenciaCadena(char, char, char *);
void cambiaOcurrenciaArchivo(char, char, char *, char *);

int main(int argc, char *argv[]){
   if(argc != 5)
      printf("Uso: %s char_viejo char_nuevo archivo_fuente "
             "archivo_destino\n", argv[0]);
   else{
      cambiaOcurrenciaArchivo(argv[1][0], argv[2][0], argv[3], argv[4]);
      printf("Listo!\n");
   }
      
   return 0;
}

void cambiaOcurrenciaCadena(char v, char n, char *cad){   
   while(*cad){
      if(*cad == v)
         *cad = n;
      cad++;
   }
}

void cambiaOcurrenciaArchivo(char v, char n, char *a1, char *a2){
   #define TAM 200
   FILE *archivoPtr1, *archivoPtr2;
   char cadena[TAM];
   
   if((archivoPtr1 = fopen(a1, "r")) == NULL){
      printf("Error al abrir el archivo: %s\n", a1);
      exit(1);
   }else if((archivoPtr2 = fopen(a2, "w")) == NULL){
      printf("Error al abrir el archivo: %s\n", a2);
      exit(1);
   }
   
   while(fgets(cadena, TAM, archivoPtr1) != NULL){
      cambiaOcurrenciaCadena(v, n, cadena);
      fputs(cadena, archivoPtr2);
   }
   fclose(archivoPtr1);
   fclose(archivoPtr2);
}
