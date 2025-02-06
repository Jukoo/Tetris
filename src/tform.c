/*
 *
 */
#include <stdlib.h>
#include <time.h> 
#include <unistd.h> 

#include "tform.h" 
#include "termconfigure.h" 
#include "tarea.h" 

#include "ttris.h"  //! just for debug print //!TODO to be removed after usage 

//!TODO: use hex values to represente form 
int ttris_forms[NFORM][NROTS][FSIZE][SSIZE]={
	{ /* []  */
		{
			{0, 0, 0, 0},
			{0, 1, 1, 0},
			{0, 1, 1, 0},
			{0, 0, 0, 0}
		},

		{
			{0, 0, 0, 0},
			{0, 1, 1, 0},
			{0, 1, 1, 0},
			{0, 0, 0, 0}
		},

		{
			{0, 0, 0, 0},
			{0, 1, 1, 0},
			{0, 1, 1, 0},
			{0, 0, 0, 0}
		},

		{
			{0, 0, 0, 0},
			{0, 1, 1, 0},
			{0, 1, 1, 0},
			{0, 0, 0, 0}
		}
	},

	{ /*|*/
		{
			{0, 0, 0, 0},
			{1, 1, 1, 1},
			{0, 0, 0, 0},
			{0, 0, 0, 0}
		},

		{
			{0, 0, 1, 0},
			{0, 0, 1, 0},
			{0, 0, 1, 0},
			{0, 0, 1, 0}
		},

		{
			{0, 0, 0, 0},
			{1, 1, 1, 1},
			{0, 0, 0, 0},
			{0, 0, 0, 0}
		},

		{
			{0, 0, 1, 0},
			{0, 0, 1, 0},
			{0, 0, 1, 0},
			{0, 0, 1, 0}
		}
	},

	{ /*   ---
          |__
             |
           ___ */
		{
			{0, 0, 0, 0},
			{0, 0, 1, 1},
			{0, 1, 1, 0},
			{0, 0, 0, 0}
		},

		{
			{0, 0, 1, 0},
			{0, 0, 1, 1},
			{0, 0, 0, 1},
			{0, 0, 0, 0}
		},

		{
			{0, 0, 0, 0},
			{0, 0, 1, 1},
			{0, 1, 1, 0},
			{0, 0, 0, 0}
		},

		{
			{0, 0, 1, 0},
			{0, 0, 1, 1},
			{0, 0, 0, 1},
			{0, 0, 0, 0}
		}
	},

	{ /* --
		 /
         --  */
		{
			{0, 0, 0, 0},
			{0, 1, 1, 0},
			{0, 0, 1, 1},
			{0, 0, 0, 0}
		},

		{
			{0, 0, 0, 1},
			{0, 0, 1, 1},
			{0, 0, 1, 0},
			{0, 0, 0, 0}
		},

		{
			{0, 0, 0, 0},
			{0, 1, 1, 0},
			{0, 0, 1, 1},
			{0, 0, 0, 0}
		},

		{
			{0, 0, 0, 1},
			{0, 0, 1, 1},
			{0, 0, 1, 0},
			{0, 0, 0, 0}
		}
	},

	{ /*|_ */
		{
			{0, 0, 0, 0},
			{0, 1, 1, 1},
			{0, 1, 0, 0},
			{0, 0, 0, 0}
		},

		{
			{0, 0, 1, 0},
			{0, 0, 1, 0},
			{0, 0, 1, 1},
			{0, 0, 0, 0}
		},

		{
			{0, 0, 0, 1},
			{0, 1, 1, 1},
			{0, 0, 0, 0},
			{0, 0, 0, 0}
		},

		{
			{0, 1, 1, 0},
			{0, 0, 1, 0},
			{0, 0, 1, 0},
			{0, 0, 0, 0}
		}
	},

	{ /* --
         _| */
		{
			{0, 0, 0, 0},
			{0, 1, 1, 1},
			{0, 0, 0, 1},
			{0, 0, 0, 0}
		},

		{
			{0, 0, 1, 1},
			{0, 0, 1, 0},
			{0, 0, 1, 0},
			{0, 0, 0, 0}
		},

		{
			{0, 1, 0, 0},
			{0, 1, 1, 1},
			{0, 0, 0, 0},
			{0, 0, 0, 0}
		},

		{
			{0, 0, 1, 0},
			{0, 0, 1, 0},
			{0, 1, 1, 0},
			{0, 0, 0, 0}
		}
	},

	{ /*___
         | */
		{
			{0, 0, 0, 0},
			{0, 1, 1, 1},
			{0, 0, 1, 0},
			{0, 0, 0, 0}
		},

		{
			{0, 0, 1, 0},
			{0, 0, 1, 1},
			{0, 0, 1, 0},
			{0, 0, 0, 0}
		},

		{
			{0, 0, 1, 0},
			{0, 1, 1, 1},
			{0, 0, 0, 0},
			{0, 0, 0, 0}
		},

		{
			{0, 0, 1, 0},
			{0, 1, 1, 0},
			{0, 0, 1, 0},
			{0, 0, 0, 0}
		}
	}
};


