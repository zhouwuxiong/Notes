#include <boost/archive/text_oarchive.hpp> 
#include <boost/archive/text_iarchive.hpp> 
#include <iostream> 
#include <sstream> 
 
std::stringstream ss; 
 
class person 
{ 
public: 
  person() 
  { 
  std::cout<<"call person() construct"<<std::endl;
  } 
 
  person(int age) 
    : age_(age) 
  {
    std::cout<<" call person(int) construct "<<age<<std::endl; 
  } 
 
  int age() const 
  { 
    return age_; 
  } 
 
private: 
  friend class boost::serialization::access; 
 
  template <typename Archive> 
  void serialize(Archive &ar, const unsigned int version) 
  { 
    ar & age_; 
  } 
 
  int age_; 
}; 
 
void save() 
{ 
  boost::archive::text_oarchive oa(ss); 
  person p(31); 
  oa << p; 
  std::cout <<std::hex << &p <<" -> save p "<<p.age() <<std::endl; 
} 
 
void load() 
{ 
  boost::archive::text_iarchive ia(ss); 
   // =========== test notice ================
  person p;
  std::cout << std::hex << &p << " -> load p before read " <<std::endl; 
  ia >> p; 
  std::cout << std::hex << &p <<" -> load p after read "<< std::endl; 
  std::cout << p.age() << std::endl;
  // =========== test notice ================
} 
 
int main() 
{ 
  save(); 
  load();
} 

