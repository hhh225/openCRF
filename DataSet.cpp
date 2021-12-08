#include "DataSet.h"
#include "Constant.h"

#include <cstdio>

#include <iostream>

#define MAX_BUF_SIZE 65536

vector<string> GetLabels(const char *data_file) //
{
    char buf[MAX_BUF_SIZE];
	char *eof;
	set<string> set_labels;
	vector<string> tokens;

    FILE *fin = fopen(data_file, "r");     //open data file
    std::cout<<"labels:"<<'\n';
    for (;;)
	{
        eof = fgets(buf, MAX_BUF_SIZE, fin);      //read a line

        // One Sample finished. (by eof / emptyline)
        if (eof == NULL || strlen(buf) == 1)
			break;


        tokens = CommonUtil::StringTokenize(buf); //split a line into many tokens with space

        if (tokens[0] == "#edge" || tokens[0] == "#triangle" || tokens[0] == "#color")   //ignore the edge and triangle and color line
			continue;
        set_labels.insert(tokens[0].substr(1));
        std::cout<<tokens[0].substr(1)<<' ';
    }
    std::cout<<'\n';
	fclose(fin);
	vector<string> labels(set_labels.begin(), set_labels.end());
	std::cout<<"return value:"<<std::endl;
    for (int i = 0; i < labels.size(); ++i) {
        std::cout<<labels[i]<<' ';

    }
    std::cout<<std::endl;
	return labels;
}

void GlobalDataSet::LoadData(const char* data_file, Config* conf)// data_file is the file's name
{
    char        buf[MAX_BUF_SIZE];
    DataSample* curt_sample = new DataSample();
    char*       eof;

    vector<string>  tokens;

	vector<string> labels = GetLabels(data_file); //get node-labels, like 1,0
	std::cout<<"size labels:"<<SIZE(labels)<<std::endl;
	for (int i = 0; i < SIZE(labels); ++i)

		label_dict.GetId(labels[i]);    //mappingdic  string string int

    FILE        *fin = fopen(data_file, "r");     //open data file(such as train file)

    for (;;)
    {
        eof = fgets(buf, MAX_BUF_SIZE, fin);


        if (eof == NULL || strlen(buf) == 1)     //one sample finish
        {
            if (curt_sample->node.size() > 0)
            {
                curt_sample->num_node = curt_sample->node.size();
                curt_sample->num_edge = curt_sample->edge.size();
				curt_sample->num_triangle = curt_sample->triangle.size();
                sample.push_back(curt_sample);   //add curt_sample to set of samples
                curt_sample = new DataSample();
            }            
            if (eof == NULL) break;    //if get to end of datafile
            else continue;   //if
        }


        tokens = CommonUtil::StringTokenize(buf); //split the line with spaces

        if (tokens[0] == "#edge") //edge
        {
            DataEdge* curt_edge = new DataEdge();
            curt_edge->a = atoi(tokens[1].c_str());//start point
            curt_edge->b = atoi(tokens[2].c_str());//end point
            curt_edge->edge_type = 0;//edge type
            if (tokens.size() >= 4)
                curt_edge->edge_type = edge_type_dict.GetId( tokens[3] );

            curt_sample->edge.push_back(curt_edge);
        }
		else if (tokens[0] == "#triangle")//triangle
		{
			DataTriangle *t = new DataTriangle();

			t->a = atoi(tokens[1].c_str());     //three nodes of triangle
			t->b = atoi(tokens[2].c_str());
			t->c = atoi(tokens[3].c_str());
			if (tokens.size() <= 4)
				t->triangle_type = 0;
			else
				t->triangle_type = atoi(tokens[4].c_str());     //type of triangle

			curt_sample->triangle.push_back(t);
		}
		else if (tokens[0] == "#color")
		{
			curt_sample->color[atoi(tokens[1].c_str())] = atoi(tokens[2].c_str());
		}
        else //node
        {
            DataNode* curt_node = new DataNode();

            char label_type = tokens[0][0];    //+(know the label) or ?(unknow the label)
            string label_name = tokens[0].substr(1);  //1(positive) or 0(negative)

            curt_node->label = label_dict.GetId(label_name);
            if (label_type == '+')
                curt_node->label_type = Enum::KNOWN_LABEL;
            else if (label_type == '?')
                curt_node->label_type = Enum::UNKNOWN_LABEL;
            else {
                fprintf(stderr, "Data format wrong! Label must start with +/?\n");
                exit ( 0 );
            }
            
            for (int i = 1; i < (int)tokens.size(); i ++)
            {
                if (conf->has_attrib_value)
                {
                    vector<string> key_val = CommonUtil::StringSplit(tokens[i], ':');
                    curt_node->attrib.push_back( attrib_dict.GetId(key_val[0]) );
                    curt_node->value.push_back( atof(key_val[1].c_str()) );
                }
                else
                {
                    curt_node->attrib.push_back( attrib_dict.GetId(tokens[i]) );
                    curt_node->value.push_back(1.0);
                }
            }

            curt_node->num_attrib = curt_node->attrib.size();
            curt_sample->node.push_back( curt_node );
        }
    }


    num_label = label_dict.GetSize();    //label is the point
    num_attrib_type = attrib_dict.GetSize();
    num_edge_type = edge_type_dict.GetSize();
    if (num_edge_type == 0) num_edge_type = 1;

    fclose(fin);
}

