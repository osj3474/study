# 엔티티 매핑

JPA에서는 객체와 테이블의 매핑이 정말 중요하다.



### 1) 객체-테이블

1. @Entity

   : JPA가 관리하는 객체에 붙인다.

   : JPA로 테이블 매핑하려면 필수 어노테이션이다.

   : 기본 생성자는 필수이다. (reflection 등을 위해)

   : final, enum, interface, inner 클래스에는 사용 불가다.

   <옵션>

   - name : 엔티티 명이다. 기본 값은 클래스 명이다.

2. @Table

   : 해당 엔티티와 매핑할 DB의 테이블 명을 설정할 수 있다.



### 2) 스키마 자동 생성

: 운영 단계에서는 사용해서는 안되며, 개발 단계에서만 사용한다.

: 어플리케이션 실행 시점에 DDL을 만들어 실행하는 역할을 해준다.

: 데이터 베이스 방언으로 사용 DB에 맞는 DDL을 생성해준다.

- 설정은 pom.xml에서 hibenate.dialect.[사용DB] 로
- hibernate.hbm2ddl.auto = 
  - create : 기존 테이블 삭제 후 다시 생성
  - create-drop : create과 같으나 종료 시점에 테이블 drop
  - update : 변경 분만 반영된다.
  - validate : 엔티티와 테이블이 정상 매핑 되었는지만 확인한다.
  - none : 사용하지 않는다.





### 3) 필드-컬럼명 매핑

1. @Column

   \<DDL 에만 먹히는 옵션들>

   - name : DB에 매핑할 컬럼명
   - unique : 해당 컬럼의 unique 유무
     - unique 이름을 설정 못해서 거의 쓰지 않으며, 쓰더라도 @Table(uniqueConstraints = ) 를 사용한다.
   - length : 문자 길이 (String 타입에만 사용 가능하다)
   - nullable : null 허용 여부  / 기본 값이 TRUE
   - columnDefinition : 컬럼 정의를 직접한다.
     - ("varchar(100) default 'EMTPY' ")
   - precision, scale : JAVA의 BigDecimal, BigInteger 타입일 때 사용한다. precision은 소수점 포함 전체 자리수이고, scakle은 소수점 자리수이다.

   \<어플리케이션 단에도 먹히는 옵션들>

   - insertable : 등록 가능 여부 / 기본 값이 TRUE
   - updatetable : 변경 가능 여부  / 기본 값이 TRUE





2. @Enumerated(EnumType.STRING)

   : JAVA의 enum 타입을 필드로 사용할 때 쓰는 어노테이션이다.

   : EnumType은

   - ORIGINAL : enum순서를 DB에 저장한다. 이게 기본 값이지만 사용하지 않는다.
   - STRING : enum이름을 DB에 저장한다.



3. Temporal(TemporalType.TIMESTAMP)

   : 날짜 타입을 필드로 사용할 때 쓰는 어노테이션이다.

   : TemporalType은 

   - DATE : 날짜
   - TIME : 시간
   - TIMESTAMP : 날짜/시간

   이 있는데, 이는 DB가 날짜 타입을 이렇게 구분하기 때문이다.

   : JAVA8부터는 LocalDate(날짜), LocalDateTime(날짜/시간) 을 쓰면 어노테이션 안 붙여도 된다.



4. @Lob

   : 큰 varchar에 사용한다.

   : 실제 DB의 타입은 둘 중에 하나인데,

   - blob : JAVA 필드의 타입이 byte인 경우
   - clob : JAVA 필드의 타입이 String인 경우



5. @Transient

   : DB에 매핑 안되게 하고 싶은 필드에 쓰는 어노테이션이다.































### 