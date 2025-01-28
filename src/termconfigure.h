/*
 */
#if !defined(termconf_h) 
#define      termconf_h

#include <termios.h> 
#include <curses.h> 
#include <term.h>  
#include <sys/cdefs.h> 

#define nptr (__ptr_t ) 0 
#define targs (1<<0),putchar

#define tcexec(__cmd) tputs(__cmd , targs)  
 
/*@brief keyboard control */
typedef struct tcmd_kblctl_t tcmd_kblctl_t ; 
struct tcmd_kblctl_t { 
  char *_key_arr_u; 
  char *_key_arr_d; 
  char *_key_arr_r; 
  char *_key_arr_l; 
}; 


/* @brief  action command control */
typedef  struct tcmd_actctl_t tcmd_actctl_t; 
struct tcmd_actctl_t {
  char * _reset;
#define  cr_address  (0<<0) 
#define  cr_mode     (1<<0)
  char * _cursors[2] ;  
  char * _cls; 
  char * _bcolor; 
  char * _fcolor;  
}; 

typedef struct tcmd_t tcmd_t; 
struct  tcmd_t
{
  struct tcmd_kblctl_t tkblctl ; 
  struct tcmd_actctl_t tactctl ;  
}; 

extern struct tcmd_t tcmd ; 
#define tcmd_kb_set(_member,_value) \
  tcmd.tkblctl._member=_value

#define tcmd_cmd_set(_member,_value) \
  tcmd.tactctl._member=_value 

#define tcmd_kb_get(_member) tcmd.tkblctl._member 
#define tcmd_cmd_get(_member) tcmd.tactctl._member 
#define tcmdexec(_member)\
  tcexec(tcmd_cmd_get(_member)) 

#define tcmdexec_p(_member, _value)\
  tcexec(tiparm(tcmd_cmd_get(_member),_value))  

#define tcmdexec_g(_member, _valuex , _valuey) \
  tcexec(tgoto(tcmd_cmd_get(_member) , _valuex , _valuey))  


extern struct termios tios[2] ;  

/* @fn initterm(void)
 * @brief initialize  terminal for io behavior 
 * @return int  0: ok otherwize error 
 */
static int initerm(void);  

/* @fn restoreterm(void)
 * @brief bring back the initial mode of the terminal 
 * @return int 0:ok otherwise error 
 * 
 */
static void  restoreterm(void) __attribute__((destructor));  
/* @fn setting_terminal(void) 
 * @brief settup the terminal using term capabilities 
 * @return int 0:ok otherwise error
 */
int setting_terminal(void); 

/* @fn configure_cmdctl 
 * @brief configure command control  
 * @return int 0 : ok otherwise error 
 **/
static int configure_cmdctl(int mode ) ; 

#endif 
