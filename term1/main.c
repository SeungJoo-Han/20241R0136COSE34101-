#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

// scheduling algorithm waiting, turnaround time, CPU Utilization
int Waiting[6] = { 0, };
int Turnaround[6] = { 0, };
int Utilization[6] = { 0, };

// structure of the process
typedef struct {
	int PID;
	int	CPU_Time;
	int IO_Time;
	int Arrival_Time;
	int Priority;
	int IO_Start;
	int Age;
} Process;

// structure of the node, queue
typedef struct{
	Process* data;
	struct Node* next;
} Node;

typedef struct {
	Node* head;
	Node* tail;
} Queue;

Queue* Create_Queue() {
	Queue* newQueue = (Queue*)malloc(sizeof(Queue));
	newQueue->head = NULL;
	newQueue->tail = NULL;
	return newQueue;
}

void Enqueue(Queue* queue, Process* process) {
	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->data = process;
	newNode->next = NULL;
	if (queue->head == NULL) {
		queue->head = newNode;
		queue->tail = newNode;
	}
	else {
		queue->tail->next = newNode;
		queue->tail = newNode;
	}
}

Process* Dequeue(Queue* queue) {
	if (queue->head == NULL) {
		return NULL;
	}
	Node* temp = queue->head;
	Process* process = temp->data;
	queue->head = queue->head->next;
	free(temp);
	return process;
}

void Free_Queue(Queue* queue) {
	Node* temp = queue->head;
	while (temp != NULL) {
		Node* next = temp->next;
		free(temp);
		temp = next;
	}
	free(queue);
}

// compare arrival time
int compare(const void* a, const void* b) {
	Process* A = *(Process**)a;
	Process* B = *(Process**)b;
	return A->Arrival_Time - B->Arrival_Time;
}

// create process with random values
Process* Create_Proces(int pid) {
	Process* newProcess = (Process*)malloc(sizeof(Process));
	newProcess->PID = pid;
	newProcess->CPU_Time = rand() % 100 + 1;
	newProcess->IO_Time = rand() % 100 + 1;
	newProcess->Arrival_Time = rand() % 100;
	newProcess->Priority = rand() % 10 + 1;
	newProcess->IO_Start = rand() % newProcess->CPU_Time + 1;
	newProcess->Age = 0;

	return newProcess;
}

// create process * num
Process** Config(int num) {
	Process** processes = (Process**)malloc(sizeof(Process*) * num);

	for (int i = 0; i < num; i++) {
		processes[i] = Create_Proces(i);
	}
	//sort arrival time by ascending
	qsort(processes, num, sizeof(Process*), compare);
	
	return processes;
}

// duplicate processes
Process** DupProcess(Process** processes, int num) {
	Process** p = (Process**)malloc(sizeof(Process*) * num);
	for (int i = 0; i < num; i++) {
		p[i] = (Process*)malloc(sizeof(Process));
		p[i]->PID = processes[i]->PID;
		p[i]->CPU_Time = processes[i]->CPU_Time;
		p[i]->IO_Time = processes[i]->IO_Time;
		p[i]->Arrival_Time = processes[i]->Arrival_Time;
		p[i]->Priority = processes[i]->Priority;
		p[i]->IO_Start = processes[i]->IO_Start;
		p[i]->Age = processes[i]->Age;
	}
	return p;
}

// process time
typedef struct {
	int startTime;
	int endTime;
	int PID;
} PTime;

// Add process start, end time
int AddPTime(int start, int end, int pid, int i, PTime** arr) {
	if (i == 0 && start > 0) {							// it means idle
		arr[i] = (PTime*)malloc(sizeof(PTime));
		arr[i]->startTime = 0;
		arr[i]->endTime = start;
		arr[i]->PID = -1;
		i++;
	}
	else if (i != 0 && arr[i - 1]->endTime != start) {			// idle
		arr[i] = (PTime*)malloc(sizeof(PTime));
		arr[i]->startTime = arr[i - 1]->endTime;
		arr[i]->endTime = start;
		arr[i]->PID = -1;
		i++;
	}
	if (start < end) {
		arr[i] = (PTime*)malloc(sizeof(PTime));
		arr[i]->startTime = start;
		arr[i]->endTime = end;
		arr[i]->PID = pid;
		i++;
	}
	return i;
}

