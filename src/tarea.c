/*
 * 
 */
#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h> 
#include "termconfigure.h"
#include "tarea.h"
#include "ttris.h" 

int  area_surface[TEREA_HEIGHT][TEREA_WIDTH]={0} ; 

void  draw_area_zone(int start_colx , int start_rowy , int height , int width) 
{ 
  /*! make sure if  the given cols and rows fit  well*/ 
  int dim =  term_xymax() ; 
  /* TODO : Enable verification on release build 
  if((dim >>8) < start_colx || (dim  & 0xff) < start_rowy)   
    return nptr;    
  */ 
  
  tcmdexec_g(_cursors[cr_address] ,start_colx,start_rowy) ;
  int y=~0;
  int upline =1 ; 
  while(++y  < height)
  {
    if (0 == y ||  y == (height -1))  
    {  
      if(!upline) 
        tcmdexec_g(_cursors[cr_address],start_colx,y+start_rowy) ;  

      int x=~0; 
      ascii_prt(0x2b); 
      while ( ++x   < (width<<1))  
        ascii_prt(0x2d);  
      
      ascii_prt(0x2b); 
      upline^=upline; 
    }else 
    { 
      tcmdexec_g(_cursors[cr_address] ,start_colx,y+start_rowy); 
      ascii_prt(0x7c);  
      tcmdexec_g(_cursors[cr_address] ,((width<<1)+1)+start_colx,y+start_rowy); 
      ascii_prt(0x7c);  
    }

  }
  tcmdexec(_reset) ; 
} 

void  draw_area_zone_based(struct playground_area * restrict  plgrd , int height , int width)  
{
  draw_area_zone(plgrd->_colx , plgrd->_rowy,  height , width) ; 
}


static int term_xymax(void)
{  
  int term_dimension =0 ; 
  term_dimension|= columns ; 
  term_dimension<<=8; 
  term_dimension|= lines ; 
  return term_dimension ; 

}
