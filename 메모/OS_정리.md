# Chap.4 Threads



## 1. Thread

### 	1) 정의

​		: 하나의 프로그램 내에서 실행되고 있는 명령어를 다른 두 branch로 나누게 하는 개념

​		(영어로는, a way for a program to split itself into two or more simultaneously running tasks)

​		: ThreadID, PC, register set, stack, etc. 를 가진다.

### 	2) 효과

​		a) Scalability  

​			: single-thread에 비해, CPU 활용의 확장이 용이하다.

​		b) Responsiveness

​			: single-thread에 비해, 반응성이 높다.

​		c) Resource sharing

​			: multiple processes에 비해, 자원을 공유할 수 있다.

​		d) Economy

​			: multiple processes에 비해, 경제적이다.



### 	3) 종류

##### 		a) User thread

​			1.  thread supported by **thread library in user level**

​			2.  Created by library **function call (not system call)**

​			3.  Kernel is not concerned in user thread

​			4.  Switching of user thread is faster than kernel thread

##### 		b) Kernel thread

​			1. thread supported by kernel

​			2. **Created** and **managed** by kernel

​			3. **Scheduled** by kernel

​			4. kernel thread 하나당 LWP(Light Weight Process) 하나에 맵핑되어 있다.

##### 		---> User thread와 Kernel thread을 어떻게 맵핑하는가에 따른, Multi Threading 모델

​				1. many-to-one   (user library 책임)

​				2. one-to-one     (kernel scheduler 책임)

​				3. many-to-many   (둘다 책임)

​				4. two level   (둘다 책임)

##### 										---> 3번과 4번의 경우, Scheduler activation (둘다 책임)

​						: many-to-many 모델은 thread스케줄링의 책임은 커널 스케줄러, user lbrary가 같이 책임진다. 이 때, 소통이 필요한데 이것이 스케줄러 activation

​								cf) 용어

​									-  upcall

​										: If kernel thread is **blocked**, assign the LWP to **another** thread

​										: If kernel thread is **unblocked**, assign an LWP to **it**

### 	5) 라이브러리

##### 		a) 명시적 (thread를 사용할 때, 변수나 클래스 생성하고 사용) 

​			1. POSIX Pthread

​			2. Java Thread class, Runnable class

​			3. Windows XP Threads

​				: 기본적으로, one-to-one mapping, kernel level이다.

​				: many-to-many 모델은 fiber libaray로 제공한다.

​					cf) fiber library

​						: 유저레벨에서 쓰레드를 브랜치 해주는 라이브러리

​				: 각 쓰레드는 context를 가진다.

​					cf) context of the thread

​						: user stack, kernel stack, private data storage area(TLS)

​				: 내부적으로 데이터구조 ETHREAD(PCB와 같은 개념), KTHREAD(ETHREAD의 일부로서, 스케줄링 정보를 담고 있다.)이다.

##### 		b) 암묵적 (thread를 사용할 때, 변수나 클래스 생성 필요 없음)

​			1. Thread pools

​				: Pool에 여러개의 thread를 할당해놓고 threading하는 방법

​			2. OpenMp

​				: C or C++에서 #pragma 로 옵션을 주어 threading하는 방법

### 	6) Thread 사용 가운데, 다음 명령어로 인한 이슈

##### 		1. fork(), exec()

​			1) fork

​				: 프로세스 전체 복제

​			2) fork1

​				: 해당 **쓰레드만** 복제

​			3) exec

​				: exec 사용시, 프로세스 전체를 교체함.



##### 		2. cancellation

​		: 다른 쓰레드를 종료시키는 것.

​				1) PTHREAD_CANCEL_ASYNCHRONOUS : 바로 종료.

​				2) PTHREAD_CANCEL_DEFERRED : 일정 지점마다 cancel요청 확인하고 종료.

​				3) PTHREAD_CANCEL_DISABLE : cancel 허용하지 않음.



##### 		3. Signal handling

​		: 시그널은 특정한 유저 프로세스에게 전달되는 소프트웨어 인터럽트, 이를 처리하는 것.

​		: (영어로는) a software interrupt delivered to a process. it is an asynchronous notification sent to a process in order to notify it of an event that occurred

