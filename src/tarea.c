/*
 * 
 */
#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h> 
#include "termconfigure.h"
#include "tarea.h"
 

int  area_surface[TEREA_HEIGHT][TEREA_WIDTH]={0} ; 

struct area_location_xy * draw_area_zone(int start_colx , int start_rowy) 
{ 
  /*! make sure if  the given cols and rows fit  well*/ 
  int dim =  term_xymax() ; 
  /* TODO : Enable verification on release build 
  if((dim >>8) < start_colx || (dim  & 0xff) < start_rowy)   
    return nptr;    
  */ 
 
 
  area_location_xy * ttris_playground_zone = (area_location_xy *) malloc(sizeof(*ttris_playground_zone)) ; 
  if (!ttris_playground_zone)  
  {
    return  nptr ;  
  }
 
  ttris_playground_zone->_colx = start_colx ; 
  ttris_playground_zone->_rowy = start_rowy ;  

  tcmdexec_g(_cursors[cr_address] ,start_colx,start_rowy) ; 
  
  int y=~0;
  int upline =1 ; 
  while(++y  <TEREA_HEIGHT)
  {
    if (0 == y ||  y == (TEREA_HEIGHT -1))  
    {  
      if(!upline) 
        tcmdexec_g(_cursors[cr_address],start_colx,y) ;  

      int x=~0; 
      ascii_prt(0x2b); 
      while ( ++x   < (TEREA_WIDTH<<1))  
        ascii_prt(0x2d);  
      
      ascii_prt(0x2b); 
      upline^=upline; 
    }else 
    { 
      tcmdexec_g(_cursors[cr_address] ,start_colx,y); 
      ascii_prt(0x7c);  
      tcmdexec_g(_cursors[cr_address] ,((TEREA_WIDTH<<1)+1)+start_colx,y); 
      ascii_prt(0x7c);  
    }

  }

  return ttris_playground_zone ;  
} 

static int term_xymax(void)
{  
  int term_dimension =0 ; 
  term_dimension|= columns ; 
  term_dimension<<=8; 
  term_dimension|= lines ; 
  return term_dimension ; 

}
