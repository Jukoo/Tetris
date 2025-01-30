/*
 */

#if !defined(tform_h) 
#define tform_h 

#define  NFORM  0x7
#define  NROTS  0x4 
#define  FSIZE  0x4 
#define  SSIZE  0x4  

#include <sys/cdefs.h> 
#include <time.h>
#include <stdlib.h> 
#include <stdint.h>  

#include "tarea.h"

typedef struct  tformctl tformctl; 
struct tformctl
{
  int _form_type;  
  int _orientation; 
  int _figure; 
  int _shape;   
}; 

/*  
typedef struct  tformctl_queue tformctl_queue ; 
struct tformctl_queue {
   struct  tformctl* _current_form ; 
   struct  tformctl* _next_form
}; 
*/
#define  CURRENT 0 
#define  NEXT    1 
extern struct tformctl forms[2] ; 
 

extern int ttris_forms[NFORM][NROTS][FSIZE][SSIZE]; 

/* @fn ttris_form_generator(struct  tformctl *) 
 * @brief form  generator 
 * @param struct  tformctl 
 * @return tformctl* - hold  the information about the form  
 */
int ttris_form_generator(struct  tformctl * __restrict__ tform , int form_id);  

__extern_always_inline  
int  __give_ttris_form_id(int fid)  
{
   srand(time(0)) ; 
   return  rand() %  fid ; 
}

#define  give_ttris_form\
  __give_ttris_form_id(NFORM) 

#define  give_ttris_orientation\
  __give_ttris_form_id(NROTS) 

/* @fn ttris_draw_form()
 */ 
void  ttris_draw_form(struct  tformctl * __restrict__ tform ,  
                      struct  area_location_xy * __restrict__  plgnd_zone ,  int color ,  int  gap_consideration); 



#endif //!tform_h   
