#ifndef __program__Perceptron__
#define __program__Perceptron__

#include <vector>

using namespace std;

class Perceptron
{
private:
    // combined matrix
    vector< vector<float> > merge_matrix;
public:
    Perceptron(){};
    ~Perceptron(){};
    
    // execution function
    vector<float> execution(char training_data_name[]);
};

#endif