# 1 "squid_ldap_auth_and_ip.c"
# 1 "/tmp/b/squid3-3.1.20/helpers/basic_auth/LDAP//"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "squid_ldap_auth_and_ip.c"
# 87 "squid_ldap_auth_and_ip.c"
# 1 "../../../include/rfc1738.h" 1




# 1 "../../../include/config.h" 1
# 37 "../../../include/config.h"
# 1 "../../../include/autoconf.h" 1
# 38 "../../../include/config.h" 2



# 1 "../../../include/version.h" 1
# 42 "../../../include/config.h" 2
# 58 "../../../include/config.h"
# 1 "../../../compat/compat.h" 1
# 27 "../../../compat/compat.h"
# 1 "../../../compat/osdetect.h" 1
# 28 "../../../compat/compat.h" 2






# 1 "../../../compat/fdsetsize.h" 1
# 39 "../../../compat/fdsetsize.h"
# 1 "/usr/include/features.h" 1 3 4
# 313 "/usr/include/features.h" 3 4
# 1 "/usr/include/bits/predefs.h" 1 3 4
# 314 "/usr/include/features.h" 2 3 4
# 346 "/usr/include/features.h" 3 4
# 1 "/usr/include/sys/cdefs.h" 1 3 4
# 353 "/usr/include/sys/cdefs.h" 3 4
# 1 "/usr/include/bits/wordsize.h" 1 3 4
# 354 "/usr/include/sys/cdefs.h" 2 3 4
# 347 "/usr/include/features.h" 2 3 4
# 378 "/usr/include/features.h" 3 4
# 1 "/usr/include/gnu/stubs.h" 1 3 4



# 1 "/usr/include/bits/wordsize.h" 1 3 4
# 5 "/usr/include/gnu/stubs.h" 2 3 4


# 1 "/usr/include/gnu/stubs-32.h" 1 3 4
# 8 "/usr/include/gnu/stubs.h" 2 3 4
# 379 "/usr/include/features.h" 2 3 4
# 40 "../../../compat/fdsetsize.h" 2


# 1 "/usr/include/bits/types.h" 1 3 4
# 28 "/usr/include/bits/types.h" 3 4
# 1 "/usr/include/bits/wordsize.h" 1 3 4
# 29 "/usr/include/bits/types.h" 2 3 4


typedef unsigned char __u_char;
typedef unsigned short int __u_short;
typedef unsigned int __u_int;
typedef unsigned long int __u_long;


typedef signed char __int8_t;
typedef unsigned char __uint8_t;
typedef signed short int __int16_t;
typedef unsigned short int __uint16_t;
typedef signed int __int32_t;
typedef unsigned int __uint32_t;




__extension__ typedef signed long long int __int64_t;
__extension__ typedef unsigned long long int __uint64_t;







__extension__ typedef long long int __quad_t;
__extension__ typedef unsigned long long int __u_quad_t;
# 131 "/usr/include/bits/types.h" 3 4
# 1 "/usr/include/bits/typesizes.h" 1 3 4
# 132 "/usr/include/bits/types.h" 2 3 4


__extension__ typedef __u_quad_t __dev_t;
__extension__ typedef unsigned int __uid_t;
__extension__ typedef unsigned int __gid_t;
__extension__ typedef unsigned long int __ino_t;
__extension__ typedef __u_quad_t __ino64_t;
__extension__ typedef unsigned int __mode_t;
__extension__ typedef unsigned int __nlink_t;
__extension__ typedef long int __off_t;
__extension__ typedef __quad_t __off64_t;
__extension__ typedef int __pid_t;
__extension__ typedef struct { int __val[2]; } __fsid_t;
__extension__ typedef long int __clock_t;
__extension__ typedef unsigned long int __rlim_t;
__extension__ typedef __u_quad_t __rlim64_t;
__extension__ typedef unsigned int __id_t;
__extension__ typedef long int __time_t;
__extension__ typedef unsigned int __useconds_t;
__extension__ typedef long int __suseconds_t;

__extension__ typedef int __daddr_t;
__extension__ typedef long int __swblk_t;
__extension__ typedef int __key_t;


__extension__ typedef int __clockid_t;


__extension__ typedef void * __timer_t;


__extension__ typedef long int __blksize_t;




__extension__ typedef long int __blkcnt_t;
__extension__ typedef __quad_t __blkcnt64_t;


__extension__ typedef unsigned long int __fsblkcnt_t;
__extension__ typedef __u_quad_t __fsblkcnt64_t;


__extension__ typedef unsigned long int __fsfilcnt_t;
__extension__ typedef __u_quad_t __fsfilcnt64_t;

__extension__ typedef int __ssize_t;



typedef __off64_t __loff_t;
typedef __quad_t *__qaddr_t;
typedef char *__caddr_t;


__extension__ typedef int __intptr_t;


__extension__ typedef unsigned int __socklen_t;
# 43 "../../../compat/fdsetsize.h" 2

# 1 "/usr/include/linux/posix_types.h" 1 3 4



# 1 "/usr/include/linux/stddef.h" 1 3 4
# 5 "/usr/include/linux/posix_types.h" 2 3 4
# 36 "/usr/include/linux/posix_types.h" 3 4
typedef struct {
 unsigned long fds_bits [(1024/(8 * sizeof(unsigned long)))];
} __kernel_fd_set;


typedef void (*__kernel_sighandler_t)(int);


typedef int __kernel_key_t;
typedef int __kernel_mqd_t;

# 1 "/usr/include/asm/posix_types.h" 1 3 4

# 1 "/usr/include/asm/posix_types_32.h" 1 3 4
# 10 "/usr/include/asm/posix_types_32.h" 3 4
typedef unsigned long __kernel_ino_t;
typedef unsigned short __kernel_mode_t;
typedef unsigned short __kernel_nlink_t;
typedef long __kernel_off_t;
typedef int __kernel_pid_t;
typedef unsigned short __kernel_ipc_pid_t;
typedef unsigned short __kernel_uid_t;
typedef unsigned short __kernel_gid_t;
typedef unsigned int __kernel_size_t;
typedef int __kernel_ssize_t;
typedef int __kernel_ptrdiff_t;
typedef long __kernel_time_t;
typedef long __kernel_suseconds_t;
typedef long __kernel_clock_t;
typedef int __kernel_timer_t;
typedef int __kernel_clockid_t;
typedef int __kernel_daddr_t;
typedef char * __kernel_caddr_t;
typedef unsigned short __kernel_uid16_t;
typedef unsigned short __kernel_gid16_t;
typedef unsigned int __kernel_uid32_t;
typedef unsigned int __kernel_gid32_t;

typedef unsigned short __kernel_old_uid_t;
typedef unsigned short __kernel_old_gid_t;
typedef unsigned short __kernel_old_dev_t;


typedef long long __kernel_loff_t;


typedef struct {
 int val[2];
} __kernel_fsid_t;
# 3 "/usr/include/asm/posix_types.h" 2 3 4
# 48 "/usr/include/linux/posix_types.h" 2 3 4
# 45 "../../../compat/fdsetsize.h" 2
# 35 "../../../compat/compat.h" 2
# 45 "../../../compat/compat.h"
# 1 "../../../include/squid_types.h" 1
# 61 "../../../include/squid_types.h"
# 1 "/usr/include/sys/types.h" 1 3 4
# 29 "/usr/include/sys/types.h" 3 4






typedef __u_char u_char;
typedef __u_short u_short;
typedef __u_int u_int;
typedef __u_long u_long;
typedef __quad_t quad_t;
typedef __u_quad_t u_quad_t;
typedef __fsid_t fsid_t;




typedef __loff_t loff_t;





typedef __ino64_t ino_t;
# 62 "/usr/include/sys/types.h" 3 4
typedef __dev_t dev_t;




typedef __gid_t gid_t;




typedef __mode_t mode_t;




typedef __nlink_t nlink_t;




typedef __uid_t uid_t;







typedef __off64_t off_t;
# 100 "/usr/include/sys/types.h" 3 4
typedef __pid_t pid_t;




typedef __id_t id_t;




typedef __ssize_t ssize_t;





typedef __daddr_t daddr_t;
typedef __caddr_t caddr_t;





typedef __key_t key_t;
# 133 "/usr/include/sys/types.h" 3 4
# 1 "/usr/include/time.h" 1 3 4
# 74 "/usr/include/time.h" 3 4


typedef __time_t time_t;



# 92 "/usr/include/time.h" 3 4
typedef __clockid_t clockid_t;
# 104 "/usr/include/time.h" 3 4
typedef __timer_t timer_t;
# 134 "/usr/include/sys/types.h" 2 3 4
# 147 "/usr/include/sys/types.h" 3 4
# 1 "/usr/lib/gcc/i486-linux-gnu/4.4.5/include/stddef.h" 1 3 4
# 211 "/usr/lib/gcc/i486-linux-gnu/4.4.5/include/stddef.h" 3 4
typedef unsigned int size_t;
# 148 "/usr/include/sys/types.h" 2 3 4



typedef unsigned long int ulong;
typedef unsigned short int ushort;
typedef unsigned int uint;
# 195 "/usr/include/sys/types.h" 3 4
typedef int int8_t __attribute__ ((__mode__ (__QI__)));
typedef int int16_t __attribute__ ((__mode__ (__HI__)));
typedef int int32_t __attribute__ ((__mode__ (__SI__)));
typedef int int64_t __attribute__ ((__mode__ (__DI__)));


typedef unsigned int u_int8_t __attribute__ ((__mode__ (__QI__)));
typedef unsigned int u_int16_t __attribute__ ((__mode__ (__HI__)));
typedef unsigned int u_int32_t __attribute__ ((__mode__ (__SI__)));
typedef unsigned int u_int64_t __attribute__ ((__mode__ (__DI__)));

typedef int register_t __attribute__ ((__mode__ (__word__)));
# 217 "/usr/include/sys/types.h" 3 4
# 1 "/usr/include/endian.h" 1 3 4
# 37 "/usr/include/endian.h" 3 4
# 1 "/usr/include/bits/endian.h" 1 3 4
# 38 "/usr/include/endian.h" 2 3 4
# 61 "/usr/include/endian.h" 3 4
# 1 "/usr/include/bits/byteswap.h" 1 3 4
# 28 "/usr/include/bits/byteswap.h" 3 4
# 1 "/usr/include/bits/wordsize.h" 1 3 4
# 29 "/usr/include/bits/byteswap.h" 2 3 4
# 62 "/usr/include/endian.h" 2 3 4
# 218 "/usr/include/sys/types.h" 2 3 4


# 1 "/usr/include/sys/select.h" 1 3 4
# 31 "/usr/include/sys/select.h" 3 4
# 1 "/usr/include/bits/select.h" 1 3 4
# 23 "/usr/include/bits/select.h" 3 4
# 1 "/usr/include/bits/wordsize.h" 1 3 4
# 24 "/usr/include/bits/select.h" 2 3 4
# 32 "/usr/include/sys/select.h" 2 3 4


# 1 "/usr/include/bits/sigset.h" 1 3 4
# 24 "/usr/include/bits/sigset.h" 3 4
typedef int __sig_atomic_t;




typedef struct
  {
    unsigned long int __val[(1024 / (8 * sizeof (unsigned long int)))];
  } __sigset_t;
# 35 "/usr/include/sys/select.h" 2 3 4



typedef __sigset_t sigset_t;





# 1 "/usr/include/time.h" 1 3 4
# 120 "/usr/include/time.h" 3 4
struct timespec
  {
    __time_t tv_sec;
    long int tv_nsec;
  };
# 45 "/usr/include/sys/select.h" 2 3 4

# 1 "/usr/include/bits/time.h" 1 3 4
# 69 "/usr/include/bits/time.h" 3 4
struct timeval
  {
    __time_t tv_sec;
    __suseconds_t tv_usec;
  };
# 47 "/usr/include/sys/select.h" 2 3 4


typedef __suseconds_t suseconds_t;





typedef long int __fd_mask;
# 67 "/usr/include/sys/select.h" 3 4
typedef struct
  {






    __fd_mask __fds_bits[65536 / (8 * (int) sizeof (__fd_mask))];


  } fd_set;






typedef __fd_mask fd_mask;
# 99 "/usr/include/sys/select.h" 3 4

# 109 "/usr/include/sys/select.h" 3 4
extern int select (int __nfds, fd_set *__restrict __readfds,
     fd_set *__restrict __writefds,
     fd_set *__restrict __exceptfds,
     struct timeval *__restrict __timeout);
# 121 "/usr/include/sys/select.h" 3 4
extern int pselect (int __nfds, fd_set *__restrict __readfds,
      fd_set *__restrict __writefds,
      fd_set *__restrict __exceptfds,
      const struct timespec *__restrict __timeout,
      const __sigset_t *__restrict __sigmask);



# 221 "/usr/include/sys/types.h" 2 3 4


# 1 "/usr/include/sys/sysmacros.h" 1 3 4
# 30 "/usr/include/sys/sysmacros.h" 3 4
__extension__
extern unsigned int gnu_dev_major (unsigned long long int __dev)
     __attribute__ ((__nothrow__));
__extension__
extern unsigned int gnu_dev_minor (unsigned long long int __dev)
     __attribute__ ((__nothrow__));
__extension__
extern unsigned long long int gnu_dev_makedev (unsigned int __major,
            unsigned int __minor)
     __attribute__ ((__nothrow__));


__extension__ extern __inline unsigned int
__attribute__ ((__nothrow__)) gnu_dev_major (unsigned long long int __dev)
{
  return ((__dev >> 8) & 0xfff) | ((unsigned int) (__dev >> 32) & ~0xfff);
}

__extension__ extern __inline unsigned int
__attribute__ ((__nothrow__)) gnu_dev_minor (unsigned long long int __dev)
{
  return (__dev & 0xff) | ((unsigned int) (__dev >> 12) & ~0xff);
}

__extension__ extern __inline unsigned long long int
__attribute__ ((__nothrow__)) gnu_dev_makedev (unsigned int __major, unsigned int __minor)
{
  return ((__minor & 0xff) | ((__major & 0xfff) << 8)
   | (((unsigned long long int) (__minor & ~0xff)) << 12)
   | (((unsigned long long int) (__major & ~0xfff)) << 32));
}
# 224 "/usr/include/sys/types.h" 2 3 4
# 248 "/usr/include/sys/types.h" 3 4
typedef __blkcnt64_t blkcnt_t;



typedef __fsblkcnt64_t fsblkcnt_t;



typedef __fsfilcnt64_t fsfilcnt_t;
# 270 "/usr/include/sys/types.h" 3 4
# 1 "/usr/include/bits/pthreadtypes.h" 1 3 4
# 23 "/usr/include/bits/pthreadtypes.h" 3 4
# 1 "/usr/include/bits/wordsize.h" 1 3 4
# 24 "/usr/include/bits/pthreadtypes.h" 2 3 4
# 50 "/usr/include/bits/pthreadtypes.h" 3 4
typedef unsigned long int pthread_t;


typedef union
{
  char __size[36];
  long int __align;
} pthread_attr_t;
# 67 "/usr/include/bits/pthreadtypes.h" 3 4
typedef struct __pthread_internal_slist
{
  struct __pthread_internal_slist *__next;
} __pthread_slist_t;





typedef union
{
  struct __pthread_mutex_s
  {
    int __lock;
    unsigned int __count;
    int __owner;





    int __kind;





    unsigned int __nusers;
    __extension__ union
    {
      int __spins;
      __pthread_slist_t __list;
    };

  } __data;
  char __size[24];
  long int __align;
} pthread_mutex_t;

typedef union
{
  char __size[4];
  int __align;
} pthread_mutexattr_t;




typedef union
{
  struct
  {
    int __lock;
    unsigned int __futex;
    __extension__ unsigned long long int __total_seq;
    __extension__ unsigned long long int __wakeup_seq;
    __extension__ unsigned long long int __woken_seq;
    void *__mutex;
    unsigned int __nwaiters;
    unsigned int __broadcast_seq;
  } __data;
  char __size[48];
  __extension__ long long int __align;
} pthread_cond_t;

typedef union
{
  char __size[4];
  int __align;
} pthread_condattr_t;



typedef unsigned int pthread_key_t;



typedef int pthread_once_t;





typedef union
{
# 170 "/usr/include/bits/pthreadtypes.h" 3 4
  struct
  {
    int __lock;
    unsigned int __nr_readers;
    unsigned int __readers_wakeup;
    unsigned int __writer_wakeup;
    unsigned int __nr_readers_queued;
    unsigned int __nr_writers_queued;


    unsigned char __flags;
    unsigned char __shared;
    unsigned char __pad1;
    unsigned char __pad2;
    int __writer;
  } __data;

  char __size[32];
  long int __align;
} pthread_rwlock_t;

typedef union
{
  char __size[8];
  long int __align;
} pthread_rwlockattr_t;





typedef volatile int pthread_spinlock_t;




typedef union
{
  char __size[20];
  long int __align;
} pthread_barrier_t;

typedef union
{
  char __size[4];
  int __align;
} pthread_barrierattr_t;
# 271 "/usr/include/sys/types.h" 2 3 4



# 62 "../../../include/squid_types.h" 2


# 1 "/usr/include/linux/types.h" 1 3 4



# 1 "/usr/include/asm/types.h" 1 3 4



# 1 "/usr/include/asm-generic/types.h" 1 3 4






# 1 "/usr/include/asm-generic/int-ll64.h" 1 3 4
# 11 "/usr/include/asm-generic/int-ll64.h" 3 4
# 1 "/usr/include/asm/bitsperlong.h" 1 3 4
# 10 "/usr/include/asm/bitsperlong.h" 3 4
# 1 "/usr/include/asm-generic/bitsperlong.h" 1 3 4
# 11 "/usr/include/asm/bitsperlong.h" 2 3 4
# 12 "/usr/include/asm-generic/int-ll64.h" 2 3 4







typedef __signed__ char __s8;
typedef unsigned char __u8;

typedef __signed__ short __s16;
typedef unsigned short __u16;

typedef __signed__ int __s32;
typedef unsigned int __u32;


__extension__ typedef __signed__ long long __s64;
__extension__ typedef unsigned long long __u64;
# 8 "/usr/include/asm-generic/types.h" 2 3 4



typedef unsigned short umode_t;
# 5 "/usr/include/asm/types.h" 2 3 4
# 5 "/usr/include/linux/types.h" 2 3 4
# 27 "/usr/include/linux/types.h" 3 4
typedef __u16 __le16;
typedef __u16 __be16;
typedef __u32 __le32;
typedef __u32 __be32;
typedef __u64 __le64;
typedef __u64 __be64;

typedef __u16 __sum16;
typedef __u32 __wsum;
# 65 "../../../include/squid_types.h" 2


# 1 "/usr/include/stdlib.h" 1 3 4
# 33 "/usr/include/stdlib.h" 3 4
# 1 "/usr/lib/gcc/i486-linux-gnu/4.4.5/include/stddef.h" 1 3 4
# 323 "/usr/lib/gcc/i486-linux-gnu/4.4.5/include/stddef.h" 3 4
typedef int wchar_t;
# 34 "/usr/include/stdlib.h" 2 3 4


# 96 "/usr/include/stdlib.h" 3 4


typedef struct
  {
    int quot;
    int rem;
  } div_t;



typedef struct
  {
    long int quot;
    long int rem;
  } ldiv_t;







__extension__ typedef struct
  {
    long long int quot;
    long long int rem;
  } lldiv_t;


# 140 "/usr/include/stdlib.h" 3 4
extern size_t __ctype_get_mb_cur_max (void) __attribute__ ((__nothrow__)) __attribute__ ((__warn_unused_result__));




extern double atof (__const char *__nptr)
     __attribute__ ((__nothrow__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1))) __attribute__ ((__warn_unused_result__));

extern int atoi (__const char *__nptr)
     __attribute__ ((__nothrow__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1))) __attribute__ ((__warn_unused_result__));

extern long int atol (__const char *__nptr)
     __attribute__ ((__nothrow__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1))) __attribute__ ((__warn_unused_result__));





__extension__ extern long long int atoll (__const char *__nptr)
     __attribute__ ((__nothrow__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1))) __attribute__ ((__warn_unused_result__));





extern double strtod (__const char *__restrict __nptr,
        char **__restrict __endptr)
     __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1))) __attribute__ ((__warn_unused_result__));





extern float strtof (__const char *__restrict __nptr,
       char **__restrict __endptr) __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1))) __attribute__ ((__warn_unused_result__));

extern long double strtold (__const char *__restrict __nptr,
       char **__restrict __endptr)
     __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1))) __attribute__ ((__warn_unused_result__));





extern long int strtol (__const char *__restrict __nptr,
   char **__restrict __endptr, int __base)
     __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1))) __attribute__ ((__warn_unused_result__));

extern unsigned long int strtoul (__const char *__restrict __nptr,
      char **__restrict __endptr, int __base)
     __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1))) __attribute__ ((__warn_unused_result__));




__extension__
extern long long int strtoq (__const char *__restrict __nptr,
        char **__restrict __endptr, int __base)
     __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1))) __attribute__ ((__warn_unused_result__));

__extension__
extern unsigned long long int strtouq (__const char *__restrict __nptr,
           char **__restrict __endptr, int __base)
     __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1))) __attribute__ ((__warn_unused_result__));





__extension__
extern long long int strtoll (__const char *__restrict __nptr,
         char **__restrict __endptr, int __base)
     __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1))) __attribute__ ((__warn_unused_result__));

__extension__
extern unsigned long long int strtoull (__const char *__restrict __nptr,
     char **__restrict __endptr, int __base)
     __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1))) __attribute__ ((__warn_unused_result__));

# 277 "/usr/include/stdlib.h" 3 4

extern __inline double
__attribute__ ((__nothrow__)) atof (__const char *__nptr)
{
  return strtod (__nptr, (char **) ((void *)0));
}
extern __inline int
__attribute__ ((__nothrow__)) atoi (__const char *__nptr)
{
  return (int) strtol (__nptr, (char **) ((void *)0), 10);
}
extern __inline long int
__attribute__ ((__nothrow__)) atol (__const char *__nptr)
{
  return strtol (__nptr, (char **) ((void *)0), 10);
}




__extension__ extern __inline long long int
__attribute__ ((__nothrow__)) atoll (__const char *__nptr)
{
  return strtoll (__nptr, (char **) ((void *)0), 10);
}

# 311 "/usr/include/stdlib.h" 3 4
extern char *l64a (long int __n) __attribute__ ((__nothrow__)) __attribute__ ((__warn_unused_result__));


extern long int a64l (__const char *__s)
     __attribute__ ((__nothrow__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1))) __attribute__ ((__warn_unused_result__));
# 327 "/usr/include/stdlib.h" 3 4
extern long int random (void) __attribute__ ((__nothrow__));


extern void srandom (unsigned int __seed) __attribute__ ((__nothrow__));





extern char *initstate (unsigned int __seed, char *__statebuf,
   size_t __statelen) __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (2)));



extern char *setstate (char *__statebuf) __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1)));







struct random_data
  {
    int32_t *fptr;
    int32_t *rptr;
    int32_t *state;
    int rand_type;
    int rand_deg;
    int rand_sep;
    int32_t *end_ptr;
  };

extern int random_r (struct random_data *__restrict __buf,
       int32_t *__restrict __result) __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1, 2)));

extern int srandom_r (unsigned int __seed, struct random_data *__buf)
     __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (2)));

extern int initstate_r (unsigned int __seed, char *__restrict __statebuf,
   size_t __statelen,
   struct random_data *__restrict __buf)
     __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (2, 4)));

extern int setstate_r (char *__restrict __statebuf,
         struct random_data *__restrict __buf)
     __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1, 2)));






extern int rand (void) __attribute__ ((__nothrow__));

extern void srand (unsigned int __seed) __attribute__ ((__nothrow__));




extern int rand_r (unsigned int *__seed) __attribute__ ((__nothrow__));







extern double drand48 (void) __attribute__ ((__nothrow__));
extern double erand48 (unsigned short int __xsubi[3]) __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1)));


extern long int lrand48 (void) __attribute__ ((__nothrow__));
extern long int nrand48 (unsigned short int __xsubi[3])
     __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1)));


extern long int mrand48 (void) __attribute__ ((__nothrow__));
extern long int jrand48 (unsigned short int __xsubi[3])
     __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1)));


extern void srand48 (long int __seedval) __attribute__ ((__nothrow__));
extern unsigned short int *seed48 (unsigned short int __seed16v[3])
     __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1)));
extern void lcong48 (unsigned short int __param[7]) __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1)));





struct drand48_data
  {
    unsigned short int __x[3];
    unsigned short int __old_x[3];
    unsigned short int __c;
    unsigned short int __init;
    unsigned long long int __a;
  };


extern int drand48_r (struct drand48_data *__restrict __buffer,
        double *__restrict __result) __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1, 2)));
extern int erand48_r (unsigned short int __xsubi[3],
        struct drand48_data *__restrict __buffer,
        double *__restrict __result) __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1, 2)));


extern int lrand48_r (struct drand48_data *__restrict __buffer,
        long int *__restrict __result)
     __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1, 2)));
extern int nrand48_r (unsigned short int __xsubi[3],
        struct drand48_data *__restrict __buffer,
        long int *__restrict __result)
     __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1, 2)));


extern int mrand48_r (struct drand48_data *__restrict __buffer,
        long int *__restrict __result)
     __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1, 2)));
extern int jrand48_r (unsigned short int __xsubi[3],
        struct drand48_data *__restrict __buffer,
        long int *__restrict __result)
     __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1, 2)));


extern int srand48_r (long int __seedval, struct drand48_data *__buffer)
     __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (2)));

extern int seed48_r (unsigned short int __seed16v[3],
       struct drand48_data *__buffer) __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1, 2)));

extern int lcong48_r (unsigned short int __param[7],
        struct drand48_data *__buffer)
     __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1, 2)));









extern void *malloc (size_t __size) __attribute__ ((__nothrow__)) __attribute__ ((__malloc__)) __attribute__ ((__warn_unused_result__));

extern void *calloc (size_t __nmemb, size_t __size)
     __attribute__ ((__nothrow__)) __attribute__ ((__malloc__)) __attribute__ ((__warn_unused_result__));










extern void *realloc (void *__ptr, size_t __size)
     __attribute__ ((__nothrow__)) __attribute__ ((__warn_unused_result__));

extern void free (void *__ptr) __attribute__ ((__nothrow__));




extern void cfree (void *__ptr) __attribute__ ((__nothrow__));



# 1 "/usr/include/alloca.h" 1 3 4
# 25 "/usr/include/alloca.h" 3 4
# 1 "/usr/lib/gcc/i486-linux-gnu/4.4.5/include/stddef.h" 1 3 4
# 26 "/usr/include/alloca.h" 2 3 4







extern void *alloca (size_t __size) __attribute__ ((__nothrow__));






# 498 "/usr/include/stdlib.h" 2 3 4




extern void *valloc (size_t __size) __attribute__ ((__nothrow__)) __attribute__ ((__malloc__)) __attribute__ ((__warn_unused_result__));




extern int posix_memalign (void **__memptr, size_t __alignment, size_t __size)
     __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1))) __attribute__ ((__warn_unused_result__));




extern void abort (void) __attribute__ ((__nothrow__)) __attribute__ ((__noreturn__));



extern int atexit (void (*__func) (void)) __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1)));
# 530 "/usr/include/stdlib.h" 3 4





extern int on_exit (void (*__func) (int __status, void *__arg), void *__arg)
     __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1)));






extern void exit (int __status) __attribute__ ((__nothrow__)) __attribute__ ((__noreturn__));
# 553 "/usr/include/stdlib.h" 3 4






extern void _Exit (int __status) __attribute__ ((__nothrow__)) __attribute__ ((__noreturn__));






extern char *getenv (__const char *__name) __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1))) __attribute__ ((__warn_unused_result__));




extern char *__secure_getenv (__const char *__name)
     __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1))) __attribute__ ((__warn_unused_result__));





extern int putenv (char *__string) __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1)));





extern int setenv (__const char *__name, __const char *__value, int __replace)
     __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (2)));


extern int unsetenv (__const char *__name) __attribute__ ((__nothrow__));






extern int clearenv (void) __attribute__ ((__nothrow__));
# 604 "/usr/include/stdlib.h" 3 4
extern char *mktemp (char *__template) __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1))) __attribute__ ((__warn_unused_result__));
# 618 "/usr/include/stdlib.h" 3 4
extern int mkstemp (char *__template) __asm__ ("" "mkstemp64")
     __attribute__ ((__nonnull__ (1))) __attribute__ ((__warn_unused_result__));
# 640 "/usr/include/stdlib.h" 3 4
extern int mkstemps (char *__template, int __suffixlen) __asm__ ("" "mkstemps64") __attribute__ ((__nonnull__ (1))) __attribute__ ((__warn_unused_result__));
# 658 "/usr/include/stdlib.h" 3 4
extern char *mkdtemp (char *__template) __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1))) __attribute__ ((__warn_unused_result__));
# 707 "/usr/include/stdlib.h" 3 4





extern int system (__const char *__command) __attribute__ ((__warn_unused_result__));

# 729 "/usr/include/stdlib.h" 3 4
extern char *realpath (__const char *__restrict __name,
         char *__restrict __resolved) __attribute__ ((__nothrow__)) __attribute__ ((__warn_unused_result__));






typedef int (*__compar_fn_t) (__const void *, __const void *);
# 747 "/usr/include/stdlib.h" 3 4



extern void *bsearch (__const void *__key, __const void *__base,
        size_t __nmemb, size_t __size, __compar_fn_t __compar)
     __attribute__ ((__nonnull__ (1, 2, 5))) __attribute__ ((__warn_unused_result__));



extern void qsort (void *__base, size_t __nmemb, size_t __size,
     __compar_fn_t __compar) __attribute__ ((__nonnull__ (1, 4)));
# 766 "/usr/include/stdlib.h" 3 4
extern int abs (int __x) __attribute__ ((__nothrow__)) __attribute__ ((__const__)) __attribute__ ((__warn_unused_result__));
extern long int labs (long int __x) __attribute__ ((__nothrow__)) __attribute__ ((__const__)) __attribute__ ((__warn_unused_result__));



__extension__ extern long long int llabs (long long int __x)
     __attribute__ ((__nothrow__)) __attribute__ ((__const__)) __attribute__ ((__warn_unused_result__));







extern div_t div (int __numer, int __denom)
     __attribute__ ((__nothrow__)) __attribute__ ((__const__)) __attribute__ ((__warn_unused_result__));
extern ldiv_t ldiv (long int __numer, long int __denom)
     __attribute__ ((__nothrow__)) __attribute__ ((__const__)) __attribute__ ((__warn_unused_result__));




