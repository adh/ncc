#include <string>
#include <exception>

namespace NCC {
  class UnexpectedToken : public std::exception {
  private:
    char got;
  public:
    UnexpectedToken(char got) throw() : got(got) {};
    virtual ~UnexpectedToken() throw() {};
    virtual const char* what() const throw () {
      return "Unexpected token";
    }
  };
  class ExpectedToken : public std::exception {
  private:
    char expected;
    char got;
  public:
    ExpectedToken(char expected, char got) throw() : expected(expected), got(got) {};
    virtual ~ExpectedToken() throw() {};
    virtual const char* what() const throw () {
      return "Unexpected token";
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

