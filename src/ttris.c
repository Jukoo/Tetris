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
    //!NOTE : Enable WASD  direction control  keyboard by default ; 
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


static int  ttris_get_next_form(struct  tformctl  * restrict form , int id ) 
{
  return ttris_form_generator(form, id) ;
}

static void ttris_update_visualizer_area(int ids_mask , struct  preview_area *  visualizer_area) 
{ 
   
  struct  tformctl next_forms[2] = {
    {._form_type = (ids_mask>>SSIZE), ._shape=SSIZE },
    {._form_type = (ids_mask & 0x0f), ._shape=SSIZE }
  } ;  

  
  struct playground_area   prev_zone= { visualizer_area->_colx ,visualizer_area->_rowy ,PREV_AREA} ; 

  ttris_draw_form((next_forms)   , &prev_zone ,0,1);
  ttris_draw_form((next_forms+1) , &prev_zone ,1,1); 
  
  tcmdexec(_reset) ; 
}

static struct playground_area *  ttris_init_playground_at(int  ttris_coordx , int ttris_coordy )
{
  struct playground_area  *pgnd_zone =  ( struct playground_area* ) malloc(sizeof(*pgnd_zone)) ; 

  if (!pgnd_zone) 
    return (struct playground_area*) 0 ; 
  
  pgnd_zone->_colx = ttris_coordx ; 
  pgnd_zone->_rowy = ttris_coordy ; 
  pgnd_zone->_tag_marker = PGND_AREA ; 
  draw_area_zone_based(pgnd_zone , TEREA_HEIGHT , TEREA_WIDTH) ; 

#if LINK_PREV_AREA_2_PGND_AREA 
  
  pgnd_zone->_preview_area._rowy = pgnd_zone->_rowy + 5 ;//  Related  to top position   
  pgnd_zone->_preview_area._colx = pgnd_zone->_colx + ((TEREA_WIDTH<<1))+GAP_BETWEEN_PGND_AREA_n_PREV_AREA; 
  pgnd_zone->_preview_area._tag_marker = PREV_AREA ; 

  struct playground_area prev_clone = { pgnd_zone->_preview_area._colx , pgnd_zone->_preview_area._rowy}  ; 
  draw_area_zone_based(&prev_clone , 5,5) ; 

#endif 

  return pgnd_zone; 
}

int ttris(int ttris_coordx , int ttris_coordy ,  int dds ) 
{
  if(clscr()) 
    warnx("Fail to clear the screen at first stage") ; 
 
  ttris_init_virtual_area_surface(); 
  ttris_touch_ctrl() ; 
  
  struct playground_area  *playground_zone = ttris_init_playground_at(ttris_coordx , ttris_coordy);
  if (!playground_zone) 
  {
     warnx("Cannot draw in area zone that overflow the current term") ; 
     return ~0 ; 
  }
 
  struct tformctl  ttris_form_shadow; 
  
  struct tformctl_queue *ttris_shapes  = ttris_form_init() ; 
  if (!ttris_shapes) 
    return   ~0 ;  
 
  struct tformctl  *ttris_form =  &ttris_shapes->_current_form ; 

  int  ids = ttris_shapes->_formctl_idsmask  ; 
  ttris_update_visualizer_area(ids , &playground_zone->_preview_area) ; 

  int  reach_bottom =0 ; 
  while(1) 
  {
     /*Generate new form when it reach the bottom */ 
     if(reach_bottom & RBTM)  
     {
       ttris_record_form(ttris_form  ,  playground_zone) ; 
       ids = ttris_form_get_next(ttris_shapes) ;   
       ttris_update_visualizer_area(ids, &playground_zone->_preview_area) ; 
       reach_bottom&=~RBTM; 
     }
    

     ttris_draw_form(ttris_form,playground_zone,1,0) ; 
     ttris_form_shadow = *ttris_form; 
     //!listen to keyboard direction control event  
     int input_evt   =  ttris_listen_touch_ctrl(ttris_form,  dds); 

     //!no key pressed 
     if ( (POLL_EVT_TIMEOUT & 0x0f) == input_evt)  
     {
       //!draw  shadow in current position of the form 
       //!and move down forward  and draw the form  with color  
       ttris_draw_form(ttris_form,playground_zone,0,0) ;
       ttris_form->_figure++; 
        //!TODO : Should be removed or  Found new ways  to ignore the other  key control  
       // if(BAD_KEY  !=  (POLL_EVT_TIMEOUT >>4)  & 0xf)
       reach_bottom = ttris_figure_is_in_area(ttris_form) ;
       //!TODO: FIXME  : Object Collision Detection 
       if (reach_bottom  == RCLS)   
       {
         ttris_dectect_collision_between_object(playground_zone ,ttris_form ,  &ttris_form_shadow); 
         reach_bottom<<=4; 
         reach_bottom|=RBTM ;
       }
     
       ttris_draw_form(ttris_form,playground_zone,1,0) ;  
       continue;  
     } 

     if (POOL_EVT_JIT == input_evt)  
     {
       reach_bottom = ttris_figure_is_in_area(ttris_form) ;
       if (reach_bottom  == RCLS)   
       {
         ttris_dectect_collision_between_object( playground_zone, ttris_form ,  &ttris_form_shadow); 

         reach_bottom<<=4; 
         reach_bottom|=RBTM ;  
         continue ; 
       }

       ttris_draw_form(ttris_form,playground_zone,0,0)  ; 

       if ( (ttris_form->_shape != ttris_form_shadow._shape) || 
            (ttris_form->_figure!= ttris_form_shadow._figure)||
            (ttris_form->_orientation!= ttris_form_shadow._orientation)) 
     
       {

         ttris_draw_form(&ttris_form_shadow,playground_zone,0,0) ; 
         ttris_form_shadow =  *ttris_form; 
         ttris_draw_form(ttris_form,playground_zone,1,0) ; 
     
       }
      
     }  

  } 
  return 0 ; 
}