void GlobalDataSet::LoadDataWithDict(const char* data_file, Config* conf, const MappingDict& ref_label_dict, const MappingDict& ref_attrib_dict, const MappingDict& ref_edge_type_dict)
{
    char        buf[MAX_BUF_SIZE];
    DataSample* curt_sample = new DataSample();    
    char*       eof;

    vector<string>  tokens;

    FILE        *fin = fopen(data_file, "r");

    
    for (;;)
    {
        eof = fgets(buf, MAX_BUF_SIZE, fin);

        // One Sample finished. (by eof / emptyline)
        if (eof == NULL || strlen(buf) == 1)
        {
            if (curt_sample->node.size() > 0)
            {
                sample.push_back(curt_sample);
                curt_sample = new DataSample();
            }            
            if (eof == NULL) break;
            else continue;
        }

        // Parse detail information
        tokens = CommonUtil::StringTokenize(buf);

        if (tokens[0] == "#edge") //edge
        {
            DataEdge* curt_edge = new DataEdge();

            curt_edge->a = atoi(tokens[1].c_str());
            curt_edge->b = atoi(tokens[2].c_str());

            curt_edge->edge_type = 0;
            if (tokens.size() >= 4)
                curt_edge->edge_type = ref_edge_type_dict.GetIdConst( tokens[3] );
            
            if (curt_edge->edge_type >= 0)
                curt_sample->edge.push_back( curt_edge );
        }
		else if (tokens[0] == "#triangle")
		{
			DataTriangle *t = new DataTriangle();

			t->a = atoi(tokens[1].c_str());
			t->b = atoi(tokens[2].c_str());
			t->c = atoi(tokens[3].c_str());
			if (tokens.size() <= 4)
				t->triangle_type = 0;
			else
				t->triangle_type = atoi(tokens[4].c_str());

			curt_sample->triangle.push_back(t);
		}
        else //node
        {
            DataNode* curt_node = new DataNode();
            curt_node->label = ref_label_dict.GetIdConst(tokens[0].substr(1));
            
            for (int i = 1; i < (int)tokens.size(); i ++)
            {
                if (conf->has_attrib_value)
                {
                    vector<string> key_val = CommonUtil::StringSplit(tokens[i], ':');
                    int attrib_id = ref_attrib_dict.GetIdConst(key_val[0]);
                    if (attrib_id >= 0)
                    {
                        curt_node->attrib.push_back( attrib_id );
                        curt_node->value.push_back( atof(key_val[1].c_str()) );
                    }
                }
                else
                {
                    int attrib_id = ref_attrib_dict.GetIdConst( tokens[i] );
                    if (attrib_id >= 0)
                    {
                        curt_node->attrib.push_back( attrib_id );
                        curt_node->value.push_back(1.0);
                    }
                }
            }

            curt_sample->node.push_back( curt_node );
        }
    }
    
    num_label = ref_label_dict.GetSize();
    num_attrib_type = ref_attrib_dict.GetSize();
    num_edge_type = ref_edge_type_dict.GetSize();
    if (num_edge_type == 0) num_edge_type = 1;

    fclose(fin);
}
