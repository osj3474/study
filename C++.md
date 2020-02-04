### [앞으로 내가 할 일]

1. 교수님 코드를 받아서 Header파일만 남기고 나머지 모든 코드를 직접 코딩할 수 잇도록 하기!
2. 기존 예외처리 코드 분석 => try & catch
3. 특정 클래스 예외처리 코드 추가, 코멘트 쓰기, PPT 만들기
4. Clone(Deep copy, Shallow Copy) 추가
5. Distribution learning코드 짜기 (병렬처리)
6. Master Slave 모델 

지도교수 팀원 구성 프로젝트 주제 사용할 기술 현재까지 진척도 향후 계획 팀원과의 분배

김인중 일심동체 WICWIU(딥러닝 오픈소스 프레임워크) Exception Handling 딥러닝과 C++

코드 분석 중, try&catch 추가  일심동체 







# C++ 공부



## [ 개념 ]

##### 0. 전처리문 정리



##### 1. #ifdef  => 'n' 은 no이다.

```c++
#ifdef TEST       // TEST가 #define으로 정의되어 있으면 실행
#endif

#ifndef TEST     // TEST가 정의 안되어 있으면 실행
#endif
```



##### 1-1. #define

```c++
단순히 변수 하나를 숫자처리 해주는게 다가 아니다.
```

![1556898632897](C:\Users\User\AppData\Roaming\Typora\typora-user-images\1556898632897.png)



##### 2. #include할 때, <>와  ""

```c++
#include <>   // 시스템 디렉토리에서 include
#include ""   // 현재 디렉토리에서 include
```

cf) 참고로 main함수는 iostream에 있다.



##### 3. inline 함수란?

 : header파일에서 선언과 동시에 구현해두는 것.



##### 4. header파일을 사용하는 이유

 : 우선 헤더파일이란 function prototype(함수를 미리 정의해두는 것)을 모아둔 것이다. 사용하는 이유는 재활용을 위해서.



##### 5. reference, 별명 붙이기

```c++
int a = 3;
int &k = a;   // k는 a랑 완전 똑같아지는 거임
int &k = 3;   // 이거는 안된다. 둘다 rvalue(메모리주소가 없는것)라서..
```



##### 6. 컴파일

```c++
/*
	.cpp파일  =>(컴파일)=> .o파일  =>(link)=>  .exe파일  
*/


// 가령, 파워쉘에서
=> g++ -c hello.cpp
// 하면 compile만 하는 것으로 .o파일 생성됨
```



##### 7. enum : 사용자정의 자료형 中 열거형

  =>   특정한 상태 집합을 나타내야 할 때 코드 문서화 및 가독성 목적으로 매우 유용하다.

```c++
enum DeepLearning{
    CNN,   // 순서대로 정수값이 할당된다.(default 0)
    RNN,
    GAN,
}; // 이 상태는 메모리할당X

DeepLearning d1 = CNN;  // 메모리할당
```



##### 8. magic number란?

 : 설명 없이 등장하는 상수

ex) 학생의 학적 상태를 "1이면 재학, 2이면 휴학, 3이면 제적, 4이면 졸업"으로 나타낸다고 가정하자. 이 경우, "이 학생이 졸업했는가"를 따지기 위해 "학생.상태 == 1" 따위로 나타낼 경우에 1,2,3 이 매직넘버에 해당된다.

그래서 **7.의 enum**을 사용해주는 것이 가독성을 높이는데 좋다. 



##### 9. argc와 argv

```c++
int main(int argc, char *argv[]){}

// argc : 콘솔 창에 입력되는 문자의 갯수
// argv : 콘솔 창에 입력되는 문자의 배열(char array로 받아야함)
```



##### 10. prompt에 대하여

```c++
int GetInt(std::string prompt = "Enter an integer: ");
// GetInt할 때는 prompt가 필요한데,
// = 부터는 optional argument로, GetInt()도 되고, GetInt(" sdf ")도 된다는 것.




 int GetInt(std::string prompt = "Enter an integer: ") {  
   int keyin;
   std::string line;
   std::cout << prompt;                   // display a prompt
   while (true) {
     if (std::getline(std::cin, line)) {  // get an input
       std::stringstream sstr(line);      // make sstream obj
       if (sstr >> keyin) break;          // scan for int
     }
     std::cout << "Retry: ";              // not an int, retry
   }
   return keyin;
 }
```



##### 11. 컴파일할 때,

 : include는 폴더만 알려주면 되고
library는 폴더랑 파일명도 알려줘야한다.

```powershell
g++ selection.cpp insertion.cpp quicksort.cpp bubble.cpp sortDriver2.cpp -I../../include -L../../lib -lnowic -o sort2
```

만약에 .o파일을 거쳐서 .a파일을 만들었다면 

예시로, libsort.a 파일을 만들었다면 '-l' 다음에 파일명이 오는데 lib 빼고 'sort'만 해서 링크를 해야한다. 

```
g++ sort.cpp -I../../include -L../../lib -lsort
```





