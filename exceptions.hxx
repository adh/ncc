#ifndef HXX__ncc__exceptions__
#define HXX__ncc__exceptions__

#include "token.hxx"

#include <string>
#include <exception>

namespace NCC {
  class UnexpectedToken : public std::exception {
  private:
    std::string message;
  public:
    UnexpectedToken(char got) throw() {
      message = "Unexpected token: "+get_token_name(got);
    };
    virtual ~UnexpectedToken() throw() {};
    virtual const char* what() const throw () {
      return message.c_str();
    }
  };
  class ExpectedToken : public std::exception {
  private:
    std::string message;
  public:
    ExpectedToken(char expected, char got) throw() {
      message = "Unexpected token: " + get_token_name(got) 
        + " expected: " + get_token_name(expected);
    };
    virtual ~ExpectedToken() throw() {};
    virtual const char* what() const throw () {
      return message.c_str();
    }
  };
  class InvalidToken : public std::exception {
  public:
    InvalidToken() throw() {};
    virtual ~InvalidToken() throw() {};
    virtual const char* what() const throw () {
      return "Invalid token";
    }    
  };
}

#endif
