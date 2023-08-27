/*
 * Categorize instructions.
 *
 * @author: Adrian Herrera
 */

#include <fstream>
#include <iostream>
#include <map>

#include "pin.H"

static KNOB<std::string> KnobOutputFile(KNOB_MODE_WRITEONCE, "pintool", "o",
                                        "inst-categories.csv",
                                        "Specify output CSV path");
static KNOB<bool> KnobAppend(KNOB_MODE_WRITEONCE, "pintool", "a",
                             "0", "Append to output CSV");

static std::map<INT32, UINT64> InstCategories;
static std::ofstream OutFile;

static INT32 Usage() {
  std::cerr << "Categorizes instructions" << std::endl;
  std::cerr << KNOB_BASE::StringKnobSummary() << std::endl;

  return -1;
}

static VOID Categorize(INT32 category) { InstCategories[category]++; }

static VOID Instruction(INS ins, VOID *v) {
  INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)Categorize, IARG_UINT32,
                 INS_Category(ins), IARG_END);
}

static VOID Fini(INT32 code, VOID *v) {
  if (!KnobAppend) {
    OutFile << "category,count" << std::endl;
  }
  for (const auto &CategoryCount : InstCategories) {
    OutFile << CATEGORY_StringShort(CategoryCount.first) << ","
            << CategoryCount.second << std::endl;
  }
  OutFile.close();
}

int main(int argc, char *argv[]) {
  if (PIN_Init(argc, argv)) {
    return Usage();
  }

  OutFile.open(KnobOutputFile.Value().c_str(),
               KnobAppend ? std::ios::app : std::ios::out);

  INS_AddInstrumentFunction(Instruction, nullptr);
  PIN_AddFiniFunction(Fini, nullptr);
  PIN_StartProgram();

  return 0;
}
