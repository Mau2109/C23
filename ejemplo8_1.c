/* Abstraccion de numeros racionales (primera version).
   @autor: Ricardo Ruiz Rodriguez
*/
#include <stdio.h>

struct r{
   int p;
   int q;
};

typedef struct r RACIONAL;

int main(){
   struct r r1;
   RACIONAL r2;
   RACIONAL res;
   char op;
   
   do{
      printf("Expresion: ");
      scanf("%d %*c %d %c %d %*c %d", &r1.p, &r1.q, &op, &r2.p, &r2.q);
      if(r1.q == 0 || r2.q == 0)
         printf("\tEl denominador no puede ser cero!\n");
  }while(r1.q == 0 || r2.q == 0);
  
   switch(op){
      case '+':
         break;
      case '-':
         break;
      case '*':
         res.p = r1.p * r2.p;
         res.q = r1.q * r2.q;
         break;
      case '/':
         res.p = r1.p * r2.q;
         res.q = r1.q * r2.p;
         break;
      default:
         printf("\aOperador NO valido:\"%c\"\n", op);
         return 0;
   }
   
   printf("%d/%d %c %d/%d = %d/%d\n",
          r1.p, r1.q, op, r2.p, r2.q, res.p, res.q);
   
   return 0;
}
