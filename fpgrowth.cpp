#include<iostream>
#include<vector>
#include<set>
#include<algorithm>
#include<cstdlib>
#include<cstdio>



using namespace std;

struct fptree{
    char item;
    int  count;
    struct fptree * links[100];
    int linkcount;
    struct fptree *parent; 
    struct fptree *next;  
};

struct itemset{
    char items;
    struct fptree * head;
    int count;
    itemset() {
       head=NULL;
    }
};



int count(char a,vector<set<char> > b){
    int count=0;
    for(vector<set<char> >::iterator it=b.begin();it!=b.end();it++){
	if(it->find(a)!=it->end())
            count++;  
    }
    return count;
}


void set_ptr(vector<struct itemset> & item_set,char c,struct fptree *  node){
    for(vector<struct itemset>::iterator itr=item_set.begin();itr!=item_set.end();itr++){
        if(itr->items==c){
               if(itr->head==NULL)
                   itr->head=node;
                else{
                    struct fptree *ptr;
                    for(ptr=itr->head;ptr->next!=NULL;ptr=ptr->next);
                        ptr->next=node;
                }
                return;
        }

    }
}        




bool cmp(const struct itemset& lhs, const struct itemset& rhs)
{
  return lhs.count > rhs.count;
}


/*FPTree creation*/

void fpTreeCreation(vector<char>a,vector<struct itemset> & item_set,struct fptree * &root){
  
    if(a.empty()){
        return;
    }
    
        int flag=0;
        vector<char>::iterator it=a.begin();
        for(int i=0;i<=root->linkcount;i++){  

                   struct fptree *node=root->links[i];
                   if(node->item==*it){
                      node->count+=1;
                      a.erase(it);
                      fpTreeCreation(a,item_set,node);                      
                      flag=1;
                      break;
                    }
        }
        if(flag==0){
              struct fptree *node=new(struct fptree);
              node->item=*it;
              node->count=1;
              node->linkcount=-1;  
              node->parent=root;
              root->linkcount+=1; 
              (root->links)[root->linkcount]=node;
              set_ptr(item_set,*it,node);
              a.erase(it); 
              fpTreeCreation(a,item_set,node);                      
        }
}



vector<set<char> > L;
void fpgrowth(vector<set<char> > trans,vector<struct itemset> & item_set,struct fptree * root,int numItems,int min_support,set<char> myset){

    if(item_set.size()==0)
        return;

    //creating tree for given trans 
    for(vector<set<char> >::iterator it=trans.begin();it!=trans.end();it++){

        vector<char> newset;
        for(vector<struct itemset>::iterator it1=item_set.begin();it1!=item_set.end();it1++){

               struct itemset it2=*it1;
               char ch=it2.items;
               if(it->find(ch)!=it->end())
                     newset.push_back(ch);
        }
       fpTreeCreation(newset,item_set,root); 
    }  

     


        for(vector<struct itemset>::iterator it1=item_set.end()-1;it1>=item_set.begin();it1--){
            if(it1->count>=min_support){
                myset.insert(it1->items);
                L.push_back(myset);

            vector<set<char> > t;
            vector<struct itemset> ditem_set; 

            //creating the transaction vector
            for(struct fptree *ptr1=it1->head;ptr1!=NULL;ptr1=ptr1->next){
                 set<char> temp;
                for(struct fptree *ptr2=ptr1->parent;ptr2->item!='-';ptr2=ptr2->parent){
                    temp.insert(ptr2->item);
                }
                for(int i=0;i<ptr1->count;i++)
                    t.push_back(temp);
             }

                 for(int i=0;i<numItems;i++){
                    struct itemset * iset=new(struct itemset);
                    iset->items=i+'a';
                    iset->count=count(iset->items,t);
                    iset->head=NULL;
                    if(iset->count>=min_support)
                        ditem_set.push_back(*iset);
                }


             struct fptree* croot=new(struct fptree);
             croot->item='-';
             croot->count=0;
             croot->parent=NULL;
             croot->linkcount=-1;
             fpgrowth(t,ditem_set,croot,numItems,min_support,myset);
             myset.erase(it1->items);
          }
           
          else return;

        }
}
                


            