__extension__ extern lldiv_t lldiv (long long int __numer,
        long long int __denom)
     __attribute__ ((__nothrow__)) __attribute__ ((__const__)) __attribute__ ((__warn_unused_result__));

# 802 "/usr/include/stdlib.h" 3 4
extern char *ecvt (double __value, int __ndigit, int *__restrict __decpt,
     int *__restrict __sign) __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (3, 4))) __attribute__ ((__warn_unused_result__));




extern char *fcvt (double __value, int __ndigit, int *__restrict __decpt,
     int *__restrict __sign) __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (3, 4))) __attribute__ ((__warn_unused_result__));




extern char *gcvt (double __value, int __ndigit, char *__buf)
     __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (3))) __attribute__ ((__warn_unused_result__));




extern char *qecvt (long double __value, int __ndigit,
      int *__restrict __decpt, int *__restrict __sign)
     __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (3, 4))) __attribute__ ((__warn_unused_result__));
extern char *qfcvt (long double __value, int __ndigit,
      int *__restrict __decpt, int *__restrict __sign)
     __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (3, 4))) __attribute__ ((__warn_unused_result__));
extern char *qgcvt (long double __value, int __ndigit, char *__buf)
     __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (3))) __attribute__ ((__warn_unused_result__));




extern int ecvt_r (double __value, int __ndigit, int *__restrict __decpt,
     int *__restrict __sign, char *__restrict __buf,
     size_t __len) __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (3, 4, 5)));
extern int fcvt_r (double __value, int __ndigit, int *__restrict __decpt,
     int *__restrict __sign, char *__restrict __buf,
     size_t __len) __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (3, 4, 5)));

extern int qecvt_r (long double __value, int __ndigit,
      int *__restrict __decpt, int *__restrict __sign,
      char *__restrict __buf, size_t __len)
     __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (3, 4, 5)));
extern int qfcvt_r (long double __value, int __ndigit,
      int *__restrict __decpt, int *__restrict __sign,
      char *__restrict __buf, size_t __len)
     __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (3, 4, 5)));







extern int mblen (__const char *__s, size_t __n) __attribute__ ((__nothrow__)) __attribute__ ((__warn_unused_result__));


extern int mbtowc (wchar_t *__restrict __pwc,
     __const char *__restrict __s, size_t __n) __attribute__ ((__nothrow__)) __attribute__ ((__warn_unused_result__));


extern int wctomb (char *__s, wchar_t __wchar) __attribute__ ((__nothrow__)) __attribute__ ((__warn_unused_result__));



extern size_t mbstowcs (wchar_t *__restrict __pwcs,
   __const char *__restrict __s, size_t __n) __attribute__ ((__nothrow__));

extern size_t wcstombs (char *__restrict __s,
   __const wchar_t *__restrict __pwcs, size_t __n)
     __attribute__ ((__nothrow__));








extern int rpmatch (__const char *__response) __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1))) __attribute__ ((__warn_unused_result__));
# 907 "/usr/include/stdlib.h" 3 4
extern int posix_openpt (int __oflag) __attribute__ ((__warn_unused_result__));
# 942 "/usr/include/stdlib.h" 3 4
extern int getloadavg (double __loadavg[], int __nelem)
     __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1)));





# 1 "/usr/include/bits/stdlib.h" 1 3 4
# 24 "/usr/include/bits/stdlib.h" 3 4
extern char *__realpath_chk (__const char *__restrict __name,
        char *__restrict __resolved,
        size_t __resolvedlen) __attribute__ ((__nothrow__)) __attribute__ ((__warn_unused_result__));
extern char *__realpath_alias (__const char *__restrict __name, char *__restrict __resolved) __asm__ ("" "realpath") __attribute__ ((__nothrow__)) __attribute__ ((__warn_unused_result__));


extern char *__realpath_chk_warn (__const char *__restrict __name, char *__restrict __resolved, size_t __resolvedlen) __asm__ ("" "__realpath_chk") __attribute__ ((__nothrow__)) __attribute__ ((__warn_unused_result__))



     __attribute__((__warning__ ("second argument of realpath must be either NULL or at " "least PATH_MAX bytes long buffer")));


extern __inline __attribute__ ((__always_inline__)) __attribute__ ((__artificial__)) __attribute__ ((__warn_unused_result__)) char *
__attribute__ ((__nothrow__)) realpath (__const char *__restrict __name, char *__restrict __resolved)
{
  if (__builtin_object_size (__resolved, 2 > 1) != (size_t) -1)
    {




      return __realpath_chk (__name, __resolved, __builtin_object_size (__resolved, 2 > 1));
    }

  return __realpath_alias (__name, __resolved);
}


extern int __ptsname_r_chk (int __fd, char *__buf, size_t __buflen,
       size_t __nreal) __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (2)));
extern int __ptsname_r_alias (int __fd, char *__buf, size_t __buflen) __asm__ ("" "ptsname_r") __attribute__ ((__nothrow__))

     __attribute__ ((__nonnull__ (2)));
extern int __ptsname_r_chk_warn (int __fd, char *__buf, size_t __buflen, size_t __nreal) __asm__ ("" "__ptsname_r_chk") __attribute__ ((__nothrow__))


     __attribute__ ((__nonnull__ (2))) __attribute__((__warning__ ("ptsname_r called with buflen bigger than " "size of buf")));


extern __inline __attribute__ ((__always_inline__)) __attribute__ ((__artificial__)) int
__attribute__ ((__nothrow__)) ptsname_r (int __fd, char *__buf, size_t __buflen)
{
  if (__builtin_object_size (__buf, 2 > 1) != (size_t) -1)
    {
      if (!__builtin_constant_p (__buflen))
 return __ptsname_r_chk (__fd, __buf, __buflen, __builtin_object_size (__buf, 2 > 1));
      if (__buflen > __builtin_object_size (__buf, 2 > 1))
 return __ptsname_r_chk_warn (__fd, __buf, __buflen, __builtin_object_size (__buf, 2 > 1));
    }
  return __ptsname_r_alias (__fd, __buf, __buflen);
}


extern int __wctomb_chk (char *__s, wchar_t __wchar, size_t __buflen)
  __attribute__ ((__nothrow__)) __attribute__ ((__warn_unused_result__));
extern int __wctomb_alias (char *__s, wchar_t __wchar) __asm__ ("" "wctomb") __attribute__ ((__nothrow__)) __attribute__ ((__warn_unused_result__));


extern __inline __attribute__ ((__always_inline__)) __attribute__ ((__artificial__)) __attribute__ ((__warn_unused_result__)) int
__attribute__ ((__nothrow__)) wctomb (char *__s, wchar_t __wchar)
{







  if (__builtin_object_size (__s, 2 > 1) != (size_t) -1 && 16 > __builtin_object_size (__s, 2 > 1))
    return __wctomb_chk (__s, __wchar, __builtin_object_size (__s, 2 > 1));
  return __wctomb_alias (__s, __wchar);
}


extern size_t __mbstowcs_chk (wchar_t *__restrict __dst,
         __const char *__restrict __src,
         size_t __len, size_t __dstlen) __attribute__ ((__nothrow__));
extern size_t __mbstowcs_alias (wchar_t *__restrict __dst, __const char *__restrict __src, size_t __len) __asm__ ("" "mbstowcs") __attribute__ ((__nothrow__));



extern size_t __mbstowcs_chk_warn (wchar_t *__restrict __dst, __const char *__restrict __src, size_t __len, size_t __dstlen) __asm__ ("" "__mbstowcs_chk") __attribute__ ((__nothrow__))



     __attribute__((__warning__ ("mbstowcs called with dst buffer smaller than len " "* sizeof (wchar_t)")));


extern __inline __attribute__ ((__always_inline__)) __attribute__ ((__artificial__)) size_t
__attribute__ ((__nothrow__)) mbstowcs (wchar_t *__restrict __dst, __const char *__restrict __src, size_t __len)

{
  if (__builtin_object_size (__dst, 2 > 1) != (size_t) -1)
    {
      if (!__builtin_constant_p (__len))
 return __mbstowcs_chk (__dst, __src, __len,
          __builtin_object_size (__dst, 2 > 1) / sizeof (wchar_t));

      if (__len > __builtin_object_size (__dst, 2 > 1) / sizeof (wchar_t))
 return __mbstowcs_chk_warn (__dst, __src, __len,
         __builtin_object_size (__dst, 2 > 1) / sizeof (wchar_t));
    }
  return __mbstowcs_alias (__dst, __src, __len);
}


extern size_t __wcstombs_chk (char *__restrict __dst,
         __const wchar_t *__restrict __src,
         size_t __len, size_t __dstlen) __attribute__ ((__nothrow__));
extern size_t __wcstombs_alias (char *__restrict __dst, __const wchar_t *__restrict __src, size_t __len) __asm__ ("" "wcstombs") __attribute__ ((__nothrow__));



extern size_t __wcstombs_chk_warn (char *__restrict __dst, __const wchar_t *__restrict __src, size_t __len, size_t __dstlen) __asm__ ("" "__wcstombs_chk") __attribute__ ((__nothrow__))



     __attribute__((__warning__ ("wcstombs called with dst buffer smaller than len")));

extern __inline __attribute__ ((__always_inline__)) __attribute__ ((__artificial__)) size_t
__attribute__ ((__nothrow__)) wcstombs (char *__restrict __dst, __const wchar_t *__restrict __src, size_t __len)

{
  if (__builtin_object_size (__dst, 2 > 1) != (size_t) -1)
    {
      if (!__builtin_constant_p (__len))
 return __wcstombs_chk (__dst, __src, __len, __builtin_object_size (__dst, 2 > 1));
      if (__len > __builtin_object_size (__dst, 2 > 1))
 return __wcstombs_chk_warn (__dst, __src, __len, __builtin_object_size (__dst, 2 > 1));
    }
  return __wcstombs_alias (__dst, __src, __len);
}
# 950 "/usr/include/stdlib.h" 2 3 4
# 958 "/usr/include/stdlib.h" 3 4

# 68 "../../../include/squid_types.h" 2
# 1 "/usr/lib/gcc/i486-linux-gnu/4.4.5/include/stddef.h" 1 3 4
# 149 "/usr/lib/gcc/i486-linux-gnu/4.4.5/include/stddef.h" 3 4
typedef int ptrdiff_t;
# 69 "../../../include/squid_types.h" 2


# 1 "/usr/include/inttypes.h" 1 3 4
# 28 "/usr/include/inttypes.h" 3 4
# 1 "/usr/include/stdint.h" 1 3 4
# 27 "/usr/include/stdint.h" 3 4
# 1 "/usr/include/bits/wchar.h" 1 3 4
# 28 "/usr/include/stdint.h" 2 3 4
# 1 "/usr/include/bits/wordsize.h" 1 3 4
# 29 "/usr/include/stdint.h" 2 3 4
# 49 "/usr/include/stdint.h" 3 4
typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;

typedef unsigned int uint32_t;





__extension__
typedef unsigned long long int uint64_t;






typedef signed char int_least8_t;
typedef short int int_least16_t;
typedef int int_least32_t;



__extension__
typedef long long int int_least64_t;



typedef unsigned char uint_least8_t;
typedef unsigned short int uint_least16_t;
typedef unsigned int uint_least32_t;



__extension__
typedef unsigned long long int uint_least64_t;






typedef signed char int_fast8_t;





typedef int int_fast16_t;
typedef int int_fast32_t;
__extension__
typedef long long int int_fast64_t;



typedef unsigned char uint_fast8_t;





typedef unsigned int uint_fast16_t;
typedef unsigned int uint_fast32_t;
__extension__
typedef unsigned long long int uint_fast64_t;
# 126 "/usr/include/stdint.h" 3 4
typedef int intptr_t;


typedef unsigned int uintptr_t;
# 138 "/usr/include/stdint.h" 3 4
__extension__
typedef long long int intmax_t;
__extension__
typedef unsigned long long int uintmax_t;
# 29 "/usr/include/inttypes.h" 2 3 4






typedef int __gwchar_t;
# 274 "/usr/include/inttypes.h" 3 4

# 288 "/usr/include/inttypes.h" 3 4
typedef struct
  {
    long long int quot;
    long long int rem;
  } imaxdiv_t;





extern intmax_t imaxabs (intmax_t __n) __attribute__ ((__nothrow__)) __attribute__ ((__const__));


extern imaxdiv_t imaxdiv (intmax_t __numer, intmax_t __denom)
      __attribute__ ((__nothrow__)) __attribute__ ((__const__));


extern intmax_t strtoimax (__const char *__restrict __nptr,
      char **__restrict __endptr, int __base) __attribute__ ((__nothrow__));


extern uintmax_t strtoumax (__const char *__restrict __nptr,
       char ** __restrict __endptr, int __base) __attribute__ ((__nothrow__));


extern intmax_t wcstoimax (__const __gwchar_t *__restrict __nptr,
      __gwchar_t **__restrict __endptr, int __base)
     __attribute__ ((__nothrow__));


extern uintmax_t wcstoumax (__const __gwchar_t *__restrict __nptr,
       __gwchar_t ** __restrict __endptr, int __base)
     __attribute__ ((__nothrow__));
# 379 "/usr/include/inttypes.h" 3 4
__extension__
extern long long int __strtoll_internal (__const char *__restrict __nptr,
      char **__restrict __endptr,
      int __base, int __group)
  __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1))) __attribute__ ((__warn_unused_result__));

extern __inline intmax_t
__attribute__ ((__nothrow__)) strtoimax (__const char *__restrict nptr, char **__restrict endptr, int base)

{
  return __strtoll_internal (nptr, endptr, base, 0);
}

__extension__
extern unsigned long long int __strtoull_internal (__const char *
         __restrict __nptr,
         char **
         __restrict __endptr,
         int __base,
         int __group)
  __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1))) __attribute__ ((__warn_unused_result__));

extern __inline uintmax_t
__attribute__ ((__nothrow__)) strtoumax (__const char *__restrict nptr, char **__restrict endptr, int base)

{
  return __strtoull_internal (nptr, endptr, base, 0);
}

__extension__
extern long long int __wcstoll_internal (__const __gwchar_t *
      __restrict __nptr,
      __gwchar_t **__restrict __endptr,
      int __base, int __group)
  __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1))) __attribute__ ((__warn_unused_result__));

extern __inline intmax_t
__attribute__ ((__nothrow__)) wcstoimax (__const __gwchar_t *__restrict nptr, __gwchar_t **__restrict endptr, int base)

{
  return __wcstoll_internal (nptr, endptr, base, 0);
}


__extension__
extern unsigned long long int __wcstoull_internal (__const __gwchar_t *
         __restrict __nptr,
         __gwchar_t **
         __restrict __endptr,
         int __base,
         int __group)
  __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1))) __attribute__ ((__warn_unused_result__));

extern __inline uintmax_t
__attribute__ ((__nothrow__)) wcstoumax (__const __gwchar_t *__restrict nptr, __gwchar_t **__restrict endptr, int base)

{
  return __wcstoull_internal (nptr, endptr, base, 0);
}





# 72 "../../../include/squid_types.h" 2


# 1 "/usr/include/sys/bitypes.h" 1 3 4
# 75 "../../../include/squid_types.h" 2






# 1 "/usr/include/netinet/in_systm.h" 1 3 4
# 26 "/usr/include/netinet/in_systm.h" 3 4

# 35 "/usr/include/netinet/in_systm.h" 3 4
typedef u_int16_t n_short;
typedef u_int32_t n_long;
typedef u_int32_t n_time;


# 82 "../../../include/squid_types.h" 2
# 46 "../../../compat/compat.h" 2






# 1 "../../../compat/os/aix.h" 1
# 53 "../../../compat/compat.h" 2
# 1 "../../../compat/os/dragonfly.h" 1
# 54 "../../../compat/compat.h" 2
# 1 "../../../compat/os/freebsd.h" 1
# 55 "../../../compat/compat.h" 2
# 1 "../../../compat/os/hpux.h" 1
# 56 "../../../compat/compat.h" 2
# 1 "../../../compat/os/linux.h" 1
# 44 "../../../compat/os/linux.h"
# 1 "/usr/include/sys/capability.h" 1 3 4
# 32 "/usr/include/sys/capability.h" 3 4
# 1 "/usr/include/linux/capability.h" 1 3 4
# 18 "/usr/include/linux/capability.h" 3 4
struct task_struct;
# 40 "/usr/include/linux/capability.h" 3 4
typedef struct __user_cap_header_struct {
 __u32 version;
 int pid;
} *cap_user_header_t;

typedef struct __user_cap_data_struct {
        __u32 effective;
        __u32 permitted;
        __u32 inheritable;
} *cap_user_data_t;
# 69 "/usr/include/linux/capability.h" 3 4
struct vfs_cap_data {
 __le32 magic_etc;
 struct {
  __le32 permitted;
  __le32 inheritable;
 } data[2];
};
# 33 "/usr/include/sys/capability.h" 2 3 4
# 42 "/usr/include/sys/capability.h" 3 4
typedef struct _cap_struct *cap_t;







typedef int cap_value_t;




typedef enum {
    CAP_EFFECTIVE=0,
    CAP_PERMITTED=1,
    CAP_INHERITABLE=2
} cap_flag_t;




typedef enum {
    CAP_CLEAR=0,
    CAP_SET=1
} cap_flag_value_t;






extern cap_t cap_dup(cap_t);
extern int cap_free(void *);
extern cap_t cap_init(void);


extern int cap_get_flag(cap_t, cap_value_t, cap_flag_t, cap_flag_value_t *);
extern int cap_set_flag(cap_t, cap_flag_t, int, const cap_value_t *,
       cap_flag_value_t);
extern int cap_clear(cap_t);
extern int cap_clear_flag(cap_t, cap_flag_t);


extern cap_t cap_get_fd(int);
extern cap_t cap_get_file(const char *);
extern int cap_set_fd(int, cap_t);
extern int cap_set_file(const char *, cap_t);


extern cap_t cap_get_proc(void);
extern cap_t cap_get_pid(pid_t);
extern int cap_set_proc(cap_t);


extern ssize_t cap_size(cap_t);
extern ssize_t cap_copy_ext(void *, cap_t, ssize_t);
extern cap_t cap_copy_int(const void *);


extern cap_t cap_from_text(const char *);
extern char * cap_to_text(cap_t, ssize_t *);
extern int cap_from_name(const char *, cap_value_t *);
extern char * cap_to_name(cap_value_t);


extern int cap_compare(cap_t, cap_t);


extern int capset(cap_user_header_t header, cap_user_data_t data);
extern int capget(cap_user_header_t header, const cap_user_data_t data);


extern int capgetp(pid_t pid, cap_t cap_d);


extern int capsetp(pid_t pid, cap_t cap_d);
# 45 "../../../compat/os/linux.h" 2
# 57 "../../../compat/compat.h" 2
# 1 "../../../compat/os/macosx.h" 1
# 58 "../../../compat/compat.h" 2
# 1 "../../../compat/os/mswin.h" 1
# 59 "../../../compat/compat.h" 2
# 1 "../../../compat/os/next.h" 1
# 60 "../../../compat/compat.h" 2
# 1 "../../../compat/os/openbsd.h" 1
# 61 "../../../compat/compat.h" 2
# 1 "../../../compat/os/os2.h" 1
# 62 "../../../compat/compat.h" 2
# 1 "../../../compat/os/qnx.h" 1
# 63 "../../../compat/compat.h" 2
# 1 "../../../compat/os/sgi.h" 1
# 64 "../../../compat/compat.h" 2
# 1 "../../../compat/os/solaris.h" 1
# 65 "../../../compat/compat.h" 2
# 1 "../../../compat/os/sunos.h" 1
# 66 "../../../compat/compat.h" 2
# 1 "../../../compat/os/windows.h" 1
# 67 "../../../compat/compat.h" 2







# 1 "../../../compat/compat_shared.h" 1
# 25 "../../../compat/compat_shared.h"
# 1 "/usr/include/sys/time.h" 1 3 4
# 27 "/usr/include/sys/time.h" 3 4
# 1 "/usr/include/time.h" 1 3 4
# 28 "/usr/include/sys/time.h" 2 3 4

# 1 "/usr/include/bits/time.h" 1 3 4
# 30 "/usr/include/sys/time.h" 2 3 4
# 39 "/usr/include/sys/time.h" 3 4

# 57 "/usr/include/sys/time.h" 3 4
struct timezone
  {
    int tz_minuteswest;
    int tz_dsttime;
  };

typedef struct timezone *__restrict __timezone_ptr_t;
# 73 "/usr/include/sys/time.h" 3 4
extern int gettimeofday (struct timeval *__restrict __tv,
    __timezone_ptr_t __tz) __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1)));




extern int settimeofday (__const struct timeval *__tv,
    __const struct timezone *__tz)
     __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1)));





extern int adjtime (__const struct timeval *__delta,
      struct timeval *__olddelta) __attribute__ ((__nothrow__));




enum __itimer_which
  {

    ITIMER_REAL = 0,


    ITIMER_VIRTUAL = 1,



    ITIMER_PROF = 2

  };



struct itimerval
  {

    struct timeval it_interval;

    struct timeval it_value;
  };






typedef int __itimer_which_t;




extern int getitimer (__itimer_which_t __which,
        struct itimerval *__value) __attribute__ ((__nothrow__));




extern int setitimer (__itimer_which_t __which,
        __const struct itimerval *__restrict __new,
        struct itimerval *__restrict __old) __attribute__ ((__nothrow__));




extern int utimes (__const char *__file, __const struct timeval __tvp[2])
     __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1)));



extern int lutimes (__const char *__file, __const struct timeval __tvp[2])
     __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1)));


extern int futimes (int __fd, __const struct timeval __tvp[2]) __attribute__ ((__nothrow__));
# 191 "/usr/include/sys/time.h" 3 4

# 26 "../../../compat/compat_shared.h" 2


# 1 "/usr/include/sys/resource.h" 1 3 4
# 25 "/usr/include/sys/resource.h" 3 4
# 1 "/usr/include/bits/resource.h" 1 3 4
# 33 "/usr/include/bits/resource.h" 3 4
enum __rlimit_resource
{

  RLIMIT_CPU = 0,



  RLIMIT_FSIZE = 1,



  RLIMIT_DATA = 2,



  RLIMIT_STACK = 3,



  RLIMIT_CORE = 4,






  __RLIMIT_RSS = 5,



  RLIMIT_NOFILE = 7,
  __RLIMIT_OFILE = RLIMIT_NOFILE,




  RLIMIT_AS = 9,



  __RLIMIT_NPROC = 6,



  __RLIMIT_MEMLOCK = 8,



  __RLIMIT_LOCKS = 10,



  __RLIMIT_SIGPENDING = 11,



  __RLIMIT_MSGQUEUE = 12,





  __RLIMIT_NICE = 13,




  __RLIMIT_RTPRIO = 14,


  __RLIMIT_NLIMITS = 15,
  __RLIM_NLIMITS = __RLIMIT_NLIMITS


};
# 129 "/usr/include/bits/resource.h" 3 4
typedef __rlim64_t rlim_t;





struct rlimit
  {

    rlim_t rlim_cur;

    rlim_t rlim_max;
  };
# 154 "/usr/include/bits/resource.h" 3 4
enum __rusage_who
{

  RUSAGE_SELF = 0,



  RUSAGE_CHILDREN = -1
# 172 "/usr/include/bits/resource.h" 3 4
};


# 1 "/usr/include/bits/time.h" 1 3 4
# 176 "/usr/include/bits/resource.h" 2 3 4


struct rusage
  {

    struct timeval ru_utime;

    struct timeval ru_stime;

    long int ru_maxrss;


    long int ru_ixrss;

    long int ru_idrss;

    long int ru_isrss;


    long int ru_minflt;

    long int ru_majflt;

    long int ru_nswap;


    long int ru_inblock;

    long int ru_oublock;

    long int ru_msgsnd;

    long int ru_msgrcv;

    long int ru_nsignals;



    long int ru_nvcsw;


    long int ru_nivcsw;
  };







enum __priority_which
{
  PRIO_PROCESS = 0,

  PRIO_PGRP = 1,

  PRIO_USER = 2

};
# 26 "/usr/include/sys/resource.h" 2 3 4







# 43 "/usr/include/sys/resource.h" 3 4
typedef int __rlimit_resource_t;
typedef int __rusage_who_t;
typedef int __priority_which_t;
# 55 "/usr/include/sys/resource.h" 3 4
extern int getrlimit (__rlimit_resource_t __resource, struct rlimit *__rlimits) __asm__ ("" "getrlimit64") __attribute__ ((__nothrow__));
# 74 "/usr/include/sys/resource.h" 3 4
extern int setrlimit (__rlimit_resource_t __resource, __const struct rlimit *__rlimits) __asm__ ("" "setrlimit64") __attribute__ ((__nothrow__));
# 88 "/usr/include/sys/resource.h" 3 4
extern int getrusage (__rusage_who_t __who, struct rusage *__usage) __attribute__ ((__nothrow__));





extern int getpriority (__priority_which_t __which, id_t __who) __attribute__ ((__nothrow__));



extern int setpriority (__priority_which_t __which, id_t __who, int __prio)
     __attribute__ ((__nothrow__));


# 29 "../../../compat/compat_shared.h" 2







# 1 "/usr/include/dirent.h" 1 3 4
# 28 "/usr/include/dirent.h" 3 4

# 62 "/usr/include/dirent.h" 3 4
# 1 "/usr/include/bits/dirent.h" 1 3 4
# 23 "/usr/include/bits/dirent.h" 3 4
struct dirent
  {




    __ino64_t d_ino;
    __off64_t d_off;

    unsigned short int d_reclen;
    unsigned char d_type;
    char d_name[256];
  };
# 63 "/usr/include/dirent.h" 2 3 4
# 98 "/usr/include/dirent.h" 3 4
enum
  {
    DT_UNKNOWN = 0,

    DT_FIFO = 1,

    DT_CHR = 2,

    DT_DIR = 4,

    DT_BLK = 6,

    DT_REG = 8,

    DT_LNK = 10,

    DT_SOCK = 12,

    DT_WHT = 14

  };
# 128 "/usr/include/dirent.h" 3 4
typedef struct __dirstream DIR;






extern DIR *opendir (__const char *__name) __attribute__ ((__nonnull__ (1)));






extern DIR *fdopendir (int __fd);







extern int closedir (DIR *__dirp) __attribute__ ((__nonnull__ (1)));
# 166 "/usr/include/dirent.h" 3 4
extern struct dirent *readdir (DIR *__dirp) __asm__ ("" "readdir64")
     __attribute__ ((__nonnull__ (1)));
# 190 "/usr/include/dirent.h" 3 4
extern int readdir_r (DIR *__restrict __dirp, struct dirent *__restrict __entry, struct dirent **__restrict __result) __asm__ ("" "readdir64_r") __attribute__ ((__nonnull__ (1, 2, 3)));
# 209 "/usr/include/dirent.h" 3 4
extern void rewinddir (DIR *__dirp) __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1)));





extern void seekdir (DIR *__dirp, long int __pos) __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1)));


extern long int telldir (DIR *__dirp) __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1)));





extern int dirfd (DIR *__dirp) __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1)));
# 233 "/usr/include/dirent.h" 3 4
# 1 "/usr/include/bits/posix1_lim.h" 1 3 4
# 157 "/usr/include/bits/posix1_lim.h" 3 4
# 1 "/usr/include/bits/local_lim.h" 1 3 4
# 39 "/usr/include/bits/local_lim.h" 3 4
# 1 "/usr/include/linux/limits.h" 1 3 4
# 40 "/usr/include/bits/local_lim.h" 2 3 4
# 158 "/usr/include/bits/posix1_lim.h" 2 3 4
# 234 "/usr/include/dirent.h" 2 3 4
# 245 "/usr/include/dirent.h" 3 4
# 1 "/usr/lib/gcc/i486-linux-gnu/4.4.5/include/stddef.h" 1 3 4
# 246 "/usr/include/dirent.h" 2 3 4
# 260 "/usr/include/dirent.h" 3 4
extern int scandir (__const char *__restrict __dir, struct dirent ***__restrict __namelist, int (*__selector) (__const struct dirent *), int (*__cmp) (__const struct dirent **, __const struct dirent **)) __asm__ ("" "scandir64") __attribute__ ((__nonnull__ (1, 2)));
# 290 "/usr/include/dirent.h" 3 4
extern int alphasort (__const struct dirent **__e1, __const struct dirent **__e2) __asm__ ("" "alphasort64") __attribute__ ((__nothrow__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1, 2)));
# 319 "/usr/include/dirent.h" 3 4
extern __ssize_t getdirentries (int __fd, char *__restrict __buf, size_t __nbytes, __off64_t *__restrict __basep) __asm__ ("" "getdirentries64") __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (2, 4)));
# 362 "/usr/include/dirent.h" 3 4

# 37 "../../../compat/compat_shared.h" 2
# 186 "../../../compat/compat_shared.h"
# 1 "/usr/include/fcntl.h" 1 3 4
# 30 "/usr/include/fcntl.h" 3 4




# 1 "/usr/include/bits/fcntl.h" 1 3 4
# 25 "/usr/include/bits/fcntl.h" 3 4
# 1 "/usr/include/bits/wordsize.h" 1 3 4
# 26 "/usr/include/bits/fcntl.h" 2 3 4
# 160 "/usr/include/bits/fcntl.h" 3 4
struct flock
  {
    short int l_type;
    short int l_whence;




    __off64_t l_start;
    __off64_t l_len;

    __pid_t l_pid;
  };
# 259 "/usr/include/bits/fcntl.h" 3 4

# 304 "/usr/include/bits/fcntl.h" 3 4

# 35 "/usr/include/fcntl.h" 2 3 4
# 64 "/usr/include/fcntl.h" 3 4
extern int fcntl (int __fd, int __cmd, ...);
# 76 "/usr/include/fcntl.h" 3 4
extern int open (__const char *__file, int __oflag, ...) __asm__ ("" "open64")
     __attribute__ ((__nonnull__ (1)));
# 101 "/usr/include/fcntl.h" 3 4
extern int openat (int __fd, __const char *__file, int __oflag, ...) __asm__ ("" "openat64") __attribute__ ((__nonnull__ (2)));






extern int openat64 (int __fd, __const char *__file, int __oflag, ...)
     __attribute__ ((__nonnull__ (2)));
