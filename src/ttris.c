/* @file ttris.c 
 * @brief  tetris game logic function 
 * @author Umar Ba <jUmarB@protonmail.com>         
 * @song :  Downtown Binary  (Electronic Gems)
 */

#include <stdlib.h> 
#include <err.h>
#include <unistd.h>
#include <poll.h> 
#include <assert.h>
#include <string.h>  
#include <stdarg.h>
#include "termconfigure.h" 
#include "tform.h" 
#include "ttris.h"
#include "tarea.h" 

int dirctl_cmd[2][4]={0} ; 

int kbrctl_mode=FLEXCTRL ; 

static int clscr(void) 
{
   return tcmdexec(_cls); 
}


static void ttris_touch_ctrl(void) 
{ 
  if (0 == kbrctl_mode)
  {
    warnx("ctrl mode can't be empty") ; 
    //!NOTE : Enable WASDCTRL  by default ; 
    kbrctl_mode|= WASDCTRL ;   
  }
  if (kbrctl_mode& WASDCTRL)   
  {
     
     int * wasd =  *(dirctl_cmd+0) ; 
     int  cmd[] = WASDCTRL_CMD ;
     int i  = ~0; 
     while (++i  < 4) 
     {
       *(wasd+i) = *(cmd+i) ; 
     } 
  }

}
static  void ttris_init_virtual_area_surface(void)
{
   int line=~0; 
   while(++line < TEREA_HEIGHT) 
   {
     int col=~0; 
     while(++col<TEREA_WIDTH)  
       *(*(area_surface+line)+col)  = TEREA_SURFACE_DEFVAL ; 

   }
}


int ttris(void) 
{
  if(clscr()) 
    warnx("Fail to clear the screen at first stage") ; 

  ttris_dbg_prt(100,0 , "current process : %i\n",  (int)getpid()) ; 
 
  ttris_init_virtual_area_surface(); 
  ttris_touch_ctrl() ; 
  

  struct area_location_xy * playground_zone = nptr; 
  //!TODO :  move area coordonate across files 
  playground_zone =  draw_area_zone(10,0) ; 
  
  if (!playground_zone) 
  {
     warnx("Cannot draw in area zone that overflow the current term") ; 
     return ~0 ; 
  }
   
  struct tformctl  ttris_form ; 
  struct tformctl  ttris_form_shadow;  
  ttris_form_generator(&ttris_form) ;  

  int  reach_bottom =0 ; 
  while(1) 
  {
     /*Generate new form when it reach the bottom */ 
     if(reach_bottom & RBTM)  
     {
       ttris_record_form(&ttris_form) ; 
       ttris_form_generator(&ttris_form); 
       reach_bottom&=~RBTM; 
     }
    

     ttris_draw_form(&ttris_form,playground_zone,1) ; 
     ttris_form_shadow = ttris_form; 
     //!listen to keyboard direction control event  
     int input_evt   =  ttris_listen_touch_ctrl(&ttris_form); 

     //!no key pressed 
     if ( (POLL_EVT_TIMEOUT & 0x0f) == input_evt)  
     {
       //!draw  shadow in current position of the form 
       //!and move down forward  and draw the form  with color  
       ttris_draw_form(&ttris_form,playground_zone,0) ;
       ttris_form._figure++; 
        //!TODO : Should be removed or  Found new ways  to ignore the other  key control  
       // if(BAD_KEY  !=  (POLL_EVT_TIMEOUT >>4)  & 0xf)
       reach_bottom = ttris_figure_is_in_area(&ttris_form) ;
       //!TODO: FIXME  : Object Collision Detection 
       if (reach_bottom  == RCLS)   
       {
         ttris_dectect_collision_between_object(&ttris_form ,  &ttris_form_shadow); 
         reach_bottom<<=4; 
         reach_bottom|=RBTM ;
       }
     

       ttris_draw_form(&ttris_form,playground_zone,1) ;  
       continue;  
     } 

     if (POOL_EVT_JIT == input_evt)  
     {
       reach_bottom = ttris_figure_is_in_area(&ttris_form) ;
       if (reach_bottom  == RCLS)   
       {
         ttris_dectect_collision_between_object(&ttris_form ,  &ttris_form_shadow); 

         reach_bottom<<=4; 
         reach_bottom|=RBTM ;  
         continue ; 
       }

       ttris_draw_form(&ttris_form,playground_zone,0)  ; 

       if ( (ttris_form._shape != ttris_form_shadow._shape) || 
            (ttris_form._figure!= ttris_form_shadow._figure)||
            (ttris_form._orientation!= ttris_form_shadow._orientation)) 
     {
       ttris_draw_form(&ttris_form_shadow,playground_zone,0) ; 
       ttris_form_shadow =  ttris_form; 
       ttris_draw_form(&ttris_form,playground_zone,1) ; 
     }

       nanosleep( &(struct  timespec){0,1000},  nptr) ; 
      
     }  

  } 
  return 0 ; 
}