​		: 생각해야하는 것은, 멀티 쓰레드에서 외부에서 signal이 날라오면, 어떤 쓰레드가 signal을 처리할 것인가.

​			1. user interface를 담당하던 쓰레드라면, 해당 쓰레드가 처리하면 됨.

​			2. 모든 쓰레드에게 signal을 전달해서 각각이 핸들링하게 한다.

​			3. 그냥 아무 쓰레드에게 넘겨준다.

​			4. 특정 쓰레드에게 모두넘겨준다.

​				cf) pthread_kill(tid, signal)

​						: tid에게 signal 전달



##### 		4. Thread-local storage(TLS)

​			: TLS는 global 변수 같기는 하지만, 쓰레드별로 존재한다.

​			: allows each thread to have its own copy of data

​			: Useful when you do not have control over the thread creation process (i.e., when using a thread pool)s



##### 		5. Scheduler activation

​			: 위의 설명이 전부

​			: many-to-many & two-level 모델은 thread스케줄링의 책임은 커널 스케줄러, user lbrary가 같이 책임진다.





## 2. Multicore Programming

### 	1) 정의

​		: 멀티코어 환경에서 **동시성** 시스템을 만드는데 사용되는 프로그래밍

​		: (영어로는) helps to create concurrent systems for deployment on multicore processor and multiprocessor systems

​	

### 	2) 용어

​		a) **Concurrency** (동시)

​			: 여러개의 일이 동시에 진전되는 것. 

​			 : supports more than one task making progress

​		b) **Parallelism** (병렬)

​			 : 실제 물리적으로 병렬적으로 일 처리하는 것. (실제 계산이 빨라지는 것은 paralelism이다.)

​			 : implies a system can perform more than one task simultaneously

​			i) Data parallelism

​			 	: 같은 연산을 영역 나눠서 처리하는 것

​				 : distributes subsets of the same data

​			ii) Task parallelism

​				 : 하는 연산 자체가 다른 경우

​				 : distributing threads across cores, each thread performing unique operation



# Chap.5 CPU Scheduling

## 1. CPU Scheduling

### 1) 정의

​	: reday queue에서 한 프로세스를 골라 CPU에 할당하는 것.

​	cf) CPU스케줄러가 프로세스를 선택하는 것이라면, 실제 선택된 프로세스를 실행시키는 것을 **dispatcher**가 한다.



### 2) 효과

​	: multiprogramming과 multitasking으로, **CPU활용도** 높여줌.



### 3) 용어

 1. I/O-bound process

    : CPU-burst가 짧음.

 2. CPU-bound process

    : CPU-burst가 김.

##### ---> First and last bursts are CPU bursts (맨 처음 busrst와 맨 마지막 burst는 CPU burst임.) 



### 4) 스케줄링 발생 시점

1. running —> waiting 

2. running —> ready 

3. waiting —> ready

4. running —> terminated 



##### 1. preemptive scheduling

: scheduling may occur while a process is running

: race condition의 가능성이 있다.

: 1, 2, 3, 4 모두에 해당된다.



##### 2. non-preemptive scheduling

: 1, 4에만 해당된다.



##### 3. preemptive kernel 

: preemptive scheduling은 유저 프로세스가 중간에 인터럽트 당할 수 있는지 없는지
preemptive kernel은 시스템 콜 중간에 인터럽트 당할 수 있는지 없는지 

: (영어로는) kernel allows preemption in system call



### 5) 평가 기준

1. CPU Utilization : CPU가 최대로 일하는가

2. Throughput : 동일 시간내에 얼마나 많은 일을 했는가

3. **Turnaround time** : 프로세스 시작~종료까지 전체 시간 = 기다리는시간 + 실제돌아가는시간

4. Waiting time : ready queue에서 기다린 시간의 총합

5. Response time (응답시간) : request를 보내고 나서 유저가 처음으로 반응을 받는데까지 걸리는 시간



### 6) 알고리즘 

##### 1. First-come, first-served (FCFS) scheduling

 : 인터럽트 당하지도 않고, 먼저온 프로세스 먼저 실행



##### 2. Shortest-job-first (SJF) scheduling   - *OPTIMAL(최적)*

: CPU burst 짧은거 먼저 실행. 하지만, burst time을 알아야하는데, 정확하게 알 수가 없음.

 : waiting time 관점에서는 SJF가 최적임. 



