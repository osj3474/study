# WICWIU 코드 분석 입문서

 : 아래 설명된 코드는 WICWIU/tutorials/MNIST를 기반으로 하고 있다. 설명 하기에 앞서 다음 문장은 아주 중요한 문장이므로 bold처리가 아닌 h4로 강조하겠다.

#### WICWIU 연산의 핵심에는 O.p.e.r.a.t.o.r 가 있다.

위의 말을 강조하는 이유는 WICWIU 코드 이해의 중심에도 Operator가 있기 때문이다. 시작하겠다.



#### [ 클래스 Operator ] 

 1) Operator에게는 4가지 핵심 변수가 있다.

![w1](D:\GitHub\mws\Images\WICWIU정리본\w1.png)

*m_apOutput     =>   Operator의 m_aaResult값을 사용할 Operator들의 주소 값.

*m_apInput        =>   Operator에 input으로  들어오는 Operator들의 주소 값.

*m_aaResult       =>   Operator의 결과 값.

*m_aaGradient  =>   Operator의 Gradient값들의 Array.



2) Operator에게는 2가지 핵심 메서드가 있다.

![w2](D:\GitHub\mws\Images\WICWIU정리본\w2.png)

하지만 Operator.hpp에서는 단순히 정의만 해둘 뿐, Operator를 상속받은 각각의 Operator들이 각자의 forwardprop과 backprop을 정의한다.







## MNIST의 main.cpp

cf) 지금부터는 계속해서 코드가 코드를 타고 상위 클래스로 갔다가 내려왔다가 하게 된다. 



```c++
NeuralNetwork<float> *net = new my_CNN(x, label);
```

단지 위의 한 줄로, Convolution layer, ReLU, Loss Function, Optimizer 가 모두 들어있는 뉴럴 네트워크(앞으로 NN) 모델을 생성, 학습, 평가한다. (my_CNN코드 안을 살펴보면 한 눈에 이해가 된다. 단지, AnalyzeGraph는 잘 이해가 가지 않을 수 있으므로 추후 설명을 추가하겠다.)

다음은 한 epoch에서 일어나는 일이다.

```c++
float train_accuracy = 0.f;
float train_avg_loss = 0.f;

net->SetModeTrain();

for (int j = 0; j < LOOP_FOR_TRAIN; j++) {
    std::vector<Tensor<float> *> * temp =  train_dataloader->GetDataFromGlobalBuffer();
    // 지금 Batch가 100이다. 총 60000개의 데이터가 있으므로 LOPP_FOR_TRAIN은 600이 되고, 위의 함수는 		한 Batch의 input과 label을 가지고 오는 것이다.
    Tensor<float> *x_t = (*temp)[0];
    Tensor<float> *l_t = (*temp)[1];
    delete temp;

    net->FeedInputTensor(2, x_t, l_t);   // (1)
    net->ResetParameterGradient();  
    net->Train();   // (2)
    train_accuracy += net->GetAccuracy();
    train_avg_loss += net->GetLoss();

}
```



(1) net->FeedInputTensor(2, x_t, l_t);
 : 가변인수를 사용하는데, 현재 (input의 갯수, *input tensor*, *label tensor*)이다. 

```c++
template<typename DTYPE> int Module<DTYPE>::FeedInputTensor(int pNumOfInput, ...) {
    Tensor<DTYPE> *temp = NULL;

    va_list ap;  // 파라미터의 ...에 해당하는 인자들을 ap로 접근할 수 있도록한다.
    va_start(ap, pNumOfInput);   // 가변인수 접근을 시작. (가변인자 포인터, 가변인자 갯수)로 준다.

    for (int i = 0; i < pNumOfInput; i++) {
        temp = va_arg(ap, Tensor<DTYPE> *);// 실제로 값을 가지고 오는 부분. (가변인자 포인터, 타입)
        (*m_apInput)[i]->SetResult(temp);  // *m_apInput[i]는 Operator이고 SetResult하면 *m_aaResult에 값이 들어간다.
    }

    va_end(ap);   // 가변인자 변수를 끝낸다.
    return TRUE;
}
```



