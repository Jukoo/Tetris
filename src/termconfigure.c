/* @file termconfigure.c 
 * @brief configure  terminal  for initial setup 
 * @author Umar Ba <jUmarB@protonmail.com> 
 * @song : (Electronic Gems : Maximum Love Dream Gataways)  
 *
 * ###  GOOD ViBes ## 
 */ 
#include <stdlib.h> 
#include <unistd.h> 
#include <termios.h> 
#include <term.h> 
#include <curses.h> 
#include <stdio.h>
#include <error.h>
#include <errno.h>


#include "ttris.h"
#include "termconfigure.h" 


struct tcmd_t   tcmd ; 
struct termios tios[2]; 


static int initerm(void) 
{
   if (tcgetattr(STDIN_FILENO , (tios)))      goto _initermfail;  
   if (tcgetattr(STDIN_FILENO , (tios+0x1)))  goto _initermfail; 
  
   (tios)->c_lflag &=~(ICANON|ECHO) ;
   
   return tcsetattr(STDIN_FILENO , 0  , (tios)) ;

_initermfail: 
   return ~0 ;  
} 

static void restoreterm(void) 
{
  tcexec(flash_screen) ; 
  tcmdexec(_cls)  ;  
  tcmdexec(_reset);
  tcsetattr(STDIN_FILENO , 0 , (tios+0x1)) ;  
}

int setting_terminal(void) 
{
   if(initerm())
     return ~0 ; //!TODO : make warning instead  
   
   int error_return=0; 
   int status = setupterm(nptr ,  STDOUT_FILENO , &error_return); 
   if(OK != status && 1 != error_return) 
     return ~0 ; 
 
   //! disable buffering 
   setvbuf(stdout , nptr , _IONBF ,0) ; 
   configure_cmdctl(kbrctl_mode); 
}

static int configure_cmdctl(int  __mode) 
{
   //!set up keyboard control  NOTE : for later    
   //
   if  (__mode & DIRSCTRL) 
   {
     tcmd_kb_set(_key_arr_u, key_up); 
     tcmd_kb_set(_key_arr_d, key_down); 
     tcmd_kb_set(_key_arr_l, key_left); 
     tcmd_kb_set(_key_arr_r, key_right);  
   } 


   tcmd_cmd_set(_cls, clear_screen) ; 
   tcmd_cmd_set(_reset, exit_attribute_mode); 
   tcmd_cmd_set(_cursors[cr_address],cursor_address) ;  
   tcmd_cmd_set(_cursors[cr_mode],cursor_invisible) ;  
   tcmd_cmd_set(_bcolor, set_a_background) ; 
   tcmd_cmd_set(_fcolor, set_a_foreground) ; 
   

   return 0 ; 
}   