int   ttris_form_generator(struct tformctl * restrict figure , int id ) 
{ 
  figure->_form_type = id;    
  figure->_orientation= give_ttris_orientation; 
  figure->_figure=0 ;      
  figure->_shape=SSIZE;    
                      

  return   give_ttris_form  ;  
}


struct  tformctl_queue  * ttris_form_init(void)    
{
  struct tformctl_queue *  tfQ = (struct tformctl_queue*) malloc(sizeof(*tfQ)); 
  if(!tfQ)  
    return nptr; 
  
  int next_shape_id =  ttris_form_generator(&tfQ->_current_form , give_ttris_form); 
  tfQ->_formctl_idsmask = ((tfQ->_current_form._form_type << SSIZE) | (next_shape_id & 0xf)) ;  
  (void) ttris_form_generator(&tfQ->_next_form ,  next_shape_id) ; 

  return tfQ ; 
  
}

int ttris_form_get_next(struct  tformctl_queue * restrict tfQ)  
{
  /*! reset idmask form  to initial state */ 
  tfQ->_formctl_idsmask&=~tfQ->_formctl_idsmask ;  

  /*! get previous form type id */   
  int previous =  tfQ->_current_form._form_type;  
  /*! set the current form  to reach the next form   */
  tfQ->_current_form =  tfQ->_next_form; 
  
  /*!  Generate new shape/form  for  tfQ->next_form attribute */
  (void)ttris_form_generator(&tfQ->_next_form,  give_ttris_form);  

  /*! Get  next form type id  */ 
  int next  =  tfQ->_next_form._form_type;  
  
  /*! reconstitute the  idmask */  
  tfQ->_formctl_idsmask |=((previous << SSIZE) | next & 0xf ); 

  return tfQ->_formctl_idsmask ; 
}

void ttris_draw_form(struct  tformctl * tform, struct  playground_area *  location_playground,
    int should_apply_color, int gap_consideration)
{

    if(0  == should_apply_color ) 
    {
      //!TODO  : add flag for form shadow 
      tcmdexec(_reset) ; 
    }else 
      tcmdexec_p(_bcolor, COLOR_WHITE+ tform->_form_type);  

	int rows=~0; 
    int fill_the_gap =0 ;    
    
	while(++rows <  FSIZE)
	{
       int cols=~0; 
       while (++cols < SSIZE) 
       {  

           if(ttris_forms[tform->_form_type][tform->_orientation][rows][cols]!=0) 
           {
             tcmdexec_g(_cursors[cr_address],
                ((location_playground->_colx -(gap_consideration!=0 ?  7 : 0 ) + 1))   +           /* start at col 1 */ 
                (tform->_shape <<1) +                      /* doubling the shape size  */
                 (cols << 1)                                /* and column to corolate the shape fit */

                 ,tform->_figure+1+location_playground->_rowy+rows+(gap_consideration !=0 ? location_playground->_rowy:0)) ; //! rows or lines 

             ascii_prt(0x20) ; 
             
             /*! Due to an empty gap present in the form that cause wierd  effect ! 
              *  this portion of code bellow   fix it  well */ 
             fill_the_gap^=1 ; 
           }

           if(fill_the_gap)
           {
             ascii_prt(0x20) ; 
             fill_the_gap^=1 ; 
           }

       } 
	} 
     
    tcmdexec(_reset) ; 

}

