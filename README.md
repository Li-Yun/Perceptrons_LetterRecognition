# Perceptrons_LetterRecognition

This program implmented a simple letter-recognition classifier using Perceptron classifiers. <br />
Basically, the perceptron takes 16 elements as a feature of letters and feeds all training <br />
instances into the perceptron to train the perceptron classifier. When training the perceptrons, <br />
I applied a all-pair approach to acheive a multi-class recognntion task. <br />

Dataset: http://archive.ics.uci.edu/ml/datasets/Letter+Recognition <br />

# Usage

To compile the program, please follow this command: <br />

g++ -std=c++11 main.cpp Perceptron.cpp -o file_name 

For example: 

g++ -std=c++11 main.cpp Perceptron.cpp -o output 

To start training all perceptron classifiers, please use the following command: <br />

./output training

To test new instances in a testing dataset, please run the following command: <br />

./output testing

Have fun!!
