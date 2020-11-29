#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum PID_STATE{IDLE, RUN, STOP, JOMBIE }state;

static unsigned long executeTime;

typedef struct TimeSlice{
	int pid;

	long arrivalTime;
	long waitTime;
	long executeTime;
	long returnTime;
}TimeSlice;

// 큐 정의
typedef struct Queue
{
	int PID[30];
	unsigned long task[30];
	unsigned long thread[30];
	short priority[30];
	enum PID_STATE state[30];
	
	// 큐 도착 시간
	struct timespec arrival_time[30];
	// 시간 정보 저장
	struct TimeSlice timeSlice[30];

	int top;
	int front;
	int rear;
}queue;

void init(queue* q){
	int i = 0;
	q->top = -1;
	q->front = 0;
	q->rear = 0;

	for (i; i < 30; i++){
		q->thread[i] = -1;
	}
}

int isFull(queue* q){
	if (q->rear == 99){
		return 1;
	}
	return 0;
}

int isEmpty(queue* q){
	if (q->front == q->rear){
		return 1;
	}
	return 0;
}

int push(queue* q, int task, int priority){
	if (isFull(q)){
		return -1;
	}

	// 큐 도착 시간 저장 
	clock_gettime(CLOCK_MONOTONIC, &q->arrival_time[q->rear]);
	
	q->PID[q->rear] = q->rear; 
	q->task[q->rear] = task;
	q->thread[q->rear] = -1;
	q->state[q->rear] = IDLE;
	q->priority[q->rear++] = priority;

	return q->rear - 1;
}

int pop(queue* q){
	if (isEmpty(q)){
		return -1;
	}

	q->top = ++q->top % q->rear;
	return q->PID[q->top];
}

int isTail(queue* q){
	if ((q->top + 1) == q->rear)
		return 1;
	return 0;
}

int getPID(queue* q){
	if (isEmpty(q)){
		return -1;
	}

	return q->PID[q->top];
}

long getTask(queue* q, int PID){
	int i, idx = -1;
	if (isEmpty(q)){
		return -1;
	}
	
	for (i = q->front; i<q->rear; i++){
		if (PID == q->PID[i])
			idx = i;
	}
	
	if (idx == -1)
		return -1;

	return q->task[idx];
}

long getThread(queue* q, int PID){
	int i, idx = -1;
	if (isEmpty(q)){
		return -1;
	}
	
	for (i = q->front; i<q->rear; i++){
		if (PID == q->PID[i]){
			idx = i;
			break;
		}
	}
	
	if (idx == -1)
		return -1;

	if (q->task[idx] >= q->thread[idx])
		return q->thread[idx];
	else
		return q->task[idx];
}

struct timespec* getArrTime(queue* q, int PID){
	int i, idx = -1;
	if (isEmpty(q)){
		return (struct timespec*) NULL;
	}

	for (i = q->front; i<q->rear; i++)
		if (PID == q->PID[i])
			idx = i;

	if (idx == -1)
		return (struct timespec*) NULL;

	return &(q->arrival_time[idx]);
}

struct TimeSlice* getTimeSlice(queue* q, int PID){
	int i, idx = -1;
	if (isEmpty(q)){
		return (struct TimeSlice*) NULL;
	}

	for (i = q->front; i<q->rear; i++)
		if (PID == q->PID[i])
			idx = i;

	if (idx == -1)
		return (struct TimeSlice*) NULL;

	return &(q->timeSlice[idx]);
}


int getTimeSlicePid(queue* q, int PID){
	int i, idx = -1;
	if (isEmpty(q)){
		return -1;
	}

	for (i = q->front; i<q->rear; i++)
		if (PID == q->PID[i])
			idx = i;

	if (idx == -1)
		return -1;

	return q->timeSlice[idx].pid;
}


unsigned long getTimeSliceArrivalTime(queue* q, int PID){
	int i, idx = -1;
	if (isEmpty(q)){
		return 0;
	}

	for (i = q->front; i<q->rear; i++)
		if (PID == q->PID[i])
			idx = i;

	if (idx == -1)
		return 0;

	return q->timeSlice[idx].arrivalTime;
}