// Print GanttChart
void GanttChart(PTime** arr, int size) {
	int n = size / 15;
	for (int j = 0; j < n; j++) {
		printf("+");
		for (int i = j * 15; i < (j + 1) * 15; i++) {
			printf("------+");
		}
		printf("\n");

		for (int i = j * 15; i < (j + 1) * 15; i++) {
			if (arr[i]->PID == -1) {
				printf("| IDLE ");
			}
			else {
				printf("|  %d   ", arr[i]->PID);
			}
		}
		printf("|");
		printf("\n");

		printf("+");
		for (int i = j * 15; i < (j + 1) * 15; i++) {
			printf("------+");
		}
		printf("\n");

		for (int i = j * 15; i < (j + 1) * 15; i++) {
			if (arr[i]->startTime < 10) {
				printf("%d      ", arr[i]->startTime);
			}
			else if (arr[i]->startTime < 100) {
				printf("%d     ", arr[i]->startTime);
			}
			else {
				printf("%d    ", arr[i]->startTime);
			}
		}
		if (size != (j + 1) * 15) {
			printf("%d\n\n", arr[(j + 1) * 15]->startTime);
		}
		else {
			printf("%d\n\n", arr[size - 1]->endTime);
		}
	}

	if (size % 15 != 0) {
		printf("+");
		for (int i = n * 15; i < size; i++) {
			printf("------+");
		}
		printf("\n");

		for (int i = n * 15; i < size; i++) {
			if (arr[i]->PID == -1) {
				printf("| IDLE ");
			}
			else {
				printf("|  %d   ", arr[i]->PID);
			}
		}
		printf("|");
		printf("\n");

		printf("+");
		for (int i = n * 15; i < size; i++) {
			printf("------+");
		}
		printf("\n");

		for (int i = n * 15; i < size; i++) {
			if (arr[i]->startTime < 10) {
				printf("%d      ", arr[i]->startTime);
			}
			else if (arr[i]->startTime < 100) {
				printf("%d     ", arr[i]->startTime);
			}
			else {
				printf("%d    ", arr[i]->startTime);
			}
		}
		printf("%d\n\n", arr[size - 1]->endTime);
	}
}

// Evaluate Waiting time, Turnaround time, CPU Utilization
void Evaluation(PTime** arr, int size, Process** processes, int process_end[], int IO[], int num, int AID) {
	int eval_wait[10] = { 0, };
	int eval_end[10] = { 0, };
	int idle = 0;
	int cpu_end = 0;

	for (int i = 0; i < size; i++) {
		if (arr[i]->PID == -1) {
			idle += arr[i]->endTime - arr[i]->startTime;
			continue;
		}
		eval_wait[arr[i]->PID] += arr[i]->startTime - eval_end[arr[i]->PID];
		eval_end[arr[i]->PID] = arr[i]->endTime;
		if (i == size - 1) {
			cpu_end = arr[i]->endTime;
		}
	}

	for (int i = 0; i < num; i++) {
		eval_wait[processes[i]->PID] -= processes[i]->Arrival_Time;
		eval_wait[processes[i]->PID] -= IO[processes[i]->PID];
		process_end[processes[i]->PID] -= processes[i]->Arrival_Time;
	}

	int wait = 0;
	int end = 0;
	printf("Average waiting time : (");
	for (int i = 0; i < num; i++) {
		if (i == num - 1) {
			printf(" %d ) / ", eval_wait[i]);
		}
		else {
			printf(" %d +", eval_wait[i]);
		}
		wait += eval_wait[i];
	}
	Waiting[AID] = wait / num;
	printf("%d = %d\n", num, Waiting[AID]);

	printf("Average turnaround time : (");
	for (int i = 0; i < num; i++) {
		if (i == num - 1) {
			printf(" %d ) / ", process_end[i]);
		}
		else {
			printf(" %d +", process_end[i]);
		}
		end += process_end[i];
	}
	Turnaround[AID] = end / num;
	printf("%d = %d\n", num, Turnaround[AID]);


	Utilization[AID] = (cpu_end - idle) * 100 / cpu_end;
	printf("CPU Utilization : %d%%\n", Utilization[AID]);

	for (int i = 0; i < size; i++) {
		free(arr[i]);
	}
}