##### 3. Priority scheduling

 : 중요한 프로세스를 먼저 실행시킬 수 있다는 장점이 있음. 

 : 모든 프로세스가 priority가 같다면 FCFS. 

 : 우선순위를 waiting time으로 주면 SJF. 

 : preemptive할수도 있고, non-preemptive할 수도 있다.

 : Indefinite blocking (= starvation)이 발생할 수 있다. 우선순위가 낮은 프로세스의 경우, 최악의 경우 영원히 CPU time을 받지 못하게 될 수도 있음.

 : aging : CPU time을 받지 못하는 시간이 길어질수록 우선순위를 높여주는 것.



##### 4. Round-robin scheduling

 : FCFS + preemptive

 : 먼저 온 순서대로, 일정한 시간(time quantum) 만큼만 실행이 되고, CPU 자리를 내준다.

 : time-sharing 시스템으로 디자인 됨.

 : ready queue는 circular queue로

 : 성능, context switching 오버헤드 모두 time quantum에 영향 받는다.



##### 5. Multilevel queue scheduling

 : 여러개의 queue를 사용함. 각각이 별도의 스케줄링 알고리즘을 사용함.

 : 각 queue마다 대표를 하나씩을 내면, 그 중에서 CPU에 올릴 것을 결정해야함.

 	a) Fixed-priority preemptive scheduling

​		: 우선순위가 높은 queue부터 확인해서 실행한다.

​	 b) Time-slicing among queues

​		:  foreground queue가 우선순위가 높게 가지게 하지만, background도 기회를 갖고 돌아간다.

 : 프로세스는 다른 queue로 이동할 수 없음.



##### 6. Multiple feedback-queue scheduling

 : 프로세스는 다른 queue로 이동할 수 있음.

 : CPU time을 많이 사용 —> CPU-bound process —> user와 상호작용이 적음 —> 우선순위를 낮춤

 : 결정해야할 것들이 많다.



## Multi-processor scheduling

 : CPU가 여러개 있는 시스템



### 1) 타입

1. Symmetric

    : 어떤 프로세서에서 돌아도 괜찮은 것.

2. Asymmetric

   : 어떤 프로세서 중에 마스터 CPU가 있음.



### 2) 문제 발생

​	: CPU가 여러개이기 때문에, A프로세스를 1번 CPU에 넣었다가, 2번 CPU에 넣는 migration이 발생할 수 있음. 하지만 이러면, 오버헤드가 발생함.



### 3) 해결 정책 -> Processor affinity

 : 프로세스 돌리기 시작하면, 되도록 한 CPU에서

 : (영어로는) keeping a process running on the same processor to avoid migration overhead

​		1) Soft affinity (필요시 migration 허용)

​		2) Hard affinity (반드시 해당 CPU에서만 돌도록)



### 3) Load balancing

​	: CPU가 여러개 있을 때, 여러개의 workload가 다수의 CPU에 골고루 분배되는 것이 가장 효율적인데, 그렇게 해주는 것

​	: (영어로는) attempt to keep the workload evenly distributed across all processors.

​	: 필연적으로, Load balancing는 processor affinity를 상쇄한다.

##### 		1) Push migration

​			: 특정 한 프로세스(OS)가 밸런스를 확인하고, 불균형이 발견되면, 밸런스 맞춰줌

##### 		2) Pull migration

​			: 노는 CPU있으면, 일을 더 할당해주는 것

​			**cf) Push and pull migration can be implemented in parallel.**



### 4) 문제점

 : 메모리에 접근할 때 시간이 많이 걸린다. (**Memory stall**) 

​	--> 해결법 : **multithreaded processor cores**



## Thread scheduling

### 1) 정의 

​	: Scheduled indirectly through LWP

​	(쓰레드 스케쥴러는 커널 쓰레드(LWP) 단위로 스케줄링함.)



### 2) 경쟁 범위(contention scope)

##### 		1. PCS (process 안에서)

​		: LWP에 대하여 user thread들끼리 경쟁, many-to-one

##### 		2. SCS (system 전체에서)

​		: CPU에 대하여 kernel thread들끼리 경쟁, one-to-one



 

## Real-time CPU scheduling

