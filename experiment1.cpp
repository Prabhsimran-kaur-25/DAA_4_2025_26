#include<iostream>
using namespace std;
using namespace std::chrono;
int counter=0;
int  complexRec(int n) {


   if (n <= 2) {
       return 1;
   }


   int p = n;
   while (p > 0) {
       vector<int> temp(n);
       for (int i = 0; i < n; i++) {
           temp[i] = i ^ p;counter++;
       }
       p >>= 1;
       
   }
 

   vector<int> small(n);
   for (int i = 0; i < n; i++) {
   
       small[i] = i * i;
      counter++;
   }


   if (n % 3 == 0) {
       reverse(small.begin(), small.end());
   } else {
       reverse(small.begin(), small.end());
   }


   complexRec(n / 2);
   complexRec(n / 2);
   complexRec(n / 2);
   return counter;
}
     
int main(){
    auto start = high_resolution_clock::now();
    int y=complexRec(4444);
    cout<<"counter"<<y<<endl;
    auto end = high_resolution_clock::now();
auto duration = duration_cast<milliseconds>(end - start);
cout<<duration.count();
}


  //Answers
// 1. Recurrance Relation - T(n)=3T(n/2)+ (n^2/log n)+n
// 2. Time complexity - O(n^2)

      