##### 12. Static Library

 : library란 미리 컴파일 되는 object file의 모음이다. (Static library와 Shared library가 있다.)

1) Static library는 유닉스에서는 .a(아카이브) 확장자를 가지고, 윈도우에서는 .lib(라이브러리) 확장자를 가진다.

2) Shared library는 유닉스에서는 .so(shared object) 확장자를 가지고, 윈도우에서는 .dll(dynamic link library) 확장자를 가진다. (참고로 g++은 기본적으로 shared library를 링크한다. )

##### [중요] Static library(.a파일) 만들기

 : nowic.cpp 파일을 (.o파일을 거쳐서) libnowic.a 파일로 만들것이다.

```powershell
g++ -c nowic.cpp -o nowic.o -I../include
ar rcs libnowic.a nowic.o     // ar rcs로 library이름 object이름 해서 static library 만듬
ar 				         // list all the options available(쓸수 있는 명령어 보여줌)
ar t libnowic.a 	     // list ~.o files archived(a파일이 가지고 있는 o파일들을 보여줌)
ar x libnowic.a 	     // extract ~.o files archived
nm nowic.o 		         // list the actual function names in .o file
```

cf ) ar flags:

- -r: Insert the files member... into archive (with replacement).
- -c: Indicates you are creating an archive file
- -s: Write an object-file index into the archive, change is made to the archive
- struc-t: display contents of archive (show the list of .o files, use nm ~.o to see functions in ~.o)



##### 13. 벡터(vector) 사용하기  : array의 확장

```c++
#include <vector>
using namespace std;

vector<int> v;  // 배열(array)와는 다르게 delete를 해줄 필요가 없다.
vector<int> v(10);
```



cf) size_t 타입

 : size_t 타입은 C언어에서 임의의 객체가 가질 수 있는 최대 크기를 나타내며, 부호없는 정수를 사용하며, unsigned int로 쓰이는 경우도 있다.

##### 13-1. vector 자주 사용되는 함수들

##### (1) .at

```c++
v.at(index)랑 v[index]랑 같다
```

##### (2) std::out_of_range

```c++
#include <iostream>       // std::cerr
#include <stdexcept>      // std::out_of_range
#include <vector>         // std::vector

int main (void) {
  std::vector<int> myvector(10);
  try {
    myvector.at(50)=100;      // vector::at throws an out-of-range
  }
  catch (const std::out_of_range& oor) { 
    std::cerr << "Out of Range error: " << oor.what() << '\n';
  }
}

// oor은 어떤 error인지 출력해주는 것이다.
```



##### (3)  reserve  (reverse 아님!!!)

 : 인수만큼 메모리를 할당함.



##### (4) size

 : 벡터 원소 갯수



##### (5) push_back

 : 벡터 뒤에 (인수) 추가



##### 14. 템플릿 (클래스 편)

```c++
template<typename DTYPE>    // 클래스를 템플릿으로 융통성을 높이겠다. 이때는 Tensorholder클래스를 그렇게 만들겠다는 뜻이다.
class Tensorholder : public Operator<DTYPE>{  // 상속받는 Operator의 타입을 지정해도 되지만 여기서도 DTYPE으로 두어 융통성을 높이고 있다.
    Tensorholder(int pTimeSize, int pBatchSize, int pChannelSize, int pRowSize, int pColSize, std::string pName, int pTrainable = TRUE) : Operator<DTYPE>(pName) {
    } 	    // 생성자
   	~Tensorholder() {
	}       // 소멸자
}

// 실제로 main에서 클래스 생성해서 사용할 때는 다음과 같다.
Tensorholder<float> *x = new Tensorholder<float>(1, BATCH, 1, 1, 784, "x"); // Tensorholder를 float타입으로 사용하겠다.
```



##### 15. linked list(링크드 리스트)

```c++
struct Node{
  int item;
  Node *next;

  Node(int i, Node *n){
    item = i;
    next = n;
  }

  ~Node(){}
};

using pNode = Node*;

bool empty(pNode p){
  return p == nullptr;
}

pNode find(pNode p, int val){
  if(empty(p)) return nullptr;  // 애초에 노드 안에 아무것도 없으면 찾지도 마라.
  pNode x = p;  // x => p1->data, p1->next => p2->data
                // 즉, x는 그 다음 노드의 data를 포인팅 하고 있는 현재 노드의 next값이다.
  while(x != nullptr){   // x == nullptr이라는 말은 그 다음 노드가 없다. 현재 노드의 next값이 nullptr이다.
    if(x->item == val) return x;   // 이 때의 x는 내가 찾는 값이 있는 노드를 가리키는 그 전 노드의 next값이다.
    x = x->next; // x가 가리키는 data가 있는0, 즉 다음 노드의 next값으로 x를 바꿈
  }
  return x; // 결국, 리턴 값은 찾으려는 값을 가리키는 주소거나, nullptr이다.
}
```



##### 16. auto타입

```c++
auto a = 0;  // a의 타입을 보고 알아서 타입을 결정
```



