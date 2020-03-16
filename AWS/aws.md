# VPC

 : Amazon **VPC**는 Amazon **EC2의 네트워킹 계층**입니다.

(A *virtual private cloud* (VPC) is a virtual network dedicated to your AWS account.)

1. ***서브넷***은 VPC의 **IP 주소 범위**입니다. 

   (A *subnet* is a range of IP addresses in your VPC.)

2. ***라우팅 테이블***에는 **네트워크 트래픽을 전달할 위치를 결정**하는 데 사용되는 라우팅이라는 규칙 집합이 포함되어 있습니다.

   (A *route table* contains a set of rules, called routes, that are used to determine where network traffic is directed.)

3. ***인터넷 게이트웨이***는 수평 확장되고 가용성이 높은 중복 VPC 구성 요소로, VPC의 **인스턴스와 인터넷 간에 통신**할 수 있게 해줍니다. 따라서 네트워크 트래픽에 가용성 위험이나 대역폭 제약 조건이 발생하지 않습니다.

   (An *internet gateway* is a horizontally scaled, redundant, and highly available VPC component that allows communication between instances in your VPC and the internet. It therefore imposes no availability risks or bandwidth constraints on your network traffic.)

4. *VPC 엔드포인트*를 통해 인터넷 게이트웨이, NAT 디바이스, VPN 연결 또는 AWS Direct Connect 연결을 필요로 하지 않고 PrivateLink 구동 지원 AWS 서비스 및 VPC 엔드포인트 서비스에 비공개로 연결할 수 있습니다. VPC의 인스턴스는 서비스의 리소스와 통신하는 데 퍼블릭 IP 주소를 필요로 하지 않습니다. VPC와 기타 서비스 간의 트래픽은 Amazon 네트워크를 벗어나지 않습니다.

   (A *VPC endpoint* enables you to privately connect your VPC to supported AWS services and VPC endpoint services powered by PrivateLink without requiring an internet gateway, NAT device, VPN connection, or AWS Direct Connect connection. Instances in your VPC do not require public IP addresses to communicate with resources in the service. Traffic between your VPC and the other service does not leave the Amazon network.)



# EC2

1. 한 region의 여러개의 AZ를 가진다. (적어도 3개의 데이터 센터)

   => Availability Zones are in isolated data centers.

2. AWS console은 region scoped이다. (IAM, S3 은 Global이다.)

3. 되도록이면, Elastic IP쓰지 말고, random public IP 와 DNS name을 등록해서 써라. (root 53)

4. private IP란? 특징을 살펴보자. 일단 ssh로 접근이 안된다. 또 인스턴스를 껐다가 키면 public IP는 날라가 버리는데, private IP는 날라가지 않는다. (백서) 프라이빗 IPv4 주소는 인터넷을 통해 연결할 수 없는 IP 주소입니다. 프라이빗 IPv4 주소는 동일 VPC에서 인스턴스 간의 통신을 위해 사용될 수 있습니다. 

5. EC2 User Data

   1) It is possible to bootstrap our instances using an EC2 User data script.

   2) It runs with the root user, and only run once at the instance first start.

   3) You want to provide startup instructions to your EC2 instances, you should be using EC2 User Data.

   : user data 는 Advanced details에서 추가할 수 있는데, ec2가 생성될 때, 자동으로 그 script를 실행해서 생성해준다.(유용할 듯.)

6. Anything that ends with a letter is an AZ.

7. You are getting a **permission error** exception when trying to SSH into your Linux Instance.

   => the key is missing permissions **chmod 0400**.

8. You are getting a network **timeout** when trying to SSH into your EC2 instance

   => security groups are **misconfigured**.

9. Security groups can reference all of the following - (IP address, CIDR block, Security Group)

   => **NOT DNS name**.

10. database in EC2. requires block storage for lowest cost.

=> EBS Cold HDD Volume (S3, Glacier은 block storage가 아니다.)

11. cost saving을 위해서 어떤 launch type을 가져가는게 유리한지 결정할 필요가 있다.

    1) On demand : short workload

    2) Reserved : database 사용할 경우 (세부적으로, 그냥타입, convertible타입, scheduled타입)

    3) Spot : for cheap, less reliable

    4) Dedicated : no share (instance : hardware 공유x, hosts : 서버 전체 예약)

### Instance 정리

![image-20200227161436026](/Users/osangjin/Library/Application Support/typora-user-images/image-20200227161436026.png)