### 1) 정의

 : 요구가 들어왔을 때, 정해진 시간내에 처리하는 것을 보장하는 것.

 : 모든 RT는 event-driven이다.

 : Priority-based 스케줄링이다.



​	1) soft RT

​		: Guarantee only that the process will be given preference over noncritical processes. 

​	2) hard RT



### 2) Event latency

##### 1. Interrupt latency

 : 인터럽트 도착시점 ~ ISR시작시점

##### 2. Dispatch latency

 : 한 프로세스 멈추고, 다음 프로세스 시작까지 시간

 : dispatch latency를 낮추는데 가장 중요한 요소는 preemptive kernel이다. 

​	1) conflict

​		: 커널에서 돌아가고 있는 프로세스를 preemption하는 단계, resource를 해제 및 제공

​	2) dispatch

​		: 실제 사용가능한 CPU에 올리는 스케줄링 단계



### 3) 종류

##### 1. Rate-Monotonic scheduling   -*OPTIMAL(최적)*

 : periodic task를 스케줄링한는 것. 이 때, 처리 시간은 CPUburst와 같아야 한다.

 : static priority policy이며 이 정책에서 최적의 알고리즘이다.

 : inversely vased on its period이다.

 : CPU utilization이 제한된다.



##### 2. Earliest-deadline-first scheduling    -*OPTIMAL(최적)*

 : 데드라인이 임박한 것부터 스케줄링한다.

 : periodic task가 아니라도 가능하며, burst당 CPUtime의 양이 일정하지 않아도 된다.

 : CPU utilization이 이론적으로 최적이다.



##### 3. Proportional Share Scheduling

 : CPU time에 대한 지분에 따라 스케줄링한다.



## Operating system

### 1) 종류

1. Linux
2. Windows
3. Solaris



### 2) 용어

1. nice value : 상대적 우선순위, 0이 default이다.
2. targeted latency : 실행 가능한 task가 적어도 한번 이상 실행되어야 하는 time interval
3. decay factor : 시간이 지날때마다 곱해지는 값으로, 우선순위가 낮다면, 큰 값을 곱한다.(task가 실행된 시간을 뜻하는 vruntime이 작을수록 우선순위가 높다.)



# Chap.6 Process Synchronization

## Process Synchronization

### 1) 정의

​	: 여러 프로세스가 동시에 하나의 데이터에 접근하려고 할 때 발생하는 race condition(여러 프로세스, 쓰레드를 사용할 때, concurency 문제가 발생하는 것) 문제를 해결하는 것을 말한다.



### 2) 임계영역 

 : Critical-section이란, Sync문제를 통칭하는 말이다. 여러개의 프로세스가 있을 때, 공유하는 데이터의 업데이트가 이루어지는 곳이다.

##### 	1. Critical section

​		: shared data에 접근하는 파트

##### 	2. Remainder section

​		: shared data접근이 아니라서 문제가 없는 파트

##### 	3. Entry section

​		: critical section 들어갈때, 처리해야할 일을 처리하는 파트

##### 	4. Exit section

​		: critical section 나올 때, 해야하는 일을 처리하는 파트



### 3) 조건

##### 	1. Mutual exclusion

​		: 한 프로세스가 c.s에 있으면, 다른 프로세스는 c.s에 들어갈 수 없다.

##### 	2. Progress

​		: c.s에 아무도 없고, 프로세스들이 c.s에 들어가고 싶다고 한다면, 하나만 뽑아서 즉시 허용하는것.

##### 	3. Bounded waiting

​		: 기다리는 횟수는 일정 횟수 이상 되면 안된다.



### 4) 해결 접근

##### 	1. Non-preemptive kernel

​		: 커널 내부에서 race condition 발생하지 않음

##### 	2. Preemptive kernel

​		: 커널 내부에서도 스위칭이 일어날 수 있는 경우 —> race condition 발생 가능 —> Peterson's Solution



### 3) 해결

#### 	1. Peterson's Solution

##### 		1) 변수

​			: turn     // 어느 프로세스가 입장할 차례인지    ex) 0, 1

​			: flag[i]   // i번째 프로세스가 들어갈 준비가 되었는지    ex) true, false

##### 		2) 한계

​			: compiler의 최적화로 인해, 연산 순서가 하드웨어의 추가적인 도움이 필요하게 됨.

