#include "token.hxx"
#include "parse.hxx"
#include "AST.hxx"
#include "symbol.hxx"

#include "llvm/ExecutionEngine/ExecutionEngine.h"

#include <iostream>
#include <fstream>

#include "commandoptions.hxx"
int main(int argc, char**argv){
  CommandOptions co;
  std::string input_file;
  bool dump_ast = false;
  bool dump_ir = false;
  bool run = false;
  std::vector<std::string> args;

  co.register_flag(dump_ast, "dump-ast", 0, "Dump AST during parsing");
  co.register_flag(dump_ir, "dump-ir", 0, "Dump compiled LLVM IR");
  co.register_flag(run, "run", 0, "Run compiled code");
  co.register_argument(input_file, "input-file", "Name of input file");
  try {
    co.process_command_line(argc,(const char**)argv);
  } catch (CommandOptions_error &ex){
    std::cerr << "Error: " << ex.what() << std::endl;
    return 1;
  }

  std::ifstream is(input_file.c_str());
  ncc::TopLevelForm *f;
  ncc::SymbolTable global_symbols(new ncc::FunctionTable());
  llvm::Module module("");
  ncc::Tokenizer t(is);
  ncc::Parser p(t);
  while (1){
    try {
      f = p.read_toplevel();
    } catch (std::exception* e){
      std::cerr << "Parse Error: " << t.get_line() << ":" << t.get_column() 
                << ": " << e->what() << std::endl;
      return 1;
    }
    
    if (f){
      if (dump_ast){
        f->print(std::cerr, 0);
      }
    } else {
      break;
    }

    try {
      f->generate(&module, &global_symbols);
    } catch (std::exception* e){
      std::cerr << "Error: " << e->what() << std::endl;
      return 1;
    }

    delete f;
  }
  
  if (dump_ir){
    module.dump();
  }

  if (run){
    llvm::ExecutionEngine* ee = llvm::ExecutionEngine::create(&module);
    llvm::Function* mf = module.getFunction("main");
    int retval;
    if (!mf){
      std::cerr << "Fatal Error: No main()!" << std::endl;
      return 0;
    }
    
    retval = ee->runFunctionAsMain(mf, args, environ);
    std::cout << "main() returned: " << retval << std::endl; 
  }
}