// FCFS scheduling
void FCFS(Queue* readyQueue, Queue* waitingQueue, Process** processes, int num) {
	int time = 0;
	int i = 0;
	Process* CPUProcess = NULL;
	Process* IOProcess = NULL;
	Process** p = DupProcess(processes, num);
	PTime* t[1000] = { 0, };
	int *process_end = (int*)malloc(sizeof(int) * num);
	int* IO = (int*)malloc(sizeof(int) * num);
	int start = 0;
	int j = 0;
	int AID = 0; // algorithm id

	while (1) {
		if (i < num && p[i]->Arrival_Time == time) {
			Enqueue(readyQueue, p[i]);
			if (p[i]->IO_Start == p[i]->CPU_Time) {
				IO[p[i]->PID] = 0;
			}
			else {
				IO[p[i]->PID] = p[i]->IO_Time;
			}
			i++;
			continue;
		}

		if (readyQueue->head != NULL && CPUProcess == NULL) {
			CPUProcess = Dequeue(readyQueue);
			start = time;
		}
		if (CPUProcess != NULL && CPUProcess->IO_Start == 0 && CPUProcess->IO_Time > 0) {
			Enqueue(waitingQueue, CPUProcess);
			j = AddPTime(start, time, CPUProcess->PID, j, t);
			CPUProcess = NULL;
			continue;
		}
		if (waitingQueue->head != NULL && IOProcess == NULL) {
			IOProcess = Dequeue(waitingQueue);
		}
		time++;

		//IO
		if (IOProcess != NULL){
			IOProcess->IO_Time--;
			if (IOProcess->IO_Time == 0) {
				if (IOProcess->CPU_Time != 0) {
					Enqueue(readyQueue, IOProcess);
				}
				else if (IOProcess->CPU_Time == 0) {
					process_end[IOProcess->PID] = time;
				}
				IOProcess = NULL;
			}
		}

		//CPU
		if (CPUProcess != NULL){
			CPUProcess->CPU_Time--;
			CPUProcess->IO_Start--;
		}

		if (CPUProcess != NULL && CPUProcess->CPU_Time == 0) {
			if (CPUProcess->IO_Time != 0) {
				Enqueue(waitingQueue, CPUProcess);
			}
			else if (CPUProcess->IO_Time == 0) {
				process_end[CPUProcess->PID] = time;
			}
			j = AddPTime(start, time, CPUProcess->PID, j, t);
			CPUProcess = NULL;
		}

		if (i == num && readyQueue->head == NULL && CPUProcess == NULL && waitingQueue->head == NULL && IOProcess == NULL) {
			break;
		}
	}
	printf("FCFS Scheduling\n");
	GanttChart(t, j);
	Evaluation(t, j, p, process_end , IO, num, AID);
	printf("\n");

	for (int i = 0; i < num; i++) {
		free(p[i]);
	}
	free(p);
	free(process_end);
	free(IO);
}

// find shortest job and pop
Process* PopSJ(Queue* queue) {
	Process* shortest = NULL;
	Node* temp = queue->head;

	while (temp != NULL) {
		if (shortest == NULL || temp->data->CPU_Time < shortest->CPU_Time) {
			shortest = temp->data;
		}
		temp = temp->next;
	}

	//pop
	temp = queue->head;
	Node* prev = NULL;
	while (temp != NULL) {
		if (temp->data == shortest) {
			if (temp == queue->head) {
				queue->head = temp->next;
			}
			else {
				prev->next = temp->next;
			}
			if (temp == queue->tail) {
				queue->tail = prev;
			}
			free(temp);
			break;
		}
		prev = temp;
		temp = temp->next;
	}

	return shortest;
}

//SJF Aging algorithm
Process* SJFAging(Queue* queue) {
	Node* n = queue->head;
	Node* prev = NULL;
	
	Process* shortest = NULL;
	Node* temp = queue->head;
	while (temp != NULL) {
		if (shortest == NULL || temp->data->CPU_Time < shortest->CPU_Time) {
			shortest = temp->data;
		}
		temp = temp->next;
	}

	while (n != NULL) {
		n->data->Age++;
		if (n->data->Age >= 100 && shortest->Age < 100 && (shortest->CPU_Time) > (n->data->CPU_Time - n->data->Age / 5)) {
			if (n == queue->head) {
				queue->head = n->next;
			}
			else {
				prev->next = n->next;
			}
			if (n == queue->tail) {
				queue->tail = prev;
			}
			return n->data;
		}
		prev = n;
		n = n->next;
	}
	return NULL;
}