vector< set<char> > getAllSubsets(set<char> s)
{
    vector< set<char> > subset;
    int set_size=s.size();
    unsigned int pow_set_size = 1<<set_size;
    int counter, j;
 
    for(counter = 1; counter < pow_set_size-1; counter++)
    {
      set<char> temp;
      for(j = 0; j<set_size; j++)
       {
          if(counter & (1<<j)){
             set<char>::iterator it = s.begin();
             advance(it, j);
             temp.insert(*it);
            } 
       }
      subset.push_back(temp); 
    }
    return subset;
}


set<char> difference(set<char> a, set<char> b){
     int count;
     set<char> myset; 
     for(set<char>::iterator it=a.begin();it!=a.end();it++){
           myset.insert(*it);
           for(set<char>::iterator it1=b.begin();it1!=b.end();it1++){
                if(*it==*it1){
                     myset.erase(*it);
                     break; 
                }
           }     
     }       
     return myset;
}

bool check_subset(set<char> a,set<char> b){
//checks whether a is a subset of b
    for(set<char>:: iterator itr=a.begin();itr!=a.end();itr++){
             if(b.find(*itr)==b.end())
             return 0;
     }
return 1;
}

int count(set<char>a,vector<set<char> > b){
    //counts no of occurences of set a in vector of sets b
    int count=0;

    for(vector<set<char> >:: iterator itr=b.begin();itr!=b.end();itr++)
    count+=check_subset(a,*itr);

return count;
}

void printRules(vector<set<char> > L,vector<set<char> > vec,float min_thresh){
int counter=0;
  for(vector<set<char> >:: iterator itr1=L.begin();itr1!=L.end();itr1++){
            vector <set<char> > temp=getAllSubsets(*itr1);
            for(vector<set<char> >:: iterator itr2=temp.begin();itr2!=temp.end();itr2++){
                if((float)count(*itr1,vec)/count(*itr2,vec)>=min_thresh){
                    for(set<char>:: iterator itr3=(*itr2).begin();itr3!=(*itr2).end();itr3++)
                    cout<<*itr3;
                    set<char> v;
                    v=difference(*itr1,*itr2); 
                    cout<<"->";
                    for(set<char>:: iterator itr3=v.begin();itr3!=v.end();itr3++)
                    cout<<*itr3;
                    cout<<endl;
            counter++;
                }
            }
        }
     cout<<"\nNumber of rules generated=";
     cout<<counter<<endl;
}      
           

int main(){
      int numTrans,min_support,numItems;
      float min_thresh;
      vector<set<char> > trans;
      vector<struct itemset> item_set;
      

     cin>>min_support>>numItems>>numTrans>>min_thresh;

      string items;
      for(int i=0;i<numTrans;i++){
            cin>>items;
            set<char> temp;
            for(int j=1;items.at(j-1)!='}';j+=2){
                  temp.insert(items.at(j));
            }
        trans.push_back(temp);
      }

     for(int i=0;i<numItems;i++){
        struct itemset * iset=new(struct itemset);
        iset->items=i+'a';
        iset->count=count(iset->items,trans);
        iset->head=NULL;
        if(iset->count>=min_support)
            item_set.push_back(*iset);
    }
    sort(item_set.begin(),item_set.end(),cmp);  //sorting item set on the basis of count
    

    struct fptree* root=new(struct fptree);
    root->item='-';
    root->count=0;
    root->parent=NULL;
    root->linkcount=-1;

     set<char> myset;
     fpgrowth(trans,item_set,root,numItems,min_support,myset); 

    cout<<"Frequent Itemset are:-\n"; 
      for(vector<set<char> >:: iterator itr1=L.begin();itr1!=L.end();itr1++)
        {
            for(set<char>:: iterator itr2=(*itr1).begin();itr2!=(*itr1).end();itr2++)
            cout<<*itr2;
            cout<<endl;
        }

    cout<<"Number of frequent item set=";
    cout<<L.size()<<endl;

    cout<<"Association Rules are:-\n"; 
    printRules(L,trans,min_thresh);
    return 0;  


}







