// 프로세스 수, 프로세스 ID, 도착시간, 서비스시간, 우선순위, 시간할당량

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

#include "queue/queue.h"

// struct timespec
// tv_sec, tv_nsec
// clock_gettime(CLOCK_MONOTONIC, &before);
//
// exit(EXIT_SUCCESS)
// exit(EXIT_FAILURE)

#define milliToSec 1000000
#define microToSec 1000000000

#define MIN_PRIORITY (-19)
#define MAX_PRIORITY (20)

#define BUFFER_SIZE (1000)

int _METHOD_COUNT = 0;

static char commandLine[BUFFER_SIZE];

queue* RRq;
int round_idx = 0;
int _USE_ROUND = 0;

queue q;
queue core;

int nproc;
int resol;

// index of TimeSlice
int ps_num = 0;

typedef enum SCHEDULING_METHOD {FCFS, SJF, HRN, RR, SRT}sched;
typedef enum BOOLEAN {TRUE, FALSE} boolean;

sched sch = FCFS;

// ATT, AWT
unsigned long ATT = 0L, AWT = 0L;

static unsigned long GetDeltaTime(struct timespec* start, struct timespec* end){
	return (end->tv_sec * microToSec + end->tv_nsec) - (start->tv_sec * microToSec + start->tv_nsec);
}

static unsigned long GetTime(struct timespec* time){
	return (time->tv_sec * microToSec + time->tv_nsec);
}

// push new Process
static void pushPS(queue* q, unsigned long task, int priority){
	int pid;
	unsigned long time;
	struct timespec arr;
	struct TimeSlice nts;
	
	// push new task and get pid
	pid = push(q, task, priority);
	// calculating arrival time
	clock_gettime(CLOCK_MONOTONIC, &arr);
	time = GetTime(&arr);
	
	nts.pid			= pid;
	nts.arrivalTime 	= time;
	nts.executeTime		= 0;
	nts.waitTime		= 0;
	nts.returnTime		= 0;

	// setTimeSlice(q, pid, &nts);
	setTimeSlicePid(q, pid, pid);
	setTimeSliceArrivalTime(q, pid, time);

	printf("efefef:%d %d %ld\n", pid, getTimeSlicePid(q, pid), getTimeSliceArrivalTime(q, pid));
}


// push new Process
static void pushPS_RR(queue* q, unsigned long task, int priority){
	int pid;
	unsigned long time;
	struct timespec arr;
	struct TimeSlice nts;
	
	// push new task and get pid
	pid = push(&q[priority], task, priority);
	// calculating arrival time
	clock_gettime(CLOCK_MONOTONIC, &arr);
	time = GetTime(&arr);
	
	nts.pid			= pid;
	nts.arrivalTime 	= time;
	nts.executeTime		= 0;
	nts.waitTime		= 0;
	nts.returnTime		= 0;

	setTimeSlice(&q[priority], pid, &nts);

	snprintf(commandLine, BUFFER_SIZE, "echo %d %ld >>chartthemes/purified", nts.pid, nts.arrivalTime);
	system(commandLine);
}

// work
static int Load(int pid, unsigned long task){
	queue* nq;

	if (!_USE_ROUND)
		nq = &q;
	else 
		nq = &RRq[round_idx];
	
	if (pid == -1 || task == -1){
		return -1;
	}

	int i;
	int isDone;
	struct timespec st, et;
	// 받은 t / resol 만큼 while처리 합니다
	// 추후 setTask를 통하여 남은 task를 저장하고 
	// 남은 task가 0이면 큐를 삭제하던가 말던가~
	long thread = getThread(nq, pid);
	unsigned long calcWaitTime;
	int waitLoop;

	long double test;

	// 프로세스 진행도	
	printf("\t- PID<%d> -> task amount %ld / %ld\n", pid, getTask(nq, pid), thread);
	// 프로세스를 모두 처리 했는가?
	isDone = setTask(nq, pid, thread);
	
	// 실행 시간 측정
	clock_gettime(CLOCK_MONOTONIC, &st);

	while(thread--){
	}

	clock_gettime(CLOCK_MONOTONIC, &et);

	// 실행 시간 누적
	setTimeSliceExecuteTime(nq, pid, (getTimeSliceExecuteTime(nq, pid) + GetDeltaTime(&st, &et)));
	
	// 반환시간
	struct timespec returnTime;
	clock_gettime(CLOCK_MONOTONIC, &returnTime);

	i = pid;

	// setTask -> thread만큼 task를 실행, 감소
	// 만일 끝났으면
	if (isDone){
		// 반환 시간
		setTimeSliceReturnTime(nq, pid, GetTime(&returnTime));
		// 대기 시간
		setTimeSliceWaitTime(nq, pid, GetTime(&returnTime) - getTimeSliceArrivalTime(nq, pid) - getTimeSliceExecuteTime(nq, pid));
		calcWaitTime = GetTime(&returnTime) - getTimeSliceArrivalTime(nq, pid);

		printf("\t\t> <%d> 도착: <%ld>, 실행: <%ld>, 대기: <%ld>, 반환: <%ld>\n\n",\
			       	i, getTimeSliceArrivalTime(nq, pid), getTimeSliceExecuteTime(nq, pid), \
				getTimeSliceWaitTime(nq, pid), getTimeSliceReturnTime(nq, pid));
		
		snprintf(commandLine, BUFFER_SIZE, "echo %d %ld %ld %ld >>chartthemes/purified",\
				i, getTimeSliceExecuteTime(nq, pid), \
				getTimeSliceWaitTime(nq, pid), getTimeSliceReturnTime(nq, pid));
			
		system(commandLine);

		AWT += getTimeSliceExecuteTime(nq, pid);
		ATT += getTimeSliceReturnTime(nq, pid);

		// 모든 프로세스의 wait에 returnTime을 더한다.
		for (waitLoop = 0; waitLoop < q.rear; waitLoop++){
			if (q.PID[waitLoop] != pid){
				q.timeSlice[waitLoop].waitTime += calcWaitTime;
			}
		}

		delete(nq, pid);

		return 1;
	}
	// 처리할 테스크가 존재 한다면
	else{
		// 대기 시간
		setTimeSliceWaitTime(nq, pid, GetTime(&returnTime) - getTimeSliceArrivalTime(nq, pid) - getTimeSliceExecuteTime(nq, pid));
		
		printf("\t\t> leaf_task_amount %ld\n", getTask(nq, pid));
	}

	return 0;
}