// SJF scheduling
void SJF(Queue* readyQueue, Queue* waitingQueue, Process** processes, int num) {
	int time = 0;
	int i = 0;
	Process* CPUProcess = NULL;
	Process* IOProcess = NULL;
	Process* AgingProcess = NULL;
	Process** p = DupProcess(processes, num);
	PTime t[1000] = { 0, };
	int* process_end = (int*)malloc(sizeof(int) * num);
	int* IO = (int*)malloc(sizeof(int) * num);
	int start = 0;
	int j = 0;
	int AID = 1;

	while (1) {
		if (i < num && p[i]->Arrival_Time == time) {
			Enqueue(readyQueue, p[i]);
			if (p[i]->IO_Start == p[i]->CPU_Time) {
				IO[p[i]->PID] = 0;
			}
			else {
				IO[p[i]->PID] = p[i]->IO_Time;
			}
			i++;
			continue;
		}

		if (readyQueue->head != NULL && CPUProcess == NULL) {
			CPUProcess = PopSJ(readyQueue);
			start = time;
		}
		if (CPUProcess != NULL && CPUProcess->IO_Start == 0 && CPUProcess->IO_Time > 0) {
			Enqueue(waitingQueue, CPUProcess);
			j = AddPTime(start, time, CPUProcess->PID, j, t);
			CPUProcess = NULL;
			continue;
		}
		if (waitingQueue->head != NULL && IOProcess == NULL) {
			IOProcess = Dequeue(waitingQueue);
		}
		time++;

		//IO
		if (IOProcess != NULL){
			IOProcess->IO_Time--;
			if (IOProcess->IO_Time == 0) {
				if (IOProcess->CPU_Time != 0) {
					Enqueue(readyQueue, IOProcess);
				}
				else if (IOProcess->CPU_Time == 0) {
					process_end[IOProcess->PID] = time;
				}
				IOProcess = NULL;
			}
		}

		//CPU
		if (CPUProcess != NULL){
			CPUProcess->CPU_Time--;
			CPUProcess->IO_Start--;
			CPUProcess->Age = 0;
		}

		if (CPUProcess != NULL && CPUProcess->CPU_Time == 0) {
			if (CPUProcess->IO_Time != 0) {
				Enqueue(waitingQueue, CPUProcess);
			}
			else if (CPUProcess->IO_Time == 0) {
				process_end[CPUProcess->PID] = time;
			}
			j = AddPTime(start, time, CPUProcess->PID, j, t);
			CPUProcess = NULL;
		}

		AgingProcess = SJFAging(readyQueue);
		if (AgingProcess != NULL && CPUProcess == NULL) {
			CPUProcess = AgingProcess;
			start = time;
			CPUProcess->Age = 0;
			printf("***Aging process %d, time: %d***\n", CPUProcess->PID, time);
		}
		else if (AgingProcess != NULL) {
			Enqueue(readyQueue, AgingProcess);
		}

		if (i == num && readyQueue->head == NULL && CPUProcess == NULL && waitingQueue->head == NULL && IOProcess == NULL) {
			break;
		}
	}
	printf("SJF Scheduling\n");
	GanttChart(t, j);
	Evaluation(t, j, p, process_end, IO, num, AID);
	printf("\n");

	for (int i = 0; i < num; i++) {
		free(p[i]);
	}
	free(p);
	free(process_end);
	free(IO);
}

//find highest priority job and pop  ***lower number higher prioriy
Process* PopHPJ(Queue* queue) {
	Process* highest = NULL;
	Node* temp = queue->head;

	while (temp != NULL) {
		if (highest == NULL || temp->data->Priority < highest->Priority) {
			highest = temp->data;
		}
		temp = temp->next;
	}

	//pop
	temp = queue->head;
	Node* prev = NULL;
	while (temp != NULL) {
		if (temp->data == highest) {
			if (temp == queue->head) {
				queue->head = temp->next;
			}
			else {
				prev->next = temp->next;
			}
			if (temp == queue->tail) {
				queue->tail = prev;
			}
			free(temp);
			break;
		}
		prev = temp;
		temp = temp->next;
	}

	return highest;
}

//Priority Aging algorithm
void PrirorityAging(Queue* queue) {
	Node* n = queue->head;

	while (n != NULL) {
		n->data->Age++;
		if (n->data->Age >= 100) {
			n->data->Priority--;
			n->data->Age = 0;
			printf("***Process %d Priority changed to %d***\n", n->data->PID, n->data->Priority);
		}
		n = n->next;
	}
}

