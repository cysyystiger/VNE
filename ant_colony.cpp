#include<iostream>
#include<cstdlib>
#include<ctime>
#include<fstream>
using namespace std;

void initant(double snr[20], double vnr[], int ant[], int N);
void shortest(double sbv[20][20], int a[20][20]);
double fitness(int a[], int dis[20][20], double *vbv[],int N);
void iteration(int *init[], double t[5][20], int dis[20][20], double *vbv[], double snr[20], double vnr[], double vnv[], int N, int i, double rect[2][100]);
void update(int a[], int dis[20][20], double snv[20], double vnv[20], double *vbv[], double sbv[20][20], double snr[20], int N);
void updatebw(int x, int y, int dis[20][20], double vbvalue, double sbv[20][20]);
void rupdate(int a[], int dis[20][20], double snv[20], double vnv[20], double *vbv[], double sbv[20][20], double snr[20], int N);
void rupdatebw(int x, int y, int dis[20][20], double vbvalue, double sbv[20][20]);

int main() {
    double q=0;
        srand((unsigned)time(NULL));
    ifstream sub("SN/SN_20181207_1.txt");
    ifstream vir("VN/VN_strategy1_20181207_1.txt");
    double snv[20];
    int err;
    sub>>err;
    for(int i=0;i<20;i++) {
        sub>>snv[i];
    }
    double sbv[20][20];
    double temp[20][20];
    for(int i=0;i<20;i++) {
        for(int j=0;j<20;j++) {
            sub>>temp[i][j];
        }
    }
    for(int i=0;i<20;i++) {
        for(int j=0;j<20;j++) {
            if(j>=i) {
                if(temp[i][j]==1) {
                    sub>>sbv[i][j];
                } else {
                    sbv[i][j]=0;
                }
                sbv[j][i]=sbv[i][j];
            }
        }
    }
    double snr[20];
    for(int i=0;i<20;i++) {
        snr[i]=0;
    }
    for(int i=0;i<20;i++) {
        for(int j=0;j<20;j++) {
            snr[i]=snr[i]+snv[i]*sbv[i][j];
        }
    }
//construct substrate above
    double allsbw=0;
    for(int i=0;i<20;i++) {
        for(int j=0;j<20;j++) {
            if(j>i) {
                allsbw=allsbw+sbv[i][j];
            }
        }
    }
    double allsnv=0;
    for(int i=0;i<20;i++) {
        allsnv=allsnv+snv[i];
    }
    int dis[20][20];
    shortest(sbv, dis);//construct distance between 2 nodes
    /**for(int i=0;i<20;i++) {
        for(int j=0;j<20;j++) {
            cout<<dis[i][j]<<" ";
        }
        cout<<endl;
    }**/
    double all=0;
    double admit=0;
    for(int m=0;m<1;m++) {
        int N;
        int st;
        int en;
        double *vnv;
        double **vtemp;
        double **vbv;
        double *vnr;
        vir>>st;
        vir>>en;
        vir>>N;
        vnv=new double[N];
        vnr=new double[N];
        vtemp=new double*[N];
        vbv=new double*[N];
        for(int j=0;j<N;j++) {
            vtemp[j]=new double[N];
        }
        vbv=new double*[N];
        for(int j=0;j<N;j++) {
            vbv[j]=new double[N];
        }
        for (int j=0;j<N;j++) {
            vir>>vnv[j];
        }
        for(int i=0;i<N;i++) {
            for(int j=0;j<N;j++) {
                vir>>vtemp[i][j];
            }
        }
        for(int i=0;i<N;i++) {
            for(int j=0;j<N;j++) {
                if(j>=i) {
                    if(vtemp[i][j]==1) {
                        vir>>vbv[i][j];
                    } else {
                        vbv[i][j]=0;
                    }
                    vbv[j][i]=vbv[i][j];
                }
            }
        }
        for(int i=0;i<N;i++) {
            for(int j=0;j<N;j++) {
                vnr[i]=vnr[i]+vnv[i]*vbv[i][j];
            }
        }
        //construct virtual above
        //if(m==18){cout<<vnv[0]<<"sdf";}

        double t[5][20]; //5 ants
        for(int i=0;i<5;i++) {
            for(int j=0;j<20;j++) {
                t[i][j]=100;
            }
        }
        int **init; //initial ants 5*N
        init=new int*[5];
        for(int i=0;i<5;i++) {
            init[i]=new int[N];
        }
        for(int i=0;i<5;i++) {
            initant(snr, vnr, init[i], N);
        }
        ofstream outcome("outcome.txt");
        for(int i=4;i<5;i++) {
            for(int j=0;j<N;j++) {
                outcome<<init[i][j]<<" ";
            }
            outcome<<endl;
        }
        //abc<<endl;
        double rect[2][100];
        for(int i=0;i<100;i++) {
            iteration(init, t, dis, vbv, snr, vnr, vnv, N, i, rect);
            for(int k=4;k<5;k++) {
                for(int j=0;j<N;j++) {
                    outcome<<init[k][j]<<" ";
                }
                outcome<<endl;
            }
        }
        for(int k=0;k<100;k++) {
            outcome<<rect[0][k]<<endl;
        }
        for(int k=0;k<100;k++) {
            outcome<<rect[1][k]<<endl;
        }
        for(int i=0;i<5;i++) {
            for(int j=0;j<N;j++) {
                //cout<<init[i][j]<<" ";
            }
        }
        //cout<<endl;
        double fitvalue[5];
        for(int i=0;i<5;i++) {
            fitvalue[i]=fitness(init[i], dis, vbv, N);
        }
        double num=fitvalue[0];
        int k=0;
        for(int i=1;i<5;i++) {
            if(num>fitvalue[i]) {
                num=fitvalue[i];
                k=i;
            }
        }
        //cout<<init[k][0]<<" "<<init[k][1]<<endl;
        //cout<<fitvalue[k]<<endl;
        update(init[k],dis,snv,vnv,vbv,sbv,snr,N);
        int err=0;
        for(int i=0;i<20;i++) {
            if(snv[i]<0) {
                //cout<<"Ãz¤F";
                err=1;
            }
        }
        for(int i=0;i<20;i++) {
            for(int j=0;j<20;j++) {
                if(sbv[i][j]<0) {
                    //cout<<"Ãz¤F";
                    err=1;
                }
            }
        }
        for(int i=0;i<N;i++) {
            //all=all+vnv[i];
            for(int j=0;j<N;j++) {
                if(j>i) {
                    all=all+vbv[i][j];
                }
            }
        }
        if(!err) {
            for(int i=0;i<N;i++) {
                //admit=admit+vnv[i];
                for(int j=0;j<N;j++) {
                    if(j>i) {
                        admit=admit+vbv[i][j];
                    }
                }
            }
            double nowsbw=0;
            for(int i=0;i<20;i++) {
                for(int j=0;j<20;j++) {
                    if(j>i) {
                        nowsbw=nowsbw+sbv[i][j];
                    }
                }
            }
            //cout<<m<<" "<<st<<" "<<allsbw-nowsbw<<" ";q++;
        }
        else {rupdate(init[k],dis,snv,vnv,vbv,sbv,snr,N);}
        delete [] vnv;
        delete [] vnr;
        for (int i=0;i<N;i++) {
            delete [] vtemp[i];
        }
        delete [] vtemp;
        for (int i=0;i<N;i++) {
            delete [] vbv[i];
        }
        delete [] vbv;

    }
    double endsbw=0;
    for(int i=0;i<20;i++) {
        for(int j=0;j<20;j++) {
            if(j>i) {
                endsbw=endsbw+sbv[i][j];
            }
        }
    }
    //cout<<endl<<q/39.<<endl;
    //cout<<allsbw-endsbw;
    return 0;


}

