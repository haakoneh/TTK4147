#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include "fifo.h"

dispatch_t              *dpp;
resmgr_attr_t           resmgr_attr;
dispatch_context_t      *ctp;
resmgr_connect_funcs_t  connect_funcs;
resmgr_io_funcs_t       io_funcs;
iofunc_attr_t           io_attr;

int io_read(resmgr_context_t *ctp, io_read_t *msg, iofunc_ocb_t *ocb);
int io_write(resmgr_context_t *ctp, io_write_t *msg, RESMGR_OCB_T *ocb);
void *counter_thread();
char buf[] = "Hello World\n";
char buffer[255];


int counter;

int incrementer = 0;

void error(char *s)
{
	perror(s);
	exit(EXIT_FAILURE);
}

pthread_mutex_t io_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;
fifo_t queue;
int main(int argc, char *argv[]) {
	printf("Start resource manager\n");

	// create dispatch.
	if (!(dpp = dispatch_create()))
		error("Create dispatch");

	pthread_t counting_thread;
	pthread_create(&counting_thread,NULL,counter_thread,NULL);

	init_fifo(&queue);

	// initialize resource manager attributes.
	memset(&resmgr_attr, 0, sizeof(resmgr_attr));
	resmgr_attr.nparts_max = 1;
	resmgr_attr.msg_max_size = 2048;

	// set standard connect and io functions.
	iofunc_func_init(_RESMGR_CONNECT_NFUNCS, &connect_funcs, _RESMGR_IO_NFUNCS, &io_funcs);
	iofunc_attr_init(&io_attr, S_IFNAM | 0666, 0, 0);

	// override functions
	io_funcs.read = io_read;
	io_funcs.write = io_write;

	//pthread_mutex_init()

	// establish resource manager
	if (resmgr_attach(dpp, &resmgr_attr, "/dev/myresource", _FTYPE_ANY, 0, &connect_funcs, &io_funcs, &io_attr) < 0)
		error("Resmanager attach");
	ctp = dispatch_context_alloc(dpp);

	// wait forever, handling messages.
	while(1)
	{
		if (!(ctp = dispatch_block(ctp)))
			error("Dispatch block");
		dispatch_handler(ctp);
	}

	exit(EXIT_SUCCESS);
}

int io_read(resmgr_context_t *ctp, io_read_t *msg, iofunc_ocb_t *ocb)
{
	int nonblock;
	pthread_mutex_lock(&io_mutex);
	char read_buffer[255];
	if(!ocb->offset)
	{
		if(fifo_status(&queue)){
		fifo_rem_string(&queue, read_buffer);

		// set data to return
		SETIOV(ctp->iov, read_buffer, strlen(read_buffer));
		_IO_SET_READ_NBYTES(ctp, strlen(read_buffer));

		// increase the offset (new reads will not get the same data)
		ocb->offset = 1;

		pthread_mutex_unlock(&io_mutex);
		// return
		return (_RESMGR_NPARTS(1));
		}else{
			iofunc_read_verify(ctp,msg,ocb,&nonblock);
			if(nonblock){
				return (EAGAIN);
			}else{
				//client wants to wait
				fifo_add_blocked_id(&queue, ctp->rcvid);
				return (_RESMGR_NOREPLY);
			}


		}
	}
	else
	{
		// set to return no data
		_IO_SET_READ_NBYTES(ctp, 0);

		pthread_mutex_unlock(&io_mutex);
		// return
		return (_RESMGR_NPARTS(0));
	}
}

int io_write(resmgr_context_t *ctp, io_write_t *msg, RESMGR_OCB_T *ocb) {
	pthread_mutex_lock(&io_mutex);
	memset(buffer,0,strlen(buffer));
	_IO_SET_WRITE_NBYTES(ctp, msg->i.nbytes);

	resmgr_msgread(ctp, buffer, msg->i.nbytes, sizeof(msg->i));

	pthread_mutex_unlock(&io_mutex);
	int blocked_id = -1;
	printf ("Received %d bytes = '%s'\n", msg -> i.nbytes, buffer);
	if(!fifi_is_full(&queue)) {
		blocked_id = fifo_rem_blocked_id(&queue);
		if(blocked_id != -1){
			MsgReply(blocked_id,0,buffer,strlen(buffer));
		}else{
		fifo_add_string(&queue, buffer);
		}
	}
	if(strncmp(buffer,"up",2) == 0 ){
		incrementer = 1;
	}else if(strncmp(buffer,"down",4) == 0){
		incrementer = -1;
	}else if(strncmp(buffer,"stop",4) == 0){
		incrementer = 0;
	}

	return (_RESMGR_NPARTS(0));

}

void *counter_thread(){
	counter = 0;
	while(1){
		pthread_mutex_lock(&counter_mutex);
		counter+=incrementer;
		pthread_mutex_unlock(&counter_mutex);
		//printf("Counter Value: %i\n ",counter);
		delay(100);
	}
}
