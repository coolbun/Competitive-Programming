#include<bits/stdc++.h>
using namespace std;
#define pb push_back
#define mp make_pair
#define mod 1000000007
#define cin(n) scanf("%d",&n)

struct Node
{
    int cnt,priority,info;
    long long sum;
    bool rev;
    Node *l,*r,*par;
    Node()
    {
        cnt=1;
        priority=info=0;
        sum=0;
        rev=false;
        l=r=par=0;
    }
};

typedef Node* tree;

const int MAX_BUF = 2e6;
int szbuf;
Node buf[MAX_BUF];

struct Treap
{
    tree root;
public:
    Treap(){
        root=0;
        szbuf=0;
    }
    int random(){
        return ((rand()&((1<<15)-1))<<15)^rand();
    }
    tree allocate(int data){
        tree cur=&buf[szbuf++];
        cur->priority=random();
        cur->info=cur->sum=data;
        return cur;
    }
    int get_cnt(tree t){
        return !t?0:t->cnt;
    }
    long long get_sum(tree t){
        return !t?0:t->sum;
    }
    void rev(tree t){
        if(!t)
            return;
        t->rev^=1;
    }
    void push(tree t){
        if(!t)
            return;
        if(t->rev){
            swap(t->l,t->r);
            rev(t->l),rev(t->r);
            t->rev^=1;
        }
    }
    void update(tree t){
        if(t){
            t->cnt=get_cnt(t->l)+get_cnt(t->r)+1;
            t->sum=get_sum(t->l)+get_sum(t->r)+t->info;
            t->par=0;
        }
    }
    void split(tree t,int key,tree &l,tree &r){
        push(t);
        if(!t){
            l=r=0;
            return;
        }
        int ckey=get_cnt(t->l);
        if(ckey<key){
            split(t->r,key-ckey-1,t->r,r);
            l=t;
        }
        else{
            split(t->l,key,l,t->l);
            r=t;
        }
        update(l);update(r);
    }
    tree merge(tree l,tree r){
        push(l);push(r);
        if(!l||!r)
            return !l?r:l;
        tree t;
        if(l->priority>r->priority){
            l->r=merge(l->r,r);
            t=l;
        }
        else{
            r->l=merge(l,r->l);
            t=r;
        }
        update(t);
        return t;
    }
    long long get_sum(int l,int r){
        tree p1,p2,p3;
        split(root,r+1,p2,p3);
        split(p2,l,p1,p2);
        long long ans=get_sum(p2);
        root=merge(p1,merge(p2,p3));
        return ans;
    }
    void append(int number){
        tree t=allocate(number);
        root=merge(root,t);
    }
};

void tswap(Treap &t1,int l1,int r1,Treap &t2,int l2,int r2){
    tree p1,p2,p3,q1,q2,q3;
    
    t1.split(t1.root,r1+1,p2,p3);
    t1.split(p2,l1,p1,p2);
    
    t2.split(t2.root,r2+1,q2,q3);
    t2.split(q2,l2,q1,q2);
    
    t1.root=t1.merge(p1,t1.merge(q2,p3));
    t2.root=t2.merge(q1,t2.merge(p2,q3));
    
}

int main()
{
    int m,n,i,j,k,l;
    
    Treap t[2];
    cin(n);
    cin(m);
    
    for(i=0;i<n;i++)
    {
        cin(j);
        t[i&1].append(j);
    }
    
    while(m--)
    {
        int l,r,type;
        cin(type);
        cin(l);cin(r);
        l--;r--;
        int ql[2], qr[2];

        for(int par = 0; par <= 1; par++){
            if(l == r && l % 2 != par){
                ql[par] = 0, qr[par] = -1;
            }else{
                ql[par] = (l % 2 == par ? l / 2 : (l + 1) / 2);
                qr[par] = (r % 2 == par ? r / 2 : (r - 1) / 2);
            }
        }

        if(type == 1)
            tswap(t[0], ql[0], qr[0], t[1], ql[1], qr[1]);
        else{
            long long ans = 0;
            for(int par = 0; par <= 1; par++){
                if(ql[par] <= qr[par])
                    ans += t[par].get_sum(ql[par], qr[par]);
            }
            printf("%lld\n", ans); 
        }
    }
    
    return 0;

}