// Priority scheduling
void Priority(Queue* readyQueue, Queue* waitingQueue, Process** processes, int num) {
	int time = 0;
	int i = 0;
	Process* CPUProcess = NULL;
	Process* IOProcess = NULL;
	Process** p = DupProcess(processes, num);
	PTime t[1000] = { 0, };
	int* process_end = (int*)malloc(sizeof(int) * num);
	int* IO = (int*)malloc(sizeof(int) * num);
	int start = 0;
	int j = 0;
	int AID = 2;

	while (1) {
		if (i < num && p[i]->Arrival_Time == time) {
			Enqueue(readyQueue, p[i]);
			if (p[i]->IO_Start == p[i]->CPU_Time) {
				IO[p[i]->PID] = 0;
			}
			else {
				IO[p[i]->PID] = p[i]->IO_Time;
			}
			i++;
			continue;
		}

		if (readyQueue->head != NULL && CPUProcess == NULL) {
			CPUProcess = PopHPJ(readyQueue);
			start = time;
		}
		if (CPUProcess != NULL && CPUProcess->IO_Start == 0 && CPUProcess->IO_Time > 0) {
			Enqueue(waitingQueue, CPUProcess);
			j = AddPTime(start, time, CPUProcess->PID, j, t);
			CPUProcess = NULL;
			continue;
		}
		if (waitingQueue->head != NULL && IOProcess == NULL) {
			IOProcess = Dequeue(waitingQueue);
		}
		time++;
		
		//IO
		if (IOProcess != NULL){
			IOProcess->IO_Time--;
			if (IOProcess->IO_Time == 0) {
				if (IOProcess->CPU_Time != 0) {
					Enqueue(readyQueue, IOProcess);
				}
				else if (IOProcess->CPU_Time == 0) {
					process_end[IOProcess->PID] = time;
				}
				IOProcess = NULL;
			}
		}

		//CPU
		if (CPUProcess != NULL){
			CPUProcess->CPU_Time--;
			CPUProcess->IO_Start--;
		}

		if (CPUProcess != NULL && CPUProcess->CPU_Time == 0) {
			if (CPUProcess->IO_Time != 0) {
				Enqueue(waitingQueue, CPUProcess);
			}
			else if (CPUProcess->IO_Time == 0) {
				process_end[CPUProcess->PID] = time;
			}
			j = AddPTime(start, time, CPUProcess->PID, j, t);
			CPUProcess = NULL;
		}

		PrirorityAging(readyQueue);

		if (i == num && readyQueue->head == NULL && CPUProcess == NULL && waitingQueue->head == NULL && IOProcess == NULL) {
			break;
		}
	}
	printf("Priority Scheduling\n");
	GanttChart(t, j);
	Evaluation(t, j, p, process_end, IO, num, AID);
	printf("\n");

	for (int i = 0; i < num; i++) {
		free(p[i]);
	}
	free(p);
	free(process_end);
	free(IO);
}

// Round Robin Scheduling
void RR(Queue* readyQueue, Queue* waitingQueue, Process** processes, int num) {
	int time = 0;
	int time_quantum = 10; //can change
	int process_time = 0;
	int i = 0;
	Process* CPUProcess = NULL;
	Process* IOProcess = NULL;
	Process** p = DupProcess(processes, num);
	PTime t[1000] = { 0, };
	int* process_end = (int*)malloc(sizeof(int) * num);
	int* IO = (int*)malloc(sizeof(int) * num);
	int start = 0;
	int j = 0;
	int AID = 3;

	while (1) {
		if (i < num && p[i]->Arrival_Time == time) {
			Enqueue(readyQueue, p[i]);
			if (p[i]->IO_Start == p[i]->CPU_Time) {
				IO[p[i]->PID] = 0;
			}
			else {
				IO[p[i]->PID] = p[i]->IO_Time;
			}
			i++;
			continue;
		}

		if (readyQueue->head != NULL && CPUProcess == NULL) {
			CPUProcess = Dequeue(readyQueue);
			start = time;
		}
		if (CPUProcess != NULL && CPUProcess->IO_Start == 0 && CPUProcess->IO_Time > 0) {
			Enqueue(waitingQueue, CPUProcess);
			j = AddPTime(start, time, CPUProcess->PID, j, t);
			CPUProcess = NULL;
			continue;
		}
		if (waitingQueue->head != NULL && IOProcess == NULL) {
			IOProcess = Dequeue(waitingQueue);
		}
		time++;		
		
		//IO
		if (IOProcess != NULL){
			IOProcess->IO_Time--;
			if (IOProcess->IO_Time == 0) {
				if (IOProcess->CPU_Time != 0) {
					Enqueue(readyQueue, IOProcess);
				}
				else if (IOProcess->CPU_Time == 0) {
					process_end[IOProcess->PID] = time;
				}
				IOProcess = NULL;
			}
		}

		//CPU
		if (CPUProcess != NULL){
			CPUProcess->CPU_Time--;
			CPUProcess->IO_Start--;
			process_time++;
		}

		if (CPUProcess != NULL && CPUProcess->CPU_Time != 0 && process_time == time_quantum){
			Enqueue(readyQueue, CPUProcess);
			j = AddPTime(start, time, CPUProcess->PID, j, t);
			process_time = 0;
			CPUProcess = NULL;
		}
		if (CPUProcess != NULL && CPUProcess->CPU_Time == 0) {
			if (CPUProcess->IO_Time != 0) {
				Enqueue(waitingQueue, CPUProcess);
			}
			else if (CPUProcess->IO_Time == 0) {
				process_end[CPUProcess->PID] = time;
			}
			j = AddPTime(start, time, CPUProcess->PID, j, t);
			process_time = 0;
			CPUProcess = NULL;
		}

		if (i == num && readyQueue->head == NULL && CPUProcess == NULL && waitingQueue->head == NULL && IOProcess == NULL) {
			break;
		}
	}
	printf("RR Scheduling\n");
	GanttChart(t, j);
	Evaluation(t, j, p, process_end, IO, num, AID);
	printf("\n");

	for (int i = 0; i < num; i++) {
		free(p[i]);
	}
	free(p);
	free(process_end);
	free(IO);
}

