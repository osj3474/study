# MS Azure





### 클라우드 구분

: 클라우드 카테고리를 IaaS, SaaS, Paas로 많이 말했었는데, 이제 이 경계는 많이 모호해졌다.  



infrastructure--------------------code

IaaS ----- CaaS ----- PaaS ----- FaaS





|       | IaaS            | PaaS                             | CaaS                  | FaaS            |
| ----- | --------------- | -------------------------------- | --------------------- | --------------- |
| AWS   | EC2             | Elastic Beanstalk                | EC2 Container Service | Lambda          |
| Azure | Virtual Machine | App Service,<br />Service Fabric | Container Service     | Functions       |
| GCP   | Compute Engine  | App Engine                       | Container Engine      | Cloud Functions |



Q. IaaS만 사용한다면, 클라우드를 제대로 못 쓰는 걸까?

=> 새로운 서비스 기획단계라면, FaaS쪽이 유리하다. 하지만, 오른쪽으로 갈수록, 해당 클라우드에 종속된다.





<br />

### 헷갈리는 용어

1. CapEx(자본 비용)

   => 물리적 인프라의 비용을 초기에 지출하고, 시간이 지나면서 초기 비용을 공제하는 것.

2. OpEx(운영 비용)

   => 현재 사용하는 서비스에 대해 청구되는 비용.





<br />

### 왜  Azure를 써야하는가?

- 하이브리드 클라우드! *(이게 핵심)

  => 인증 관리에 대해 완전히 밀결합되어 있다.

  => 기존 On-premise와 결합을 극대화할 수 있게 한다.

  ​		(인증, 기존 인프라, DB, 모던 App 등)

- Office 365(SaaS) 서비스의 강점

  => Azure 서비스와 통합 가능

- 가장 많은 데이터 센터 운영 중

- 강력한 하이브리드(AD, Office365, EMS와 통합)

- 90개 이상의 규정 준수 인증

-  forntune 500대 기업의 95%가 이용중.

  => 기존에 인프라가 있지 않으면, AWS가 좋은 선택.

  => 기존에 인프라가 있으면, 넘어가기가 쉽지 않다. (Azure Stack)

  





<br />

### Azure Stack

: Azure의 하이브리드 클라우드 플랫폼

: 물리적 서버가 내 앞에 있고, Azure의 수많은 서비스 이용 가능.		

​	(이는, 기존 서버를  IaaS로만 쓰다가 MS의 PaaS까지 사용가능해진다.)

:  아래의 **물리적 서버**를 구매 or 대여 해서 사용하면 된다.

| Azure Stack Edge                    | Azure Stack HCI                 | Azure Stack HUb                                      |
| ----------------------------------- | ------------------------------- | ---------------------------------------------------- |
| 현장에서 AI를 제공할 수 있도록 제공 | 비용 효과적으로 가상환경을 제공 | 인터넷이 연결되지 않는 배, 비행기 등에서 도 이용가능 |



<div align=center>
  <img src="./images/Azure/img1.png">

*Source. https://azure.microsoft.com/ko-kr/services/azure-arc/*



<br />

### ARM(Azure Resource Manager) ❕

1. Auth and RBAC
2. Activity logs and telemetry
3. Resource metadata





<br />

### Azure에서 Resource는 무엇일까?

: VM

: Storage

: Virtual network

: Security group

: .....





<br />

### Azure App Service

: Web App, REST API, Mobile Backend를 호스팅하는 HTTP Service이다.

: 보안, 부하분산, AutoScaling, 자동화된 관리 지원한다.





















<br />

### 구조

- Geography
   - Region
      - Data center

  <div align=center>
    <img src="./images/Azure/img2.png">

  ​	*Source. MS특강*



- AD

  : Active Directory, 기업 내에 있는 네트워크에 물려있는 모든 리소스를 통제하는 것.

  (컴퓨터, 사람 등)

  : Windows AD의 클라우드 버전이 아님.

  ​	=> 하지만 Windows AD 서버를 이미 사용하고 있는 경우, Azure AD에 연결해 확장 가능.

  : Windows AD를 대체하기 위한 것이 아님.

  <div align=center>
    <img src="./images/Azure/img3.png">

  *Source. MS특강*

  cf) 기존 Windows AD는 SaaS 지원 안되고, 모바일 기기 통제 기능 없음. (참고 : Intune)

  







<br />

# 실습



<br />

### 실습 관련

1. 디렉토리(테넌트)

   : 하나의 회사라고 생각하자.

   ex) handong.edu

2. 구독

   : Azure 계정과 연결된 클라우드 서비스 리소스의 놀리적인 단위이며, 비용을 청구하는 단위이다.

   



cf) 어떠 서비스 어느  리전에서 사용가능한지 확인하기

: https://azureservices.io/ 







<br />

### CLI 참고

1. azure 시작

   ```bash
   az
   ```

2. vscode 도 가능하다.

   ```bash
   code .
   ```









<br />

### Azure Virtual Machine 실습

1. 리소스 그룹 만들기, 윈도우 VM 만들기

   ```bash
   az
   az group create --name myResourceGroup --location koreacentral
   az vm create \
   --resource-group myResourceGroup \
   --name myVM_Win01 \
   --image win2016datacenter \
   --admin-username azureuser \
   --admin-password P@ssw0rd0000
   ```

   

2. 윈도우 pc열기

   : 윈도우 + R

   ```
   mstsc /v:{publicIP}
   ```

   

