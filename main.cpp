#include"head/config.h"
#include"head/matrix.h"
#include"head/dataloader.h"
#include"head/method.h"
using namespace std;

// 方法选择
// #define __GD__
// #define __CD__
#define __QuasiNewton__

int main(){

#ifdef __GD__
    // GD
    int step=ITERS;
    GD calculer = GD("./datasets/abalone.txt");
    for(int i=0;i<step;i++){
        cout <<"iters:   " << i;
        calculer.update_step();
    }
#endif
#ifdef __CD__
    // CD
    CD calculer = CD("./datasets/abalone.txt");
    calculer.update();
#endif
#ifdef __QuasiNewton__
    // QuasiNewton
    QuasiNewton calculer = QuasiNewton("./datasets/abalone.txt");
    calculer.quasi_newton_iter();
#endif

    calculer.show_result();
}

