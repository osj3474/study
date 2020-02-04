#ifndef __RNN__

class HGURNN : public HGUConnectionLayer {
protected:
	int m_hiddenDim;
	int m_hiddenActFun;

	enum WeightType { WT_InputToHidden, WT_HiddenToHidden, WT_HiddenToOutput };
	enum { DefaultMaxTrainStep = 100 };

	int m_aWeightIndex[8];		// # of weights from input to hidden = (m_inDim) * m_hiddenDim
								// # of weights from hidden to hidden = (m_hiddenDim + 1) * m_hiddenDim
								// # of weights from hidden to output = (m_hiddenDim + 1) * m_outDim 

	HGULongArray<Float> m_aHidden[2];
	int m_activeHidden;

	// for train
	int m_maxTrainStep;
	int m_trainStep;
	int m_stepTail;					// circular queue for input, hidden, and output

	int m_deltaDim;					// should be Max(m_outDim, m_hiddenDim);

	HGULongArray<Float> *m_aInputBuffer;				// array size == m_maxTrainStep 
	HGULongArray<Float> *m_aHiddenBuffer;				// array size == m_maxTrainStep 
	HGULongArray<Float> *m_aOutputBuffer;				// array size == m_maxTrainStep 

public:
	HGURNN() : HGUConnectionLayer(){
		m_hiddenDim = 0;

		m_inputWidth = 0;
		m_inputHeight = 1;
		m_inputSize = 0;
		m_noInputChannel = 1;
		
		m_outputWidth = 0;
		m_outputHeight = 1;
		m_outputSize = 0;
		m_noOutputChannel = 1;

		m_actFun = AF_SoftMax;
		m_hiddenActFun = AF_Tanh;

		m_learningCriteria = LC_CrossEntropy;

		for (int i = 0; i < 4; i++)
			m_aWeightIndex[i] = 0;

		m_activeHidden = 0;

		// for train
		m_deltaDim = 0;
		m_maxTrainStep = 0;
		m_trainStep = 0;
		m_stepTail = 0;

		m_aInputBuffer = NULL;
		m_aHiddenBuffer = NULL;
		m_aOutputBuffer = NULL;
	}

	~HGURNN() {
		Delete();
	}

	void Delete() {
		if (m_deltaDim > 0)
			CloseTraining();

		DeleteTrainBuffer();
	}

	int GetHiddenDim()					{	return  m_hiddenDim;	}
	Float GetCurrentHidden(int idx)		{	return m_aHidden[m_activeHidden][idx]; }
	HGULongArray<Float>* GetActiveHidden()	{	return &m_aHidden[m_activeHidden]; }
	void ResetHidden()					{ m_aHidden[0].Fill(0);  m_activeHidden = 0; }
	void ResetOutput()					{ m_aOutput.Fill(0); }

	Float& GetWeight_InputToHidden(int i, int h)		{ return m_aWeight[m_aWeightIndex[WT_InputToHidden] + h * m_inDim + i]; }
	Float& GetWeight_HiddenToHidden(int prevH, int h)	{ return m_aWeight[m_aWeightIndex[WT_HiddenToHidden] + h * (m_hiddenDim + 1) + prevH]; }
	Float& GetWeight_HiddenToOutput(int h, int o)		{ return m_aWeight[m_aWeightIndex[WT_HiddenToOutput] + o * (m_hiddenDim + 1) + h]; }

	Float& GetGradient_InputToHidden(int i, int h)		{ return m_aAccGradient[m_aWeightIndex[WT_InputToHidden] + h * m_inDim + i]; }
	Float& GetGradient_HiddenToHidden(int prevH, int h)	{ return m_aAccGradient[m_aWeightIndex[WT_HiddenToHidden] + h * (m_hiddenDim + 1) + prevH]; }
	Float& GetGradient_HiddenToOutput(int h, int o)		{ return m_aAccGradient[m_aWeightIndex[WT_HiddenToOutput] + o * (m_hiddenDim + 1) + h]; }

	virtual int Alloc(int inputDim, int hiddenDim, int outputDim, int actFun = AF_SoftMax, int hiddenActFun = AF_Tanh);
	
	virtual HGULongArray<Float>* Step(HGULongArray<Float> *pInput);
	virtual HGULongArray<Float>* StepForTrain(HGULongArray<Float> *pInput, HGULongArray<Float> *pDesiredOutput, float *pAccError = NULL);

	int SetMaxTrainStep(int maxTrainStep);
	void DeleteTrainBuffer();
	void ResetTrainStep()	{ m_trainStep = m_stepTail = 0;	}
	int GetTrainStep()		{ return m_trainStep; }
	void SaveCurrentStep();

	HGULongArray<Float>& GetPreviousInput(int backStep)					{ return m_aInputBuffer[(m_stepTail - 1 - backStep + m_maxTrainStep) % m_maxTrainStep]; }
	HGULongArray<Float>& GetPreviousHidden(int backStep)					{ return m_aHiddenBuffer[(m_stepTail - 1 - backStep + m_maxTrainStep) % m_maxTrainStep]; }
	HGULongArray<Float>& GetPreviousOutput(int backStep)					{ return m_aOutputBuffer[(m_stepTail - 1 - backStep + m_maxTrainStep) % m_maxTrainStep]; }

	virtual int PrepareToTrain(Float *pSharedGradient = NULL);		// allocate variables for training
	virtual void CloseTraining();							// deallocate variables for training

	int ComputeWeightedSum(int inDim, int outDim, Float *pInput, Float *pWeight, Float *pOutput, int bBias, int bClear, int pitch = 0);
	// int ComputeDelta0(HGULongArray<Float> *pDesiredOutput, float weight = 1.F, HGULongArray<Float> *pOutput = NULL, HGULongArray<Float> *pDelta0 = NULL);	// defined in HGUConnectionLayer
	int ComputeDeltaFromDelta0(int actFun, int outDim, Float *pOutput, Float *pDelta0, Float *pDelta);
	int ComputeGradientFromDelta(int inDim, int outDim, Float *pDelta, Float *pInput, Float *pGradient, int bBias, int pitch = 0);
	int BackPropagate(int inDim, int outDim, Float *pDelta, Float *pWeight, Float *pPrevDelta0, int bBias, int pitch = 0);

	virtual int AllocWithConfig(HGULayerConfig *, HGUConnectionLayer *) { return FALSE; }
	virtual int Load(HGULayerConfig *pCfg, FILE *fp, int bWeightOnly = FALSE) { return FALSE; }
	virtual int Load(FILE *fp, int bWeightOnly = FALSE) { return FALSE; }
	virtual int Save(FILE *fp, int bWeightOnly = FALSE) { return FALSE; }
	virtual int PropagateBase(HGULongArray<Float> *) { return FALSE; }
	virtual int ComputeGradientFromDelta(void) { return FALSE; }
	virtual void NormalizeWeight(float, int, float) {}
	virtual void ScaleIncommingWeights(float) {}
	virtual void ShiftWeight(float) {}
	virtual void Regularize_L1(float, float) {}
	virtual int BackPropagate(HGULongArray<Float> &, int) { return FALSE; }
	virtual int ComputeGradientFromDeltaOnDevice(int, int) { return FALSE; }
	virtual int BackPropagateOnDevice(HGULongArray<Float> *, int) { return FALSE; }
};

void RNNTest();

#endif // !__RNN__
