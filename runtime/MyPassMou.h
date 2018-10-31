//
//  MyPassMou.h
//  LLVM
//
//  Created by 李维奉 on 2018/1/10.
//

#ifndef MyPassMou_h
#define MyPassMou_h


#endif /* MyPassMou_h */

#include "llvm/ADT/SetVector.h"
#include "llvm/Pass.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Support/Format.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Constants.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include <set>
#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

namespace llvm {
    Pass *createMyPassMou(bool flag);
}