static void child(){
	// int stop = 0;
	int result;
	
	int i;
	int Ncore, PID, ps_num;	
	unsigned long task;

	int loop = 0;

	//RR variable
	int restPS = 0;
	int taskNum;

	int hrnIdx;
	
	// divid task time into resolution
	if (!_USE_ROUND)
		ps_num = setThread(&q, resol);
	else
		for (i = 0; i<40; i++){
			ps_num += setThread(&RRq[i], resol);
		}
	
	// sort process as defined scheduler method
	switch (sch){
		// first in first out
		case FCFS:
			break;
		// small task first
		case SJF:
			minSortQueue(&q);
			break;
		// highest respone ratio next
		case HRN:
			break;

		default:
			puts("Please Select CPU Scheduler\n");
			fflush(NULL);

			break;
	}
	
	// 일반 프로세스 전용
	if (!_USE_ROUND){
		// 하나의 큐에서 루프
		while(pop(&q) != -1)
		{
			loop++;
			Ncore = pop(&core);

			printf("Ncore<%d>\n", Ncore);

			if (sch == HRN){
				// 여기서 hrn이 가장 큰 프로세스를 골라서 진행 시킨다.
				hrnIdx = hrnSortQueue(&q);

				PID = hrnIdx;
				task = getTask(&q, PID);

				if (task == -1)
					break;
			}
			else{
				PID = getPID(&q);
				task = getTask(&q, PID);
			}

			Load(PID, task);
		}
	}
	// 라운드로빈 프로세스 전용
	else{
		restPS = 0;
		// 라운드 로빈용 루프
		while(1){
			// 라운드 돌기
			for(round_idx = 0; round_idx < 40; round_idx++){
				// 만약 SRT이면 minSortQueue를 강행합니다
				if (sch == SRT){
					minSortQueue(&RRq[round_idx]);
				}
				// 한 큐에 해당하는 테스크 개수
				taskNum = RRq[round_idx].rear;

				printf("Round<%d> Ncore<%d>\n", round_idx - 20, Ncore);

				// 처리할 프로세스 만큼 처리
				for(i = 0; i < taskNum; i++){
					loop++;
						
					Ncore = pop(&core);

					PID = getPID(&RRq[round_idx]);
					task = getTask(&RRq[round_idx], PID);
					
					result = Load(PID, task);
	
					// 작업이 남아있다면 0을 리턴
					if(result == 0){
						restPS++;
					}
	
					RRq[round_idx].top++;
				}
	
				core.top++;
				printf("\n\n");
			}
			
			// 모든 라운드를 돌았다면
			// 일이 남았다면 
			if (restPS){
				restPS = 0;
			}
			// 모든 일이 끝이 났다면
			else
				break;
		}
	}
	
	ATT /= ps_num;
	AWT /= ps_num;

	printf("<%d> Process processed!\n", ps_num);	
	printf("<%d> per Loop\n", loop);

	printf("ATT: <%ld> AWT: <%ld> ps_num<%d>\n", ATT, AWT, ps_num);
	
	snprintf(commandLine, BUFFER_SIZE, "echo ATT %ld AWT %ld >>chartthemes/purified", ATT, AWT);
	printf("%s\n", commandLine);
	system(commandLine);

	exit(EXIT_SUCCESS);
}

static void parent(){
	wait(NULL);
}

