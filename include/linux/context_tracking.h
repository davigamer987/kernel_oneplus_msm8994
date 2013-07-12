#ifndef _LINUX_CONTEXT_TRACKING_H
#define _LINUX_CONTEXT_TRACKING_H

#include <linux/sched.h>
#include <linux/vtime.h>
#include <linux/context_tracking_state.h>
#include <asm/ptrace.h>


#ifdef CONFIG_CONTEXT_TRACKING
extern void context_tracking_cpu_set(int cpu);

extern void guest_enter(void);
extern void guest_exit(void);

static inline enum ctx_state exception_enter(void)
{
	enum ctx_state prev_ctx;

	prev_ctx = this_cpu_read(context_tracking.state);
	user_exit();

	return prev_ctx;
}

static inline void exception_exit(enum ctx_state prev_ctx)
{
	if (prev_ctx == IN_USER)
		user_enter();
}

extern void context_tracking_task_switch(struct task_struct *prev,
					 struct task_struct *next);
#else
static inline void user_enter(void) { }
static inline void user_exit(void) { }

static inline void guest_enter(void)
{
	__guest_enter();
}

static inline void guest_exit(void)
{
	__guest_exit();
}

static inline enum ctx_state exception_enter(void) { return 0; }
static inline void exception_exit(enum ctx_state prev_ctx) { }
static inline void context_tracking_task_switch(struct task_struct *prev,
						struct task_struct *next) { }
#endif /* !CONFIG_CONTEXT_TRACKING */

#endif
