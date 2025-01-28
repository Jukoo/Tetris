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

int
main(int ac  , char **av , char**env) 
{
  int pstatus=EXIT_SUCCESS ; 

  if (setting_terminal())
  {
     hdlerr(pstatus=EXIT_FAILURE, setting_terminal, "Broken Term") ;
  }

  kbrctl_mode&=~DIRSCTRL ; //!disable keyboard direction control 
  if(ttris()) 
  {
     hdlerr(pstatus=EXIT_FAILURE, setting_terminal,
         "Cannot Run Tetris Game\n") ;
  }

lgoto:
  return pstatus ; 

}
