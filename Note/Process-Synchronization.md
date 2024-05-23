---
Create Time: 11th April 2024
Title: "[[Process-Synchronization]]"
status: DONE
Author:
  - AllenYGY
tags:
  - Lec6
  - OS
  - NOTE
created: 2024-04-11T16:09
updated: 2024-05-23T19:43
---

# [[Process-Synchronization]]

- Background
- The Critical Section *临界区*
- Synchronization Software *软件实现同步*
- Synchronization Hardware *硬件实现同步*
- Mutex Locks *互斥*
- Semaphores *信号两*
- Classic Problems of Synchronization *同步问题*
- Monitors *管程*
- Deadlock and Starvation *死锁与饥饿*

## Background

Processes can execute concurrently *进程并发地执行*
- May be interrupted at any time, partially completing execution.
	- *如当一个进程时间片用尽，被另一个进程切换*
- Concurrent access to shared data may result in data inconsistency. 
	- *如果此时访问相同的数据，将导致数据不一致*

进程具有异步性。
异步性：各并发执行的进程以及各自独立的、不可预知的速度向前推进。

To solve this problem, we need to make processes access the same data mutually exclusive *令进程互斥地访问相同数据来解决数据不一致的问题*

## Critical Section *临界区*

> [!info]+ Critical Section
> - The segment of code in a process that modifies shared variables, tables, files
> - When one process is in critical section, other process should not enter their critical sections for these shared data. *互斥访问* 

一个时间段内只允许一个进程使用的资源称为临界资源。
临界资源只能互斥的访问。

对临界资源的互斥访问，可以在逻辑上分成四部分：
- Entry section *进入区*
	- Ask for Permission *申请权限*
- Critical section *临界区*
	- Protect this Section
- Exit section *退出区*
	- Do sth in order to allow other process to enter critical  section 
- Reminder section *剩余区*

> [!abstract]+ Process with critical section should follow the following steps 
> 1. execute entry section to ask for permission
> 2. then execute critical section
> 3. execute exit section to allow other process to enter critical section
> 4. then execute remainder section


```c
do {
	entry section
		critical section
	exit section 
		reminder section 
}
```


### Requirements & Solution to Critical Section Problem

#### Requirement

> [!success]+ Requirements to Critical Section Problem
> 1. Mutual Exclusion *互斥访问（忙则等待）*
> 	- If process $P_i$ is executing in its critical section, then no other processes can be executing in their critical sections.
> 2. Progress *空闲让进*
> 	- If no process is executing in its critical section and if other processes want to enter critical section, one of them must be selected. They cannot be postponed indefinitely.
> 3. Bounded Waiting *有限等待*
> 	- If a process has made a request to enter its critical section, then, before that request is granted, there is a bound to the times that others can enter critical section.

> [!info]- Assumptions in the solution 
> 1. Each process executes at a nonzero speed 
> 2. No concerning relative speed of the n processes 
> 3. The load and store machine-language instructions are atomic (that is, cannot be interrupted)

> [!quote]- 为了实现对临界资源的互斥访问，同时保证系统整体性能，需要遵循以下原则:
> 1. 空闲让进。临界区空闲时，可以允许一个请求进入临界区的进程立即进入临界区:
> 2. 忙则等待。当已有进程进入临界区时，其他试图进入临界区的进程必须等待;
> 3. 有限等待。对请求访问的进程，应保证能在有限时间内进入临界区(保证不会饥饿)
> 4. 让权等待。当进程不能进入临界区时，应立即释放处理机，防止进程忙等待。*(课件未提及)*

## Software Synchronization Solution 

> [!success]+ Solution to Critical Section Problem
> 1. 单标志法
> 2. 双标志先检查
> 3. 双标志后检查
> 4. Peterson算法

### Solution-I *单标志法*

turn’s value is initialized to be either i or j

- $P_i$

```cpp
do{
	while(turn==j); // Entry section
		critical section; // turn is i 
	turn = j; // Exit section
		reminder section; 
} while(true);
```

- $P_j$

```cpp
do{
	while(turn==i); // Entry section
		critical section; // turn is i 
	turn = i; // Exit section
		reminder section; 
} while(true);
```

- Mutual Exclusion： Yes 
- Progress： No 
- Bounded waiting： Yes

Can't Satisfy *Progress* *无法实现，空闲让进*

