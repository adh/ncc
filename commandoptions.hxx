// A -*- C++ -*- class for parsing command line options.
//
// Copyright (C) 2001, 2002, 2003  Ole Laursen <olau@hardworking.dk>
// Copyright (c) 2004              Ales Hakl 
//                                 < dfox at users dot sourceforge dot net >
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the 
// Free Software Foundation, Inc., 59 Temple Place - Suite 330, 
// Boston, MA  02111-1307  USA.


#ifndef COMMANDOPTIONS_HEADER
#define COMMANDOPTIONS_HEADER

#include <vector>
#include <string>
#include <exception>
#include <sstream>

class CommandOptions
{
public:
  // register an option with pointer to variable, long and short argument names, a
  // description of the option and the name of the argument to the option
  template <typename T>
  void register_option(T &par, std::string long_name, char short_name,
		       std::string des, std::string arg_name)
  {
    option_table.push_back(option(new updater<T>(par), long_name, short_name, des, arg_name));
  }

  void register_flag(bool &par, std::string long_name, char short_name, std::string des)
  {
    flag_table.push_back(flag(par, long_name, short_name, des));
  }
  
  template <typename T>
  void register_argument(T &par, std::string name, std::string des)
  {
    argument_table.push_back(argument(new updater<T>(par), name, des));
  }

  // register a vector for swallowing leftover arguments
  template <typename T>
  void register_leftover_arguments(std::vector<T> &par, std::string name, std::string des)
  {
    if (leftover_arguments != 0)
      delete leftover_arguments;

    leftover_arguments = new vector_updater<T>(par);
    leftover_arguments_name = name;
    leftover_arguments_des = des;
  }
  
  void set_help(const std::string& help){
    helpString=help;
  }

  void process_command_line(unsigned int argc,
			    const char *argv[]);

  void process_command_line(const std::vector<std::string>& argv,
			    const std::string& bname);

  void process_command_line_multipass(unsigned int argc,
				      const char *argv[]);

  void process_command_line_multipass(const std::vector<std::string>& argv,
				      const std::string& bname);


  CommandOptions()
    : leftover_arguments(0)
  {}
  
  ~CommandOptions()
  {
    if (leftover_arguments != 0)
      delete leftover_arguments;
  }
  
  
private:


  //
  // member helpers
  //

  std::string requirify(std::string s) 
  {
    return '<' + s + '>';
  }
  std::string optionalify(std::string s) 
  {
    return '[' + s + ']';
  }

  std::string strip_path(std::string s);

  void print_usage(std::string executable);
  void print_help(std::string executable);

  //
  // private data structures
  //
  
  // first a few classes for updating a variable from a stream
  class updaterbase 
  {
  public:
    virtual ~updaterbase()
    {}
    
    virtual bool update(std::string s) = 0;
    virtual updaterbase *clone() = 0;
  };
  
  template <typename T>
  class updater: public updaterbase
  {
  public:
    updater(T &par)
      : var(par)
    {}
    virtual ~updater()
    {}
    
    // update the contained variable through reference - return false upon failure
    virtual bool update(std::string s)
    {
      std::istringstream is(s);
      is >> var;
      if (!is)
	return false;
      else
	return true;
    }
    
    virtual updaterbase *clone() 
    {
      return new updater<T>(var);
    }
    
  private:
    T &var;
  };

  
  // a similar class for updating the leftover vector of arguments
  class vector_updaterbase 
  {
  public:
    virtual bool add(std::string s) = 0;
    virtual ~vector_updaterbase()
    {}
  };
  
  template <typename T>
  class vector_updater: public vector_updaterbase
  {
  public:
    vector_updater(std::vector<T> &par)
      : vec(par)
    {}

    // update the contained variable through reference - return false upon failure
    virtual bool add(std::string s)
    {
      std::istringstream is(s);
      T var;
      is >> var;
      if (!is)
	return false;
      else {
	vec.push_back(var);
	return true;
      }
    }
    
    virtual ~vector_updater()
    {}
  private:
    std::vector<T> &vec;
  };

  
  // a structure for keeping a command line option
  struct option
  {
    updaterbase *par;
    std::string long_name, des, arg_name;
    char short_name;

    option(updaterbase *p, std::string ln, char sn, std::string d, std::string an)
      : par(p), long_name(ln), des(d), arg_name(an), short_name(sn)
    {}

    ~option()
    {
      delete par;
    }
    
    option(const option &o)
      : par(o.par->clone()), long_name(o.long_name), des(o.des),
	arg_name(o.arg_name), short_name(o.short_name)
    {}

    void operator =(const option &o)
    {
      updaterbase *tmp = o.par->clone();
      delete par;
      par = tmp;
      long_name = o.long_name;
      des = o.des;
      arg_name = o.arg_name;
      short_name = o.short_name;
    }
  };

  // one for keeping a flag
  struct flag
  {
    bool *par;
    std::string long_name, des;
    char short_name;
    
    flag(bool &p, std::string ln, char sn, std::string d)
      : par(&p), long_name(ln), des(d), short_name(sn)
    {}
  };
  
  // and one for keeping an argument
  struct argument
  {
    updaterbase *par;
    std::string name, des;
    
    argument(updaterbase *p, std::string n, std::string d)
      : par(p), name(n), des(d)
    {}
    
    ~argument()
    {
      delete par;
    }
    
    argument(const argument &a)
      : par(a.par->clone()), name(a.name), des(a.des)
    {}

    void operator =(const argument &a)
    {
      updaterbase *tmp = a.par->clone();
      delete par;
      par = tmp;
      name = a.name;
      des = a.des;
    }
  };
  // internal typedefs

  typedef std::vector<option>::iterator option_iterator;
  typedef std::vector<flag>::iterator flag_iterator;
  typedef std::vector<argument>::iterator argument_iterator;

  //
  // command line parsing
  //

  template <typename T>
  void process_option(T argv, 
		      unsigned int argc,
		      unsigned int& i, 
		      const std::string& s,
		      const std::string& hPrefix,
		      std::vector<flag_iterator>& processed_flags);

  void process_argument(const std::string& s,
			argument_iterator& ait);

  void check_arguments(argument_iterator& ait);

  std::vector<option> option_table;
  std::vector<flag> flag_table;
  std::vector<argument> argument_table;

  vector_updaterbase *leftover_arguments;
  std::string leftover_arguments_name;
  std::string leftover_arguments_des;
  std::string helpString;
};

template <> class CommandOptions::updater<std::string>: public CommandOptions::updaterbase
{
public:
  updater(std::string &par)
    : var(par)
  {}

  // update the contained variable through reference - return false upon failure
  virtual bool update(std::string s)
  {
    var = s;
    return true;
  }

  virtual updaterbase *clone() 
  {
    return new updater<std::string>(var);
  }
  
    
private:
  std::string &var;
};


// an exception class
class CommandOptions_error: std::exception
{
public:
  CommandOptions_error(std::string s) throw(): msg(s)
  {}
  ~CommandOptions_error() throw()
  {}
    
  virtual const char *what() const throw()
  {
    return msg.c_str();
  }
  
private:
  std::string msg;
};


#endif // COMMANDOPTIONS_HEADER
