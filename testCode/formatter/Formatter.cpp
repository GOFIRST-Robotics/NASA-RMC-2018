#include "Formatter.h"
#include <vector>
#include <string>

// Gen Funcs
inline bool isValidSymbol(char c){
  return (c >= 33 && c <= 45) || c == 47 || (c >= 58 && c <= 64) || (c >= 91 && c <= 96) || (c >= 123 && c <= 126);
}

inline char letter(int i){
  if(i < 0)
    return 21;
  else if(i < 26)
    return (i + 65);
  else
    return (i + 97);
}

inline int number(char c){
  return ((int)c < 91) ? (int)c - 65 : (int)c - 97;
}

val_fmt* Formatter::getFormat(char symbol){
  if(isValidSymbol(symbol))
    for(val_fmt fmt : formats){
      if(fmt.symbol == symbol)
        return &fmt;
    }
  return NULL;
}

val_fmt* Formatter::getFormat(std::string data_t){
  for(val_fmt fmt : formats){
    if(fmt.data_t == data_t)
      return &fmt;
  }
  return NULL;
}

Formatter::Formatter(std::vector<val_fmt> fmts){
  formats = fmts;
  newMsg = true;
  msg.reserve(300);
  msg = "";
}

inline template <class T> int Formatter::getVal(T value, val_fmt* fmt){
  int out = (int)(value * fmt->scale + fmt->off);
  if(out > fmt->max_val)
    return fmt->max_val;
  else if(out < fmt->min_val)
    return fmt->min_val;
  else
    return out;
}

float Formatter::getValue(int val){
  return ((float)(val - off)) / scale;
}

template <class T> void Formatter::add(std::string data_t, std::vector<int_T<T>> ids_values){
  if(newMsg){
    newMsg = false;
    msg = "";
  }
  val_fmt* fmt = getFormat(data_t);
  assert(fmt); // Check that code runs; this should 
  for(int_T<T> idv : ids_values){
    msg += fmt->symbol;
    msg += letter(idv.i);
    std::string tmp = std::to_string(getVal(idv.v,fmt));
    msg.append(fmt->bytes - tmp.size(),'0');
    msg += tmp;
  }
}

inline std::string Formatter::emit(){
  newMsg = true;
  return msg;
}

std::vector<int_T<float>> Formatter::parse(std::string message, std::string data_t){
  std::vector<int_T<float>> out;
  val_fmt* fmt = getFormat(data_t);
  assert(fmt);
  for(std::string::size_type i = 0; i < message.size(); ++i){
    if(message[i] == fmt->symbol){
      out.push_back({
          number(message[i+1]),
          ((float)(std::stoi(message.substr(i+2,i+2 + fmt->bytes)) - fmt->off))
            / fmt-> scale});
    }
  }
  return out;
}

