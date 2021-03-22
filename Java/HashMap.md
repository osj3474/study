# HashMap 동작 방식

### Q. HashMap 동작 방식에 대해서 설명하세요.



단순히 자바의 HashMap을 넘어서 `해시 테이블`, `해시 맵` 에 대한 기본적인 이해를 해봅시다.



=> 객체를 Map에 넣는 것이 전부이다. 이 때,

`key-value쌍` 하나만 넣는 것이 가장 기본적이며, 배열의 한 요소를 `bucket` 이라고 한다.

(자바에서는, 키와 값의 타입은  `클래스  ` 및 `인터페이스` 타입만 가능하다. `기본 타입`  은 사용할 수 없음.

또한, HashMap에서의 key는 unique해야합니다. (key는 중복 불가, value는 중복 가능))



![image](https://user-images.githubusercontent.com/42775225/112005807-6d344800-8b66-11eb-92e8-ad8f03e237bd.png)





`key-value쌍` 이 들어가는 위치는, `key` 의 `Hash값 (HashCode)` 이며, 이로 인해 데이터를 탐색하는데 `O(1)` 로 가능하다.

이 때, 동일하지 않은 두 객체가 같은 위치에 들어가려고 하는 경우를 `Collision` 이라고 하는데,  `Collision` 은 Map의 성능에 큰 영향을 미치므로,

어떤 `Hash 함수` 를 사용하는 가에 따라서 더 나은 Map이 될 수 있다. 





<br>

이 때, Map에 더 들어갈 공간이 없을 때, 어떻게 할까? 

1. 리스트로 넣는다.
2. size를 늘린다.



오늘은 size를 늘리는 방식에 대해서 조금 더 살펴보자.



`Load factor` 는 Map의 `capacity` 를 몇 %로 할지를 정하는 값이다.

가령, Map의 사이즈가 4이고, load factor가 0.75라고 해보자.

(자바의 경우 다음과 같이 생성할 수 있다.)

```java
Map<String, String> map = new HashMap<>(4, 0.75f);
```

cf) 반복문 돌릴 때, Iterator 바로 못 쓰고, `keySet()` 하고 써야함.



그러면, Map의 공간은 다음과 같이 동작한다.



![image](https://user-images.githubusercontent.com/42775225/111992408-be3d3f80-8b58-11eb-8eb5-715376235417.png)













# (번외) 

### 1. HashMap의 `HashCode()`, `equals()` 

1. hashCode()

   : 객체 고유의 해시코드를 반환한다.

   : 두 객체가 같은 객체인지 확인할 때 사용한다.

   

2. equals()

   : `==` 와 같은 결과를 반환한다.

   : 두 객체의 내용이 같은지 확인할 때 사용한다.

   ![image](https://user-images.githubusercontent.com/42775225/112002803-a0290c80-8b63-11eb-8a74-58c2786a47b5.png)





cf) instanceof

```java
a instanceof b
```

=> a는 b로 형 변환이 가능한지

=> return : T/F









<br>

### 2. HashTable vs. HashMap

=> `key-value 쌍` 으로 데이터를 저장한다는 면에서는 동일하다.

차이점은,



1. HashTable : 멀티 스레드 환경에서 안전하게 객체를 추가, 삭제할 수 있다. (thread safe)

2. HashMap :  빠른 대신에 동기화 문제가 있다.

   => 동기화를 위한 `ConcurrentHashMap` 도 있고, 다음처럼 HashMap을 사용해서 동기화를 얻어낼 수 있다.

   ```
   Map m = Collections.synchronizedMap(new HashMap(...));
   ```

   

   