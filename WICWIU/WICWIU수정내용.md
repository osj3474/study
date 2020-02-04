### 1. LossFunction 수정내용

1. 멤버 변수 정리 

   | 삭제 내용                             | 해당 변수 접근 방법              |
   | ------------------------------------- | -------------------------------- |
   | **Tensor**<DTYPE> *m_aResult          | this->GetResult()                |
   | **Tensor**<DTYPE> *m_aGradient        | this->GetGradient()              |
   | **Operator**<DTPYE> *m_pInputOperator | *(this->GetInput())              |
   | **Tensor**<DTYPE> *m_pInputTensor     | *(this->GetInput())->GetResult() |
   | **std::string** m_name                | GetName()                        |
   | **Device** m_Device                   | GetDevice()                      |
   | **int** m_idOfDevice                  | GetDeviceID()                    |



2. 멤버 함수 정리

   | 삭제 내용                                          | 해당 함수 사용 방법                    |
   | -------------------------------------------------- | -------------------------------------- |
   | **void** SetResult(**Tensor**<DTYPE> *pTensor)     | **int** SetResult(동일)                |
   | **void** SetGradient(**Tensor**<DTYPE> *pTensor)   | **int** SetGradient                    |
   | **Tensor**<DTYPE> *GetResult() const               | **Tensor**<DTYPE> *GetResult() const   |
   | **Tensor**<DTYPE> *GetGradient() const             | **Tensor**<DTYPE> *GetGradient() const |
   | **std::string**            GetName() const         | **std::string** GetName() const        |
   | **virtual Device**      GetDevice()                | **Device**      GetDevice()            |
   | **virtual int**              GetDeviceID()         | **int** GetDeviceID()                  |
   | **int **                           ResetResult()   | **int** ResetResult()                  |
   | **int**                            ResetGradient() | **int** ResetGradient()                |

   | 변경 전                                         | 변경 후                                                 |
   | ----------------------------------------------- | ------------------------------------------------------- |
   | **virtual Tensor**<DTYPE>* ForwardPropagate     | **virtual int** ForwardPropagate  // return(NULL->0)    |
   | **virtual Tensor**<DTYPE>* BackPropagate        | **virtual int** BackPropagate        // return(NULL->0) |
   | **Operator**<DTYPE>      * GetOperator() const; | **Operator**<DTYPE>      * GetOperator();               |
   | **Tensor**<DTYPE>        * GetTensor() const;   | **Tensor**<DTYPE>        * GetTensor();                 |

   

### 2. LossFunction 하위 클래스들 수정

 : HindgeLoss, SoftmaxCrossEntropy



## LongArray.hpp

| 기존 | 추가           |
| ---- | -------------- |
| X    | void SetOne(); |





## Tensor.hpp

| 기존 | 추가           |
| ---- | -------------- |
| X    | void SetOne(); |





## HindegeLoss.hpp

| 기존                                             | 수정                                  |
| ------------------------------------------------ | ------------------------------------- |
| Tensor<DTYPE>* ForwardPropagate(int timeIdx = 0) | int ForwardPropagate(int timeIdx = 0) |
| Tensor<DTYPE>* BackPropagate(int pTime = 0)      | int BackPropagate(int pTime = 0)      |



## SoftmaxCrossEntropy.hpp

| 기존                                           | 수정                                |
| ---------------------------------------------- | ----------------------------------- |
| Tensor<DTYPE>* ForwardPropagate(int pTime = 0) | int ForwardPropagate(int pTime = 0) |
| return result;                                 | return 0;                           |
| Tensor<DTYPE>* BackPropagate(int pTime = 0)    | int BackPropagate(int pTime = 0)    |
| return NULL:                                   | return 0;                           |



## SoftmaxCrossEntropy_CUDA.cu

| 기존                                                         | 수정                                                         |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| Tensor<DTYPE> *SoftmaxCrossEntropy<DTYPE>::ForwardPropagateOnGPU(int pTime) | int SoftmaxCrossEntropy<DTYPE>::ForwardPropagateOnGPU(int pTime) |
| return result;                                               | return 0;                                                    |
| Tensor<DTYPE> *SoftmaxCrossEntropy<DTYPE>::BackPropagateOnGPU(int pTime) | int SoftmaxCrossEntropy<DTYPE>::BackPropagateOnGPU(int pTime) |
| return NULL:                                                 | return 0;                                                    |



## my_CNN.hpp

| 기존                                                         | 수정                                                     |
| ------------------------------------------------------------ | -------------------------------------------------------- |
| SetLossFunction(new SoftmaxCrossEntropy<float>(out, label, "SCE")); | out = new SoftmaxCrossEntropy<float>(out, label, "SCE"); |











