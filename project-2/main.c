
/**
 * @file main.c
 * @author Siddhant Jajoo and Satya Mehta
 * @brief This is a multithreaded C code which records temperature and light values at regular intervals
 * of time using a synchronized I2C and sends it to the logger thread using message queues to log into a
 * text file. A socket thread is also created to send data to a remote host on a socket request.  
 * @version 0.1
 * @date 2019-03-28
 * 
 * @copyright Copyright (c) 2019
 * 
 */

//#include <string.h>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "main.h"
#include "light.h"
#include "temp.h"
#include "logger.h"
#include "sockets.h"
#include "my_signal.h"
#include "queue.h"
#include "gpio.h"
#include "timer.h"
#include "nrf.h"

//Global Variables
pthread_t my_thread[4];
pthread_attr_t my_attributes;
volatile static uint32_t temp_hb_value;
volatile static uint32_t light_hb_value;
volatile static uint32_t logger_hb_value;
int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		printf("ERROR: Wrong number of parameters.\n");
		printf("Input first parameter = name of log file; second parameter = log level: 'info' or 'warning' or 'error' or 'debug'.\n");
		exit(EXIT_FAILURE);
	}

	if ((strcmp(argv[1], "info") == 0) || (strcmp(argv[1], "warning") == 0) || (strcmp(argv[1], "error") == 0) || (strcmp(argv[1], "debug") == 0))
	{
		printf("ERROR: Filename not valid ; Choose another filename, cannot choose filename same as log levels.\n");
		exit(EXIT_FAILURE);
	}

	if ((strcmp(argv[2], "info") != 0) && (strcmp(argv[2], "warning") != 0) && (strcmp(argv[2], "error") != 0) && (strcmp(argv[2], "debug") != 0))
	{
		printf("ERROR: Invalid Log Level; Valid Log Levels: 'info' or 'warning' or 'error' or 'debug'.\n");
		exit(EXIT_FAILURE);
	}

	filename = argv[1];
	if (!(strcmp(argv[2], "info")))
	{
		g_ll = INFO;
	}
	else if (!(strcmp(argv[2], "debug")))
	{
		g_ll = DEBUG;
	}
	else if (!(strcmp(argv[2], "warning")))
	{
		g_ll = WARNING;
	}
	else if (!(strcmp(argv[2], "error")))
	{
		g_ll = ERROR;
	}

	// //Initializing global variables
	main_exit = 0;
	err_t res;
	uint16_t rcv;
	spi_init();
	int ret;
	nrf_init();
	// gpio_init(LED1);
	// gpio_init(LED2);
	// gpio_init(LED3);
	// gpio_init(LED4);
	//Initializing queues
	res = queue_init();
	if (!res)
	{
		printf("BIST: Queue initialization successful.\n");
		msg_log("BIST: Queue initialization successful.\n", DEBUG, P0);
	}
	else
	{
		//gpio_ctrl(GPIO53, GPIO53_V, 1);
	}

	//Initializing Signal handler
	res = sig_init();
	if (!res)
	{
		printf("Signal initialization successful.\n");
		msg_log("Signal initialization successful.\n", DEBUG, P0);
	}
	//Initializing Mutexes
	res = mutex_init();
	if (!res)
	{
		printf("BIST: Mutex Initalization successful.\n");
		msg_log("BIST: Mutex initialization successful.\n", DEBUG, P0);
	}
	else
	{
		//gpio_ctrl(GPIO53, GPIO53_V, 1);
	}

	//Deleting previous logfile
	if (remove(filename))
	{
		error_log("ERROR: remove(); cannot delete log file", ERROR_DEBUG, P2);
	}
	else
	{
		//gpio_ctrl(GPIO53, GPIO53_V, 1);
	}

	//Creating threads
	res = create_threads(filename);
	if (!res)
	{
		msg_log("BIST: Thread Creation successful.\n", DEBUG, P0);
		//gpio_ctrl(GPIO53, GPIO53_V, 0);
	}
	else
	{
		//gpio_ctrl(GPIO53, GPIO53_V, 1);
	}
	//Initializing Timer
	//timer_init(TIMER_HB);
	msg_log("Reached main while loop.\n", DEBUG, P0);

	while (!main_exit)
	{
		hb_handle(hb_receive());
	}
	msg_log("Exiting Main while loop.\n", DEBUG, P0);
	msg_log("Destroying all initialized elements.\n", DEBUG, P0);

	destroy_all();

	for (int i = 0; i < 2; i++)
	{
		pthread_join(my_thread[i], NULL);
	}
	close(spi_fd);
	return OK;
}

