# memo



1. Jackson : **Java JSON library** or **best JSON parser for Java**

   쉽게 말하면, 타입을 json으로 marshal을 해주는 역할, web starter에 포함되어 있음.

2. Spring application은 https://start.spring.io/ 에서 시작해야함.

   ![image](https://user-images.githubusercontent.com/42775225/104589292-f6358b80-56ac-11eb-9d33-b4631e9da877.png)

3. 빌드하고 실행하기

   for 맥

   ```shell
   ./gradlew build
   cd build/libs
   java -jar hello-spring-0.0.1-SNAPSHOT.jar
   ```

   for 윈도우

   ```shell
   콘솔로 이동 명령 프롬프트(cmd)로 이동
   ./gradlew gradlew.bat 를 실행하면 됩니다.
   명령 프롬프트에서 gradlew.bat 를 실행하려면 gradlew 하고 엔터를 치면 됩니다.
   gradlew build
   폴더 목록 확인 ls dir
   윈도우에서 Git bash 터미널 사용하기
   ```

   

4. spring-boot-devtools 라이브러리를 추가하면, html 파일을 컴파일만 해주면 서버 재시작 없이 View 파일 변경이 가능하다.

