#pragma once
#include"matrix.h"
#include"config.h"
#include<iostream>
#include<string.h>
#include<fstream>
using namespace std;

class Dataloader{
    Matrix* input;
    Matrix* ground_truth;
    int data_num=0;
    int dimension=0;
public:
    int get_d(){return this->dimension;}
    int get_data_num(){return this->data_num;}
    Matrix& get_input_matrix(){return *(this->input);}
    Matrix& get_ground_truth(){return *(this->ground_truth);}

    void load_data(const char* filename){
        ifstream f;
        f.open(filename);
        string s;
        if(f){
            get_line_and_dimension(f);

            input = new Matrix(data_num,dimension); // 加1为了增加一维与常数b组合
            ground_truth = new Matrix(1, data_num);

            f.clear();
            f.seekg(0, ios::beg);
            cout << "file:" <<filename << " opened!" << endl;
            for(int i=0; getline(f,s); i++){
                s[s.length()-1]=' '; //方便后面调用分割函数使用,此处由于windows有\r符号需要替换成空格
                char buf[READ_BUFFERSIZE];
                const char* p_const;
                double value;
                int tmp_d;

                s.copy(buf,sizeof(buf),0);
                p_const = strtok(buf, " ");
                sscanf(p_const, "%lf", &value);

                // 加载预测值
                (*ground_truth)[0][i] = value;
                // 加载各个维度的信息
                for(int j=0;j<dimension-1;j++){
                    p_const = strtok(NULL, " ");
                    sscanf(p_const ,"%d:%lf", &tmp_d, &value);
                    (*input)[i][j]=value;
                }
                (*input)[i][dimension-1]=1; // 与b相乘
            }
        }else{
            cout << "can't open file!"<<endl;
        }

    }

    void get_line_and_dimension(ifstream& f){
        string s;
        string tmp;
        char buf[READ_BUFFERSIZE];
        char* p;
        if(getline(f,s)){
            data_num++;
            s.copy(buf, sizeof(buf), 0);
            // cout << buf << endl;
            p = strtok(buf,":");
            while((p = strtok(NULL,":"))){
                dimension++;
            }
            dimension++; // 增加一维和常数b对其
        }
        while(getline(f,s)){
            data_num++;    
        }
        cout << "with date dimension: " << dimension << endl;
        cout << "with total data point: " << data_num << endl;
    }
};