unsigned long getTimeSliceExecuteTime(queue* q, int PID){
	int i, idx = -1;
	if (isEmpty(q)){
		return 0;
	}

	for (i = q->front; i<q->rear; i++)
		if (PID == q->PID[i])
			idx = i;

	if (idx == -1)
		return 0;

	return q->timeSlice[idx].executeTime;
}


unsigned long getTimeSliceWaitTime(queue* q, int PID){
	int i, idx = -1;
	if (isEmpty(q)){
		return 0;
	}

	for (i = q->front; i<q->rear; i++)
		if (PID == q->PID[i])
			idx = i;

	if (idx == -1)
		return 0;

	return q->timeSlice[idx].waitTime;
}


unsigned long getTimeSliceReturnTime(queue* q, int PID){
	int i, idx = -1;
	if (isEmpty(q)){
		return 0;
	}

	for (i = q->front; i<q->rear; i++)
		if (PID == q->PID[i])
			idx = i;

	if (idx == -1)
		return 0;

	return q->timeSlice[idx].returnTime;
}

// 테스크가 끝나면 1 작업이 남아 있다면 0 리턴
int setTask(queue* q, int PID, long tasked){
	int i, idx = -1;
	if (isEmpty(q)){
		return -1;
	}
	
	for (i = q->front; i<q->rear; i++)
		if (PID == q->PID[i])
			idx = i;
	
	if (idx == -1)
		return -1;
	
	q->task[idx] -= tasked;

        if (q->task[idx] > 0) 	
		return 0;
	else
		return 1;
}

int setThread(queue* q, int resol){
	int i, ps_num = 0;
	if (isEmpty(q)){
		return 0;
	}

	for (i = q->front; i < q->rear; i++){
		ps_num++;
		if (q->thread[i] == -1){
			q->thread[i] = q->task[i] / resol;
		}
	}
	return ps_num;
}

int setTimeSlice(queue* q, int PID, struct TimeSlice* ts){
	int i, idx = -1;
	if (isEmpty(q)){
		return -1;
	}

	for (i = q->front; i<=q->rear; i++){
		if (PID == q->PID[i]){
			idx = i;
		}
	}

	if (idx == -1)
		return -1;
	
	// set
	q->timeSlice[idx].pid 		= ts->pid;
	q->timeSlice[idx].arrivalTime 	= ts->arrivalTime;
	q->timeSlice[idx].executeTime 	= ts->executeTime;
	q->timeSlice[idx].waitTime 	= ts->waitTime;
	q->timeSlice[idx].returnTime 	= ts->returnTime;
}

int setTimeSlicePid(queue* q, int PID, int pid){
	int i, idx = -1;
	if (isEmpty(q)){
		return -1;
	}

	for (i = q->front; i<q->rear; i++)
		if (PID == q->PID[i])
			idx = i;

	if (idx == -1)
		return -1;
	
	// set
	q->timeSlice[idx].pid 		= pid;
}

int setTimeSliceArrivalTime(queue* q, int PID, unsigned long arr){
	int i, idx = -1;
	if (isEmpty(q)){
		return -1;
	}

	for (i = q->front; i<q->rear; i++)
		if (PID == q->PID[i])
			idx = i;

	if (idx == -1)
		return -1;

	// set
	q->timeSlice[idx].arrivalTime 	= arr;
}

int setTimeSliceExecuteTime(queue* q, int PID, unsigned long exe){
	int i, idx = -1;
	
	if (isEmpty(q)){
		return -1;
	}

	for (i = q->front; i<q->rear; i++)
		if (PID == q->PID[i])
			idx = i;

	if (idx == -1)
		return -1;
	
	// set
	q->timeSlice[idx].executeTime 	= exe;
}

int setTimeSliceWaitTime(queue* q, int PID, unsigned long wait){
	int i, idx = -1;
	if (isEmpty(q)){
		return -1;
	}

	for (i = q->front; i<q->rear; i++)
		if (PID == q->PID[i])
			idx = i;

	if (idx == -1)
		return -1;
	
	// set
	q->timeSlice[idx].waitTime	= wait;
}

int setTimeSliceReturnTime(queue* q, int PID, unsigned long ret){
	int i, idx = -1;
	if (isEmpty(q)){
		return -1;
	}

	for (i = q->front; i<q->rear; i++)
		if (PID == q->PID[i])
			idx = i;

	if (idx == -1)
		return -1;
	
	// set
	q->timeSlice[idx].returnTime 	= ret;
}

