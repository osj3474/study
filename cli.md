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
ps aux | grep 프로세스 // 현재 실행되고 있는 프로세스 확인
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



# VIM 기본

```shell
a          # 다음 칸 이동 후 입력모드
o          # 다음 줄 이동 후 입력모드
I          # 맨 앞 칸 이동 후 입력모드
A          # 맨 마지막 칸 이동 후 입력모드
cw         # 커서에 있는 단어 지우고 입력모드
$          # 마지막 칸 이동
^          # 맨 앞 칸 이동
V          # 선택모드
y          # 블록 복사
yy         # 한 줄 복사
숫자+yy     # 숫자 만큼의 줄 복사
p          # 붙여넣기
d          # 블록 샂게
dd         # 한 줄 삭제
숫자+dd     # 숫자 만큼의 줄 삭제
D          # 커서 뒷 내용 삭제
x          # 문자 1개 삭제
x+숫자      # 숫자 만큼의 문자 삭제
F          # 다음 페이지
B          # 이전 페이지
J          # 밑에 줄 위로 옮기기
=          # 블록 정렬
=%         # 커서 { } 정렬
*          # 커서 문자와 같은 문자 찾기
n          # 밑으로 찾기
N          # 위로 찾기
u          # 되돌리기
R          # 되돌리기 취소
:숫자       # 숫자line으로 이동
:$         # 마지막line으로 이동 
:%s/찾을단어/바꿀단어   # 문자 한꺼번에 바꾸기
:r 파일명    # 파일 복붙
:!리눅스명령어 # 쉘 사용 가능
h          # 왼쪽 이동
j          # 아래 이동
k          # 위로 이동
l          # 오른쪽 이동
```







# AWS 

1. 파일 전송

```bash
scp -i ~/Documents/dev/key/main.pem sch.png ubuntu@ec2-00-00-000-000.ap-northeast-2.compute.amazonaws.com:~/
```



2. 외부 사용자 연결해주기

   ```bash
   # 새로운 유저를 생성합니다.
   - $ sudo useradd -s /bin/bash -m -d /home/USERNAME -g root USERNAME
   
   # 유저 비밀번호를 설정합니다.
   - $ sudo passwd USERNAME
   - 패스워드 입력
   
   # sudoers 파일 권한 변경
   - $ sudo chmod u+w /etc/sudoers
   
   # sudoers 파일 열고, username 추가
   - $ sudo vi /etc/sudoers 
   - USERNAME ALL=(ALL:ALL) ALL
   
   # sshd_config 파일에 PasswordAuthentication 설정 추가(혹은 변경)
   - $ sudo vi /etc/ssh/sshd_config
   - PasswordAuthentication yes
   
   # ssh를 재시작합니다.
   - $ sudo service ssh restart
   
   # ssh [username]@host
   - $ ssh user@52.23.68.21 와 같은 형식으로 입력합니다.
   
   # 위에서 설정한 패스워드를 입력합니다.
   
   # 완료!
   ```

   

3. Scale up

   : 급하게 scale up해야한다면, image만들고, elastip ip만 기존꺼 옮겨서 쓰면된다.

   : 그런데 이렇게만 하면, 처음 만들 때의 IP와 elastic ip가 바뀌게 되고, 결국 접속을 시도했을 때, WARNING: REMOTE HOST IDENTIFICATION HAS CHANGED! 라는 경고가 뜨게; 된다.

   네트워크를 조금 더 공부하고 지금 해결한 방법이 왜 맞는지 이해할 수 있도록 하자.

   ```shell
   ssh-keygen -R 나의IP
   ```

   



# GCP

1. VM 인스턴스를 필요한 스펙에 맞게 만듭니다. 외부 IP 주소를 킵 해둡니다.

2. 자신의 로컬에 키를 만듭니다. (추가로 입력하라고 하는 비밀번호는 추후 서버 접속 비밀번호가 됩니다.)

   ```shell
   ssh-keygen -t rsa -f ~/.ssh/[KEY_FILE_NAME] -C "GCP이메일" 
   ```

3. 키 정보를 확인하고, 복사 해둡니다.

   ```shell
   cat ~/.ssh/[KEY_FILE_NAME].pub
   ```