### Solution-II *双标志先检查*（课件未提及）

- 在进入临界区前先检查其他进程是否进入临界区 
	- 如果存在进程进程进入临界区，则等待
	- 否则，标志自己进入临界区
- 存在问题：当越过判断时发生进程切换，则无法互斥访问数据 *违反忙则等待*
- 原因：检查上锁无法连贯执行

```cpp
bool flag[2];
flag[0]=false;
flag[1]=false;
```

- $P_0$

```cpp
// For P0 Process
while(flag[1]);
flag[0] = true;
critical section;
flag[0] = false;
reminder section;
```

- $P_1$

```cpp
// For P0 Process
while(flag[0]);
flag[1] = true;
critical section;
flag[1] = false;
reminder section;
```

### Solution-III *双标志后检查*（课件未提及）

- 先上锁后检查以解决 *双标志先检查* 的问题

- 在进入临界区前先标志自己需要进入临界区，然后检查其他进程是否进入临界区 
	- 如果存在进程进程进入临界区，则等待
	- 否则，则进入临界区
- 存在问题：当标记时发生进程切换，多个进程都需要进入临界区---*导致死锁*
- 违反*空闲让进*，*有限等待*

```cpp
bool flag[2];
flag[0]=false;
flag[1]=false;
```

- $P_0$

```cpp
// For P0 Process
flag[0] = true;
while(flag[1]);
critical section;
flag[0] = false;
reminder section;
```

- $P_1$

```cpp
// For P0 Process
flag[1] = true;
while(flag[0]);
critical section;
flag[1] = false;
reminder section;
```

### Peterson’s Solution

It is a classic software-based solution to the critical-section problem

- Good algorithmic description of solving the problem 

Solution for two processes by using two variables:
-  int turn; // indicates whose turn it is to enter the critical section.  *表示谦让*  **最后谦让的无法执行**
-  boolean flag[2] // indicate if a process is ready to enter the critical section. *表达意愿*
	- flag[i] = true implies that process $P_i$ is ready! 
	- It is initialized to FALSE.

- $P_i$

```cpp
do{
	flag[i] = true; //ready
	turn = j; //allow pj to enter
	while(flag[j] && turn == j);
	critical section
	falg[i] = false; //exit
	reminder section 
} while(true);
```

- $P_j$

```cpp
do{
	flag[i] = true; //ready
	turn = j; //allow pj to enter
	while(flag[j] && turn == j);
	critical section
	falg[i] = false; //exit
	reminder section 
} while(true);
```

- Mutual Exclusion： Yes 
- Progress： Yes
- Bounded waiting： Yes

## Synchronization Hardware Solution

Software-based solutions are not guaranteed to work on modern computer architecture

Many systems provide hardware support for implementing the critical section code.

Modern machines provide special atomic hardware instructions 
Atomic = non-interruptible，the atomic hardware instruction will do the following work

### Disable interrupts *中断屏蔽方法*

*Uniprocessors* -  could disable interrupts  *单处理器 禁止中断 实现*
Currently running code would execute without preemption
- Too inefficient 

> [!abstract]+ Advantage & Disadvantage
> Advantage:
> - 简单
> 
> Disadvantage:
> - 不适用于多处理机
> - 只适用于操作系统内核进程，不适用于用户进程
> 	- 因为开/关中断指令只能运行在内核态，这组指令不能让用户随意使用


### Solution to Critical-section Problem Using Locks 

Use the idea of locking
- Protecting critical regions via locks
- A process that wants to enter the critical section must first get the lock.
- If the lock is already acquired by another process, the process will wait until the lock becomes available.
%% 	- Violate Progress *违反空闲让进*  %%

> [!success]- Solution to Critical-section Problem Using Locks
> ```cpp
> do {
> 	acquire lock; // Entry section
> 	critical section;
> 	release lock; // Exit Section 
> 	reminder section;
> }while(true);
> ```

1. Test memory word and set value
2. Swap contents of two memory words


### Test and Set Instruction

> [!tip]+ TS 指令
> Definition:  TS 指令由硬件实现的，只能一气呵成。


```cpp
bool test_and_set(bool *target); // Do nothing just wait;
	/*Critical  section*/	
 	bool rv = *target；
 	*target = true;
 	return rv;
```
- Using Test and Set:
```cpp
do {
	while(test_and_set(&lock));
	/*Critical  section*/	
	lock = false;
	/*Reminder Section*/
} while(true);
```

