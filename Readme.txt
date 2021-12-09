Welcome to use the code under the terms of the license for research or non commercial purposes, however please acknowledge its use with a citation:



1. implementation

      Triad Factor Graph Model                    
	by Tiancheng Lou, Tsinghua University 
   -est -niter 10 -nbpiter 10 -dstmodel model.txt -method gradient -gradientstep 0.1 -hasvalue -trainfile example.txt -testfile example.txt                                                                        
  Usage: mpiexec -n NUM_PROCS OpenCRF <task> [options]                   
  Options:                                                               
   task: -est                                             
                                                                         
   -niter int           : number of iterations
   -nbpiter int         : number of iterations in belif propgation       
   -srcmodel string     : (for -estc) the model to load                  
   -dstmodel string     : model file to save                             
   -method string       : methods (lbfgs/gradient), default: gradient    
   -gradientstep double : learning rate                                  
   -hasvalue            : [default] attributes with values (format: attr:val)
   -novalue             : attributes without values (format: attr)       
   -trainfile string    : train file                                     
   -testfile string     : test file                                      


2. data preparation
First n line describes n variable nodes and their attribute factors in the factor graph mode. 
A line starts with character '?' or '+',
where '?' indicates the label of this node is UNKNOWN,
while '+' implies a known label.
The following character is the label of this node.
Note that even if we assume the label is unknown for the model, you can still give the label here, if it is actually available.
The model will ignore the label of '?' sample during the training process, but it can be used in the evaluation.
Separated by space, the rest tokens in a line are organized as "[feature name]:[feature value]", depicting the attrubutes of the node.
The feature value can be a real number, and the feature name should be a string without space.
+表示这个点的标签知道，?表示这个点的标签不知道
-----------
example:
+1 feature_cnt_ratio_1:1 paper_cover_ratio_11:1 coauthor_paper_cnt_6:1 conference_cover_ratio_11:1
**This line depicts a node with a known label '1'.
Four features are given: "feature_cnt_ratio_1", "paper_cover_ratio_11", "coauthor_paper_cnt_6" and "conference_cover_ratio_11".
The values of all these four features are 1, while the other features appearing in other lines but not this line, has the value 0.
-----------
The following m lines establish the factors between nodes.
The factors can only be established in two nodes.
A line started by "#edge", followed by two positive integers i and j, indicates a factor between node i and node j. (Node described in the i-th line of this file is node i)
At the end of this line is a string, indicating the name of this factor.
Factors with the same name share the same form of function g(yi, yj).

-----------
example:
#edge 143 4289 same-advisor
**Nodes in the 143-rd line and the 4289-th line are correlated with a "same-advisor" factor. 
-----------

For transfer learning across different networks, please list all networks using the above format into one file. To clarify, for features you want to transfer across different networks, please use the same feature prefix(e.g., feature_cnt_ratio_1) as well as edge and triad features. Otherwise please use different feature prefix.

3. output     explaining of output hhhhhh
The accuracy, precision, recall, and F1-score are calculated in each iteration, over both the sets of All nodes and Unlabeled nodes.
Note that the result is only valid for binary classification. OpenCRF will take the label of the first line as "positive" label.
"pred.txt" gives the prediction result of all the nodes (including known nodes).
The result is presented by the label id. 0 represents the first kind of label detected in the train file, 1 represents the second kind of label encountered in the file, 2 for the third kind of label, etc.
"marginal.txt" gives the marginal probability of each label for every node. Each line, the marginal probability is presented in the order of label id (p(y=label0), p(y=labe1), p(y=label2)....) 


The is beta version code used in paper Confluence: Conformity Influence in Large Social Networks, may has some problems.

Feel free to contact me if you have any questions:

Sen Wu: ronaldosen@gmail.com