//!TODO : add new parameter   for drop down speed  
//->  int  ddown_mode_t(enum  { SLOW , NORMAL, FAST }) 
static int ttris_listen_touch_ctrl(struct  tformctl * restrict figure  , int dds )
{

  struct pollfd pfd = { 
     .fd =STDIN_FILENO,  
     .events=POLLIN, 
     .revents=0
  };  
  struct timeval  timeout  = { 
    .tv_sec  = 0 , 
    .tv_usec =  (dds ==0)  ? DROP_DOWN_SPEED_OBJECT  : dds  
  };
  
  /*NOTE: Only listening on function failure ; No verification on timeout event */
  int status = poll(&pfd , 1,(int)timeout.tv_usec); 
  switch(status)
  {
    case ~0 : return ~0; 
    case POLL_EVT_TIMEOUT:return POLL_EVT_TIMEOUT ;  
  }

  unsigned char bf_key[0xff]={0} ; 
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



static void ttris_record_form(struct  tformctl  *restrict figure , struct playground_area *restrict pgnd_zone) 
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
      ttris_check_rows_line_completed(pgnd_zone) ; 
  }

}

static void ttris_check_rows_line_completed(struct playground_area * restrict  pgnd_zone) 
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
       ttris_move_all_downward(line , pgnd_zone) ; 
       continue ; 
       
     }
     rows_completed=1 ; 
  }
   
}

//!static void ttris_move_block_above_to_downward
static void ttris_move_all_downward(int  line ,  struct playground_area *restrict pgnd_zone) 
{
    
   int completed_at = line+(~0) ; 

   struct line_above { 
    int *row_above ;   
    int cell_value ; 
   } line_above ;  

   while(0 < completed_at) 
   {
      int * completed_line = *(area_surface+completed_at) ;  
      line_above.row_above= (completed_line--) ; 
      line_above.cell_value =~0 ; 
      int  cols=~0 ; 
      while (TEREA_WIDTH >++cols) 
      { 
        line_above.cell_value = *(line_above.row_above+cols);  

        if( 0  ==  completed_line ) 
           line_above.cell_value =  *(completed_line+cols) = ~0 ; 


        if (~0 != line_above.cell_value)  
        {
           tcmdexec_p(_bcolor,  COLOR_WHITE+line_above.cell_value); 
        }else {
          tcmdexec(_reset);
        }

        tcmdexec_g(_cursors[cr_address], pgnd_zone->_colx +(cols<<1)+1, pgnd_zone->_rowy+completed_at+2) ; 
        ascii_prt(0x20),ascii_prt(0x20); 
      }

      completed_at+=~(completed_at^completed_at) ;  
   }
  
   //tcmdexec(_reset) ; 
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
         {
           ttris_dbg_prt(100 , 4 , "Ojbect collision detected");  
           return RCLS ; 
             
         }
       }
     } 
  }
  
  return 0 ; 
}

static void ttris_dectect_collision_between_object( struct  playground_area  * restrict  pgnd ,  
                                                    struct  tformctl * restrict current_form,  
                                                    struct  tformctl * restrict  prevs_form) 
{

   current_form->_figure-=1; 

   ttris_draw_form(current_form ,pgnd,0,0)  ;
   ttris_draw_form(prevs_form, pgnd,1,0)  ; 

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