/**
 * @brief - This thread will wait for the NRF data, process it and transmit
 * 
 * @param filename - This is the textfile name that is passed to the thread. This is obtained as a 
 * 					command line argument.
 * @return void* 
 */
void *nrf_thread(void *filename)
{
	msg_log("Entered NRF Thread.\n", DEBUG, P0);
	//timer_init(TIMER_TEMP);
	int ret;
	interrupt();
	while (1)
	{
		usleep(10);
		nrf_configure_mode(mode_rx);
		ret = gpio_poll();
		if (ret == 2)
		{
			rx_packet();
		}

	}
}

/**
 * @brief - This thread receives data from all the threads using message queues and prints the data to a 
 * 			textfile.
 * 
 * @param filename - This is the textfile name that is passed to the thread. This is obtained as a 
 * 					command line argument.
 * @return void* 
 */
void *logger_thread(void *filename)
{
	msg_log("Entered Logger Thread.\n", DEBUG, P0);
	while (1)
	{
		usleep(1);
		log_data(queue_receive(log_mq));
		hb_send(LOGGER_HB);
	}
}

/**
 * @brief - Create 4 different threads.
 * 
 * @param filename - This is the textfile name that is passed to the thread. This is obtained as a 
 * 					command line argument.
 * @return err_t - Returns error value. (0 for success).
 */
err_t create_threads(char *filename)
{
	if (pthread_create(&my_thread[0],		   // pointer to thread descriptor
					   (void *)&my_attributes, // use default attributes
					   nrf_thread,			   // thread function entry point
					   (void *)0))			   // parameters to pass in
	{

		perror("ERROR: pthread_create(); in create_threads function, temp_thread not created");
		/*Closing all the previous resources and freeing memory uptil failure*/
		mq_close(heartbeat_mq);
		mq_unlink(HEARTBEAT_QUEUE);
		mq_close(log_mq);
		mq_unlink(LOG_QUEUE);
		pthread_mutex_destroy(&mutex_a);
		pthread_mutex_destroy(&mutex_b);
		pthread_mutex_destroy(&mutex_error);
		exit(EXIT_FAILURE);
	}

	if (pthread_create(&my_thread[1],		   // pointer to thread descriptor
					   (void *)&my_attributes, // use default attributes
					   logger_thread,		   // thread function entry point
					   (void *)filename))	  // parameters to pass in

	{
		perror("ERROR: pthread_create(); in create_threads function, logger_thread not created");
		/*Closing all the previous resources and freeing memory uptil failure*/
		mq_close(heartbeat_mq);
		mq_unlink(HEARTBEAT_QUEUE);
		mq_close(log_mq);
		mq_unlink(LOG_QUEUE);
		pthread_mutex_destroy(&mutex_a);
		pthread_mutex_destroy(&mutex_b);
		pthread_mutex_destroy(&mutex_error);
		pthread_cancel(my_thread[0]);
		pthread_cancel(my_thread[1]);
		exit(EXIT_FAILURE);
	}
	return OK;
}

/**
 * @brief - This function initializes all the mutexes
 * 
 * @return err_t 
 */
err_t mutex_init(void)
{
	if (pthread_mutex_init(&mutex_a, NULL))
	{
		perror("ERROR: pthread_mutex_init(mutex_a); mutex_a not created");
		/*Closing all the previous resources and freeing memory uptil failure*/
		mq_close(heartbeat_mq);
		mq_unlink(HEARTBEAT_QUEUE);
		mq_close(log_mq);
		mq_unlink(LOG_QUEUE);
		exit(EXIT_FAILURE);
	}

	if (pthread_mutex_init(&mutex_b, NULL))
	{
		perror("ERROR: pthread_mutex_init(mutex_b); mutex_b not created");
		/*Closing all the previous resources and freeing memory uptil failure*/
		mq_close(heartbeat_mq);
		mq_unlink(HEARTBEAT_QUEUE);
		mq_close(log_mq);
		mq_unlink(LOG_QUEUE);
		exit(EXIT_FAILURE);
		pthread_mutex_destroy(&mutex_a);
		exit(EXIT_FAILURE);
	}

	if (pthread_mutex_init(&mutex_error, NULL))
	{
		perror("ERROR: pthread_mutex_init(mutex_error); mutex_error not created");
		/*Closing all the previous resources and freeing memory uptil failure*/
		mq_close(heartbeat_mq);
		mq_unlink(HEARTBEAT_QUEUE);
		mq_close(log_mq);
		mq_unlink(LOG_QUEUE);
		exit(EXIT_FAILURE);
		pthread_mutex_destroy(&mutex_a);
		pthread_mutex_destroy(&mutex_b);
		exit(EXIT_FAILURE);
	}

	return OK;
}