​				ex) Memory barrier : 변경된 값이 다른 프로세스들에게 모두 반영되도록 강제하는 것



##### 		3) 실제 코드 구현

```c
1. Process P0
do {
	flag[0] = true;
	turn = 1;
	while (flag[1] and turn == 1);
		critical section
	flag[0] = false;
		remainder section
} while (1);

2. Process P1
do {
	flag[1] = true;
	turn = 0;
	while (flag[0] and turn == 0); 
  	critical section
	flag[1] = false;
		remainder section
} while (1);
```



#### 	2. Interrupt Disable

​		: CPU가 1개이던 시절 사용, 인터럽트 허용 안하면 됨.



#### 	3. Hardware instruction (atomic instruction)

​		: 이건 내용이 많으니 밑으로



### 4) Hardware instruction (atomic instruction)

#### 	1) Mutual exclusion or Bounded waiting  or 둘다 해결 (예제에 따라)

##### 			1. TestAndSet(&lock)

​				리턴) lock의 처음값

​				내용) lock = true 

```c
boolean TestAndSet(boolean *target) {
	boolean rv = *target; 
  *target = true;
	return rv;
}
```



##### 			2. Swap(a, b)

​				리턴) 없음

​				내용) a와 b를 바꿈

```c
void Swap(boolean *a, boolean *b) {
	boolean temp = *a; 
  *a = *b;
	*b = temp;
}
```



##### 			3. Compare-and-swap(&lock, m, n)

​				리턴) lock의 처음값

​				내용) lock과 m의 값이 같으면, lock = n

```c
int compare_and_swap(int *value, int expected, int new_value) { 
  int temp = *value;
	if (*value == expected)
		*value = new_value; 
  return temp;
}
```



cf) atomic variable

​	: 값을 변화시키는 연산 자체가 atomic한 변수

​	: special data type + operation





## Mutex & Semaphore

### 1. Mutex

#####  	1) 변수

​		: available     // 1일때, c.s에 들어가도 된다.

##### 	2) 실행

​		: acquire     // entry section, available이 0이면, busy waiting, 1이면, available = false

​		: release     // exit section, available = true

```c
acquire() {
	while (!available); /* busy wait */
	available = false; 
}

release() {
	available = true; 
}
```

​		

### 2. Semaphore

##### 	1) 변수

​		: S       // c.s에 들어갈 수 있는 프로세스의 수

##### 	2) 실행

​		: wait     // entry section, S<=0이면, busy waiting, S--

​		: signal  // exit section, S++

```c
wait(S) {
	while(S <= 0); // waits for the lock 
  S--; // holds the lock
}

signal(S) {
	S++; // releases the lock
}



// 실제 예제
wait(semaphore *S){
	S->value--; 
  if(S->value < 0){
		add this process to S->list;
		block(); // suspend 
  }
}

signal(semaphore *S) {
	S->value++; 
  if(S->value <= 0){
		remove a process P from S->list;
    wakeup(P);  //resume
	} 
}
```





# 핵심 예제

### [ Bounded Waiting Mutual Exclusion ]

#### 1. TestAndSet

```c
while(TRUE){ 
  waiting[i] = TRUE;
	key = TRUE; // 나는 c.s에 들어가고 싶다.
  while(waiting[i] && key)
		key = TestAndSet(&lock); 
  waiting[i] = FALSE; // 나는 c.s에 들어왔다.
  
	/* critical section */
  
	j = (i + 1) % n; // 나의 오른쪽에 있는 프로세스에게 기회를 준다.
	while(j != i && !waiting[j])
		j = (j + 1) % n;
  
	if(j == i) // c.s에 들어가고자 하는 프로세스가 없다.
		lock = FALSE;
	else
		waiting[j] = FALSE;
	/* remainder section */
}
```



#### 2. Compare-and-swap

```c
while(true) {
	waiting[i] = true;
	key = 1;
	while (waiting[i] && key == 1)
		key = compare_and_swap(&lock,0,1);
	waiting[i] = false;
	
  /* critical section */
	
  j = (i + 1) % n;
	while ((j != i) && !waiting[j])
		j = (j + 1) % n; 
  if (j == i)
		lock = 0; 
  else
		waiting[j] = false;
  
	/* remainder section */
}
```













