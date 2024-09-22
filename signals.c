#include "minishell.h"  

//define cntrl c signal handler
void sigint_handler(int sig)
{
    if(sig == SIGINT)
    {
        write(1, "\n", 1);
        rl_on_new_line(); 
        rl_replace_line("", 0); 
        rl_redisplay();
    }
}



void all_signals()
{
    signal(SIGINT, sigint_handler);
    signal(SIGQUIT, SIG_IGN);   
}