(2) net->Train();

 : NN의 Train으로 간다. (Device에 따라 메서드 명이 다르게 되어 있다.)

```c++
template<typename DTYPE> int NeuralNetwork<DTYPE>::TrainOnCPU() {
    this->ResetResult();
    this->ResetGradient();
    this->ResetLossFunctionResult();
    this->ResetLossFunctionGradient();

    this->ForwardPropagate();   // forward1
    m_aLossFunction->ForwardPropagate();  // forward2
    m_aLossFunction->BackPropagate();  // backward1
    this->BackPropagate();   // backward2

    m_aOptimizer->UpdateParameter();

    return TRUE;
}
```

reset부분은 나중에 다시 확인하고, this->ForwardPropagate()와 m_aLossFunction->ForwardPropagate()의 차이를 한번 확인해보자.



##### forward1

 :  this->ForwardPropagate()이거는 NN에서가 아니라 Module의 메서드를 사용한다. Module의 forwardPropagate보면 ExcutableOperator에 한해서 forwardPropagate를 진행한다.

```c++
template<typename DTYPE> int Module<DTYPE>::ForwardPropagate(int pTime) {
    for (int i = 0; i < m_numOfExcutableOperator; i++) {
        (*m_aaExcutableOperator)[i]->ForwardPropagate(pTime);
    }
    return TRUE;
}
```

자! 이 때, Module의 상위클래스로 가서 내용을 확인해야겠다고 생각했다면 앞부분을 다시 읽어야 할 것이다. 분명 forward와 backward는 Operator.hpp가 아닌 그걸 상속받는 각각의 Opertator에서 구체화한다고 했다.

cf) Operator.hpp 의 ForwardPropagate()

```c++
template<typename DTYPE> int Operator<DTYPE>::ForwardPropagate(int pTime) {
    #ifdef __DEBUG__

    #endif  // __DEBUG__
    return TRUE;
}
```

각각의 Operator의 forwardpropagate가 모두 다르기 때문에 기본적으로 override하도록 되어있다.



##### forward2

 : m_aLossFunction->ForwardPropagate()는 해당 뉴럴네트워크 모델의 Loss Fucntion의 메서드이다. 그러면 그 녀석의 ForwardPropagate를 봐야할 것이다. MNIST경우 SoftmaxCrossEntropy()를 봐야하겠지만, MSE가 한층 쉬운 Loss Function이므로 MSE로 Loss의 forward가 무슨 일을 하는지 살펴보자. 

```c++
    Tensor<DTYPE>* ForwardPropagate(int pTime = 0) { 
        Tensor<DTYPE> *input  = this->GetTensor();
        Tensor<DTYPE> *label  = this->GetLabel()->GetResult();
        Tensor<DTYPE> *result = this->GetResult();  // 포인터로, 함수를 빠져나가도 변환 값이 유효.

        int batchsize = input->GetBatchSize();
        int channelsize = input->GetChannelSize();
        int rowsize     = input->GetRowSize();
        int colsize     = input->GetColSize();
        
        int capacity    = channelsize * rowsize * colsize; // 육면체 블럭의 크기.

        int ti = pTime;  // 기본적으로 0으로 세팅되어 있다.

        for (int ba = 0, i = 0; ba < batchsize; ba++) {
            i = ti * batchsize + ba; // 여기서 i는 몇번째 batch인지를 말한다.

            for (int j = 0, index = 0; j < capacity; j++) {
                index         = i * capacity + j;
                (*result)[i] += Error((*input)[index], (*label)[index]); 
            }
        }

        return result; 
    }
```

그런데 보면, return이 Tensor타입으로 계산한 결과값을 포인터로 가지고 있다. 그 결과값을 내기 위해서는 반드시 그 앞전의 layer들의 input과 label Tensor값이 정의가 되어야 할 것이다. 그래서 먼저 this->forwardPropagate해준다.



