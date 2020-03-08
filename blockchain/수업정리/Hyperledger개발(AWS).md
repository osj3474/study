# Hyperledger fabric 개발 with AWS



### AWS EC2로 할 경우 권장 스펙

- Ubuntu 18.04
- memory 2GB
- Storage 16GB



### 개발환경 세팅 코드

##### 1. curl, docker 설치

```bash
sudo apt-get update
sudo apt-get install curl
sudo apt-get install docker.io docker-compose
sudo apt-get install software-properties-common
sudo usermod -aG docker $USER
```



##### 2. Node.js 설치

```bash
sudo apt-get update
sudo apt-get install build-essential libssl-dev
curl -sL https://raw.githubusercontent.com/creationix/nvm/v0.31.0/install.sh -o install_nvm.sh
bash install_nvm.sh
source ~/.profile
nvm install v8.11.1
```



##### 3. Go 설치

```bash
curl -O https://storage.googleapis.com/golang/go1.11.2.linux-amd64.tar.gz
tar -xvf go1.11.2.linux-amd64.tar.gz
sudo mv go /usr/local
sudo ln -s /usr/local/go/bin/go /usr/local/bin/go
vi ~/.profile
export GOPATH=$HOME/go
export PATH=$PATH:/usr/local/go/bin:$GOPATH/bin
source ~/.profile

sudo reboot
```



##### 4. 패브릭 Sample 설치, Docker 이미지 설치

1. Google검색 : hyperledger fabric tutorial 
2. Getting Started - Install Samples, Binaries and Docker Images

```bash
curl -sSL http://bit.ly/2ysbOFE | bash -s -- 1.4.4 1.4.4 0.4.18
* 하다보면 docker 못 쓰고, sudo docker 해야되는데, 그거 권한 주고 재부팅해야한다.(위에 있음)
```



##### 5. 하이퍼레저 explorer

1. Google검색 : hyperledger fabric explorer
2. Get the code
3. READ_ME 대로 설치한다.

```bash
기본적으로 다음의 프로그램이 설치되어 있어야한다.
- Nodejs (완료)
- postgreSQL    => sudo apt-get install postgresql
- jq            => sudo apt-get install jq
- docker (완료)
- docker-compose (완료)
```



##### 6. postgreSQL DB만들어주고, fabcar예제에 맞게 설정

```bash
git clone https://github.com/hyperledger/blockchain-explorer.git
cd blockchain-explorer/app/persistence/fabric/postgreSQL
chmod -R 775 db/
cd blockchain-explorer/app/persistence/fabric/postgreSQL/db
sudo -u postgres ./createdb.sh
cd blockchain-explorer/app/platform/fabric
vi config.json     => con

{
        "network-configs": {
                "fabcar-network": {
                        "name": "fabcar-network",
                        "profile": "./connection-profile/fabcar.json"
                }
        },
        "license": "Apache-2.0"
}
whereis node
sudo ln -s /home/ubuntu/.nvm/versions/node/v8.11.1/bin/node /usr/local/bin/node
```



##### 7. docker로 fabcar 네트워크 실행

```bash
cd fabric-samples/fabcar
git checkout v1.4.1 (버전확인!)
./startFabric.sh
```



##### 8. 네트워크 확인

```bash
docker ps -a 해서 ca, orderer, peer, couchdb, cli 있는지 확인하기. 
```

밑은 명령어 참고.

```bash
[ 지우기 ] (지금은 지우면 안됩니다.)
docker rm -f $(docker ps -aq)
docker rmi -f $(docker images dev-* -q)
docker network prune
[ 확인 ]
docker ps -a
docker images dev-*
docker network ls
```



##### 9. Explorer 실행시키기

```bash
cd blockchain-explorer
./main.sh install
./main.sh test
(./main.sh clean 해서 새롭게 해야한다!)
./start.sh
./syncstart.sh (혹시 json 파일이나 등 수정한 거 있으면 이거 실행)
```



##### 10. 트러블 슈팅

```bash
[ 트러블 슈팅 ]
1. tab키가 먹히지 않는 경우
 : 한도 용량 이상으로 시스템이 차지하고 있어서 mount가 되지 않은 상황, 수시로 df . 혹은 df -h 와 같은 명령어로 현재 사용 가능한 용량확인을 잘하기. 빠져 나올 수 있는 방법은 용량이 많이 먹을 것 같이 의심이 되는 폴더를 du ./해당폴더 해서 확인해서 지운다. log, temp와 같은 것들 지우거나 아예 패브릭 Sample다운받았던거 docker rmi -f $(docker images -q)로 지우고, scale-up 한 다음에 다시 샘플 받기.
 
2. next command line 'npm install update'라는 명령어에서 멈춰버린 경우
 : 메모리가 부족하여 시간이 오래 걸리거나 먹통이 된 상황이다. scale-up을 해주어야 한다.
 
3. 웹 브라우저에 퍼블릭 IP를 넣었는데 안 열리는 경우
 : AWS 인바운드 규칙에서 포트를 열어준다. 가령 8080포트를 열어주면, 퍼블릭IP:8080으로 들어가면 된다.
 
4. npm 문제가 아니니 log를 봐라는 말이 나오면 build-essential을 제대로 안 해준 것.
 : sudo apt install build-essential
```

