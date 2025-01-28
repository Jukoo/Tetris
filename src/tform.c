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
			{0, 1, 2, 0},
			{0, 1, 1, 0},
			{0, 0, 0, 0}
		},

		{
			{0, 0, 0, 0},
			{0, 1, 2, 0},
			{0, 1, 1, 0},
			{0, 0, 0, 0}
		},

		{
			{0, 0, 0, 0},
			{0, 1, 2, 0},
			{0, 1, 1, 0},
			{0, 0, 0, 0}
		}
	},

	{ /*|*/
		{
			{0, 0, 0, 0},
			{1, 1, 2, 1},
			{0, 0, 0, 0},
			{0, 0, 0, 0}
		},

		{
			{0, 0, 1, 0},
			{0, 0, 2, 0},
			{0, 0, 1, 0},
			{0, 0, 1, 0}
		},

		{
			{0, 0, 0, 0},
			{1, 1, 2, 1},
			{0, 0, 0, 0},
			{0, 0, 0, 0}
		},

		{
			{0, 0, 1, 0},
			{0, 0, 2, 0},
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
			{0, 0, 2, 1},
			{0, 1, 1, 0},
			{0, 0, 0, 0}
		},

		{
			{0, 0, 1, 0},
			{0, 0, 2, 1},
			{0, 0, 0, 1},
			{0, 0, 0, 0}
		},

		{
			{0, 0, 0, 0},
			{0, 0, 2, 1},
			{0, 1, 1, 0},
			{0, 0, 0, 0}
		},

		{
			{0, 0, 1, 0},
			{0, 0, 2, 1},
			{0, 0, 0, 1},
			{0, 0, 0, 0}
		}
	},

	{ /* --
		 /
         --  */
		{
			{0, 0, 0, 0},
			{0, 1, 2, 0},
			{0, 0, 1, 1},
			{0, 0, 0, 0}
		},

		{
			{0, 0, 0, 1},
			{0, 0, 2, 1},
			{0, 0, 1, 0},
			{0, 0, 0, 0}
		},

		{
			{0, 0, 0, 0},
			{0, 1, 2, 0},
			{0, 0, 1, 1},
			{0, 0, 0, 0}
		},

		{
			{0, 0, 0, 1},
			{0, 0, 2, 1},
			{0, 0, 1, 0},
			{0, 0, 0, 0}
		}
	},

	{ /*|_ */
		{
			{0, 0, 0, 0},
			{0, 1, 2, 1},
			{0, 1, 0, 0},
			{0, 0, 0, 0}
		},

		{
			{0, 0, 1, 0},
			{0, 0, 2, 0},
			{0, 0, 1, 1},
			{0, 0, 0, 0}
		},

		{
			{0, 0, 0, 1},
			{0, 1, 2, 1},
			{0, 0, 0, 0},
			{0, 0, 0, 0}
		},

		{
			{0, 1, 1, 0},
			{0, 0, 2, 0},
			{0, 0, 1, 0},
			{0, 0, 0, 0}
		}
	},

	{ /* --
         _| */
		{
			{0, 0, 0, 0},
			{0, 1, 2, 1},
			{0, 0, 0, 1},
			{0, 0, 0, 0}
		},

		{
			{0, 0, 1, 1},
			{0, 0, 2, 0},
			{0, 0, 1, 0},
			{0, 0, 0, 0}
		},

		{
			{0, 1, 0, 0},
			{0, 1, 2, 1},
			{0, 0, 0, 0},
			{0, 0, 0, 0}
		},

		{
			{0, 0, 1, 0},
			{0, 0, 2, 0},
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
			{0, 0, 2, 1},
			{0, 0, 1, 0},
			{0, 0, 0, 0}
		},

		{
			{0, 0, 1, 0},
			{0, 1, 2, 1},
			{0, 0, 0, 0},
			{0, 0, 0, 0}
		},

		{
			{0, 0, 1, 0},
			{0, 1, 2, 0},
			{0, 0, 1, 0},
			{0, 0, 0, 0}
		}
	}
};


struct tformctl * ttris_form_generator(struct tformctl * restrict figure) 
{ 
  figure->_form_type = give_ttris_form; 
  figure->_orientation= give_ttris_orientation; 
  figure->_figure=0 ;  //!row  
  figure->_shape=4;   //!cols  

  return figure ; 
}


void ttris_draw_form(struct  tformctl * restrict tform, struct area_location_xy *  location_playground,
    int should_apply_color)
{

    if(0  == should_apply_color ) 
    {
      //!TODO  : add flag for form shadow 
      tcmdexec(_reset) ; 
    }else 
      tcmdexec_p(_bcolor, COLOR_WHITE+tform->_form_type);  

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
                (location_playground->_colx +1) +           /* start at col 1 */ 
                (tform->_shape <<1) +                      /* doubling the shape size  */
                 (cols << 1)                                /* and column to corolate the shape fit */
                 ,tform->_figure+1+rows) ;                  //! rows or lines 

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