//Preemptive SJF scheduling
void PreemptSJF(Queue* readyQueue, Queue* waitingQueue, Process** processes, int num) {
	int time = 0;
	int i = 0;
	Process* CPUProcess = NULL;
	Process* IOProcess = NULL;
	Process* newProcess = NULL;
	Process* AgingProcess = NULL;
	bool flag = false;
	Process** p = DupProcess(processes, num);
	PTime t[1000] = { 0, };
	int* process_end = (int*)malloc(sizeof(int) * num);
	int* IO = (int*)malloc(sizeof(int) * num);
	int start = 0;
	int j = 0;
	int AID = 4;

	while (1) {
		if (i < num && p[i]->Arrival_Time == time) {
			Enqueue(readyQueue, p[i]);
			if (p[i]->IO_Start == p[i]->CPU_Time) {
				IO[p[i]->PID] = 0;
			}
			else {
				IO[p[i]->PID] = p[i]->IO_Time;
			}
			flag = true;
			i++;
			continue;
		}

		if ((readyQueue->head != NULL && CPUProcess == NULL)) {
			CPUProcess = PopSJ(readyQueue);
			start = time;
			flag = false;
		}
		else if (readyQueue->head != NULL && flag) {
			newProcess = PopSJ(readyQueue);
			if (CPUProcess->CPU_Time <= newProcess->CPU_Time) {
				Enqueue(readyQueue, newProcess);
			}
			else {
				Enqueue(readyQueue, CPUProcess);
				j = AddPTime(start, time, CPUProcess->PID, j, t);
				CPUProcess = newProcess;
				start = time;
				newProcess = NULL;
			}
			flag = false;
		}
		if (CPUProcess != NULL && CPUProcess->IO_Start == 0 && CPUProcess->IO_Time > 0) {
			Enqueue(waitingQueue, CPUProcess);
			j = AddPTime(start, time, CPUProcess->PID, j, t);
			CPUProcess = NULL;
			continue;
		}
		if (waitingQueue->head != NULL && IOProcess == NULL) {
			IOProcess = Dequeue(waitingQueue);
		}
		time++;

		//IO
		if (IOProcess != NULL){
			IOProcess->IO_Time--;
			if (IOProcess->IO_Time == 0) {
				if (IOProcess->CPU_Time != 0) {
					Enqueue(readyQueue, IOProcess);
				}
				else if (IOProcess->CPU_Time == 0) {
					process_end[IOProcess->PID] = time;
				}
				IOProcess = NULL;
			}
		}

		//CPU
		if (CPUProcess != NULL){
			CPUProcess->CPU_Time--;
			CPUProcess->IO_Start--;
			CPUProcess->Age = 0;
		}

		if (CPUProcess != NULL && CPUProcess->CPU_Time == 0) {
			if (CPUProcess->IO_Time != 0) {
				Enqueue(waitingQueue, CPUProcess);
			}
			else if (CPUProcess->IO_Time == 0) {
				process_end[CPUProcess->PID] = time;
			}
			j = AddPTime(start, time, CPUProcess->PID, j, t);
			CPUProcess = NULL;
		}

		AgingProcess = SJFAging(readyQueue);
		if (AgingProcess != NULL && CPUProcess == NULL) {
			CPUProcess = AgingProcess;
			CPUProcess->Age = 0;
			start = time;
			printf("******Aging process %d, time: %d\n", CPUProcess->PID, time);
		}
		else if (AgingProcess != NULL) {
			Enqueue(readyQueue, AgingProcess);
		}

		if (i == num && readyQueue->head == NULL && CPUProcess == NULL && waitingQueue->head == NULL && IOProcess == NULL) {
			break;
		}
	}
	printf("Preemptive SJF Scheduling\n");
	GanttChart(t, j);
	Evaluation(t, j, p, process_end, IO, num, AID);
	printf("\n");

	for (int i = 0; i < num; i++) {
		free(p[i]);
	}
	free(p);
	free(process_end);
	free(IO);
}

