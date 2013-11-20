#include "spimcore.h"


/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{

}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
	*instruction = Mem[PC >> 2];

	return 0;
}


/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
	
	*op     = instruction & 0xfc000000; // 11111100 00000000 00000000 00000000
	*r1     = instruction & 0x03e00000; // 00000011 11100000 00000000 00000000
	*r2     = instruction & 0x001f0000; // 00000000 00011111 00000000 00000000
	*r3     = instruction & 0x0000f800; // 00000000 00000000 11111000 00000000
	*funct  = instruction & 0x0000003f; // 00000000 00000000 00000000 00111111
	*offset = instruction & 0x0000ffff; // 00000000 00000000 11111111 11111111
	*jsec   = instruction & 0x03ffffff; // 00000011 11111111 11111111 11111111

}



/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{


}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
	*data1 = Reg[r1 >> 21];
	*data2 = Reg[r2 >> 16];
}


/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{
	*extended_value = offset << 16;	
}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{

}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{
	if(MemRead){
		*memdata = Mem[ALUresult >> 2];
	}

	if(MemWrite){
		Mem[ALUresult >> 2] = data2;
	}


	return 0;
}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{

}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{

}