3. 리눅스 VM 만들기

   ```bash
   az vm create \
   --resource-group myResourceGroup \
   --name myVM_Ubu01 \
   --image UbuntuLTS \
   --admin-username azureuser \
   --generate-ssh-keys
   ```

   

4. NGINX 설치

   ```bash
   az vm open-port \
   --port 80 \
   --resource-group myResourceGroup \
   --name myVM_Ubu01
   
   ssh azureuser@{publicIp}
   
   sudo apt-get -y update
   sudo apt-get -y install nginx
   ```

   

5. WAS설치 확인

   : 브라우저에서 {publicIp} 를 쳐본다.

   <div align=center>
     <img src="./images/Azure/img4.png">





6. 삭제해보기

   ```bash
   az group delete --name myResourceGroup
   ```

   



### WebApp 만들기

1. 닷넷

   ```bash
   mkdir myWebApp01
   cd myWebApp01
   
   dotnet new webapp
   
   dotnet run
   ```

   

2. 웹

   ```bash
   az webapp up -g myResourceGroup -p myPlan \
   --location koreacentral \
   --name <사번>myWebApp01 \
   --os-type linux
   ```

   

3. Docker Compose 기반

   ```bash
   git clone https://github.com/Azure-Samples/multicontainerwordpress
   cd multicontainerwordpress
   
   az appservice plan create -n myAppPlan -g myResourceGroup \
   --sku S1 \
   --is-linux
   
   az webapp create -n <사번>myWebApp03 -g myResourceGroup \
   --plan myAppPlan \
   --multicontainer-config-type compose \
   --multicontainer-config-file docker-compose-wordpress.yml
   ```

   





<br />

### Azure Container Registry

: Private Docker Registry를 만들 수 있는 서비스입니다.

1. CR만들기

   ```bash
   az acr create --name <사번>myregistry --resource-group myResourceGroup --sku standard --admin-enabled true
   ```







cf) Azure의 mysql 사용하기

```bash
az mysql server create \
--resource-group myResourceGroup \
--name <사번>mySql01 \
--location "koreacentral" \
--admin-user adminuser \
--admin-password P@ssw0rd0000 \
--sku-name B_Gen5_1 \
--version 5.7

az mysql db create \
--resource-group myResourceGroup \
--service-name <사번>mySql01 \
--name wordpress

az webapp config appsettings set \
--resource-group myResourceGroup \
--name <사번>myWebApp03 \
--settings WORDPRESS_DB_HOST="<사번>mySql01.mysql.database.azure.com" WORDPRESS_DB_USER="adminuser@<사번>mySql01.mysql.database.azure.com"
WORDPRESS_DB_PASSWORD="P@ssw0rd0000" WORDPRESS_DB_NAME="wordpress" 
MYSQL_SSL_CA="BaltimoreCyberTrustroot.crt.pem"
```

 





<br />

### ACI(Azure Container Instances)

: 컨테이너(어플리케이션을 패키징하고, 관리하기 위한 세트)를 실행할 수 있게 해주는 서비스

1. ACI 생성

   ```bash
   az container create -g myResourceGroup -n mycontainer01 \
   --image microsoft/aci-helloworld \
   --ports 80 \
   --dns-name-label 82224038aci01 \
   --location koreacentral
   ```

   

2. Docker랑 동일하게 사용하면 된다.

   ```bash
   az container show -g myResourceGroup -n mycontainer01 \
   --query "{FQDN:ipAddress.fqdn,ProvisioningState:provisioningState}" \
   --out table
   
   az container list -g myResourceGroup --output table
   
   az container logs -g myResourceGroup -n mycontainer01
   
   az container attach -g myResourceGroup -n mycontainer01
   
   az container delete -g myResourceGroup -n mycontainer01
   
   az group delete --name myResourceGroup
   ```



cf) 랜덤 이름 만들고 싶을 때, 참고

```bash
DNS_NAME_LABEL = aci-demo-$RANDOM
echo $DNS_NAME_LABEL
```





<br />

### Azure Cosmos DB

: 완전 관리형 NoSQL(분산될 수 있고, scale-out가능하고, 스키마를 강제하지 않음.) DB이다.

: 다양한 API를 지원한다.





<br />

### Azure Functions

: 적은 코드를 쓰고, 인프라를 작게 유지하는 비용절감 솔루션이다.

: 쪼개고 쪼개다 보면, 함수 단위가 된다.

: 어플리케이션 실행에만 집중할 수 있게 한다.

: 실시간 데이터 처리

: 트리거 타입

- HTTP Request Trigger

  : url요청오면 처리

- Timer Trigger

  : 일정 주기에 대한 처리

- Queue Trigger

- Cosmos DB Trigger

  : DB생성, 업데이트 시

- Blob Trigger

  : 파일 업로드





1. HTTP Request Trigger

   : `리소스 그룹` 에 들어간다.

   : `추가`

   : `Function App`

   : 이번에는 'Node.js' 로 생성한다.

   : 리소스로 이동한다.

   : `함수` 탭을 선택한다.

   : `추가`

   : `HTTP trigger` 선택하고, 함수이름 설정한다.

   : `코드+테스트` 이동

   : `테스트/실행` 으로 확인

   

   

2. Timer Trigger

   : `일정`  설정하면, 해당 주기마다 함수가 호출된다.







<br />

### Azure RBAC

: Azure role-based access control, 역할을 분리해줘야할 때 사용한다.

: 사전 정의한 RBAC 기본 역할을 제공

ex) 한 사용자는 VM을 관리하고, 다른 사용자는 가상 네트워크를 관리하도록 권한을 부여한다.











