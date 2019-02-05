#include <stdio.h>
#include <stdlib.h>
#include "welcome.h"

void printWelcome(){

fprintf(stdout,
"██████╗ ██╗     ██╗████████╗███████╗██╗  ██╗██████╗ ██╗███████╗ ██████╗ \n" 
"██╔══██╗██║     ██║╚══██╔══╝╚══███╔╝██║ ██╔╝██╔══██╗██║██╔════╝██╔════╝ \n"
"██████╔╝██║     ██║   ██║     ███╔╝ █████╔╝ ██████╔╝██║█████╗  ██║  ███╗\n"
"██╔══██╗██║     ██║   ██║    ███╔╝  ██╔═██╗ ██╔══██╗██║██╔══╝  ██║   ██║\n"
"██████╔╝███████╗██║   ██║   ███████╗██║  ██╗██║  ██║██║███████╗╚██████╔╝\n"
"╚═════╝ ╚══════╝╚═╝   ╚═╝   ╚══════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝╚══════╝ ╚═════╝ \n");

fprintf(stdout, 
"Blitzkrieg is a word finder for the currently popular messenger game \"Word   \n"
"Blitz\" on the Facebook platform. Enter the 16 letters of the word puzzle (row \n"
"by row) in one line and then hit enter. All possible words will then be        \n"
"displayed in columns respective to the rank of each letter.                    \n\n");
}