# 121 "/usr/include/fcntl.h" 3 4
extern int creat (__const char *__file, __mode_t __mode) __asm__ ("" "creat64") __attribute__ ((__nonnull__ (1)));
# 150 "/usr/include/fcntl.h" 3 4
extern int lockf (int __fd, int __cmd, __off64_t __len) __asm__ ("" "lockf64");
# 168 "/usr/include/fcntl.h" 3 4
extern int posix_fadvise (int __fd, __off64_t __offset, __off64_t __len, int __advise) __asm__ ("" "posix_fadvise64") __attribute__ ((__nothrow__));
# 189 "/usr/include/fcntl.h" 3 4
extern int posix_fallocate (int __fd, __off64_t __offset, __off64_t __len) __asm__ ("" "posix_fallocate64");
# 205 "/usr/include/fcntl.h" 3 4
# 1 "/usr/include/bits/fcntl2.h" 1 3 4
# 31 "/usr/include/bits/fcntl2.h" 3 4
extern int __open_2 (__const char *__path, int __oflag) __asm__ ("" "__open64_2") __attribute__ ((__nonnull__ (1)));

extern int __open_alias (__const char *__path, int __oflag, ...) __asm__ ("" "open64") __attribute__ ((__nonnull__ (1)));


extern void __open_too_many_args (void) __attribute__((__error__ ("open can be called either with 2 or 3 arguments, not more")));

extern void __open_missing_mode (void) __attribute__((__error__ ("open with O_CREAT in second argument needs 3 arguments")));


extern __inline __attribute__ ((__always_inline__)) __attribute__ ((__artificial__)) int
open (__const char *__path, int __oflag, ...)
{
  if (__builtin_va_arg_pack_len () > 1)
    __open_too_many_args ();

  if (__builtin_constant_p (__oflag))
    {
      if ((__oflag & 0100) != 0 && __builtin_va_arg_pack_len () < 1)
 {
   __open_missing_mode ();
   return __open_2 (__path, __oflag);
 }
      return __open_alias (__path, __oflag, __builtin_va_arg_pack ());
    }

  if (__builtin_va_arg_pack_len () < 1)
    return __open_2 (__path, __oflag);

  return __open_alias (__path, __oflag, __builtin_va_arg_pack ());
}
# 105 "/usr/include/bits/fcntl2.h" 3 4
extern int __openat_2 (int __fd, __const char *__path, int __oflag) __asm__ ("" "__openat64_2")

     __attribute__ ((__nonnull__ (2)));
extern int __openat_alias (int __fd, __const char *__path, int __oflag, ...) __asm__ ("" "openat64")

     __attribute__ ((__nonnull__ (2)));

extern void __openat_too_many_args (void) __attribute__((__error__ ("openat can be called either with 3 or 4 arguments, not more")));

extern void __openat_missing_mode (void) __attribute__((__error__ ("openat with O_CREAT in third argument needs 4 arguments")));


extern __inline __attribute__ ((__always_inline__)) __attribute__ ((__artificial__)) int
openat (int __fd, __const char *__path, int __oflag, ...)
{
  if (__builtin_va_arg_pack_len () > 1)
    __openat_too_many_args ();

  if (__builtin_constant_p (__oflag))
    {
      if ((__oflag & 0100) != 0 && __builtin_va_arg_pack_len () < 1)
 {
   __openat_missing_mode ();
   return __openat_2 (__fd, __path, __oflag);
 }
      return __openat_alias (__fd, __path, __oflag, __builtin_va_arg_pack ());
    }

  if (__builtin_va_arg_pack_len () < 1)
    return __openat_2 (__fd, __path, __oflag);

  return __openat_alias (__fd, __path, __oflag, __builtin_va_arg_pack ());
}
# 206 "/usr/include/fcntl.h" 2 3 4



# 187 "../../../compat/compat_shared.h" 2
# 207 "../../../compat/compat_shared.h"
# 1 "/usr/include/signal.h" 1 3 4
# 31 "/usr/include/signal.h" 3 4


# 1 "/usr/include/bits/sigset.h" 1 3 4
# 104 "/usr/include/bits/sigset.h" 3 4
extern int __sigismember (__const __sigset_t *, int);
extern int __sigaddset (__sigset_t *, int);
extern int __sigdelset (__sigset_t *, int);
# 118 "/usr/include/bits/sigset.h" 3 4
extern __inline int __sigismember (__const __sigset_t *__set, int __sig) { unsigned long int __mask = (((unsigned long int) 1) << (((__sig) - 1) % (8 * sizeof (unsigned long int)))); unsigned long int __word = (((__sig) - 1) / (8 * sizeof (unsigned long int))); return (__set->__val[__word] & __mask) ? 1 : 0; }
extern __inline int __sigaddset ( __sigset_t *__set, int __sig) { unsigned long int __mask = (((unsigned long int) 1) << (((__sig) - 1) % (8 * sizeof (unsigned long int)))); unsigned long int __word = (((__sig) - 1) / (8 * sizeof (unsigned long int))); return ((__set->__val[__word] |= __mask), 0); }
extern __inline int __sigdelset ( __sigset_t *__set, int __sig) { unsigned long int __mask = (((unsigned long int) 1) << (((__sig) - 1) % (8 * sizeof (unsigned long int)))); unsigned long int __word = (((__sig) - 1) / (8 * sizeof (unsigned long int))); return ((__set->__val[__word] &= ~__mask), 0); }
# 34 "/usr/include/signal.h" 2 3 4







typedef __sig_atomic_t sig_atomic_t;

# 58 "/usr/include/signal.h" 3 4
# 1 "/usr/include/bits/signum.h" 1 3 4
# 59 "/usr/include/signal.h" 2 3 4
# 76 "/usr/include/signal.h" 3 4
# 1 "/usr/include/time.h" 1 3 4
# 77 "/usr/include/signal.h" 2 3 4


# 1 "/usr/include/bits/siginfo.h" 1 3 4
# 25 "/usr/include/bits/siginfo.h" 3 4
# 1 "/usr/include/bits/wordsize.h" 1 3 4
# 26 "/usr/include/bits/siginfo.h" 2 3 4







typedef union sigval
  {
    int sival_int;
    void *sival_ptr;
  } sigval_t;
# 51 "/usr/include/bits/siginfo.h" 3 4
typedef struct siginfo
  {
    int si_signo;
    int si_errno;

    int si_code;

    union
      {
 int _pad[((128 / sizeof (int)) - 3)];


 struct
   {
     __pid_t si_pid;
     __uid_t si_uid;
   } _kill;


 struct
   {
     int si_tid;
     int si_overrun;
     sigval_t si_sigval;
   } _timer;


 struct
   {
     __pid_t si_pid;
     __uid_t si_uid;
     sigval_t si_sigval;
   } _rt;


 struct
   {
     __pid_t si_pid;
     __uid_t si_uid;
     int si_status;
     __clock_t si_utime;
     __clock_t si_stime;
   } _sigchld;


 struct
   {
     void *si_addr;
   } _sigfault;


 struct
   {
     long int si_band;
     int si_fd;
   } _sigpoll;
      } _sifields;
  } siginfo_t;
# 129 "/usr/include/bits/siginfo.h" 3 4
enum
{
  SI_ASYNCNL = -60,

  SI_TKILL = -6,

  SI_SIGIO,

  SI_ASYNCIO,

  SI_MESGQ,

  SI_TIMER,

  SI_QUEUE,

  SI_USER,

  SI_KERNEL = 0x80

};



enum
{
  ILL_ILLOPC = 1,

  ILL_ILLOPN,

  ILL_ILLADR,

  ILL_ILLTRP,

  ILL_PRVOPC,

  ILL_PRVREG,

  ILL_COPROC,

  ILL_BADSTK

};


enum
{
  FPE_INTDIV = 1,

  FPE_INTOVF,

  FPE_FLTDIV,

  FPE_FLTOVF,

  FPE_FLTUND,

  FPE_FLTRES,

  FPE_FLTINV,

  FPE_FLTSUB

};


enum
{
  SEGV_MAPERR = 1,

  SEGV_ACCERR

};


enum
{
  BUS_ADRALN = 1,

  BUS_ADRERR,

  BUS_OBJERR

};


enum
{
  TRAP_BRKPT = 1,

  TRAP_TRACE

};


enum
{
  CLD_EXITED = 1,

  CLD_KILLED,

  CLD_DUMPED,

  CLD_TRAPPED,

  CLD_STOPPED,

  CLD_CONTINUED

};


enum
{
  POLL_IN = 1,

  POLL_OUT,

  POLL_MSG,

  POLL_ERR,

  POLL_PRI,

  POLL_HUP

};
# 273 "/usr/include/bits/siginfo.h" 3 4
typedef struct sigevent
  {
    sigval_t sigev_value;
    int sigev_signo;
    int sigev_notify;

    union
      {
 int _pad[((64 / sizeof (int)) - 3)];



 __pid_t _tid;

 struct
   {
     void (*_function) (sigval_t);
     void *_attribute;
   } _sigev_thread;
      } _sigev_un;
  } sigevent_t;






enum
{
  SIGEV_SIGNAL = 0,

  SIGEV_NONE,

  SIGEV_THREAD,


  SIGEV_THREAD_ID = 4

};
# 80 "/usr/include/signal.h" 2 3 4




typedef void (*__sighandler_t) (int);




extern __sighandler_t __sysv_signal (int __sig, __sighandler_t __handler)
     __attribute__ ((__nothrow__));
# 99 "/usr/include/signal.h" 3 4


extern __sighandler_t signal (int __sig, __sighandler_t __handler)
     __attribute__ ((__nothrow__));
# 113 "/usr/include/signal.h" 3 4

# 126 "/usr/include/signal.h" 3 4
extern int kill (__pid_t __pid, int __sig) __attribute__ ((__nothrow__));






extern int killpg (__pid_t __pgrp, int __sig) __attribute__ ((__nothrow__));




extern int raise (int __sig) __attribute__ ((__nothrow__));




extern __sighandler_t ssignal (int __sig, __sighandler_t __handler)
     __attribute__ ((__nothrow__));
extern int gsignal (int __sig) __attribute__ ((__nothrow__));




extern void psignal (int __sig, __const char *__s);




extern void psiginfo (__const siginfo_t *__pinfo, __const char *__s);
# 168 "/usr/include/signal.h" 3 4
extern int __sigpause (int __sig_or_mask, int __is_sig);
# 196 "/usr/include/signal.h" 3 4
extern int sigblock (int __mask) __attribute__ ((__nothrow__)) __attribute__ ((__deprecated__));


extern int sigsetmask (int __mask) __attribute__ ((__nothrow__)) __attribute__ ((__deprecated__));


extern int siggetmask (void) __attribute__ ((__nothrow__)) __attribute__ ((__deprecated__));
# 216 "/usr/include/signal.h" 3 4
typedef __sighandler_t sig_t;





extern int sigemptyset (sigset_t *__set) __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1)));


extern int sigfillset (sigset_t *__set) __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1)));


extern int sigaddset (sigset_t *__set, int __signo) __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1)));


extern int sigdelset (sigset_t *__set, int __signo) __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1)));


extern int sigismember (__const sigset_t *__set, int __signo)
     __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1)));
# 252 "/usr/include/signal.h" 3 4
# 1 "/usr/include/bits/sigaction.h" 1 3 4
# 25 "/usr/include/bits/sigaction.h" 3 4
struct sigaction
  {


    union
      {

 __sighandler_t sa_handler;

 void (*sa_sigaction) (int, siginfo_t *, void *);
      }
    __sigaction_handler;







    __sigset_t sa_mask;


    int sa_flags;


    void (*sa_restorer) (void);
  };
# 253 "/usr/include/signal.h" 2 3 4


extern int sigprocmask (int __how, __const sigset_t *__restrict __set,
   sigset_t *__restrict __oset) __attribute__ ((__nothrow__));






extern int sigsuspend (__const sigset_t *__set) __attribute__ ((__nonnull__ (1)));


extern int sigaction (int __sig, __const struct sigaction *__restrict __act,
        struct sigaction *__restrict __oact) __attribute__ ((__nothrow__));


extern int sigpending (sigset_t *__set) __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1)));






extern int sigwait (__const sigset_t *__restrict __set, int *__restrict __sig)
     __attribute__ ((__nonnull__ (1, 2)));






extern int sigwaitinfo (__const sigset_t *__restrict __set,
   siginfo_t *__restrict __info) __attribute__ ((__nonnull__ (1)));






extern int sigtimedwait (__const sigset_t *__restrict __set,
    siginfo_t *__restrict __info,
    __const struct timespec *__restrict __timeout)
     __attribute__ ((__nonnull__ (1)));



extern int sigqueue (__pid_t __pid, int __sig, __const union sigval __val)
     __attribute__ ((__nothrow__));
# 310 "/usr/include/signal.h" 3 4
extern __const char *__const _sys_siglist[65];
extern __const char *__const sys_siglist[65];


struct sigvec
  {
    __sighandler_t sv_handler;
    int sv_mask;

    int sv_flags;

  };
# 334 "/usr/include/signal.h" 3 4
extern int sigvec (int __sig, __const struct sigvec *__vec,
     struct sigvec *__ovec) __attribute__ ((__nothrow__));



# 1 "/usr/include/bits/sigcontext.h" 1 3 4
# 26 "/usr/include/bits/sigcontext.h" 3 4
# 1 "/usr/include/bits/wordsize.h" 1 3 4
# 27 "/usr/include/bits/sigcontext.h" 2 3 4

struct _fpreg
{
  unsigned short significand[4];
  unsigned short exponent;
};

struct _fpxreg
{
  unsigned short significand[4];
  unsigned short exponent;
  unsigned short padding[3];
};

struct _xmmreg
{
  __uint32_t element[4];
};





struct _fpstate
{

  __uint32_t cw;
  __uint32_t sw;
  __uint32_t tag;
  __uint32_t ipoff;
  __uint32_t cssel;
  __uint32_t dataoff;
  __uint32_t datasel;
  struct _fpreg _st[8];
  unsigned short status;
  unsigned short magic;


  __uint32_t _fxsr_env[6];
  __uint32_t mxcsr;
  __uint32_t reserved;
  struct _fpxreg _fxsr_st[8];
  struct _xmmreg _xmm[8];
  __uint32_t padding[56];
};
# 81 "/usr/include/bits/sigcontext.h" 3 4
struct sigcontext
{
  unsigned short gs, __gsh;
  unsigned short fs, __fsh;
  unsigned short es, __esh;
  unsigned short ds, __dsh;
  unsigned long edi;
  unsigned long esi;
  unsigned long ebp;
  unsigned long esp;
  unsigned long ebx;
  unsigned long edx;
  unsigned long ecx;
  unsigned long eax;
  unsigned long trapno;
  unsigned long err;
  unsigned long eip;
  unsigned short cs, __csh;
  unsigned long eflags;
  unsigned long esp_at_signal;
  unsigned short ss, __ssh;
  struct _fpstate * fpstate;
  unsigned long oldmask;
  unsigned long cr2;
};
# 340 "/usr/include/signal.h" 2 3 4


extern int sigreturn (struct sigcontext *__scp) __attribute__ ((__nothrow__));






# 1 "/usr/lib/gcc/i486-linux-gnu/4.4.5/include/stddef.h" 1 3 4
# 350 "/usr/include/signal.h" 2 3 4




extern int siginterrupt (int __sig, int __interrupt) __attribute__ ((__nothrow__));

# 1 "/usr/include/bits/sigstack.h" 1 3 4
# 26 "/usr/include/bits/sigstack.h" 3 4
struct sigstack
  {
    void *ss_sp;
    int ss_onstack;
  };



enum
{
  SS_ONSTACK = 1,

  SS_DISABLE

};
# 50 "/usr/include/bits/sigstack.h" 3 4
typedef struct sigaltstack
  {
    void *ss_sp;
    int ss_flags;
    size_t ss_size;
  } stack_t;
# 357 "/usr/include/signal.h" 2 3 4
# 365 "/usr/include/signal.h" 3 4
extern int sigstack (struct sigstack *__ss, struct sigstack *__oss)
     __attribute__ ((__nothrow__)) __attribute__ ((__deprecated__));



extern int sigaltstack (__const struct sigaltstack *__restrict __ss,
   struct sigaltstack *__restrict __oss) __attribute__ ((__nothrow__));
# 395 "/usr/include/signal.h" 3 4
# 1 "/usr/include/bits/sigthread.h" 1 3 4
# 31 "/usr/include/bits/sigthread.h" 3 4
extern int pthread_sigmask (int __how,
       __const __sigset_t *__restrict __newmask,
       __sigset_t *__restrict __oldmask)__attribute__ ((__nothrow__));


extern int pthread_kill (pthread_t __threadid, int __signo) __attribute__ ((__nothrow__));
# 396 "/usr/include/signal.h" 2 3 4






extern int __libc_current_sigrtmin (void) __attribute__ ((__nothrow__));

extern int __libc_current_sigrtmax (void) __attribute__ ((__nothrow__));




# 208 "../../../compat/compat_shared.h" 2
# 75 "../../../compat/compat.h" 2
# 1 "../../../compat/stdvarargs.h" 1
# 18 "../../../compat/stdvarargs.h"
# 1 "/usr/lib/gcc/i486-linux-gnu/4.4.5/include/stdarg.h" 1 3 4
# 40 "/usr/lib/gcc/i486-linux-gnu/4.4.5/include/stdarg.h" 3 4
typedef __builtin_va_list __gnuc_va_list;
# 102 "/usr/lib/gcc/i486-linux-gnu/4.4.5/include/stdarg.h" 3 4
typedef __gnuc_va_list va_list;
# 19 "../../../compat/stdvarargs.h" 2
# 42 "../../../compat/stdvarargs.h"
# 1 "/usr/include/syslog.h" 1 3 4
# 1 "/usr/include/sys/syslog.h" 1 3 4
# 40 "/usr/include/sys/syslog.h" 3 4
# 1 "/usr/include/bits/syslog-path.h" 1 3 4
# 41 "/usr/include/sys/syslog.h" 2 3 4
# 169 "/usr/include/sys/syslog.h" 3 4






extern void closelog (void);





extern void openlog (__const char *__ident, int __option, int __facility);


extern int setlogmask (int __mask) __attribute__ ((__nothrow__));





extern void syslog (int __pri, __const char *__fmt, ...)
     __attribute__ ((__format__ (__printf__, 2, 3)));
# 200 "/usr/include/sys/syslog.h" 3 4
extern void vsyslog (int __pri, __const char *__fmt, __gnuc_va_list __ap)
     __attribute__ ((__format__ (__printf__, 2, 0)));





# 1 "/usr/include/bits/syslog.h" 1 3 4
# 25 "/usr/include/bits/syslog.h" 3 4
extern void __syslog_chk (int __pri, int __flag, __const char *__fmt, ...)
     __attribute__ ((__format__ (__printf__, 3, 4)));


extern __inline __attribute__ ((__always_inline__)) __attribute__ ((__artificial__)) void
syslog (int __pri, __const char *__fmt, ...)
{
  __syslog_chk (__pri, 2 - 1, __fmt, __builtin_va_arg_pack ());
}







extern void __vsyslog_chk (int __pri, int __flag, __const char *__fmt,
      __gnuc_va_list __ap)
     __attribute__ ((__format__ (__printf__, 3, 0)));

extern __inline __attribute__ ((__always_inline__)) __attribute__ ((__artificial__)) void
vsyslog (int __pri, __const char *__fmt, __gnuc_va_list __ap)
{
  __vsyslog_chk (__pri, 2 - 1, __fmt, __ap);
}
# 208 "/usr/include/sys/syslog.h" 2 3 4






# 1 "/usr/include/syslog.h" 2 3 4
# 43 "../../../compat/stdvarargs.h" 2
# 76 "../../../compat/compat.h" 2
# 1 "../../../compat/assert.h" 1
# 52 "../../../compat/assert.h"
extern void

xassert(const char *, const char *, int);
# 77 "../../../compat/compat.h" 2


# 1 "../../../compat/stdio.h" 1
# 18 "../../../compat/stdio.h"
# 1 "/usr/include/stdio.h" 1 3 4
# 30 "/usr/include/stdio.h" 3 4




# 1 "/usr/lib/gcc/i486-linux-gnu/4.4.5/include/stddef.h" 1 3 4
# 35 "/usr/include/stdio.h" 2 3 4
# 45 "/usr/include/stdio.h" 3 4
struct _IO_FILE;



typedef struct _IO_FILE FILE;





# 65 "/usr/include/stdio.h" 3 4
typedef struct _IO_FILE __FILE;
# 75 "/usr/include/stdio.h" 3 4
# 1 "/usr/include/libio.h" 1 3 4
# 32 "/usr/include/libio.h" 3 4
# 1 "/usr/include/_G_config.h" 1 3 4
# 15 "/usr/include/_G_config.h" 3 4
# 1 "/usr/lib/gcc/i486-linux-gnu/4.4.5/include/stddef.h" 1 3 4
# 16 "/usr/include/_G_config.h" 2 3 4




# 1 "/usr/include/wchar.h" 1 3 4
# 83 "/usr/include/wchar.h" 3 4
typedef struct
{
  int __count;
  union
  {

    unsigned int __wch;



    char __wchb[4];
  } __value;
} __mbstate_t;
# 21 "/usr/include/_G_config.h" 2 3 4

typedef struct
{
  __off_t __pos;
  __mbstate_t __state;
} _G_fpos_t;
typedef struct
{
  __off64_t __pos;
  __mbstate_t __state;
} _G_fpos64_t;
# 53 "/usr/include/_G_config.h" 3 4
typedef int _G_int16_t __attribute__ ((__mode__ (__HI__)));
typedef int _G_int32_t __attribute__ ((__mode__ (__SI__)));
typedef unsigned int _G_uint16_t __attribute__ ((__mode__ (__HI__)));
typedef unsigned int _G_uint32_t __attribute__ ((__mode__ (__SI__)));
# 33 "/usr/include/libio.h" 2 3 4
# 170 "/usr/include/libio.h" 3 4
struct _IO_jump_t; struct _IO_FILE;
# 180 "/usr/include/libio.h" 3 4
typedef void _IO_lock_t;





struct _IO_marker {
  struct _IO_marker *_next;
  struct _IO_FILE *_sbuf;



  int _pos;
# 203 "/usr/include/libio.h" 3 4
};


enum __codecvt_result
{
  __codecvt_ok,
  __codecvt_partial,
  __codecvt_error,
  __codecvt_noconv
};
# 271 "/usr/include/libio.h" 3 4
struct _IO_FILE {
  int _flags;




  char* _IO_read_ptr;
  char* _IO_read_end;
  char* _IO_read_base;
  char* _IO_write_base;
  char* _IO_write_ptr;
  char* _IO_write_end;
  char* _IO_buf_base;
  char* _IO_buf_end;

  char *_IO_save_base;
  char *_IO_backup_base;
  char *_IO_save_end;

  struct _IO_marker *_markers;

  struct _IO_FILE *_chain;

  int _fileno;



  int _flags2;

  __off_t _old_offset;



  unsigned short _cur_column;
  signed char _vtable_offset;
  char _shortbuf[1];



  _IO_lock_t *_lock;
# 319 "/usr/include/libio.h" 3 4
  __off64_t _offset;
# 328 "/usr/include/libio.h" 3 4
  void *__pad1;
  void *__pad2;
  void *__pad3;
  void *__pad4;
  size_t __pad5;

  int _mode;

  char _unused2[15 * sizeof (int) - 4 * sizeof (void *) - sizeof (size_t)];

};


typedef struct _IO_FILE _IO_FILE;


struct _IO_FILE_plus;

extern struct _IO_FILE_plus _IO_2_1_stdin_;
extern struct _IO_FILE_plus _IO_2_1_stdout_;
extern struct _IO_FILE_plus _IO_2_1_stderr_;
# 364 "/usr/include/libio.h" 3 4
typedef __ssize_t __io_read_fn (void *__cookie, char *__buf, size_t __nbytes);







typedef __ssize_t __io_write_fn (void *__cookie, __const char *__buf,
     size_t __n);







typedef int __io_seek_fn (void *__cookie, __off64_t *__pos, int __w);


typedef int __io_close_fn (void *__cookie);
# 416 "/usr/include/libio.h" 3 4
extern int __underflow (_IO_FILE *);
extern int __uflow (_IO_FILE *);
extern int __overflow (_IO_FILE *, int);
# 460 "/usr/include/libio.h" 3 4
extern int _IO_getc (_IO_FILE *__fp);
extern int _IO_putc (int __c, _IO_FILE *__fp);
extern int _IO_feof (_IO_FILE *__fp) __attribute__ ((__nothrow__));
extern int _IO_ferror (_IO_FILE *__fp) __attribute__ ((__nothrow__));

extern int _IO_peekc_locked (_IO_FILE *__fp);





extern void _IO_flockfile (_IO_FILE *) __attribute__ ((__nothrow__));
extern void _IO_funlockfile (_IO_FILE *) __attribute__ ((__nothrow__));
extern int _IO_ftrylockfile (_IO_FILE *) __attribute__ ((__nothrow__));
# 490 "/usr/include/libio.h" 3 4
extern int _IO_vfscanf (_IO_FILE * __restrict, const char * __restrict,
   __gnuc_va_list, int *__restrict);
extern int _IO_vfprintf (_IO_FILE *__restrict, const char *__restrict,
    __gnuc_va_list);
extern __ssize_t _IO_padn (_IO_FILE *, int, __ssize_t);
extern size_t _IO_sgetn (_IO_FILE *, void *, size_t);

extern __off64_t _IO_seekoff (_IO_FILE *, __off64_t, int, int);
extern __off64_t _IO_seekpos (_IO_FILE *, __off64_t, int);

extern void _IO_free_backup_area (_IO_FILE *) __attribute__ ((__nothrow__));
# 76 "/usr/include/stdio.h" 2 3 4
# 89 "/usr/include/stdio.h" 3 4




typedef _G_fpos64_t fpos_t;


# 141 "/usr/include/stdio.h" 3 4
# 1 "/usr/include/bits/stdio_lim.h" 1 3 4
# 142 "/usr/include/stdio.h" 2 3 4



extern struct _IO_FILE *stdin;
extern struct _IO_FILE *stdout;
extern struct _IO_FILE *stderr;







extern int remove (__const char *__filename) __attribute__ ((__nothrow__));

extern int rename (__const char *__old, __const char *__new) __attribute__ ((__nothrow__));




extern int renameat (int __oldfd, __const char *__old, int __newfd,
       __const char *__new) __attribute__ ((__nothrow__));



# 175 "/usr/include/stdio.h" 3 4
extern FILE *tmpfile (void) __asm__ ("" "tmpfile64") __attribute__ ((__warn_unused_result__));
# 186 "/usr/include/stdio.h" 3 4
extern char *tmpnam (char *__s) __attribute__ ((__nothrow__)) __attribute__ ((__warn_unused_result__));





extern char *tmpnam_r (char *__s) __attribute__ ((__nothrow__)) __attribute__ ((__warn_unused_result__));
# 204 "/usr/include/stdio.h" 3 4
extern char *tempnam (__const char *__dir, __const char *__pfx)
     __attribute__ ((__nothrow__)) __attribute__ ((__malloc__)) __attribute__ ((__warn_unused_result__));








extern int fclose (FILE *__stream);




extern int fflush (FILE *__stream);

# 229 "/usr/include/stdio.h" 3 4
extern int fflush_unlocked (FILE *__stream);
# 243 "/usr/include/stdio.h" 3 4

# 260 "/usr/include/stdio.h" 3 4
extern FILE *fopen (__const char *__restrict __filename, __const char *__restrict __modes) __asm__ ("" "fopen64")

  __attribute__ ((__warn_unused_result__));
extern FILE *freopen (__const char *__restrict __filename, __const char *__restrict __modes, FILE *__restrict __stream) __asm__ ("" "freopen64")


  __attribute__ ((__warn_unused_result__));






# 283 "/usr/include/stdio.h" 3 4
extern FILE *fdopen (int __fd, __const char *__modes) __attribute__ ((__nothrow__)) __attribute__ ((__warn_unused_result__));
# 296 "/usr/include/stdio.h" 3 4
extern FILE *fmemopen (void *__s, size_t __len, __const char *__modes)
  __attribute__ ((__nothrow__)) __attribute__ ((__warn_unused_result__));




extern FILE *open_memstream (char **__bufloc, size_t *__sizeloc) __attribute__ ((__nothrow__)) __attribute__ ((__warn_unused_result__));






extern void setbuf (FILE *__restrict __stream, char *__restrict __buf) __attribute__ ((__nothrow__));



extern int setvbuf (FILE *__restrict __stream, char *__restrict __buf,
      int __modes, size_t __n) __attribute__ ((__nothrow__));





extern void setbuffer (FILE *__restrict __stream, char *__restrict __buf,
         size_t __size) __attribute__ ((__nothrow__));


extern void setlinebuf (FILE *__stream) __attribute__ ((__nothrow__));








extern int fprintf (FILE *__restrict __stream,
      __const char *__restrict __format, ...);




extern int printf (__const char *__restrict __format, ...);

extern int sprintf (char *__restrict __s,
      __const char *__restrict __format, ...) __attribute__ ((__nothrow__));





extern int vfprintf (FILE *__restrict __s, __const char *__restrict __format,
       __gnuc_va_list __arg);




extern int vprintf (__const char *__restrict __format, __gnuc_va_list __arg);

extern int vsprintf (char *__restrict __s, __const char *__restrict __format,
       __gnuc_va_list __arg) __attribute__ ((__nothrow__));





extern int snprintf (char *__restrict __s, size_t __maxlen,
       __const char *__restrict __format, ...)
     __attribute__ ((__nothrow__)) __attribute__ ((__format__ (__printf__, 3, 4)));

extern int vsnprintf (char *__restrict __s, size_t __maxlen,
        __const char *__restrict __format, __gnuc_va_list __arg)
     __attribute__ ((__nothrow__)) __attribute__ ((__format__ (__printf__, 3, 0)));

# 394 "/usr/include/stdio.h" 3 4
extern int vdprintf (int __fd, __const char *__restrict __fmt,
       __gnuc_va_list __arg)
     __attribute__ ((__format__ (__printf__, 2, 0)));
extern int dprintf (int __fd, __const char *__restrict __fmt, ...)
     __attribute__ ((__format__ (__printf__, 2, 3)));








extern int fscanf (FILE *__restrict __stream,
     __const char *__restrict __format, ...) __attribute__ ((__warn_unused_result__));




extern int scanf (__const char *__restrict __format, ...) __attribute__ ((__warn_unused_result__));

extern int sscanf (__const char *__restrict __s,
     __const char *__restrict __format, ...) __attribute__ ((__nothrow__));
# 425 "/usr/include/stdio.h" 3 4
extern int fscanf (FILE *__restrict __stream, __const char *__restrict __format, ...) __asm__ ("" "__isoc99_fscanf") __attribute__ ((__warn_unused_result__));


extern int scanf (__const char *__restrict __format, ...) __asm__ ("" "__isoc99_scanf") __attribute__ ((__warn_unused_result__));

