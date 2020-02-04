# Recurrent Neural Networks

 : feed forward network(일반적인 네트워크) 와는 달리 time series data(과거 혹은 미래의 데이터가 현재 데이터에 영향을 미치는 데이터)를 처리하기 위해서 recurrent connection이 있는 networks.



### 1) RNN 도식과 수식



![rnn 1](https://user-images.githubusercontent.com/42775225/73590972-47944b80-452c-11ea-9a70-fc23d8ecd013.PNG) : input에 해당하는 x셀, hidden에 해당되는 h셀, output에 해당되는 y셀 모두 각 state를 벡터이며, connection weight과의 matrix multiplication으로 정의됨.

![rnn 2](https://user-images.githubusercontent.com/42775225/73590992-7f02f800-452c-11ea-9917-1a62bba55c98.png)



### 2) 장점

배경 : [과거state] --> [현재state] --> [미래state] 로 가는데(transition때마다) 같은 function을 사용한다.

cf) 여기서 사용되는 function

 : NN으로 training시 connection weight값을 최적화 함. 실제 RNN을 동작할 때는 최적화된 고정값을 사용한다.



결국,

RNN의 장점은 transition때마다 적용되는 function이 하나이기 때문에

1.  데이터의 길이에 제약이 없다.
2. 하나의 파라미터를 공유한다.



### 3) Activation Function

 : RNN의 경우 activation function을 일반적으로 tanh을 많이 사용함. (RELU의 경우 과거의 정보 없어짐. 음수의 값을 0으로 보내버림으로.)



### 4) 다양한 RNN

#### 4-1) Bi-direction RNN

 : hidden state가 2개로, 현재state가 과거state와 미래state의 영향을 모두 받아 판단함. 그래서 forward connection과 backward connection이 있다. 



#### 4-2) Deep RNN

 :     output

​          RNN

​          RNN

​          RNN

​         input



### 5) Encoder와 Decoder

 : RNN은 Encoder와  Decoder의 기반이 된다.



1) Encoder : 하나의 summary vector인 context vector를 만듦

2) Decoder : context vector와 이전 output을 가지고 현재 output을 만듦

![rnn 3](https://user-images.githubusercontent.com/42775225/73590995-92ae5e80-452c-11ea-8400-0a6c53085596.PNG)

![rnn 4](https://user-images.githubusercontent.com/42775225/73590996-92ae5e80-452c-11ea-8de2-a2f3c61c1ef0.PNG)

cf) Encoder와 Decoder을 결합한 것이 번역기의 구조가 된다.

cf) 이 과정에서 log를 사용하는 이유는 곱하기를 더하기로 바꾸고, exponential도 있다면 없애준다.



또한 Encoder와 Decoder는 Attention개념과 항상 같이 쓰이게 되는데, 그 전에 알아야 할 것은 A라는 state는 A와 가까운 과거의 정보일수록 손실이 적다. 하지만 어떤 경우에는 우리가 초점을 맞춰야 하는 state가 꽤 오래 전 과거의 state일 수도 있다. 이 경우 Encoder에서 Decoder로 정보가 전달이 될 때, 

![rnn 5](https://user-images.githubusercontent.com/42775225/73590997-92ae5e80-452c-11ea-8db3-367daa5ab44f.PNG)

각 state 전체의 weighted average가 전달이 되는데 각 state의 가중치를 다르게 주어 decoder에서 적절한 output을 낼 수 있도록 하는 개념이다. 그렇다면 어느 state에 얼마만큼의 가중치를 줄지는 NN으로 구한다.

다음은 위의 내용을 수식으로 정리한 것이다.

![rnn 6](https://user-images.githubusercontent.com/42775225/73590998-9346f500-452c-11ea-9af8-472c3702e8af.PNG)



### 6) RNN Training

![rnn 8](https://user-images.githubusercontent.com/42775225/73590999-9346f500-452c-11ea-924c-8b090a4ea983.png)

6-1) Loss function

 : loss function이 maximum likelihood인데, 우리는 loss가 가장 낮은 loss function의 최소값을 찾기 때문에 loss function은 maximum likelihood 앞에 '-' 가 붙은 negative log-likelihood 를 사용한다.



수식설명]

 : x들은 입력 데이터의 전체이고 이것이 given term으로 주어진다. y(t)는 우리가 바라는 desired output이고 확률값이다.  

cf) negative log-likelihood는 cross-entropy와 같아짐.



전체 Loss는 각 타임의 loss를 다 더한





#### 6-2) Optimizer

 : 일반적으로 Gradient Descent Algorithm을 사용하는데, Chain Rule을 바탕으로 BPTT(Back Propagation Through Time)한다.



cf) Deviation of activation function

 1) tanh

![rnn 9](https://user-images.githubusercontent.com/42775225/73591000-93df8b80-452c-11ea-9a78-41260e8c0a00.png)

![rnn 10](https://user-images.githubusercontent.com/42775225/73591001-93df8b80-452c-11ea-89ee-9a5012ec4e51.png)

2) softmax

![rnn 11](https://user-images.githubusercontent.com/42775225/73591002-93df8b80-452c-11ea-8f6b-babc7f067543.png)

![rnn 12](https://user-images.githubusercontent.com/42775225/73591004-94782200-452c-11ea-8fba-c174b2d5ddcb.png)





#### LSTM

 : LSTM은 기본적인 모델의 컨셉은 RNN과 같으나, RNN이 가지고 있는 Gradient vanishing 문제를 해결하기 위해서 고안된 모델이다. RNN에서는데이터의 시계열이 길어지면, 먼 과거의 정보가 현재에 전달하기 어렵게 되며, 이에 따라 정보가 소실되는 문제가 발생한다. LSTM은 이러한 문제를 해결하기 위해서 고안된 모델이다. 아래의 그림으로 보면, O은 그 정보를 인정해서 넘겨주는 것이고, _는 넘겨주지 않는다.





![rnn 13](https://user-images.githubusercontent.com/42775225/73591005-94782200-452c-11ea-8433-7d0854553821.PNG)







