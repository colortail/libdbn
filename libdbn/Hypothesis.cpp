#include "Hypothesis.h"

Hypothesis::Hypothesis()
	:sampleSize(0)
{
}


Hypothesis::~Hypothesis()
{
	model = NULL;
	if (this->param != NULL)
		delete param;
	param = NULL;
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

	hypo->param = bnet.getCPTList();

	return hypo;
}

void Hypothesis::setValue(BNet & bnet) {

	switch (modelType) {
	case MODEL_TYPE::SBNET:{
		for (int i = 0; i < bnet.vertexSize(); i++) {
			//do something
			set<Factor> factorSet;
			for (int j = 0; j < param->size(); j++) {
				factorSet.insert(param->at(j));
			}
			bnet.setCPTs(factorSet);
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
		vector<int> lack;
		split(s, splited);
		for (int i = 0; i < splited.size() || i < sampleNum; i++) {
			if (i < splited.size() && "null" != splited[i] && "" != splited[i]) {
			    rowData.push_back(atof(splited[i].c_str()));
			}
			else {
				rowData.push_back(-1);
				lack.push_back(i);
			}
		}
		sampleSize++;
		data.push_back(rowData);
		lacks.push_back(lack);
	}
	ifs.close();
}

void Hypothesis::doMLE() {
	for (int i = 0; i < param->size(); i++) {
		vector<vector<double>> * table = param->at(i).pTable;
		vector<string> names = (*param->at(i).getElementsName());
		for (int j = 0; j < table->size(); j++) {
			//节点在网络中位置 + 该节点值
			std::map<int, double> loc;
			for (int w = 0; w < names.size(); w++) {
				int wLoc = ((BNet*)model)->getNodeIndex(names[w]);
				loc.insert({ wLoc, (*table)[j][w] });
			}
			//statistic
			int cnt = 0;
			for (int k = 0; k < data.size(); k++) {
				bool posi = true;
				for (std::map<int, double>::iterator iter = loc.begin();
					iter != loc.end();
					iter++) {
					//double equals
					if (!(std::abs(data[k][iter->first] - iter->second) < 0.000001)) {
						posi = false;
						break;
					}
				}
				if (posi) {
					cnt++;
				}
			}
			//mle formula
			if (table->at(j).size() > 0) {
				table->at(j)[table->at(j).size() - 1] = (double)cnt / table->size();
			}
		}
		param->at(i).normalize();
	}
}