##### 17. const

```c++
int get_size const{
    return size;
}  // size값은 리턴 받아서도 바꿀수 없다.
```



##### 18. 생성자와 소멸자 확실히 알자

 : new를 하면 생성자가 호출되고, delete를 하면 소멸자가 호출된다. 그런데, 가상함수를 정의할 때는 소멸자도 가상함수로 만들어야 한다. 이 개념이 이해가 되기 위해서는 상위 클래스 포인터 타입으로 하위 클래스를 동적할당 하는 것을 선이해 해야한다. 이때, 등장하는 것이 Contaioner개념이다. 



##### 18-2. Container

 :  클래스 템플릿으로, 같은 타입의 여러 객체를 저장하는 일종의 집합

1. Sequence Container(데이터를 선형으로 저장하며, 특별한 제약이나 규칙이 없는 가장 일반적인 컨테이너)

    => array, vector, list, deque

2.  Associative Container(데이터를 일정 규칙에 따라 조직화하여 저장하고 관리하는 컨테이너)

    => set, multiset, map, multimap





##### 18-3. promotion

 : 부모 클래스 타입으로 자녀 타입클래스 생성

=> 어쨋든 new로 생성하잖아. 다시말해서 

```c++
int *a = new int[5];   // int타입 공간을 5개 만들고 그 array를 a가 가리킴
```

 여기서 a라는 포인터가 적어도 자기가 가리키고 있는 element보다는 상위개념이어야 품을 수 있다? 부모 클래스 타입으로 하위 클래스를 new하는 것은 자연스러운 일이다. 단, JAVA에서는 

ex) Person p = new Student

이렇게 사용한다면 Student안에 있는 고유함수에 접근하려면 원래의 타입으로 변경해줘야한다.(타입 캐스팅) 기본적으로 Person에서 해당 함수를 찾기 때문!



```c++
Base *b = new Derived();  // Derived라는 객체를 생성했고 이것을 가리키는 포인터 변수 b를 정의한다.
```





##### 19. Semaphore & mutex

 : critical-section(thread가 공통 변수를 바꾸려는 구간) 문제을 해결하기 위한 solution이 동기화이다. (동기화 툴은 Semaphores, Monitors, ..)

 : 무슨 말이냐면!! thread1이 변수 a의 값을 바꾸려고 하는데, thread2도 변수 a에 접근해서 값을 바꾸려고 하는 중이다. 이 때, 변수 a를 바꾸는 구간의 코드를 ciritical-section(CS)라고 하자. CS에는 하나의 thread만 들어가서 작업을 해야 문제가 생기지 않을 것이다. 그래서 Semaphore(Sem)라는 개념을 도입하는데 Sem의 값이 음수가 되면 CS에 접근하지 못하도록 하여 위의 문제를 해결하는 것이다. Sem값을 처음에 1로 두고, thread가 CS구간에 들어가기 전에 Sem값을 -1해주고, CS구간을 나오면 Sem값을 +1해주는게 알고리즘이다.

1. Mutual exclusion(상호베타) : 오직 한 thread만 진입
2. Progress(진행) : 진입 결정은 유한 시간 내
3. Bounded waiting(유한대기) : 어느 thread라도 유한 시간 내



busy waiting : Sem값 때문에 계속 while문 돌고있는 상황

block : 프로세스를 waiting Q에 두는 것

wakeup : 프로세스를 ready Q로 이동하는 것



##### 20. 가상 소멸자 // virtual ~CLASS();

 : 클래스에 가상 함수를 정의할 때는 소멸자도 가상함수로 만들어야 한다. 가령, 

```c++
class Student : public Person{
    
}

Person p = new Student;
```

라고 한다면, Person에서 virtual함수가 있어서 Student에서 virtual을 정의했다면 Person에서 소멸자도 virtual로 해주어야 나중에 p를 delete할 때, Student를 먼저 delete하고, Person을 delete한다. 쉽게 말해서, 상속받은 클래스도 완전히 해제하기 위한 하나의 스킬이다.





##### 21. overriding(재정의)  vs.  overloading(다형성)

 Overriding => 하위 클래스에서 특성이 다를 때, 같은 이름으로 재정의 하는 것.(상속 기반)

 Overloading => 함수의 이름은 같게!! 하면서 함수 안에 들어가는 인자 값을!! 다르게 해서 융통성을 높이는 것. (하나의 클래스 내에서 일어나는 일)





##### 22. 복사 생성자(copy constructor)

 : 생성자 중에 본인 클래스를 파라미터로 받는 생성자. 이 때, 파라미터로 받는 클래스는 const로 받기.

cf) 그런데 복사 생성자를 정의하지 않으면, 멤버 대 멤버의 복사를 진행하는 복사 생성자가 자동으로 삽입된다. 이를 디폴트 복사 생정자라고 하는데, 이는 얕은 복사(shallow copy)가 되어 소멸과정에서 문제가 발생할 수 있다.



