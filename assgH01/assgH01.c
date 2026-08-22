#include<stdio.h>
#include<math.h>
#include<stdbool.h>
#include <time.h>

long long H(int);
long long L(int);

struct HL{
    long long h,l;
};

struct Mat{
    long long mat[2][2];
};

struct Matx{
    long long matx[2][2];
};

//individual H
long long H(int n){
    if(n == 0)return 1;

    return 2*H(n-1) + L(n-1);
}

//individual L
long long L(int n){
    if(n == 0)return 0;

    return H(n-1) + L(n-1);
}

//combined H&L
struct HL funx(int n){

    if(n == 0){
        struct HL hl;
        hl.h = 1;
        hl.l = 0;
        return hl;
    }

    struct HL val = funx(n-1);

    struct HL hl;
    hl.h = 2*val.h + val.l;
    hl.l = val.h + val.l;

    return hl;
}

//combined H&L using closed function
struct HL funy(int n){
    if(n == 0){
        struct HL hl;
        hl.h = 1;
        hl.l = 0;
        return hl;
    }

    struct HL val;

    val.h = ((5 + sqrt(5))*pow((3 - sqrt(5)),n+1) / (10 * pow(2,n+1))) + ((5 - sqrt(5))*pow((3 + sqrt(5)),n+1) / (10 * pow(2,n+1)));
    val.l = ((-5 - 3*sqrt(5))*pow((3 - sqrt(5)),n+1) / (10 * pow(2,n+1))) + ((-5 + 3*sqrt(5))*pow((3 + sqrt(5)),n+1) / (10 * pow(2,n+1)));

    return val;
}

//H&L through matrix multiplication
struct Mat funz(int n){
    if(n == 0){
        struct Mat mat = {0};
        mat.mat[0][0] = 1;
        mat.mat[1][0] = 0;
        return mat;
    }

    struct Mat mat = funz(n-1);
    
    struct Mat ans = {0};
    long long matx[2][2] = {{2,1},{1,1}};
    for(int i=0; i<2; i++){
        for(int j=0; j<2; j++){
            for(int k=0; k<2; k++){
                ans.mat[i][j] += matx[i][k] * mat.mat[k][j];
            }
        }
    }
    return ans;
}

//H&L through log(n) approach
struct Matx funw(int n){

    if(n == 1){
        struct Matx matx;
        matx.matx[0][0] = 2;
        matx.matx[1][0] = 1;
        matx.matx[0][1] = 1;
        matx.matx[1][1] = 1;

        return matx;
    }
    struct Matx matf = funw(floor(n/2.0));

    bool flag = false;
    if(n % 2 != 0) flag = true;

    struct Matx ans = {0}, ansx = {0}, og;

    og.matx[0][0] = 2;
    og.matx[1][0] = 1;
    og.matx[0][1] = 1;
    og.matx[1][1] = 1;

    if(!flag){
        for(int i=0; i<2; i++){
            for(int j=0; j<2; j++){
                for(int k=0; k<2; k++){
                    ans.matx[i][j] += matf.matx[i][k] * matf.matx[k][j];
                }
            }
        }
    }

    if(flag){
        for(int i=0; i<2; i++){
            for(int j=0; j<2; j++){
                for(int k=0; k<2; k++){
                    ansx.matx[i][j] += matf.matx[i][k] * matf.matx[k][j];
                }
            }
        }
        for(int i=0; i<2; i++){
            for(int j=0; j<2; j++){
                for(int k=0; k<2; k++){
                    ans.matx[i][j] += ansx.matx[i][k] * og.matx[k][j];
                }
            }
        }
    }
    return ans;
}

struct HL finalFun(int n){
    if(n == 0){
        struct HL ans;
        ans.h = 1;
        ans.l = 0;
        return ans;
    }

    struct Matx matrix = funw(n);
    long long ansMat[2][1] = {0};
    long long mat[2][1] = {{1},{0}};
    for(int i=0; i<2; i++){
        for(int j=0; j<1; j++){
            for(int k=0; k<2; k++){
                ansMat[i][j] += matrix.matx[i][k] * mat[k][j];
            }
        }
    } 
    struct HL ans;
    ans.h = ansMat[0][0];
    ans.l = ansMat[1][0];
    return ans;
}

int main(){
    struct timespec start, end;
    while(true){
        int n = 1;;
        int x;
        printf("enter number : "); scanf("%d",&x);
        if(n == 1){
            
            timespec_get(&start, TIME_UTC);
            long long ans = H(x);
            printf("%lld  \n",ans);
            timespec_get (&end, TIME_UTC);
            double time_spent = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/ 1000000000.0;
            printf("Elapsed wall-clock time: %f seconds\n", time_spent);
        }
        n++;
        if(n == 2){
            timespec_get(&start, TIME_UTC);
            long long ans = L(x);
            printf("%lld  \n",ans);
            timespec_get (&end, TIME_UTC);
            double time_spent = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/ 1000000000.0;
            printf("Elapsed wall-clock time: %f seconds\n", time_spent);
        }
        n++;
        if(n == 3){
            timespec_get(&start, TIME_UTC);
            struct HL ans = funx(x);
            printf("%lld  %lld\n",ans.h,ans.l);
            timespec_get (&end, TIME_UTC);
            double time_spent = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/ 1000000000.0;
            printf("Elapsed wall-clock time: %f seconds\n", time_spent);
        }
        n++;
        if(n == 4){
            timespec_get(&start, TIME_UTC);
            struct HL ans = funy(x);
            printf("%lld  %lld\n",ans.h,ans.l);
            timespec_get (&end, TIME_UTC);
            double time_spent = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/ 1000000000.0;
            printf("Elapsed wall-clock time: %f seconds\n", time_spent);
        }
        n++;
        if(n == 5){
            timespec_get(&start, TIME_UTC);
            struct Mat ans = funz(x);
            printf("%lld  %lld\n",ans.mat[0][0],ans.mat[1][0]);
            timespec_get (&end, TIME_UTC);
            double time_spent = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/ 1000000000.0;
            printf("Elapsed wall-clock time: %f seconds\n", time_spent);
        }
        n++;
        if(n == 6){
            timespec_get(&start, TIME_UTC);
            struct HL ans = finalFun(x);
            printf("%lld  %lld\n",ans.h,ans.l);
            timespec_get (&end, TIME_UTC);
            double time_spent = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/ 1000000000.0;
            printf("Elapsed wall-clock time: %f seconds\n", time_spent);
        }
        break;
    }
    return 0;
}
