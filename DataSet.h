#pragma once
#include <cstring>
#include "Util.h"
#include "Config.h"

#include <string>
#include <vector>
#include <map>
using std::string;
using std::vector;
using std::map;

/*
 * a node
 */
class DataNode
{
public:
    int                 label_type;
    int                 label;
    int                 num_attrib;
    vector<int>         attrib;
    vector<double>      value;

    int GetSize() const { return sizeof(int) * (2 + num_attrib) + sizeof(double) * num_attrib; }
};


/*
 * a edge
 */
class DataEdge
{
public:
    int                 a, b, edge_type;    //one point and another point of the edge,

    int GetSize() const { return sizeof(int) * 3; }
};

/*
 * a triangle
 */
class DataTriangle
{
public:
	int                 a, b, c, triangle_type;

	int GetSize() const { return sizeof(int) * 4; }
};

/*
 * a sample including nodes, edges and triangles
 * samples are splited by empty lines
 */
class DataSample
{
public:
    int						num_node;
    int						num_edge;
	int						num_triangle;
    vector<DataNode*>		node;
    vector<DataEdge*>		edge;
	vector<DataTriangle*>	triangle;
	map<int, int>			color;

    ~DataSample()
    {
        for (int i = 0; i < (int)node.size(); i ++)
            delete node[i];
        for (int i = 0; i < (int)edge.size(); i ++)
            delete edge[i];
		for (int i = 0; i < (int)triangle.size(); i++)
			delete triangle[i];
    }

    int GetSize() const
    {
        int size = sizeof(int) * 2;
        for (int i = 0; i < (int)node.size(); i ++)
            size += node[i]->GetSize();
        for (int i = 0; i < (int)edge.size(); i ++)
            size += edge[i]->GetSize();
		for (int i = 0; i < (int)triangle.size(); i++)
			size += triangle[i]->GetSize();
        return size;
    }
};

/*
 * a dataset including many samples
 */
class DataSet
{
public:    
    int num_label;    
    int num_sample;

    int num_attrib_type;
    int num_edge_type;
    
    vector<DataSample*> sample;

    ~DataSet()
    {
        for (int i = 0; i < (int)sample.size(); i ++)
            delete sample[i];
    }
};

/*
 * a global dataset including many samples, label dict, attrib dict, edge type dict
 */
class GlobalDataSet
{
public:
    vector<DataSample*> sample; //set of samples

    int num_label;
    int num_attrib_type;
    int num_edge_type;

    MappingDict         label_dict;
    MappingDict         attrib_dict;
    MappingDict         edge_type_dict;

    void LoadData(const char* data_file, Config* conf);
    void LoadDataWithDict(const char* data_file, Config* conf, const MappingDict& ref_label_dict, const MappingDict& ref_attrib_dict, const MappingDict& ref_edge_type_dict);
};