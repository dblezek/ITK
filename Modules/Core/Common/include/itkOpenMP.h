#if defined(_OPENMP)
#include <omp.h>
#else

#include <stdio.h>
#include <stdlib.h>
#ifdef __cplusplus
extern "C" {
#endif

  typedef int omp_lock_t;

  void omp_init_lock(omp_lock_t *lock);
  void omp_destroy_lock(omp_lock_t *lock);
  void omp_set_lock(omp_lock_t *lock);
  void omp_unset_lock(omp_lock_t *lock);
  int omp_test_lock(omp_lock_t *lock);

  void omp_set_num_threads(int num_threads);
  int omp_get_num_threads(void);
  int omp_get_max_threads(void);
  int omp_get_thread_num(void);
  int omp_get_num_procs(void);
  void omp_set_dynamic(int dynamic_threads);
  int omp_get_dynamic(void);
  int omp_in_parallel(void);
  void omp_set_nested(int nested);
  int omp_get_nested(void);

#ifndef OMP_NEST_LOCK_T
  typedef struct { /* This really belongs in omp.h */
    int owner;
    int count;
  } omp_nest_lock_t;
#endif

  void omp_init_nest_lock(omp_nest_lock_t *nlock);
  void omp_destroy_nest_lock(omp_nest_lock_t *nlock);
  void omp_set_nest_lock(omp_nest_lock_t *nlock);
  void omp_unset_nest_lock(omp_nest_lock_t *nlock);
  int omp_test_nest_lock(omp_nest_lock_t *nlock);
  double omp_get_wtime(void);
  double omp_get_wtick(void);
#ifdef __cplusplus
}
#endif


#endif