void initant(double snr[20], double vnr[], int ant[], int N) {
    if(N==2) {
        double r1=(double)(rand())/(RAND_MAX);
        double sum=0;
        for(int i=0;i<20;i++) {
            sum=sum+snr[i];
        }
        int a;
        double a1=0;
        for(int i=0;i<20;i++) {
            if(r1>a1) {
                a=i;
                a1=a1+snr[i]/sum;
            } else {
                break;
            }
        }
        double r2=(double)(rand())/(RAND_MAX);
        sum=sum-snr[a];
        int b;
        double b1=0;
        for(int i=0;i<20;i++) {
            if(i!=a) {
                if(r2>b1) {
                    b=i;
                    b1=b1+snr[i]/sum;
                } else {
                    break;
                }
            }
        }
        if(vnr[0]>vnr[1]) {
            ant[0]=a; ant[1]=b;
        } else {
            ant[0]=b; ant[1]=a;
        }
    }
    else if(N==3) {
        double r1=(double)(rand())/(RAND_MAX);
        double sum=0;
        for(int i=0;i<20;i++) {
            sum=sum+snr[i];
        }
        int a;
        double a1=0;
        for(int i=0;i<20;i++) {
            if(r1>a1) {
                a=i;
                a1=a1+snr[i]/sum;
            } else {
                break;
            }
        }
        double r2=(double)(rand())/(RAND_MAX);
        sum=sum-snr[a];
        int b;
        double b1=0;
        for(int i=0;i<20;i++) {
            if(i!=a) {
                if(r2>b1) {
                    b=i;
                    b1=b1+snr[i]/sum;
                } else {
                    break;
                }
            }
        }
        double r3=(double)(rand())/(RAND_MAX);
        sum=sum-snr[b];
        int c;
        double c1=0;
        for(int i=0;i<20;i++) {
            if(i!=a&&i!=b) {
                if(r3>c1) {
                    c=i;
                    c1=c1+snr[i]/sum;
                } else {
                    break;
                }
            }
        }
        if(vnr[0]>=vnr[1]&&vnr[0]>=vnr[2]) {
            ant[0]=a;
            if(vnr[1]>=vnr[2]) {
                ant[1]=b; ant[2]=c;
            } else {
                ant[1]=c; ant[2]=b;
            }
        } else if(vnr[1]>=vnr[0]&&vnr[1]>=vnr[2]) {
            ant[1]=a;
            if(vnr[0]>=vnr[2]) {
                ant[0]=b; ant[2]=c;
            } else {
                ant[0]=c; ant[2]=b;
            }
        } else {
            ant[2]=a;
            if(vnr[0]>=vnr[1]) {
                ant[0]=b; ant[1]=c;
            } else {
                ant[1]=b; ant[0]=c;
            }
        }
    }
    else {
        double r1=(double)(rand())/(RAND_MAX);
        double sum=0;
        for(int i=0;i<20;i++) {
            sum=sum+snr[i];
        }
        int a;
        double a1=0;
        for(int i=0;i<20;i++) {
            if(r1>a1) {
                a=i;
                a1=a1+snr[i]/sum;
            } else {
                break;
            }
        }
        double r2=(double)(rand())/(RAND_MAX);
        sum=sum-snr[a];
        int b;
        double b1=0;
        for(int i=0;i<20;i++) {
            if(i!=a) {
                if(r2>b1) {
                    b=i;
                    b1=b1+snr[i]/sum;
                } else {
                    break;
                }
            }
        }
        double r3=(double)(rand())/(RAND_MAX);
        sum=sum-snr[b];
        int c;
        double c1=0;
        for(int i=0;i<20;i++) {
            if(i!=a&&i!=b) {
                if(r3>c1) {
                    c=i;
                    c1=c1+snr[i]/sum;
                } else {
                    break;
                }
            }
        }
        double r4=(double)(rand())/(RAND_MAX);
        sum=sum-snr[c];
        int d;
        double d1=0;
        for(int i=0;i<20;i++) {
            if(i!=a&&i!=b&&i!=c) {
                if(r4>d1) {
                    d=i;
                    d1=d1+snr[i]/sum;
                } else {
                    break;
                }
            }
        }
        double maximum=vnr[0];
        int maxindex=0;
        for(int i=0;i<4;i++) {
            if(vnr[i]>vnr[0]) {
                maximum=vnr[i];
                maxindex=i;
            }
        }
        ant[maxindex]=a;
        double secmax;
        int secindex;
        if(maxindex==0) {
            secmax=vnr[1];
            secindex=1;
        } else {
            secmax=vnr[0];
            secindex=0;
        }
        for(int i=0;i<4;i++) {
            if(i!=maxindex) {
                if(vnr[i]>secmax) {
                    secmax=vnr[i];
                    secindex=i;
                }
            }
        }
        ant[secindex]=b;
        double thimax;
        int thiindex;
        for(int i=0;i<4;i++) {
            if(i!=secindex&&i!=maxindex) {
                thimax=vnr[i];
                thiindex=i;
                break;
            }
        }
        for(int i=3;i>=0;i--) {
            if(i!=secindex&&i!=maxindex) {
                if(vnr[i]>thimax) {
                    ant[i]=c; ant[thiindex]=d;
                    break;
                } else {
                    ant[i]=d; ant[thiindex]=c;
                    break;
                }
            }
        }
    }
}