extern int sscanf (__const char *__restrict __s, __const char *__restrict __format, ...) __asm__ ("" "__isoc99_sscanf") __attribute__ ((__nothrow__));
# 445 "/usr/include/stdio.h" 3 4








extern int vfscanf (FILE *__restrict __s, __const char *__restrict __format,
      __gnuc_va_list __arg)
     __attribute__ ((__format__ (__scanf__, 2, 0))) __attribute__ ((__warn_unused_result__));





extern int vscanf (__const char *__restrict __format, __gnuc_va_list __arg)
     __attribute__ ((__format__ (__scanf__, 1, 0))) __attribute__ ((__warn_unused_result__));


extern int vsscanf (__const char *__restrict __s,
      __const char *__restrict __format, __gnuc_va_list __arg)
     __attribute__ ((__nothrow__)) __attribute__ ((__format__ (__scanf__, 2, 0)));
# 476 "/usr/include/stdio.h" 3 4
extern int vfscanf (FILE *__restrict __s, __const char *__restrict __format, __gnuc_va_list __arg) __asm__ ("" "__isoc99_vfscanf")



     __attribute__ ((__format__ (__scanf__, 2, 0))) __attribute__ ((__warn_unused_result__));
extern int vscanf (__const char *__restrict __format, __gnuc_va_list __arg) __asm__ ("" "__isoc99_vscanf")

     __attribute__ ((__format__ (__scanf__, 1, 0))) __attribute__ ((__warn_unused_result__));
extern int vsscanf (__const char *__restrict __s, __const char *__restrict __format, __gnuc_va_list __arg) __asm__ ("" "__isoc99_vsscanf") __attribute__ ((__nothrow__))



     __attribute__ ((__format__ (__scanf__, 2, 0)));
# 504 "/usr/include/stdio.h" 3 4









extern int fgetc (FILE *__stream);
extern int getc (FILE *__stream);





extern int getchar (void);

# 532 "/usr/include/stdio.h" 3 4
extern int getc_unlocked (FILE *__stream);
extern int getchar_unlocked (void);
# 543 "/usr/include/stdio.h" 3 4
extern int fgetc_unlocked (FILE *__stream);











extern int fputc (int __c, FILE *__stream);
extern int putc (int __c, FILE *__stream);





extern int putchar (int __c);

# 576 "/usr/include/stdio.h" 3 4
extern int fputc_unlocked (int __c, FILE *__stream);







extern int putc_unlocked (int __c, FILE *__stream);
extern int putchar_unlocked (int __c);






extern int getw (FILE *__stream);


extern int putw (int __w, FILE *__stream);








extern char *fgets (char *__restrict __s, int __n, FILE *__restrict __stream)
     __attribute__ ((__warn_unused_result__));






extern char *gets (char *__s) __attribute__ ((__warn_unused_result__));

# 638 "/usr/include/stdio.h" 3 4
extern __ssize_t __getdelim (char **__restrict __lineptr,
          size_t *__restrict __n, int __delimiter,
          FILE *__restrict __stream) __attribute__ ((__warn_unused_result__));
extern __ssize_t getdelim (char **__restrict __lineptr,
        size_t *__restrict __n, int __delimiter,
        FILE *__restrict __stream) __attribute__ ((__warn_unused_result__));







extern __ssize_t getline (char **__restrict __lineptr,
       size_t *__restrict __n,
       FILE *__restrict __stream) __attribute__ ((__warn_unused_result__));








extern int fputs (__const char *__restrict __s, FILE *__restrict __stream);





extern int puts (__const char *__s);






extern int ungetc (int __c, FILE *__stream);






extern size_t fread (void *__restrict __ptr, size_t __size,
       size_t __n, FILE *__restrict __stream) __attribute__ ((__warn_unused_result__));




extern size_t fwrite (__const void *__restrict __ptr, size_t __size,
        size_t __n, FILE *__restrict __s) __attribute__ ((__warn_unused_result__));

# 710 "/usr/include/stdio.h" 3 4
extern size_t fread_unlocked (void *__restrict __ptr, size_t __size,
         size_t __n, FILE *__restrict __stream) __attribute__ ((__warn_unused_result__));
extern size_t fwrite_unlocked (__const void *__restrict __ptr, size_t __size,
          size_t __n, FILE *__restrict __stream) __attribute__ ((__warn_unused_result__));








extern int fseek (FILE *__stream, long int __off, int __whence);




extern long int ftell (FILE *__stream) __attribute__ ((__warn_unused_result__));




extern void rewind (FILE *__stream);

# 754 "/usr/include/stdio.h" 3 4
extern int fseeko (FILE *__stream, __off64_t __off, int __whence) __asm__ ("" "fseeko64");


extern __off64_t ftello (FILE *__stream) __asm__ ("" "ftello64");








# 779 "/usr/include/stdio.h" 3 4
extern int fgetpos (FILE *__restrict __stream, fpos_t *__restrict __pos) __asm__ ("" "fgetpos64");

extern int fsetpos (FILE *__stream, __const fpos_t *__pos) __asm__ ("" "fsetpos64");







# 797 "/usr/include/stdio.h" 3 4


extern void clearerr (FILE *__stream) __attribute__ ((__nothrow__));

extern int feof (FILE *__stream) __attribute__ ((__nothrow__)) __attribute__ ((__warn_unused_result__));

extern int ferror (FILE *__stream) __attribute__ ((__nothrow__)) __attribute__ ((__warn_unused_result__));




extern void clearerr_unlocked (FILE *__stream) __attribute__ ((__nothrow__));
extern int feof_unlocked (FILE *__stream) __attribute__ ((__nothrow__)) __attribute__ ((__warn_unused_result__));
extern int ferror_unlocked (FILE *__stream) __attribute__ ((__nothrow__)) __attribute__ ((__warn_unused_result__));








extern void perror (__const char *__s);






# 1 "/usr/include/bits/sys_errlist.h" 1 3 4
# 27 "/usr/include/bits/sys_errlist.h" 3 4
extern int sys_nerr;
extern __const char *__const sys_errlist[];
# 827 "/usr/include/stdio.h" 2 3 4




extern int fileno (FILE *__stream) __attribute__ ((__nothrow__)) __attribute__ ((__warn_unused_result__));




extern int fileno_unlocked (FILE *__stream) __attribute__ ((__nothrow__)) __attribute__ ((__warn_unused_result__));
# 846 "/usr/include/stdio.h" 3 4
extern FILE *popen (__const char *__command, __const char *__modes) __attribute__ ((__warn_unused_result__));





extern int pclose (FILE *__stream);





extern char *ctermid (char *__s) __attribute__ ((__nothrow__));
# 886 "/usr/include/stdio.h" 3 4
extern void flockfile (FILE *__stream) __attribute__ ((__nothrow__));



extern int ftrylockfile (FILE *__stream) __attribute__ ((__nothrow__)) __attribute__ ((__warn_unused_result__));


extern void funlockfile (FILE *__stream) __attribute__ ((__nothrow__));
# 907 "/usr/include/stdio.h" 3 4
# 1 "/usr/include/bits/stdio.h" 1 3 4
# 44 "/usr/include/bits/stdio.h" 3 4
extern __inline int
getchar (void)
{
  return _IO_getc (stdin);
}




extern __inline int
fgetc_unlocked (FILE *__fp)
{
  return (__builtin_expect (((__fp)->_IO_read_ptr >= (__fp)->_IO_read_end), 0) ? __uflow (__fp) : *(unsigned char *) (__fp)->_IO_read_ptr++);
}





extern __inline int
getc_unlocked (FILE *__fp)
{
  return (__builtin_expect (((__fp)->_IO_read_ptr >= (__fp)->_IO_read_end), 0) ? __uflow (__fp) : *(unsigned char *) (__fp)->_IO_read_ptr++);
}


extern __inline int
getchar_unlocked (void)
{
  return (__builtin_expect (((stdin)->_IO_read_ptr >= (stdin)->_IO_read_end), 0) ? __uflow (stdin) : *(unsigned char *) (stdin)->_IO_read_ptr++);
}




extern __inline int
putchar (int __c)
{
  return _IO_putc (__c, stdout);
}




extern __inline int
fputc_unlocked (int __c, FILE *__stream)
{
  return (__builtin_expect (((__stream)->_IO_write_ptr >= (__stream)->_IO_write_end), 0) ? __overflow (__stream, (unsigned char) (__c)) : (unsigned char) (*(__stream)->_IO_write_ptr++ = (__c)));
}





extern __inline int
putc_unlocked (int __c, FILE *__stream)
{
  return (__builtin_expect (((__stream)->_IO_write_ptr >= (__stream)->_IO_write_end), 0) ? __overflow (__stream, (unsigned char) (__c)) : (unsigned char) (*(__stream)->_IO_write_ptr++ = (__c)));
}


extern __inline int
putchar_unlocked (int __c)
{
  return (__builtin_expect (((stdout)->_IO_write_ptr >= (stdout)->_IO_write_end), 0) ? __overflow (stdout, (unsigned char) (__c)) : (unsigned char) (*(stdout)->_IO_write_ptr++ = (__c)));
}
# 125 "/usr/include/bits/stdio.h" 3 4
extern __inline int
__attribute__ ((__nothrow__)) feof_unlocked (FILE *__stream)
{
  return (((__stream)->_flags & 0x10) != 0);
}


extern __inline int
__attribute__ ((__nothrow__)) ferror_unlocked (FILE *__stream)
{
  return (((__stream)->_flags & 0x20) != 0);
}
# 908 "/usr/include/stdio.h" 2 3 4


# 1 "/usr/include/bits/stdio2.h" 1 3 4
# 24 "/usr/include/bits/stdio2.h" 3 4
extern int __sprintf_chk (char *__restrict __s, int __flag, size_t __slen,
     __const char *__restrict __format, ...) __attribute__ ((__nothrow__));
extern int __vsprintf_chk (char *__restrict __s, int __flag, size_t __slen,
      __const char *__restrict __format,
      __gnuc_va_list __ap) __attribute__ ((__nothrow__));


extern __inline __attribute__ ((__always_inline__)) __attribute__ ((__artificial__)) int
__attribute__ ((__nothrow__)) sprintf (char *__restrict __s, __const char *__restrict __fmt, ...)
{
  return __builtin___sprintf_chk (__s, 2 - 1,
      __builtin_object_size (__s, 2 > 1), __fmt, __builtin_va_arg_pack ());
}






extern __inline __attribute__ ((__always_inline__)) __attribute__ ((__artificial__)) int
__attribute__ ((__nothrow__)) vsprintf (char *__restrict __s, __const char *__restrict __fmt, __gnuc_va_list __ap)

{
  return __builtin___vsprintf_chk (__s, 2 - 1,
       __builtin_object_size (__s, 2 > 1), __fmt, __ap);
}



extern int __snprintf_chk (char *__restrict __s, size_t __n, int __flag,
      size_t __slen, __const char *__restrict __format,
      ...) __attribute__ ((__nothrow__));
extern int __vsnprintf_chk (char *__restrict __s, size_t __n, int __flag,
       size_t __slen, __const char *__restrict __format,
       __gnuc_va_list __ap) __attribute__ ((__nothrow__));


extern __inline __attribute__ ((__always_inline__)) __attribute__ ((__artificial__)) int
__attribute__ ((__nothrow__)) snprintf (char *__restrict __s, size_t __n, __const char *__restrict __fmt, ...)

{
  return __builtin___snprintf_chk (__s, __n, 2 - 1,
       __builtin_object_size (__s, 2 > 1), __fmt, __builtin_va_arg_pack ());
}






extern __inline __attribute__ ((__always_inline__)) __attribute__ ((__artificial__)) int
__attribute__ ((__nothrow__)) vsnprintf (char *__restrict __s, size_t __n, __const char *__restrict __fmt, __gnuc_va_list __ap)

{
  return __builtin___vsnprintf_chk (__s, __n, 2 - 1,
        __builtin_object_size (__s, 2 > 1), __fmt, __ap);
}





extern int __fprintf_chk (FILE *__restrict __stream, int __flag,
     __const char *__restrict __format, ...);
extern int __printf_chk (int __flag, __const char *__restrict __format, ...);
extern int __vfprintf_chk (FILE *__restrict __stream, int __flag,
      __const char *__restrict __format, __gnuc_va_list __ap);
extern int __vprintf_chk (int __flag, __const char *__restrict __format,
     __gnuc_va_list __ap);


extern __inline __attribute__ ((__always_inline__)) __attribute__ ((__artificial__)) int
fprintf (FILE *__restrict __stream, __const char *__restrict __fmt, ...)
{
  return __fprintf_chk (__stream, 2 - 1, __fmt,
   __builtin_va_arg_pack ());
}

extern __inline __attribute__ ((__always_inline__)) __attribute__ ((__artificial__)) int
printf (__const char *__restrict __fmt, ...)
{
  return __printf_chk (2 - 1, __fmt, __builtin_va_arg_pack ());
}







extern __inline __attribute__ ((__always_inline__)) __attribute__ ((__artificial__)) int
vprintf (__const char *__restrict __fmt, __gnuc_va_list __ap)
{

  return __vfprintf_chk (stdout, 2 - 1, __fmt, __ap);



}

extern __inline __attribute__ ((__always_inline__)) __attribute__ ((__artificial__)) int
vfprintf (FILE *__restrict __stream,
   __const char *__restrict __fmt, __gnuc_va_list __ap)
{
  return __vfprintf_chk (__stream, 2 - 1, __fmt, __ap);
}
# 220 "/usr/include/bits/stdio2.h" 3 4
extern char *__gets_chk (char *__str, size_t) __attribute__ ((__warn_unused_result__));
extern char *__gets_warn (char *__str) __asm__ ("" "gets")
     __attribute__ ((__warn_unused_result__)) __attribute__((__warning__ ("please use fgets or getline instead, gets can't " "specify buffer size")));


extern __inline __attribute__ ((__always_inline__)) __attribute__ ((__artificial__)) __attribute__ ((__warn_unused_result__)) char *
gets (char *__str)
{
  if (__builtin_object_size (__str, 2 > 1) != (size_t) -1)
    return __gets_chk (__str, __builtin_object_size (__str, 2 > 1));
  return __gets_warn (__str);
}

extern char *__fgets_chk (char *__restrict __s, size_t __size, int __n,
     FILE *__restrict __stream) __attribute__ ((__warn_unused_result__));
extern char *__fgets_alias (char *__restrict __s, int __n, FILE *__restrict __stream) __asm__ ("" "fgets") __attribute__ ((__warn_unused_result__));


extern char *__fgets_chk_warn (char *__restrict __s, size_t __size, int __n, FILE *__restrict __stream) __asm__ ("" "__fgets_chk")


     __attribute__ ((__warn_unused_result__)) __attribute__((__warning__ ("fgets called with bigger size than length " "of destination buffer")));


extern __inline __attribute__ ((__always_inline__)) __attribute__ ((__artificial__)) __attribute__ ((__warn_unused_result__)) char *
fgets (char *__restrict __s, int __n, FILE *__restrict __stream)
{
  if (__builtin_object_size (__s, 2 > 1) != (size_t) -1)
    {
      if (!__builtin_constant_p (__n) || __n <= 0)
 return __fgets_chk (__s, __builtin_object_size (__s, 2 > 1), __n, __stream);

      if ((size_t) __n > __builtin_object_size (__s, 2 > 1))
 return __fgets_chk_warn (__s, __builtin_object_size (__s, 2 > 1), __n, __stream);
    }
  return __fgets_alias (__s, __n, __stream);
}

extern size_t __fread_chk (void *__restrict __ptr, size_t __ptrlen,
      size_t __size, size_t __n,
      FILE *__restrict __stream) __attribute__ ((__warn_unused_result__));
extern size_t __fread_alias (void *__restrict __ptr, size_t __size, size_t __n, FILE *__restrict __stream) __asm__ ("" "fread") __attribute__ ((__warn_unused_result__));



extern size_t __fread_chk_warn (void *__restrict __ptr, size_t __ptrlen, size_t __size, size_t __n, FILE *__restrict __stream) __asm__ ("" "__fread_chk")




     __attribute__ ((__warn_unused_result__)) __attribute__((__warning__ ("fread called with bigger size * nmemb than length " "of destination buffer")));


extern __inline __attribute__ ((__always_inline__)) __attribute__ ((__artificial__)) __attribute__ ((__warn_unused_result__)) size_t
fread (void *__restrict __ptr, size_t __size, size_t __n,
       FILE *__restrict __stream)
{
  if (__builtin_object_size (__ptr, 0) != (size_t) -1)
    {
      if (!__builtin_constant_p (__size)
   || !__builtin_constant_p (__n)
   || (__size | __n) >= (((size_t) 1) << (8 * sizeof (size_t) / 2)))
 return __fread_chk (__ptr, __builtin_object_size (__ptr, 0), __size, __n, __stream);

      if (__size * __n > __builtin_object_size (__ptr, 0))
 return __fread_chk_warn (__ptr, __builtin_object_size (__ptr, 0), __size, __n, __stream);
    }
  return __fread_alias (__ptr, __size, __n, __stream);
}
# 319 "/usr/include/bits/stdio2.h" 3 4
extern size_t __fread_unlocked_chk (void *__restrict __ptr, size_t __ptrlen,
        size_t __size, size_t __n,
        FILE *__restrict __stream) __attribute__ ((__warn_unused_result__));
extern size_t __fread_unlocked_alias (void *__restrict __ptr, size_t __size, size_t __n, FILE *__restrict __stream) __asm__ ("" "fread_unlocked") __attribute__ ((__warn_unused_result__));



extern size_t __fread_unlocked_chk_warn (void *__restrict __ptr, size_t __ptrlen, size_t __size, size_t __n, FILE *__restrict __stream) __asm__ ("" "__fread_unlocked_chk")




     __attribute__ ((__warn_unused_result__)) __attribute__((__warning__ ("fread_unlocked called with bigger size * nmemb than " "length of destination buffer")));


extern __inline __attribute__ ((__always_inline__)) __attribute__ ((__artificial__)) __attribute__ ((__warn_unused_result__)) size_t
fread_unlocked (void *__restrict __ptr, size_t __size, size_t __n,
  FILE *__restrict __stream)
{
  if (__builtin_object_size (__ptr, 0) != (size_t) -1)
    {
      if (!__builtin_constant_p (__size)
   || !__builtin_constant_p (__n)
   || (__size | __n) >= (((size_t) 1) << (8 * sizeof (size_t) / 2)))
 return __fread_unlocked_chk (__ptr, __builtin_object_size (__ptr, 0), __size, __n,
         __stream);

      if (__size * __n > __builtin_object_size (__ptr, 0))
 return __fread_unlocked_chk_warn (__ptr, __builtin_object_size (__ptr, 0), __size, __n,
       __stream);
    }


  if (__builtin_constant_p (__size)
      && __builtin_constant_p (__n)
      && (__size | __n) < (((size_t) 1) << (8 * sizeof (size_t) / 2))
      && __size * __n <= 8)
    {
      size_t __cnt = __size * __n;
      char *__cptr = (char *) __ptr;
      if (__cnt == 0)
 return 0;

      for (; __cnt > 0; --__cnt)
 {
   int __c = (__builtin_expect (((__stream)->_IO_read_ptr >= (__stream)->_IO_read_end), 0) ? __uflow (__stream) : *(unsigned char *) (__stream)->_IO_read_ptr++);
   if (__c == (-1))
     break;
   *__cptr++ = __c;
 }
      return (__cptr - (char *) __ptr) / __size;
    }

  return __fread_unlocked_alias (__ptr, __size, __n, __stream);
}
# 911 "/usr/include/stdio.h" 2 3 4






# 19 "../../../compat/stdio.h" 2
# 80 "../../../compat/compat.h" 2






# 1 "../../../compat/valgrind.h" 1
# 87 "../../../compat/compat.h" 2


# 1 "../../../include/squid_endian.h" 1
# 58 "../../../include/squid_endian.h"
# 1 "../../../include/config.h" 1
# 59 "../../../include/squid_endian.h" 2
# 1 "../../../include/squid_types.h" 1
# 60 "../../../include/squid_endian.h" 2
# 69 "../../../include/squid_endian.h"
# 1 "/usr/include/byteswap.h" 1 3 4
# 23 "/usr/include/byteswap.h" 3 4
# 1 "/usr/include/bits/byteswap.h" 1 3 4
# 24 "/usr/include/byteswap.h" 2 3 4
# 70 "../../../include/squid_endian.h" 2
# 90 "../../../compat/compat.h" 2






# 1 "../../../compat/GnuRegex.h" 1
# 19 "../../../compat/GnuRegex.h"
# 1 "/usr/include/regex.h" 1 3 4
# 26 "/usr/include/regex.h" 3 4
# 1 "/usr/include/gnu/option-groups.h" 1 3 4
# 27 "/usr/include/regex.h" 2 3 4
# 37 "/usr/include/regex.h" 3 4
typedef long int s_reg_t;
typedef unsigned long int active_reg_t;






typedef unsigned long int reg_syntax_t;
# 187 "/usr/include/regex.h" 3 4
extern reg_syntax_t re_syntax_options;
# 318 "/usr/include/regex.h" 3 4
typedef enum
{

  REG_ENOSYS = -1,


  REG_NOERROR = 0,
  REG_NOMATCH,



  REG_BADPAT,
  REG_ECOLLATE,
  REG_ECTYPE,
  REG_EESCAPE,
  REG_ESUBREG,
  REG_EBRACK,
  REG_EPAREN,
  REG_EBRACE,
  REG_BADBR,
  REG_ERANGE,
  REG_ESPACE,
  REG_BADRPT,


  REG_EEND,
  REG_ESIZE,
  REG_ERPAREN
} reg_errcode_t;
# 367 "/usr/include/regex.h" 3 4
struct re_pattern_buffer
{



  unsigned char *__buffer;


  unsigned long int __allocated;


  unsigned long int __used;


  reg_syntax_t __syntax;




  char *__fastmap;





  unsigned char * __translate;


  size_t re_nsub;





  unsigned __can_be_null : 1;
# 412 "/usr/include/regex.h" 3 4
  unsigned __regs_allocated : 2;



  unsigned __fastmap_accurate : 1;



  unsigned __no_sub : 1;



  unsigned __not_bol : 1;


  unsigned __not_eol : 1;


  unsigned __newline_anchor : 1;
};

typedef struct re_pattern_buffer regex_t;


typedef int regoff_t;
# 462 "/usr/include/regex.h" 3 4
typedef struct
{
  regoff_t rm_so;
  regoff_t rm_eo;
} regmatch_t;
# 567 "/usr/include/regex.h" 3 4
extern int regcomp (regex_t *__restrict __preg,
      const char *__restrict __pattern,
      int __cflags);

extern int regexec (const regex_t *__restrict __preg,
      const char *__restrict __string, size_t __nmatch,
      regmatch_t __pmatch[__restrict],
      int __eflags);

extern size_t regerror (int __errcode, const regex_t *__restrict __preg,
   char *__restrict __errbuf, size_t __errbuf_size);

extern void regfree (regex_t *__preg);
# 20 "../../../compat/GnuRegex.h" 2
# 97 "../../../compat/compat.h" 2
# 59 "../../../include/config.h" 2
# 179 "../../../include/config.h"
typedef long mtyp_t;
# 269 "../../../include/config.h"
# 1 "../../../include/strnstr.h" 1
# 14 "../../../include/strnstr.h"
const char * squid_strnstr(const char *s, const char *find, size_t slen);
# 270 "../../../include/config.h" 2
# 6 "../../../include/rfc1738.h" 2
# 36 "../../../include/rfc1738.h"
extern char *rfc1738_do_escape(const char *url, int flags);
# 88 "squid_ldap_auth_and_ip.c" 2
# 1 "../../../include/util.h" 1
# 43 "../../../include/util.h"
# 1 "/usr/include/time.h" 1 3 4
# 30 "/usr/include/time.h" 3 4

/*
 * Converts a ascii hex code into a binary character.
 */
static int
frommyhex(char ch)
{
    if (ch >= '0' && ch <= '9')
        return ch - '0';
    if (ch >= 'a' && ch <= 'f')
        return ch - 'a' + 10;
    if (ch >= 'A' && ch <= 'F')
        return ch - 'A' + 10;
    return -1;
}


/*
 *  rfc1738_unescape() - Converts escaped characters (%xy numbers) in
 *  given the string.  %% is a %. %ab is the 8-bit hexadecimal number "ab"
 */
void
rfc1738_unescape(char *s)
{
    int i, j;			/* i is write, j is read */
    for (i = j = 0; s[j]; i++, j++) {
        s[i] = s[j];
        if (s[j] != '%') {
            /* normal case, nothing more to do */
        } else if (s[j + 1] == '%') {	/* %% case */
            j++;		/* Skip % */
        } else {
            /* decode */
            int v1, v2, x;
            v1 = frommyhex(s[j + 1]);
            if (v1 < 0)
                continue;  /* non-hex or \0 */
            v2 = frommyhex(s[j + 2]);
            if (v2 < 0)
                continue;  /* non-hex or \0 */
            x = v1 << 4 | v2;
            if (x > 0 && x <= 255) {
                s[i] = x;
                j += 2;
            }
        }
    }
    s[i] = '\0';
}



# 1 "/usr/lib/gcc/i486-linux-gnu/4.4.5/include/stddef.h" 1 3 4
# 39 "/usr/include/time.h" 2 3 4



# 1 "/usr/include/bits/time.h" 1 3 4
# 43 "/usr/include/time.h" 2 3 4
# 58 "/usr/include/time.h" 3 4


typedef __clock_t clock_t;



# 131 "/usr/include/time.h" 3 4


struct tm
{
  int tm_sec;
  int tm_min;
  int tm_hour;
  int tm_mday;
  int tm_mon;
  int tm_year;
  int tm_wday;
  int tm_yday;
  int tm_isdst;


  long int tm_gmtoff;
  __const char *tm_zone;




};








struct itimerspec
  {
    struct timespec it_interval;
    struct timespec it_value;
  };


struct sigevent;
# 180 "/usr/include/time.h" 3 4



extern clock_t clock (void) __attribute__ ((__nothrow__));


extern time_t time (time_t *__timer) __attribute__ ((__nothrow__));


extern double difftime (time_t __time1, time_t __time0)
     __attribute__ ((__nothrow__)) __attribute__ ((__const__));


extern time_t mktime (struct tm *__tp) __attribute__ ((__nothrow__));





extern size_t strftime (char *__restrict __s, size_t __maxsize,
   __const char *__restrict __format,
   __const struct tm *__restrict __tp) __attribute__ ((__nothrow__));

# 215 "/usr/include/time.h" 3 4
# 1 "/usr/include/xlocale.h" 1 3 4
# 28 "/usr/include/xlocale.h" 3 4
typedef struct __locale_struct
{

  struct locale_data *__locales[13];


  const unsigned short int *__ctype_b;
  const int *__ctype_tolower;
  const int *__ctype_toupper;


  const char *__names[13];
} *__locale_t;


typedef __locale_t locale_t;
# 216 "/usr/include/time.h" 2 3 4

extern size_t strftime_l (char *__restrict __s, size_t __maxsize,
     __const char *__restrict __format,
     __const struct tm *__restrict __tp,
     __locale_t __loc) __attribute__ ((__nothrow__));
# 230 "/usr/include/time.h" 3 4



extern struct tm *gmtime (__const time_t *__timer) __attribute__ ((__nothrow__));



extern struct tm *localtime (__const time_t *__timer) __attribute__ ((__nothrow__));





extern struct tm *gmtime_r (__const time_t *__restrict __timer,
       struct tm *__restrict __tp) __attribute__ ((__nothrow__));



extern struct tm *localtime_r (__const time_t *__restrict __timer,
          struct tm *__restrict __tp) __attribute__ ((__nothrow__));





extern char *asctime (__const struct tm *__tp) __attribute__ ((__nothrow__));


extern char *ctime (__const time_t *__timer) __attribute__ ((__nothrow__));







extern char *asctime_r (__const struct tm *__restrict __tp,
   char *__restrict __buf) __attribute__ ((__nothrow__));


extern char *ctime_r (__const time_t *__restrict __timer,
        char *__restrict __buf) __attribute__ ((__nothrow__));




extern char *__tzname[2];
extern int __daylight;
extern long int __timezone;




extern char *tzname[2];



extern void tzset (void) __attribute__ ((__nothrow__));



extern int daylight;
extern long int timezone;





extern int stime (__const time_t *__when) __attribute__ ((__nothrow__));
# 313 "/usr/include/time.h" 3 4
extern time_t timegm (struct tm *__tp) __attribute__ ((__nothrow__));


extern time_t timelocal (struct tm *__tp) __attribute__ ((__nothrow__));


extern int dysize (int __year) __attribute__ ((__nothrow__)) __attribute__ ((__const__));
# 328 "/usr/include/time.h" 3 4
extern int nanosleep (__const struct timespec *__requested_time,
        struct timespec *__remaining);



extern int clock_getres (clockid_t __clock_id, struct timespec *__res) __attribute__ ((__nothrow__));


extern int clock_gettime (clockid_t __clock_id, struct timespec *__tp) __attribute__ ((__nothrow__));


extern int clock_settime (clockid_t __clock_id, __const struct timespec *__tp)
     __attribute__ ((__nothrow__));






extern int clock_nanosleep (clockid_t __clock_id, int __flags,
       __const struct timespec *__req,
       struct timespec *__rem);


extern int clock_getcpuclockid (pid_t __pid, clockid_t *__clock_id) __attribute__ ((__nothrow__));




extern int timer_create (clockid_t __clock_id,
    struct sigevent *__restrict __evp,
    timer_t *__restrict __timerid) __attribute__ ((__nothrow__));


extern int timer_delete (timer_t __timerid) __attribute__ ((__nothrow__));


extern int timer_settime (timer_t __timerid, int __flags,
     __const struct itimerspec *__restrict __value,
     struct itimerspec *__restrict __ovalue) __attribute__ ((__nothrow__));


extern int timer_gettime (timer_t __timerid, struct itimerspec *__value)
     __attribute__ ((__nothrow__));


extern int timer_getoverrun (timer_t __timerid) __attribute__ ((__nothrow__));
# 417 "/usr/include/time.h" 3 4

# 44 "../../../include/util.h" 2





# 1 "/usr/include/arpa/inet.h" 1 3 4
# 23 "/usr/include/arpa/inet.h" 3 4
# 1 "/usr/include/netinet/in.h" 1 3 4
# 25 "/usr/include/netinet/in.h" 3 4
# 1 "/usr/include/sys/socket.h" 1 3 4
# 26 "/usr/include/sys/socket.h" 3 4


# 1 "/usr/include/sys/uio.h" 1 3 4
# 26 "/usr/include/sys/uio.h" 3 4



