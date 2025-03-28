
/* Abstraccion de numeros racionales (segunda version).
   Se ejemplifica  el paso de estructuras por valor y referencia,
   asi como la notacion con operador punto "." y flecha "->".
   @autor: Ricardo Ruiz Rodriguez
*/
#include <stdio.h>
#include <stdlib.h>

typedef struct{
   int p, q;
} RACIONAL;

char leeExpresion(RACIONAL *, RACIONAL *);
RACIONAL hazOperacion(RACIONAL, RACIONAL, char);

int main(){
   RACIONAL r1;
   RACIONAL r2;
   RACIONAL res;
   char op;

   op = leeExpresion(&r1, &r2);
   res = hazOperacion(r1, r2, op);   

   printf("%d/%d %c %d/%d = %d/%d\n",
          r1.p, r1.q, op, r2.p, r2.q, res.p, res.q);
   
   return 0;
}

RACIONAL hazOperacion(RACIONAL r1, RACIONAL r2, char op){
   RACIONAL r;
   
   switch(op){
      case '+':
         break;
      case '-':
         break;
      case '*':
         r.p = r1.p * r2.p;
         r.q = r1.q * r2.q;
         break;
      case '/':
         r.p = r1.p * r2.q;
         r.q = r1.q * r2.p;
         break;
      default:
         printf("\aOperador NO valido:\"%c\"\n", op);
         exit(0);
   }
   
   return r;
}

char leeExpresion(RACIONAL *r1, RACIONAL *r2){
   char op;
   
   do{
      printf("Expresion: ");
      scanf("%d %*c %d %c %d %*c %d", &r1->p, &r1->q, &op, &r2->p, &r2->q);
      if(r1->q == 0 || r2->q == 0)
         printf("\tEl denominador no puede ser cero!\n");
  }while(r1->q == 0 || r2->q == 0);
  
  return op;
}
