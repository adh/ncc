#ifndef HXX__ncc__variable__
#define HXX__ncc__variable__

#include "types.hxx"
#include "exceptions.hxx"

#include "llvm/DerivedTypes.h"
#include "llvm/Module.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/Support/LLVMBuilder.h"

#include <string>
#include <map>

namespace ncc {

  class Function {
  protected:
    ValueType ret_type;
    std::vector<ValueType> arg_types;
    llvm::Function* address;
  public:
    Function(){}
    Function(ValueType ret_type,
             const std::vector<ValueType>& arg_types,
             llvm::Function* address) : ret_type(ret_type),
                                        arg_types(arg_types),
                                        address(address) {}
    ValueType get_ret_type(){
      return ret_type;
    }
    ValueType get_arg_type(int n){
      return arg_types[n];
    }
    int get_arg_count(){
      return arg_types.size();
    }
    llvm::Function* get_address(){
      return address;
    }
  };

  class FunctionTable {
  protected:
    std::map<std::string, Function> table;
  public:
    void put_function(const std::string& name,
                      const Function& func){
      table[name] = func;
    }
    Function& get_function(const std::string& name){
      std::map<std::string, Function>::iterator f = table.find(name);
      if (f == table.end()){
        throw new UnknownSymbol(name);
      }
      return f->second;
    }
  };

  class Variable {
  protected:
    llvm::Value* address;
    ValueType type;
  public:
    Variable(llvm::Value* address, ValueType type):
      address(address), type(type) {}
    Variable(){}
    llvm::Value* get_address(){
      return address;
    }
    ValueType get_type(){
      return type;
    }
  };

  class SymbolTable {
  protected:
    std::map<std::string, Variable> symbols;
    SymbolTable* parent;
    ValueType lex_rtype;
    llvm::Value* lex_retval;
    llvm::BasicBlock* lex_epilog;
    FunctionTable* ft;
  public:
    SymbolTable(FunctionTable* ft): parent(NULL), 
                                    lex_rtype(TYPE_VOID),
                                    lex_retval(NULL),
                                    lex_epilog(NULL),
                                    ft(ft){}
    SymbolTable(SymbolTable* parent): parent(parent){
      lex_rtype = parent->get_lex_rtype();
      lex_retval = parent->get_lex_retval();
      lex_epilog = parent->get_lex_epilog();
      ft = parent->ft;
    }
    SymbolTable(SymbolTable* parent,
                ValueType lex_rtype,
                llvm::Value* lex_retval,
                llvm::BasicBlock* lex_epilog): parent(parent), 
                                               lex_rtype(lex_rtype),
                                               lex_retval(lex_retval),
                                               lex_epilog(lex_epilog){
      ft = parent->ft;
    }
    Variable& get_symbol(const std::string name){
      SymbolTable *i = this;
      std::map<std::string, Variable>::iterator j;
      while (i){
        j = i->symbols.find(name);
        if (j != i->symbols.end()){
          return j->second;
        }
        i = i->parent;
      }
      throw UnknownSymbol(name);
    }
    Function& get_function(const std::string name){
      return ft->get_function(name);
    }
    void put_symbol(const std::string name, const Variable& var){
      symbols[name] = var;
    }
    void put_function(const std::string name, const Function& func){
      ft->put_function(name, func);
    }
    ValueType get_lex_rtype(){
      return lex_rtype;
    }
    llvm::Value* get_lex_retval(){
      return lex_retval;
    }
    llvm::BasicBlock* get_lex_epilog(){
      return lex_epilog;
    }
  };
}

#endif