##### backward1

 : m_aLossFunction->BackPropagate()로, 이제 BackProp이다. Loss부터 backprop이 시작된다.

```c++
template<typename DTYPE> Tensor<DTYPE> *LossFunction<DTYPE>::BackPropagate(int pTime) {
    #ifdef __DEBUG__
    std::cout << "LossFunction<DTYPE>::BackPropagate(int pTime)" << '\n';
    std::cout << this->GetName() << '\n';
    #endif  // __DEBUG__
    return NULL;
}
```

역시 없다. 하위에서 override된다. 각각의 backprop이 어떤 계산을 하던지 간에, 딱 한가지만 기억하면, 해당 Loss function에서 this->gradient에 값이 각각의 수식에 맞게 계산되어 저장된다.



##### backward2

 : this->BackPropagate()로, NN에서 부른 메서드이지만, 역시나 Module의 backprop으로 가게 되고, 거기서도 excutableOperator에 한해서 backprop을 진행한다. (반대의 경우인, dummy Operator는 forward, backward가 아닌 값만 가지고 있는 것.)

[아래는 Module의 BackPorpagate]

```c++
template<typename DTYPE> int Module<DTYPE>::BackPropagate(int pTime) {
    for (int i = m_numOfExcutableOperator - 1; i >= 0; i--) {
        (*m_aaExcutableOperator)[i]->BackPropagate(pTime);
    }
    return TRUE;
}
```

그러면 여기서 Operator.hpp에서 backprop이 정의되어있다고 생각했다면, 다시 차근차근 생각해보라.

각각의 Operator는 각각의 backprop 수식을 가지고 있을 것이라, Operator.hpp에 없고 상속받는 하위 클래스에서 계산이 정의되어있다. 딱 한가지만 기억하면, 해당 Operator의 this->gradient에 값이 각각의 수식에 맞게 계산되어 저장된다.





[추가 설명]

 :  Module에서 ForwardPropagate나 BackwardPropagate를 할 때, m_aaExcutableOperator에 한해서 수행한다는데, 언제 m_aaExcutableOperator를 세팅해주는지 확인해보자.

확인결과,  Module의 AnalyzeGraph에서 하더라. 위에서 추가로 설명한다고 한 부분과 일치한다. 

이 메서드의 우ㅏ

```c++
template<typename DTYPE> Operator<DTYPE> *Module<DTYPE>::AnalyzeGraph(Operator<DTYPE> *pResultOperator) {     // NN의 prediction을 내는 마지막 Fully connected layer를 파라미터로 받음.
    // BFS
    Container<Operator<DTYPE> *> queue;  // Operator포인터를 담는 Container를 큐처럼 이용.

    queue.Push(pResultOperator);  // 마지막 Operator를 큐의 마지막에 추가
    m_pLastOperator = pResultOperator;

    Container<Operator<DTYPE> *> *nextOp = NULL;  // 
    Container<Operator<DTYPE> *> *prevOp = NULL;  // 
    int numOfInputEdge                   = 0;

    Operator<DTYPE> *out = NULL;

    while (queue.GetSize() > 0) {
        out = queue.Pop();   // queue에 넣었던 건 마지막 Operator였다. 그걸 순차적으로 할건데 잘보자

        if (!(this->IsInput(out))) {  // (1)
            if (this->IsValid(out)) {  // (2)

                if (out->GetIsTensorholder()) {   // (3)
                    this->SetParameter(out);  // out은 dummy Operator 임.(업데이트 됨.)
                } else {
                    this->SetExecutableOperater(out);  // out은 dummy Operator가 아니라는 말
                }

                nextOp         = out->GetInputContainer(); // nextOp는 input을 담음
                numOfInputEdge = nextOp->GetSize();

                for (int i = 0; i < numOfInputEdge; i++) {
                    prevOp = (*nextOp)[i]->GetOutputContainer(); //prevOp는 nextOp의 out을
                    prevOp->Pop(out);

                    queue.Push((*nextOp)[i]);
                }
            } else continue;
        } else continue;
    }
    // std::cout << '\n';

    m_aaExcutableOperator->Reverse();

    return pResultOperator;
}

```

