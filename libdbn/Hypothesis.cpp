#include "Hypothesis.h"

Hypothesis::Hypothesis()
	:sampleSize(0)
{
}


Hypothesis::~Hypothesis()
{
	model = NULL;
	for (int i = 0; i < param.size(); i++) {
		if (param[i] != NULL)
			delete[] param[i];
		param[i] = NULL;
	}
}

void Hypothesis::releaseHypothesis() {
	this->~Hypothesis();
}

Hypothesis* Hypothesis::init(BNet& bnet) {
	Hypothesis* hypo = new Hypothesis();
	
	hypo->modelType = MODEL_TYPE::SBNET;
	hypo->model = (void *)(&bnet);

	hypo->sampleNum = bnet.vertexSize();

	if (hypo->sampleNum <= 0)
		throw exception("该贝叶斯网无法初始化参数");

	for (int i = 0; i < hypo->sampleNum; i++) {
		int varParamSize = bnet.getVarRange(i) * bnet.getPaVarsRange(i);
		hypo->param[i] = new double[hypo->sampleNum];
		memset(hypo->param[i], 0, hypo->sampleNum);
	}
	return hypo;
}

void Hypothesis::setValue(BNet & bnet) {

	switch (modelType) {
	case MODEL_TYPE::SBNET:{
		for (int i = 0; i < bnet.vertexSize(); i++) {
			//do something
		}
		break;
	}
	case MODEL_TYPE::DBNET: {
		break;
	}
	default:break;
	}

}

void Hypothesis::readBNetData(string & filename) {
	ifstream ifs;
	ifs.open(filename.c_str(), ifstream::in);
	
	size_t readSize = 0;
	char buff[1024];
	while (ifs.getline(buff, sizeof(buff))) {
		string s(buff);
		vector<string> splited;
		vector<double> rowData;
		split(s, splited);
		for (int i = 0; i < sampleNum; i++) {
			if (i < splited.size()) {
			    rowData.push_back(atof(splited[i].c_str()));
				printf("%f ", rowData[i]);
			}
			else {
				rowData.push_back(-1);
				printf("%f ", rowData[i]);
			}
		}
	}
	ifs.close();
}
