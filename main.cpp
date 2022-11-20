#include<iostream>
using namespace std;
#define MAXLEN 10

// 超参数
#define LAMDA 0.1
#define LR 0.0009
#define ITERS 2000

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
            for(int j=0;j<col;j++){
                cout << m[i][j] << ' ';
            }
            cout << endl;
        }
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
            cout << "can\'t do the add!" << endl;
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

Matrix& operator+(Matrix& a, Matrix& b){
    Matrix * new_mat = new Matrix(a);
    *new_mat += b;
    return *new_mat;
}

class GD{
    Matrix* w, *y, *input;
    int d;
    double lamda=LAMDA;
    double lr=LR;
public:
    //构造函数
    // GD(int d):d(d){
    //     w = new Matrix(1,d);
    // }
    //构造函数
    GD(int d, Matrix& input, Matrix& y):d(d){
        this->w = new Matrix(1,d);
        this->input = new Matrix(input);
        this->y = new Matrix(y);
        (*w)[0][0]=0.1;
    }
    //计算内积
    double vector_inner_product(double* a, double *b, int d){
        double result=0;
        for(int i=0;i<d;i++){
            result+=*(a+i) * *(b+i);
        }
        // cout << *a << " " << *b << endl;
        return result;
    }
    //计算梯度
    Matrix& get_gradient(Matrix* ground_truth, Matrix* input){
        Matrix gradient(1,d);
        for(int i=0;i<input->get_row();i++){
            // gradient.show();
            // cout <<">>>" <<input->get_col()<<"  ";
            // Matrix((*input)[i], input->get_col()).show();
            // cout << ">>>" << ((*ground_truth)[0][i] - vector_inner_product((*w)[0],(*input)[i], this->d));
            // Matrix( ((*ground_truth)[0][i] - vector_inner_product((*w)[0],(*input)[i], this->d)) ).show();
            // Matrix test = Matrix((*input)[i], input->get_col()) * Matrix( ((*ground_truth)[0][i] - vector_inner_product((*w)[0],(*input)[i], this->d)) );
            // test.show();
            // cout <<"!!!"<<gradient[0][0]<<endl;
            // cout <<"!!!"<<gradient[0][1]<<endl;
            // gradient+=test;
            gradient += Matrix((*input)[i], input->get_col()) * Matrix( ((*ground_truth)[0][i] - vector_inner_product((*w)[0],(*input)[i], this->d)) ); //
            
        }
        return -2*gradient + lamda*(*w);
    }
    // 使用梯度下降更新一步参数
    void update_step(){
        Matrix gradient(get_gradient(y, input));
        cout << "grad:";
        gradient.show();
        (*w)-=lr*gradient;
        cout << "new w:";
        w->show();
        show_loss();
    }

    //输出loss
    void show_loss(){
        double loss=0;
        for(int i=0;i<input->get_row();i++){
            Matrix tmp =  Matrix( ((*y)[0][i] - vector_inner_product((*w)[0],(*input)[i], this->d)) );
            loss += tmp[0][0]*tmp[0][0];
        }
        loss += lamda*w->norm();
        cout << "loss: "<<loss <<endl;
    }

};

int main(){

    Matrix x(6,2);
    Matrix y(1,6);
    x.get_input();
    y.get_input();
  
    int step=ITERS;

    GD calculer = GD(2,x,y);
    for(int i=0;i<step;i++)
        calculer.update_step();
}