1. This instruction is executed atomically by CPU as a single hardware instruction.
2. In practice, target is a pointer to the lock itself, shared by all the processes that want to acquire the lock.
   - If target if FALSE, the return value of `rv` is FALSE, means lock is FALSE (available), target’s new value is TRUE 
   - If target is true, the return value of `rv` is TRUE, means lock is TRUE (locked)

---

- When lock = true, keep while looping.
- When lock = false, process can enter the critical section
- And set lock = true, block other processes to enter.
- After finish the critical section, reset lock = false, to allow other processes to enter the critical section.

> [!abstract]+ Advantage & Disadvantage
> Advantage:
> - 实现简单
> 
> Disadvantage:
> - 不满足“让权等待”
> - 暂时无法进入临界区的进程会占用CPU并循环执行TSL指令，从而导致 “忙等


- Mutual Exclusion： Yes 
- Progress： Yes 
- Bounded waiting： No **?**


### Compare and Swap Instruction 

`Exchange Instruction `/`XCHG Instruction `

> [!tip]+ Swap 指令
> Definition:  TS 指令由硬件实现的，只能一气呵成。
> 1. In practice, value is a pointer to the lock itself, shared by all the processes that want to acquire the lock.
> 2. Set 	`*value` (the lock) to the value of the passed parameter new_value but only if `*value` == expected. That is, the swap takes place only under this condition.

Returns as result the original value of the lock. Similar to test_and_set but with an integer lock and an extra condition.

```cpp
int compare_and_swap (int *value, int expected, int new_value){
 	
 	int temp = *value;
 	if(*value==expected)
 		*value=new_value;
 	return temp;
 }
```
> Lock: 上锁

```cpp
bool 
while(test_and_set(&lock));
lock = false;
```

---

## Mutex Lock *互斥锁*

`Previous hardware-based solutions are complicated and generally inaccessible to application programmers`

OS designers build `high level` software tools to solve critical section problem
> [!info]+ Mutex Lock
> - Use mutex lock to protect a critical section by first
> 	- acquire() a lock then release() the lock
> - Assumption: Calls to acquire() and release() must be atomic
> 	-	Usually implemented via hardware atomic instructions

```cpp
acquire (){
	while(!available);
	available = false;
}

release (){
	available = true;
}
```

This solution still requires busy waiting  This lock therefore is called a spin lock
- *忙等待* 违反让权等待
- *spin lock*  TSL, SWAP, 单标志法

> 需忙等，进程时间片用完才下处理机，违反让权等待
> 一般用于多处理器，一个核忙等，其他核照常工作，并快速释放临界区
> 不太适用于单处理机系统、忙等的过程中不可能解锁

## Semaphore *信号量*

> [!info]+ Background 
> Semaphore is a synchronization tool more sophisticated than mutex locks 
> - 用户进程可以通过使用操作系统提供的一对原语来对信号量进行操作，从而很方便的实现了进程互斥、进程同步。 
> 	- 从而很方便的实现了进程互斥、进程同步。
> 	
> *信号量*其实就是一个变量(可以是一个*整数*，也可以是更复杂的*记录型变量*)，可以用一个信号量来表示系统中某种资源的数量
> - 比如:系统中只有一台打印机，就可以设置一个初值为1的信号量
> 
> 原语是一种特殊的程序段，其执行只能一气呵成，不可被中断。**原语是由关中断/开中断指令实现的。** 软件解决方案的主要问题是由 **“进入区的各种操作无法一气呵成”**，因此如果能把进入区、退出区的操作都用“原语”实现，使这些操作能“一气呵成”就能避免问题。
> - 一对原语:wait(s)原语和signal(s)原语，可以把原语理解为我们自己写的函数，函数名分别为wait和signal，括号里的信号量S其实就是函数调用时传入的一个参数。
> - *Wait、signal* 原语常简称为P、V操作(来自荷兰语`proberen`和`verhogen`)。


>POSIX Named Semaphore Normally used among **different processes**
>POSIX Named Semaphore Normally used among **different threads within a process**

### *整型信号量*

Semaphore S: an integer variable S can only be accessed via two atomic operations

```cpp

int S=1;

//acquire lock
void wait(int S){
	while(S<=0){
		S-=1;
	}
}
//release lock
void signal(int S){
	S+=1;
}

```