enum PID_STATE getState(queue* q, int PID){
	int i, idx = -1;
	if (isEmpty(q)){
		return -1;
	}
	
	for (i = q->front; i<q->rear; i++)
		if (PID == q->PID[i])
			idx = i;
	
	if (idx == -1)
		return -1;

	return q->state[idx];
}

void delete(queue* q, int del){
	int i = q->front;
	int delIdx = -1;
	for (i; i < q->rear; i++){
		if (q->PID[i] == del){
			delIdx = i;
			break;
		}
	}

	if (delIdx == -1)
		return;

	for (delIdx; delIdx < q->rear-1; delIdx++){
		q->PID[delIdx] = q->PID[delIdx+1];
		q->task[delIdx] = q->task[delIdx+1];
		q->thread[delIdx] = q->thread[delIdx+1];
		q->state[delIdx] = q->state[delIdx+1];

	}
	--q->rear;

	q->top = q->front-1;
}


void printQueue(queue* q){
	int i = q->front;
	for (i; i < q->rear; i++)
		printf("<%d>%ld (%ld) -> \n", q->PID[i], q->task[i], q->arrival_time[i].tv_sec * 1000000000 + q->arrival_time[i].tv_nsec);

	printf("\n");
}

void minSortQueue(queue* q){
	if (isEmpty(q)){
		return;
	}

	int i = q->front, j;
	int min, minIdx;
	int isTemp = 0;
	int temp;
	struct timespec st;
	struct TimeSlice ts;

	for (i; i < q->rear; i++){
		min = q->task[i];
		minIdx = -1;
		for (j = i+1; j<q->rear; j++){
			if (min > q->task[j]){
				min = q->task[j];
				minIdx = j;
			}
		}
	
		if (minIdx != -1){
			temp = q->PID[i];
			q->PID[i] = q->PID[minIdx];
			q->PID[minIdx] = temp;

			temp = q->task[i];
			q->task[i] = q->task[minIdx];
			q->task[minIdx] = temp;

			temp = q->thread[i];
			q->thread[i] = q->thread[minIdx];
			q->thread[minIdx] = temp;

			st = q->arrival_time[i];
			q->arrival_time[i] = q->arrival_time[minIdx];
			q->arrival_time[minIdx] = st;

			ts = q->timeSlice[i];
			q->timeSlice[i] = q->timeSlice[minIdx];
			q->timeSlice[minIdx] = ts;
		}

	}
}

void maxSortQueue(queue* q){
	if (isEmpty(q)){
		return;
	}

	int i = q->front, j;
	int max, maxIdx;
	int isTemp = 0;
	int temp;
	struct timespec st;
	struct TimeSlice ts;

	for (i; i < q->rear; i++){
		max = q->task[i];
		maxIdx = -1;
		for (j = i+1; j<q->rear; j++){
			if (max < q->task[j]){
				max = q->task[j];
				maxIdx = j;
			}
		}
	
		if (maxIdx != -1){
			temp = q->PID[i];
			q->PID[i] = q->PID[maxIdx];
			q->PID[maxIdx] = temp;

			temp = q->task[i];
			q->task[i] = q->task[maxIdx];
			q->task[maxIdx] = temp;

			temp = q->thread[i];
			q->thread[i] = q->thread[maxIdx];
			q->thread[maxIdx] = temp;

			st = q->arrival_time[i];
			q->arrival_time[i] = q->arrival_time[maxIdx];
			q->arrival_time[maxIdx] = st;

			ts = q->timeSlice[i];
			q->timeSlice[i] = q->timeSlice[maxIdx];
			q->timeSlice[maxIdx] = ts;
		}
	}
}

int hrnSortQueue(queue* q){
	if (isEmpty(q)){
		return -1;
	}
	int i, j;
	double hrn;

	unsigned long max = 0;
	int maxIdx = -1;
	
	for (i = 0; i < q->rear; i++)
	{
		if (q->task[i] > 0)
			hrn = (q->timeSlice[i].waitTime + q->task[i]) / q->task[i];
		else 
			hrn = 1.0f;

		if (max < hrn)
		{
			max = hrn;
			maxIdx = i;
		}
	}
	
	return q->PID[maxIdx];
}

// 큐 정의 종료
