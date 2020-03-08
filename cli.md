# CLI 

 : command line interface

 : UNIX, LINUX, MacOS에서 사용되는데, Window, Android, IOS에서 사용하려고 할 때는 emulator가 필요하다. 지금은 대중적으로 git bash가 이 역할을 감당하고 있다. 

 : File과 Directory(폴더)를 대할 때 사용하는 명령어를 알아야한다.

### 기본

```bash
pwd : 현재 디렉토리 확인(print working directory)
cd / : 최상위 디렉토리로 이동
cd ~ : 홈 디렉토리(커맨드 창을 켰을 때의 디렉토리)로 이동
cd .. : 상위 디렉토리로 (한번)이동
ls : 파일 리스트 확인
ls --help 혹은 man ls : ls의 사용 매뉴얼을 확인
ls -l : (상세한)파일 리스트 확인 // 맨 앞에 d는 디렉토리를 의미하고, -는 File임을 의미한다.
touch a.txt : 내용이 없는 a라는 텍스트 파일 생성
touch .a.txt : 내용이 없는 a라는 (숨긴) 텍스트 파일 생성
ls -a : (숨김파일까지)파일 리스트 확인
ls -l -a 혹은 ls -la : (숨김파일까지)(상세한)파일 리스트 확인
ls -R : (디렉토리 밑에 있는 파일까지)파일 리스트 확인
mkdir a : a라는 디렉토리 생성
cd a 혹은 cd ./a : 현재 디렉토리에서 a라는 디렉토리로 이동
cd /a : 최상위 디렉토리에서 a라는 디렉토리로 이동
mv a b : a라는 파일을 현재 디렉토리로 b라는 이름으로 이동(이름 바꾸기)
rm a : a라는 파일을 지운다
rm -r a : a라는 디렉토리를 지운다.
cat a.txt : a라는 텍스트 파일의 내용을 출력한다.
; : 한 줄에서 명령 구분자 역할
&& : 한 줄에서 명령 구분자 역할, 단, 앞선 명령어를 실패하면 뒤에 명령어는 받아들이지 않음. 
```



### 서버 관련

```bash
lsof -i :8080  // 8080포트가 사용되고 있는지 확인한다.
lsof -i :80
lsof -i
lsof

sudo chown 사용자 디렉토리  // 
sudo chgrp 사용자 디렉토리

ps -ef | grep 명령어  //현재 실행되고 있는 프로세스 중에 '명령어'의 이름이 들어가 있는 것만 확인한다. 
```



### 권한 관련

```bash
ls -l   // 디렉토리의 권한을 리스트업한다.
```

##### 1. 맨 처음에 'drwxr-xr-x' 이런 식의 문자는 총 10개로, 구분은 다음과 같다.

* 1 : 파일(-), 디렉토리(d)
* 2, 3, 4  : 소유자 권한
* 5, 6, 7  : 그룹 권한
* 8, 9, 10 : 다른 사람들 권한

r은 읽기(read), w는 쓰기(write), x는 실행(execution)의 권한을 나타낸다. 또한 r, w, x는 각각 4, 2, 1의 값을 가진다.

cf) 디렉토리에 들어가려면 실행 권한이 필요하다.

##### 2. 그 다음으로 이어지는 두 문자는 각각 '소유자', '그룹' 이다.



##### 3. 권한 부여하기

1) chmod

 : u (사용자), g (그룹), o (다른 사용자), a (전부), + (권한추가), - (권한제거) 

```bash
chmod u+x a.out    // 소유자에게 a.out파일의 실행권한을 부여한다.
chmod g+r a.txt    // 그룹에 a.txt파일의 읽기권한을 부여한다.
chmod o-w a.txt    // 다른 사용자에게 a.txt파일의 수정권한을 제거한다.
chmod 740 a.txt    // 소유자는 rwx, 그룹은 r, 다른 사용자는 아무권한이 없다.
```



2) chown

```bash
sudo chown ubuntu .   // 현재 디렉토리의 사용자를 ubuntu(사용자)로 바꾼다.
```



3) chgrp

```bash
sudo chgrp staff .    // 현재 디렉토리의 그룹을 staff(그룹)로 바꾼다. 
```

 





##### 추가적으로 참고할 만한 사이트

1. 리눅스 명령어 모음집

   https://www.howtogeek.com/412055/37-important-linux-commands-you-should-know/

2. 권한 관련

   https://withcoding.com/103







# AWS 

1. 파일 전송

```bash
scp -i ~/Documents/dev/key/main.pem sch.png ubuntu@ec2-15-165-142-48.ap-northeast-2.compute.amazonaws.com:~/
```



# GCP

1. 접속

   ```bash
   ssh -i ~/.ssh/osj-gcp-key 21500404@34.82.251.243
   ```

   (물론, rsa key를 GCP에 등록해야 사용할 수 있을 것이다.)

   

2. 파일 전송

   ```bash
   scp -i [인증키] [업로드할 파일] [계정]@[원격ip]:[업로드 디렉토리 경로]
   
   ex)
   scp -i ~/.ssh/osj-gcp-key screenshot.png 21500404@34.82.251.243:~/21500404
   ```

3. 파일 다운로드

   ```bash
   scp -i ~/.ssh/my-ssh-key username@ip-address:remote-file-path local-file-path
   
   ex)
   scp -i ~/.ssh/osj-gcp-key 21500404@34.82.251.243:~/hw1/21500404.tgz ~/Desktop
   ```

   











