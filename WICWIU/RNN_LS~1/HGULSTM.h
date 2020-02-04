#ifndef __LSTM__
#define __LSTM__

class HGULSTM : public HGURNN {
//	enum WeightType { WT_InputToHidden, WT_HiddenToHidden, WT_HiddenToOutput };		// defined in HGURNN.h
	enum WeightType { WT_InputToHidden, WT_HiddenToHidden, WT_HiddenToOutput, WT_OutputToHidden, WT_InputGate, WT_ForgetGate, WT_OutputGate };


//	int m_aWeightIndex[8];		// declared in HGURNN.h
								// # of weights from input to hidden = (m_inDim) * m_hiddenDim
								// # of weights from output to hidden = (m_outDim + 1) * m_hiddenDim
								// # of weights from hidden to output = (m_hiddenDim + 1) * m_outDim 
								// # of weights for inputGate = (m_inDim + m_hiddenDim + 1)
								// # of weights for forgetGate = (m_inDim + m_hiddenDim + 1)
								// # of weights for outputGate = (m_inDim + m_hiddenDim + 1)

	HGULongArray<Float> m_hidden0;
	HGULongArray<Float> m_hidden2;

	HGULongArray<Float> m_aInputGate;
	HGULongArray<Float> m_aForgetGate;
	HGULongArray<Float> m_aOutputGate;

	int m_gatePitch;			// input degree of gate networks (inDim + outDim + 1)

	HGULongArray<Float> m_aDelta0_hidden;		// gradient w.r.t. hidden
	HGULongArray<Float> m_aDelta_hidden;		// gradient w.r.t. NET_hidden

public:
	HGULSTM() : HGURNN() {
//		m_actFun = AF_Tanh;
//		m_learningCriteria = LC_MSE;

		m_actFun = AF_SoftMax;
		m_learningCriteria = LC_CrossEntropy;

		m_hiddenActFun = AF_Tanh;

		m_gatePitch = 0;
	}

	Float& GetWeight_OutputToHidden(int prevO, int h)	{ return m_aWeight[m_aWeightIndex[WT_OutputToHidden] + h * (m_outDim + 1) + prevO]; }
	Float& GetGradient_OutputToHidden(int prevO, int h)	{ return m_aAccGradient[m_aWeightIndex[WT_OutputToHidden] + h * (m_outDim + 1) + prevO]; }

	Float& GetWeight_InputGate()		{ return m_aWeight[m_aWeightIndex[WT_InputGate]]; }
	Float& GetWeight_ForgetGate()		{ return m_aWeight[m_aWeightIndex[WT_ForgetGate]]; }
	Float& GetWeight_OutputGate()		{ return m_aWeight[m_aWeightIndex[WT_OutputGate]]; }

	Float& GetGradient_InputGate()		{ return m_aAccGradient[m_aWeightIndex[WT_InputGate]]; }
	Float& GetGradient_ForgetGate()		{ return m_aAccGradient[m_aWeightIndex[WT_ForgetGate]]; }
	Float& GetGradient_OutputGate()		{ return m_aAccGradient[m_aWeightIndex[WT_OutputGate]]; }

	virtual int Alloc(int inputDim, int hiddenDim, int outputDim, int actFun = -1, int hiddenActFun = -1);
	virtual HGULongArray<Float>* Step(HGULongArray<Float> *pInput);
	virtual HGULongArray<Float>* StepForTrain(HGULongArray<Float> *pInput, HGULongArray<Float> *pDesiredOutput, float *pAccError = NULL);

	virtual int PrepareToTrain(Float *pSharedGradient = NULL);		// allocate variables for training
	virtual void CloseTraining();							// deallocate variables for training

	void SetGateBias(int inDim, int outDim, Float *pWeight, Float v);
};

#endif // !__LSTM__
