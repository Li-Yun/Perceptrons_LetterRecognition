# include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include "Perceptron.h"
#include <algorithm>

using namespace std;

// calculate testing accuracy and build confusion matrix
void build_confusion_matrix(vector< vector<float> > testing_data, vector<int> classification_result)
{
	// setting parameters
	int confusion_matrix[26][26] = {0};
    
    
	// calculate accuracy for testing data
	int correct_number = 0;
	for (int example_index = 0 ; example_index < testing_data.size() ; example_index++)
	{
		if ( classification_result[example_index] == testing_data[example_index][0] )
		{
			correct_number++;
		}
	}
	float testing_accuracy = ( (float)correct_number / (float)testing_data.size() ) * 100;
    
	cout << "Classification accuracy: " << testing_accuracy << "% " << endl;
    
    
	// build confusion matrix
	for (int i = 0 ; i < testing_data.size() ; i++)
	{
		int true_class = testing_data[i][0] - 1;
		int predicted_class = classification_result[i] - 1;
        
		confusion_matrix[true_class][predicted_class] = confusion_matrix[true_class][predicted_class] + 1;
        
	}
    
	// write confusion matrix.
	ofstream confusion("../confusion_matrix.txt");
	for (int i = 0 ; i < 26 ; i++)
	{
		for (int j = 0 ; j < 26 ; j++)
		{
			confusion << confusion_matrix[i][j] << "\t";
		}
		confusion << "\n";
	}
    cout << "Confusion Matrix is stored into ML_hw1 folder." << endl;
    
}

// all-pairs method
void all_pairs_training()
{
	// setting parameters
	char training_data_name[50];
	int file_count = 0;
	float weight_set_matrix[325][17] = {0};
    
	// train all possible combinations, 325 perceptrons
	for (int i = 1 ; i <= 26 ; i++)
	{
		for (int j = i + 1 ; j <= 26 ; j++)
		{
			std::cout << "Perceptron: " << (file_count + 1)  << endl;
			// read each training file
			sprintf(training_data_name, "../new training data/%d.txt", file_count + 1);
            
			// training single perceptron
	        Perceptron* result = new Perceptron();
	        vector<float> weight_set = result-> execution(training_data_name);
            
			// build weight matrix
			for (int i = 0 ; i < 17 ; i++)
			{
				weight_set_matrix[file_count][i] = weight_set[i];
			}
            
			file_count++;
		}
	}
    
    
	// write weight_set_matrix to txt file
	ofstream output("../weight_set.txt");
	for (int i = 0 ; i < 325 ; i++)
	{
		for (int j = 0 ; j < 17 ; j++)
		{
			output << weight_set_matrix[i][j] << " ";
		}
		output << "\n";
	}
    
}

// execute testing function
void testing_function()
{
	// setting parameters
	int label_set[325][2];
	int file_count = 0;
	int letter_count[26];
	vector<int> classification_result;
    
    
	// build label matrix
	for (int i = 1 ; i <= 26 ; i++)
	{
		for (int j = i + 1 ; j <= 26 ; j++)
		{
			label_set[file_count][0] = i;
			label_set[file_count][1] = j;
            
			file_count++;
		}
	}
	
    
	// read the file of weight set
	vector< vector<float> > weight_set_matrix;
	ifstream input_weight_file("../weight_set.txt"); // open the file
	int index_2 = 0;
	string line_2;
    
	while (getline(input_weight_file, line_2))
	{
		float temp_value_2;
		stringstream string_stream_2(line_2);
		weight_set_matrix.push_back( vector<float>() );
        
		while (string_stream_2 >> temp_value_2)
		{
			weight_set_matrix[index_2].push_back(temp_value_2);
		}
		index_2 = index_2 + 1;
	}
	input_weight_file.close();
	
    
	// read testing data
	vector< vector<float> > testing_data;
	ifstream inputfile("../new testing data/aaa_2.txt"); // open the file
	int index_1 = 0;
	string line_1;
    
	while (getline(inputfile, line_1))
	{
		float temp_value;
		stringstream string_stream_1(line_1);
		testing_data.push_back( vector<float>() );
        
		while (string_stream_1 >> temp_value)
        {
            testing_data[index_1].push_back(temp_value);
        }
		
		index_1 = index_1 + 1;
	}
	inputfile.close();
    
    
	// all-pairs testing
	for (int example_index = 0 ; example_index < testing_data.size() ; example_index++)
	{
		vector<int> voting_vector;
        
		// using each perceptron to predict instance's class
		for(int perceptron_index = 0 ; perceptron_index < 325 ; perceptron_index++)
		{
            
			// compute neuron value
			float temp_output_2 = weight_set_matrix[perceptron_index][0];
			for(int i = 1 ; i < 17 ; i++)
			{
				temp_output_2 = temp_output_2 + (weight_set_matrix[perceptron_index][i] * testing_data[example_index][i]);
			}
			
			// predict the result
			if( temp_output_2 >= 0 )
			{
				voting_vector.push_back(label_set[perceptron_index][0]);
			}
			else if( temp_output_2 < 0)
			{
				voting_vector.push_back(label_set[perceptron_index][1]);
			}
            
		}
        
		// voting the number of each class
		for (int class_index = 0 ; class_index < 26 ; class_index++)
		{
			letter_count[class_index] = count(voting_vector.begin(), voting_vector.end(), (class_index + 1) );
		}
        
        // find the position of max value from the letter_count array
        int position_index = distance(letter_count, max_element(letter_count, letter_count + 26));
        
		// record classification result
		classification_result.push_back( position_index + 1 );
        
	}
    
	// build confusion matrix
	build_confusion_matrix(testing_data, classification_result);
    
}

// main program
int main(int argc, char* argv[])
{
    if ( argc != 2 )
    {
        cout << "Program Error." << endl;
        cout << "This program must have two arguments: (1)./a.out and (2) [training/testing]." << endl;
        cout << "Please try again." << endl;
    }
	else
    {
        // convert char array to string
        string input_string(argv[1]);
        
        // Input format is correct
        if ( input_string.compare("training") == 0 )
        {
            // classification task.
            all_pairs_training();
        }
        else if ( input_string.compare("testing") == 0 )
        {
            // testing function
            testing_function();
        }
        
    }
	
	return 0;
}





