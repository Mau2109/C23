/* Lectura y actualizacion de un archivo de acceso aleatorio.
   @autor: Ricardo Ruiz Rodriguez
*/
#include <stdio.h>
#define TAM_NOM 50
#define TAM_TEL 20
#define SALIR 4

typedef struct{
   int num;
   char nombre[TAM_NOM];
   char telefono[TAM_TEL];
} CONTACTO;

int leeDirectorio(FILE *);
void agregaContacto(FILE *);
void imprimeContacto(const CONTACTO *);
void leeContacto(CONTACTO *);
int menu();

int main(){
   FILE *archivoPtr;
   int opcion;   
   
   if((archivoPtr = fopen("contactos.dat", "rb+")) == NULL)
      printf("El archivo no pudo ser abierto.\n");
   else{
      leeDirectorio(archivoPtr);
      while((opcion = menu()) != SALIR){
         switch(opcion){
            case 1: agregaContacto(archivoPtr);
               break;
            case 2: /* Bajas */
               break;
            case 3: /* Modificaciones */
               break;
            default: printf("\n\tOpcion no valida.\n\n");
         }
      }   
      fclose(archivoPtr);
   }
   return 0;
}

void agregaContacto(FILE *archivoPtr){ 
   int num;
   CONTACTO contacto;

   do{
      printf("Contacto numero? (1 - 100): ");
      scanf("%d", &num);
   }while(num < 1 || num > 100);
   getchar(); /* Se "come" el ENTER del numero de contacto */
   
   fseek(archivoPtr, (num - 1) * sizeof(CONTACTO), SEEK_SET);
   fread(&contacto, sizeof(CONTACTO), 1, archivoPtr);

   if(contacto.num != 0)
      printf("\n\tEl contacto %d ya tiene informacion.\n\n", num);
   else{
      contacto.num = num;
      leeContacto(&contacto);
      fseek(archivoPtr, (num - 1) * sizeof(CONTACTO), SEEK_SET);
      fwrite(&contacto, sizeof(CONTACTO), 1, archivoPtr);
      printf("\n\tContacto agregado...\n\n");
   }
}

int leeDirectorio(FILE *archivoPtr){
   int n = 0;
   CONTACTO contacto;
 
   printf("Leyendo archivo...\n");
   while(fread(&contacto, sizeof(CONTACTO), 1, archivoPtr) > 0)
      if(contacto.num != 0){
         n++;
         imprimeContacto(&contacto);
      }
   printf("Existen %d contactos registrados.\n", n);
 
   return n;
}

void imprimeContacto(const CONTACTO *contacto){
   printf("%d. ", contacto -> num);
   printf("%s", contacto -> nombre);
   printf("Tel. %s\n", contacto -> telefono);
}

void leeContacto(CONTACTO *contacto){
   printf("\tNombre?: ");
   fgets(contacto -> nombre, TAM_NOM, stdin);
   printf("\tTelefono?: ");
   fgets(contacto -> telefono, TAM_TEL, stdin);
   putchar('\n');
}

int menu(){ 
   int op;

   printf("\n\t**** MENU ****\n");
   printf("[1] Agregar contacto\n");
   printf("[2] Eliminar contacto\n");
   printf("[3] Modificar contacto\n");
   printf("[4] Salir\n");
   printf("Opcion?: ");
   scanf("%d", &op);

   return op;
}
