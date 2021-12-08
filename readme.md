# 代码解释
## 数据集
### mobiled
+表示这个关系的标签是可以获取的  
?表示这个关系的标签是不可以获取的  
1表示有权力关系  
0表示非权力关系  
xxx:y表示属性xxx的值是y  
## 输出结果
每个循环都计算accuracy、precision、recall、F1-score，包括所有node和无标签node  
该结果只对二元分类有效  
opencrf能够采取positive标签的点  
pred.txt给出了所有节点的预测值（包括已知标签节点）  
marginal.txt给出了所有节点所有标签的边缘概率
每个`iter`代表一个迭代，`A_Accuracy`代表所有关系的准确度，`u_Accuracy`代表未标记关系的准确度
## 输入参数
niter 迭代次数  
nbpiter belief传播的迭代次数  
method 最优化方法：梯度下降  
hasvalue 属性是否有值  
novalue 属性没有值
## 训练集
初始的n行描述了n个关系和他们的属性  
？和+表示这个关系的类型是知道或不知道的  
0和1代表了这个关系的类型
属性名:属性值