# 1 "/usr/include/bits/uio.h" 1 3 4
# 44 "/usr/include/bits/uio.h" 3 4
struct iovec
  {
    void *iov_base;
    size_t iov_len;
  };
# 30 "/usr/include/sys/uio.h" 2 3 4
# 40 "/usr/include/sys/uio.h" 3 4
extern ssize_t readv (int __fd, __const struct iovec *__iovec, int __count)
  __attribute__ ((__warn_unused_result__));
# 51 "/usr/include/sys/uio.h" 3 4
extern ssize_t writev (int __fd, __const struct iovec *__iovec, int __count)
  __attribute__ ((__warn_unused_result__));
# 82 "/usr/include/sys/uio.h" 3 4
extern ssize_t preadv (int __fd, __const struct iovec *__iovec, int __count, __off64_t __offset) __asm__ ("" "preadv64") __attribute__ ((__warn_unused_result__));


extern ssize_t pwritev (int __fd, __const struct iovec *__iovec, int __count, __off64_t __offset) __asm__ ("" "pwritev64") __attribute__ ((__warn_unused_result__));
# 121 "/usr/include/sys/uio.h" 3 4

# 29 "/usr/include/sys/socket.h" 2 3 4

# 1 "/usr/lib/gcc/i486-linux-gnu/4.4.5/include/stddef.h" 1 3 4
# 31 "/usr/include/sys/socket.h" 2 3 4
# 40 "/usr/include/sys/socket.h" 3 4
# 1 "/usr/include/bits/socket.h" 1 3 4
# 29 "/usr/include/bits/socket.h" 3 4
# 1 "/usr/lib/gcc/i486-linux-gnu/4.4.5/include/stddef.h" 1 3 4
# 30 "/usr/include/bits/socket.h" 2 3 4





typedef __socklen_t socklen_t;




enum __socket_type
{
  SOCK_STREAM = 1,


  SOCK_DGRAM = 2,


  SOCK_RAW = 3,

  SOCK_RDM = 4,

  SOCK_SEQPACKET = 5,


  SOCK_DCCP = 6,

  SOCK_PACKET = 10,







  SOCK_CLOEXEC = 02000000,


  SOCK_NONBLOCK = 04000


};
# 175 "/usr/include/bits/socket.h" 3 4
# 1 "/usr/include/bits/sockaddr.h" 1 3 4
# 29 "/usr/include/bits/sockaddr.h" 3 4
typedef unsigned short int sa_family_t;
# 176 "/usr/include/bits/socket.h" 2 3 4


struct sockaddr
  {
    sa_family_t sa_family;
    char sa_data[14];
  };
# 191 "/usr/include/bits/socket.h" 3 4
struct sockaddr_storage
  {
    sa_family_t ss_family;
    unsigned long int __ss_align;
    char __ss_padding[(128 - (2 * sizeof (unsigned long int)))];
  };



enum
  {
    MSG_OOB = 0x01,

    MSG_PEEK = 0x02,

    MSG_DONTROUTE = 0x04,






    MSG_CTRUNC = 0x08,

    MSG_PROXY = 0x10,

    MSG_TRUNC = 0x20,

    MSG_DONTWAIT = 0x40,

    MSG_EOR = 0x80,

    MSG_WAITALL = 0x100,

    MSG_FIN = 0x200,

    MSG_SYN = 0x400,

    MSG_CONFIRM = 0x800,

    MSG_RST = 0x1000,

    MSG_ERRQUEUE = 0x2000,

    MSG_NOSIGNAL = 0x4000,

    MSG_MORE = 0x8000,


    MSG_CMSG_CLOEXEC = 0x40000000



  };




struct msghdr
  {
    void *msg_name;
    socklen_t msg_namelen;

    struct iovec *msg_iov;
    size_t msg_iovlen;

    void *msg_control;
    size_t msg_controllen;




    int msg_flags;
  };


struct cmsghdr
  {
    size_t cmsg_len;




    int cmsg_level;
    int cmsg_type;

    __extension__ unsigned char __cmsg_data [];

  };
# 297 "/usr/include/bits/socket.h" 3 4
extern struct cmsghdr *__cmsg_nxthdr (struct msghdr *__mhdr,
          struct cmsghdr *__cmsg) __attribute__ ((__nothrow__));




extern __inline struct cmsghdr *
__attribute__ ((__nothrow__)) __cmsg_nxthdr (struct msghdr *__mhdr, struct cmsghdr *__cmsg)
{
  if ((size_t) __cmsg->cmsg_len < sizeof (struct cmsghdr))

    return 0;

  __cmsg = (struct cmsghdr *) ((unsigned char *) __cmsg
          + (((__cmsg->cmsg_len) + sizeof (size_t) - 1) & (size_t) ~(sizeof (size_t) - 1)));
  if ((unsigned char *) (__cmsg + 1) > ((unsigned char *) __mhdr->msg_control
     + __mhdr->msg_controllen)
      || ((unsigned char *) __cmsg + (((__cmsg->cmsg_len) + sizeof (size_t) - 1) & (size_t) ~(sizeof (size_t) - 1))
   > ((unsigned char *) __mhdr->msg_control + __mhdr->msg_controllen)))

    return 0;
  return __cmsg;
}




enum
  {
    SCM_RIGHTS = 0x01





  };
# 370 "/usr/include/bits/socket.h" 3 4
# 1 "/usr/include/asm/socket.h" 1 3 4
# 1 "/usr/include/asm-generic/socket.h" 1 3 4



# 1 "/usr/include/asm/sockios.h" 1 3 4
# 1 "/usr/include/asm-generic/sockios.h" 1 3 4
# 1 "/usr/include/asm/sockios.h" 2 3 4
# 5 "/usr/include/asm-generic/socket.h" 2 3 4
# 1 "/usr/include/asm/socket.h" 2 3 4
# 371 "/usr/include/bits/socket.h" 2 3 4
# 404 "/usr/include/bits/socket.h" 3 4
struct linger
  {
    int l_onoff;
    int l_linger;
  };
# 41 "/usr/include/sys/socket.h" 2 3 4




struct osockaddr
  {
    unsigned short int sa_family;
    unsigned char sa_data[14];
  };




enum
{
  SHUT_RD = 0,

  SHUT_WR,

  SHUT_RDWR

};
# 105 "/usr/include/sys/socket.h" 3 4
extern int socket (int __domain, int __type, int __protocol) __attribute__ ((__nothrow__));





extern int socketpair (int __domain, int __type, int __protocol,
         int __fds[2]) __attribute__ ((__nothrow__));


extern int bind (int __fd, __const struct sockaddr * __addr, socklen_t __len)
     __attribute__ ((__nothrow__));


extern int getsockname (int __fd, struct sockaddr *__restrict __addr,
   socklen_t *__restrict __len) __attribute__ ((__nothrow__));
# 129 "/usr/include/sys/socket.h" 3 4
extern int connect (int __fd, __const struct sockaddr * __addr, socklen_t __len);



extern int getpeername (int __fd, struct sockaddr *__restrict __addr,
   socklen_t *__restrict __len) __attribute__ ((__nothrow__));






extern ssize_t send (int __fd, __const void *__buf, size_t __n, int __flags);






extern ssize_t recv (int __fd, void *__buf, size_t __n, int __flags);






extern ssize_t sendto (int __fd, __const void *__buf, size_t __n,
         int __flags, __const struct sockaddr * __addr,
         socklen_t __addr_len);
# 166 "/usr/include/sys/socket.h" 3 4
extern ssize_t recvfrom (int __fd, void *__restrict __buf, size_t __n,
    int __flags, struct sockaddr *__restrict __addr,
    socklen_t *__restrict __addr_len);







extern ssize_t sendmsg (int __fd, __const struct msghdr *__message,
   int __flags);






extern ssize_t recvmsg (int __fd, struct msghdr *__message, int __flags);





extern int getsockopt (int __fd, int __level, int __optname,
         void *__restrict __optval,
         socklen_t *__restrict __optlen) __attribute__ ((__nothrow__));




extern int setsockopt (int __fd, int __level, int __optname,
         __const void *__optval, socklen_t __optlen) __attribute__ ((__nothrow__));





extern int listen (int __fd, int __n) __attribute__ ((__nothrow__));
# 214 "/usr/include/sys/socket.h" 3 4
extern int accept (int __fd, struct sockaddr *__restrict __addr,
     socklen_t *__restrict __addr_len);
# 232 "/usr/include/sys/socket.h" 3 4
extern int shutdown (int __fd, int __how) __attribute__ ((__nothrow__));




extern int sockatmark (int __fd) __attribute__ ((__nothrow__));







extern int isfdtype (int __fd, int __fdtype) __attribute__ ((__nothrow__));





# 1 "/usr/include/bits/socket2.h" 1 3 4
# 24 "/usr/include/bits/socket2.h" 3 4
extern ssize_t __recv_chk (int __fd, void *__buf, size_t __n, size_t __buflen,
      int __flags);
extern ssize_t __recv_alias (int __fd, void *__buf, size_t __n, int __flags) __asm__ ("" "recv");

extern ssize_t __recv_chk_warn (int __fd, void *__buf, size_t __n, size_t __buflen, int __flags) __asm__ ("" "__recv_chk")


     __attribute__((__warning__ ("recv called with bigger length than size of destination " "buffer")));


extern __inline __attribute__ ((__always_inline__)) __attribute__ ((__artificial__)) ssize_t
recv (int __fd, void *__buf, size_t __n, int __flags)
{
  if (__builtin_object_size (__buf, 0) != (size_t) -1)
    {
      if (!__builtin_constant_p (__n))
 return __recv_chk (__fd, __buf, __n, __builtin_object_size (__buf, 0), __flags);

      if (__n > __builtin_object_size (__buf, 0))
 return __recv_chk_warn (__fd, __buf, __n, __builtin_object_size (__buf, 0), __flags);
    }
  return __recv_alias (__fd, __buf, __n, __flags);
}

extern ssize_t __recvfrom_chk (int __fd, void *__restrict __buf, size_t __n,
          size_t __buflen, int __flags,
          struct sockaddr *__restrict __addr,
          socklen_t *__restrict __addr_len);
extern ssize_t __recvfrom_alias (int __fd, void *__restrict __buf, size_t __n, int __flags, struct sockaddr *__restrict __addr, socklen_t *__restrict __addr_len) __asm__ ("" "recvfrom");



extern ssize_t __recvfrom_chk_warn (int __fd, void *__restrict __buf, size_t __n, size_t __buflen, int __flags, struct sockaddr *__restrict __addr, socklen_t *__restrict __addr_len) __asm__ ("" "__recvfrom_chk")




     __attribute__((__warning__ ("recvfrom called with bigger length than size of " "destination buffer")));


extern __inline __attribute__ ((__always_inline__)) __attribute__ ((__artificial__)) ssize_t
recvfrom (int __fd, void *__restrict __buf, size_t __n, int __flags,
   struct sockaddr *__restrict __addr, socklen_t *__restrict __addr_len)
{
  if (__builtin_object_size (__buf, 0) != (size_t) -1)
    {
      if (!__builtin_constant_p (__n))
 return __recvfrom_chk (__fd, __buf, __n, __builtin_object_size (__buf, 0), __flags,
          __addr, __addr_len);
      if (__n > __builtin_object_size (__buf, 0))
 return __recvfrom_chk_warn (__fd, __buf, __n, __builtin_object_size (__buf, 0), __flags,
        __addr, __addr_len);
    }
  return __recvfrom_alias (__fd, __buf, __n, __flags, __addr, __addr_len);
}
# 252 "/usr/include/sys/socket.h" 2 3 4



# 26 "/usr/include/netinet/in.h" 2 3 4






enum
  {
    IPPROTO_IP = 0,

    IPPROTO_HOPOPTS = 0,

    IPPROTO_ICMP = 1,

    IPPROTO_IGMP = 2,

    IPPROTO_IPIP = 4,

    IPPROTO_TCP = 6,

    IPPROTO_EGP = 8,

    IPPROTO_PUP = 12,

    IPPROTO_UDP = 17,

    IPPROTO_IDP = 22,

    IPPROTO_TP = 29,

    IPPROTO_DCCP = 33,

    IPPROTO_IPV6 = 41,

    IPPROTO_ROUTING = 43,

    IPPROTO_FRAGMENT = 44,

    IPPROTO_RSVP = 46,

    IPPROTO_GRE = 47,

    IPPROTO_ESP = 50,

    IPPROTO_AH = 51,

    IPPROTO_ICMPV6 = 58,

    IPPROTO_NONE = 59,

    IPPROTO_DSTOPTS = 60,

    IPPROTO_MTP = 92,

    IPPROTO_ENCAP = 98,

    IPPROTO_PIM = 103,

    IPPROTO_COMP = 108,

    IPPROTO_SCTP = 132,

    IPPROTO_UDPLITE = 136,

    IPPROTO_RAW = 255,

    IPPROTO_MAX
  };



typedef uint16_t in_port_t;


enum
  {
    IPPORT_ECHO = 7,
    IPPORT_DISCARD = 9,
    IPPORT_SYSTAT = 11,
    IPPORT_DAYTIME = 13,
    IPPORT_NETSTAT = 15,
    IPPORT_FTP = 21,
    IPPORT_TELNET = 23,
    IPPORT_SMTP = 25,
    IPPORT_TIMESERVER = 37,
    IPPORT_NAMESERVER = 42,
    IPPORT_WHOIS = 43,
    IPPORT_MTP = 57,

    IPPORT_TFTP = 69,
    IPPORT_RJE = 77,
    IPPORT_FINGER = 79,
    IPPORT_TTYLINK = 87,
    IPPORT_SUPDUP = 95,


    IPPORT_EXECSERVER = 512,
    IPPORT_LOGINSERVER = 513,
    IPPORT_CMDSERVER = 514,
    IPPORT_EFSSERVER = 520,


    IPPORT_BIFFUDP = 512,
    IPPORT_WHOSERVER = 513,
    IPPORT_ROUTESERVER = 520,


    IPPORT_RESERVED = 1024,


    IPPORT_USERRESERVED = 5000
  };



typedef uint32_t in_addr_t;
struct in_addr
  {
    in_addr_t s_addr;
  };
# 198 "/usr/include/netinet/in.h" 3 4
struct in6_addr
  {
    union
      {
 uint8_t __u6_addr8[16];

 uint16_t __u6_addr16[8];
 uint32_t __u6_addr32[4];

      } __in6_u;





  };

extern const struct in6_addr in6addr_any;
extern const struct in6_addr in6addr_loopback;
# 225 "/usr/include/netinet/in.h" 3 4
struct sockaddr_in
  {
    sa_family_t sin_family;
    in_port_t sin_port;
    struct in_addr sin_addr;


    unsigned char sin_zero[sizeof (struct sockaddr) -
      (sizeof (unsigned short int)) -
      sizeof (in_port_t) -
      sizeof (struct in_addr)];
  };


struct sockaddr_in6
  {
    sa_family_t sin6_family;
    in_port_t sin6_port;
    uint32_t sin6_flowinfo;
    struct in6_addr sin6_addr;
    uint32_t sin6_scope_id;
  };




struct ip_mreq
  {

    struct in_addr imr_multiaddr;


    struct in_addr imr_interface;
  };

struct ip_mreq_source
  {

    struct in_addr imr_multiaddr;


    struct in_addr imr_interface;


    struct in_addr imr_sourceaddr;
  };




struct ipv6_mreq
  {

    struct in6_addr ipv6mr_multiaddr;


    unsigned int ipv6mr_interface;
  };




struct group_req
  {

    uint32_t gr_interface;


    struct sockaddr_storage gr_group;
  };

struct group_source_req
  {

    uint32_t gsr_interface;


    struct sockaddr_storage gsr_group;


    struct sockaddr_storage gsr_source;
  };



struct ip_msfilter
  {

    struct in_addr imsf_multiaddr;


    struct in_addr imsf_interface;


    uint32_t imsf_fmode;


    uint32_t imsf_numsrc;

    struct in_addr imsf_slist[1];
  };





struct group_filter
  {

    uint32_t gf_interface;


    struct sockaddr_storage gf_group;


    uint32_t gf_fmode;


    uint32_t gf_numsrc;

    struct sockaddr_storage gf_slist[1];
};
# 356 "/usr/include/netinet/in.h" 3 4
# 1 "/usr/include/bits/in.h" 1 3 4
# 86 "/usr/include/bits/in.h" 3 4
struct ip_opts
  {
    struct in_addr ip_dst;
    char ip_opts[40];
  };


struct ip_mreqn
  {
    struct in_addr imr_multiaddr;
    struct in_addr imr_address;
    int imr_ifindex;
  };


struct in_pktinfo
  {
    int ipi_ifindex;
    struct in_addr ipi_spec_dst;
    struct in_addr ipi_addr;
  };
# 357 "/usr/include/netinet/in.h" 2 3 4
# 365 "/usr/include/netinet/in.h" 3 4
extern uint32_t ntohl (uint32_t __netlong) __attribute__ ((__nothrow__)) __attribute__ ((__const__));
extern uint16_t ntohs (uint16_t __netshort)
     __attribute__ ((__nothrow__)) __attribute__ ((__const__));
extern uint32_t htonl (uint32_t __hostlong)
     __attribute__ ((__nothrow__)) __attribute__ ((__const__));
extern uint16_t htons (uint16_t __hostshort)
     __attribute__ ((__nothrow__)) __attribute__ ((__const__));




# 1 "/usr/include/bits/byteswap.h" 1 3 4
# 377 "/usr/include/netinet/in.h" 2 3 4
# 440 "/usr/include/netinet/in.h" 3 4
extern int bindresvport (int __sockfd, struct sockaddr_in *__sock_in) __attribute__ ((__nothrow__));


extern int bindresvport6 (int __sockfd, struct sockaddr_in6 *__sock_in)
     __attribute__ ((__nothrow__));
# 565 "/usr/include/netinet/in.h" 3 4

# 24 "/usr/include/arpa/inet.h" 2 3 4











extern in_addr_t inet_addr (__const char *__cp) __attribute__ ((__nothrow__));


extern in_addr_t inet_lnaof (struct in_addr __in) __attribute__ ((__nothrow__));



extern struct in_addr inet_makeaddr (in_addr_t __net, in_addr_t __host)
     __attribute__ ((__nothrow__));


extern in_addr_t inet_netof (struct in_addr __in) __attribute__ ((__nothrow__));



extern in_addr_t inet_network (__const char *__cp) __attribute__ ((__nothrow__));



extern char *inet_ntoa (struct in_addr __in) __attribute__ ((__nothrow__));




extern int inet_pton (int __af, __const char *__restrict __cp,
        void *__restrict __buf) __attribute__ ((__nothrow__));




extern __const char *inet_ntop (int __af, __const void *__restrict __cp,
    char *__restrict __buf, socklen_t __len)
     __attribute__ ((__nothrow__));






extern int inet_aton (__const char *__cp, struct in_addr *__inp) __attribute__ ((__nothrow__));



extern char *inet_neta (in_addr_t __net, char *__buf, size_t __len) __attribute__ ((__nothrow__));




extern char *inet_net_ntop (int __af, __const void *__cp, int __bits,
       char *__buf, size_t __len) __attribute__ ((__nothrow__));




extern int inet_net_pton (int __af, __const char *__cp,
     void *__buf, size_t __len) __attribute__ ((__nothrow__));




extern unsigned int inet_nsap_addr (__const char *__cp,
        unsigned char *__buf, int __len) __attribute__ ((__nothrow__));



extern char *inet_nsap_ntoa (int __len, __const unsigned char *__cp,
        char *__buf) __attribute__ ((__nothrow__));



# 50 "../../../include/util.h" 2






extern const char *mkhttpdlogtime(const time_t *);
extern const char *mkrfc1123(time_t);
extern char *uudecode(const char *);
extern char *xstrdup(const char *);
extern char *xstrndup(const char *, size_t);
extern const char *xstrerr(int xerrno);
extern const char *xstrerror(void);
extern int tvSubMsec(struct timeval, struct timeval);
extern int tvSubUsec(struct timeval, struct timeval);
extern double tvSubDsec(struct timeval, struct timeval);
extern char *xstrncpy(char *, const char *, size_t);
extern size_t xcountws(const char *str);
extern time_t parse_rfc1123(const char *str);
extern void *xcalloc(size_t, size_t);
extern void *xmalloc(size_t);
extern void *xrealloc(void *, size_t);
extern void Tolower(char *);
extern void xfree(void *);
extern void xxfree(const void *);
# 94 "../../../include/util.h"
extern char *latin1_to_utf8(char *out, size_t size, const char *in);


extern char *html_quote(const char *);
# 118 "../../../include/util.h"
extern time_t parse_iso3307_time(const char *buf);
extern char *base64_decode(const char *coded);
extern const char *base64_encode(const char *decoded);
extern const char *base64_encode_bin(const char *data, int len);

extern double xpercent(double part, double whole);
extern int xpercentInt(double part, double whole);
extern double xdiv(double nom, double denom);

extern const char *xitoa(int num);
extern const char *xint64toa(int64_t num);





typedef struct {
    size_t count;
    size_t bytes;
    size_t gb;
} gb_t;






extern double gb_to_double(const gb_t *);
extern const char *double_to_str(char *buf, int buf_size, double value);
extern const char *gb_to_str(const gb_t *);
extern void gb_flush(gb_t *);




double statMemoryAccounted(void);
# 89 "squid_ldap_auth_and_ip.c" 2


# 1 "/usr/include/string.h" 1 3 4
# 28 "/usr/include/string.h" 3 4





# 1 "/usr/lib/gcc/i486-linux-gnu/4.4.5/include/stddef.h" 1 3 4
# 34 "/usr/include/string.h" 2 3 4









extern void *memcpy (void *__restrict __dest,
       __const void *__restrict __src, size_t __n)
     __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1, 2)));


extern void *memmove (void *__dest, __const void *__src, size_t __n)
     __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1, 2)));






extern void *memccpy (void *__restrict __dest, __const void *__restrict __src,
        int __c, size_t __n)
     __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1, 2)));





extern void *memset (void *__s, int __c, size_t __n) __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1)));


extern int memcmp (__const void *__s1, __const void *__s2, size_t __n)
     __attribute__ ((__nothrow__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1, 2)));
# 94 "/usr/include/string.h" 3 4
extern void *memchr (__const void *__s, int __c, size_t __n)
      __attribute__ ((__nothrow__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1)));


# 125 "/usr/include/string.h" 3 4


extern char *strcpy (char *__restrict __dest, __const char *__restrict __src)
     __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1, 2)));

extern char *strncpy (char *__restrict __dest,
        __const char *__restrict __src, size_t __n)
     __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1, 2)));


extern char *strcat (char *__restrict __dest, __const char *__restrict __src)
     __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1, 2)));

extern char *strncat (char *__restrict __dest, __const char *__restrict __src,
        size_t __n) __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1, 2)));


extern int strcmp (__const char *__s1, __const char *__s2)
     __attribute__ ((__nothrow__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1, 2)));

extern int strncmp (__const char *__s1, __const char *__s2, size_t __n)
     __attribute__ ((__nothrow__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1, 2)));


extern int strcoll (__const char *__s1, __const char *__s2)
     __attribute__ ((__nothrow__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1, 2)));

extern size_t strxfrm (char *__restrict __dest,
         __const char *__restrict __src, size_t __n)
     __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (2)));

# 164 "/usr/include/string.h" 3 4
extern int strcoll_l (__const char *__s1, __const char *__s2, __locale_t __l)
     __attribute__ ((__nothrow__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1, 2, 3)));

extern size_t strxfrm_l (char *__dest, __const char *__src, size_t __n,
    __locale_t __l) __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (2, 4)));




extern char *strdup (__const char *__s)
     __attribute__ ((__nothrow__)) __attribute__ ((__malloc__)) __attribute__ ((__nonnull__ (1)));






extern char *strndup (__const char *__string, size_t __n)
     __attribute__ ((__nothrow__)) __attribute__ ((__malloc__)) __attribute__ ((__nonnull__ (1)));
# 208 "/usr/include/string.h" 3 4

# 233 "/usr/include/string.h" 3 4
extern char *strchr (__const char *__s, int __c)
     __attribute__ ((__nothrow__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1)));
# 260 "/usr/include/string.h" 3 4
extern char *strrchr (__const char *__s, int __c)
     __attribute__ ((__nothrow__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1)));


# 279 "/usr/include/string.h" 3 4



extern size_t strcspn (__const char *__s, __const char *__reject)
     __attribute__ ((__nothrow__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1, 2)));


extern size_t strspn (__const char *__s, __const char *__accept)
     __attribute__ ((__nothrow__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1, 2)));
# 312 "/usr/include/string.h" 3 4
extern char *strpbrk (__const char *__s, __const char *__accept)
     __attribute__ ((__nothrow__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1, 2)));
# 340 "/usr/include/string.h" 3 4
extern char *strstr (__const char *__haystack, __const char *__needle)
     __attribute__ ((__nothrow__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1, 2)));




extern char *strtok (char *__restrict __s, __const char *__restrict __delim)
     __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (2)));




extern char *__strtok_r (char *__restrict __s,
    __const char *__restrict __delim,
    char **__restrict __save_ptr)
     __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (2, 3)));

extern char *strtok_r (char *__restrict __s, __const char *__restrict __delim,
         char **__restrict __save_ptr)
     __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (2, 3)));
# 395 "/usr/include/string.h" 3 4


extern size_t strlen (__const char *__s)
     __attribute__ ((__nothrow__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1)));





extern size_t strnlen (__const char *__string, size_t __maxlen)
     __attribute__ ((__nothrow__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1)));





extern char *strerror (int __errnum) __attribute__ ((__nothrow__));

# 425 "/usr/include/string.h" 3 4
extern int strerror_r (int __errnum, char *__buf, size_t __buflen) __asm__ ("" "__xpg_strerror_r") __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (2)));
# 443 "/usr/include/string.h" 3 4
extern char *strerror_l (int __errnum, __locale_t __l) __attribute__ ((__nothrow__));





extern void __bzero (void *__s, size_t __n) __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1)));



extern void bcopy (__const void *__src, void *__dest, size_t __n)
     __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1, 2)));


extern void bzero (void *__s, size_t __n) __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1)));


extern int bcmp (__const void *__s1, __const void *__s2, size_t __n)
     __attribute__ ((__nothrow__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1, 2)));
# 487 "/usr/include/string.h" 3 4
extern char *index (__const char *__s, int __c)
     __attribute__ ((__nothrow__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1)));
# 515 "/usr/include/string.h" 3 4
extern char *rindex (__const char *__s, int __c)
     __attribute__ ((__nothrow__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1)));




extern int ffs (int __i) __attribute__ ((__nothrow__)) __attribute__ ((__const__));
# 534 "/usr/include/string.h" 3 4
extern int strcasecmp (__const char *__s1, __const char *__s2)
     __attribute__ ((__nothrow__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1, 2)));


extern int strncasecmp (__const char *__s1, __const char *__s2, size_t __n)
     __attribute__ ((__nothrow__)) __attribute__ ((__pure__)) __attribute__ ((__nonnull__ (1, 2)));
# 557 "/usr/include/string.h" 3 4
extern char *strsep (char **__restrict __stringp,
       __const char *__restrict __delim)
     __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1, 2)));




extern char *strsignal (int __sig) __attribute__ ((__nothrow__));


extern char *__stpcpy (char *__restrict __dest, __const char *__restrict __src)
     __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1, 2)));
extern char *stpcpy (char *__restrict __dest, __const char *__restrict __src)
     __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1, 2)));



extern char *__stpncpy (char *__restrict __dest,
   __const char *__restrict __src, size_t __n)
     __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1, 2)));
extern char *stpncpy (char *__restrict __dest,
        __const char *__restrict __src, size_t __n)
     __attribute__ ((__nothrow__)) __attribute__ ((__nonnull__ (1, 2)));
# 632 "/usr/include/string.h" 3 4
# 1 "/usr/include/bits/string.h" 1 3 4
# 633 "/usr/include/string.h" 2 3 4


# 1 "/usr/include/bits/string2.h" 1 3 4
# 394 "/usr/include/bits/string2.h" 3 4
extern void *__rawmemchr (const void *__s, int __c);
# 969 "/usr/include/bits/string2.h" 3 4
extern __inline size_t __strcspn_c1 (__const char *__s, int __reject);
extern __inline size_t
__strcspn_c1 (__const char *__s, int __reject)
{
  register size_t __result = 0;
  while (__s[__result] != '\0' && __s[__result] != __reject)
    ++__result;
  return __result;
}

extern __inline size_t __strcspn_c2 (__const char *__s, int __reject1,
         int __reject2);
extern __inline size_t
__strcspn_c2 (__const char *__s, int __reject1, int __reject2)
{
  register size_t __result = 0;
  while (__s[__result] != '\0' && __s[__result] != __reject1
  && __s[__result] != __reject2)
    ++__result;
  return __result;
}

extern __inline size_t __strcspn_c3 (__const char *__s, int __reject1,
         int __reject2, int __reject3);
extern __inline size_t
__strcspn_c3 (__const char *__s, int __reject1, int __reject2,
       int __reject3)
{
  register size_t __result = 0;
  while (__s[__result] != '\0' && __s[__result] != __reject1
  && __s[__result] != __reject2 && __s[__result] != __reject3)
    ++__result;
  return __result;
}
# 1045 "/usr/include/bits/string2.h" 3 4
extern __inline size_t __strspn_c1 (__const char *__s, int __accept);
extern __inline size_t
__strspn_c1 (__const char *__s, int __accept)
{
  register size_t __result = 0;

  while (__s[__result] == __accept)
    ++__result;
  return __result;
}

extern __inline size_t __strspn_c2 (__const char *__s, int __accept1,
        int __accept2);
extern __inline size_t
__strspn_c2 (__const char *__s, int __accept1, int __accept2)
{
  register size_t __result = 0;

  while (__s[__result] == __accept1 || __s[__result] == __accept2)
    ++__result;
  return __result;
}

extern __inline size_t __strspn_c3 (__const char *__s, int __accept1,
        int __accept2, int __accept3);
extern __inline size_t
__strspn_c3 (__const char *__s, int __accept1, int __accept2, int __accept3)
{
  register size_t __result = 0;

  while (__s[__result] == __accept1 || __s[__result] == __accept2
  || __s[__result] == __accept3)
    ++__result;
  return __result;
}
# 1121 "/usr/include/bits/string2.h" 3 4
extern __inline char *__strpbrk_c2 (__const char *__s, int __accept1,
         int __accept2);
extern __inline char *
__strpbrk_c2 (__const char *__s, int __accept1, int __accept2)
{

  while (*__s != '\0' && *__s != __accept1 && *__s != __accept2)
    ++__s;
  return *__s == '\0' ? ((void *)0) : (char *) (size_t) __s;
}

