/* Uso de un archivo secuencial para la lectura de datos.
   @autor: Ricardo Ruiz Rodriguez
*/
#include <stdio.h>
#define N 30
#define TAM_NOM 50
#define TAM_TEL 20

typedef struct{
   char nombre[TAM_NOM];
   char telefono[TAM_TEL];
} CONTACTO;

void imprimeDirectorio(CONTACTO *, int);
int leeDirectorio(CONTACTO *);

int main(){
   CONTACTO contactos[N];
   int n;
   
   n = leeDirectorio(contactos);
   printf("Se leyeron %d contactos del directorio.\n", n);
   imprimeDirectorio(contactos, n);
   
   return 0;
}

int leeDirectorio(CONTACTO *contactos){
   FILE *archivoPtr;
   int i = 0;

   if((archivoPtr = fopen("contactos.txt", "r")) == NULL)
      printf("El archivo no pudo ser abierto.\n");
   else{
      for( ; fgets(contactos[i].nombre, TAM_NOM, archivoPtr) != NULL; i++)
         fgets(contactos[i].telefono, TAM_TEL, archivoPtr);
      fclose(archivoPtr);
   }
   return i;
}

void imprimeDirectorio(CONTACTO *contactos, int n){
   int i;
   
   printf("Lista de amigos:\n");
   for(i = 0; i < n; i++){
      printf("%d. %s", i+1, contactos[i].nombre);
      printf("Tel. %s\n", contactos[i].telefono);
   }
}
