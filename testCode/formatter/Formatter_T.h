#ifndef FORMATTER_H
#define FORMATTER_H

/* Formatter defines a class to parameterize a standard formatter for sending 
 * and recieving messages, usually for the same expected interface. 
 * A type of communicatable object is parameterized by a val_fmt, which describes 
 * the processing of that type, encoding and decoding it from a stream or buffer. 
 * A formatter for a specific interface is initialized / constructed with a 
 * vector of val_fmt structs, to which that interface is limited to. These
 * should be a runtime constant, constant with the program designs, internal. 
 * A group of values to be communicated, such as the speeds for 7 motors, are
 * processed as a vector of int_T{ int i; T v} structs, where 'i' is the index
 * or id of the unit, ex motor, and 'v' is a value of any consistant type, as
 * long as it can be converted by the val_fmt, ex int, float, double.
 * Each kind of group of values to be communicated is added to a buffer with 
 * 'add', and then the buffer is emptied and returned with 'emit'.
 * Return types were decided to be floats, given the expected ARM arch.
 * Parsing is similar, given a string message: it is handled for each data_t
 * and val_fmt, returned in index / float pairs (for simplicity). 
 * The vectors can be cheaply initiated by {{0,4},{1,5},{2,-3}} etc. 
 */

#include <string>
#include <vector>

// For each type "data_t" of send/recv -able data, define simple conv:
// Convert from arb. value to a storable val with scale==resolution
// value := (val - off) / scale -> val := value * scale + off
// "data_t" in message has form of:
// char 'symbol' + char bundleID + 'bytes' length chars
// Symbol: printable non-alpha-numeric: ASCII [33,45],47,[58,64],[91,96],[123,126]
struct val_fmt {
  std::string data_t;
  char symbol;
  int bytes;
  int min_val; // Safety checks on 'val' possible values, bounds
  int max_val;
  int off; // Equal to scale*real_offset(float)
  int scale;
};

template <typename T> struct int_T {
  int i;
  T v;
};

std::vector<int_T<int>> convert(val_fmt from, val_fmt to, std::vector<int_T<int>> values);

class Formatter {
  public: 
    Formatter(std::vector<val_fmt>);
    
    // Sending / Encoding:
    // Build up a string to output
    template <class T> void 
      add(std::string data_t, std::vector<int_T<T>> ids_values);
    // Emit string. Resets to empty
    std::string emit();

    // Receiving / Parsing:
    // Parse from string all values of type data_t
    std::vector<int_T<float>> parse(std::string message, std::string data_t);

    // Basic conversions
    float getValue(int val);
    template <class T> int getVal(T value, val_fmt*);
    val_fmt* getFormat(char symbol);
    val_fmt* getFormat(std::string data_t);

  private:
    std::vector<val_fmt> formats;
    std::string msg; 
    bool newMsg;

};

#endif