extern __inline char *__strpbrk_c3 (__const char *__s, int __accept1,
         int __accept2, int __accept3);
extern __inline char *
__strpbrk_c3 (__const char *__s, int __accept1, int __accept2,
       int __accept3)
{

  while (*__s != '\0' && *__s != __accept1 && *__s != __accept2
  && *__s != __accept3)
    ++__s;
  return *__s == '\0' ? ((void *)0) : (char *) (size_t) __s;
}
# 1172 "/usr/include/bits/string2.h" 3 4
extern __inline char *__strtok_r_1c (char *__s, char __sep, char **__nextp);
extern __inline char *
__strtok_r_1c (char *__s, char __sep, char **__nextp)
{
  char *__result;
  if (__s == ((void *)0))
    __s = *__nextp;
  while (*__s == __sep)
    ++__s;
  __result = ((void *)0);
  if (*__s != '\0')
    {
      __result = __s++;
      while (*__s != '\0')
 if (*__s++ == __sep)
   {
     __s[-1] = '\0';
     break;
   }
    }
  *__nextp = __s;
  return __result;
}
# 1204 "/usr/include/bits/string2.h" 3 4
extern char *__strsep_g (char **__stringp, __const char *__delim);
# 1222 "/usr/include/bits/string2.h" 3 4
extern __inline char *__strsep_1c (char **__s, char __reject);
extern __inline char *
__strsep_1c (char **__s, char __reject)
{
  register char *__retval = *__s;
  if (__retval != ((void *)0) && (*__s = (__extension__ (__builtin_constant_p (__reject) && !__builtin_constant_p (__retval) && (__reject) == '\0' ? (char *) __rawmemchr (__retval, __reject) : __builtin_strchr (__retval, __reject)))) != ((void *)0))
    *(*__s)++ = '\0';
  return __retval;
}

extern __inline char *__strsep_2c (char **__s, char __reject1, char __reject2);
extern __inline char *
__strsep_2c (char **__s, char __reject1, char __reject2)
{
  register char *__retval = *__s;
  if (__retval != ((void *)0))
    {
      register char *__cp = __retval;
      while (1)
 {
   if (*__cp == '\0')
     {
       __cp = ((void *)0);
   break;
     }
   if (*__cp == __reject1 || *__cp == __reject2)
     {
       *__cp++ = '\0';
       break;
     }
   ++__cp;
 }
      *__s = __cp;
    }
  return __retval;
}

extern __inline char *__strsep_3c (char **__s, char __reject1, char __reject2,
       char __reject3);
extern __inline char *
__strsep_3c (char **__s, char __reject1, char __reject2, char __reject3)
{
  register char *__retval = *__s;
  if (__retval != ((void *)0))
    {
      register char *__cp = __retval;
      while (1)
 {
   if (*__cp == '\0')
     {
       __cp = ((void *)0);
   break;
     }
   if (*__cp == __reject1 || *__cp == __reject2 || *__cp == __reject3)
     {
       *__cp++ = '\0';
       break;
     }
   ++__cp;
 }
      *__s = __cp;
    }
  return __retval;
}
# 1303 "/usr/include/bits/string2.h" 3 4
extern char *__strdup (__const char *__string) __attribute__ ((__nothrow__)) __attribute__ ((__malloc__));
# 1322 "/usr/include/bits/string2.h" 3 4
extern char *__strndup (__const char *__string, size_t __n)
     __attribute__ ((__nothrow__)) __attribute__ ((__malloc__));
# 636 "/usr/include/string.h" 2 3 4




# 1 "/usr/include/bits/string3.h" 1 3 4
# 23 "/usr/include/bits/string3.h" 3 4
extern void __warn_memset_zero_len (void) __attribute__((__warning__ ("memset used with constant zero length parameter; this could be due to transposed parameters")));
# 48 "/usr/include/bits/string3.h" 3 4
extern __inline __attribute__ ((__always_inline__)) __attribute__ ((__artificial__)) void *
__attribute__ ((__nothrow__)) memcpy (void *__restrict __dest, __const void *__restrict __src, size_t __len)

{
  return __builtin___memcpy_chk (__dest, __src, __len, __builtin_object_size (__dest, 0));
}

extern __inline __attribute__ ((__always_inline__)) __attribute__ ((__artificial__)) void *
__attribute__ ((__nothrow__)) memmove (void *__dest, __const void *__src, size_t __len)
{
  return __builtin___memmove_chk (__dest, __src, __len, __builtin_object_size (__dest, 0));
}
# 76 "/usr/include/bits/string3.h" 3 4
extern __inline __attribute__ ((__always_inline__)) __attribute__ ((__artificial__)) void *
__attribute__ ((__nothrow__)) memset (void *__dest, int __ch, size_t __len)
{
  if (__builtin_constant_p (__len) && __len == 0
      && (!__builtin_constant_p (__ch) || __ch != 0))
    {
      __warn_memset_zero_len ();
      return __dest;
    }
  return __builtin___memset_chk (__dest, __ch, __len, __builtin_object_size (__dest, 0));
}


extern __inline __attribute__ ((__always_inline__)) __attribute__ ((__artificial__)) void
__attribute__ ((__nothrow__)) bcopy (__const void *__src, void *__dest, size_t __len)
{
  (void) __builtin___memmove_chk (__dest, __src, __len, __builtin_object_size (__dest, 0));
}

extern __inline __attribute__ ((__always_inline__)) __attribute__ ((__artificial__)) void
__attribute__ ((__nothrow__)) bzero (void *__dest, size_t __len)
{
  (void) __builtin___memset_chk (__dest, '\0', __len, __builtin_object_size (__dest, 0));
}


extern __inline __attribute__ ((__always_inline__)) __attribute__ ((__artificial__)) char *
__attribute__ ((__nothrow__)) strcpy (char *__restrict __dest, __const char *__restrict __src)
{
  return __builtin___strcpy_chk (__dest, __src, __builtin_object_size (__dest, 2 > 1));
}
# 117 "/usr/include/bits/string3.h" 3 4
extern __inline __attribute__ ((__always_inline__)) __attribute__ ((__artificial__)) char *
__attribute__ ((__nothrow__)) strncpy (char *__restrict __dest, __const char *__restrict __src, size_t __len)

{
  return __builtin___strncpy_chk (__dest, __src, __len, __builtin_object_size (__dest, 2 > 1));
}


extern char *__stpncpy_chk (char *__dest, __const char *__src, size_t __n,
       size_t __destlen) __attribute__ ((__nothrow__));
extern char *__stpncpy_alias (char *__dest, __const char *__src, size_t __n) __asm__ ("" "stpncpy") __attribute__ ((__nothrow__));



extern __inline __attribute__ ((__always_inline__)) __attribute__ ((__artificial__)) char *
__attribute__ ((__nothrow__)) stpncpy (char *__dest, __const char *__src, size_t __n)
{
  if (__builtin_object_size (__dest, 2 > 1) != (size_t) -1
      && (!__builtin_constant_p (__n) || __n <= __builtin_object_size (__dest, 2 > 1)))
    return __stpncpy_chk (__dest, __src, __n, __builtin_object_size (__dest, 2 > 1));
  return __stpncpy_alias (__dest, __src, __n);
}


extern __inline __attribute__ ((__always_inline__)) __attribute__ ((__artificial__)) char *
__attribute__ ((__nothrow__)) strcat (char *__restrict __dest, __const char *__restrict __src)
{
  return __builtin___strcat_chk (__dest, __src, __builtin_object_size (__dest, 2 > 1));
}


extern __inline __attribute__ ((__always_inline__)) __attribute__ ((__artificial__)) char *
__attribute__ ((__nothrow__)) strncat (char *__restrict __dest, __const char *__restrict __src, size_t __len)

{
  return __builtin___strncat_chk (__dest, __src, __len, __builtin_object_size (__dest, 2 > 1));
}
# 641 "/usr/include/string.h" 2 3 4




# 92 "squid_ldap_auth_and_ip.c" 2

# 1 "/usr/include/ctype.h" 1 3 4
# 30 "/usr/include/ctype.h" 3 4

# 48 "/usr/include/ctype.h" 3 4
enum
{
  _ISupper = ((0) < 8 ? ((1 << (0)) << 8) : ((1 << (0)) >> 8)),
  _ISlower = ((1) < 8 ? ((1 << (1)) << 8) : ((1 << (1)) >> 8)),
  _ISalpha = ((2) < 8 ? ((1 << (2)) << 8) : ((1 << (2)) >> 8)),
  _ISdigit = ((3) < 8 ? ((1 << (3)) << 8) : ((1 << (3)) >> 8)),
  _ISxdigit = ((4) < 8 ? ((1 << (4)) << 8) : ((1 << (4)) >> 8)),
  _ISspace = ((5) < 8 ? ((1 << (5)) << 8) : ((1 << (5)) >> 8)),
  _ISprint = ((6) < 8 ? ((1 << (6)) << 8) : ((1 << (6)) >> 8)),
  _ISgraph = ((7) < 8 ? ((1 << (7)) << 8) : ((1 << (7)) >> 8)),
  _ISblank = ((8) < 8 ? ((1 << (8)) << 8) : ((1 << (8)) >> 8)),
  _IScntrl = ((9) < 8 ? ((1 << (9)) << 8) : ((1 << (9)) >> 8)),
  _ISpunct = ((10) < 8 ? ((1 << (10)) << 8) : ((1 << (10)) >> 8)),
  _ISalnum = ((11) < 8 ? ((1 << (11)) << 8) : ((1 << (11)) >> 8))
};
# 81 "/usr/include/ctype.h" 3 4
extern __const unsigned short int **__ctype_b_loc (void)
     __attribute__ ((__nothrow__)) __attribute__ ((__const));
extern __const __int32_t **__ctype_tolower_loc (void)
     __attribute__ ((__nothrow__)) __attribute__ ((__const));
extern __const __int32_t **__ctype_toupper_loc (void)
     __attribute__ ((__nothrow__)) __attribute__ ((__const));
# 96 "/usr/include/ctype.h" 3 4






extern int isalnum (int) __attribute__ ((__nothrow__));
extern int isalpha (int) __attribute__ ((__nothrow__));
extern int iscntrl (int) __attribute__ ((__nothrow__));
extern int isdigit (int) __attribute__ ((__nothrow__));
extern int islower (int) __attribute__ ((__nothrow__));
extern int isgraph (int) __attribute__ ((__nothrow__));
extern int isprint (int) __attribute__ ((__nothrow__));
extern int ispunct (int) __attribute__ ((__nothrow__));
extern int isspace (int) __attribute__ ((__nothrow__));
extern int isupper (int) __attribute__ ((__nothrow__));
extern int isxdigit (int) __attribute__ ((__nothrow__));



extern int tolower (int __c) __attribute__ ((__nothrow__));


extern int toupper (int __c) __attribute__ ((__nothrow__));








extern int isblank (int) __attribute__ ((__nothrow__));


# 142 "/usr/include/ctype.h" 3 4
extern int isascii (int __c) __attribute__ ((__nothrow__));



extern int toascii (int __c) __attribute__ ((__nothrow__));



extern int _toupper (int) __attribute__ ((__nothrow__));
extern int _tolower (int) __attribute__ ((__nothrow__));
# 190 "/usr/include/ctype.h" 3 4
extern __inline int
__attribute__ ((__nothrow__)) tolower (int __c)
{
  return __c >= -128 && __c < 256 ? (*__ctype_tolower_loc ())[__c] : __c;
}

extern __inline int
__attribute__ ((__nothrow__)) toupper (int __c)
{
  return __c >= -128 && __c < 256 ? (*__ctype_toupper_loc ())[__c] : __c;
}
# 247 "/usr/include/ctype.h" 3 4
extern int isalnum_l (int, __locale_t) __attribute__ ((__nothrow__));
extern int isalpha_l (int, __locale_t) __attribute__ ((__nothrow__));
extern int iscntrl_l (int, __locale_t) __attribute__ ((__nothrow__));
extern int isdigit_l (int, __locale_t) __attribute__ ((__nothrow__));
extern int islower_l (int, __locale_t) __attribute__ ((__nothrow__));
extern int isgraph_l (int, __locale_t) __attribute__ ((__nothrow__));
extern int isprint_l (int, __locale_t) __attribute__ ((__nothrow__));
extern int ispunct_l (int, __locale_t) __attribute__ ((__nothrow__));
extern int isspace_l (int, __locale_t) __attribute__ ((__nothrow__));
extern int isupper_l (int, __locale_t) __attribute__ ((__nothrow__));
extern int isxdigit_l (int, __locale_t) __attribute__ ((__nothrow__));

extern int isblank_l (int, __locale_t) __attribute__ ((__nothrow__));



extern int __tolower_l (int __c, __locale_t __l) __attribute__ ((__nothrow__));
extern int tolower_l (int __c, __locale_t __l) __attribute__ ((__nothrow__));


extern int __toupper_l (int __c, __locale_t __l) __attribute__ ((__nothrow__));
extern int toupper_l (int __c, __locale_t __l) __attribute__ ((__nothrow__));
# 323 "/usr/include/ctype.h" 3 4

# 94 "squid_ldap_auth_and_ip.c" 2
# 124 "squid_ldap_auth_and_ip.c"
# 1 "/usr/include/lber.h" 1 3 4
# 29 "/usr/include/lber.h" 3 4
# 1 "/usr/include/lber_types.h" 1 3 4
# 24 "/usr/include/lber_types.h" 3 4
# 1 "/usr/include/ldap_cdefs.h" 1 3 4
# 25 "/usr/include/lber_types.h" 2 3 4


# 43 "/usr/include/lber_types.h" 3 4
typedef int ber_int_t;


typedef signed int ber_sint_t;
typedef unsigned int ber_uint_t;


typedef unsigned long ber_tag_t;


typedef int ber_socket_t;


typedef unsigned long ber_len_t;


typedef signed long ber_slen_t;


# 30 "/usr/include/lber.h" 2 3 4



# 117 "/usr/include/lber.h" 3 4
typedef int* (*BER_ERRNO_FN) ( void );

typedef void (*BER_LOG_PRINT_FN) ( const char *buf );

typedef void* (BER_MEMALLOC_FN) ( ber_len_t size, void *ctx );
typedef void* (BER_MEMCALLOC_FN) ( ber_len_t n, ber_len_t size, void *ctx );
typedef void* (BER_MEMREALLOC_FN) ( void *p, ber_len_t size, void *ctx );
typedef void (BER_MEMFREE_FN) ( void *p, void *ctx );

typedef struct lber_memory_fns {
 BER_MEMALLOC_FN *bmf_malloc;
 BER_MEMCALLOC_FN *bmf_calloc;
 BER_MEMREALLOC_FN *bmf_realloc;
 BER_MEMFREE_FN *bmf_free;
} BerMemoryFunctions;
# 164 "/usr/include/lber.h" 3 4
extern char ber_pvt_opt_on;






typedef struct berelement BerElement;
typedef struct sockbuf Sockbuf;

typedef struct sockbuf_io Sockbuf_IO;


typedef struct sockbuf_io_desc {
 int sbiod_level;
 Sockbuf *sbiod_sb;
 Sockbuf_IO *sbiod_io;
 void *sbiod_pvt;
 struct sockbuf_io_desc *sbiod_next;
} Sockbuf_IO_Desc;


struct sockbuf_io {
 int (*sbi_setup)( Sockbuf_IO_Desc *sbiod, void *arg );
 int (*sbi_remove)( Sockbuf_IO_Desc *sbiod );
 int (*sbi_ctrl)( Sockbuf_IO_Desc *sbiod, int opt, void *arg);

 ber_slen_t (*sbi_read)( Sockbuf_IO_Desc *sbiod, void *buf,
  ber_len_t len );
 ber_slen_t (*sbi_write)( Sockbuf_IO_Desc *sbiod, void *buf,
  ber_len_t len );

 int (*sbi_close)( Sockbuf_IO_Desc *sbiod );
};
# 212 "/usr/include/lber.h" 3 4
typedef struct berval {
 ber_len_t bv_len;
 char *bv_val;
} BerValue;

typedef BerValue *BerVarray;






extern void
ber_error_print ( const char *data );


extern void
ber_bprint ( const char *data, ber_len_t len );


extern void
ber_dump ( BerElement *ber, int inout );





typedef int (*BERDecodeCallback) ( BerElement *ber, void *data, int mode );




extern ber_tag_t
ber_get_tag ( BerElement *ber );


extern ber_tag_t
ber_skip_tag ( BerElement *ber, ber_len_t *len );



extern ber_tag_t
ber_peek_tag ( BerElement *ber, ber_len_t *len );



extern ber_tag_t
ber_skip_element ( BerElement *ber, struct berval *bv );



extern ber_tag_t
ber_peek_element ( const BerElement *ber, struct berval *bv );



extern ber_tag_t
ber_get_int ( BerElement *ber, ber_int_t *num );



extern ber_tag_t
ber_get_enum ( BerElement *ber, ber_int_t *num );



extern ber_tag_t
ber_get_stringb ( BerElement *ber, char *buf, ber_len_t *len );
# 291 "/usr/include/lber.h" 3 4
extern ber_tag_t
ber_get_stringbv ( BerElement *ber, struct berval *bv, int options );




extern ber_tag_t
ber_get_stringa ( BerElement *ber, char **buf );



extern ber_tag_t
ber_get_stringal ( BerElement *ber, struct berval **bv );



extern ber_tag_t
ber_get_bitstringa ( BerElement *ber, char **buf, ber_len_t *len );




extern ber_tag_t
ber_get_null ( BerElement *ber );


extern ber_tag_t
ber_get_boolean ( BerElement *ber, ber_int_t *boolval );



extern ber_tag_t
ber_first_element ( BerElement *ber, ber_len_t *len, char **last );




extern ber_tag_t
ber_next_element ( BerElement *ber, ber_len_t *len, const char *last );




extern ber_tag_t
ber_scanf ( BerElement *ber, const char *fmt, ... );




extern int
ber_decode_oid ( struct berval *in, struct berval *out );






extern int
ber_encode_oid ( struct berval *in, struct berval *out );



typedef int (*BEREncodeCallback) ( BerElement *ber, void *data );



extern int
ber_put_enum ( BerElement *ber, ber_int_t num, ber_tag_t tag );




extern int
ber_put_int ( BerElement *ber, ber_int_t num, ber_tag_t tag );




extern int
ber_put_ostring ( BerElement *ber, const char *str, ber_len_t len, ber_tag_t tag );





extern int
ber_put_berval ( BerElement *ber, struct berval *bv, ber_tag_t tag );




extern int
ber_put_string ( BerElement *ber, const char *str, ber_tag_t tag );




extern int
ber_put_bitstring ( BerElement *ber, const char *str, ber_len_t bitlen, ber_tag_t tag );





extern int
ber_put_null ( BerElement *ber, ber_tag_t tag );



extern int
ber_put_boolean ( BerElement *ber, ber_int_t boolval, ber_tag_t tag );




extern int
ber_start_seq ( BerElement *ber, ber_tag_t tag );



extern int
ber_start_set ( BerElement *ber, ber_tag_t tag );



extern int
ber_put_seq ( BerElement *ber );


extern int
ber_put_set ( BerElement *ber );


extern int
ber_printf ( BerElement *ber, const char *fmt, ... );
# 435 "/usr/include/lber.h" 3 4
extern ber_slen_t
ber_skip_data ( BerElement *ber, ber_len_t len );



extern ber_slen_t
ber_read ( BerElement *ber, char *buf, ber_len_t len );




extern ber_slen_t
ber_write ( BerElement *ber, const char *buf, ber_len_t len, int zero );





extern void
ber_free ( BerElement *ber, int freebuf );



extern void
ber_free_buf ( BerElement *ber );

extern int
ber_flush2 ( Sockbuf *sb, BerElement *ber, int freeit );
# 471 "/usr/include/lber.h" 3 4
extern int
ber_flush ( Sockbuf *sb, BerElement *ber, int freeit );




extern BerElement *
ber_alloc ( void );

extern BerElement *
der_alloc ( void );

extern BerElement *
ber_alloc_t ( int beroptions );


extern BerElement *
ber_dup ( BerElement *ber );


extern ber_tag_t
ber_get_next ( Sockbuf *sb, ber_len_t *len, BerElement *ber );




extern void
ber_init2 ( BerElement *ber, struct berval *bv, int options );




extern void
ber_init_w_nullc ( BerElement *ber, int options );



extern void
ber_reset ( BerElement *ber, int was_writing );



extern BerElement *
ber_init ( struct berval *bv );


extern int
ber_flatten ( BerElement *ber, struct berval **bvPtr );



extern int
ber_flatten2 ( BerElement *ber, struct berval *bv, int alloc );




extern int
ber_remaining ( BerElement *ber );






extern int
ber_get_option ( void *item, int option, void *outvalue);




extern int
ber_set_option ( void *item, int option, const void *invalue);
# 552 "/usr/include/lber.h" 3 4
extern Sockbuf *
ber_sockbuf_alloc ( void );


extern void
ber_sockbuf_free ( Sockbuf *sb );


extern int
ber_sockbuf_add_io ( Sockbuf *sb, Sockbuf_IO *sbio, int layer, void *arg );





extern int
ber_sockbuf_remove_io ( Sockbuf *sb, Sockbuf_IO *sbio, int layer );




extern int
ber_sockbuf_ctrl ( Sockbuf *sb, int opt, void *arg );




extern Sockbuf_IO ber_sockbuf_io_tcp;
extern Sockbuf_IO ber_sockbuf_io_readahead;
extern Sockbuf_IO ber_sockbuf_io_fd;
extern Sockbuf_IO ber_sockbuf_io_debug;
extern Sockbuf_IO ber_sockbuf_io_udp;




extern void *
ber_memalloc ( ber_len_t s );


extern void *
ber_memrealloc ( void* p, ber_len_t s );



extern void *
ber_memcalloc ( ber_len_t n, ber_len_t s );



extern void
ber_memfree ( void* p );


extern void
ber_memvfree ( void** vector );


extern void
ber_bvfree ( struct berval *bv );


extern void
ber_bvecfree ( struct berval **bv );


extern int
ber_bvecadd ( struct berval ***bvec, struct berval *bv );



extern struct berval *
ber_dupbv ( struct berval *dst, struct berval *src );


extern struct berval *
ber_bvdup ( struct berval *src );


extern struct berval *
ber_mem2bv ( const char *, ber_len_t len, int duplicate, struct berval *bv);


extern struct berval *
ber_str2bv ( const char *, ber_len_t len, int duplicate, struct berval *bv);





extern char *
ber_strdup ( const char * );


extern ber_len_t
ber_strnlen ( const char *s, ber_len_t len );


extern char *
ber_strndup ( const char *s, ber_len_t l );


extern struct berval *
ber_bvreplace ( struct berval *dst, const struct berval *src );


extern void
ber_bvarray_free ( BerVarray p );

extern int
ber_bvarray_add ( BerVarray *p, BerValue *bv );
# 672 "/usr/include/lber.h" 3 4
extern int * ber_errno_addr (void);







# 125 "squid_ldap_auth_and_ip.c" 2
# 1 "/usr/include/ldap.h" 1 3 4
# 33 "/usr/include/ldap.h" 3 4
# 1 "/usr/include/ldap_features.h" 1 3 4
# 34 "/usr/include/ldap.h" 2 3 4


# 220 "/usr/include/ldap.h" 3 4
typedef struct ldapapiinfo {
 int ldapai_info_version;

 int ldapai_api_version;
 int ldapai_protocol_version;
 char **ldapai_extensions;
 char *ldapai_vendor_name;
 int ldapai_vendor_version;
} LDAPAPIInfo;

typedef struct ldap_apifeature_info {
 int ldapaif_info_version;

 char* ldapaif_name;
 int ldapaif_version;
} LDAPAPIFeatureInfo;




typedef struct ldapcontrol {
 char * ldctl_oid;
 struct berval ldctl_value;
 char ldctl_iscritical;
} LDAPControl;
# 718 "/usr/include/ldap.h" 3 4
typedef struct ldapmsg LDAPMessage;


typedef struct ldapmod {
 int mod_op;
# 735 "/usr/include/ldap.h" 3 4
 char *mod_type;
 union mod_vals_u {
  char **modv_strvals;
  struct berval **modv_bvals;
 } mod_vals;


} LDAPMod;






typedef struct ldap LDAP;
# 766 "/usr/include/ldap.h" 3 4
typedef struct ldap_url_desc {
 struct ldap_url_desc *lud_next;
 char *lud_scheme;
 char *lud_host;
 int lud_port;
 char *lud_dn;
 char **lud_attrs;
 int lud_scope;
 char *lud_filter;
 char **lud_exts;
 int lud_crit_exts;
} LDAPURLDesc;
# 796 "/usr/include/ldap.h" 3 4
typedef struct ldap_sync_t ldap_sync_t;

typedef enum {

 LDAP_SYNC_CAPI_NONE = -1,

 LDAP_SYNC_CAPI_PHASE_FLAG = 0x10U,
 LDAP_SYNC_CAPI_IDSET_FLAG = 0x20U,
 LDAP_SYNC_CAPI_DONE_FLAG = 0x40U,


 LDAP_SYNC_CAPI_PRESENT = 0,
 LDAP_SYNC_CAPI_ADD = 1,
 LDAP_SYNC_CAPI_MODIFY = 2,
 LDAP_SYNC_CAPI_DELETE = 3,


 LDAP_SYNC_CAPI_PRESENTS = ( LDAP_SYNC_CAPI_PHASE_FLAG | LDAP_SYNC_CAPI_PRESENT ),
 LDAP_SYNC_CAPI_DELETES = ( LDAP_SYNC_CAPI_PHASE_FLAG | LDAP_SYNC_CAPI_DELETE ),

 LDAP_SYNC_CAPI_PRESENTS_IDSET = ( LDAP_SYNC_CAPI_PRESENTS | LDAP_SYNC_CAPI_IDSET_FLAG ),
 LDAP_SYNC_CAPI_DELETES_IDSET = ( LDAP_SYNC_CAPI_DELETES | LDAP_SYNC_CAPI_IDSET_FLAG ),

 LDAP_SYNC_CAPI_DONE = ( LDAP_SYNC_CAPI_DONE_FLAG | LDAP_SYNC_CAPI_PRESENTS )
} ldap_sync_refresh_t;
# 830 "/usr/include/ldap.h" 3 4
typedef int (*ldap_sync_search_entry_f) ( ldap_sync_t *ls, LDAPMessage *msg, struct berval *entryUUID, ldap_sync_refresh_t phase );
# 840 "/usr/include/ldap.h" 3 4
typedef int (*ldap_sync_search_reference_f) ( ldap_sync_t *ls, LDAPMessage *msg );
# 857 "/usr/include/ldap.h" 3 4
typedef int (*ldap_sync_intermediate_f) ( ldap_sync_t *ls, LDAPMessage *msg, BerVarray syncUUIDs, ldap_sync_refresh_t phase );
# 868 "/usr/include/ldap.h" 3 4
typedef int (*ldap_sync_search_result_f) ( ldap_sync_t *ls, LDAPMessage *msg, int refreshDeletes );
# 877 "/usr/include/ldap.h" 3 4
struct ldap_sync_t {

 char *ls_base;
 int ls_scope;
 char *ls_filter;
 char **ls_attrs;
 int ls_timelimit;
 int ls_sizelimit;


 int ls_timeout;


 ldap_sync_search_entry_f ls_search_entry;
 ldap_sync_search_reference_f ls_search_reference;
 ldap_sync_intermediate_f ls_intermediate;
 ldap_sync_search_result_f ls_search_result;


 void *ls_private;


 LDAP *ls_ld;







 int ls_msgid;



 int ls_reloadHint;


 struct berval ls_cookie;


 ldap_sync_refresh_t ls_refreshPhase;
};
# 927 "/usr/include/ldap.h" 3 4
struct ldap_conncb;
struct sockaddr;


typedef int (ldap_conn_add_f) ( LDAP *ld, Sockbuf *sb, LDAPURLDesc *srv, struct sockaddr *addr, struct ldap_conncb *ctx );


typedef void (ldap_conn_del_f) ( LDAP *ld, Sockbuf *sb, struct ldap_conncb *ctx );




typedef struct ldap_conncb {
 ldap_conn_add_f *lc_add;
 ldap_conn_del_f *lc_del;
 void *lc_arg;
} ldap_conncb;





struct timeval;




extern int
ldap_get_option ( LDAP *ld, int option, void *outvalue);




extern int
ldap_set_option ( LDAP *ld, int option, const void *invalue);





typedef int (LDAP_REBIND_PROC) ( LDAP *ld, const char *url, ber_tag_t request, ber_int_t msgid, void *params );




extern int
ldap_set_rebind_proc ( LDAP *ld, LDAP_REBIND_PROC *rebind_proc, void *params );





typedef int (LDAP_NEXTREF_PROC) ( LDAP *ld, char ***refsp, int *cntp, void *params );



extern int
ldap_set_nextref_proc ( LDAP *ld, LDAP_NEXTREF_PROC *nextref_proc, void *params );





typedef int (LDAP_URLLIST_PROC) ( LDAP *ld, LDAPURLDesc **urllist, LDAPURLDesc **url, void *params );





extern int
ldap_set_urllist_proc ( LDAP *ld, LDAP_URLLIST_PROC *urllist_proc, void *params );
# 1006 "/usr/include/ldap.h" 3 4
extern int
ldap_create_control ( const char *requestOID, BerElement *ber, int iscritical, LDAPControl **ctrlp );





extern LDAPControl *
ldap_find_control ( const char *oid, LDAPControl **ctrls );




extern int
ldap_control_create ( const char *requestOID, int iscritical, struct berval *value, int dupval, LDAPControl **ctrlp );






extern LDAPControl *
ldap_control_find ( const char *oid, LDAPControl **ctrls, LDAPControl ***nextctrlp );




extern void
ldap_control_free ( LDAPControl *ctrl );


extern void
ldap_controls_free ( LDAPControl **ctrls );


extern LDAPControl **
ldap_controls_dup ( LDAPControl *const *controls );


extern LDAPControl *
ldap_control_dup ( const LDAPControl *c );





extern int
ldap_domain2dn ( const char* domain, char** dn );



extern int
ldap_dn2domain ( const char* dn, char** domain );



extern int
ldap_domain2hostlist ( const char *domain, char** hostlist );






extern int
ldap_extended_operation ( LDAP *ld, const char *reqoid, struct berval *reqdata, LDAPControl **serverctrls, LDAPControl **clientctrls, int *msgidp );