static int ttris_listen_touch_ctrl(struct  tformctl * restrict figure)
{

  struct pollfd pfd = { 
     .fd =STDIN_FILENO,  
     .events=POLLIN, 
     .revents=0
  };  
  struct timeval  timeout  = { 
    .tv_sec  = 0 , 
    .tv_usec = 200
  };
  
  /*NOTE: Only listening on function failure ; No verification on timeout event */
  int status = poll(&pfd , 1,(int)timeout.tv_usec); 
  switch(status)
  {
    case ~0 : return ~0; 
    case POLL_EVT_TIMEOUT:return POLL_EVT_TIMEOUT ;  
  }

  char bf_key[0xff]={0} ; 
  if( pfd.revents & POLLIN ) 
  {
     ssize_t rb =  read(pfd.fd,  bf_key ,  0xff); 
     assert(!rb^strlen(bf_key)); 
  }  
   
  /*      w  @rotation 
   *      ^ 
   *   d < > d
   *      v 
   *      s  @move down forward  
   */
  if(kbrctl_mode & WASDCTRL)  
  {
    switch( ((*bf_key)& 0xff))
    {
      case LFT: --figure->_shape; break;  
      case RGT: ++figure->_shape; break; 
      case DWN: ++figure->_figure;break; 
      case ROT: 
                figure->_orientation=(++figure->_orientation % NROTS);  break;
      default: 
                return  BAD_KEY ; 
    }
   
  }
  
  switch(ttris_figure_is_in_area(figure)) 
  {
    case   RLFT :
      ++figure->_shape; 
      break ; 
    case   RRGT : 
      --figure->_shape; 
      break;
  }
  
  return  POOL_EVT_JIT ; 
}



static void ttris_record_form(struct  tformctl  *restrict figure) 
{

  int  form_item  = ~0 ; 
  int *ttris_scan_line  ; 
 
  while (  FSIZE > ++form_item) 
  {
     int shape=~0 ; 
     ttris_scan_line =  *(area_surface+ (figure->_figure +form_item)) ; 

     while(SSIZE > ++shape) 
     {
       if (ttris_forms[figure->_form_type][figure->_orientation][form_item][shape] != 0) 
       {
         if(figure->_shape+shape >= TEREA_WIDTH  || 
            figure->_shape+shape <0 ) continue ; 

         if(figure->_figure + form_item >= TEREA_HEIGHT) continue ; 
         
         *(ttris_scan_line+(figure->_shape+shape)) = figure->_form_type ;  
        
        //printf("[%i]" , *(ttris_scan_line+(figure->_shape+shape)))  ; 
       }//else 
         //printf("[x]"); 
      
     }
      ttris_check_rows_line_completed() ; 
  }

}

static void ttris_check_rows_line_completed(void) 
{ 
  
  int line=~0; 
  int rows_completed = 1 ; 

  while(TEREA_HEIGHT > ++line)
  {
     int col=~0; 
     while(TEREA_WIDTH > ++col) 
     {
       int *mtrx_val= (*(area_surface+line)+col) ; 
       if (~0 == *mtrx_val) 
       {
         rows_completed^=1 ; 
         break;  
       }
     } 
     
     if(1 == rows_completed)
     { 
       ttris_dbg_prt(100,1, "completed! at row  %i\n" , line+3);
       ttris_move_all_downward(line) ; 
       continue ; 
       
     }
     rows_completed=1 ; 
  }
   
}

static void ttris_move_all_downward(int  rowy) 
{
 
   while(0 < rowy--) 
   {
      int * current_matched_line = *(area_surface+rowy) ;  
      int * line_above = (current_matched_line--) ; 
      int  cols=~0 ; 
      int line_above_value =~0 ; 
      while (TEREA_WIDTH >++cols) 
      { 
        
        line_above_value= *(line_above+cols) ;
      
        //if (rowy==0) 
          //line_above_value= *(current_matched_line+cols) = ~0; 
        
        if (~0 !=  line_above_value) 
        {
           tcmdexec_p(_bcolor,  COLOR_WHITE+line_above_value);  
        }else  
          tcmdexec(_reset);

        tcmdexec_g(_cursors[cr_address], 10+(cols<<1), rowy+2) ; 
        ascii_prt(0x20);
        ascii_prt(0x20); 
      }
   }
  
   tcmdexec(_reset) ; 
}

static int  ttris_figure_is_in_area(struct tformctl * restrict figure)   
{ 
  int y=~0,x=~0 ; 

  while(++y  < FSIZE) 
  {
     x=~0;  
     int * line  = *(area_surface+(figure->_figure+y)) ; 
     while (++x <  SSIZE) 
     {
       if(ttris_forms[figure->_form_type][figure->_orientation][y][x] !=0) 
       {
          //!NOTE  BOTTOM  collision reach 
         if(figure->_figure+y+3>=  TEREA_HEIGHT ) 
         {
           return  RBTM  ;  
         }

         //!NOTE RIGTH collision reach 
         if(figure->_shape+x >=  TEREA_WIDTH) 
         {
           return RRGT ;  
         }

         //!NOTE : LEFT  collision reach 
         if(figure->_shape+x < 0 )  
         {
           return RLFT;   
         }

         //!NOTE: OBJECT collision 
         if(*(line+figure->_shape+x) != ~0 ) 
           return RCLS ; 
         
       }
     } 
     
  }
  
  return 0 ; 
}

static void ttris_dectect_collision_between_object(struct  tformctl * restrict current_form,  
                                                   struct  tformctl * restrict  prevs_form) 
{
   

   ttris_dbg_prt(100 , 4 , "Ojbect collision detected");  
   current_form->_figure-=1; 

   ttris_draw_form(current_form ,&(struct area_location_xy){10, 0} ,0)  ;
   ttris_draw_form(prevs_form, &(struct area_location_xy){10, 0} ,1)  ; 

   prevs_form  = current_form ; 
    
}

void ttris_dbg_prt(int coordx , int coordy ,  const char * fmt , ...) 
{
  va_list ap ; 
  va_start(ap , fmt) ;
  tcmdexec_g(_cursors[cr_address] , coordx , coordy) ; 
  vprintf(fmt, ap) ;
  
  va_end(ap); 
}
