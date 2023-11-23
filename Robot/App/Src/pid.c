#include "pid.h"

void pid_init(PID_t *pid, float kp, float ki, float kd)
{
    pid->set_val = 0.0f;
    pid->actual_val = 0.0f;
    pid->err = 0.0f;
    pid->err_last = 0.0f;
    pid->err_next = 0.0f;
    pid->Kp = kp;
    pid->Ki = ki;
    pid->Kd = kd;
    pid->output = 0.0f;
    pid->integral = 0.0f;
    pid->umax = 0.0f;
    pid->umin = 0.0f;
}

/*
    PID
	pwm += Kp[e（k）-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
*/
int pid_execute(PID_t *pid, float actual_val, float set_val)
{
    pid->set_val = set_val;
	pid->actual_val = actual_val;
	pid->err = set_val - actual_val;
	pid->output += pid->Kp * (pid->err - pid->err_last) + pid->Ki * pid->err;
	pid->err_last = pid->err;
	
    if (pid->output > 16800)
        pid->output = 16800;
    if (pid->output < -16800)
        pid->output = -16800;
    return pid->output;
}