extern int
ldap_extended_operation_s ( LDAP *ld, const char *reqoid, struct berval *reqdata, LDAPControl **serverctrls, LDAPControl **clientctrls, char **retoidp, struct berval **retdatap );
# 1089 "/usr/include/ldap.h" 3 4
extern int
ldap_parse_extended_result ( LDAP *ld, LDAPMessage *res, char **retoidp, struct berval **retdatap, int freeit );






extern int
ldap_parse_intermediate ( LDAP *ld, LDAPMessage *res, char **retoidp, struct berval **retdatap, LDAPControl ***serverctrls, int freeit );
# 1110 "/usr/include/ldap.h" 3 4
extern int
ldap_abandon_ext ( LDAP *ld, int msgid, LDAPControl **serverctrls, LDAPControl **clientctrls );






extern int
ldap_abandon ( LDAP *ld, int msgid );







extern int
ldap_add_ext ( LDAP *ld, const char *dn, LDAPMod **attrs, LDAPControl **serverctrls, LDAPControl **clientctrls, int *msgidp );







extern int
ldap_add_ext_s ( LDAP *ld, const char *dn, LDAPMod **attrs, LDAPControl **serverctrls, LDAPControl **clientctrls );







extern int
ldap_add ( LDAP *ld, const char *dn, LDAPMod **attrs );




extern int
ldap_add_s ( LDAP *ld, const char *dn, LDAPMod **attrs );
# 1162 "/usr/include/ldap.h" 3 4
extern int
ldap_sasl_bind ( LDAP *ld, const char *dn, const char *mechanism, struct berval *cred, LDAPControl **serverctrls, LDAPControl **clientctrls, int *msgidp );
# 1186 "/usr/include/ldap.h" 3 4
typedef int (LDAP_SASL_INTERACT_PROC) ( LDAP *ld, unsigned flags, void* defaults, void *interact );


extern int
ldap_sasl_interactive_bind_s ( LDAP *ld, const char *dn, const char *saslMechanism, LDAPControl **serverControls, LDAPControl **clientControls, unsigned flags, LDAP_SASL_INTERACT_PROC *proc, void *defaults );
# 1202 "/usr/include/ldap.h" 3 4
extern int
ldap_sasl_bind_s ( LDAP *ld, const char *dn, const char *mechanism, struct berval *cred, LDAPControl **serverctrls, LDAPControl **clientctrls, struct berval **servercredp );
# 1212 "/usr/include/ldap.h" 3 4
extern int
ldap_parse_sasl_bind_result ( LDAP *ld, LDAPMessage *res, struct berval **servercredp, int freeit );
# 1224 "/usr/include/ldap.h" 3 4
extern int
ldap_bind ( LDAP *ld, const char *who, const char *passwd, int authmethod );





extern int
ldap_bind_s ( LDAP *ld, const char *who, const char *cred, int authmethod );
# 1241 "/usr/include/ldap.h" 3 4
extern int
ldap_simple_bind ( LDAP *ld, const char *who, const char *passwd );




extern int
ldap_simple_bind_s ( LDAP *ld, const char *who, const char *passwd );
# 1259 "/usr/include/ldap.h" 3 4
extern int
ldap_compare_ext ( LDAP *ld, const char *dn, const char *attr, struct berval *bvalue, LDAPControl **serverctrls, LDAPControl **clientctrls, int *msgidp );
# 1269 "/usr/include/ldap.h" 3 4
extern int
ldap_compare_ext_s ( LDAP *ld, const char *dn, const char *attr, struct berval *bvalue, LDAPControl **serverctrls, LDAPControl **clientctrls );
# 1279 "/usr/include/ldap.h" 3 4
extern int
ldap_compare ( LDAP *ld, const char *dn, const char *attr, const char *value );





extern int
ldap_compare_s ( LDAP *ld, const char *dn, const char *attr, const char *value );
# 1298 "/usr/include/ldap.h" 3 4
extern int
ldap_delete_ext ( LDAP *ld, const char *dn, LDAPControl **serverctrls, LDAPControl **clientctrls, int *msgidp );






extern int
ldap_delete_ext_s ( LDAP *ld, const char *dn, LDAPControl **serverctrls, LDAPControl **clientctrls );






extern int
ldap_delete ( LDAP *ld, const char *dn );



extern int
ldap_delete_s ( LDAP *ld, const char *dn );
# 1329 "/usr/include/ldap.h" 3 4
extern int
ldap_parse_result ( LDAP *ld, LDAPMessage *res, int *errcodep, char **matcheddnp, char **errmsgp, char ***referralsp, LDAPControl ***serverctrls, int freeit );
# 1340 "/usr/include/ldap.h" 3 4
extern char *
ldap_err2string ( int err );



extern int
ldap_result2error ( LDAP *ld, LDAPMessage *r, int freeit );




extern void
ldap_perror ( LDAP *ld, const char *s );
# 1361 "/usr/include/ldap.h" 3 4
extern int
ldap_gssapi_bind ( LDAP *ld, const char *dn, const char *creds );




extern int
ldap_gssapi_bind_s ( LDAP *ld, const char *dn, const char *creds );
# 1377 "/usr/include/ldap.h" 3 4
extern int
ldap_modify_ext ( LDAP *ld, const char *dn, LDAPMod **mods, LDAPControl **serverctrls, LDAPControl **clientctrls, int *msgidp );







extern int
ldap_modify_ext_s ( LDAP *ld, const char *dn, LDAPMod **mods, LDAPControl **serverctrls, LDAPControl **clientctrls );







extern int
ldap_modify ( LDAP *ld, const char *dn, LDAPMod **mods );




extern int
ldap_modify_s ( LDAP *ld, const char *dn, LDAPMod **mods );
# 1412 "/usr/include/ldap.h" 3 4
extern int
ldap_rename ( LDAP *ld, const char *dn, const char *newrdn, const char *newSuperior, int deleteoldrdn, LDAPControl **sctrls, LDAPControl **cctrls, int *msgidp );
# 1423 "/usr/include/ldap.h" 3 4
extern int
ldap_rename_s ( LDAP *ld, const char *dn, const char *newrdn, const char *newSuperior, int deleteoldrdn, LDAPControl **sctrls, LDAPControl **cctrls );
# 1434 "/usr/include/ldap.h" 3 4
extern int
ldap_rename2 ( LDAP *ld, const char *dn, const char *newrdn, const char *newSuperior, int deleteoldrdn );






extern int
ldap_rename2_s ( LDAP *ld, const char *dn, const char *newrdn, const char *newSuperior, int deleteoldrdn );






extern int
ldap_modrdn ( LDAP *ld, const char *dn, const char *newrdn );




extern int
ldap_modrdn_s ( LDAP *ld, const char *dn, const char *newrdn );




extern int
ldap_modrdn2 ( LDAP *ld, const char *dn, const char *newrdn, int deleteoldrdn );





extern int
ldap_modrdn2_s ( LDAP *ld, const char *dn, const char *newrdn, int deleteoldrdn);
# 1482 "/usr/include/ldap.h" 3 4
extern LDAP *
ldap_init ( const char *host, int port );



extern LDAP *
ldap_open ( const char *host, int port );




extern int
ldap_create ( LDAP **ldp );


extern int
ldap_initialize ( LDAP **ldp, const char *url );







extern int
ldap_tls_inplace ( LDAP *ld );


extern int
ldap_start_tls ( LDAP *ld, LDAPControl **serverctrls, LDAPControl **clientctrls, int *msgidp );





extern int
ldap_install_tls ( LDAP *ld );


extern int
ldap_start_tls_s ( LDAP *ld, LDAPControl **serverctrls, LDAPControl **clientctrls );







extern LDAPMessage *
ldap_first_message ( LDAP *ld, LDAPMessage *chain );



extern LDAPMessage *
ldap_next_message ( LDAP *ld, LDAPMessage *msg );



extern int
ldap_count_messages ( LDAP *ld, LDAPMessage *chain );






extern LDAPMessage *
ldap_first_reference ( LDAP *ld, LDAPMessage *chain );



extern LDAPMessage *
ldap_next_reference ( LDAP *ld, LDAPMessage *ref );



extern int
ldap_count_references ( LDAP *ld, LDAPMessage *chain );



extern int
ldap_parse_reference ( LDAP *ld, LDAPMessage *ref, char ***referralsp, LDAPControl ***serverctrls, int freeit);
# 1575 "/usr/include/ldap.h" 3 4
extern LDAPMessage *
ldap_first_entry ( LDAP *ld, LDAPMessage *chain );



extern LDAPMessage *
ldap_next_entry ( LDAP *ld, LDAPMessage *entry );



extern int
ldap_count_entries ( LDAP *ld, LDAPMessage *chain );



extern int
ldap_get_entry_controls ( LDAP *ld, LDAPMessage *entry, LDAPControl ***serverctrls);
# 1600 "/usr/include/ldap.h" 3 4
extern LDAPMessage *
ldap_delete_result_entry ( LDAPMessage **list, LDAPMessage *e );



extern void
ldap_add_result_entry ( LDAPMessage **list, LDAPMessage *e );







extern char *
ldap_get_dn ( LDAP *ld, LDAPMessage *entry );



typedef struct ldap_ava {
 struct berval la_attr;
 struct berval la_value;
 unsigned la_flags;







 void *la_private;
} LDAPAVA;

typedef LDAPAVA** LDAPRDN;
typedef LDAPRDN* LDAPDN;
# 1653 "/usr/include/ldap.h" 3 4
extern void ldap_rdnfree ( LDAPRDN rdn );
extern void ldap_dnfree ( LDAPDN dn );

extern int
ldap_bv2dn ( struct berval *bv, LDAPDN *dn, unsigned flags );




extern int
ldap_str2dn ( const char *str, LDAPDN *dn, unsigned flags );




extern int
ldap_dn2bv ( LDAPDN dn, struct berval *bv, unsigned flags );




extern int
ldap_dn2str ( LDAPDN dn, char **str, unsigned flags );




extern int
ldap_bv2rdn ( struct berval *bv, LDAPRDN *rdn, char **next, unsigned flags );





extern int
ldap_str2rdn ( const char *str, LDAPRDN *rdn, char **next, unsigned flags );





extern int
ldap_rdn2bv ( LDAPRDN rdn, struct berval *bv, unsigned flags );




extern int
ldap_rdn2str ( LDAPRDN rdn, char **str, unsigned flags );




extern int
ldap_dn_normalize ( const char *in, unsigned iflags, char **out, unsigned oflags );



extern char *
ldap_dn2ufn ( const char *dn );


extern char **
ldap_explode_dn ( const char *dn, int notypes );



extern char **
ldap_explode_rdn ( const char *rdn, int notypes );



typedef int LDAPDN_rewrite_func
 ( LDAPDN dn, unsigned flags, void *ctx );

extern int
ldap_X509dn2bv ( void *x509_name, struct berval *dn, LDAPDN_rewrite_func *func, unsigned flags );


extern char *
ldap_dn2dcedn ( const char *dn );


extern char *
ldap_dcedn2dn ( const char *dce );


extern char *
ldap_dn2ad_canonical ( const char *dn );


extern int
ldap_get_dn_ber ( LDAP *ld, LDAPMessage *e, BerElement **berout, struct berval *dn );


extern int
ldap_get_attribute_ber ( LDAP *ld, LDAPMessage *e, BerElement *ber, struct berval *attr, struct berval **vals );






extern char *
ldap_first_attribute ( LDAP *ld, LDAPMessage *entry, BerElement **ber );




extern char *
ldap_next_attribute ( LDAP *ld, LDAPMessage *entry, BerElement *ber );
# 1772 "/usr/include/ldap.h" 3 4
extern struct berval **
ldap_get_values_len ( LDAP *ld, LDAPMessage *entry, const char *target );




extern int
ldap_count_values_len ( struct berval **vals );


extern void
ldap_value_free_len ( struct berval **vals );



extern char **
ldap_get_values ( LDAP *ld, LDAPMessage *entry, const char *target );




extern int
ldap_count_values ( char **vals );


extern void
ldap_value_free ( char **vals );






extern int
ldap_result ( LDAP *ld, int msgid, int all, struct timeval *timeout, LDAPMessage **result );






extern int
ldap_msgtype ( LDAPMessage *lm );


extern int
ldap_msgid ( LDAPMessage *lm );


extern int
ldap_msgfree ( LDAPMessage *lm );


extern int
ldap_msgdelete ( LDAP *ld, int msgid );







extern int
ldap_bv2escaped_filter_value ( struct berval *in, struct berval *out );



extern int
ldap_search_ext ( LDAP *ld, const char *base, int scope, const char *filter, char **attrs, int attrsonly, LDAPControl **serverctrls, LDAPControl **clientctrls, struct timeval *timeout, int sizelimit, int *msgidp );
# 1853 "/usr/include/ldap.h" 3 4
extern int
ldap_search_ext_s ( LDAP *ld, const char *base, int scope, const char *filter, char **attrs, int attrsonly, LDAPControl **serverctrls, LDAPControl **clientctrls, struct timeval *timeout, int sizelimit, LDAPMessage **res );
# 1868 "/usr/include/ldap.h" 3 4
extern int
ldap_search ( LDAP *ld, const char *base, int scope, const char *filter, char **attrs, int attrsonly );







extern int
ldap_search_s ( LDAP *ld, const char *base, int scope, const char *filter, char **attrs, int attrsonly, LDAPMessage **res );
# 1887 "/usr/include/ldap.h" 3 4
extern int
ldap_search_st ( LDAP *ld, const char *base, int scope, const char *filter, char **attrs, int attrsonly, struct timeval *timeout, LDAPMessage **res );
# 1902 "/usr/include/ldap.h" 3 4
extern int
ldap_unbind_ext ( LDAP *ld, LDAPControl **serverctrls, LDAPControl **clientctrls);




extern int
ldap_unbind_ext_s ( LDAP *ld, LDAPControl **serverctrls, LDAPControl **clientctrls);





extern int
ldap_unbind ( LDAP *ld );


extern int
ldap_unbind_s ( LDAP *ld );






extern int
ldap_put_vrFilter ( BerElement *ber, const char *vrf );







extern void *
ldap_memalloc ( ber_len_t s );


extern void *
ldap_memrealloc ( void* p, ber_len_t s );



extern void *
ldap_memcalloc ( ber_len_t n, ber_len_t s );



extern void
ldap_memfree ( void* p );


extern void
ldap_memvfree ( void** v );


extern char *
ldap_strdup ( const char * );


extern void
ldap_mods_free ( LDAPMod **mods, int freemods );
# 1972 "/usr/include/ldap.h" 3 4
typedef int (LDAP_SORT_AD_CMP_PROC) ( const char *left, const char *right );



typedef int (LDAP_SORT_AV_CMP_PROC) ( const void *left, const void *right );



extern int
ldap_sort_entries ( LDAP *ld, LDAPMessage **chain, const char *attr, LDAP_SORT_AD_CMP_PROC *cmp );




extern int
ldap_sort_values ( LDAP *ld, char **vals, LDAP_SORT_AV_CMP_PROC *cmp );




extern int
ldap_sort_strcasecmp ( const void *a, const void *b );







extern int
ldap_is_ldap_url ( const char *url );


extern int
ldap_is_ldaps_url ( const char *url );


extern int
ldap_is_ldapi_url ( const char *url );


extern int
ldap_url_parse ( const char *url, LDAPURLDesc **ludpp );



extern char *
ldap_url_desc2str ( LDAPURLDesc *ludp );


extern void
ldap_free_urldesc ( LDAPURLDesc *ludp );
# 2033 "/usr/include/ldap.h" 3 4
extern int
ldap_cancel ( LDAP *ld, int cancelid, LDAPControl **sctrls, LDAPControl **cctrls, int *msgidp );





extern int
ldap_cancel_s ( LDAP *ld, int cancelid, LDAPControl **sctrl, LDAPControl **cctrl );
# 2052 "/usr/include/ldap.h" 3 4
extern int
ldap_turn ( LDAP *ld, int mutual, const char* identifier, LDAPControl **sctrls, LDAPControl **cctrls, int *msgidp );






extern int
ldap_turn_s ( LDAP *ld, int mutual, const char* identifier, LDAPControl **sctrl, LDAPControl **cctrl );
# 2073 "/usr/include/ldap.h" 3 4
extern int
ldap_create_page_control_value ( LDAP *ld, ber_int_t pagesize, struct berval *cookie, struct berval *value );





extern int
ldap_create_page_control ( LDAP *ld, ber_int_t pagesize, struct berval *cookie, int iscritical, LDAPControl **ctrlp );







extern int
ldap_parse_page_control ( LDAP *ld, LDAPControl **ctrls, ber_int_t *count, struct berval **cookie );







extern int
ldap_parse_pageresponse_control ( LDAP *ld, LDAPControl *ctrl, ber_int_t *count, struct berval *cookie );
# 2112 "/usr/include/ldap.h" 3 4
typedef struct ldapsortkey {
 char *attributeType;
 char *orderingRule;
 int reverseOrder;
} LDAPSortKey;

extern int
ldap_create_sort_keylist ( LDAPSortKey ***sortKeyList, char *keyString );



extern void
ldap_free_sort_keylist ( LDAPSortKey **sortkeylist );


extern int
ldap_create_sort_control_value ( LDAP *ld, LDAPSortKey **keyList, struct berval *value );




extern int
ldap_create_sort_control ( LDAP *ld, LDAPSortKey **keyList, int iscritical, LDAPControl **ctrlp );





extern int
ldap_parse_sortresponse_control ( LDAP *ld, LDAPControl *ctrl, ber_int_t *result, char **attribute );
# 2154 "/usr/include/ldap.h" 3 4
typedef struct ldapvlvinfo {
 ber_int_t ldvlv_version;
    ber_int_t ldvlv_before_count;
    ber_int_t ldvlv_after_count;
    ber_int_t ldvlv_offset;
    ber_int_t ldvlv_count;
    struct berval * ldvlv_attrvalue;
    struct berval * ldvlv_context;
    void * ldvlv_extradata;
} LDAPVLVInfo;

extern int
ldap_create_vlv_control_value ( LDAP *ld, LDAPVLVInfo *ldvlistp, struct berval *value);




extern int
ldap_create_vlv_control ( LDAP *ld, LDAPVLVInfo *ldvlistp, LDAPControl **ctrlp );




extern int
ldap_parse_vlvresponse_control ( LDAP *ld, LDAPControl *ctrls, ber_int_t *target_posp, ber_int_t *list_countp, struct berval **contextp, int *errcodep );
# 2192 "/usr/include/ldap.h" 3 4
extern int
ldap_parse_whoami ( LDAP *ld, LDAPMessage *res, struct berval **authzid );




extern int
ldap_whoami ( LDAP *ld, LDAPControl **sctrls, LDAPControl **cctrls, int *msgidp );




extern int
ldap_whoami_s ( LDAP *ld, struct berval **authzid, LDAPControl **sctrls, LDAPControl **cctrls );
# 2217 "/usr/include/ldap.h" 3 4
extern int
ldap_parse_passwd ( LDAP *ld, LDAPMessage *res, struct berval *newpasswd );




extern int
ldap_passwd ( LDAP *ld, struct berval *user, struct berval *oldpw, struct berval *newpw, LDAPControl **sctrls, LDAPControl **cctrls, int *msgidp );







extern int
ldap_passwd_s ( LDAP *ld, struct berval *user, struct berval *oldpw, struct berval *newpw, struct berval *newpasswd, LDAPControl **sctrls, LDAPControl **cctrls );
# 2249 "/usr/include/ldap.h" 3 4
typedef enum passpolicyerror_enum {
       PP_passwordExpired = 0,
       PP_accountLocked = 1,
       PP_changeAfterReset = 2,
       PP_passwordModNotAllowed = 3,
       PP_mustSupplyOldPassword = 4,
       PP_insufficientPasswordQuality = 5,
       PP_passwordTooShort = 6,
       PP_passwordTooYoung = 7,
       PP_passwordInHistory = 8,
       PP_noError = 65535
} LDAPPasswordPolicyError;

extern int
ldap_create_passwordpolicy_control ( LDAP *ld, LDAPControl **ctrlp );



extern int
ldap_parse_passwordpolicy_control ( LDAP *ld, LDAPControl *ctrl, ber_int_t *expirep, ber_int_t *gracep, LDAPPasswordPolicyError *errorp );






extern const char *
ldap_passwordpolicy_err2txt ( LDAPPasswordPolicyError );
# 2285 "/usr/include/ldap.h" 3 4
extern int
ldap_parse_refresh ( LDAP *ld, LDAPMessage *res, ber_int_t *newttl );




extern int
ldap_refresh ( LDAP *ld, struct berval *dn, ber_int_t ttl, LDAPControl **sctrls, LDAPControl **cctrls, int *msgidp );






extern int
ldap_refresh_s ( LDAP *ld, struct berval *dn, ber_int_t ttl, ber_int_t *newttl, LDAPControl **sctrls, LDAPControl **cctrls );
# 2348 "/usr/include/ldap.h" 3 4
extern ldap_sync_t *
ldap_sync_initialize ( ldap_sync_t *ls );





extern void
ldap_sync_destroy ( ldap_sync_t *ls, int freeit );






extern int
ldap_sync_init ( ldap_sync_t *ls, int mode );






extern int
ldap_sync_init_refresh_only ( ldap_sync_t *ls );





extern int
ldap_sync_init_refresh_and_persist ( ldap_sync_t *ls );





extern int
ldap_sync_poll ( ldap_sync_t *ls );
# 2426 "/usr/include/ldap.h" 3 4
extern int
ldap_create_assertion_control_value ( LDAP *ld, char *assertion, struct berval *value );




extern int
ldap_create_assertion_control ( LDAP *ld, char *filter, int iscritical, LDAPControl **ctrlp );
# 2443 "/usr/include/ldap.h" 3 4
typedef struct LDAPDerefSpec {
 char *derefAttr;
 char **attributes;
} LDAPDerefSpec;

typedef struct LDAPDerefVal {
 char *type;
 BerVarray vals;
 struct LDAPDerefVal *next;
} LDAPDerefVal;

typedef struct LDAPDerefRes {
 char *derefAttr;
 struct berval derefVal;
 LDAPDerefVal *attrVals;
 struct LDAPDerefRes *next;
} LDAPDerefRes;

extern int
ldap_create_deref_control_value ( LDAP *ld, LDAPDerefSpec *ds, struct berval *value );




extern int
ldap_create_deref_control ( LDAP *ld, LDAPDerefSpec *ds, int iscritical, LDAPControl **ctrlp );





extern void
ldap_derefresponse_free ( LDAPDerefRes *dr );


extern int
ldap_parse_derefresponse_control ( LDAP *ld, LDAPControl *ctrl, LDAPDerefRes **drp );




extern int
ldap_parse_deref_control ( LDAP *ld, LDAPControl **ctrls, LDAPDerefRes **drp );
# 2495 "/usr/include/ldap.h" 3 4
extern int
ldap_ntlm_bind ( LDAP *ld, const char *dn, ber_tag_t tag, struct berval *cred, LDAPControl **sctrls, LDAPControl **cctrls, int *msgidp );







extern int
ldap_parse_ntlm_bind_result ( LDAP *ld, LDAPMessage *res, struct berval *challenge);





# 126 "squid_ldap_auth_and_ip.c" 2






static const char *basedn;
static const char *searchfilter = ((void *)0);
static const char *binddn = ((void *)0);
static const char *bindpasswd = ((void *)0);
static const char *userattr = "uid";
static const char *passwdattr = ((void *)0);
static int searchscope = ((ber_int_t) 0x0002);
static int persistent = 0;
static int bind_once = 0;
static int noreferrals = 0;
static int aliasderef = 0x00;




static int connect_timeout = 0;
static int timelimit = 0;
static int debug = 0;


static int use_tls = 0;
static int version = -1;

static int checkLDAP(LDAP * ld, const char *userid, const char *password, const char *server, int port);
static int readSecret(const char *filename);
# 165 "squid_ldap_auth_and_ip.c"
static int
squid_ldap_errno(LDAP * ld)
{
    int err = 0;
    ldap_get_option(ld, 0x0031, &err);
    return err;
}
static void
squid_ldap_set_aliasderef(LDAP * ld, int deref)
{
    ldap_set_option(ld, 0x0002, &deref);
}
static void
squid_ldap_set_referrals(LDAP * ld, int referrals)
{
    int *value = referrals ? ((void *) &ber_pvt_opt_on) :((void *) 0);
    ldap_set_option(ld, 0x0008, value);
}
static void
squid_ldap_set_timelimit(LDAP * ld, int aTimeLimit)
{
    ldap_set_option(ld, 0x0004, &aTimeLimit);
}
static void
squid_ldap_set_connect_timeout(LDAP * ld, int aTimeLimit)
{

    struct timeval tv;
    tv.tv_sec = aTimeLimit;
    tv.tv_usec = 0;
    ldap_set_option(ld, 0x5005, &tv);




}
static void
squid_ldap_memfree(char *p)
{
    ldap_memfree(p);
}
# 250 "squid_ldap_auth_and_ip.c"
static LDAP *
open_ldap_connection(const char *ldapServer, int port)
{
    LDAP *ld = ((void *)0);

    if (strstr(ldapServer, "://") != ((void *)0)) {
        int rc = ldap_initialize(&ld, ldapServer);
        if (rc != 0x00) {
            fprintf(stderr, "\nUnable to connect to LDAPURI:%s\n", ldapServer);
            exit(1);
        }
    } else
# 279 "squid_ldap_auth_and_ip.c"
            if ((ld = ldap_init(ldapServer, port)) == ((void *)0)) {
                fprintf(stderr, "\nUnable to connect to LDAP server:%s port:%d\n",
                        ldapServer, port);
                exit(1);
            }
    if (connect_timeout)
        squid_ldap_set_connect_timeout(ld, connect_timeout);


    if (version == -1) {
        version = 3;
    }
    if (ldap_set_option(ld, 0x0011, &version) != 0x00) {
        fprintf(stderr, "Could not set LDAP_OPT_PROTOCOL_VERSION %d\n",
                version);
        exit(1);
    }
    if (use_tls) {

        if (version != 3) {
            fprintf(stderr, "TLS requires LDAP version 3\n");
            exit(1);
        } else if (ldap_start_tls_s(ld, ((void *)0), ((void *)0)) != 0x00) {
            fprintf(stderr, "Could not Activate TLS connection\n");
            exit(1);
        }




    }

    squid_ldap_set_timelimit(ld, timelimit);
    squid_ldap_set_referrals(ld, !noreferrals);
    squid_ldap_set_aliasderef(ld, aliasderef);
    return ld;
}


static int
validUsername(const char *user)
{
    const unsigned char *p = (const unsigned char *) user;


    if (((*__ctype_b_loc ())[(int) (((unsigned char)p[0]))] & (unsigned short int) _ISspace))
        return 0;
    while (p[0] && p[1]) {
        if (((*__ctype_b_loc ())[(int) (((unsigned char)p[0]))] & (unsigned short int) _ISspace)) {

            if (((*__ctype_b_loc ())[(int) (((unsigned char)p[1]))] & (unsigned short int) _ISspace))
                return 0;

            if (p[0] != ' ')
                return 0;
        }
        p++;
    }

    if (((*__ctype_b_loc ())[(int) (((unsigned char)p[0]))] & (unsigned short int) _ISspace))
        return 0;
    return 1;
}

