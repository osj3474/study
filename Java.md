# Java Spring



# ✅ 설치

- ### Java 11

  : 자신도 모르게 다운로드 받았을 지도 모르니, 버전을 확인하고,

  ```shell
  java --version
  ```

  다운로드 된 것이 없다면, 

  https://www.oracle.com/kr/java/technologies/javase-downloads.html 에서 다운로드 받는다.

  ![image](https://user-images.githubusercontent.com/42775225/96862727-d113f100-14a0-11eb-8081-913e816890ad.png)

  

- ### Intellij

  : 에디터는 이제 이클립스보다 인텔리제이가 더 좋다고 한다.

  https://www.jetbrains.com/ko-kr/idea/download/#section=mac 에서 다운로드 받는다.

  ![image](https://user-images.githubusercontent.com/42775225/96862923-159f8c80-14a1-11eb-9a8e-ff3d0985aa55.png)

  참고로, 학생 계정은 https://www.jetbrains.com/ 에서 학교메일 인증을 받으면 된다.





# ✅ 시작



### 0. 용어

- JVM : Java바이트 코드가 실행될 수 있는 환경을 제공하는 Java Virtual Machine이다.
- JRE : 자바를 실행하는데 필요한 라이브러리를 가지고 있는, Java Runtime Environment이다.
- JDK : java 파일을 컴파일 하는 javac와 같은 컴파일, 디버깅, 실행하는데 필요한 모든 도구인, Java Development Kit이다.



### 1. 스프링부트로 프로젝트 생성

 : https://start.spring.io/ 에서 설정을 하고 다운로드 받는다.

- Gradle Project : 필요한 라이브러리를 가져오고 빌드를 관리하는 방식으로, 현재 대부분은 Maven이 아닌, Gradle로 한다.

- Group : 회사 도메인 명, 연습할 때는 그냥 아무거나 해도 된다.

- Artifact : 프로젝트 명

- Spring Web : web과 관련된 라이브러리를 가져온다.

- Thymeleaf : 템플릿 엔진이다. pug같은 것이다.

  ![image](https://user-images.githubusercontent.com/42775225/96864117-e7bb4780-14a2-11eb-9fd8-3bfa4eaa01ea.png)

  이렇게 다운로드 받은 파일을 Intellij로 엽니다.



### 2. 구조 파악

[ 구조 ]

- gradle : gradle 관련 파일
- src
  - main : 
    - java : 
    - resources : java파일을 제외한 모든 것 ex) html 등
      - static : 정적 파일들 
      - templates :
  - test : 테스트 코드와 관련된 파일
    - 



### 3. 추가 설정

: Gradle을 거쳐서 실행하면 더 느리다고 한다.

![image](https://user-images.githubusercontent.com/42775225/96876120-01fd2180-14b3-11eb-9d53-390908a3bde9.png)



### 4. Run

: 그리고 http://localhost:8080/ 에서

![image](https://user-images.githubusercontent.com/42775225/96878029-543f4200-14b5-11eb-8f93-1e2434943aba.png)

이 화면 나오면 된 것.



❌ 혹시 run이 비활성화 되어 있다면! 

: 일단, jdk를 제대로 다운 받지 않았을 가능성이 크다. 하지만, 그럼에도 해결되지 않는다면, 직접 default SDK를 잡아주자.

[해결법]

![image](https://user-images.githubusercontent.com/42775225/96876339-48eb1700-14b3-11eb-8dad-8a3dfeee6fc6.png)



![image](https://user-images.githubusercontent.com/42775225/96876384-599b8d00-14b3-11eb-9255-bc6063bd3ab1.png)





❌ 혹시 OpenJDK 64-Bit Server VM warning 이 뜬다면!

![image](https://user-images.githubusercontent.com/42775225/96878105-6a4d0280-14b5-11eb-8eae-9d7a56561e7a.png)

[해결법]

![image](https://user-images.githubusercontent.com/42775225/96877963-41c50880-14b5-11eb-8fbf-4d2cc09d691d.png)

