cf) explicit는 객체를 생성시 초기화 값을 줄때, '=' 을 사용할 수 없다.



```c++
// class A 가 있다고 가정했을 때, public에서 복사 생성자

A(const A &copy)
  : num(copy.num)
{
    
}
```



 : 호출시점은 다음과 같다.

1. 기존에 생성된 객체를 이용해서 새로운 객체를 초기화하는 경우
2. call-by-value  방식의 함수호출 과정에서 객체를 인자로 전달하는 경우
3. 객체를 반환하되, 참조형으로 반환하지 않는 경우



상속받는 상황으로도 확인해보자

```c++
// class Student가 Person을 상속 받는다고 가정했을 때, Student의 public에 복사 생성자
Student(char* myname, int myage, char* mymajor)
  : Person(myage, myname)     // 부모 클래스의 멤버까지 초기화해야할 의무가 있다.
{
  strcpy(major, mymajor);
}
```



##### 23. 상속

 : 생성과 소멸은 다음과 같다.

생성은 1)부모생성자 2)자녀생성자, 소멸은 1)자녀생성자 2)부모생성자

cf) 상속을 하는 두가지 케이스

1. is-a : 자녀는 부모다. 라는 말이 성립될때   ex) 사과는 과일이다. 
2. has-a : 자녀는 부모를 가진다. 라는 말이 성립될때    ex) 경찰은 총을 가지고 있다.



cf) 참조자

 : const int &ref = 50; 은 가능. 또한 임시변수 사용가능.

```c++
int Adder(const int &num1, const int &num2) return num1+num2;

int main(){
    cout << Adder(3,4) << endl;
}
```



cf) 어려웠던거 : return 타입이 A&과 *this 같은이유

```c++
class A
{
private:
    int num;
public:
    A& add(int n){
        num+=n;
		return *this;
    }
}
```



##### 24. 가상함수(virtual function)

[먼저 알아야할 것]

1. 객체 포인터 변수

   ```c++
   Person *ptr;
   ```

   : Person형 포인터 ptr은 Person 객체뿐만 아니라, Person을 상속하는 유도 클래스의 객체도 가리킬 수 있다. 그리고 포인터 연산 가능성 여부를 판단할 때, 포인터의 자료형을 기준으로 판단한다. 가령,

   ```c++
   int main(){
       Base* bptr = new Derived();   // compile OK
       Derived* dptr = bptr;         // compile X
   }
   ```

    : 두번째 문장이 컴파일되지 않는 이유는 dptr이 Derived객체와 Derived를 상속하는 자녀 클래스의 객체를 가리키는데 bptr은 Base형 포인터이기 때문에 Base객체를 가리킨다고 했을 때 이는 문제가 된다. 다음의 코드는 실행 가능하다.

   ```c++
   int main(){
       Derived* dptr = new Derived();   // compile OK
       Base* bptr = dptr;               // compile OK
   }
   ```
   
   : 포인터 연산의 가능성 여부를 판단할 때, 포인터의 자료형을 기준으로 판단하지, 실제 가리키는 객체의 자료형을 기준으로 판단하지 않는다.
   
   ```c++
   int main(void)
   {
       Base *bptr = new Derived();     // compile OK
       bptr->DerivedFunc();            // compile Error
   }
   ```
   
   






2. 계층구조상 맨 위에 존재하는 기초 클래스의 가상함수 혹은 소멸자만 virtual로 선언하면 이를 상속하는 자녀 클래스의 가상함수나 소멸자들도 모두 virtual로 선언된다. (이 때, 상속 받는 클래스에서 overriding할 때, virtual을 써줘도 되고 안 써줘도 되나, 써주는 것이 더 낫다.) 소멸자의 경우 그냥 무조건 virtual로 한다고 생각하자. 왜냐하면 문제는 다음과 같은 상황에서 발생되는데.

   ```c++
   Base *b = new Derived();
   delete b;  
   ```

    이렇게되면 Base의 소멸자만 불러져 Derived가 제대로 소멸되지 않아 누수가 발생한다. 따라서 소멸자는 virtual로 선언해주자.

   

   ```
   class First{
   public:
       First(){}
       virtual ~First(){}
   }
   
   class Second: public First{
   public:
       Second():Fist(){}
       virtual ~Second(){}
   }
   
   class Third: public Second{
   public:
       Third():Second(){}
       virtual ~Third(){}
   }
   ```

   이렇게 되면 Third의 소멸자가 불리고, Second의 소멸자가 불리고, First의 소멸자가 차례로 불려 모두 소멸된다.





##### 25. public, protected, private

 : 상속을 받을 때 멤버변수 접근제한의 차이

|           | 외부접근 | 자녀클래스 |
| :-------- | -------- | ---------- |
| public    | o        | o          |
| protected | x        | o          |
| private   | x        | x          |

그리고 상속받을 때 : 뒤에 붙는 거는