int main(long argc, char* args[]){
	unsigned long task;
	int PID;

	system("echo >chartthemes/purified");

	// core routine
	int i;
	// RR용
	RRq = mmap(NULL, sizeof(queue) * 40, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

	if (RRq == (void*) -1)
		err(EXIT_FAILURE, "RRq 생성 실패! (mmap())\n");
	
	// get option
	if (argc < 3){
		puts("main <nproc><resol>\n");
		fflush(NULL);

		exit(EXIT_FAILURE);
	}
	
	nproc = atoi(args[1]);
	resol = atoi(args[2]);

	if (nproc < 1){
		puts("You should get nproc values 1 or more.\n");
		fflush(NULL);
		
		exit(EXIT_FAILURE);
	}
	if (resol < 1){
		puts("You should get resolution values 1 or more.\n");
		fflush(NULL);

		exit(EXIT_FAILURE);
	}

	// coreQueue
	init(&core);

	// declare core Queue
	for (i = 0; i < nproc; i++)
		// will build priority
		push(&core, i, 0);
	
	for (_METHOD_COUNT = 0; _METHOD_COUNT < 5; _METHOD_COUNT++)
	{
		sch = _METHOD_COUNT;
		
		printf("method %d\n", sch);

		snprintf(commandLine, BUFFER_SIZE, "echo method %d >>chartthemes/purified", sch);
		system(commandLine);

		// 예제 큐를 초기화	
		// 스케쥴링에 따라서 프로세스를 정렬합니다
		switch (sch){
		case FCFS:
			printf("FCFS\n");
			break;

		case SJF:
			printf("SJF\n");
			minSortQueue(&q);
			break;

		case HRN:
			printf("HRN\n");
			break;

		case RR:
			printf("RR\n");
			_USE_ROUND = 1;
			break;

		case SRT:
			printf("SRT\n");
			_USE_ROUND = 1;
			break;
		}

		// 예제 큐를 초기화	
		if (!_USE_ROUND){
			// init readyQueue
			init(&q);
		
			// declare readyQueue
			// insert Task
			// will build priority
			pushPS(&q, 110000000UL, 0);
			pushPS(&q, 91000000UL, 1);
			pushPS(&q, 70000000UL, 3);
			pushPS(&q, 500000000UL, 2);
			pushPS(&q, 230000000UL, 5);
			pushPS(&q, 100000000UL, 4);
			pushPS(&q, 9000000UL, 8);
			pushPS(&q, 990000000UL, 6);
			pushPS(&q, 1100000UL, 7);

			printQueue(&q);
			
			snprintf(commandLine, BUFFER_SIZE, "echo %d >>chartthemes/purified", q.rear-1);
			system(commandLine);

			for (i = 0; i < q.rear; i++){
				snprintf(commandLine, BUFFER_SIZE, "echo info %d %ld %ld %d >>chartthemes/purified", \
						q.PID[i], q.task[q.PID[i]], getTimeSliceArrivalTime(&q, getTimeSlicePid(&q, i)), q.priority[q.PID[i]]);
				system(commandLine);
			}

		}else{
			if (RRq->rear < 2)
			{
				// push Process in RR queue
				pushPS(RRq, 110000000UL, 0);
				pushPS(RRq, 91000000UL, 1);
				pushPS(RRq, 70000000UL, 2);
				pushPS(RRq, 500000000UL, 3);
				pushPS(RRq, 230000000UL, 4);
				pushPS(RRq, 100000000UL, 5);
				pushPS(RRq, 9000000UL, 6);
				pushPS(RRq, 990000000UL, 7);
				pushPS(RRq, 1100000UL, 8);
			}

			snprintf(commandLine, BUFFER_SIZE, "echo %d >>chartthemes/purified", 8);
			system(commandLine);

			for (i = 0; i < 8; i++){
				snprintf(commandLine, BUFFER_SIZE, "echo info %d %ld >>chartthemes/purified", \
						i, getTimeSliceArrivalTime(&q, getTimeSlicePid(&q, i)));
				system(commandLine);
			}
		}


		// 예제 큐를 초기화	
		// 스케쥴링에 따라서 프로세스를 정렬합니다
		switch (sch){
		case FCFS:
			printf("FCFS\n");
			break;

		case SJF:
			printf("SJF\n");
			minSortQueue(&q);
			break;

		case HRN:
			printf("HRN\n");
			break;

		case RR:
			printf("RR\n");
			_USE_ROUND = 1;
			break;

		case SRT:
			printf("SRT\n");
			_USE_ROUND = 1;
			break;
		}

		// 예제 큐를 초기화	
		PID = fork();
		
		if (PID < 0){
			puts("Can't created or duplicated new process\n");
			fflush(NULL);
			
			exit(EXIT_FAILURE);
		}
		else if (PID == 0){
			child();
		}
		else{
			parent();
		}

	}
		
	// free
	// free(RRq);
	if (munmap(RRq, (sizeof(queue) * 40)) == -1)
		err(EXIT_FAILURE, "RRq 동적할당 해제 실패\n");

	exit(EXIT_SUCCESS);


_NEGATIVE_TASK_EXCEPTION:
	puts("테스크의 잘못된 값이나 오버플로로인해 프로그램을 종료합니다.\n");
	fflush(NULL);

	// free
	free(RRq);

	exit(EXIT_FAILURE);
}


