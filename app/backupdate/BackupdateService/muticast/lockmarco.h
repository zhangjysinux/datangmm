#ifndef _LOCK_MARCO_H_
#define _LOCK_MARCO_H_

#include "externlockDefine.h"
#include <pthread.h>
#include<sys/wait.h>
#define LOCK(n) m_lock##n
#define MCLOCK(n) mclock##n
#define CON(n) cond##n

#define PTHREAD_MUTEX_M_LOCK()\
    switch(m_iGroupId)\
{\
    case 1:\
    pthread_mutex_lock(&LOCK(1));\
    break;\
    case 2:\
    pthread_mutex_lock(&LOCK(2));\
    break;\
    case 3:\
    pthread_mutex_lock(&LOCK(3));\
    break;\
    case 4:\
    pthread_mutex_lock(&LOCK(4));\
    break;\
    case 5:\
    pthread_mutex_lock(&LOCK(5));\
    break;\
    case 6:\
    pthread_mutex_lock(&LOCK(6));\
    break;\
    case 7:\
    pthread_mutex_lock(&LOCK(7));\
    break;\
    case 8:\
    pthread_mutex_lock(&LOCK(8));\
    break;\
    case 9:\
    pthread_mutex_lock(&LOCK(9));\
    break;\
    case 10:\
    pthread_mutex_lock(&LOCK(10));\
    break;\
    case 11:\
    pthread_mutex_lock(&LOCK(11));\
    break;\
    case 12:\
    pthread_mutex_lock(&LOCK(12));\
    break;\
    case 13:\
    pthread_mutex_lock(&LOCK(13));\
    break;\
    case 14:\
    pthread_mutex_lock(&LOCK(14));\
    break;\
    case 15:\
    pthread_mutex_lock(&LOCK(15));\
    break;\
    case 16:\
    pthread_mutex_lock(&LOCK(16));\
    break;\
    case 17:\
    pthread_mutex_lock(&LOCK(17));\
    break;\
    case 18:\
    pthread_mutex_lock(&LOCK(18));\
    break;\
    case 19:\
    pthread_mutex_lock(&LOCK(19));\
    break;\
    case 20:\
    pthread_mutex_lock(&LOCK(20));\
    break;\
    default:\
    break;\
    }\


#define PTHREAD_MUTEX_M_UNLOCK()\
    switch(m_iGroupId)\
{\
    case 1:\
    pthread_mutex_unlock(&LOCK(1));\
    break;\
    case 2:\
    pthread_mutex_unlock(&LOCK(2));\
    break;\
    case 3:\
    pthread_mutex_unlock(&LOCK(3));\
    break;\
    case 4:\
    pthread_mutex_unlock(&LOCK(4));\
    break;\
    case 5:\
    pthread_mutex_unlock(&LOCK(5));\
    break;\
    case 6:\
    pthread_mutex_unlock(&LOCK(6));\
    break;\
    case 7:\
    pthread_mutex_unlock(&LOCK(7));\
    break;\
    case 8:\
    pthread_mutex_unlock(&LOCK(8));\
    break;\
    case 9:\
    pthread_mutex_unlock(&LOCK(9));\
    break;\
    case 10:\
    pthread_mutex_lock(&LOCK(10));\
    break;\
    case 11:\
    pthread_mutex_unlock(&LOCK(11));\
    break;\
    case 12:\
    pthread_mutex_unlock(&LOCK(12));\
    break;\
    case 13:\
    pthread_mutex_unlock(&LOCK(13));\
    break;\
    case 14:\
    pthread_mutex_unlock(&LOCK(14));\
    break;\
    case 15:\
    pthread_mutex_unlock(&LOCK(15));\
    break;\
    case 16:\
    pthread_mutex_unlock(&LOCK(16));\
    break;\
    case 17:\
    pthread_mutex_unlock(&LOCK(17));\
    break;\
    case 18:\
    pthread_mutex_unlock(&LOCK(18));\
    break;\
    case 19:\
    pthread_mutex_unlock(&LOCK(19));\
    break;\
    case 20:\
    pthread_mutex_unlock(&LOCK(20));\
    break;\
    default:\
    break;\
    }\