- 仍旧*Busy waiting*
- 无法让权等待

### *纪录型信号量*

```cpp
typedef struct{
	int value;
	struct process* L;
} semaphore;

// acquire lock
void wait(semaphore S){
	S.value--;
	if(S.value < 0){
		block(S.L); //阻塞该进程
	}
}

//release lock
void signal(semaphore S){
	S.value++;
	if(s.value<=0){
		wakeup(S.L); //唤醒进程为就绪态
	}
}

```

### Semaphore mechanism implements process mutual exclusion, synchronization and precursor

`信号量机制实现进程互斥、同步、前驱`
$P(S)$ 申请一个资源`S`, 如果资源不够就阻塞等待
$V(S)$ 释放一个资源`S`, 如果有进程在等待该资源，则唤醒一个进程

### Process mutual exclusion `信号量机制实现进程互斥`

1. 分析并发进程的关键活动，划定临界区
2. 设置互斥信号量mutex，初值为1
3. 进入区P(mutex) -- 申请资源 
4. 退出区V(mutex) -- 释放资源

```cpp
semaphore mutex=1;

P1(){
	P(mutex);
	...;
	V(mutex);
}

P2(){
	P(mutex);
	...;
	V(mutex);
}

```

> [!warning]- P, V操作必须成对出现
> - 缺少P(mutex) 就不能保证临界资源的互斥访问。
> - 缺少V(mutex) 会导资源永不被释放，等待进程永不被唤醒。

### Process synchronization `信号量机制实现进程同步`

1. 分析什么地方需要实现“同步关系”，即 “一前一后” 执行的两个操作
2. 设置同步信号量`S`，初始为0
3. 在`前操作`之后执行 `V(S)`
4. 在`后操作`之前执行 `P(S)`
	- `前V后P`

```cpp
semaphore S=0;
P1(){
	...;
	V(S);
	...;
}
P2(){
	P(S);
	...;
	...;
}
```

保证了只能先执行`P1()`再执行	`P2()`

### Process precursor `信号量机制实现进程前驱`

