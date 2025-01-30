/* @file tarea.h 
 * @brief area setup 
 * @author Umar Ba <jUmarB@protmail.com> 
 * */

#if  !defined(tarea_h)
#define tarea_h

#define  TEREA_WIDTH 0xf 
#define  TEREA_HEIGHT (TEREA_WIDTH << 1) 
#define  TEREA_SURFACE_DEFVAL  ~0 

#define  ascii_prt(__ascii_code_symbole)  \
  write(STDOUT_FILENO, (char[2]){__ascii_code_symbole & 0xff},2)



typedef struct  area_location_xy area_location_xy ; 
struct  area_location_xy { 
  int   _colx; 
  int   _rowy; 
  int   _area_playground[TEREA_HEIGHT][TEREA_WIDTH] ; 
}; 

extern int area_surface[TEREA_HEIGHT][TEREA_WIDTH] ; 


/* @fn draw_area_zone (int colum , int row) 
 * @brief draw the area of the game 
 * @param int  - coordonate  of colum x  
 * @param int  - coordonate  of row   y 
 * @return int status code 
 */

struct area_location_xy * draw_area_zone(int  start_colx , int start_rowy ,  int height , int width) ; 
struct area_location_xy * draw_area_zone_based(struct area_location_xy *  __restrict__   playground,
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
