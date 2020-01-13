# OCR neural network

This is the neural network I programmed in C for the OCR project in 2nd year of EPITA. I developped the neural network alone but the whole OCR was done in a group of 4 in 3 months (This repo only contains my work).

The performances of the neural network were pretty good in training (98% of success over 3000 training examples after 1 min of training), and also good in practise. The majority of the charaters were correctly identified on good quality scanned texts, but there were some problems due to characters separation (sometimes letters like f and t touched and the network received the 2 characters as one) or due to overfitting.

### Network generation
A random network can be generated using newNetwork in network.c. You just have to provide an array on the number of neurons on each layer and the size of this array:  
&nbsp;&nbsp;&nbsp;&nbsp;int layers[] = {784, 100, 10};  
&nbsp;&nbsp;&nbsp;&nbsp;Network* network = newNetwork(layers, 3);  
This will generate a network that takes 784 inputs and gives 10 outputs.

### Network training
It can then be trained using the functions in training.c. The simplest function to use is void learn(Network\* network, long maxTimeSec, TrainingExample\*\* trnExamples, int nbTrn, int batchSize).  
maxTimeSec is the time the training will take in seconds (it will just loop until the time is up)  
trnExamples is the array of training examples used for the training (more on than later). nbTrn is the size of this array.  
batchSize is the number of examples each training will use. On each loop the examples are randomly selected within the trnExamples array.

On the top of the file there is these two lines:  
\#define ETA 1  
\#define USE\_CROSS\_ENTROPY  
ETA is the speed of the training. Increase this value and it will be faster but less precise.  
USE\_CROSS\_ENTROPY tells the program if it has to use the cross entropy cost or not. When it is defined, the training is much much faster, but will reach a maximum and then get worse with time. Without it the training will use the quadratic cost, which is slower but it will keep converging to a 0 cost.  
My advice is to use cross entropy to reach the maximum that can be reached with it, and then use the quadratic cost.

To get the training examples you can use newTrainingExample but it's not convenient. The best way to get them is the readDatabase function. You have to give it a path to your database file and a pointer to the number of examples you want to read (0 to read all, the function will automatically set the number in that case).  
The database file must be of this form:  
nbExamples nbInputsPerEx nbLabelsPerEx  
input0 input1 ... inputn label0 label1 ... labeln  
input0 input1 ... inputn label0 label1 ... labeln  
input0 input1 ... inputn label0 label1 ... labeln  
...  

The inputs are the input that will be given to the network, the labels are the expected outputs of the network  
Example: XOR training examples:  
4 2 1  
0 0 0  
0 1 1  
1 0 1  
1 1 0  

### Database generation

To generate a database you have 3 options:  
- Write it by hand  
- Use the png_to_database.py script that can generate the database file from a folder containing png files (no transparency or it will crash). More info in the script file.  
- Use generateDatabase.c to generate it from a raw image file which is a file with the number of images in the first line and on each line the width, the height and the pixels values separated by spaces. The pixels values are 0 and 1. The labels of each image can be either in order (AUTO_LABELS), given by the user (USER_LABELS), or given in a text file (FILE_LABELS). Use this thing only if you have no other choice, I programmed it in a rush and modified it a lot, some things don't even work.  

### Network simulation

When the network is trained you can test it.  
To get the network answer over a specific input you can use getNetworkAnswer (in network.c). You give it the inputs and it returns the activations of the output neurons. You can then use getMaxIndex to get the index of the neuron that was the most active.  
You can also use network_cost (in training.c) to get the cost of the network over a set of training examples (quadratic cost).

### Network saving

There are functions to save and load networks in saving.c (saveNetwork and readNetwork)
