/* @file main.c  
 * @brief  main  file that run the tetris game 
 * @author Umar Ba <jUmarB@protonmail.com> 
 */

#include <stdlib.h> 
#include <stdio.h>
#include <error.h> 
#include <unistd.h>
#include "termconfigure.h" 
#include "ttris.h" 

#define lgoto  __prlg  

#define hdlerr(errcode , fcall, ...)\
  do{error(errcode,0,#fcall #__VA_ARGS__); goto lgoto;}while(0)

#define TTRIS_DEFAULT_COORDXY_STARTUP_POSITION 10,0 

struct ttris_xy { 
   int _coordx;  
   int _coordy; 
}; 

int
main(int ac  , char **av , char**env) 
{
  int pstatus=EXIT_SUCCESS ;
  struct ttris_xy xy_begin = {TTRIS_DEFAULT_COORDXY_STARTUP_POSITION} ; 
  if(3 <= ac )
  {
    xy_begin._coordx = strtol(*(av+1),  nptr ,  10 ) ; 
    xy_begin._coordy = strtol(*(av+2),  nptr ,  10 ) ; 
  }  

  
  if (setting_terminal())
  {
     hdlerr(pstatus=EXIT_FAILURE, setting_terminal, "Broken Term") ;
  }

  kbrctl_mode&=~DIRSCTRL ; //!disable keyboard direction control 
  if(ttris(xy_begin._coordx , xy_begin._coordy))  
  {
     hdlerr(pstatus=EXIT_FAILURE, setting_terminal,
         "Cannot Run Tetris Game\n") ;
  }

lgoto:
  return pstatus ; 

}
