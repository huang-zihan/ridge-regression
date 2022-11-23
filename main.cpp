#include<iostream>
#include<fstream>
#include<string>
#include<stdio.h>
#include<string.h>
#include<math.h>
using namespace std;
#define MAXLEN 10

// 超参数
#define LAMDA 1
// #define LR 0.15  //for GD
#define LR 0.00000000001
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
                char buf[64];
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
        char buf[64];
        char* p;
        if(getline(f,s)){
            data_num++;
            s.copy(buf, sizeof(buf), 0);
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


class MethodBase{
protected:
    Matrix* w, *y, *input;
    int d;
    double lamda=LAMDA;
    double lr=LR;
    Dataloader loader;
public:
    MethodBase(int d, Matrix& input, Matrix& y):d(d){
        this->w = new Matrix(1,d);
        this->input = new Matrix(input);
        this->y = new Matrix(y);
        (*w)[0][0]=0.1;
    }

    MethodBase(const char* filename){
        loader.load_data(filename);
        this->d = loader.get_d();
        this->w = new Matrix(1,d);
        this->input = new Matrix(loader.get_input_matrix());
        this->y = new Matrix(loader.get_ground_truth());

        (*w)[0][0]=0.1;
    }

    //计算内积
    double vector_inner_product(double* a, double *b, int d){
        double result=0;
        for(int i=0;i<d;i++){
            result+=*(a+i) * *(b+i);
        }
        return result;
    }

    //计算梯度
    Matrix& get_gradient(Matrix* ground_truth, Matrix* input){
        Matrix gradient(1,d);
        for(int i=0;i<input->get_row();i++){
            gradient += Matrix((*input)[i], input->get_col()) * Matrix( ((*ground_truth)[0][i] - vector_inner_product((*w)[0],(*input)[i], this->d)) ); // 
        }
        return -2*gradient + lamda*(*w);
    }

    //输出loss
    void show_loss(){
        double loss=0;
        for(int i=0;i<input->get_row();i++){
            Matrix tmp =  Matrix( ((*y)[0][i] - vector_inner_product((*w)[0],(*input)[i], this->d)) );
            loss += tmp[0][0]*tmp[0][0];
        }
        loss += lamda*w->norm();
        loss /= input->get_row();
        cout << "loss: "<< loss <<endl;
    }
    //返回loss
    double get_loss(){
        double loss=0;
        for(int i=0;i<input->get_row();i++){
            Matrix tmp =  Matrix( ((*y)[0][i] - vector_inner_product((*w)[0],(*input)[i], this->d)) );
            loss += tmp[0][0]*tmp[0][0];
        }
        loss += lamda*w->norm();
        loss /= input->get_row();
        return loss;
    }

    void show_result(){
        cout << "result{" << endl << "parameter: ";
        w->show();
        cout <<"loss: " << get_loss() << endl;
        cout <<"}"<<endl;
    }
};


// 梯度下降
class GD : public MethodBase{
public:
    //构造函数
    GD(int d, Matrix& input, Matrix& y):MethodBase(d, input, y){}
    //构造函数,读文件输入
    GD(const char* filename):MethodBase(filename){}

    // 使用梯度下降更新一步参数
    void update_step(){
        Matrix gradient(get_gradient(y, input));
        (*w)-= 1/(double)input->get_row() * lr*gradient;
        show_loss();
    }
};


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

// 共轭梯度下降
// 在非ridge回归模型中，回归系数正是线性方程组Ax=b的解，其中A=X′X，b=X′y
// 在ridge回归模型中，回归系数正是线性方程组Ax=b的解，其中A=X′X+(1/2)*lamda，b=X′y
class CD : public MethodBase{
public:
    //构造函数
    CD(int d, Matrix& input, Matrix& y):MethodBase(d, input, y){}
    //构造函数,读文件输入
    CD(const char* filename):MethodBase(filename){}
    // 迭代更新参数
    // input×w=y
    void update(){
        Matrix A = Matrix( matmul((*input).T(), (*input)) );
        Matrix lamda_mat = Matrix(d,d);
        cout << lamda << endl;
        for(int i=0;i<d;i++)
            lamda_mat[i][i]=1.0/2.0*lamda;
        A +=lamda_mat;
        Matrix b = Matrix( matmul((*input).T(), (*y).T()) );
        Matrix x = Matrix((*w).T() );

        Matrix r = b - matmul(A, x);
        Matrix p = Matrix(r);
        Matrix a(0,0), beta(0,0);
        while(get_loss()>=1){  //abs(r[0][0])>0.1
            a = matmul(r.T(),r)/matmul(matmul(p.T(), A), p);
            x += matmul(a,p);
            Matrix next_r = r - matmul(matmul(a,A), p);
            // 将原本的r小条件改为loss小条件也可执行，由于r后期十分小
            // if r is small exit, I change the condition to if loss is small
            beta = matmul(next_r.T(),next_r)/matmul(r.T(),r);
            p = next_r + matmul(beta, p);
            r = next_r;
            cout <<"r: [ " << r[0][0] <<" ]";
            x.T().show();
            (*w) = x;
            w->T().show();
            show_loss();
        }
    }

};

// 拟牛顿法
class QuasiNewton: public MethodBase{

public:
    //构造函数
    QuasiNewton(int d, Matrix& input, Matrix& y):MethodBase(d, input, y){
        
    }
    //构造函数,读文件输入
    QuasiNewton(const char* filename):MethodBase(filename){
        // 参考参数(由GD得 -0.415388 4.4534 5.51199 4.64595 4.90556 -15.4694 -2.40434 11.9906 4.58096)
        double a[9]={-0.415388, 4.4534, 5.51199, 4.64595, 4.90556, -15.4694, -2.40434, 11.9906, 4.58096};
        delete w;
        w = new Matrix(a,9);
    }
    // 使用梯度下降更新一步参数
    void quasi_newton_iter(){
        Matrix G = Matrix(input->get_col(), input->get_col());
        Matrix delta = Matrix(w->get_row(), w->get_col());
        Matrix A = Matrix(*input);
        // Matrix x = Matrix(*w);
        Matrix g = 1/(double)input->get_row() * get_gradient(y, &A).T();
        Matrix delta_g = Matrix(g.get_row(), g.get_col());
        for(int i=0; i<input->get_col();i++)
            G[i][i]=1;
        for(int i=0;i<4;i++){ //while(g.norm()>1)
            show_loss();
            G.show();
            delta = matmul(G,g);
            // 此处的lr可以通过搜索步长得出
            cout <<"vvvv"<<endl;
            // w->show();
            // delta.T().show(); // very big
            cout <<"!!!!";
            G.show();
            (*w) += LR * delta.T(); 
            // w->show();
            cout <<"^^^^"<<endl;
            delta_g = get_gradient(y, &A).T() - g;
            g = get_gradient(y, &A).T();
            G += matmul(delta,delta.T())/matmul(delta.T(),delta_g) - matmul(matmul(G,delta_g), matmul(delta_g.T(), G))/matmul(matmul(delta_g.T(),G), delta_g);
            show_loss();
        }
    }
};


int main(){
  
    int step=ITERS;
    // GD
    // GD calculer = GD("./datasets/abalone.txt");
    // for(int i=0;i<step;i++){
    //     cout <<"iters:   " << i;
    //     calculer.update_step();
    // }
    // CD
    // CD calculer = CD("./datasets/abalone.txt");
    // calculer.update();

    // QuasiNewton
    QuasiNewton calculer = QuasiNewton("./datasets/abalone.txt");
    calculer.quasi_newton_iter();

    calculer.show_result();
}