// Preemptive Priority scheduling
void PreemptPriority(Queue* readyQueue, Queue* waitingQueue, Process** processes, int num) {
	int time = 0;
	int i = 0;
	Process* CPUProcess = NULL;
	Process* IOProcess = NULL;
	Process* newProcess = NULL;
	bool flag = false;
	Process** p = DupProcess(processes, num);
	PTime t[1000] = { 0, };
	int* process_end = (int*)malloc(sizeof(int) * num);
	int* IO = (int*)malloc(sizeof(int) * num);
	int start = 0;
	int j = 0;
	int AID = 5;

	while (1) {
		if (i < num && p[i]->Arrival_Time == time) {
			Enqueue(readyQueue, p[i]);
			if (p[i]->IO_Start == p[i]->CPU_Time) {
				IO[p[i]->PID] = 0;
			}
			else {
				IO[p[i]->PID] = p[i]->IO_Time;
			}
			flag = true;
			i++;
			continue;
		}

		if (readyQueue->head != NULL && CPUProcess == NULL) {
			CPUProcess = PopHPJ(readyQueue);
			start = time;
			flag = false;
		}
		else if (readyQueue->head != NULL && flag) {
			newProcess = PopHPJ(readyQueue);
			if (CPUProcess->Priority <= newProcess->Priority) {
				Enqueue(readyQueue, newProcess);
			}
			else {
				Enqueue(readyQueue, CPUProcess);
				j = AddPTime(start, time, CPUProcess->PID, j, t);
				CPUProcess = newProcess;
				start = time;
				newProcess = NULL;
			}
			flag = false;
		}
		if (CPUProcess != NULL && CPUProcess->IO_Start == 0 && CPUProcess->IO_Time > 0) {
			Enqueue(waitingQueue, CPUProcess);
			j = AddPTime(start, time, CPUProcess->PID, j, t);
			CPUProcess = NULL;
			continue;
		}
		if (waitingQueue->head != NULL && IOProcess == NULL) {
			IOProcess = Dequeue(waitingQueue);
		}
		time++;
		
		//IO
		if (IOProcess != NULL){
			IOProcess->IO_Time--;
			if (IOProcess->IO_Time == 0) {
				if (IOProcess->CPU_Time != 0) {
					Enqueue(readyQueue, IOProcess);
				}
				else if (IOProcess->CPU_Time == 0) {
					process_end[IOProcess->PID] = time;
				}
				IOProcess = NULL;
			}
		}

		//CPU
		if (CPUProcess != NULL){
			CPUProcess->CPU_Time--;
			CPUProcess->IO_Start--;
		}
		PrirorityAging(readyQueue);

		if (CPUProcess != NULL && CPUProcess->CPU_Time == 0) {
			if (CPUProcess->IO_Time != 0) {
				Enqueue(waitingQueue, CPUProcess);
			}
			else if (CPUProcess->IO_Time == 0) {
				process_end[CPUProcess->PID] = time;
			}
			j = AddPTime(start, time, CPUProcess->PID, j, t);
			CPUProcess = NULL;
		}

		if (i == num && readyQueue->head == NULL && CPUProcess == NULL && waitingQueue->head == NULL && IOProcess == NULL) {
			break;
		}
	}
	printf("Preemptive Priority Scheduling\n");
	GanttChart(t, j);
	Evaluation(t, j, p, process_end, IO, num, AID);
	printf("\n");

	for (int i = 0; i < num; i++) {
		free(p[i]);
	}
	free(p);
	free(process_end);
	free(IO);
}

