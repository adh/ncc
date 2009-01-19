#ifndef HXX__ncc__exceptions__
#define HXX__ncc__exceptions__

#include "types.hxx"

#include <string>
#include <exception>

namespace ncc {
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
  class FeatureNotImplemented : public std::exception {
  private:
    std::string message;
  public:
    FeatureNotImplemented(const std::string& message) throw(): 
      message("Feature not implemented: " + message) {}
    virtual ~FeatureNotImplemented() throw() {};
    virtual const char* what() const throw () {
      return message.c_str();
    }
  };
  class UnknownSymbol : public std::exception {
  private:
    std::string message;
  public:
    UnknownSymbol(const std::string& message) throw(): 
      message("Unknown symbol: " + message) {}
    virtual ~UnknownSymbol() throw() {};
    virtual const char* what() const throw () {
      return message.c_str();
    }
  };
  class IncompatibleTypes : public std::exception {
  public:
    IncompatibleTypes() throw() {};
    virtual ~IncompatibleTypes() throw() {};
    virtual const char* what() const throw () {
      return "Incompatible types";
    }    
  };
  class TooManyArguments : public std::exception {
  private:
    std::string message;
  public:
    TooManyArguments(const std::string& func) throw(): 
      message("Too many arguments passed to " + func) {}
    virtual ~TooManyArguments() throw() {};
    virtual const char* what() const throw () {
      return message.c_str();
    }
  };
}

#endif
