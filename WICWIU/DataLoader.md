# DataLoader

### 1. 멤버변수

```c++
Dataset<DTYPE> *m_pDataset;
std::queue<vector<int> *> m_splitedIdxBuffer;   // 여기에 idx가 죄다 들어간다.
std::queue<vector<Tensor<DTYPE>*> *> m_globalBuffer;
```

```c++
m_lenOfDataset = Dataset 60000개 이런식
numOfBatchBlockSize = 총 block 갯수
BatchSize = 한 block 안에 data갯수
m_numOfEachDatasetMember = 
```



### 2. 멤버함수

```c++
1. StartProcess
2. StopProcess
3. DistributeIdxOfData2Thread
4. DataPreprocess
5. Push2IdxBuffer
6. GetIdxSetFromIdxBuffer
7. Concatenate
8. Push2GlobalBuffer
9. GetDataFromGlobalBuffer
```

```c++
1. StartProcess  // 3. => 5. => 4. => 6. => 7. => 8.

m_nowWorking = TRUE;
m_aThreadForDistInfo = std::thread([&](){this->DistributeIdxOfData2Thread();});
m_aaWorkerForProcess를 각각, DataPreprocess();
```

```c++
2. StopProcess 

for(Worker갯수만큼) sem_post(&globalEmpty); m_aaWorkerForProcess[j].join();
sem_post(&m_distIdxEmpty);
m_aThreadForDistInfo.join();
```

```c++
3. DistributeIdxOfData2Thread   // while문으로 Push2IdxBuffer 한다.

세팅을 먼저해준다.
    numOfbatchBlockSize = m_lenOfDataset / m_batchSize;

while(m_nowWorking)
    std::vector<int> *setOfIdx = std::vector<int>(m_batchSize);
	for(batchSize만큼) 
        (*setOfIdx)[i] = alloc해줌
    this->Push2IdxBuffer(setOfIdx); 
```

```c++
4. DataPreprocess   // localBuffer에 데이터 넣고, preprocessedData에 local묶음을 넣는다.

while(m_nowWorking)
    setOfIdx = this->GetIdxSetFromIdxBuffer();
	for(batchsize만큼)
        idx와 data를 세팅해준다.
        for(m_numOfEachDatasetMember만큼)
            localBufferp[j].push((*data)[j]);
	delete setOfIdx;
	for(m_numOfEachDatasetMember만큼)
        (*preprocessedData)[k] = this->Concatenate(localBuffer[k]);
	this->Push2GlobalBuffer(preprocessedData);
```

```c++
5. Push2IdxBuffer  // m_splitedIdxBuffer에 index를 넣는다.

m_distIdxEmpty 와 m_distIdxMutex -1 해주고
m_splitedIdxBuffer.push(setOfIdx);
m_distIdxMutex 와 m_distIdxFull  +1 해준다.
```

```c++
6. GetIdxSetFromIdxBuffer  // m_splitedIdxBuffer에서 데이터 가지고 온다.

m_distIdxFull 와 m_distIdxMutex -1 해주고
m_splitedIdxBuffer 맨 앞쪽 데이터 꺼내준다.
m_distIdxMutex 와 m_distIdxEmpty +1 해준다.
```

```c++
7. Concatenate // setofData(localBuffer)를 묶게된다.

setOfData(localBuffer[k] 임.)를 레퍼런스로 받아와서
Tensor<DTPYE> *result;

for(batchSize만큼)
    temp = setOfData.front();
    for(capacity만큼)
		result에 temp넣음
```

```c++
8. Push2GlobalBuffer   // globalBuffer에 preprocessedData를 넣는다.

preprocessedData 받아와서

m_globalEmpty 와 m_globalMutex -1 해주고
m_globalBuffer.push(preprocessedData);
m_globalMutex 와 m_globalFull  +1 해준다.
```

```c++
9. GetDataFromGlobalBuffer  // m_globalBuffer에서 데이터 가지고 온다.

m_globalFull 와 m_globalFull   -1 해주고
m_globalBuffer 맨 앞쪽 데이터 꺼내준다.
m_globalMutex 와 m_globalEmpty +1 해준다.
```

