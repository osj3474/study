# SSL 적용

https://certbot.eff.org/instructions?ws=other&os=ubuntufocal 에서

본인에게 맞는 SofWare와 os를 선택합니다.

- software : spring boot가 없어서 other로 선택
- os : Ubuntu 20

![image](https://user-images.githubusercontent.com/42775225/150618704-01990d23-aded-4f71-ae17-b66299f88ab4.png)



1. WAS 서버에 접속한다.

2. snap을 설치 및 최신 버전 확인

   (https://snapcraft.io/docs/snapcraft-overview)

   ```shell
   sudo snap install snapcraft --classic
   sudo snap install core; sudo snap refresh core
   ```

3. 기존 certbot 설치되어 있다면 삭제

   ```shell
   sudo apt-get remove certbot
   ```

4. certbot 설치

   ```
   sudo snap install --classic certbot
   ```

5. 도메인 - IP 맵핑을 확인한다.

   ```
   nslookup dev-brave-people.o-r.kr
   ```

6. 상황에 맞게 certbot 실행

   일단, 80, 443 도는지 확인

   ```shell
   sudo netstat -lntp | grep 80
   sudo netstat -lntp | grep 443
   ```

   - 웹 서버 중지할 수 있다면 중지를 하고 아래 명령어

     ```shell
     sudo certbot certonly --standalone -d dev-brave-people.o-r.kr
     ```

   - 웹 서버 중지 못할 경우 아래 명령어

     ```shell
     sudo certbot certonly --webroot
     ```

7. 인증서 확인

   ```shell
   sudo certbot certificates sudo letsencrypt certonly --standalone -d dev-brave-people.o-r.kr
   ```

8. spring boot는 JKS 또는 PCKS12 인증서가 필요. (openssl 명령어로 PCKS12 키를 생성)

   ```shell
   openssl pkcs12 -export -in cert.pem -inkey privkey.pem -out keystore.p12 -name ttp -CAfile chain.pem -caname root
   ```

9. application.yml 에 정보 추가

   ```yaml
   server:
     port: 443
     ssl:
       key-store: ${pemkey.path} #p12key 파일 위치
       key-store-type: PKCS12
       key-store-password: ${ssl.password} #키 생성시 입력한 비밀번호
   ```

   

