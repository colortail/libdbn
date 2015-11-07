class Metric {
	virtual std::string getType() = 0;
	virtual int operator()(BNet & bn, int x) = 0;
};

class MinNeighbors : public Metric {

};

class MinWeight : public Metric {

};

class MinFill : public Metric {

};

class WeightedMinFill : public Metric {

};