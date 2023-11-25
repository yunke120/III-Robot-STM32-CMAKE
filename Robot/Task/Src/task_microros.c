
#include "task_microros.h"
#include "main.h"
#include "usart.h"
#include "utils.h"
#include <stdio.h>

#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <uxr/client/transport.h>
#include <rmw_microxrcedds_c/config.h>
#include <rmw_microros/rmw_microros.h>
#include <std_msgs/msg/byte_multi_array.h>




#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){printf("Failed status on line %d: %d. Aborting.\r\n",__LINE__,(int)temp_rc); return 1;}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){printf("Failed status on line %d: %d. Continuing.\r\n",__LINE__,(int)temp_rc);}}
#define RCRECHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){printf("Failed status on line %d: %d. Return.\r\n",__LINE__,(int)temp_rc); return;}}

#define SEND_MSG_SIZE	10	
#define RECV_MSG_SIZE	10	

extern bool agent_init_flag;

extern uint32_t g_RobotVelocity;
extern uint32_t g_RobotVoltage;
osMutexId_t robotVelocityMutex;
osMutexId_t robotVoltageMutex;
const osMutexAttr_t robotVelocityMutex_attributes = {
	.name = "robotVelocityMutex"};
const osMutexAttr_t robotVoltageMutex_attributes = {
	.name = "robotVoltageMutex"};


static rclc_support_t support;
static rcl_allocator_t allocator;
static rcl_publisher_t publisher;
static rcl_subscription_t subscriber;
std_msgs__msg__ByteMultiArray recv_msg_array;
std_msgs__msg__ByteMultiArray send_msg_array;
uint8_t send_buf[SEND_MSG_SIZE];
uint8_t recv_buf[RECV_MSG_SIZE];
/*
#define ROSIDL_RUNTIME_C__PRIMITIVE_SEQUENCE(STRUCT_NAME, TYPE_NAME) \
  typedef struct rosidl_runtime_c__ ## STRUCT_NAME ## __Sequence \
  { \
    TYPE_NAME * data; 
    size_t size; 
    size_t capacity; 
  } rosidl_runtime_c__ ## STRUCT_NAME ## __Sequence;
*/

static rcl_ret_t microros_init(void);

osThreadId_t microrosTaskHandle;
uint32_t microrosTaskBuffer[3000];
StaticTask_t microrosTaskControlBlock;
const osThreadAttr_t microrosTask_attributes = {
	.name = "microros",
	.cb_mem = &microrosTaskControlBlock,
	.cb_size = sizeof(microrosTaskControlBlock),
	.stack_mem = &microrosTaskBuffer[0],
	.stack_size = sizeof(microrosTaskBuffer),
	.priority = (osPriority_t)osPriorityNormal,
};

bool cubemx_transport_open(struct uxrCustomTransport *transport);
bool cubemx_transport_close(struct uxrCustomTransport *transport);
size_t cubemx_transport_write(struct uxrCustomTransport *transport, const uint8_t *buf, size_t len, uint8_t *err);
size_t cubemx_transport_read(struct uxrCustomTransport *transport, uint8_t *buf, size_t len, int timeout, uint8_t *err);

void *microros_allocate(size_t size, void *state);
void microros_deallocate(void *pointer, void *state);
void *microros_reallocate(void *pointer, size_t size, void *state);
void *microros_zero_allocate(size_t number_of_elements, size_t size_of_element, void *state);

void timer_callback(rcl_timer_t * timer, int64_t last_call_time)
{
    (void) last_call_time;
    if (NULL != timer) {

		osMutexAcquire(robotVelocityMutex, portMAX_DELAY);
		send_msg_array.data.data[0] = g_RobotVelocity;
		osMutexRelease(robotVelocityMutex);
		osMutexAcquire(robotVoltageMutex, portMAX_DELAY);
		send_msg_array.data.data[1] = g_RobotVoltage;
		osMutexRelease(robotVoltageMutex);
		send_msg_array.data.data[2] = 3;
		send_msg_array.data.data[3] = 4;
		send_msg_array.data.data[4] = 5;
		send_msg_array.data.data[5] = 6;
		send_msg_array.data.data[6] = 7;
		send_msg_array.data.data[7] = 8;
		send_msg_array.data.data[8] = 9;
		send_msg_array.data.data[9] = 10;
		send_msg_array.data.size = 10;
		if(rcl_publish(&publisher, &send_msg_array, NULL) == RCL_RET_OK)	// publish message to RaspberryPi
		{
			HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);	// normal running
		}
		else
		{
			app_printf("Error publishing message!\r\n");
		}
    }
	
}

void subscription_cb(const void *param)
{
	const std_msgs__msg__ByteMultiArray * msg = (const std_msgs__msg__ByteMultiArray *)param;
	int size = msg->data.size;
	app_printf("size = %d --> [", size);
	for(int i = 0; i < size; i++)
	{
		app_printf("%d ", msg->data.data[i]);
	}
	app_printf("]\r\n");
}


