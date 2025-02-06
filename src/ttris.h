/* @file ttris.h
 * @brief tetris game  logic function proto 
 * @author Umar Ba <jUmarB@protonmail.com> 
 */

#if !defined(ttris_h) 
#define     ttris_h

#include "tform.h" 

enum { 
  MV_LFT , 
#define  __MV_LFT  0x61 
  MV_RGT ,  
#define  __MV_RGT  0x64
  MV_DWN , 
#define  __MV_DWN  0x73  
  MV_UP   
#define  __MV_UP   0x77  
}; 

#define  getmove(__move_action)  __#__move_action 

#define  LFT   __MV_LFT    
#define  RGT   __MV_RGT
#define  DWN   __MV_DWN   
#define  ROT   __MV_UP 
#define  ROTATE  ROT 

#define  WASDCTRL_CMD  { __MV_LFT ,  __MV_RGT , __MV_DWN ,  __MV_UP}
#define  WASDCTRL  (1<<0) 
#define  DIRSCTRL  (1<<2)  
#define  FLEXCTRL  WASDCTRL |  DIRSCTRL   //! use both 
extern int dirctl_cmd[2][4] ;  

extern int kbrctl_mode;  

#define  RLFT  (1<<1)
#define  RRGT  (1<<2)
#define  RBTM  (1<<3)
#define  RCLS  (1<<4)   //!collision with form 


#define POLL_EVT_TIMEOUT  0 
#define POOL_EVT_JIT      1   /*! Key pressed just in time */  
#define BAD_KEY  (1<<5)

#define PREVISUALIZER_AREA   10 

/* @fn clscr(void) 
 * @brief clear the screen first 
 * @return O:ok  otherwise error 
 */
static int clscr(void) ;  

/* @fn ttris_init_virtual_area_surface() 
 * @brief initialize area_surface with default value 
 * */

static void ttris_init_virtual_area_surface(void); 


/* @fn ttris_touch_ctlr(void) 
 * @brief  handle  keyboard control movement
 **/

static void ttris_touch_ctrl(void) ; 

/* @fn  ttris_next_shape_visualizer_zone(struct  area_location_xy  *) 
 * @brief  draw a small  area  to visualize  incomming form  
 * @parm  struct area_location_xy *  
 */
static struct area_location_xy * ttris_next_shape_visualizer_zone(struct area_location_xy * main_playground_zone,
                                                int height ,int width); 

/* @fn 
 *
 * */ 

static  int ttris_get_next_form(struct tformctl  *  __restrict__ figure , int id  ) ; 



static void  ttris_update_visualizer_area(int ids_mask , struct  area_location_xy *  visualizer_zone) ;  

/* @fn ttris(void)  *  entry of the game 
 * @brief handle  the game main function of the game 
 * @return int 0:ok otherwise error 
 */
int ttris(int ttris_coordx , int ttris_coordy) ;  


/* @fn ttris_listen_touch_ctrl(void) 
 * @brief  listen  for direction key touch 
           to controle the  shape form  left rigt down and rotation  
 */
static int ttris_listen_touch_ctrl(struct  tformctl * __restrict__ figure) ; 

/* @fn ttris_record_form(struct tformctl *) 
 * @brief record tetris  form  in  area matix 
 * @parm  struct tformctl * 
 * @return - 
 *  
 */
static void  ttris_record_form (struct tformctl * __restrict__ figure) ; 

/*
 *  @fn  ttris_is_rows_line_completed() 
 *  @brief check in the matrix aka area matrix 
 *         thats represent the area  of playground game 
 * */ 
static void ttris_check_rows_line_completed(void) ; 

/* @fn ttris_mova_all_downward(int line_or_row_location)  
 * @brief  after checking when the the rows is completed 
 *         move all form down 
 * @param  int line_or_row_location - where the complet line check happend 
 **/
static void ttris_move_all_downward(int  line_or_row_location ) ; 

/* @fn ttris_figure_is_in_area(struct  *tformctl) 
 * @brief check  the figure  is in the area  
 * @parm struct tformctl * 
 * @return  int 
 */

static int ttris_figure_is_in_area(struct  tformctl *__restrict__ figure) ; 

/* @fn  ttris_dectect_collision_between_object(struct  tformctl * ,  struct  tformctl *) ; 
 * @brief detecting collision between  object   
 */ 
static void ttris_dectect_collision_between_object(struct  tformctl* __restrict__ current_figure , 
                                                   struct  tformctl* __restrict__ prevs_figure) ; 
/* @fn  ttris_dbg_prt(int , int  , const char * , ... )  
 * @brief  a debug print function  thats write on  (x,y) location in terminal 
 * @param  int coord x 
 * @param  int coord y 
 * @parm   const char  format 
 * @param  ... 
 */
void ttris_dbg_prt(int coordx  , int coordy , const char * fmt , ...)  ; 
#endif 
