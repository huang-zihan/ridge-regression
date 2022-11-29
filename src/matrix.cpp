#include"../head/matrix.h"
#include<iostream>
using namespace std;

Matrix& operator *(double k, Matrix a){
    Matrix* new_mat = new Matrix(a);
    for(int i=0;i<a.get_row();i++)
        for(int j=0;j<a.get_col();j++)
            (*new_mat)[i][j]*=k;
    return *new_mat;
}

Matrix& operator *( const Matrix &a, const Matrix &b){
        
        Matrix* new_mat;

        if( (b.get_col()!=a.get_col() || b.get_row()!=a.get_row()) && 
            !((b.get_row()==1&&b.get_col()==1||a.get_row()==1&&a.get_col()==1))
        ){
            cout << ">>>>" <<b.get_col()<<"  "<<a.get_col()<<endl;
            cout <<"can't mul with different shape!"<<endl;
            return *new_mat;
        }
        if(b.get_row()==1&&b.get_col()==1){
            new_mat = new Matrix(a.get_row(),a.get_col());
            for(int i=0;i<a.get_row();i++)
                for(int j=0;j<a.get_col();j++)
                    (*new_mat)[i][j]=b[0][0] * a[i][j];
        }else if(a.get_row()==1&&a.get_col()==1){
            new_mat = new Matrix(b.get_row(), b.get_col());
            for(int i=0;i<b.get_row();i++)
                for(int j=0;j<b.get_col();j++)
                    (*new_mat)[i][j]=b[i][j] * a[0][0];
        }else{
            new_mat = new Matrix(b.get_row(), b.get_col());
            for(int i=0;i<b.get_row();i++)
                for(int j=0;j<b.get_col();j++)
                    (*new_mat)[i][j]=b[i][j] * a[i][j];
        }

        return *new_mat;
}

Matrix& operator /( const Matrix &a, const Matrix &b){
        
        Matrix* new_mat;

        if( (b.get_col()!=a.get_col() || b.get_row()!=a.get_row()) && 
            !(b.get_row()==1&&b.get_col()==1)
        ){
            cout << ">>>>" <<b.get_col()<<"  "<<a.get_col()<<endl;
            cout <<"can't mul with different shape!"<<endl;
            return *new_mat;
        }
        
        if(b.get_row()==1&&b.get_col()==1){
            // cout <<"called!"<<a.get_row() << a.get_col() <<endl;
            new_mat = new Matrix(a.get_row(),a.get_col());
            for(int i=0;i<a.get_row();i++)
                for(int j=0;j<a.get_col();j++)
                    (*new_mat)[i][j]=a[i][j] / b[0][0];
        }else{
            new_mat = new Matrix(b.get_row(), b.get_col());
            for(int i=0;i<b.get_row();i++)
                for(int j=0;j<b.get_col();j++)
                    (*new_mat)[i][j]=a[i][j] / b[i][j];
        }
        // new_mat->show();
        return *new_mat;
}


Matrix& operator+(Matrix& a, Matrix& b){
    Matrix * new_mat = new Matrix(a);
    *new_mat += b;
    return *new_mat;
}

Matrix& operator-(Matrix& a, Matrix& b){
    Matrix * new_mat = new Matrix(a);
    *new_mat -= b;
    return *new_mat;
}

Matrix& matmul( Matrix& a, Matrix&b){
    if(a.get_col()!=b.get_row()){
        if( !((a.get_col()==1&&a.get_row()==1) || (b.get_col()==1&&b.get_row()==1)) ){
            cout << "can't do the matmul!" <<endl;
            return a;
        }
    }
    Matrix* new_mat = new Matrix(a.get_row(), b.get_col());
    double value;
    if(a.get_col()==1&&a.get_row()==1){
        (*new_mat) = a*b;
    }else if(b.get_col()==1&&b.get_row()==1){
        (*new_mat) = b*a;
    }else{
        for(int i=0;i<a.get_row();i++){
            for(int j=0;j<b.get_col();j++){
                value = 0;
                for(int k=0;k<a.get_col();k++){
                    value+=a[i][k]*b[k][j];
                }
                (*new_mat)[i][j]=value;
            }
        }
    }
    
    return *new_mat;
}