static void microros_entry(void *param)
{
	rcl_node_t node;
    rcl_timer_t timer;
	rclc_executor_t executor;

	if(microros_init() == RCL_RET_OK)	/* Initialize the Microros environment */
	{
		/**
		 * if agent_init_flag is true, start other tasks.
		*/
		agent_init_flag = true;
		printf("****************************\r\n");
		printf("**** MicroROS Success ******\r\n");
	}
	else
	{
		agent_init_flag = false; 
		printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!\r\n");
		printf("**** MicroROS Failed ******\r\n");
		return;
	}

	// create node
	RCRECHECK(rclc_node_init_default(&node, "stm32", "", &support));

	// create publisher
	RCRECHECK(rclc_publisher_init_default(
		&publisher,
		&node,
		ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, ByteMultiArray),
		"/stm32/send"));
	
	// create timer 
	const unsigned int timer_timeout = 500;
	RCRECHECK(rclc_timer_init_default(
		&timer,
		&support,
		RCL_MS_TO_NS(timer_timeout),
		timer_callback));

	// create subscriber 
	RCRECHECK(rclc_subscription_init_default(
		&subscriber,
		&node,
		ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, ByteMultiArray),
		"/stm32/recv"));

	// Initialize send_msg_array
	send_msg_array.layout.data_offset = 0;
	send_msg_array.layout.dim.size = 0;
	send_msg_array.layout.dim.capacity = SEND_MSG_SIZE;
	send_msg_array.data.capacity = SEND_MSG_SIZE;
	send_msg_array.data.data = send_buf;
	send_msg_array.data.size = 0;
	// Initialize recv_msg_array
	recv_msg_array.layout.data_offset = 0;
	recv_msg_array.layout.dim.size = 0;
	recv_msg_array.layout.dim.capacity = RECV_MSG_SIZE;
	recv_msg_array.data.capacity = RECV_MSG_SIZE;
	recv_msg_array.data.data = recv_buf;
	recv_msg_array.data.size = 0;

	// create executor
	RCRECHECK(rclc_executor_init(&executor, &support.context, 2, &allocator));
	RCRECHECK(rclc_executor_add_timer(&executor, &timer));
	RCRECHECK(rclc_executor_add_subscription(&executor, &subscriber, &recv_msg_array, &subscription_cb, ON_NEW_DATA));
	
	while(1)
	{
		// Spin executor
		rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100));
		osDelay(10);
	}
}

void create_microros_thread(void)
{
	robotVelocityMutex = osMutexNew(&robotVelocityMutex_attributes);
	if(robotVelocityMutex == NULL)
	{
		printf("Failed to create robotVelocityMutex!\r\n");
		return;
	}
	robotVoltageMutex = osMutexNew(&robotVoltageMutex_attributes);
	if(robotVoltageMutex == NULL)
	{
		printf("Failed to create robotVoltageMutex!\r\n");
		return;
	}

	microrosTaskHandle = osThreadNew(microros_entry, NULL, &microrosTask_attributes);
	if (microrosTaskHandle == NULL)
	{
		printf("Failed to create microros Task.\r\n");
	}
}



rcl_ret_t microros_init(void)
{
	rcl_init_options_t init_options;

	rmw_uros_set_custom_transport(
		true,
		(void *)&huart1,
		cubemx_transport_open,
		cubemx_transport_close,
		cubemx_transport_write,
		cubemx_transport_read);

	rcl_allocator_t freeRTOS_allocator = rcutils_get_zero_initialized_allocator();
	freeRTOS_allocator.allocate = microros_allocate;
	freeRTOS_allocator.deallocate = microros_deallocate;
	freeRTOS_allocator.reallocate = microros_reallocate;
	freeRTOS_allocator.zero_allocate = microros_zero_allocate;

	if (!rcutils_set_default_allocator(&freeRTOS_allocator))
	{
		printf("rcutils_set_default_allocator error (line %d)\r\n", __LINE__);
        return RCL_RET_ERROR;
	}

	// micro-ROS app
	allocator = rcl_get_default_allocator();
	if (allocator.allocate == NULL || allocator.deallocate == NULL)
	{
		printf("rcl_get_default_allocator error (line %d)\r\n", __LINE__);
        return RCL_RET_ERROR;
	}

	// set ROS_DOMAIN_ID
	init_options = rcl_get_zero_initialized_init_options();
	RCCHECK(rcl_init_options_init(&init_options, allocator));
	RCCHECK(rcl_init_options_set_domain_id(&init_options, 0));

	rmw_init_options_t* rmw_options = rcl_init_options_get_rmw_init_options(&init_options);
	// Auto discover micro-ROS agent
	while(rmw_uros_ping_agent_options(1000, 5, rmw_options) != RMW_RET_OK){
		printf("Please, start your micro-ROS Agent first\r\n");
	}

	// create init_options
	RCCHECK(rclc_support_init_with_options(&support, 0, NULL, &init_options, &allocator));

    return RCL_RET_OK;
}