#define PTHREAD_MUTEX_MC_LOCK()\
    switch(m_iGroupId)\
{\
    case 1:\
    pthread_mutex_lock(&MCLOCK(1));\
    break;\
    case 2:\
    pthread_mutex_lock(&MCLOCK(2));\
    break;\
    case 3:\
    pthread_mutex_lock(&MCLOCK(3));\
    break;\
    case 4:\
    pthread_mutex_lock(&MCLOCK(4));\
    break;\
    case 5:\
    pthread_mutex_lock(&MCLOCK(5));\
    break;\
    case 6:\
    pthread_mutex_lock(&MCLOCK(6));\
    break;\
    case 7:\
    pthread_mutex_lock(&MCLOCK(7));\
    break;\
    case 8:\
    pthread_mutex_lock(&MCLOCK(8));\
    break;\
    case 9:\
    pthread_mutex_lock(&MCLOCK(9));\
    break;\
    case 10:\
    pthread_mutex_lock(&MCLOCK(10));\
    break;\
    case 11:\
    pthread_mutex_lock(&MCLOCK(11));\
    break;\
    case 12:\
    pthread_mutex_lock(&MCLOCK(12));\
    break;\
    case 13:\
    pthread_mutex_lock(&MCLOCK(13));\
    break;\
    case 14:\
    pthread_mutex_lock(&MCLOCK(14));\
    break;\
    case 15:\
    pthread_mutex_lock(&MCLOCK(15));\
    break;\
    case 16:\
    pthread_mutex_lock(&MCLOCK(16));\
    break;\
    case 17:\
    pthread_mutex_lock(&MCLOCK(17));\
    break;\
    case 18:\
    pthread_mutex_lock(&MCLOCK(18));\
    break;\
    case 19:\
    pthread_mutex_lock(&MCLOCK(19));\
    break;\
    case 20:\
    pthread_mutex_lock(&MCLOCK(20));\
    break;\
    default:\
    break;\
    }\


#define PTHREAD_MUTEX_MC_UNLOCK()\
    switch(m_iGroupId)\
{\
    case 1:\
    pthread_mutex_unlock(&MCLOCK(1));\
    break;\
    case 2:\
    pthread_mutex_unlock(&MCLOCK(2));\
    break;\
    case 3:\
    pthread_mutex_unlock(&MCLOCK(3));\
    break;\
    case 4:\
    pthread_mutex_unlock(&MCLOCK(4));\
    break;\
    case 5:\
    pthread_mutex_unlock(&MCLOCK(5));\
    break;\
    case 6:\
    pthread_mutex_unlock(&MCLOCK(6));\
    break;\
    case 7:\
    pthread_mutex_unlock(&MCLOCK(7));\
    break;\
    case 8:\
    pthread_mutex_unlock(&MCLOCK(8));\
    break;\
    case 9:\
    pthread_mutex_unlock(&MCLOCK(9));\
    break;\
    case 10:\
    pthread_mutex_lock(&MCLOCK(10));\
    break;\
    case 11:\
    pthread_mutex_unlock(&MCLOCK(11));\
    break;\
    case 12:\
    pthread_mutex_unlock(&MCLOCK(12));\
    break;\
    case 13:\
    pthread_mutex_unlock(&MCLOCK(13));\
    break;\
    case 14:\
    pthread_mutex_unlock(&MCLOCK(14));\
    break;\
    case 15:\
    pthread_mutex_unlock(&MCLOCK(15));\
    break;\
    case 16:\
    pthread_mutex_unlock(&MCLOCK(16));\
    break;\
    case 17:\
    pthread_mutex_unlock(&MCLOCK(17));\
    break;\
    case 18:\
    pthread_mutex_unlock(&MCLOCK(18));\
    break;\
    case 19:\
    pthread_mutex_unlock(&MCLOCK(19));\
    break;\
    case 20:\
    pthread_mutex_unlock(&MCLOCK(20));\
    break;\
    default:\
    break;\
    }\


