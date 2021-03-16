# MySQL (intellij)

1. MySQL 을 각 운영체제에 맞게 다운로드 받는다.



2. timezone을 설정한다.

   ```mysql
   use mysql;
   ```

   

3. https://dev.mysql.com/downloads/timezones.html 에 들어가서 전부 실행한다.

   ```
   timezone_posix.sql 참고
   ```

   

4. 다음을 실행한다.

   ```mysql
   SET GLOBAL time_zone='Asia/Seoul';
   SET time_zone='Asia/Seoul';
   ```

   

5. configure.properties 하나 만들어서

   ```java
   spring.datasource.url=jdbc:mysql://localhost:3306/baechoo?autoReconnect=true&useUnicode=true&characterEncoding=utf8&allowPublicKeyRetrieval=true&useSSL=false&serverTimezone=Asia/Seoul
   spring.datasource.username=root
   spring.datasource.password=root
   ```

   

