#include <omp.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h> 
#include <math.h>
#include <time.h>

#define MAXPOINTS 1000
#define MAXSTEPS 1000
#define MINPOINTS 20
#define PI 3.14159265

void init_param(void);
void init_line(void);
void update (void);
void printfinal (void);

int nsteps,                 	/* numero de intervalos de tiempo */
    tpoints; 	     		/* cantidad de puntos en la onda */
double values[MAXPOINTS+2], 	/* valores del campo en el tiempo t */
       oldval[MAXPOINTS+2], 	/* valores en el tiempo (t-dt) */
       newval[MAXPOINTS+2]; 	/* valores en el tiempo (t+dt) */


/***************************************************************************
 *	Input 
 ***************************************************************************/
void init_param(void)
   {
   char tchar[8];

   /* cantidad de puntos e iteraciones */
   tpoints = 0;
   nsteps = 0;
	tpoints=800;
	nsteps=800;

   printf("Usando %d puntos, y  %d pasos\n", tpoints, nsteps);

   }

/***************************************************************************
 *     Inicializacion de puntos en la onda
 **************************************************************************/
void init_line(void)
   {
   int i, j;
   double x, fac, k, tmp;

   /* Valores iniciales con la funcion seno */
   fac = 2.0 * PI;
   k = 0.0; 
   tmp = tpoints - 1;
   for (j = 1; j <= tpoints; j++) {
      x = k/tmp;
      values[j] = sin (fac * x);
      k = k + 1.0;
      } 

   /* Valores iniciales guardados en oldval*/
   for (i = 1; i <= tpoints; i++) 
      oldval[i] = values[i];
   }

/***************************************************************************
 *      Valores nuevos con la ecuacion hiperbolica de onda
 **************************************************************************/
void do_math(int i)
   {
   double dtime, c, dx, tau, sqtau;

   dtime = 0.3;
   c = 1.0;
   dx = 1.0;
   tau = (c * dtime / dx);
   sqtau = tau * tau;
   newval[i] = (2.0 * values[i]) - oldval[i] 
               + (sqtau * (values[i-1] - (2.0 * values[i]) + values[i+1]));
   }

/***************************************************************************
 *     iteracion en una cantidad determinada de pasos
 **************************************************************************/
void update()
   {
   int i, j;

   /* Actualizacion de valores en cada paso */
   for (i = 1; i<= nsteps; i++) {
      /* Actualizacion de valores para cada punto de la onda*/
      for (j = 1; j <= tpoints; j++) {
         /* puntos internos y de frontera */
         if ((j == 1) || (j  == tpoints))
            newval[j] = 0.0;
         else
            do_math(j);
         }

      /* Actualizacion de valores antiguos y nuevos */
      for (j = 1; j <= tpoints; j++) {
         oldval[j] = values[j];
         values[j] = newval[j];
         }
      }
   }

/***************************************************************************
 *     Output
 **************************************************************************/
void printfinal()
   {
   int i;
   for (i = 1; i <= tpoints; i++) {
      printf("%6.4f \n", values[i]);
    //  if (i%10 == 0)
      //   printf("\n");
      }
   }
/***************************************************************************
 *	Programa principal
 **************************************************************************/
int main(int argc, char *argv[])
{
/*
int left, right;
*/

printf("Onda secuencial ...\n");
init_param();
printf("Inicializando puntos de la onda...\n");
init_line();
printf("Actualizando puntos para la cantidad total de pasos...\n");
update();
printf("Imprimiendo resultados...\n");
printfinal();
printf("\nFin.\n\n");
}

