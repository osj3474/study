### < 인덱스 >

: 데이터 베이스의 **조회 성능을 향상**시키기 위해 데이터 베이스의 저장 공간 일부를 할애하여 특정 컬럼을 기준으로 데이터의 위치를 저장하는 것입니다.

create(인덱스 테이블에도 insert필요), update(기존 row 사용하지 않음 처리, 새로운 row insert), delete(기존 row 사용하지 않음 처리) 연산의 경우 오버헤드가 있기 때문에

인덱스는 조회가 빈번한 경우에 장점을 극대화할 수 있습니다.

또한 데이터의 중복도가 낮을수록 인덱스로 필터했을 때 남은 데이터의 양이 적기 때문에 유리합니다. (카디널리티가 높을 수록 유리)

- 인덱스의 **자료구조**는 1)HashTable 2)B+Tree 이 사용될 수 있지만, 보통 B+Tree를 사용합니다.

  - HashTable

    : `HashTable`은 기본적으로 정렬되지 않은 배열처럼 동작하며,

    `key-value 로 데이터를 저장`하는 자료구조입니다.

    (물론, Java의 LinkedHashMap은 sorting되어 있습니다.)

    이 때, `Hash Function`과 `Hash Scheme`에 대한 이해가 필요합니다. `Hash Function`은 key에 대한 해시 값(정수)을 반환하는 함수로, 단순히 mod함수부터 xxHash64까지 여러 종류의 함수가 사용이 됩니다.

    참고로 실제 속도가 매우 느린 SHA-256(어떤 입력에 대해서든지 256비트 고정 값을 반환, 단방향으로 복호화 불가능)는 사용하지 않습니다.

    `Hash Scheme`이란 collision(2개의 키가 같은 테이블의 index를 가리키는 경우)을 대처하는 방식을 의미하며,

    종류로는 Linear probing(테이블의 빈 공간을 선형 탐색하여 insert/delete/find 함),

    Robin hood hashing (movement가 일어난 횟수를 함께 저장하여 movement를 균등하게 하여 linear probing의 성능을 향상시킨 방식),

    Cuckoo hashing이 있습니다.

    그런데, 데이터 베이스의 검색은 `=` 연산만이 아니라, `>`, `<` 연산도 사용되며, `LIKE` 와 같은 값의 일부를 가지고 검색하기도 하는데 이 때는 `=` 으로 밖에 사용하지 못하는 HashTable은 적합하지 않기 때문입니다.

  - B+Tree

    : 모든 노드에 데이터를 저장했던 B-Tree 를 개선시킨 자료구조입니다. 리프 노드만 실제 데이터를 가지고 있고 링크드리스트로 연결되어 있으며, 나머지 노드들은 데이터를 위한 인덱스만 가지고 있습니다.

- 인덱스는 `Clustered index` 와 `Non-Clustered index` 로 나뉩니다.

  - Clustered index

    : 노드가 인덱스와 데이터를 함께 가지고 있는 구조의 인덱스로, 범위 검색에는 강력하지만, 삽입/삭제에는 취약합니다.

  - Non-Clustered index

    : 노드가 인덱스와 데이터가 있는 주소를 가지고 있는 구조의 인덱스로, 범위 검색에는 취약하지만, 삽입/삭제에는 효율적입니다.

  cf) 애초에 DB는 데이터를 어떻게 찾아서 가져올까?

  => full table scan



### < 정규화 >

: 중복된 데이터를 줄이고, 데이터 베이스 변경 시 갱신을 누락하거나 **데이터가 꼬이는 일을 막기 위한 것에 목적**이 있습니다. 다만, 조인이 많이 일어나는 관계의 경우 반(역)정규화를 진행하기도 한다.

- 1NF

  - 다중 값을 가지면 안된다. (atomic)

  - 반복된 컬럼이 있으면 안된다. (column unique)

    // ex. name1, name2, ..

- 2NF

  - 모든 컬럼은 기본키의 일부에만 소속되면 안된다. (not partial dependency)

    ex. 주문 테이블의 **기본키(PK, 상품ID), 상품명**이 있는 경우, 상품명은 상품ID에만 종속적이다. 상품명 변경 시 상품 테이블만 수정하는 것이 아니라, 주문 테이블도 수정해야함.

  - 다른 말로, primary key 중 일부가 non-dependency key와의 dependency를 가지면 안된다.

- 3NF

  - 기본키가 아닌 모든 속성 간에는 서로 종속될 수 없다. (not transitive dependency)

    ex. 회원 테이블에 부서ID, 부서명이 있을 때, 부서ID가 기본키는 아니지만 부서명이 부서ID에 종속적이기 때문에 이런 경우에도 데이터 이상 현상 발생 가능.

  - 다른 말로, non-PK로 또 다른 non-PK의 정보를 알 수 있으면 안된다.

- BCNF

  - A —> B 로의 dependency가 있다면, A는 super key 중에 하나여야 한다.
  - non PK가 PK를 식별하면 안된다.







### < 격리수준 >

![image](https://user-images.githubusercontent.com/42775225/136661320-d0b739f0-c9aa-48dd-85c3-b8c125761092.png)

- phantom read : 순차적으로 같은 것을 select 하는데, 다른 값이 읽히는 경우 (단, 중간에 insert가 있어서)
- nonrepeatable read : 순차적으로 같은 것을 select하는데, 다른 값이 읽히는 경우 (단, 중간에 update, deleterk 있어서)
- dirty read : 아직 commit 전인데 update하고 있는 데이터 들고가서 작업해서 데이터 정합성깨지는 것.

serializable말고는 read는 다 가능. 다만 감당할 수 있는 에러 수준에 따라 정책 설정하면 됨.

1. serializable : 지금 작업하는 테이블에 대해서 다른 트랜잭션 read도 개입 불가
2. repeatable read : 가장 마지막에 commit된 상태를 읽되, 내가 한번 읽은 row는 다른 곳에서의 commit유무와 관계없이 처음 읽었던 그 값으로 읽는다.
3. read commmited : 가장 마지막에 commit된 상태의 값을 가지고 온다. 다른 곳에서 commit했으면 commit된 값으로 읽는다.
4. read uncommitted : commit전에 수정 상태라도 그 상태의 값을 가지고 온다. (commit돼서 인증된 값 아니라도) 사실상 트랜잭션 개입 모두 허용.



cf) 조회수 카운팅 로직에서의 트랜잭션 격리수준

조회수 기능이 가지는 지위에 따라 다르게 결정하자.

- 실시간 검색어 등 조회수 기능이 가지는 영향이 큰 경우 ⇒ serializable, repeatable read, read committed
  - serializable : 데이터 정합성은 걱정 없지만, 응답속도가 늦은 tradoff
  - repeatable read : rollback 의 경우가 잦은 경우 비교적 데이터 정합성이 높다는 장점
  - read committed : rollback이 없다는 가정하에 상대적으로 데이터 정합성도 지키고, 응답시간도 지킬 수 있다.
- 약간의 오차가 허용 가능한 경우 ⇒ 더 낮은 레벨(repeatable read, read committed 등)
  - read committed : commit된 상태의 값을 읽다보니 동시 조회 시 조회수 상승 누락이 발생 가능함
  - read uncommitted : commit되지 않더라도 최신 수정된 값으로 읽기 때문에 rollback 가능성 낮다면, 최선의 선택. (사실 조회수 관련해서 rollback 이 일어날 가능성이 없기 때문에 이게 낫다.)