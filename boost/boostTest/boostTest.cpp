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
  person *p = new person(31); 
  oa << p; 
  std::cout <<std::hex << p <<" -> save p "<<p->age() <<std::endl; 
  delete p; 
} 
 
void load() 
{ 
  boost::archive::text_iarchive ia(ss); 

  person *p_save = new person(32);
  std::cout <<std::hex << p_save << " -> load p_save "<<p_save->age()<< std::endl; // 0x5628a2059720 -> load p_save 20
  delete p_save;
  
  std::cout <<std::hex << p_save << " -> load p_save "<<p_save->age()<< std::endl; // 0x5628a2059720 -> load p_save 20

  person *p = new person(31);
  std::cout << std::hex << p << " -> load p before read " <<std::endl; 
  ia >> p; 
  std::cout << std::hex << p <<" -> load p after read "<< std::endl; 
  std::cout << p->age() << std::endl;

  delete p; 
} 
 
int main() 
{ 
  std::cout<<"start save"<<std::endl;
  save(); 
  

  std::cout<<std::endl;
  std::cout<<"start main "<<std::endl;
  person *p_main = new person(31);
  std::cout <<std::hex << p_main << " -> load p_main "<< p_main->age()<<std::endl; 
  delete p_main;
  std::cout <<std::hex << p_main << " -> load p_main "<<p_main->age() <<std::endl; 
  std::cout<<std::endl;
    
  std::cout<<std::endl;
  person *p_main_2 = new person(31);
  std::cout <<std::hex << p_main_2 << " -> load p_main_2 "<< p_main_2->age()<<std::endl; 
  delete p_main_2;
  std::cout <<std::hex << p_main_2 << " -> load p_main_2 "<<p_main_2->age() <<std::endl; 
  std::cout<<std::endl;
  
  std::cout<<"start load"<<std::endl;
  load();
  
  person *p_load = new person(31);
  std::cout <<std::hex << p_load << " -> load p_load "<< std::endl; 
  delete p_load;
} 

/*
start save
 call person(int) construct 31
0x5576b87d2b30 -> save p 1f 	        // 1.

start main 
 call person(int) construct 1f
0x5576b87d2d40 -> load p_main 1f	// 2. 对比 1、2、3 发现，boost 函数中 delete 后的内存没有立刻归还到系统，仍由 boost 管理（1-2），下次分配变量（3）时，优先在该地址分配
0x5576b87d2d40 -> load p_main b87d2cf0


 call person(int) construct 1f
0x5576b87d2d40 -> load p_main_2 1f	
0x5576b87d2d40 -> load p_main_2 b87d2cf0 // main 中打印 delete 后的变量，为随机数，说明 main 中 delete 后的内存，被随机初始化

start load
 call person(int) construct 20
0x5576b87d2b30 -> load p_save 20	// 3. 
0x5576b87d2b30 -> load p_save 0	// load 中打印 delete 后的内存变量，为0，说明在 boost 函数内部调用的 delete 内存被初始化为0,(与main中调用delete不同)
 call person(int) construct 1f
0x5576b87d2b30 -> load p before read 
call person() construct
0x5576b87d30b0 -> load p after read  // 4.
1f
 call person(int) construct 1f
0x5576b87d2ca0 -> load p_load 

*/

/*
结论：
1. boost 在 load 对象指针时，会新分配内存并调用无参构造函数进行初始化，而不是使用手动 new 出来的内存，并且指向 new 内存的指针会丢失，导致内存泄漏
2. 通过 p_main_2 7c2d9cf0 和 在boost 调用 delete P 时，会清理内春为0，而手动调用的没有

*/
