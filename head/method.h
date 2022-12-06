#pragma once
#include"matrix.h"
#include"config.h"
#include"dataloader.h"
#include<math.h>

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
        cout << "finished!" << endl;
        this->d = loader.get_d();
        // 对abalone
        // double a[9]={-0.815388, 5.4534, 3.51199, 15.64595, 4.90556, -1.4694, -2.40434, 11.9906, 4.58096};
        // this->w = new Matrix(a, 9);
        // 对bodyfat
        // double a[15]={-0.00118276, 0.00129494, -0.000437999, 0.00312879, 0.00119498, 0.00231116, 0.00110519, 0.00270953, 0.00147927, 0.00122504, 0.000819305, 0.000806894, 0.000997805, 0.000666574, 4.84302e-05};
        // this->w = new Matrix(a, 15);
        this->w = new Matrix(1,this->d);

        this->input = new Matrix(loader.get_input_matrix());
        this->y = new Matrix(loader.get_ground_truth());

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

    void update(int iters){
        cout <<"begin training..."<<endl;
        cout <<" : : initial loss:  " << get_loss() << endl;
        for(int i=0;i<iters;i++){
            cout << "iters: "<< i<< "  ";
            update_step();
        }
    }

    // 使用梯度下降更新一步参数
    void update_step(){
        Matrix gradient(get_gradient(y, input));
        (*w) -= 1/(double)input->get_row() * lr * gradient;
        show_loss();
    }
};

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
        cout <<"begin training..."<<endl;
        cout <<" : : initial loss:  " << get_loss() << endl;
        for(int i=0;abs(r[0][0])>1e-8 || i<=150;i++){  //abs(r[0][0])>0.1
            a = matmul(r.T(),r)/matmul(matmul(p.T(), A), p);
            x += matmul(a,p);
            Matrix next_r = r - matmul(matmul(a,A), p);
            beta = matmul(next_r.T(),next_r)/matmul(r.T(),r);
            p = next_r + matmul(beta, p);
            r = next_r;
            (*w) = x.T();
            cout << "iters: "<< i<< "  ";
            show_loss();

        }
    }
};

// 拟牛顿法
class QuasiNewton: public MethodBase{

public:
    //构造函数
    QuasiNewton(int d, Matrix& input, Matrix& y):MethodBase(d, input, y){}
    //构造函数,读文件输入
    QuasiNewton(const char* filename):MethodBase(filename){}

    // 使用梯度下降更新一步参数 H的更新采用SR1算法
    void quasi_newton_iter(){
        Matrix H = Matrix(input->get_col(), input->get_col());
        Matrix delta = Matrix(w->get_row(), w->get_col());
        Matrix A = Matrix(*input);
        Matrix g = get_gradient(y, &A).T();
        Matrix delta_g = Matrix(g.get_row(), g.get_col());
        Matrix sk = Matrix(w->get_row(), w->get_col());
        double loss = get_loss();
        cout <<"begin training..." <<endl;
        cout <<" : : initial loss:  " << loss << endl;
        for(int i=0; i<input->get_col();i++)
            H[i][i]=1;
        for(int i=0; 1; i++){ //while(g.norm()>1)
            double prev_loss = loss;
            delta = (-1)*matmul(H,g);
            // cout << g.norm() << endl;
            if( g.norm()<0.7 && i>=150)
                break;
            // 此处的lr可以通过搜索步长得出
            double step=get_best_stepsize(delta.T());
            sk = step * delta; // 计算参数更新量
            (*w) += sk.T(); // 更新参数
            delta_g = get_gradient(y, &A).T() - g; // 计算梯度差
            g = get_gradient(y, &A).T(); // 计算新梯度
            // 更新近似矩阵
            H += matmul( (sk-matmul(H,delta_g)), (sk-matmul(H,delta_g)).T() ) / ( matmul((sk-matmul(H,delta_g)).T(), delta_g) );
            loss = get_loss();
            cout << "iters: "<< i << "  " << "with step:  "<< step/LR << "  loss: "<< loss <<endl;
            // 若loss不变化则退出
            if(abs(loss-prev_loss)<1e-10 && i>=150)
                break;
        }
    }

    // 线性搜索最佳步长
    double get_best_stepsize(Matrix& delta){
        double lr=LR;
        double best_step=lr;
        double min = get_simulate_loss(*w + lr*delta);
        // 放大搜索
        for(int i=2;i<MAX_LINSEARCH_STEP;i++){
            Matrix simu_para= (*w)+pow(2,i)*LR*delta;
            if(get_simulate_loss(simu_para)<min){
                min = get_simulate_loss(simu_para);
                best_step=pow(2,i)*lr;
            }
        }
        // 缩小搜索
        for(int i=2;i<MAX_LINSEARCH_STEP;i++){
            Matrix simu_para= (*w)+LR*delta/pow(2,i);
            if(get_simulate_loss(simu_para)<min){
                min = get_simulate_loss(simu_para);
                best_step=lr/pow(2,i);
            }
        }
        return best_step;
    }

    // 用于线性搜索时计算误差
    double get_simulate_loss(Matrix& w_simu){
        double loss=0;
        for(int i=0;i<input->get_row();i++){
            Matrix tmp =  Matrix( ((*y)[0][i] - vector_inner_product((w_simu)[0],(*input)[i], this->d)) );
            loss += tmp[0][0]*tmp[0][0];
        }
        loss += lamda*w_simu.norm();
        loss /= input->get_row();
        return loss;
    }
};
