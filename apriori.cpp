#include<iostream>
#include<set>
#include<vector>
#include<algorithm>


using namespace std;

struct itemset{
    set<char> items;
    int count;
}; 


/* Pruning*/
bool pruning(set<char> a,vector<struct itemset> l){
      //vector<set<char>> myset;
	int flag=0;
      for(set<char>::iterator it=a.begin();it!=a.end();it++){
          set<char> newset;
          for(set<char>::iterator it1=a.begin();it1!=a.end();it1++){
               if(*it!=*it1){                
                  newset.insert(*it1);     
               } 
          }
	  flag=0;
	  for(vector<struct itemset>:: iterator itr1=l.begin();itr1!=l.end();itr1++){ 

               if(equal((*itr1).items.begin(),(*itr1).items.end(),newset.begin()))
		flag=1;

          }
          if(flag==0)
             return 0;  
      }
    return 1;  
}




bool check_subset(set<char> a,set<char> b){
//checks whether a is a subset of b
    for(set<char>:: iterator itr=a.begin();itr!=a.end();itr++){
             if(b.find(*itr)==b.end())
             return 0;
     }
return 1;
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





int count(set<char>a,vector<set<char> > b){
    //counts no of occurences of set a in vector of sets b
    int count=0;

    for(vector<set<char> >:: iterator itr=b.begin();itr!=b.end();itr++)
    count+=check_subset(a,*itr);

return count;
}

//joins set b vector with itself and rejects infrequent itmem set to obtain frequent itemset
vector<struct itemset> join(vector<struct itemset> a,vector<set<char> >b,int min_support){
          vector<struct itemset> l;

          for(vector<struct itemset>:: iterator itr1=a.begin();itr1!=a.end();itr1++){
                 for(vector<struct itemset>:: iterator itr2=itr1+1;itr2!=a.end();itr2++){

                       set<char>::iterator itr3=(*itr1).items.end();
                       itr3--;
                       if(equal((*itr1).items.begin(),itr3,(*itr2).items.begin())){
                                       set<char> temp((*itr2).items);
                                       temp.insert(*itr3);
                                       struct itemset i;
                                       i.items=temp;
					if(pruning(i.items,a)){
                                         i.count=count(temp,b);

                                       if(i.count<min_support)
                                                continue;
                                       l.push_back(i);
				}
                       }

                   }
           }
           return l;
}

vector<set<char> >apriori(vector<set<char> > vec, vector<struct itemset> c,int min_support){
                       vector<set<char> > l;
                       while(!c.empty()){
                                  for(vector<struct itemset>:: iterator itr1=c.begin();itr1!=c.end();itr1++)
                                                l.push_back((*itr1).items);
                                  c=join(c,vec,min_support);
                       }
                       return l;
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

//set<char> difference(set<char> a,set <char>b){

void printRules(vector<set<char> > L,vector<set<char> > vec,float min_thresh){
int counter=0;
  for(vector<set<char> >:: iterator itr1=L.begin();itr1!=L.end();itr1++)
        {
            vector <set<char> > temp=getAllSubsets(*itr1);
            for(vector<set<char> >:: iterator itr2=temp.begin();itr2!=temp.end();itr2++){
                if((float)count(*itr1,vec)/count(*itr2,vec)>=min_thresh){
                    for(set<char>:: iterator itr3=(*itr2).begin();itr3!=(*itr2).end();itr3++)
                    cout<<*itr3;
                    set<char> v;
                    //set_difference((*itr1).begin(), (*itr1).end(), (*itr2).begin(),(*itr2).end(), v.begin());
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
      int min_support,noItems,noTrans;
	float min_Thresh;

      //for storing data set of transactions
      vector<set<char> > vec;
      //for storing frequent itemset
      vector<struct itemset> c;

      cin>>min_support>>noItems>>noTrans>>min_Thresh;
      //get_input(vec,noTrans);
      string items;
      for(int i=0;i<noTrans;i++){
            cin>>items;
            set<char> temp;
            for(int j=1;items.at(j-1)!='}';j+=2){
                  temp.insert(items.at(j));
            }
       vec.push_back(temp);
      }


      for(int i=0;i<noItems;i++){
             struct itemset is;
             set<char> temp;
             temp.insert(i+'a');
             is.items=temp;
             is.count=count(temp,vec);
	     if(is.count>=min_support)
	             c.push_back(is);
       }


       cout<<"\nFrequent Item Sets are:-\n";  

       vector<set<char> > L=apriori(vec,c,min_support);
       for(vector<set<char> >:: iterator itr1=L.begin();itr1!=L.end();itr1++)
       {
            for(set<char>:: iterator itr2=(*itr1).begin();itr2!=(*itr1).end();itr2++)
                          cout<<*itr2;
            cout<<endl;
       }
        cout<<"\n\nNumber of frequent item set=";
        cout<<L.size()<<endl;
        cout<<"\nAssociation Rules are:-\n\n"; 
        printRules(L,vec,min_Thresh);
        return 0;
}
        
