#pragma once
#include<iostream>
using namespace std;

class Matrix{
    double** m;
    int row, col;
public:
    // 无内容构造
    Matrix(int row, int col) : row(row), col(col){
        m = new double* [row];
        for(int i=0;i<row ;i++){
            m[i] = new double [col];
        }
    }

    // 一维向量构造
    Matrix(double* vec, int len) : row(1), col(len){
        m = new double* [row];
        m[0] = new double[col];
        for(int i=0;i<col;i++){
            m[0][i]=vec[i];
        }
    }

    // 单个常数构造
    Matrix(double value) : row(1), col(1){
        m = new double* [1];
        m[0] = new double[1];
        m[0][0]=value;
    }

    // 拷贝构造
    Matrix(Matrix & k) : row(k.get_row()), col(k.get_col()){
        m = new double* [row];
        for(int i=0;i<row ;i++){
            m[i]=new double [col];
            for(int j=0;j<col;j++){
                m[i][j]=k[i][j];
            }
        }
    }

    ~Matrix(){
        delete[] m;
    }

    int get_row()const {return this->row;}
    int get_col()const {return this->col;}
    //打印矩阵内容
    void show(){
        for(int i=0;i<row;i++){
            cout<<"[";
            for(int j=0;j<col;j++){
                cout << m[i][j] << ' ';
            }
            cout << "]" <<endl;
        }
    }
    //
    void show_shape(){
        cout << "("<<row<<", "<<col<<")"<<endl;
    }
    // 从标准输入获取矩阵内容
    void get_input(){
        for(int i=0;i<row;i++)
            for(int j=0;j<col;j++)
                cin>>m[i][j];
    }
    // 求L2范数
    double norm(){
        double res=0;
        for(int i=0;i<row;i++)
            for(int j=0;j<col;j++)
                res+=(*this)[i][j]*(*this)[i][j];
        return res;
    }

    // 转置
    Matrix& T(){
        
        Matrix* new_mat = new Matrix(col, row);

        for(int i=0;i<col ;i++){
            for(int j=0;j<row;j++){
                (*new_mat)[i][j]=m[j][i];
            }
        }
        return (*new_mat);
    }

    // 以下为一些运算符重载，方便矩阵运算

    double* operator[](int row) const{
        return m[row];
    }

    void operator=(Matrix & mat){
        if(this == &mat){
            return;
        }
        delete[] m;
        row = mat.get_row();
        col = mat.get_col();
        m = new double* [row];
        for(int i=0;i<row ;i++){
            m[i]=new double [col];
            for(int j=0;j<col;j++){
                m[i][j]=mat[i][j];
            }
        }
    }

    Matrix& operator+=( const Matrix& mat){
        if(row!=mat.get_row()||col!=mat.get_col()){
            cout <<" >>> "<<row <<" "<<mat.get_row();
            cout << "can\'t do the add!" << endl;
            return *this;
        }
        for(int i=0;i<row;i++)
            for(int j=0;j<col;j++)
                (*this)[i][j]+=mat[i][j];
        return *this;
    }

    Matrix& operator-=( const Matrix& mat){
        if(row!=mat.get_row()||col!=mat.get_col()){
            cout << "can\'t do the sub!" << endl;
            return *this;
        }

        for(int i=0;i<row;i++)
            for(int j=0;j<col;j++){
                // cout <<"called!" << mat[i][j] << " "<<(*this)[i][j] <<endl;
                (*this)[i][j]-=mat[i][j];
            }
        return *this;
    }

    Matrix& operator *(double k){
        Matrix* new_mat = new Matrix(*this);
        for(int i=0;i<row;i++)
            for(int j=0;j<col;j++){
                (*new_mat)[i][j]*=k;
                // cout <<"k=" <<k<<" "<<(*new_mat)[i][j]<<endl;
            }
        return *new_mat;
    }

    Matrix& operator *(int k){
        Matrix* new_mat = new Matrix(*this);
        for(int i=0;i<row;i++)
            for(int j=0;j<col;j++)
                (*new_mat)[i][j]*=k;
        return *new_mat;
    }

};

Matrix& operator *(double k, Matrix a);

Matrix& operator *( const Matrix &a, const Matrix &b);

Matrix& operator /( const Matrix &a, const Matrix &b);

Matrix& operator+(Matrix& a, Matrix& b);

Matrix& operator-(Matrix& a, Matrix& b);

Matrix& matmul( Matrix& a, Matrix&b);
