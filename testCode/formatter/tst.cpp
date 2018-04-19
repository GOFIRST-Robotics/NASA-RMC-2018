#include <iostream>
#include <vector>

class TstAbst{
  public:
    virtual void print();
}; 

template <class T> class Tst : public TstAbst {
  public:
    Tst(T t){ val = t; };
    void print(){ std::cout << val << std::endl; };
  protected:
    T val;
};

int main(){
  Tst<int> tInt = Tst<int>(1);
  Tst<float> tFloat = Tst<float>(0.2f);
  Tst<double> tDub = Tst<double>(2.3);

  std::vector<TstAbst> vec;
  vec.push_back(tInt);
  vec.push_back(tFloat);
  vec.push_back(tDub);

  for(auto & v : vec){
    v.print();
  }
  return 0;
}

/*
class FormatAbstract {
  public:
    virtual std::string toString(
    std::string encode(std::vector<FormatAbstract>);

  protected:
    val_fmt format;
};

template <class T> class Formatter : public FormatAbstract {
  public:
    Formatter(val_fmt format);

    // Basic conversions
    int getVal(T value){ return (int)(value * format.scale + format.off); }
    T  getValue(int val){ return ((T)(val - format.off)) / format.scale; }

    // String generation
    std::string ;;
*/