// RUN All Scheduling Algorithm, Show Evaluation Result
void RUN(Queue* readyQueue, Queue* waitingQueue, Process** processes, int num) {
	FCFS(readyQueue, waitingQueue, processes, num);
	SJF(readyQueue, waitingQueue, processes, num);
	Priority(readyQueue, waitingQueue, processes, num);
	RR(readyQueue, waitingQueue, processes, num);
	PreemptSJF(readyQueue, waitingQueue, processes, num);
	PreemptPriority(readyQueue, waitingQueue, processes, num);

	printf("-Waiting Time Ranking-\n");
	int len = sizeof(Waiting) / sizeof(Waiting[0]);
	for (int r = 1; r < len + 1; r++) {
		printf("Rank %d - ", r);
		int res = 0;
		int t = 100000;
		for (int i = 0; i < len; i++) {
			if (Waiting[i] < t) {
				t = Waiting[i];
				res = i;
			}
		}
		switch (res)
		{
		case 0:
			printf("FCFS, time: %d\n", Waiting[res]);
			Waiting[res] = 100000;
			break;
		case 1:
			printf("SJF, time: %d\n", Waiting[res]);
			Waiting[res] = 100000;
			break;
		case 2:
			printf("Priority, time: %d\n", Waiting[res]);
			Waiting[res] = 100000;
			break;
		case 3:
			printf("Round Robin, time: %d\n", Waiting[res]);
			Waiting[res] = 100000;
			break;
		case 4:
			printf("Preemptive SJF, time: %d\n", Waiting[res]);
			Waiting[res] = 100000;
			break;
		case 5:
			printf("Preemptive Priority, time: %d\n", Waiting[res]);
			Waiting[res] = 100000;
			break;
		default:
			break;
		}
	}
	printf("\n");
	
	printf("-Turnaround Time Ranking-\n");
	int len2 = sizeof(Turnaround) / sizeof(Turnaround[0]);
	for (int r = 1; r < len2 + 1; r++) {
		printf("Rank %d - ", r);
		int res = 0;
		int t = 100000;
		for (int i = 0; i < len2; i++) {
			if (Turnaround[i] < t) {
				t = Turnaround[i];
				res = i;
			}
		}
		switch (res)
		{
		case 0:
			printf("FCFS, time: %d\n", Turnaround[res]);
			Turnaround[res] = 100000;
			break;
		case 1:
			printf("SJF, time: %d\n", Turnaround[res]);
			Turnaround[res] = 100000;
			break;
		case 2:
			printf("Priority, time: %d\n", Turnaround[res]);
			Turnaround[res] = 100000;
			break;
		case 3:
			printf("Round Robin, time: %d\n", Turnaround[res]);
			Turnaround[res] = 100000;
			break;
		case 4:
			printf("Preemptive SJF, time: %d\n", Turnaround[res]);
			Turnaround[res] = 100000;
			break;
		case 5:
			printf("Preemptive Priority, time: %d\n", Turnaround[res]);
			Turnaround[res] = 100000;
			break;
		default:
			break;
		}
	}
	printf("\n");

	printf("-CPU Utilization Ranking-\n");
	int len3 = sizeof(Utilization) / sizeof(Utilization[0]);
	for (int r = 1; r < len3 + 1; r++) {
		printf("Rank %d - ", r);
		int res = 0;
		int u = 0;
		for (int i = 0; i < len3; i++) {
			if (Utilization[i] > u) {
				u = Utilization[i];
				res = i;
			}
		}
		switch (res)
		{
		case 0:
			printf("FCFS: %d%%\n", Utilization[res]);
			Utilization[res] = 0;
			break;
		case 1:
			printf("SJF: %d%%\n", Utilization[res]);
			Utilization[res] = 0;
			break;
		case 2:
			printf("Priority: %d%%\n", Utilization[res]);
			Utilization[res] = 0;
			break;
		case 3:
			printf("Round Robin: %d%%\n", Utilization[res]);
			Utilization[res] = 0;
			break;
		case 4:
			printf("Preemptive SJF: %d%%\n", Utilization[res]);
			Utilization[res] = 0;
			break;
		case 5:
			printf("Preemptive Priority: %d%%\n", Utilization[res]);
			Utilization[res] = 0;
			break;
		default:
			break;
		}
	}
}

/***************************************************************************************************************/

int main() {
	srand(time(0));

	Queue* readyQueue = Create_Queue();
	Queue* waitingQueue = Create_Queue();

	int num = rand() % 10 + 1;
	Process** processes = Config(num);

	for (int i = 0; i < num; i++) {
		printf("PID: %d, CPU_Time: %d, IO_Time: %d, Arrival_Time: %d, Priority: %d, IO_Start: %d\n",
				processes[i]->PID, processes[i]->CPU_Time, processes[i]->IO_Time, processes[i]->Arrival_Time, processes[i]->Priority, processes[i]->IO_Start);
	}
	printf("\n");

	RUN(readyQueue, waitingQueue, processes, num);

	for (int i = 0; i < num; i++) {
		free(processes[i]);
	}
	free(processes);
	Free_Queue(readyQueue);
	Free_Queue(waitingQueue);

	return 0;
}
