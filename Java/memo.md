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







# 8080 포트 내리기

![image](https://user-images.githubusercontent.com/42775225/107880579-1615d480-6f23-11eb-8015-5e3611be45cb.png)



* 윈도우 기준

  관리자 권한으로 cmd 창을 열고 8080포트를 사용하는 프로세스를 찾습니다.

  ```shell
  netstat -ano | find "8080"
  ```

  ![image](https://user-images.githubusercontent.com/42775225/107880607-465d7300-6f23-11eb-96b2-2d1c50818991.png)

  그리고 해당 프로세스를 내립니다.

  ```shell
  taskkill /f /pid 4506
  ```

  ![image](https://user-images.githubusercontent.com/42775225/107880628-65f49b80-6f23-11eb-9bae-420dcec6c206.png)





# Gradle Refresh

view => Tool windwo => Gradle => 프로젝트 우클릭 => Refresh



# 로그 보기

logging.level.org.springframework.security=DEBUG





# HttpServletRequest

getParameter()





# Model





# SpringSecurity

https://daddyprogrammer.org/post/636/springboot2-springsecurity-authentication-authorization/

=> DispatcherServlet이 원래 Controller랑 view랑 해서 처리해주는데, 그 앞단에 filter를 두고 클라이언트의 request를 가로챔. 

=> jwt : JSON객체인데, 여기 안에 A사용자인지, B사용자인지에 대한 정보랑 resource에 대한 권한에 대한 정보가 들어가져있음. 이걸 encryption해서 string 값으로 된 것.

- claim으로 부가적인 정보들을 덧붙일 수 있음.





# message properties



1. build.gradle

   ```
   
   ```

2. application.properties

   ```
   spring.messages/messages*.properties
   spring.message.encoding=UTF-8
   ```

3. resource/messages/message.properties

   ```
   NOT_EXIST_DEVGROUP=존재하지 않는 개발자 모임입니다.
   FAIL_CREATE_DEVGROUP=해당 조건으로 개발자 모임을 생성할 수 없습니다.
   ```

4. MessageSource 주입받아서 getMessage(key, argu, Locale)로 쓰면 된다.

5. 





네, 확인하였고, DevGroupController, AdminDevGroupController 모두 final 추가하였습니다.



네, 이 또한, DevGroupController, AdminDevGroupController에 모두 적용하였습니다.



@Data > @Getter로 수정하였습니다.



ControllerAdvice는 사용하되, Error객체를 반환하는 대신, 에러 메세지를 message properties에 정의하여 다음과 같이 String을 반환하도록 하였습니다.



![image-20210726142334928](/Users/sangjin/Library/Application Support/typora-user-images/image-20210726142334928.png)







![image-20210726142357106](/Users/sangjin/Library/Application Support/typora-user-images/image-20210726142357106.png)







주석도 달아보았습니다.





