/**
 * @brief - This function reads the error values i.e errno and stores it in a local structure.
 * 
 * @param error_str - The error string that needs to be printed in the text file.
 * @return sensor_struct 
 */
sensor_struct read_error(char *error_str)
{
	sensor_struct read_data;
	read_data.id = ERROR_RCV_ID;

	if (clock_gettime(CLOCK_REALTIME, &read_data.sensor_data.temp_data.data_time))
	{
		error_log("ERROR: clock_gettime(); in read_temp_data() function", ERROR_DEBUG, P2);
	}

	//Errno is thread safe , no mutex required.
	read_data.sensor_data.error_data.error_value = errno;

	strcpy(read_data.sensor_data.error_data.error_str, error_str);

	return read_data;
}

/**
 * @brief - This function stores the string that needs to be printed in the text file in a structure.
 * 
 * @param msg_str - The string that needs to be printed in the text file.
 * @return sensor_struct 
 */
sensor_struct read_msg(char *msg_str)
{
	sensor_struct read_data;
	read_data.id = MSG_RCV_ID;

	strcpy(read_data.sensor_data.msg_data.msg_str, msg_str);

	return read_data;
}

/**
 * @brief - This function sends the error string to be printed in the textfile to the logger thread via
 * 			message queue and prints it in the textile. 
 * 
 * @param error_str - The error string that needs to be printed in the textfile.
 * @param loglevel - The loglevel of the message. Loglevels : INFO, WARNING, ERROR, DEBUG, INFO_DEBUG, ERROR_DEBUG, INFO_ERROR_DEBUG
 * @param prio - Th priorty of the message. Values can be : PO,P1,P2
 */
void error_log(char *error_str, uint8_t loglevel, uint8_t prio)
{
	queue_send(log_mq, read_error(error_str), loglevel, prio);
}

/**
 * @brief - This function sends the message string to be printed in the textfile to the logger thread via
 * 			message queue and prints it in the textile. 
 * 
 * @param str - The message string that needs to be printed in the textfile.
 * @param loglevel - The loglevel of the message. Loglevels : INFO, WARNING, ERROR, DEBUG, INFO_DEBUG, ERROR_DEBUG, INFO_ERROR_DEBUG
 * @param prio - Th priorty of the message . Values can be : PO,P1,P2
 */
void msg_log(char *str, uint8_t loglevel, uint8_t prio)
{
	queue_send(log_mq, read_msg(str), loglevel, prio);
}

/**
 * @brief - This function sends heartbeat to the main loop.
 * 
 * @param hb_value - This value specifies the heartbeat is from which thread. 
 * The values can be : 	TEMP_HB
 * 						LIGHT_HB
 * 						LOGGER_HB
 * 						SOCKET_HB
 * 						CLEAR_HB
 */
void hb_send(uint8_t hb_value)
{
	ssize_t res;
	res = mq_send(heartbeat_mq, (char *)&hb_value, sizeof(uint8_t), 0);
	if (res == -1)
	{
		error_log("ERROR: mq_send(); in queue_send() function", ERROR_DEBUG, P2);
	}
}

/**
 * @brief - This function reads the value received from the messgae queue.
 * 
 * @return uint8_t - Returns the value signifying from which thread it has received the value.
 */
uint8_t hb_receive(void)
{
	ssize_t res;
	uint8_t hb_rcv;
	res = mq_receive(heartbeat_mq, (char *)&hb_rcv, sizeof(sensor_struct), NULL);
	if (res == -1)
	{
		error_log("ERROR: mq_receive(); in queue_receive() function", ERROR_DEBUG, P2);
	}
	return hb_rcv;
}

