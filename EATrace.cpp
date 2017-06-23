#include <stdio.h>
#include <pin.H>

FILE *trace;


void printMemRead( void *addr )
{
	fprintf(trace, "[R] %p\n", addr);
}


void printMemWrite( void *addr)
{
	fprintf(trace, "[W] %p\n", addr);
}


void Instruction( INS ins, void *v )
{
	UINT32 memOperands = INS_MemoryOperandCount(ins);

	for (UINT32 memOp = 0; memOp < memOperands; memOp++)
	{
		if (INS_MemoryOperandIsRead(ins, memOp))
		{
			INS_InsertPredicatedCall ( 
				ins, IPOINT_BEFORE, (AFUNPTR)printMemRead,
				IARG_INST_PTR, IARG_MEMORYOP_EA, memOp, IARG_END);
		}
		if (INS_MemoryOperandIsWritten(ins, memOp))
		{
			INS_InsertPredicatedCall (
				ins, IPOINT_BEFORE, (AFUNPTR)printMemWrite,
				IARG_INST_PTR, IARG_MEMORYOP_EA, memOp, IARG_END);
		}
	}
}


void Fini( INT32 code, void *v )
{
	fprintf(trace, "--->EOF<---\n");
	fclose(trace);
}


int main( int argc, char *argv[] )
{
	PIN_Init(argc, argv);
	trace = fopen("EATrace.out", "w");
	
	INS_AddInstrumentFunction(Instruction, 0);
	PIN_AddFiniFunction(Fini, 0);
	
	// Never returns

	PIN_StartProgram();


	return 0;
}
