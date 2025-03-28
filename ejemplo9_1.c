/* Uso de archivos secuenciales para el almacenamiento de datos.
   @autor: Ricardo Ruiz Rodriguez
*/
#include <stdio.h>
#define N 3
#define TAM_NOM 50
#define TAM_TEL 20

typedef struct{
   char nombre[TAM_NOM];
   char telefono[TAM_TEL];
} CONTACTO;

void llenaDirectorio(CONTACTO *, int);
void guardaDirectorio(CONTACTO *, int);

int main(){
   CONTACTO contactos[N];

   llenaDirectorio(contactos, N);
   guardaDirectorio(contactos, N);

   return 0;
}

void llenaDirectorio(CONTACTO *listaContactos, int n){
   int i;

   for(i = 0; i < n; i++){
      printf("%d.\tNombre: ", i+1);
      gets(listaContactos[i].nombre);
      printf("\tTelefono: ");
      gets(listaContactos[i].telefono);
      putchar('\n');
   }
}

void guardaDirectorio(CONTACTO *listaContactos, int n){
   FILE *archivoPtr;
   int i;
   
   printf("Guardando contactos en el archivo...\n");
   if((archivoPtr = fopen("contactos.txt", "a")) == NULL)
      printf("El archivo no pudo ser abierto...\n");
   else{
      for(i = 0; i < n; i++){
         fprintf(archivoPtr, "%s\n", listaContactos[i].nombre);
         fprintf(archivoPtr, "%s\n", listaContactos[i].telefono);
      }
      fclose(archivoPtr);
      printf("Listo!\n");
   }
}
