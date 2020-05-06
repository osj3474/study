# System V

0. include

```c
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int shmid;
void *shared_memory = (void *)0;  // 1.void형:주소저장(값변경불가) 2.(void*)0 : nullptr

shmid = shmget((key_t)1234, sizeof(int), 0666|IPC_CREAT);
shared_memory = shmat(shmid, (void *)0, 0); // 

if (shared_memory == (void *)-1)
{
  printf("shmat failed : ");
  exit(0);
}

```



1. allocate

```c
int shmget(key_t key, int size, int shmflg);

[param]
1. key : option
2. size : Shared Memory 크기 
3. shmflg : flag
return : Shared Memory 고유ID
  
ex) seg_id = shmget(IPC_PRIVATE, sizeof(int), S_IRUSR|S_IWUSR);
```



2. attach

```c
void* shmat(int shmid, char* shmaddr, int shmflg);

return : Process내의 매핑된 첫번째 주소
  
ex) shared_mem = (char*)shmat(seg_id, NULL, 0);  // 사용할 때는 이렇게 type casting해서 사용
```



3. write

```c
ex) sprintf(shared_mem, "Hello world");
```



4. detach

```c
int shmdt(char* shmaddr);

ex) shmdt(shared_mem);
```



5. num_attach

```c
shm_nattach
```



6. deallocate

```c
shmctl(shmid, IPC_RMID, NULL);
```