int
main(int argc, char **argv)
{
    char buf[1024];
    char *user, *passwd;
    char *ldapServer = ((void *)0);
    LDAP *ld = ((void *)0);
    int tryagain;
    int port = 389;

    setbuf(stdout, ((void *)0));

    while (argc > 1 && argv[1][0] == '-') {
        const char *value = "";
        char option = argv[1][1];
        switch (option) {
        case 'P':
        case 'R':
        case 'z':
        case 'Z':
        case 'd':
        case 'O':
            break;
        default:
            if (strlen(argv[1]) > 2) {
                value = argv[1] + 2;
            } else if (argc > 2) {
                value = argv[2];
                argv++;
                argc--;
            } else
                value = "";
            break;
        }
        argv++;
        argc--;
        switch (option) {
        case 'H':





        case 'h':
            if (ldapServer) {
                int len = strlen(ldapServer) + 1 + strlen(value) + 1;
                char *newhost = malloc(len);
                snprintf(newhost, len, "%s %s", ldapServer, value);
                free(ldapServer);
                ldapServer = newhost;
            } else {
                ldapServer = (__extension__ (__builtin_constant_p (value) && ((size_t)(const void *)((value) + 1) - (size_t)(const void *)(value) == 1) ? (((__const char *) (value))[0] == '\0' ? (char *) calloc ((size_t) 1, (size_t) 1) : ({ size_t __len = strlen (value) + 1; char *__retval = (char *) malloc (__len); if (__retval != ((void *)0)) __retval = (char *) memcpy (__retval, value, __len); __retval; })) : __strdup (value)));
            }
            break;
        case 'b':
            basedn = value;
            break;
        case 'f':
            searchfilter = value;
            break;
        case 'u':
            userattr = value;
            break;
        case 'U':
            passwdattr = value;
            break;
        case 's':
            if (__extension__ ({ size_t __s1_len, __s2_len; (__builtin_constant_p (value) && __builtin_constant_p ("base") && (__s1_len = __builtin_strlen (value), __s2_len = __builtin_strlen ("base"), (!((size_t)(const void *)((value) + 1) - (size_t)(const void *)(value) == 1) || __s1_len >= 4) && (!((size_t)(const void *)(("base") + 1) - (size_t)(const void *)("base") == 1) || __s2_len >= 4)) ? __builtin_strcmp (value, "base") : (__builtin_constant_p (value) && ((size_t)(const void *)((value) + 1) - (size_t)(const void *)(value) == 1) && (__s1_len = __builtin_strlen (value), __s1_len < 4) ? (__builtin_constant_p ("base") && ((size_t)(const void *)(("base") + 1) - (size_t)(const void *)("base") == 1) ? __builtin_strcmp (value, "base") : (__extension__ ({ __const unsigned char *__s2 = (__const unsigned char *) (__const char *) ("base"); register int __result = (((__const unsigned char *) (__const char *) (value))[0] - __s2[0]); if (__s1_len > 0 && __result == 0) { __result = (((__const unsigned char *) (__const char *) (value))[1] - __s2[1]); if (__s1_len > 1 && __result == 0) { __result = (((__const unsigned char *) (__const char *) (value))[2] - __s2[2]); if (__s1_len > 2 && __result == 0) __result = (((__const unsigned char *) (__const char *) (value))[3] - __s2[3]); } } __result; }))) : (__builtin_constant_p ("base") && ((size_t)(const void *)(("base") + 1) - (size_t)(const void *)("base") == 1) && (__s2_len = __builtin_strlen ("base"), __s2_len < 4) ? (__builtin_constant_p (value) && ((size_t)(const void *)((value) + 1) - (size_t)(const void *)(value) == 1) ? __builtin_strcmp (value, "base") : (__extension__ ({ __const unsigned char *__s1 = (__const unsigned char *) (__const char *) (value); register int __result = __s1[0] - ((__const unsigned char *) (__const char *) ("base"))[0]; if (__s2_len > 0 && __result == 0) { __result = (__s1[1] - ((__const unsigned char *) (__const char *) ("base"))[1]); if (__s2_len > 1 && __result == 0) { __result = (__s1[2] - ((__const unsigned char *) (__const char *) ("base"))[2]); if (__s2_len > 2 && __result == 0) __result = (__s1[3] - ((__const unsigned char *) (__const char *) ("base"))[3]); } } __result; }))) : __builtin_strcmp (value, "base")))); }) == 0)
                searchscope = ((ber_int_t) 0x0000);
            else if (__extension__ ({ size_t __s1_len, __s2_len; (__builtin_constant_p (value) && __builtin_constant_p ("one") && (__s1_len = __builtin_strlen (value), __s2_len = __builtin_strlen ("one"), (!((size_t)(const void *)((value) + 1) - (size_t)(const void *)(value) == 1) || __s1_len >= 4) && (!((size_t)(const void *)(("one") + 1) - (size_t)(const void *)("one") == 1) || __s2_len >= 4)) ? __builtin_strcmp (value, "one") : (__builtin_constant_p (value) && ((size_t)(const void *)((value) + 1) - (size_t)(const void *)(value) == 1) && (__s1_len = __builtin_strlen (value), __s1_len < 4) ? (__builtin_constant_p ("one") && ((size_t)(const void *)(("one") + 1) - (size_t)(const void *)("one") == 1) ? __builtin_strcmp (value, "one") : (__extension__ ({ __const unsigned char *__s2 = (__const unsigned char *) (__const char *) ("one"); register int __result = (((__const unsigned char *) (__const char *) (value))[0] - __s2[0]); if (__s1_len > 0 && __result == 0) { __result = (((__const unsigned char *) (__const char *) (value))[1] - __s2[1]); if (__s1_len > 1 && __result == 0) { __result = (((__const unsigned char *) (__const char *) (value))[2] - __s2[2]); if (__s1_len > 2 && __result == 0) __result = (((__const unsigned char *) (__const char *) (value))[3] - __s2[3]); } } __result; }))) : (__builtin_constant_p ("one") && ((size_t)(const void *)(("one") + 1) - (size_t)(const void *)("one") == 1) && (__s2_len = __builtin_strlen ("one"), __s2_len < 4) ? (__builtin_constant_p (value) && ((size_t)(const void *)((value) + 1) - (size_t)(const void *)(value) == 1) ? __builtin_strcmp (value, "one") : (__extension__ ({ __const unsigned char *__s1 = (__const unsigned char *) (__const char *) (value); register int __result = __s1[0] - ((__const unsigned char *) (__const char *) ("one"))[0]; if (__s2_len > 0 && __result == 0) { __result = (__s1[1] - ((__const unsigned char *) (__const char *) ("one"))[1]); if (__s2_len > 1 && __result == 0) { __result = (__s1[2] - ((__const unsigned char *) (__const char *) ("one"))[2]); if (__s2_len > 2 && __result == 0) __result = (__s1[3] - ((__const unsigned char *) (__const char *) ("one"))[3]); } } __result; }))) : __builtin_strcmp (value, "one")))); }) == 0)
                searchscope = ((ber_int_t) 0x0001);
            else if (__extension__ ({ size_t __s1_len, __s2_len; (__builtin_constant_p (value) && __builtin_constant_p ("sub") && (__s1_len = __builtin_strlen (value), __s2_len = __builtin_strlen ("sub"), (!((size_t)(const void *)((value) + 1) - (size_t)(const void *)(value) == 1) || __s1_len >= 4) && (!((size_t)(const void *)(("sub") + 1) - (size_t)(const void *)("sub") == 1) || __s2_len >= 4)) ? __builtin_strcmp (value, "sub") : (__builtin_constant_p (value) && ((size_t)(const void *)((value) + 1) - (size_t)(const void *)(value) == 1) && (__s1_len = __builtin_strlen (value), __s1_len < 4) ? (__builtin_constant_p ("sub") && ((size_t)(const void *)(("sub") + 1) - (size_t)(const void *)("sub") == 1) ? __builtin_strcmp (value, "sub") : (__extension__ ({ __const unsigned char *__s2 = (__const unsigned char *) (__const char *) ("sub"); register int __result = (((__const unsigned char *) (__const char *) (value))[0] - __s2[0]); if (__s1_len > 0 && __result == 0) { __result = (((__const unsigned char *) (__const char *) (value))[1] - __s2[1]); if (__s1_len > 1 && __result == 0) { __result = (((__const unsigned char *) (__const char *) (value))[2] - __s2[2]); if (__s1_len > 2 && __result == 0) __result = (((__const unsigned char *) (__const char *) (value))[3] - __s2[3]); } } __result; }))) : (__builtin_constant_p ("sub") && ((size_t)(const void *)(("sub") + 1) - (size_t)(const void *)("sub") == 1) && (__s2_len = __builtin_strlen ("sub"), __s2_len < 4) ? (__builtin_constant_p (value) && ((size_t)(const void *)((value) + 1) - (size_t)(const void *)(value) == 1) ? __builtin_strcmp (value, "sub") : (__extension__ ({ __const unsigned char *__s1 = (__const unsigned char *) (__const char *) (value); register int __result = __s1[0] - ((__const unsigned char *) (__const char *) ("sub"))[0]; if (__s2_len > 0 && __result == 0) { __result = (__s1[1] - ((__const unsigned char *) (__const char *) ("sub"))[1]); if (__s2_len > 1 && __result == 0) { __result = (__s1[2] - ((__const unsigned char *) (__const char *) ("sub"))[2]); if (__s2_len > 2 && __result == 0) __result = (__s1[3] - ((__const unsigned char *) (__const char *) ("sub"))[3]); } } __result; }))) : __builtin_strcmp (value, "sub")))); }) == 0)
                searchscope = ((ber_int_t) 0x0002);
            else {
                fprintf(stderr, "squid_ldap_auth" ": ERROR: Unknown search scope '%s'\n", value);
                exit(1);
            }
            break;
        case 'E':





            fprintf(stderr, "squid_ldap_auth" " ERROR: -E unsupported with this LDAP library\n");
            exit(1);

            break;
        case 'c':
            connect_timeout = atoi(value);
            break;
        case 't':
            timelimit = atoi(value);
            break;
        case 'a':
            if (__extension__ ({ size_t __s1_len, __s2_len; (__builtin_constant_p (value) && __builtin_constant_p ("never") && (__s1_len = __builtin_strlen (value), __s2_len = __builtin_strlen ("never"), (!((size_t)(const void *)((value) + 1) - (size_t)(const void *)(value) == 1) || __s1_len >= 4) && (!((size_t)(const void *)(("never") + 1) - (size_t)(const void *)("never") == 1) || __s2_len >= 4)) ? __builtin_strcmp (value, "never") : (__builtin_constant_p (value) && ((size_t)(const void *)((value) + 1) - (size_t)(const void *)(value) == 1) && (__s1_len = __builtin_strlen (value), __s1_len < 4) ? (__builtin_constant_p ("never") && ((size_t)(const void *)(("never") + 1) - (size_t)(const void *)("never") == 1) ? __builtin_strcmp (value, "never") : (__extension__ ({ __const unsigned char *__s2 = (__const unsigned char *) (__const char *) ("never"); register int __result = (((__const unsigned char *) (__const char *) (value))[0] - __s2[0]); if (__s1_len > 0 && __result == 0) { __result = (((__const unsigned char *) (__const char *) (value))[1] - __s2[1]); if (__s1_len > 1 && __result == 0) { __result = (((__const unsigned char *) (__const char *) (value))[2] - __s2[2]); if (__s1_len > 2 && __result == 0) __result = (((__const unsigned char *) (__const char *) (value))[3] - __s2[3]); } } __result; }))) : (__builtin_constant_p ("never") && ((size_t)(const void *)(("never") + 1) - (size_t)(const void *)("never") == 1) && (__s2_len = __builtin_strlen ("never"), __s2_len < 4) ? (__builtin_constant_p (value) && ((size_t)(const void *)((value) + 1) - (size_t)(const void *)(value) == 1) ? __builtin_strcmp (value, "never") : (__extension__ ({ __const unsigned char *__s1 = (__const unsigned char *) (__const char *) (value); register int __result = __s1[0] - ((__const unsigned char *) (__const char *) ("never"))[0]; if (__s2_len > 0 && __result == 0) { __result = (__s1[1] - ((__const unsigned char *) (__const char *) ("never"))[1]); if (__s2_len > 1 && __result == 0) { __result = (__s1[2] - ((__const unsigned char *) (__const char *) ("never"))[2]); if (__s2_len > 2 && __result == 0) __result = (__s1[3] - ((__const unsigned char *) (__const char *) ("never"))[3]); } } __result; }))) : __builtin_strcmp (value, "never")))); }) == 0)
                aliasderef = 0x00;
            else if (__extension__ ({ size_t __s1_len, __s2_len; (__builtin_constant_p (value) && __builtin_constant_p ("always") && (__s1_len = __builtin_strlen (value), __s2_len = __builtin_strlen ("always"), (!((size_t)(const void *)((value) + 1) - (size_t)(const void *)(value) == 1) || __s1_len >= 4) && (!((size_t)(const void *)(("always") + 1) - (size_t)(const void *)("always") == 1) || __s2_len >= 4)) ? __builtin_strcmp (value, "always") : (__builtin_constant_p (value) && ((size_t)(const void *)((value) + 1) - (size_t)(const void *)(value) == 1) && (__s1_len = __builtin_strlen (value), __s1_len < 4) ? (__builtin_constant_p ("always") && ((size_t)(const void *)(("always") + 1) - (size_t)(const void *)("always") == 1) ? __builtin_strcmp (value, "always") : (__extension__ ({ __const unsigned char *__s2 = (__const unsigned char *) (__const char *) ("always"); register int __result = (((__const unsigned char *) (__const char *) (value))[0] - __s2[0]); if (__s1_len > 0 && __result == 0) { __result = (((__const unsigned char *) (__const char *) (value))[1] - __s2[1]); if (__s1_len > 1 && __result == 0) { __result = (((__const unsigned char *) (__const char *) (value))[2] - __s2[2]); if (__s1_len > 2 && __result == 0) __result = (((__const unsigned char *) (__const char *) (value))[3] - __s2[3]); } } __result; }))) : (__builtin_constant_p ("always") && ((size_t)(const void *)(("always") + 1) - (size_t)(const void *)("always") == 1) && (__s2_len = __builtin_strlen ("always"), __s2_len < 4) ? (__builtin_constant_p (value) && ((size_t)(const void *)((value) + 1) - (size_t)(const void *)(value) == 1) ? __builtin_strcmp (value, "always") : (__extension__ ({ __const unsigned char *__s1 = (__const unsigned char *) (__const char *) (value); register int __result = __s1[0] - ((__const unsigned char *) (__const char *) ("always"))[0]; if (__s2_len > 0 && __result == 0) { __result = (__s1[1] - ((__const unsigned char *) (__const char *) ("always"))[1]); if (__s2_len > 1 && __result == 0) { __result = (__s1[2] - ((__const unsigned char *) (__const char *) ("always"))[2]); if (__s2_len > 2 && __result == 0) __result = (__s1[3] - ((__const unsigned char *) (__const char *) ("always"))[3]); } } __result; }))) : __builtin_strcmp (value, "always")))); }) == 0)
                aliasderef = 0x03;
            else if (__extension__ ({ size_t __s1_len, __s2_len; (__builtin_constant_p (value) && __builtin_constant_p ("search") && (__s1_len = __builtin_strlen (value), __s2_len = __builtin_strlen ("search"), (!((size_t)(const void *)((value) + 1) - (size_t)(const void *)(value) == 1) || __s1_len >= 4) && (!((size_t)(const void *)(("search") + 1) - (size_t)(const void *)("search") == 1) || __s2_len >= 4)) ? __builtin_strcmp (value, "search") : (__builtin_constant_p (value) && ((size_t)(const void *)((value) + 1) - (size_t)(const void *)(value) == 1) && (__s1_len = __builtin_strlen (value), __s1_len < 4) ? (__builtin_constant_p ("search") && ((size_t)(const void *)(("search") + 1) - (size_t)(const void *)("search") == 1) ? __builtin_strcmp (value, "search") : (__extension__ ({ __const unsigned char *__s2 = (__const unsigned char *) (__const char *) ("search"); register int __result = (((__const unsigned char *) (__const char *) (value))[0] - __s2[0]); if (__s1_len > 0 && __result == 0) { __result = (((__const unsigned char *) (__const char *) (value))[1] - __s2[1]); if (__s1_len > 1 && __result == 0) { __result = (((__const unsigned char *) (__const char *) (value))[2] - __s2[2]); if (__s1_len > 2 && __result == 0) __result = (((__const unsigned char *) (__const char *) (value))[3] - __s2[3]); } } __result; }))) : (__builtin_constant_p ("search") && ((size_t)(const void *)(("search") + 1) - (size_t)(const void *)("search") == 1) && (__s2_len = __builtin_strlen ("search"), __s2_len < 4) ? (__builtin_constant_p (value) && ((size_t)(const void *)((value) + 1) - (size_t)(const void *)(value) == 1) ? __builtin_strcmp (value, "search") : (__extension__ ({ __const unsigned char *__s1 = (__const unsigned char *) (__const char *) (value); register int __result = __s1[0] - ((__const unsigned char *) (__const char *) ("search"))[0]; if (__s2_len > 0 && __result == 0) { __result = (__s1[1] - ((__const unsigned char *) (__const char *) ("search"))[1]); if (__s2_len > 1 && __result == 0) { __result = (__s1[2] - ((__const unsigned char *) (__const char *) ("search"))[2]); if (__s2_len > 2 && __result == 0) __result = (__s1[3] - ((__const unsigned char *) (__const char *) ("search"))[3]); } } __result; }))) : __builtin_strcmp (value, "search")))); }) == 0)
                aliasderef = 0x01;
            else if (__extension__ ({ size_t __s1_len, __s2_len; (__builtin_constant_p (value) && __builtin_constant_p ("find") && (__s1_len = __builtin_strlen (value), __s2_len = __builtin_strlen ("find"), (!((size_t)(const void *)((value) + 1) - (size_t)(const void *)(value) == 1) || __s1_len >= 4) && (!((size_t)(const void *)(("find") + 1) - (size_t)(const void *)("find") == 1) || __s2_len >= 4)) ? __builtin_strcmp (value, "find") : (__builtin_constant_p (value) && ((size_t)(const void *)((value) + 1) - (size_t)(const void *)(value) == 1) && (__s1_len = __builtin_strlen (value), __s1_len < 4) ? (__builtin_constant_p ("find") && ((size_t)(const void *)(("find") + 1) - (size_t)(const void *)("find") == 1) ? __builtin_strcmp (value, "find") : (__extension__ ({ __const unsigned char *__s2 = (__const unsigned char *) (__const char *) ("find"); register int __result = (((__const unsigned char *) (__const char *) (value))[0] - __s2[0]); if (__s1_len > 0 && __result == 0) { __result = (((__const unsigned char *) (__const char *) (value))[1] - __s2[1]); if (__s1_len > 1 && __result == 0) { __result = (((__const unsigned char *) (__const char *) (value))[2] - __s2[2]); if (__s1_len > 2 && __result == 0) __result = (((__const unsigned char *) (__const char *) (value))[3] - __s2[3]); } } __result; }))) : (__builtin_constant_p ("find") && ((size_t)(const void *)(("find") + 1) - (size_t)(const void *)("find") == 1) && (__s2_len = __builtin_strlen ("find"), __s2_len < 4) ? (__builtin_constant_p (value) && ((size_t)(const void *)((value) + 1) - (size_t)(const void *)(value) == 1) ? __builtin_strcmp (value, "find") : (__extension__ ({ __const unsigned char *__s1 = (__const unsigned char *) (__const char *) (value); register int __result = __s1[0] - ((__const unsigned char *) (__const char *) ("find"))[0]; if (__s2_len > 0 && __result == 0) { __result = (__s1[1] - ((__const unsigned char *) (__const char *) ("find"))[1]); if (__s2_len > 1 && __result == 0) { __result = (__s1[2] - ((__const unsigned char *) (__const char *) ("find"))[2]); if (__s2_len > 2 && __result == 0) __result = (__s1[3] - ((__const unsigned char *) (__const char *) ("find"))[3]); } } __result; }))) : __builtin_strcmp (value, "find")))); }) == 0)
                aliasderef = 0x02;
            else {
                fprintf(stderr, "squid_ldap_auth" ": ERROR: Unknown alias dereference method '%s'\n", value);
                exit(1);
            }
            break;
        case 'D':
            binddn = value;
            break;
        case 'w':
            bindpasswd = value;
            break;
        case 'W':
            readSecret(value);
            break;
        case 'P':
            persistent = !persistent;
            break;
        case 'O':
            bind_once = !bind_once;
            break;
        case 'p':
            port = atoi(value);
            break;
        case 'R':
            noreferrals = !noreferrals;
            break;

        case 'v':
            switch (atoi(value)) {
            case 2:
                version = 2;
                break;
            case 3:
                version = 3;
                break;
            default:
                fprintf(stderr, "Protocol version should be 2 or 3\n");
                exit(1);
            }
            break;
        case 'Z':
            if (version == 2) {
                fprintf(stderr, "TLS (-Z) is incompatible with version %d\n",
                        version);
                exit(1);
            }
            version = 3;
            use_tls = 1;
            break;

        case 'd':
            debug++;
            break;
        default:
            fprintf(stderr, "squid_ldap_auth" ": ERROR: Unknown command line option '%c'\n", option);
            exit(1);
        }
    }

    while (argc > 1) {
        char *value = argv[1];
        if (ldapServer) {
            int len = strlen(ldapServer) + 1 + strlen(value) + 1;
            char *newhost = malloc(len);
            snprintf(newhost, len, "%s %s", ldapServer, value);
            free(ldapServer);
            ldapServer = newhost;
        } else {
            ldapServer = (__extension__ (__builtin_constant_p (value) && ((size_t)(const void *)((value) + 1) - (size_t)(const void *)(value) == 1) ? (((__const char *) (value))[0] == '\0' ? (char *) calloc ((size_t) 1, (size_t) 1) : ({ size_t __len = strlen (value) + 1; char *__retval = (char *) malloc (__len); if (__retval != ((void *)0)) __retval = (char *) memcpy (__retval, value, __len); __retval; })) : __strdup (value)));
        }
        argc--;
        argv++;
    }
    if (!ldapServer)
        ldapServer = (__extension__ (__builtin_constant_p ("localhost") && ((size_t)(const void *)(("localhost") + 1) - (size_t)(const void *)("localhost") == 1) ? (((__const char *) ("localhost"))[0] == '\0' ? (char *) calloc ((size_t) 1, (size_t) 1) : ({ size_t __len = strlen ("localhost") + 1; char *__retval = (char *) malloc (__len); if (__retval != ((void *)0)) __retval = (char *) memcpy (__retval, "localhost", __len); __retval; })) : __strdup ("localhost")));

    if (!basedn) {
        fprintf(stderr, "Usage: " "squid_ldap_auth" " -b basedn [options] [ldap_server_name[:port]]...\n\n");
        fprintf(stderr, "\t-b basedn (REQUIRED)\tbase dn under which to search\n");
        fprintf(stderr, "\t-f filter\t\tsearch filter to locate user DN\n");
        fprintf(stderr, "\t-u userattr\t\tusername DN attribute\n");
        fprintf(stderr, "\t-s base|one|sub\t\tsearch scope\n");
        fprintf(stderr, "\t-D binddn\t\tDN to bind as to perform searches\n");
        fprintf(stderr, "\t-w bindpasswd\t\tpassword for binddn\n");
        fprintf(stderr, "\t-W secretfile\t\tread password for binddn from file secretfile\n");

        fprintf(stderr, "\t-H URI\t\t\tLDAPURI (defaults to ldap://localhost)\n");

        fprintf(stderr, "\t-h server\t\tLDAP server (defaults to localhost)\n");
        fprintf(stderr, "\t-p port\t\t\tLDAP server port\n");
        fprintf(stderr, "\t-P\t\t\tpersistent LDAP connection\n");



        fprintf(stderr, "\t-c timeout\t\tconnect timeout\n");
        fprintf(stderr, "\t-t timelimit\t\tsearch time limit\n");
        fprintf(stderr, "\t-R\t\t\tdo not follow referrals\n");
        fprintf(stderr, "\t-a never|always|search|find\n\t\t\t\twhen to dereference aliases\n");

        fprintf(stderr, "\t-v 2|3\t\t\tLDAP version\n");
        fprintf(stderr, "\t-Z\t\t\tTLS encrypt the LDAP connection, requires LDAP version 3\n");

        fprintf(stderr, "\t-d\t\t\tenable debug mode\n");
        fprintf(stderr, "\n");
        fprintf(stderr, "\tIf no search filter is specified, then the dn <userattr>=user,basedn\n\twill be used (same as specifying a search filter of '<userattr>=',\n\tbut quicker as as there is no need to search for the user DN)\n\n");
        fprintf(stderr, "\tIf you need to bind as a user to perform searches then use the\n\t-D binddn -w bindpasswd or -D binddn -W secretfile options\n\n");
        exit(1);
    }
# 570 "squid_ldap_auth_and_ip.c"
    while (fgets(buf, sizeof(buf), stdin) != ((void *)0)) {
        user = strtok(buf, " \r\n");
        passwd = strtok(((void *)0), "\r\n");

        if ( user )
            if (__extension__ ({ size_t __s1_len, __s2_len; (__builtin_constant_p (user) && __builtin_constant_p ("ip") && (__s1_len = __builtin_strlen (user), __s2_len = __builtin_strlen ("ip"), (!((size_t)(const void *)((user) + 1) - (size_t)(const void *)(user) == 1) || __s1_len >= 4) && (!((size_t)(const void *)(("ip") + 1) - (size_t)(const void *)("ip") == 1) || __s2_len >= 4)) ? __builtin_strcmp (user, "ip") : (__builtin_constant_p (user) && ((size_t)(const void *)((user) + 1) - (size_t)(const void *)(user) == 1) && (__s1_len = __builtin_strlen (user), __s1_len < 4) ? (__builtin_constant_p ("ip") && ((size_t)(const void *)(("ip") + 1) - (size_t)(const void *)("ip") == 1) ? __builtin_strcmp (user, "ip") : (__extension__ ({ __const unsigned char *__s2 = (__const unsigned char *) (__const char *) ("ip"); register int __result = (((__const unsigned char *) (__const char *) (user))[0] - __s2[0]); if (__s1_len > 0 && __result == 0) { __result = (((__const unsigned char *) (__const char *) (user))[1] - __s2[1]); if (__s1_len > 1 && __result == 0) { __result = (((__const unsigned char *) (__const char *) (user))[2] - __s2[2]); if (__s1_len > 2 && __result == 0) __result = (((__const unsigned char *) (__const char *) (user))[3] - __s2[3]); } } __result; }))) : (__builtin_constant_p ("ip") && ((size_t)(const void *)(("ip") + 1) - (size_t)(const void *)("ip") == 1) && (__s2_len = __builtin_strlen ("ip"), __s2_len < 4) ? (__builtin_constant_p (user) && ((size_t)(const void *)((user) + 1) - (size_t)(const void *)(user) == 1) ? __builtin_strcmp (user, "ip") : (__extension__ ({ __const unsigned char *__s1 = (__const unsigned char *) (__const char *) (user); register int __result = __s1[0] - ((__const unsigned char *) (__const char *) ("ip"))[0]; if (__s2_len > 0 && __result == 0) { __result = (__s1[1] - ((__const unsigned char *) (__const char *) ("ip"))[1]); if (__s2_len > 1 && __result == 0) { __result = (__s1[2] - ((__const unsigned char *) (__const char *) ("ip"))[2]); if (__s2_len > 2 && __result == 0) __result = (__s1[3] - ((__const unsigned char *) (__const char *) ("ip"))[3]); } } __result; }))) : __builtin_strcmp (user, "ip")))); })==0) {
                printf("OK\n");
                continue;
            }

        if (!user || !passwd || !passwd[0]) {
            printf("ERR\n");
            continue;
        }
        rfc1738_unescape(user);
        rfc1738_unescape(passwd);
        if (!validUsername(user)) {
            printf("ERR No such user\n");
            continue;
        }
        tryagain = (ld != ((void *)0));
recover:
        if (ld == ((void *)0) && persistent)
            ld = open_ldap_connection(ldapServer, port);
        if (checkLDAP(ld, user, passwd, ldapServer, port) != 0) {
            if (tryagain && squid_ldap_errno(ld) != 0x31) {
                tryagain = 0;
                ldap_unbind(ld);
                ld = ((void *)0);
                goto recover;
            }
            printf("ERR %s\n", ldap_err2string(squid_ldap_errno(ld)));
        } else {
            printf("OK\n");
        }
        if (ld && (squid_ldap_errno(ld) != 0x00 && squid_ldap_errno(ld) != 0x31)) {
            ldap_unbind(ld);
            ld = ((void *)0);
        }
    }
    if (ld)
        ldap_unbind(ld);
    return 0;
}

static int
ldap_escape_value(char *escaped, int size, const char *src)
{
    int n = 0;
    while (size > 4 && *src) {
        switch (*src) {
        case '*':
        case '(':
        case ')':
        case '\\':
            n += 3;
            size -= 3;
            if (size > 0) {
                *escaped++ = '\\';
                snprintf(escaped, 3, "%02x", (unsigned char) *src++);
                escaped += 2;
            }
            break;
        default:
            *escaped++ = *src++;
            n++;
            size--;
        }
    }
    *escaped = '\0';
    return n;
}




static int
checkLDAP(LDAP * persistent_ld, const char *userid, const char *password, const char *ldapServer, int port)
{
    char dn[1024];
    int ret = 0;
    LDAP *bind_ld = ((void *)0);

    if (!*password) {



        if (debug)
            fprintf(stderr, "Blank password given\n");
        return 1;
    }
    if (searchfilter) {
        char filter[16384];
        char escaped_login[1024];
        LDAPMessage *res = ((void *)0);
        LDAPMessage *entry;
        char *searchattr[] = {(char *)"1.1", ((void *)0)};
        char *userdn;
        int rc;
        LDAP *search_ld = persistent_ld;

        if (!search_ld)
            search_ld = open_ldap_connection(ldapServer, port);

        ldap_escape_value(escaped_login, sizeof(escaped_login), userid);
        if (binddn) {
            rc = ldap_simple_bind_s(search_ld, binddn, bindpasswd);
            if (rc != 0x00) {
                fprintf(stderr, "squid_ldap_auth" ": WARNING, could not bind to binddn '%s'\n", ldap_err2string(rc));
                ret = 1;
                goto search_done;
            }
        }
        snprintf(filter, sizeof(filter), searchfilter, escaped_login, escaped_login, escaped_login, escaped_login, escaped_login, escaped_login, escaped_login, escaped_login, escaped_login, escaped_login, escaped_login, escaped_login, escaped_login, escaped_login, escaped_login);
        if (debug)
            fprintf(stderr, "user filter '%s', searchbase '%s'\n", filter, basedn);
        rc = ldap_search_s(search_ld, basedn, searchscope, filter, searchattr, 1, &res);
        if (rc != 0x00) {
            if (noreferrals && rc == 0x09) {



                if (debug)
                    fprintf(stderr, "noreferrals && rc == LDAP_PARTIAL_RESULTS\n");
            } else {
                fprintf(stderr, "squid_ldap_auth" ": WARNING, LDAP search error '%s'\n", ldap_err2string(rc));






                ret = 1;
                goto search_done;
            }
        }
        entry = ldap_first_entry(search_ld, res);
        if (!entry) {
            if (debug)
                fprintf(stderr, "Ldap search returned nothing\n");
            ret = 1;
            goto search_done;
        }
        userdn = ldap_get_dn(search_ld, entry);
        if (!userdn) {
            fprintf(stderr, "squid_ldap_auth" ": ERROR, could not get user DN for '%s'\n", userid);
            ret = 1;
            goto search_done;
        }
        snprintf(dn, sizeof(dn), "%s", userdn);
        squid_ldap_memfree(userdn);

        if (ret == 0 && (!binddn || !bind_once || passwdattr)) {

            bind_ld = search_ld;
            search_ld = ((void *)0);
        }
search_done:
        if (res) {
            ldap_msgfree(res);
            res = ((void *)0);
        }
        if (search_ld && search_ld != persistent_ld) {
            ldap_unbind(search_ld);
            search_ld = ((void *)0);
        }
        if (ret != 0)
            return ret;
    } else {
        snprintf(dn, sizeof(dn), "%s=%s,%s", userattr, userid, basedn);
    }

    if (debug)
        fprintf(stderr, "attempting to authenticate user '%s'\n", dn);
    if (!bind_ld && !bind_once)
        bind_ld = persistent_ld;
    if (!bind_ld)
        bind_ld = open_ldap_connection(ldapServer, port);
    if (passwdattr) {
        if (ldap_compare_s(bind_ld, dn, passwdattr, password) != 0x06) {
            ret = 1;
        }
    } else if (ldap_simple_bind_s(bind_ld, dn, password) != 0x00)
        ret = 1;
    if (bind_ld != persistent_ld) {
        ldap_unbind(bind_ld);
        bind_ld = ((void *)0);
    }
    return ret;
}

int
readSecret(const char *filename)
{
    char buf[8192];
    char *e = ((void *)0);
    FILE *f;
    char *passwd = ((void *)0);

    if (!(f = fopen(filename, "r"))) {
        fprintf(stderr, "squid_ldap_auth" " ERROR: Can not read secret file %s\n", filename);
        return 1;
    }
    if (!fgets(buf, sizeof(buf) - 1, f)) {
        fprintf(stderr, "squid_ldap_auth" " ERROR: Secret file %s is empty\n", filename);
        fclose(f);
        return 1;
    }

    if ((e = strrchr(buf, '\n')))
        *e = 0;
    if ((e = strrchr(buf, '\r')))
        *e = 0;

    passwd = (char *) calloc(sizeof(char), strlen(buf) + 1);
    if (!passwd) {
        fprintf(stderr, "squid_ldap_auth" " ERROR: can not allocate memory\n");
        exit(1);
    }
    strcpy(passwd, buf);
    bindpasswd = passwd;

    fclose(f);

    return 0;
}