void shortest(double sbv[20][20], int a[20][20]) {
    for(int i=0;i<20;i++) {
        for(int j=0;j<20;j++) {
            a[i][j]=-1;
        }
    }
    for(int i=0;i<20;i++) {
        a[i][i]=0;
    }
    for(int i=0;i<20;i++) {
        for(int j=0;j<20;j++) {
            if(sbv[i][j]!=0) {
                a[i][j]=1;
            }
        }
        for(int m=1;m<19;m++) {
            for(int j=0;j<20;j++) {
                if(a[i][j]==m) {
                    for(int k=0;k<20;k++) {
                        if(sbv[j][k]!=0 && a[i][k]==-1) {
                            a[i][k]=m+1;
                        }
                    }
                }
            }
        }
    }
}
double fitness(int a[], int dis[20][20], double *vbv[],int N) {
    double ans;
    if(N==2) {
        ans=dis[a[0]][a[1]]*vbv[0][1];
    } else if(N==3) {
        ans=dis[a[0]][a[1]]*vbv[0][1]+
            dis[a[1]][a[2]]*vbv[1][2]+
            dis[a[2]][a[0]]*vbv[2][0];
    } else {
        ans=dis[a[0]][a[1]]*vbv[0][1]+
            dis[a[0]][a[2]]*vbv[0][2]+
            dis[a[0]][a[3]]*vbv[0][3]+
            dis[a[1]][a[2]]*vbv[1][2]+
            dis[a[1]][a[3]]*vbv[1][3]+
            dis[a[2]][a[3]]*vbv[2][3];
    }
    return ans;
}
void iteration(int *init[], double t[5][20], int dis[20][20], double *vbv[], double snr[20], double vnr[], double vnv[],int N, int i, double rect[2][100]) {
    double ep=0.9;
    double q=0.1;
    double rev=0;
    for(int i=0;i<N;i++) {
        //rev=rev+vnv[i];
        for(int j=0;j<N;j++) {
            if(j>i) {
                rev=rev+vbv[i][j];
            }
        }
    }
    if(N==2) {
        for(int i=0;i<5;i++) {
        double g[5][20];
        for(int j=0;j<5;j++) {
            for(int k=0;k<20;k++) {
                g[j][k]=snr[k];
            }
        }
        for(int j=0;j<20;j++) {
            if(init[i][0]==j || init[i][1]==j) {
                t[i][j]=(1-ep)*t[i][j]+ep*rev/fitness(init[i], dis, vbv, N);
            } else {
                t[i][j]=(1-ep)*t[i][j];
            }
        }
        double a=t[i][0]*g[i][0];
        double r1=(double)(rand())/(RAND_MAX);
        double b=0;
        double sum=0;
        for(int j=0;j<20;j++) {
            sum=sum+t[i][j]*g[i][j];
        }
        int index1=0;
        if(r1<q) {
            for(int j=1;j<20;j++) {
                if(a<t[i][j]*g[i][j]) {
                    a=t[i][j]*g[i][j];
                    index1=j;
                }
            }
        } else {
            double r=(double)(rand())/(RAND_MAX);
            for(int j=0;j<20;j++) {
                if(r>b) {
                    b=b+t[i][j]*g[i][j]/sum;
                    index1=j;
                } else {
                    break;
                }
            }
        }
        //t[i][index1]=(1-ep)*t[i][index1]+ep*100;
        for(int j=0;j<20;j++) {
            if(j!=index1) {
                g[i][j]=snr[j]/(dis[j][index1]+1);
            }
        }
        a=0;
        b=0;
        sum=sum-t[i][index1]*g[i][index1];
        int index2;
        double r2=(double)(rand())/(RAND_MAX);
        if(r2<q) {
            for(int j=0;j<20;j++) {
                if(j!=index1 && a<t[i][j]*g[i][j]) {
                    a=t[i][j]*g[i][j];
                    index2=j;
                }
            }
        } else {
            double r=(double)(rand())/(RAND_MAX);
            for(int j=0;j<20;j++) {
                if(j!=index1) {
                    if(r>b) {
                        b=b+t[i][j]*g[i][j]/sum;
                        index2=j;
                    } else {
                        break;
                    }
                }
            }
        }
        //init[i][1]=index2;
        //t[i][index2]=(1-ep)*t[i][index2]+ep*100;
        for(int j=0;j<20;j++) {
            if(j!=index1 && j!=index2) {
                g[i][j]=snr[j]/(dis[j][index1]+dis[j][index2]+1);
            }
        }
        if(vnr[0]>vnr[1]) {
            init[i][0]=index1; init[i][1]=index2;
        } else {
            init[i][0]=index2; init[i][1]=index1;
        }
        //global iteration
        for(int j=0;j<20;j++) {
            if(init[i][0]==j || init[i][1]==j) {
                t[i][j]=(1-ep)*t[i][j]+ep*rev/fitness(init[i], dis, vbv, N);
            } else {
                t[i][j]=(1-ep)*t[i][j];
            }
        }
    }
    rect[0][i]=t[0][19];rect[1][i]=t[0][6];
    }
    else if(N==3) {
        for(int i=0;i<5;i++) {
        double g[5][20];
        for(int j=0;j<5;j++) {
            for(int k=0;k<20;k++) {
                g[j][k]=snr[k];
            }
        }
        for(int j=0;j<20;j++) {
            if(init[i][0]==j || init[i][1]==j || init[i][2]==j) {
                t[i][j]=(1-ep)*t[i][j]+ep*t[i][j]*rev/fitness(init[i], dis, vbv, N);
            } else {
                t[i][j]=(1-ep)*t[i][j];
            }
        }
        double a=t[i][0]*g[i][0];
        double r1=(double)(rand())/(RAND_MAX);
        double b=0;
        double sum=0;
        for(int j=0;j<20;j++) {
            sum=sum+t[i][j]*g[i][j];
        }
        int index1=0;
        if(r1<q) {
            for(int j=1;j<20;j++) {
                if(a<t[i][j]*g[i][j]) {
                    a=t[i][j]*g[i][j];
                    index1=j;
                }
            }
        } else {
            double r=(double)(rand())/(RAND_MAX);
            for(int j=0;j<20;j++) {
                if(r>b) {
                    b=b+t[i][j]*g[i][j]/sum;
                    index1=j;
                } else {
                    break;
                }
            }
        }
        t[i][index1]=(1-ep)*t[i][index1]+ep*100;
        for(int j=0;j<20;j++) {
            if(j!=index1) {
                g[i][j]=snr[j]/(dis[j][index1]+1);
            }
        }
        a=0;
        b=0;
        sum=sum-t[i][index1]*g[i][index1];
        int index2;
        double r2=(double)(rand())/(RAND_MAX);
        if(r2<q) {
            for(int j=0;j<20;j++) {
                if(j!=index1 && a<t[i][j]*g[i][j]) {
                    a=t[i][j]*g[i][j];
                    index2=j;
                }
            }
        } else {
            double r=(double)(rand())/(RAND_MAX);
            for(int j=0;j<20;j++) {
                if(j!=index1) {
                    if(r>b) {
                        b=b+t[i][j]*g[i][j]/sum;
                        index2=j;
                    } else {
                        break;
                    }
                }
            }
        }
        //init[i][1]=index2;
        t[i][index2]=(1-ep)*t[i][index2]+ep*100;
        for(int j=0;j<20;j++) {
            if(j!=index1 && j!=index2) {
                g[i][j]=snr[j]/(dis[j][index1]+dis[j][index2]+1);
            }
        }
        a=0;
        b=0;
        sum=sum-t[i][index2]*g[i][index2];
        double r3=(double)(rand())/(RAND_MAX);
        int index3;
        if(r3<q) {
            for(int j=0;j<20;j++) {
                if(j!=index1 && j!=index2 && a<t[i][j]*g[i][j]) {
                    a=t[i][j]*g[i][j];
                    index3=j;
                }
            }
        } else {
            double r=(double)(rand())/(RAND_MAX);
            for(int j=0;j<20;j++) {
                if(j!=index1 && j!=index2) {
                    if(r>b) {
                        b=b+t[i][j]*g[i][j]/sum;
                        index3=j;
                    } else {
                        break;
                    }
                }
            }
        }
        //init[i][0]=index3;
        if(vnr[0]>=vnr[1]&&vnr[0]>=vnr[2]) {
            init[i][0]=index1;
            if(vnr[1]>=vnr[2]) {
                init[i][1]=index2; init[i][2]=index3;
            } else {
                init[i][1]=index3; init[i][2]=index2;
            }
        } else if(vnr[1]>=vnr[0]&&vnr[1]>=vnr[2]) {
            init[i][1]=index1;
            if(vnr[0]>=vnr[2]) {
                init[i][0]=index2; init[i][2]=index3;
            } else {
                init[i][0]=index3; init[i][2]=index2;
            }
        } else {
            init[i][2]=index1;
            if(vnr[0]>=vnr[1]) {
                init[i][0]=index2; init[i][1]=index3;
            } else {
                init[i][1]=index2; init[i][0]=index3;
            }
        }
        //global iteration
        for(int j=0;j<20;j++) {
            if(init[i][0]==j || init[i][1]==j || init[i][2]==j) {
                t[i][j]=(1-ep)*t[i][j]+ep*t[i][j]*rev/fitness(init[i], dis, vbv, N);
            } else {
                t[i][j]=(1-ep)*t[i][j];
            }
        }
    }
    }
    else {
        for(int i=0;i<5;i++) {
        double g[5][20];
        for(int j=0;j<5;j++) {
            for(int k=0;k<20;k++) {
                g[j][k]=snr[k];
            }
        }
        for(int j=0;j<20;j++) {
            if(init[i][0]==j || init[i][1]==j || init[i][2]==j || init[i][3]) {
                t[i][j]=(1-ep)*t[i][j]+ep*t[i][j]*rev/fitness(init[i], dis, vbv, N);
            } else {
                t[i][j]=(1-ep)*t[i][j];
            }
        }
        double a=t[i][0]*g[i][0];
        double r1=(double)(rand())/(RAND_MAX);
        double b=0;
        double sum=0;
        for(int j=0;j<20;j++) {
            sum=sum+t[i][j]*g[i][j];
        }
        int index1=0;
        if(r1<q) {
            for(int j=1;j<20;j++) {
                if(a<t[i][j]*g[i][j]) {
                    a=t[i][j]*g[i][j];
                    index1=j;
                }
            }
        } else {
            double r=(double)(rand())/(RAND_MAX);
            for(int j=0;j<20;j++) {
                if(r>b) {
                    b=b+t[i][j]*g[i][j]/sum;
                    index1=j;
                } else {
                    break;
                }
            }
        }
        t[i][index1]=(1-ep)*t[i][index1]+ep*100;
        for(int j=0;j<20;j++) {
            if(j!=index1) {
                g[i][j]=snr[j]/(dis[j][index1]+1);
            }
        }
        a=0;
        b=0;
        sum=sum-t[i][index1]*g[i][index1];
        int index2;
        double r2=(double)(rand())/(RAND_MAX);
        if(r2<q) {
            for(int j=0;j<20;j++) {
                if(j!=index1 && a<t[i][j]*g[i][j]) {
                    a=t[i][j]*g[i][j];
                    index2=j;
                }
            }
        } else {
            double r=(double)(rand())/(RAND_MAX);
            for(int j=0;j<20;j++) {
                if(j!=index1) {
                    if(r>b) {
                        b=b+t[i][j]*g[i][j]/sum;
                        index2=j;
                    } else {
                        break;
                    }
                }
            }
        }
        //init[i][1]=index2;
        t[i][index2]=(1-ep)*t[i][index2]+ep*100;
        for(int j=0;j<20;j++) {
            if(j!=index1 && j!=index2) {
                g[i][j]=snr[j]/(dis[j][index1]+dis[j][index2]+1);
            }
        }
        a=0;
        b=0;
        sum=sum-t[i][index2]*g[i][index2];
        double r3=(double)(rand())/(RAND_MAX);
        int index3;
        if(r3<q) {
            for(int j=0;j<20;j++) {
                if(j!=index1 && j!=index2 && a<t[i][j]*g[i][j]) {
                    a=t[i][j]*g[i][j];
                    index3=j;
                }
            }
        } else {
            double r=(double)(rand())/(RAND_MAX);
            for(int j=0;j<20;j++) {
                if(j!=index1 && j!=index2) {
                    if(r>b) {
                        b=b+t[i][j]*g[i][j]/sum;
                        index3=j;
                    } else {
                        break;
                    }
                }
            }
        }
        t[i][index3]=(1-ep)*t[i][index3]+ep*100;
        for(int j=0;j<20;j++) {
            if(j!=index1 && j!=index2 && j!=index3) {
                g[i][j]=snr[j]/(dis[j][index1]+dis[j][index2]+dis[j][index3]+1);
            }
        }
        a=0;
        b=0;
        sum=sum-t[i][index3]*g[i][index3];
        double r4=(double)(rand())/(RAND_MAX);
        int index4;
        if(r4<q) {
            for(int j=0;j<20;j++) {
                if(j!=index1 && j!=index2 && j!=index3 && a<t[i][j]*g[i][j]) {
                    a=t[i][j]*g[i][j];
                    index4=j;
                }
            }
        } else {
            double r=(double)(rand())/(RAND_MAX);
            for(int j=0;j<20;j++) {
                if(j!=index1 && j!=index2 && j!=index3) {
                    if(r>b) {
                        b=b+t[i][j]*g[i][j]/sum;
                        index4=j;
                    } else {
                        break;
                    }
                }
            }
        }
        t[i][index4]=(1-ep)*t[i][index4]+ep*100;
        double maximum=vnr[0];
        int maxindex=0;
        for(int k=0;k<4;k++) {
            if(vnr[k]>vnr[0]) {
                maximum=vnr[k];
                maxindex=k;
            }
        }
        init[i][maxindex]=index1;
        double secmax;
        int secindex;
        if(maxindex==0) {
            secmax=vnr[1];
            secindex=1;
        } else {
            secmax=vnr[0];
            secindex=0;
        }
        for(int k=0;k<4;k++) {
            if(k!=maxindex) {
                if(vnr[k]>secmax) {
                    secmax=vnr[k];
                    secindex=k;
                }
            }
        }
        //cout<<"qwe";
        init[i][secindex]=index2;
        double thimax;
        int thiindex;
        for(int k=0;k<4;k++) {
            if(k!=secindex&&k!=maxindex) {
                thimax=vnr[k];
                thiindex=k;
            }
        }//cout<<"qwe";
        for(int k=0;k<4;k++) {
            if(k!=secindex&&k!=maxindex) {
                if(vnr[k]>thimax) {
                    thimax=vnr[k];
                    thiindex=k;
                }
            }
        }//cout<<"qwe";
        init[i][thiindex]=index3;
        for(int k=0;k<4;k++) {
            if(k!=secindex&&k!=maxindex&&k!=thiindex) {
                init[i][k]=index4;
            }
        }//cout<<"qwe";
        for(int j=0;j<20;j++) {
            if(init[i][0]==j || init[i][1]==j || init[i][2]==j || init[i][3]==j || init[i][3]) {
                t[i][j]=(1-ep)*t[i][j]+ep*t[i][j]*rev/fitness(init[i], dis, vbv, N);
            } else {
                t[i][j]=(1-ep)*t[i][j];
            }
        }
    }


}}

