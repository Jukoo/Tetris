/* @file tarea.h 
 * @brief area setup 
 * @author Umar Ba <jUmarB@protmail.com> 
 * */

#if  !defined(tarea_h)
#define tarea_h

#define  TEREA_WIDTH 0xf 
#define  TEREA_HEIGHT (TEREA_WIDTH << 1) 
#define  TEREA_SURFACE_DEFVAL  ~0 


#define LINK_PREV_AREA_2_PGND_AREA   1
#define GAP_BETWEEN_PGND_AREA_n_PREV_AREA  10 

typedef struct  preview_area     preview_area ; 
struct  preview_area 
{
   int _colx ; 
   int _rowy ; 
} ; 

typedef struct  playground_area  playground_area;  
struct  playground_area { 
  int   _colx; 
  int   _rowy;
#if LINK_PREV_AREA_2_PGND_AREA
  struct preview_area  _preview_area ; 
#endif 
};


extern int area_surface[TEREA_HEIGHT][TEREA_WIDTH] ; 


/* @fn draw_area_zone (int colum , int row) 
 * @brief draw the area of the game 
 * @param int  - coordonate  of colum x  
 * @param int  - coordonate  of row   y 
 * @return int status code 
 */

void draw_area_zone(int  start_colx , int start_rowy ,  int height , int width) ; 
void draw_area_zone_based(struct playground_area *  __restrict__   playground,
                                               int height , int width);

/* @fn term_xymax(void)
 * @brief get  maximum cols and rows supported 
 * @return  int   a condensed  value 
 *  the LSB : raw 
 *  the MSB : col  
 *  using 16 bits signature 
 */

static int term_xymax(void) ; 
#endif 
