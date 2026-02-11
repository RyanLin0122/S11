
#ifdef TASK_DS_INSTANCE
#define EXTERN
#else
#define EXTERN extern
#endif


#define TASK_IDX_MAIN 5
#define TASK_IDX_FTL  6
#define TASK_IDX_FLA  7

EXTERN void mainTask();
EXTERN void ftlTask();
EXTERN void flaTask();