![Process precursor](https://cdn.jsdelivr.net/gh/ALLENYGY/ImageSpace@master/IMAGE/OS/Process_Synchornization_Semaphore.png)

## Classic Problems of Synchronization

- Producer consumer problem *Bounded-Buffer Problem*
- Readers Writers Problem 
- Dining Philosophers Problem

### Producer consumer problem `生产者消费者问题`

> [!question]+ Producer-Consumer problem 
> - n buffers, each holds one item 
> - Semaphore mutex initialized to the value 1 
> - Semaphore full initialized to the value 0 
> - Semaphore empty initialized to the value n

> [!info]- 生产者消费者问题 
> - 系统中有一组生产者进程和一组消费者进程，生产者进程每次生产一个产品进程每次从缓冲区中取出一个产品并使用。(注:这里的“产品”理解为某种数据)
> 生产者、消费者共享一个初始为空、大小为n的缓冲区。
> 只有缓冲区没满时，生产者才能把产品放入缓冲区，否则必须等待。 **同步关系**
> 只有缓冲区不空时，消费者才能从中取出产品，否则必须等待。 **同步关系**
> 缓冲区是临界资源，各进程必须互斥地访问。 **互斥关系**
> ![Producer consumer problem](https://cdn.jsdelivr.net/gh/ALLENYGY/ImageSpace@master/IMAGE/OS/Producer-Consumer-Problem.png)

```cpp
semaphore mutex = 1; // 互斥信号量，实现对缓冲区的互斥访问
semaphore empty = n; // 同步信号量，表示空闲缓冲区的数量
semaphore full = 0; // 同步信号量，表示产品的数量，也即非空缓冲区的数量

producer(){
	while(1){
		...;
		P(empty);//消耗一个空闲缓冲区
		P(mutex);
		...;
		V(mutex);
		V(full); //增加一个产品
	}
}

consumer(){
	while(1){
		P(full); //消耗一个产品
		P(mutex);
		...;
		V(mutex);
		V(empty); //增加一个空闲区
	}
}
```

> [!tip]+ 同步关系在线程之间实现，互斥关系在线程内实现

#### Multi-Producer and Consumer Problem 

![Multi-Producer and Consumer Problem](https://cdn.jsdelivr.net/gh/ALLENYGY/ImageSpace@master/IMAGE/OS/Multi-Producer-Consumer-Problem.png)

### Smokers Problem `吸烟者问题`

> [!question]+ Smokers Problem
> 1. 假设一个系统有三个抽烟者进程和一个供应者进程。
> 2. 每个抽烟者不停地卷烟并抽掉它，但是要卷起并抽掉一支烟，抽烟者需要有三种材料:烟草、纸和胶水。
> 3. 三个抽烟者中，第一个拥有烟草、第二个拥有纸、第三个拥有胶水。
> 4. 供应者进程无限地提供三种材料，供应者每次将两种材料放桌子上，拥有剩下那种材料的抽烟者卷一根烟并抽掉它，并给供应者进程一个信号告诉完成了，供应者就会放另外两种材料再桌上，这个过程一直重复(让三个抽烟者轮流地抽烟)
> ![Smokers Problem](https://cdn.jsdelivr.net/gh/ALLENYGY/ImageSpace@master/IMAGE/OS/Smokers-Problem.png)

```cpp
semaphore offer1 = 0;
semaphore offer2 = 0;
semaphore offer3 = 0;
semaphore finish = 0;
int i = 0;

provider(){
	while(1){
		if(i==0){
			...;// put Combination-1
			V(offer1);
		} else if(i==2){
			...;// put Combination-2
			V(offer2);
		} else if(i==3){
			...;// put Combination-3
			V(offer3);
		}
		i=(i+1)%3;
		P(finish);
	}
}

smoker1(){
	while(1){
		P(offer1);
		...;// Get Combination-1
		V(finish);
	}
}


smoker2(){
	while(1){
		P(offer2);
		...;// Get Combination-2
		V(finish);
	}
}


smoker3(){
	while(1){
		P(offer3);
		...;// Get Combination-3
		V(finish);
	}
}

```



### Readers Writers Problem 

> [!question]+ Readers-Writers Problem
> - Allow multiple readers to read at the same time 
> - Only one single writer can access the shared data at the same time

> [!info]- Readers Writers Problem `读者写者问题`
> 有读者和写者两组并发进程，共享一个文件，当两个或两个以上的读进程同时访问共享数据时不会产生副作用，但若某个写进程和其他进程(读进程或写进程)同时访问共享数据时则可能导致数据不一致的错误。
> 1. 允许多个读者可以同时对文件执行读操作;
> 2. 只允许一个写者往文件中写信息;
> 3. 任一写者在完成写操作之前不允许其他读者或写者工作;
> 4. 写者执行写操作前，应让已有的读者和写者全部退出。

互斥关系：读进程-写进程，写进程-写进程 

```cpp
semaphore rw=1; // 用于实现对共享文件的互斥访问
int count=0;   // 记录当前有几个读进程在访问文件
semaphore mutex; // 用于保证对count变量的互斥访问
```

```cpp
writer(){
	while(1){
		P(rw);
		...;
		V(rw);
	}
}
```

```cpp
	while(1){
		P(mutex); // 各读进程互斥访问count
		if(count==0) // 第一个读进程关门
			P(rw);
		count++;
		V(mutex);
		P(mutex);
		count--;
		if(count==0) // 最后一个读进程开门
			V(rw);
		V(mutex);
	}
```

> 这样的实现存在问题：
> 如果一直有读进程进入那么写进程将会 **starvation**
> 需要实现读写公平的算法

```cpp
semaphore rw = 1; // 用于实现对共享文件的互斥访问
int count = 0;   // 记录当前有几个读进程在访问文件
semaphore mutex; // 用于保证对count变量的互斥访问
semaphore w = 1; //实现读写公平 可以理解成排队
```

```cpp
writer(){
	while(1){
		P(w);  //谁先抢到 谁先排队
		P(rw);
		...;
		V(rw);
		V(w);
	}
}
```

```cpp
	while(1){
		P(w);
		P(mutex); // 各读进程互斥访问count
		if(count==0) // 第一个读进程关门
			P(rw);
		count++;
		V(mutex);
		V(w);
		P(mutex);
		count--;
		if(count==0) // 最后一个读进程开门
			V(rw);
		V(mutex);
	}
```

> [!success]+ **Case Analysis**
> 1.  读者1 --> 读者2
> 读者2会被阻塞在`P(w)`, 直到读者1 `V(w)` 
> 2. 写者1 --> 写者2
> 写者2会被阻塞在写者线程的`P(w)`, 直到写者1 `V(w)` 
> 3. 写者1--> 读者1
> 读者1会被阻塞在读者线程的`P(w)`, 直到写者1 `V(w)` 
> 
> 总的来说，新增的 `w` 信号量实现了一个排队的功能，读者和写者都可以排队

### Dining Philosophers Problem

> [!question]+ Dining Philosophers Problem
> 1. Philosophers spend their lives alternating thinking and eating Don’t interact with their neighbors, occasionally try to pick up 2 chopsticks (one at a time) to eat from bowl 
> 2. Need both chopsticks to eat, then release both when done In the case of 5 philosophers 
>  - Shared data 
>  	- Bowl of rice (data set)
>  	- Semaphore chopstick [5] initialized to 1
> ![Dining Philosophers Problem](https://cdn.jsdelivr.net/gh/ALLENYGY/ImageSpace@master/IMAGE/OS/Dining_Philosophers_Problem.png)

> [!info]- 哲学家进餐问题
> 1. 一张圆桌上坐着5名哲学家，每两个哲学家之间的桌上摆一根筷子，桌子的中间是一碗米饭。
> 2. 哲学家们倾注毕生的精力用于思考和进餐，哲学家在思考时，并不影响他人。
> 3. 只有当哲学家饥饿时，才试图拿起左、右两根筷子(一根一根地拿起)。
> 4. 如果筷子已在他人手上，则需等待。
> 5. 饥饿的哲学家只有同时拿起两根筷子才可以开始进餐，当进餐完毕后，放下筷子继续思考。

```cpp
semaphore chopstick[5]={1,1,1,1,1};
Pi (){ //i 号哲学家的进程
	while(1){
		P(chopstick[i]); //Take Left chopstick 
		P(chopstick[(i+1)%5]); //Take Right chopstick 
		...;
		V(chopstick[i]); //Put Left chopstick 
		V(chopstick[(i+1)%5]); //Put Right chopstick 
		...;
	}
}
```

如果每个哲学家同时拿起左边的筷子, 将导致死锁

> [!success]+ Solution
> 1. 每次只允许4个哲学家拿筷子，这样最后至少会有一个哲学家可以进餐。
> `新定义一个等于4的信号量`
> 2. 奇数号哲学家拿左边筷子，偶数号拿右边筷子
> 3. 仅当一个哲学家左右两双筷子都可以使用时才能拿起筷子

> [!abstract]+ 解释仅当一个哲学家左右两双筷子都可以使用时才能拿起筷子
> 更准确的说法应该是:各哲学家拿筷子这件事必须互斥的执行。
> - 这就保证了即使一个哲学家在拿筷子拿到一半时被阻塞，也不会有别的哲学家会继续尝试拿筷子。
> - 这样的话，当前正在吃饭的哲学家放下筷子后，被阻塞的哲学家就可以获得等待的筷子了。

```cpp
semaphore chopstick[5]={1,1,1,1,1};
semaphore mutex = 1; //互斥地取筷子
Pi (){ //i 号哲学家的进程
	while(1){
		P(mutex);
		P(chopstick[i]); //Take Left chopstick 
		P(chopstick[(i+1)%5]); //Take Right chopstick 
		V(mutex);
		...;
		V(chopstick[i]); //Put Left chopstick 
		V(chopstick[(i+1)%5]); //Put Right chopstick 
		...;
	}
}
```


## Deadlock and Starvation *死锁与饥饿*

>[!info]+ Deadlock - Processes wait for each other

>[!info]+ Starvation – indefinite blocking 
>A process may never be removed from the semaphore queue in which it is suspended

>[!info]+ Priority Inversion – Scheduling problem when lower-priority
>Process holds a lock needed by higher-priority process

### Priority Inversion
> [!example]+ **Priority Inversion Example Explained:**
> 1. **Process Priorities and Resource Requirement:**
>    - Assume three processes: $L$, $M$, and $H$ with priorities $L < M < H$.
>    - Process $H$ requires a resource $R$ which is currently held by process $L$.
> 2. **Scenario Development:**
>    - Process $H$ must wait for $L$ to finish using the resource $R$ because $L$ has it locked.
>    - However, process $M$ becomes runnable and, due to its higher priority compared to $L$, it preempts $L$.
> 3. **Consequence of Priority Inversion:**
>    - Process $M$ (with middle priority) has affected how long process $H$ must wait for $L$ to relinquish resource $R$.
>    - Even though $M$ does not need $R$, its operation delays $L's$ release of $R$, thereby delaying $H$ further, which is a clear instance of priority inversion.

This situation shows how a lower-priority task (in this case $L$) can indirectly prevent a higher-priority task $H$ from progressing by holding onto a needed resource longer due to the intervention of a medium-priority task ($M$). This is problematic in real-time systems where such delays can lead to failures or missed deadlines. Solutions often involve using priority inheritance protocols where $L$ would temporarily inherit $H's$ higher priority to avoid being preempted by $M$, thus resolving the inversion more swiftly.

> [!example]- **优先级反转示例解释：**
> 1. **进程优先级和资源需求：**
>    - 假设有三个进程：$L$、$M$ 和 $H$，它们的优先级依次为 $L < M < H$。
>    - 进程 $H$ 需要一个资源 $R$，该资源当前由进程 $L$ 持有。
> 2. **场景发展：**
>    - 进程 $H$ 必须等待 $L$ 完成使用资源 $R$，因为 $L$ 正在锁定该资源。
>    - 然而，进程 $M$ 变为可运行状态，并由于其优先级高于 $L$，它抢占了 $L$。
> 3. **优先级反转的后果：**
>    - 中等优先级的进程 $M$ 影响了高优先级的进程 $H$ 必须等待低优先级的 $L$ 释放资源 $R$ 的时间。
>    - 尽管 $M$ 不需要 $R$ 资源，其操作延迟了 $L$ 释放 $R$，进而延迟了 $H$，这就是一个典型的优先级反转的例子。
>
> 这种情况显示了一个低优先级任务（在这个例子中是 $L$）如何因为持有所需资源而间接阻止高优先级任务（$H$）的进行，由于中优先级任务（$M$）的介入使得 $L$ 持有资源的时间延长。这在实时系统中是有问题的，因为这样的延迟可能导致故障或错过截止时间。解决方案通常涉及使用优先级继承协议，其中 $L$ 会暂时继承 $H$ 的更高优先级，以避免被 $M$ 抢占，从而更快地解决反转问题。

> [!success]+ Solution:  **Priority-Inheritance Protocol Explained:**
> 
> 1. **Protocol Basics:**
>    - All processes accessing resources needed by a higher-priority process inherit the higher priority until they are finished with the resources in question.
>    - When these processes complete their tasks involving the resource, their priorities revert to their original values.
> 2. **Problem Resolution:**
>    - By allowing lower-priority processes (like process $L$ in the earlier example) to inherit the higher priority of a blocked higher-priority process (like process $H$), the system reduces the chance that a medium-priority process (like process $M$) will preempt them.
>    - This inheritance ensures that the lower-priority process can complete its use of the resource more quickly, thereby freeing up the resource for the higher-priority process.
> 3. **Effectiveness:**
>    - This protocol effectively resolves the problem where a lower-priority process blocks a higher-priority one by holding a needed resource, as seen in the previous examples.
>    - It reduces the wait time for high-priority processes, ensuring they can proceed with minimal delay, thus adhering more closely to their intended scheduling priorities.

> [!abstract]- 优先级继承协议
> - 在优先级继承协议中，当一个低优先级进程使用一个高优先级进程所需的资源时，低优先级进程的优先级会被临时提升到高优先级进程的级别。
> - 这样做的目的是防止中等或其他低优先级的进程在此期间抢占资源，从而允许低优先级进程更快地完成对资源的使用并释放它，这样高优先级的进程就可以不受延迟地访问所需的资源。


## Monitors *管程*

> [!info]+  Monitors `管程`
> - A high-level abstraction that provides a convenient and effective mechanism for process synchronization
> 	- Abstract data type, internal variables only accessible via procedures 
> 	- Only *one process may be active* within the monitor at a time 
> 	- Can utilize condition variables to suspend or resume processes

```cpp
monitor monitor-name { // shared variable declarations
	procedure P1 (...) {...} 
	procedure Pn (...) {...} 
	Initialization code (...) {...}
}
```

- A programmer who needs to write a tailor-made synchronization scheme can define one or more condition variables.