#define PTHREAD_COND_SIGNAL_T()\
    switch(m_iGroupId)\
{\
    case 1:\
    pthread_cond_signal(&CON(1));\
    break;\
    case 2:\
    pthread_cond_signal(&CON(2));\
    break;\
    case 3:\
    pthread_cond_signal(&CON(3));\
    break;\
    case 4:\
    pthread_cond_signal(&CON(4));\
    break;\
    case 5:\
    pthread_cond_signal(&CON(5));\
    break;\
    case 6:\
    pthread_cond_signal(&CON(6));\
    break;\
    case 7:\
    pthread_cond_signal(&CON(7));\
    break;\
    case 8:\
    pthread_cond_signal(&CON(8));\
    break;\
    case 9:\
    pthread_cond_signal(&CON(9));\
    break;\
    case 10:\
    pthread_cond_signal(&CON(10));\
    break;\
    case 11:\
    pthread_cond_signal(&CON(11));\
    break;\
    case 12:\
    pthread_cond_signal(&CON(12));\
    break;\
    case 13:\
    pthread_cond_signal(&CON(13));\
    break;\
    case 14:\
    pthread_cond_signal(&CON(14));\
    break;\
    case 15:\
    pthread_cond_signal(&CON(15));\
    break;\
    case 16:\
    pthread_cond_signal(&CON(16));\
    break;\
    case 17:\
    pthread_cond_signal(&CON(17));\
    break;\
    case 18:\
    pthread_cond_signal(&CON(18));\
    break;\
    case 19:\
    pthread_cond_signal(&CON(19));\
    break;\
    case 20:\
    pthread_cond_signal(&CON(20));\
    break;\
    default:\
    break;\
    }\

#define PTHREAD_COND_WAIT()\
    switch(m_iGroupId)\
{\
    case 1:\
    pthread_cond_wait(&CON(1),&MCLOCK(1));\
    break;\
    case 2:\
    pthread_cond_wait(&CON(2),&MCLOCK(2));\
    break;\
    case 3:\
    pthread_cond_wait(&CON(3),&MCLOCK(3));\
    break;\
    case 4:\
    pthread_cond_wait(&CON(4),&MCLOCK(4));\
    break;\
    case 5:\
    pthread_cond_wait(&CON(5),&MCLOCK(5));\
    break;\
    case 6:\
    pthread_cond_wait(&CON(6),&MCLOCK(6));\
    break;\
    case 7:\
    pthread_cond_wait(&CON(7),&MCLOCK(7));\
    break;\
    case 8:\
    pthread_cond_wait(&CON(8),&MCLOCK(8));\
    break;\
    case 9:\
    pthread_cond_wait(&CON(9),&MCLOCK(9));\
    break;\
    case 10:\
    pthread_cond_wait(&CON(10),&MCLOCK(10));\
    break;\
    case 11:\
    pthread_cond_wait(&CON(11),&MCLOCK(11));\
    break;\
    case 12:\
    pthread_cond_wait(&CON(12),&MCLOCK(12));\
    break;\
    case 13:\
    pthread_cond_wait(&CON(13),&MCLOCK(13));\
    break;\
    case 14:\
    pthread_cond_wait(&CON(14),&MCLOCK(14));\
    break;\
    case 15:\
    pthread_cond_wait(&CON(15),&MCLOCK(15));\
    break;\
    case 16:\
    pthread_cond_wait(&CON(16),&MCLOCK(16));\
    break;\
    case 17:\
    pthread_cond_wait(&CON(17),&MCLOCK(17));\
    break;\
    case 18:\
    pthread_cond_wait(&CON(18),&MCLOCK(18));\
    break;\
    case 19:\
    pthread_cond_wait(&CON(19),&MCLOCK(19));\
    break;\
    case 20:\
    pthread_cond_wait(&CON(20),&MCLOCK(20));\
    break;\
    default:\
    break;\
    }\

#endif
