
#include "task_microros.h"
//

#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <uxr/client/transport.h>
#include <rmw_microxrcedds_c/config.h>
#include <rmw_microros/rmw_microros.h>
#include <std_msgs/msg/int32.h>

#include "usart.h"

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

static void microros_entry(void *param)
{
	osDelay(5000); // Wait for ROS agent to open

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
		// u3_printf("rcutils_set_default_allocator error (line %d)\r\n", __LINE__);
	}
	else
	{
		// u3_printf("rcutils_set_default_allocator success.\r\n");
	}

	// micro-ROS app

	rcl_publisher_t publisher = rcl_get_zero_initialized_publisher();
	std_msgs__msg__Int32 msg;
	rclc_support_t support;
	rcl_allocator_t allocator;
	rcl_node_t node;
	rcl_ret_t rc;
	allocator = rcl_get_default_allocator();
	if (allocator.allocate == NULL || allocator.deallocate == NULL)
	{
		// u3_printf("rcl_get_default_allocator error (line %d)\r\n", __LINE__);
	}
	else
	{
		// u3_printf("rcl_get_default_allocator success\r\n");
	}

	// create init_options
	rc = rclc_support_init(&support, 0, NULL, &allocator);
	if (rc != RCL_RET_OK)
	{
		// u3_printf("rclc_support_init error. (line %d)\r\n", __LINE__);
		rclc_support_fini(&support);
	}
	else
	{
		// u3_printf("rclc_support_init success.\r\n");
	}

	// create node
	rclc_node_init_default(&node, "cubemx_node", "", &support);

	// create publisher
	rclc_publisher_init_default(
		&publisher,
		&node,
		ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
		"cubemx_publisher");

	msg.data = 0;

	while (1)
	{
		rcl_ret_t ret = rcl_publish(&publisher, &msg, NULL);
		if (ret != RCL_RET_OK)
		{
			// u3_printf("Error publishing (line %d)\r\n", __LINE__);
		}
		else
		{
			HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin); // normal running
		}
		msg.data++;

		osDelay(500);
	}
}

void create_microros_thread(void)
{
	microrosTaskHandle = osThreadNew(microros_entry, NULL, &microrosTask_attributes);
	if (microrosTaskHandle == NULL)
	{
		// 创建失败
		// printf("Failed to create microros Task.\r\n");
		return;
	}
}