```c++
class Derived : public Base{
    
}  // => 그냥 일반적으로 받아들이기

class Derived : protected Base{
    
}  // => 가장 낮은 수준의 접근제한자가 protected임. 즉, Base의 public멤버 변수가 protected가 되고 기존의 protected멤버 변수는 그대로 protected이고, 기존의 private멤버 변수도 그대로 private이다.

class Derived : private Base{
    
}  // => 가장 낮은 수준의 접근제한가가 private임. 즉, Base의 public멤버가 변수가 private이 되고protected멤버 변수도 private이 되고 기존의 private멤버 변수는 그대로 private이 된다.
```



##### 26. -> 와 . 의 차이

 : A->Func()으로 사용했다면 A는 포인터 변수이다. 끝.



##### 27. 정적 바인딩

```c++ 
class Base{
public:
    void Print(){ 
        cout << "Base" endl;
    }
}

class Derived{
public:
    void Print(){
        cout << "Derived" << endl;
    }
}

int main(){
    Base b;
    Derived d;
    d.Print();  // 이렇게 하면 Derived에 있는 Print함수가 호출된다.
    d.b::Print(); // 물론 부모 class의 함수에도 이런식으로 접근이 가능하다.
    
    Base* bptr = new Derived();
    bptr->Print();  // 이 때, bptr이 가리키는 객체는 Derived이지만, Base의 Print를 호출함.
                    // bptr의 타입은 바로 알수가 있다. Base에 있는 Print를 호출하자. 이것이
    				// 정적 바인딩.(바인딩 : bptr이 어떤걸 가리키는 포인터인지에 따라 어떤 Print
    				// 를 호출해야하는가 하는 것) (정적 : 포인터의 타입에 해당하는 Print호출)
}
```



##### 28. typedef 선언(C언어 구조체)

 : 기존에 존재하는 자료형의 이름에 새 이름을 부여하는 것을 목적으로 하는 선언이다.

```c
typedef int INT;  // int의 또 다른 이름 INT를 부여
typedef struct point Point;  // struct point에 Point라는 이름을 부여

// 1)
typedef struct point{
    int a;
    int b;
} Point;

// 2)
struct point{
    int a;
    int b;
} typedef struct point Point;
```



cf) virtual function을 overriding하지 않으면 어떻게 되는가?   => 부모꺼를 사용하게 됨. 문제 없음.



##### 29. 다중 상속(Multiple Inheritance)

 : C++에서는 득보다 실이 더 많은 문법이라고 하는데, 자바에서는 왜 다중 상속을?? 했던거 같은데. 어느 부모의 메서드로 접근해야할지 모호성을 가지기 때문에 되도록 사용 X.



##### 30. 연산자 오버로딩

##### 30-1) operator+, operator-, operator연산자

 : 복잡하게 생각말고, 그냥 직관적으로 받아들여보자.

```c++
class Car{
    Car(){}
    ~Car(){}
    Car operator+(const Car &ref){ 
        return Car타입;
    }
    Car operator-(const Car &ref){
        return Car타입;
    }
}

Car A;
Car B;
```

A+B를 하면 컴파일러가 A.operator+(B)라고 받아들이겠다.

A-B를 하면 컴파일러가 A.operator-(B)라고 받아들이겠다.

cf) operator의 역할이 피연산자의 값을 변경하는게 아니라 새로운 연산의 결과를 만들어내는 거라면 const로 선언하는 것이 좋다.





##### 30-2)  







cf) 계속 나오는 것 같다. 

 : A라는 클래스가 있을 때, A&와 *this의 타입은 같다.



cf) const함수

```c++
class Car{
private:
    int num;
public:
    void print() const {
        ;
    }
}

Car A;
const Car B;
```

이런 상황이라면 Car클래스 타입의 객체 중에 B가 print함수를 부를수 있게 된다. 



cf) friend선언

 : A클래스가 B클래스를 대상으로 friend 선언을 하면, B클래스는 A클래스의 private멤버에 직접 접근이 가능하다. 단, A클래스도 B클래스의 private멤버에 직접 접근이 가능하게 하려면, B클래스가 A클래스를 대상으로 friend 선언을 해줘야한다.





##### Q. 상속받은 관계의 객체들이 있을 때, 최하위 클래스만 delete하는 것? virtual도 아닌데? p.345

##### Q. 리턴 값이 없는 경우에 {}앞에 const를 붙이는 건 어떤 효과가 있는 것?  p.402









## [ 어디서든 코딩하자. 아톰 세팅]

## 1. Install MinGW/MSYS(32bit version recommended)