/**
 * @brief - This function handles the action after receiving the value from the hb_receive() function.
 * 
 * @param hb_rcv - This value is returned from the hb_receive() function.
 */

void hb_handle(uint8_t hb_rcv)
{
	switch (hb_rcv)
	{
	case TEMP_HB:
	{
		temp_hb_value++;
		msg_log("Temperaure Heartbeat received.\n", DEBUG, P0);
		break;
	}

	case LIGHT_HB:
	{
		light_hb_value++;
		msg_log("Light Heartbeat received.\n", DEBUG, P0);
		break;
	}

	case LOGGER_HB:
	{
		logger_hb_value++;
		//msg_log("Logger Heartbeat received.\n", DEBUG, P0);
		break;
	}

	case CLEAR_HB:
	{
		if (temp_hb_value == 0)
		{
			if (pthread_cancel(my_thread[0]))
			{
				error_log("ERROR: pthread_cancel(0); in hb_handle() function", ERROR_DEBUG, P2);
			}
			else
			{
				msg_log("Stopping temperature thread.\n", DEBUG, P0);
			}

			if (pthread_create(&my_thread[0],		   // pointer to thread descriptor
							   (void *)&my_attributes, // use default attributes
							   nrf_thread,			   // thread function entry point
							   (void *)0))			   // parameters to pass in
			{
				error_log("ERROR: pthread_create(); in create_threads function, temp_thread not created", ERROR_DEBUG, P2);
			}
			else
			{
				msg_log("Resetting temperature thread.\n", DEBUG, P0);
			}
		}
		else
		{
			temp_hb_value = 0;
		}

		if (light_hb_value == 0)
		{
			if (pthread_cancel(my_thread[1]))
			{
				error_log("ERROR: pthread_cancel(1); in hb_handle() function", ERROR_DEBUG, P2);
			}
			else
			{
				msg_log("Stopping light thread.\n", DEBUG, P0);
			}
		}
		else
		{
			light_hb_value = 0;
		}

		if (logger_hb_value == 0)
		{
			if (pthread_cancel(my_thread[2]))
			{
				error_log("ERROR: pthread_cancel(2); in hb_handle() function", ERROR_DEBUG, P2);
			}
			else
			{
				msg_log("Stopping logger thread.\n", DEBUG, P0);
			}

			if (pthread_create(&my_thread[2],		   // pointer to thread descriptor
							   (void *)&my_attributes, // use default attributes
							   logger_thread,		   // thread function entry point
							   (void *)0))			   // parameters to pass in
			{
				error_log("ERROR: pthread_create(); in create_threads function, temp_thread not created", ERROR_DEBUG, P2);
			}
			else
			{
				msg_log("Resetting logger thread.\n", DEBUG, P0);
			}
		}
		msg_log("Clearing all heartbeat values.\n", DEBUG, P0);
		break;
	}
	}
}

/**
 * @brief - This function destroys all the mutexes created at the start.
 * 
 * @return err_t 
 */
err_t mutex_destroy(void)
{
	if (pthread_mutex_destroy(&mutex_a))
	{
		perror("ERROR: pthread_mutex_destroy(mutex_a); cannot destroy mutex_a");
	}

	if (pthread_mutex_destroy(&mutex_b))
	{
		perror("ERROR: pthread_mutex_destroy(mutex_b); cannot destroy mutex_b");
	}

	if (pthread_mutex_destroy(&mutex_error))
	{
		perror("ERROR: pthread_mutex_destroy(mutex_error); cannot destroy mutex_error");
	}

	return OK;
}

err_t thread_destroy(void)
{
	if (pthread_cancel(my_thread[0]))
	{
		perror("ERROR: pthread_cancel(0); in thread_destroy() function");
	}

	if (pthread_cancel(my_thread[1]))
	{
		perror("ERROR: pthread_cancel(1); in thread_destroy() function");
	}
}

err_t destroy_all(void)
{
	thread_destroy();
	//timer_del();
	mutex_destroy();
	queues_close();
	queues_unlink();

	FILE *fptr = fopen(filename, "a");
	fprintf(fptr, "Terminating gracefully due to signal.\n");
	printf("\nTerminating gracefully due to signal\n");
	fclose(fptr);
	return OK;
}