#include<iostream>
#include<vector>
#include<set>
#include<cmath>
#include<algorithm>
#include <fstream>

using namespace std;

struct point{
    int x;
    int y;
    //重载"<"
    bool operator<(const point &b) const
    {
        return x < b.x || y < b.y;
    }
};

vector<struct point> fft(vector<struct point> s1,vector<struct point> s2,int u);
vector<int> fft(vector<int> s1, vector<int> s2, int u);
vector<struct point> subsetSum_1(vector<int> s1,int u);
vector<int> subSetSum_2(vector<int> s, int u);
int test1(vector<int> s, int u);

int main(){
    /*正确性的验证*/

    int u, n, sum, temp, r1;
    vector<int> s;
    /*通过文件输入数据集*/
    ifstream in("d://correctnessTesting_data.txt");
    n = 8;
    /*输入已知的每个数据集的正确结果*/
    for(int i=0;i<20;i++){
        sum = 0;
        for(int j=0;j<n;j++){
            in>>temp;
            s.push_back(temp);
            sum+= temp;
        }
        in>>u;
        in>>temp;

        /*输出判断结果*/
        r1 = test1(s, u);
        cout<<r1<<" ";

        /*将程序得到的结果与已知的正确结果进行比较，验证程序正确性*/
        if(r1 == temp)
            cout<<"yes"<<endl;
        else
            cout<<"no";
        s.clear();
    }
    return 0;
}

/*调用sunSetSum，判断是否存在满足条件的子集*/
int test1(vector<int> s, int u){
    vector<int> result;
    result = subSetSum_2(s, u);

    for(int i = 0; i < result.size(); i++){
        if(result[i] == u)
            return 1;
    }

    return 0;
}

vector<struct point> subsetSum_1(vector<int> s1,int u){
    /*递归计算每个子集和（上界为u）以及基数*/

    /*防止向量越界导致的内存访问错误*/
    if(s1.size() == 0){
        vector<struct point> s2;
        struct point p1;
        p1.x = 0;
        p1.y = 0;
        s2.push_back(p1);
        return s2;
    }
    /*递归边界*/
    if(s1.size() == 1){
        vector<struct point> s2;
        struct point p1;
        struct point p2;
        p1.x = 0;
        p1.y = 0;
        p2.x = s1.at(0);
        p2.y = 1;
        s2.push_back(p1);
        s2.push_back(p2);
        return s2;
    }
    /*递归*/
    vector<int>::iterator iter1,iter2,iter3;
    vector<int> s3,s4;
    iter1 = s1.begin();
    iter2 = s1.end();
    iter3 = s1.begin() + s1.size()/2;
    s3.assign(iter1,iter3);
    s4.assign(iter3,iter2);
    return fft(subsetSum_1(s3,u),subsetSum_1(s4,u),u);
}

vector<int> subSetSum_2(vector<int> s, int u){
    int n = s.size();
    int b = sqrt(n * log10(n));
    int u_b = u / b;
    vector<int> sum;
    sum.push_back(0);
    sort(s.begin(), s.end());
    for(int l = 0; l <= b - 1; l++){
        vector<int> sl;
        vector<int> sx;
        vector<int> ql;
        vector<struct point> s_ql;
        vector<int> rl;
        for(int x = 0; x <= u; x++){
            if (x % b == l % b)
                sx.push_back(x);
        }
        /*求交集*/
        set_intersection(s.begin(), s.end(), sx.begin(), sx.end(), back_inserter(sl));
        for(int i = 0; i < sl.size(); i++){
            int x = sl[i];
            ql.push_back((x - l) / b);
        }
        /*调用subSetSUm_1,求s_ql*/
        s_ql = subsetSum_1(ql, u_b);
        /*求rl*/
        for(int i = 0; i < s_ql.size(); i++){
            int ele = b * s_ql[i].x + l * s_ql[i].y;
            rl.push_back(ele);
        }
        sum = fft(sum, rl, u);
    }
    return sum;
}

vector<struct point> fft(vector<struct point> s1,vector<struct point> s2,int u){
    //设置s3
    vector<struct point> s3;
    //{(x1 + x2,y1 + y2)|(x1,y1)∈s1,(x2,y2)∈s2}∩(u×n)
    for(int i=0;i<s1.size();i++){
        for(int j=0;j<s2.size();j++){
            struct point s4;
            s4.x = s1[i].x + s2[j].x;

            if(s4.x > u)
                continue;

            s4.y = s1[i].y + s2[j].y;
            s3.push_back(s4);
        }
    }
    //s3去重
    set<struct point>s(s3.begin(), s3.end());
    s3.assign(s.begin(), s.end());
    return s3;
}

vector<int> fft(vector<int> s1, vector<int> s2, int u){
    vector<int> ret;
    for(int i = 0; i < s1.size(); i++){
        for(int j = 0; j < s2.size(); j++){
            int sum = s1[i] + s2[j];
            if(sum <= u)
                ret.push_back(sum);
        }
    }
    set<int>s(ret.begin(), ret.end());
    ret.assign(s.begin(), s.end());
    return ret;
}



