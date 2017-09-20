/**
 * QuickSort que ordena DESCENDENTE un arreglo de cualquier tipo de datos de forma.
 *
 * Tomado de https://mathbits.com/MathBits/CompSci/Arrays/Quick.htm y modificado
 * cambiado a templates y cambiado el nombre de params
 *
 * Pendiente: 
 * - Permitir mandar la condicion para ordenar. 
 * - Implementar que inicie con el contructor default el swap para usar objetos.
*/

#ifndef QUICKSORT_H
    #define QUICKSORT_H


//Function to determine the partitions
// partitions the array and returns the middle subscript
template <typename T>
static int partition(T array[], int bottom, int top)
{
     T x = array[bottom];
     int i = bottom - 1;
     int j = top + 1;
     T temp;
     do
     {
           do      
           {
                  j--;
           }while (x >array[j]);

          do  
         {
                 i++;
          } while (x <array[i]);

          if (i < j)
         { 
                 temp = array[i];    
                 array[i] = array[j];
                 array[j] = temp;
         }
     }while (i < j);     
     return j;           // returns middle subscript  
}

/**
 * @param bottom First value: 0
 * @param top Index del ultimo valor inclusivo: length-1
 *
 */
template <typename T>
void quickSort(T num[], int bottom, int top)
{      
     if (bottom < top)
    {
          const int middle = partition(num, bottom, top);
          quickSort(num, bottom, middle);   // sort first section
          quickSort(num, middle+1, top);    // sort second section
    }
}


#endif