void update(int a[], int dis[20][20], double snv[20], double vnv[], double *vbv[], double sbv[20][20], double snr[20], int N) {
    if(N==2) {
        snv[a[0]]=snv[a[0]]-vnv[0];
        snv[a[1]]=snv[a[1]]-vnv[1];
        updatebw(a[0],a[1],dis,vbv[0][1],sbv);
    }
    else if(N==3) {
        snv[a[0]]=snv[a[0]]-vnv[0];
        snv[a[1]]=snv[a[1]]-vnv[1];
        snv[a[2]]=snv[a[2]]-vnv[2];
        if(vbv[0][1]>0) {
            updatebw(a[0],a[1],dis,vbv[0][1],sbv);
        }
        if(vbv[0][2]>0) {
            updatebw(a[0],a[2],dis,vbv[0][2],sbv);
        }
        if(vbv[1][2]>0) {
            updatebw(a[1],a[2],dis,vbv[1][2],sbv);
        }
    } else {
        snv[a[0]]=snv[a[0]]-vnv[0];
        snv[a[1]]=snv[a[1]]-vnv[1];
        snv[a[2]]=snv[a[2]]-vnv[2];
        snv[a[3]]=snv[a[3]]-vnv[3];
        if(vbv[0][1]>0) {
            updatebw(a[0],a[1],dis,vbv[0][1],sbv);
        }
        if(vbv[0][2]>0) {
            updatebw(a[0],a[2],dis,vbv[0][2],sbv);
        }
        if(vbv[0][3]>0) {
            updatebw(a[0],a[3],dis,vbv[0][3],sbv);
        }
        if(vbv[1][2]>0) {
            updatebw(a[1],a[2],dis,vbv[1][2],sbv);
        }
        if(vbv[1][3]>0) {
            updatebw(a[1],a[3],dis,vbv[1][3],sbv);
        }
        if(vbv[2][3]>0) {
            updatebw(a[2],a[3],dis,vbv[2][3],sbv);
        }
    }
    for(int i=0;i<20;i++) {
        snr[i]=0;
    }
    for(int i=0;i<20;i++) {
        for(int j=0;j<20;j++) {
            snr[i]=snr[i]+snv[i]*sbv[i][j];
        }
    }
}
void rupdate(int a[], int dis[20][20], double snv[20], double vnv[], double *vbv[], double sbv[20][20], double snr[20], int N) {
    if(N==2) {
        snv[a[0]]=snv[a[0]]+vnv[0];
        snv[a[1]]=snv[a[1]]+vnv[1];
        rupdatebw(a[0],a[1],dis,vbv[0][1],sbv);
    }
    else if(N==3) {
        snv[a[0]]=snv[a[0]]+vnv[0];
        snv[a[1]]=snv[a[1]]+vnv[1];
        snv[a[2]]=snv[a[2]]+vnv[2];
        if(vbv[0][1]>0) {
            rupdatebw(a[0],a[1],dis,vbv[0][1],sbv);
        }
        if(vbv[0][2]>0) {
            rupdatebw(a[0],a[2],dis,vbv[0][2],sbv);
        }
        if(vbv[1][2]>0) {
            rupdatebw(a[1],a[2],dis,vbv[1][2],sbv);
        }
    } else {
        snv[a[0]]=snv[a[0]]+vnv[0];
        snv[a[1]]=snv[a[1]]+vnv[1];
        snv[a[2]]=snv[a[2]]+vnv[2];
        snv[a[3]]=snv[a[3]]+vnv[3];
        if(vbv[0][1]>0) {
            rupdatebw(a[0],a[1],dis,vbv[0][1],sbv);
        }
        if(vbv[0][2]>0) {
            rupdatebw(a[0],a[2],dis,vbv[0][2],sbv);
        }
        if(vbv[0][3]>0) {
            rupdatebw(a[0],a[3],dis,vbv[0][3],sbv);
        }
        if(vbv[1][2]>0) {
            rupdatebw(a[1],a[2],dis,vbv[1][2],sbv);
        }
        if(vbv[1][3]>0) {
            rupdatebw(a[1],a[3],dis,vbv[1][3],sbv);
        }
        if(vbv[2][3]>0) {
            rupdatebw(a[2],a[3],dis,vbv[2][3],sbv);
        }
    }
    for(int i=0;i<20;i++) {
        snr[i]=0;
    }
    for(int i=0;i<20;i++) {
        for(int j=0;j<20;j++) {
            snr[i]=snr[i]+snv[i]*sbv[i][j];
        }
    }
}
void rupdatebw(int x, int y, int dis[20][20], double vbvalue, double sbv[20][20]) {
    if(dis[x][y]==1) {
            sbv[x][y]=sbv[x][y]+vbvalue;
            sbv[y][x]=sbv[x][y];
        }
    else if(dis[x][y]==2) {
        for(int i1=0;i1<20;i1++) {
            if(sbv[x][i1]<0 || sbv[y][i1]<0) {
                sbv[x][i1]=sbv[x][i1]+vbvalue;
                sbv[i1][x]=sbv[x][i1];
                sbv[y][i1]=sbv[y][i1]+vbvalue;
                sbv[i1][y]=sbv[y][i1];
            }
        }
    }
    else if(dis[x][y]==3) {
        for(int i1=0;i1<20;i1++) {
            for(int i2=0;i2<20;i2++) {
                if(sbv[x][i1]<0 || sbv[i2][i1]<0 || sbv[i2][y]<0) {
                    sbv[x][i1]=sbv[x][i1]+vbvalue;
                    sbv[i1][x]=sbv[x][i1];
                    sbv[y][i2]=sbv[y][i2]+vbvalue;
                    sbv[i2][y]=sbv[y][i2];
                    sbv[i1][i2]=sbv[i1][i2]+vbvalue;
                    sbv[i2][i1]=sbv[i1][i2];
                }
            }
        }
    }
    else if(dis[x][y]==4) {
        for(int i1=0;i1<20;i1++) {
            for(int i2=0;i2<20;i2++) {
                for(int i3=0;i3<20;i3++) {
                    if(sbv[x][i1]<0 || sbv[i2][i1]<0 || sbv[i2][i3]<0 ||sbv[i3][y]<0) {
                        sbv[x][i1]=sbv[x][i1]+vbvalue;
                        sbv[i1][x]=sbv[x][i1];
                        sbv[y][i3]=sbv[y][i3]+vbvalue;
                        sbv[i3][y]=sbv[y][i3];
                        sbv[i1][i2]=sbv[i1][i2]+vbvalue;
                        sbv[i2][i1]=sbv[i1][i2];
                        sbv[i3][i2]=sbv[i3][i2]+vbvalue;
                        sbv[i2][i3]=sbv[i3][i2];
                    }
                }
            }
        }
    }
    else if(dis[x][y]==5) {
        for(int i1=0;i1<20;i1++) {
            for(int i2=0;i2<20;i2++) {
                for(int i3=0;i3<20;i3++) {
                    for(int i4=0;i4<20;i4++) {
                        if(sbv[x][i1]<0 || sbv[i2][i1]<0 || sbv[i2][i3]<0 || sbv[i4][i3]<0 || sbv[i4][y]<0) {
                            sbv[x][i1]=sbv[x][i1]+vbvalue;
                            sbv[i1][x]=sbv[x][i1];
                            sbv[y][i4]=sbv[y][i4]+vbvalue;
                            sbv[i4][y]=sbv[y][i4];
                            sbv[i1][i2]=sbv[i1][i2]+vbvalue;
                            sbv[i2][i1]=sbv[i1][i2];
                            sbv[i3][i2]=sbv[i3][i2]+vbvalue;
                            sbv[i2][i3]=sbv[i3][i2];
                            sbv[i3][i4]=sbv[i3][i4]+vbvalue;
                            sbv[i4][i3]=sbv[i3][i4];
                        }
                    }
                }
            }
        }
    }
    else if(dis[x][y]==6) {
        for(int i1=0;i1<20;i1++) {
            for(int i2=0;i2<20;i2++) {
                for(int i3=0;i3<20;i3++) {
                    for(int i4=0;i4<20;i4++) {
                        for(int i5=0;i5<20;i5++) {
                            if(sbv[x][i1]<0 || sbv[i2][i1]<0 || sbv[i2][i3]<0 || sbv[i4][i3]<0 || sbv[i4][i5]<0 || sbv[y][i5]<0) {
                                sbv[x][i1]=sbv[x][i1]+vbvalue;
                                sbv[i1][x]=sbv[x][i1];
                                sbv[y][i5]=sbv[y][i5]+vbvalue;
                                sbv[i5][y]=sbv[y][i5];
                                sbv[i1][i2]=sbv[i1][i2]+vbvalue;
                                sbv[i2][i1]=sbv[i1][i2];
                                sbv[i3][i2]=sbv[i3][i2]+vbvalue;
                                sbv[i2][i3]=sbv[i3][i2];
                                sbv[i3][i4]=sbv[i3][i4]+vbvalue;
                                sbv[i4][i3]=sbv[i3][i4];
                                sbv[i5][i4]=sbv[i5][i4]+vbvalue;
                                sbv[i4][i5]=sbv[i5][i4];
                            }
                        }
                    }
                }
            }
        }
    }
}
void updatebw(int x, int y, int dis[20][20], double vbvalue, double sbv[20][20]) {
    if(dis[x][y]==1) {
            sbv[x][y]=sbv[x][y]-vbvalue;
            sbv[y][x]=sbv[x][y];
        }
    else if(dis[x][y]==2) {
        for(int i1=0;i1<20;i1++) {
            if(sbv[x][i1]>0 && sbv[y][i1]>0) {
                sbv[x][i1]=sbv[x][i1]-vbvalue;
                sbv[i1][x]=sbv[x][i1];
                sbv[y][i1]=sbv[y][i1]-vbvalue;
                sbv[i1][y]=sbv[y][i1];
            }
        }
    }
    else if(dis[x][y]==3) {
        for(int i1=0;i1<20;i1++) {
            for(int i2=0;i2<20;i2++) {
                if(sbv[x][i1]>0 && sbv[i2][i1]>0 && sbv[i2][y]>0) {
                    sbv[x][i1]=sbv[x][i1]-vbvalue;
                    sbv[i1][x]=sbv[x][i1];
                    sbv[y][i2]=sbv[y][i2]-vbvalue;
                    sbv[i2][y]=sbv[y][i2];
                    sbv[i1][i2]=sbv[i1][i2]-vbvalue;
                    sbv[i2][i1]=sbv[i1][i2];
                }
            }
        }
    }
    else if(dis[x][y]==4) {
        for(int i1=0;i1<20;i1++) {
            for(int i2=0;i2<20;i2++) {
                for(int i3=0;i3<20;i3++) {
                    if(sbv[x][i1]>0 && sbv[i2][i1]>0 && sbv[i2][i3]>0 && sbv[i3][y]>0) {
                        sbv[x][i1]=sbv[x][i1]-vbvalue;
                        sbv[i1][x]=sbv[x][i1];
                        sbv[y][i3]=sbv[y][i3]-vbvalue;
                        sbv[i3][y]=sbv[y][i3];
                        sbv[i1][i2]=sbv[i1][i2]-vbvalue;
                        sbv[i2][i1]=sbv[i1][i2];
                        sbv[i3][i2]=sbv[i3][i2]-vbvalue;
                        sbv[i2][i3]=sbv[i3][i2];
                    }
                }
            }
        }
    }
    else if(dis[x][y]==5) {
        for(int i1=0;i1<20;i1++) {
            for(int i2=0;i2<20;i2++) {
                for(int i3=0;i3<20;i3++) {
                    for(int i4=0;i4<20;i4++) {
                        if(sbv[x][i1]>0 && sbv[i2][i1]>0 && sbv[i2][i3]>0 && sbv[i4][i3]>0 && sbv[i4][y]>0) {
                            sbv[x][i1]=sbv[x][i1]-vbvalue;
                            sbv[i1][x]=sbv[x][i1];
                            sbv[y][i4]=sbv[y][i4]-vbvalue;
                            sbv[i4][y]=sbv[y][i4];
                            sbv[i1][i2]=sbv[i1][i2]-vbvalue;
                            sbv[i2][i1]=sbv[i1][i2];
                            sbv[i3][i2]=sbv[i3][i2]-vbvalue;
                            sbv[i2][i3]=sbv[i3][i2];
                            sbv[i3][i4]=sbv[i3][i4]-vbvalue;
                            sbv[i4][i3]=sbv[i3][i4];
                        }
                    }
                }
            }
        }
    }
    else if(dis[x][y]==6) {
        for(int i1=0;i1<20;i1++) {
            for(int i2=0;i2<20;i2++) {
                for(int i3=0;i3<20;i3++) {
                    for(int i4=0;i4<20;i4++) {
                        for(int i5=0;i5<20;i5++) {
                            if(sbv[x][i1]>0 && sbv[i2][i1]>0 && sbv[i2][i3]>0 && sbv[i4][i3]>0 && sbv[i4][i5]>0 && sbv[y][i5]>0) {
                                sbv[x][i1]=sbv[x][i1]-vbvalue;
                                sbv[i1][x]=sbv[x][i1];
                                sbv[y][i5]=sbv[y][i5]-vbvalue;
                                sbv[i5][y]=sbv[y][i5];
                                sbv[i1][i2]=sbv[i1][i2]-vbvalue;
                                sbv[i2][i1]=sbv[i1][i2];
                                sbv[i3][i2]=sbv[i3][i2]-vbvalue;
                                sbv[i2][i3]=sbv[i3][i2];
                                sbv[i3][i4]=sbv[i3][i4]-vbvalue;
                                sbv[i4][i3]=sbv[i3][i4];
                                sbv[i5][i4]=sbv[i5][i4]-vbvalue;
                                sbv[i4][i5]=sbv[i5][i4];
                            }
                        }
                    }
                }
            }
        }
    }
}













