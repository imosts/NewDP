//
//  MyPass.h
//  LLVM
//
//  Created by 李维奉 on 2018/1/10.
//

#ifndef MyPass_h
#define MyPass_h


#endif /* MyPass_h */

#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Support/Format.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Constants.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Operator.h"
#include "llvm/IR/CallSite.h"

#include <set>
#include <fstream>

using namespace llvm;

namespace {
    Pass *createMyPass(bool flag);
}