- Follow this [good installation guide](http://holawang.blogspot.com/2014/02/gcc-installing-gcc-at-windowsmingw-or.html) available. For mac, [good installation guide](https://thdev.net/132)

- After the installation, change (add) the following two folder names into user's PATH environment variable:

  ```
  C:\MinGW\bin
  C:\MinGW\msys\1.0\bin
  ```

- To check your installation, do the following in cmd windows or in PowerShell;

  ```
  g++ --version
  ```

- If you encounter the following error message during the first compilation of your program such as 'HelloWorld.cpp', Refer to [this site](http://stackoverflow.com/questions/39185248/eclipse-mingw-c-cannot-find-lpthread).

  ```
  c:/mingw/bin/../lib/g++/mingw3 ........ mingw32/bin/ld.exe: cannot find -lpthread
  ```



## 2. Install Atom.

1. Atom is a text editor that most professional programmers love nowadays.
2. Start Atom.
3. Install some of essential packages recommended for C/C++ programmers listed below:
   - Autosave
     - It automatically saves files when the editors loses focus, are destroyed, or when the window is closed. Believe or not, it is disabled by default. **You must check enabled** in config setting or from the Autosave section of the Settings view.
   - Platformio-ide-terminal
     - On successful installation, you will see a +sign at the bottom-left corner of the editor. Click + sign to open a console.
   - File-icons
   - Mini-maps
   - Markdown-preview
     - Open a rendered version of the Markdown in the current editor with `ctrl-shift-m`.
   - language-c
     - Adds syntax highlighting and snippets to C/C++ files
   - gpp-compiler
     - Allows you to compile and run C++ and C within the editor.
     - To compile C or C++, press F5 or right click the file in tree view and click Compile and Run.
     - To compile C or C++ and attach the GNU Debugger, press F6 or right click the file in tree view and click Compile and Debug.



## [ 코딩 SKILL ]

##### 1. Testing 할 때 유용한 것

```c++
#if 1      
#endif

#if 0
#endif
```



##### 1-1.

```c++
#ifndef NOWIC_H     // .h파일을 gaurd(중복 include를 방지)해주는 것.
#define NOWIC_H
```



##### 1-1-2.

```c++
#ifdef DEBUG    // 컴파일할 때, 디버그를 체크할 수 있다.
#define DPRINT(func) func;   // ()안에 있는 것이 func가 된다.
#else
#define DPRINT(func) ;      // debug가 없으면 ()안에 있는 것이 null이 된다.
#endif
```



##### 1-3. assert 

 : (1) malloc하고는 assert하기     (2) free해야함 

```c++
char *ptr = new (nothrow) char[5];     // nothrow는 예외처리X 라는말
assert(ptr != nullptr);                // ()이어야 함을 주장. ()가 아니면 Message!
```

 





##### 2. RANDOM(랜덤)의 모든 것

```C++
1. time()
    => #include <ctime>이 필요하며, ???

2. rand()
    =>
    
3. srand()
    =>
    
    
4. 랜덤 시드 넘버
#include <ctime> 
srand((unsigned int)time(NULL));

```



##### 3. DPRINT

 : 때떄로 debugging을 쉽게 하기 위해, 잠시(임시로) 넣어둔 코드 입니다. 

컴파일 할 때, 아래와 같이 -DDEBUG 옵션을 추가하면, 해당하는 코드가 프린트 됩니다. 아직 debugger 가 배우기 전까지 복잡한 source code 어디에서 문제가 생겼는지 쉽게 찾을 수 있죠.  디버깅이 끝난 다음에는 물론 -DDEBUG없이 컴파일하면, 해당 source code는 아무런 영향이 없습니다. 

```powershell
g++  -DDEBUG 
```





##### 4. 입력을 받을 때

##### 4-1. cin 보다 getline을 활용하라!

이유) cin은 스트림에서 newline문자를 제거하거나, 타입검사를 하지 않기 때문에 문제가 발생할 수 있다.(???) 또한, 뛰어쓰기 가 될 때 멈추어 첫 단어만 입력 받게 된다. 그래서 getline을. 

```c++
getline(istream& is, string& str, char delim)
	=> (cin, 받은 string 넣을 변수, tokenize할 문자인데 입력하지 않아도 됨)
```

```c++
#include <iostream>
#include <string>

using namespace std;

int main(){
  string last;
  string first;

  cout << "Enter your last name : ";
  cin >> last;  // cin 으로는 한 '단어'를 받을 수 있다.

  cout << "Enter your first name : ";
  cin.ignore(); // cin을 또 다른 용도로 사용하기 위해 flush해주어야 된다.
  getline(cin, first);  // getline으로 한 '문장'을 받을 수 있다.

  cout << last <<  " " << first << "! Welcome!" << endl;

  return 0;

}
```



**[참고] getline으로 tokenize**

```c++
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

int main() {
  string line = "God is good all the time.";
  vector <string> tokens; // Vector of string to save tokens

  stringstream ss(line);   // stringstream object ss
  string word;
  while(getline(ss, word, ' '))  // Tokenize w.r.t. space ' '
      tokens.push_back(word);

  for(int i = 0; i < tokens.size(); i++) // print the tokens
      cout << tokens[i] << '\n';
}
```

**[참고] string 부분 사용**

```c++
string str="abcdefghijklmnop";
str.substr(startIndex, many);

ex)
    str.substr(2,4) 라면, "cdef"
```



##### [참고] string 입력 받아서  띄어쓰기로  tokenize(split)하고 벡터에 넣기

```c++
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

int main() {

  string line;
  vector <string> tokens; // Vector of string to save tokens
  vector <int> v;

  getline(cin, line);
  stringstream ss(line);   // stringstream object ss
  string word;

  while(getline(ss, word, ' '))  // Tokenize w.r.t. space ' '
      tokens.push_back(word);

  int i=0;
  while(tokens[i] != "q"){
    v.push_back(stoi(tokens.at(i)));
    cout << v[i] << endl;
    i++;
  }
}
```



##### 4-2. 타입 변환 

[char => 정수] atoi   // 그런데 이건 professional 레벨에서는 거의 안쓰고 strtol 을 쓴다!!(string to long)

```c++
#include <iostream>
using namespace std;

int main() {
  char ac[20] = "012345";
  cout << ac[5];   // 5
}
```

[string => 정수] stoi

```c++
string str = "1234567789";
stoi(str);   // 문자열 "123456778" 이 정수 123456778 로
```



##### 5. Function Pointer



##### 6. for문 클라스 높이기

1) 일반

