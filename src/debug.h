#ifndef __DEBUG_H__
#define __DEBUG_H__

#ifdef DEBUG
  #undef DEBUG
  #define DEBUG 1
#else
  #define DEBUG 0
#endif /* ifdef DEBUG */

#endif /* __DEBUG_H__ */