4. GCP console에서 VM 인스턴스의 메타 데이터 -> ssh키 로 이동합니다. 만들기 혹은 수정 탭을 눌러서 해당 키를 붙여넣습니다.

5. 여기까지 하면, 일단 iterm(외부)에서 ssh 접속은 됩니다.

6. 작업을 vim에서 하지 않고 싶다면, vscode를 킵니다.

7. cmd + shift + p에서 'ssh'만 칩니다.

   그 뒤에 Connect to Host -> Add New SSH Host

   ```
   ssh -i ~/.ssh/[KEY_FILE_NAME] id@외부ip
   ```

   /Users/osangjin/.ssh/config 가 보이면, Enter

8. 키 파일의 권한을 수정해 줍니다.

   ```shell
   cd ~/.ssh
   chmod 400 [KEY_FILE_NAME] [KEY_FILE_NAME].pub
   ```

9. cmd + shift + p 해서 'ssh'만 칩니다.

   등록한 외부 ip가 보이면 비밀번호 입력해서 접속 끝







cf) 

1. 접속

   ```bash
   ssh -i ~/.ssh/osj-gcp-key 21500404@00.00.000.000
   ```

   (물론, rsa key를 GCP에 등록해야 사용할 수 있을 것이다.)

   

2. 파일 전송

   ```bash
   scp -i [인증키] [업로드할 파일] [계정]@[원격ip]:[업로드 디렉토리 경로]
   
   ex)
   scp -i ~/.ssh/osj-gcp-key screenshot.png 21500404@00.00.000.000:~/21500404
   ```

3. 파일 다운로드

   ```bash
   scp -i ~/.ssh/my-ssh-key username@ip-address:remote-file-path local-file-path
   
   ex)
   scp -i ~/.ssh/osj-gcp-key 21500404@00.00.000.000:~/hw1/21500404.tgz ~/Desktop
   ```

   





# NCP

1. **ACG생성**

   - 설정

   ![image](https://user-images.githubusercontent.com/42775225/146666892-a6b7965c-fd9c-482e-8b07-5b72d3cc6a55.png)

   

2. **Server 생성**

   - 포트 포워딩 설정

   ![image](https://user-images.githubusercontent.com/42775225/146666805-357c54ed-64f7-4ab7-acae-40ea7ad69d4d.png)

   ![image](https://user-images.githubusercontent.com/42775225/146666844-42cf445e-4ad3-4fbd-88f2-169ccb47ddde.png)

   - 비밀번호 획득

     ![image](https://user-images.githubusercontent.com/42775225/146666911-04532266-b873-4c63-87b0-3ed095fe26f2.png)

     여기서 Server생성 시에 만든 pem 파일 넣으면, 비밀번호를 말해줌.

   

3. **Public IP생성**

   : 이때의 ip는 ssh 말고, mysql 직접 통신 이런거를 위함.

4. **ssh 접속**

   ```shell
   ssh -p 2222 root@[ip]
   ```

   ![image](https://user-images.githubusercontent.com/42775225/146850938-388c142d-698f-417d-a75b-d347f53a1665.png)





# VIM

명령모드 --> 입력모드 : i

입력모드 --> 명령모드 : Esc

1. 

   ```bash
   shift + a //
   cw // 
   shift + 4 //
   ```









# 컨테이너 접속

```
docker exec -it [컨테이너 ID] /bin/bash
```





# 우분투 시간변경

1. Seoul 파일 있는지 확인 

   ```
   ls /usr/share/zoneinfo/Asia
   ```

2. 시간 변경

   ```
   sudo ln -sf /usr/share/zoneinfo/Asia/Seoul /etc/localtime
   ```



<br />

# 서버가 응답을 못하는 경우

방화벽 port도 열었고,

listen 하고 있다는 것도 확인했음에도

응답을 못하는 경우에는

```
sudo iptables -I INPUT 5 -i ens3 -p tcp --dport 3306 -m state --state NEW,ESTABLISHED -j ACCEPT
```





<br />

# 무료 도메인 등록

==> https://xn--220b31d95hq8o.xn--3e0b707e/





# 도커

1. 컨테이너 전부 삭제

   ```shell
   docker container rm -f $(docker container ls -aq)
   ```

2. 이미지 전부 삭제

   ```shell
   docker image rm -f $(docker image ls -q)
   ```

   