```c++
for(int i=0; i<n; i++){
    cout << v[i] << " ";
}
```



2) 모든 인덱스 접근시 활용

```c++
for(int x : v){
    cout << x << " ";
}
```



3) 타입에 융통성을

```c++
for(auto x : v){
	cout << x << " ";
}
```



4) iterator로 접근하기(이걸 쓰는 이유는 stl에서 iterator만 받는 경우가 있어서 써야된다)

```c++
vector<int>::iterator it; // it은 pointer를 저장할 수 있음
for(it=v.begin(); it!=v.end(); it++){
    cout << *it << " ";
}
```

cf) 자!!  이게 예시다.

```c++
vector<int> v{3, 7, 9, 11, 8};
void foo(int *arr, int n) {
vector<int> v(arr,arr + n);
vector<int> w{0, 1};
	...
	// make w = [ w ] + [ v ] or push back w to w
	w.insert(w.end(), arr, arr + n);  //여기서 iterator w.end()가 필요하기 때문에 어쩔수 없이 iterator를 쓰는 것이다.
}
```



5) 그런데 이마저도 auto가 된다.

```c++
// it은 pointer를 저장할 수 있음
for(auto it =v.begin(); it!=v.end(); it++){
    cout << *it << " ";
}
```



6) first class object도 엄청 쓴다.

```c++
int arr[] = { 7, 4, 3, 2 }; // standard C array
vector<int> v(arr, arr+4); // initialize vector with C array
for_each(v.begin(), v.end(), show_sqr ); // apply function show
```



7) find_if

```c++
#include <vector>
#include <algorithm>
#include <iostream>
bool mytest(int n) { return (n > 2) && (n < 7); };
int main(){
    int arr[] = { 2, 3, 7, 8, 4, 6, 9 }; // standard C array
vector<int> v(arr, arr+7); // initialize vector with C array
auto iter = find_if (v.begin(), v.end(), mytest);
if(iter != v.end){
cout << "found " << *iter <<
else
cout << "not found" <<
}
}

```

이거 말고도 find_end, count_if쭉쭉 다 쓸줄 알아야된다.



# STL

## < vector >

| **멤버 함수**      | **기능**                                                     |
| ------------------ | ------------------------------------------------------------ |
| v.size()           | v의 사이즈(물리적인 저장 용량이 아닌 원소의 개수)를 리턴     |
| v.resize(new_size) | v를 new_size로 사이즈를 바꿔줌                               |
| v.empty()          | v의 사이즈가 0인지 아닌지를 확인                             |
| v.begin()          | v의 0번째 원소를 가리키는 iterator[3](https://sarah950716.tistory.com/4#footnote_4_3) 리턴 |
| v.end()            | v의 마지막 원소를 가리키는 iterator 리턴                     |
| v.front()          | v의 0번째 원소를 리턴                                        |
| v.back()           | v의 마지막 원소를 리턴                                       |
| v.push_back(val)   | v의 끝에 val을 추가                                          |
| v.pop_back()       | v의 마지막 원소를 삭제                                       |
| v.clear()          | v의 모든 원소를 삭제                                         |

cf 1) <algorithm>에 있는 **reverse()**를 이용하면, vector에 속한 원소들의 순서를 거꾸로 뒤집을 수가 있습니다.

만약 원래의 vector가 1 5 2 3 였다면 reverse()한 결과는 3 2 5 1 이 됩니다.

사용법 : reverse(v.begin(), v.end());

cf 2) <algorithm>에 있는 **sort()**를 이용하면, vector에 속한 원소들을 오름차순으로 정렬할 수 있습니다.

만약 원래의 vector가 1 5 3 4 였다면 sort()한 결과는 1 3 4 5 가 됩니다.

사용법 : sort(v.begin(), v.end());

덧) 오름차순 뿐만 아니라 다른 정렬 기준으로도 정렬할 수 있으며, int로 이루어진 vector뿐만 아니라 다른 자료형으로 이루어진 vector도 정렬할 수 있습니다.



