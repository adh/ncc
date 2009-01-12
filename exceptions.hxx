#include <string>
#include <excpetion>

namespace NCC {
  class UnexpectedToken : public std::excpetion {
  private:
    char got;
  public:
    UnexpectedToken(char got) throw() : got(got) {};
    virtual ~UnexpectedToken() throw() {};
    virtual const char* what() const throw () {
      return "Unexpected token";
    }
  };
  class ExpectedToken : public std::excpetion {
  private:
    char expected;
    char got;
  public:
    UnexpectedToken(char expected, char got) throw() : expected(expected), got(got) {};
    virtual ~Expected() throw() {};
    virtual const char* what() const throw () {
      return "Unexpected token";
    }
  };
}