![image-20200227161512696](/Users/osangjin/Library/Application Support/typora-user-images/image-20200227161512696.png)![image-20200227161522363](/Users/osangjin/Library/Application Support/typora-user-images/image-20200227161522363.png)![image-20200227161535473](/Users/osangjin/Library/Application Support/typora-user-images/image-20200227161535473.png)

![image-20200227161552407](/Users/osangjin/Library/Application Support/typora-user-images/image-20200227161552407.png)

![image-20200227161606881](/Users/osangjin/Library/Application Support/typora-user-images/image-20200227161606881.png)

![image-20200227161617067](/Users/osangjin/Library/Application Support/typora-user-images/image-20200227161617067.png)





# IAM

 : IAM = Identity and Access Management, 나중에 IAM 설정은 꼭하자.. (정말 중요한거 같다.)



1. Policies is written in JSON(JavaScript Object Notation)

2. One IAM **User** per **physical person**. One IAM **Role** per **application**. IAM credentials should never be shared.

3. All **inbound** traffic is **blocked** by default. All **outbound** traffic is **authorized** by default.

4. Two different private networks can have the same IPs.

5. Machines connect to WWW using an internet gateway (a proxy).

6. IAM components are (Users, Roles, Policies, Groups)

=> **NOT Organisation**

7. IAM is a global service (encompasses all regions)

8. An IAM user can belong to multiple groups.
9. 



# AMI

 : freezing해서 그 상태 save해두는 것.

![image-20200227172637738](/Users/osangjin/Library/Application Support/typora-user-images/image-20200227172637738.png)

1. **Copying** a source AMI results in an identical but distinct target AMI with its **own unique identifier**.

2. 중요하다고 했던 것은, 바로 copy를 할 수 있는 것이 아니라, EC2 instance를 시작하고, 새롭게 해야한다?

3. Placement Groups

   1) Cluster : 한 곳에 집중, 빠르나 한번 파괴되면 끝. (for high performance)

   2) Spread : 여러 AZ로 분산 저장. (for safe saving)

   3) Partition : 한 AZ 안에서 partition. (for big data)



# ENI

 : elastic network interface (개념 추가 필요)



1. failover(시스템 대체 작동)에 매우 효과적이다. 
2. EC2를 만들면 기본적으로 eth0이 할당된다. 그런데 Network Ineterface에서 ENI를 만들고, EC2에 이를 할당할 수 있다. 그러면, 해당 EC2는 eth0, eth1를 가진다. 



# EC2 Hibernate

 : Instance를 stop할 때, 그냥 stop도 있고, stop with hibernate도 있는데, 후자를 선택하면, Instance는 stop되나, OS는 stop되지 않음. (어떤 의미가 있을까 ?)



# EC2 Final

1. Cluster placement groups places your instances next to each other giving you high performance computing and networking

2. You plan on running an open-source MongoDB database year-round on EC2. Which instance launch mode should you choose?

   => Reserved Instances

3. You built and published an AMI in the ap-southeast-2 region, and your colleague in us-east-1 region cannot see it

   => An AMI created for a region can only be seen in that region.

4. ![image-20200229152921572](/Users/osangjin/Library/Application Support/typora-user-images/image-20200229152921572.png)

5. You would like to deploy a database technology and the vendor license bills you based on the physical cores and underlying network socket visibility. Which EC2 launch modes allows you to get visibility into them?

   => Dedicated hosts

6. Creating an AMI after installing the applications allows you to start more EC2 instances directly from that AMI, hence bypassing the need to install the application (as it's already installed)

   ![image-20200229153328497](/Users/osangjin/Library/Application Support/typora-user-images/image-20200229153328497.png)
   
7. Vertical Scaling = scale up / down

   Horizontal Scaling = scale out / in (Auto Scaling Group, Load Balancer)



# ELB

 : load balancers are servers that forward internet traffic to multiple servers (EC2 Instances) downstream.

ELB (EC2 Load Balancer) is a managed load balancer. 

1. internal-private ELB
2. external-public ELB

#### 왜 사용하는가?

1. Spread load across multiple downstream instances
2. Expose a single point of access (DNS) to your application
3. Seamlessly handle failures of downstream instances
4. Do regular health checks to your instances
5. Provide SSL termination (HTTPS) for your websites
6. Enforce stickiness with cookies
7. High availability across zones
8. Separate public traffic from private traffic



같이 사용되는 ELB 개념

Stickiness : 설정한 시간 동안은 하나의 ip를 사용한다.

Cross-Zone Load Balancing : 다른 zone의 EC2도 접근 가능. (ALB는 불가능하다. CLB, NLB는 가능.)



# SSL / TLS

1. SSL : csecure sockets 



















