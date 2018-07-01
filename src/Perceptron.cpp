#include "Perceptron.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <random>

using namespace std;

// activation function: sgn function
int sign_function(float input_value)
{
	float outpu_value;
    
	if(input_value >= 0.0)
	{
		outpu_value = 1.0;
	}
	else
	{
		outpu_value = -1.0;
	}
	
	return outpu_value;
}

// compute label vector of training data
vector<float> compute_label_vector(vector< vector<float> > merge_matrix)
{
    // label vector
    vector<float> label_vector;
    
    
    // find two different types
	float type_1 = merge_matrix[0][0];
	float type_2 = 0;
	for(int i = 0 ; i < merge_matrix.size() ; i++)
	{
		if(merge_matrix[i][0] != type_1)
		{
			type_2 = merge_matrix[i][0];
			break;
		}
	}
	
    
	// determine class label
	float first_type;
	float second_type;
	if(type_1 < type_2)
	{
		first_type = type_1;
		second_type = type_2;
	}
	else if(type_2 < type_1)
	{
		first_type = type_2;
		second_type = type_1;
	}
    
    
    // build label vector
	for (int i = 0 ; i < merge_matrix.size() ; i++)
	{
		if (merge_matrix[i][0] == first_type)
		{
			label_vector.push_back(1.0);
		}
		else if(merge_matrix[i][0] == second_type)
		{
			label_vector.push_back(-1.0);
		}
	}
    
    
    return label_vector;
}

// perceptron learning algorithm, the output of perceptron is weight vector
vector<float> perceptron_learning(vector< vector<float> > merge_matrix, vector<float> label_vector, float weight_value[])
{
    // setting parameters
    float learning_rate = 0.2;
    int maxmum_iteration_number = 11000;
    float weight_array[11000][17] = {0};
    int accuracy_array[11000] = {0};
    vector<float> weight_vector;
    int iteration_index;
    
    
	// perceptron learning, the loop of epochs
	for (iteration_index = 1 ; iteration_index <= maxmum_iteration_number ; iteration_index++)
	{
        int accuracy_number = 0;
        
        
        // the loop of checking every training example in each iteration
        for(int example_index = 0 ; example_index < merge_matrix.size() ; example_index++)
        {
            // compute neuron value (output)
            float temp_output = weight_value[0];
            for(int i = 1 ; i < 17 ; i = i + 1)
            {
                temp_output = temp_output + (weight_value[i] * merge_matrix[example_index][i]);
            }
            int neuron_output = sign_function(temp_output);
                
                
            // check whether weight value needs to be updated or not
            if( neuron_output != label_vector[example_index])
            {
                // update weight value
                weight_value[0] = weight_value[0] + (learning_rate * 1.0 * label_vector[example_index]);
                for(int i = 1 ; i < 17 ; i++)
                {
                    weight_value[i] = weight_value[i] + (learning_rate * merge_matrix[example_index][i] * label_vector[example_index]);
                }
            }
            else
            {
                // the classified result is correct
                accuracy_number = accuracy_number + 1;
            }
                
        }
            
        
		// record weight values in each epoch
		for(int index = 0 ; index < 17 ; index++)
		{
			weight_array[iteration_index][index] = weight_value[index];
		}
        
		// fill accuracy into accuracy array
		accuracy_array[iteration_index] = accuracy_number;
        
        
        // compute the difference
        double difference_value = ( float(accuracy_array[iteration_index]) / float(merge_matrix.size()) ) - ( float(accuracy_array[iteration_index - 1]) / float(merge_matrix.size()) );
        if (difference_value < 0)
        {
            difference_value = 0.0 - difference_value;
        }
        
        
        // check out stop condition
		if( difference_value <= 0.001 )
		{
            // choose correct weight values as final weight vector, and record weight values
            if (accuracy_array[iteration_index] <= accuracy_array[iteration_index - 1])
            {
                for (int i = 0 ; i < 17 ; i++)
                {
                    weight_vector.push_back(weight_array[iteration_index - 1][i]);
                }
                
            }
            else if(accuracy_array[iteration_index] > accuracy_array[iteration_index - 1])
            {
                for (int i = 0 ; i < 17 ; i++)
                {
                    weight_vector.push_back(weight_array[iteration_index][i]);
                }
                
            }
            
			break;
		}
        
    }
    
	return weight_vector;
}


// execute perceptron function, this function returns perceptron's weight vector
vector<float> Perceptron::execution(char training_data_name[])
{
	// initialize weight value between -1.0 and 1.0
	float weight_value[17] = {0};
	//for (int i = 0 ; i < 17 ; i++)
	//{
    //    weight_value[i] = -0.99 + ( (float)rand() / (float)RAND_MAX ) * (0.99 - (-0.99));
    //}
	
    // initialize weight value between -1.0 and 1.0
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-0.8, 0.8);
    for (int i = 0 ; i < 17 ; i++)
    {
        weight_value[i] =  dis(gen);
    }
    
	// read training data into one matrix
	ifstream inputfile(training_data_name); // open the file
	int index_1 = 0;
	string line_1;
    
	while (getline(inputfile, line_1))
	{
		float temp_value;
		stringstream string_stream_1(line_1);
		merge_matrix.push_back( vector<float>() );
        
		while (string_stream_1 >> temp_value)
        {
            merge_matrix[index_1].push_back(temp_value);
        }
		
		index_1 = index_1 + 1;
	}
	inputfile.close();
    
    
	// get label vector
    vector<float> label_array = compute_label_vector(merge_matrix);
    
    
	// perceptron learning
	vector<float> result = perceptron_learning(merge_matrix, label_array, weight_value);
    
	return result;
}