이것저것 많이하는 것처럼 보이지만, 마지막 Fully connected layer Operator를 queue가장 먼저 넣고, 그거를 새로운 Operator인 out에 할당해서 out을 가지고 이것이 executable인지 할당 하는 것 같다. 계속 가보면, IsInput(out)을 하는데,



##### (1) IsInput()

 : 해당 Operator가 Module의 Input인지 확인하는 메서드

```c++
template<typename DTYPE> int Module<DTYPE>::IsInput(Operator<DTYPE> *pOperator) {
    Container<Operator<DTYPE> *> *m_apInput = this->GetInputContainer();
    int m_InputDegree                       = m_apInput->GetSize();

    for (int i = 0; i < m_InputDegree; i++) {
        if ((*m_apInput)[i] == pOperator) return TRUE;
    }

    return FALSE;
}
```

 보면, 파라미터로 받은 operator가 Module의 Input인지를 확인하게 된다. 만약 Input이라면 Module의 Input Container에 포함되어 있을 것이고, 아니라면 포함되어 있지 않을 것이다. 그러면 여기서 드는 의문이 있다. Module의 Input Container에는 언제 set을 하는 것일까 하는 것이다. 그 답은 이전 layer에 있다.



##### (2) IsValid()

 : 해당 Operaotr의 Output Operator들이 중복으로 포함되는지 확인하는 메서드이다. 



##### (3) GetIsTensorholder()

 :  m_isParameter 에 대해서 정의하고 가겠다. Tensorholder인지 아닌지를 확인하기 위해 만든 변수로, 기본적으로는 False이다. 그런데 Tensorholder 로 생성이 되면, m_isParameter값이 True가 된다. 예를 들어, weight과 input을 matmul하는 경우, weight은 Tensorholder로 생성이 되고, input의 경우는 그냥 Operator로 정의가 된다. 결국 연산에 참여해서 forward, backward를 해서 값이 없데이트가 되는 weight의 경우는 Tensorholder, 즉 파라미터 가 된다.





아래의 코드는 Module에서 가져온 메서드들이다.

```c++
template<typename DTYPE> Operator<DTYPE> *Module<DTYPE>::SetInput(Operator<DTYPE> *pInput) {
    m_apInput->Push(pInput);
    m_InputDegree++;
    this->AddEdgebetweenOperators(pInput);

    return pInput;
}

template<typename DTYPE> Operator<DTYPE> *Module<DTYPE>::SetParameter(Operator<DTYPE> *pParameter) {
    m_apParameter->Push(pParameter);
    m_ParameterDegree++;
    this->AddEdgebetweenOperators(pParameter);
    return pParameter;
}

template<typename DTYPE> Operator<DTYPE> *Module<DTYPE>::SetExecutableOperater(Operator<DTYPE> *pExecutableOperater) {
    m_aaExcutableOperator->Push(pExecutableOperater);
    m_numOfExcutableOperator++;
    return pExecutableOperater;
}

template<typename DTYPE> int Module<DTYPE>::SetInput(int pNumOfInput, ...) {
    Operator<DTYPE> *temp = NULL;

    va_list ap;
    va_start(ap, pNumOfInput);

    for (int i = 0; i < pNumOfInput; i++) {
        temp = va_arg(ap, Operator<DTYPE> *);
        this->SetInput(temp);
    }

    va_end(ap);
    return TRUE;
}

```







(3) net->GetAccuracy 하는데,

이걸 train_accuracy 에 계속 누적시키는데, GetAccuracy 내부로 들어가보면,
NN의 GetResultOperator 에서 Op의 GetResult를 불러서
result에 m_aaResult[0]의 값을 넣어준다. (분명 prediction 값일 텐데, 처음 들어갔을 때를 보면 input값이 들어갔었다.)



cf) Tensor Alloc 에서 LongArray를 생성