## < queue >

| **멤버 함수** | **기능**                                                 |
| ------------- | -------------------------------------------------------- |
| q.size()      | q의 사이즈(물리적인 저장 용량이 아닌 원소의 개수)를 리턴 |
| q.empty()     | q의 사이즈가 0인지 아닌지를 확인                         |
| q.front()     | q에 가장 먼저 들어간 원소를 리턴                         |
| q.back()      | q에 가장 나중에 들어간 원소를 리턴                       |
| q.push(val)   | q의 위(뒤에 val 추가                                     |
| q.pop()       | q에 가장 먼저 들어간 원소를 삭제                         |

swap이라는 함수를 사용한 것을 볼 수 있습니다. 자주 쓰이지는 않지만, vector와는 달리 queue는 clear() 멤버 함수가 없으므로 swap이라는 함수를 이용해 clear() 하고자 하는 queue를 빈 queue와 swap해줌으로써 clear()와 같은 효과를 낼 수 있습니다. 이렇게 swap을 이용하는 것이 queue가 빌 때까지 원소를 pop()하는 것보다 빠르기 때문에 알아두는 것이 좋겠습니다.



## < stack >

| **멤버 함수** | **기능**                                                 |
| ------------- | -------------------------------------------------------- |
| s.size()      | s의 사이즈(물리적인 저장 용량이 아닌 원소의 개수)를 리턴 |
| s.empty()     | s의 사이즈가 0인지 아닌지를 확인                         |
| s.top()       | s에 가장 나중에 들어간 원소를 리턴                       |
| s.push(val)   | s의 뒤에 val 추가                                        |
| s.pop()       | s에 가장 나중에 들어간 원소를 삭제                       |



## < utility >

 : pair란, 이름이 'first', 'second'인 두 개의 변수를 저장할 수 있는 struct

```c++
//pair 선언
pair<int,int> p;
pair<char,double> p;
 
//pair 생성
int a = 1, b = 2;
pair<int,int> p = make_pair(a,b);
pair<int,int> p = make_pair(1,2);
 
//pair의 멤버 변수에 접근
int valA = p.first;
int valB = p.second;
```



출처: https://sarah950716.tistory.com/5 [주니어 개발자의 대나무숲]





## [ 기저까지 ]

##### 1. 용어들

1) 코딩 컨벤션 : 추천하는 표준의 코딩 스타일

2) linter :  코딩 컨벤션과 에러체크를 도와주는 툴

3) 정적분석 : 프로그램 실행 않고 코드를 분석하는 것.

4) 동적분석 : 프로그램 실행해보고 코드를 분석하는 것.













​	

#### [ 찾아볼것 ]

1. 아톰 에러 쉽게 보기.. (compiling error txt라도..),  linter 패키지 다운로드 받았는데... 뭐지...

2. fprint, sprintf

3. setvbuf

4. ??? 해결하기

5. 이해불가 => ss는 무엇인가

   ```c++
   #include <iostream>
   #include <string>
   #include <sstream>

   using namespace std;

   int main() {
     string input = "";
     cout << "Enter a sentence(with spaces):\n>";
     getline(cin, input);
     cout << "You entered: " << input << endl << endl;

     int num = 0;
     while (true) {
       cout << "Please enter a number: ";
       getline(cin, input);
       // This code convert from string to number safely.
       stringstream ss(input);
       if (ss >> num) break;
       cout << "Invalid number, please try again" << endl;
     }
     cout << "You entered: " << num << endl << endl;
   }
   ```















# [ 시험 ]

##### 1. struct // 모두 public임. // class의 경우 private(default), public이 있음.

```c++
struct Car {
	string model;
	double speed;
};

Car* Cset = new Car[3];

Cset[i].model = Abante;   // Cset[i]->model 아님
```

```c++
for (int i = 0; i < NCARS; i++, Cset++) {
		cout << "\t(" << i + 1 << ")" << Cset->model << "\t" << Cset->speed << endl;
	}  // 이럴때 -> 쓰는거임.
```



##### 2. inorder, preorder, postorder, levelorder

(1) inorder

 : LVR출력 순서이다. L부터이니 맨 왼쪽으로 이동. 맨 왼쪽 노드 도착하면 LVR순서로 출력. R은 가지가 더 있다면 그 안에서 LVR순서로 출력.

(2) preorder

 : VLR출력 순서이다. V부터이니 루트 노드에서 출력하고 시작한다. 다음은 L이니 마지막 L도착까지 출력하면서 이동. 맨 왼쪽 노드 도착하면 VL출력한 상태이니 R출력하는 방식인데, 마치 미로에서 한쪽 벽을 잡고 쭉 가는 식이다.

(3) postorder

 : LRV출력 순서이다. L부터이니 맨 왼쪽으로 이동. 맨 왼쪽 노드 도착하면 LRV순서로 출력.

(4) levelorder

 : level 단위 출력이다. 1level출력 -> 2level출력 -> 3level출력 -> ...







