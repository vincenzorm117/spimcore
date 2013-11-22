#include "spimcore.h"


/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{

	switch(ALUControl){

		case 0x0: // Z = A+ B
			*ALUresult = A + B;
			break;
		case 0x1: // Z = A – B
			*ALUresult = A - B;
			break;
		case 0x2: // if A < B, Z = 1; otherwise, Z = 0
			if( !(A & (1 << 31)) && !(B & (1 << 31)) )
				*ALUresult = (A < B) ? 1 : 0;
			else if( (A & (1 << 31)) && (B & (1 << 31)) )
				*ALUresult = (A > B) ? 1 : 0;
			else if( !(A & (1 << 31)) && (B & (1 << 31)) )
				*ALUresult = 0;
			else
				*ALUresult = 1;
			break;
		case 0x3: // if A < B, Z = 1; otherwise, Z = 0 (A and B are unsigned integers)
			*ALUresult = (A < B) ? 1 : 0;
			break;
		case 0x4: // Z = A AND B
			*ALUresult = A & B;
			break;
		case 0x5: // Z = A OR B
			*ALUresult = A | B;
			break;
		case 0x6: // Shift left B by 16 bits
			*ALUresult = B << 16;
		case 0x7: // Z = NOT A
			*ALUresult = !A;

	}

}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{

	if( 0 <= PC && PC < 65536 && (PC % 4 == 0) ) //Check number
		*instruction = Mem[PC >> 2];
	else
		return 1;

	return 0;
}


/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{

	*op     = (instruction & 0xfc000000) >> 26; // 11111100 00000000 00000000 00000000
	*r1     = (instruction & 0x03e00000) >> 21; // 00000011 11100000 00000000 00000000
	*r2     = (instruction & 0x001f0000) >> 16; // 00000000 00011111 00000000 00000000
	*r3     = (instruction & 0x0000f800) >> 11; // 00000000 00000000 11111000 00000000
	*funct  =  instruction & 0x0000003f; 		// 00000000 00000000 00000000 00111111
	*offset =  instruction & 0x0000ffff; 		// 00000000 00000000 11111111 11111111
	*jsec   =  instruction & 0x03ffffff; 		// 00000011 11111111 11111111 11111111

}



/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{
	switch(op){
		case 0x0: // add,sub, and, or, slt, sltu
		// *controls = (struct_controls){ 1,0,2,0,0,07,0,0,1};
			controls->RegDst   = 1;
			controls->Jump     = 2;
			controls->Branch   = 2;  //UNSURE maybe it is 0
			controls->MemRead  = 0;
			controls->MemtoReg = 0;
			controls->ALUOp    = 07;	 //ASK
			controls->MemWrite = 0;
			controls->ALUSrc   = 0;
			controls->RegWrite = 1;
			break;
		case 0x8: // addi
			controls->RegDst   = 1;
			controls->Jump     = 2;
			controls->Branch   = 2;  //UNSURE maybe it is 0
			controls->MemRead  = 0;
			controls->MemtoReg = 0;
			controls->ALUOp    = 	 //ASK
			controls->MemWrite = 0;
			controls->ALUSrc   = 0;
			controls->RegWrite = 1;
		case 0x23: // lw
		case 0x2b: // sw
		case 0xf:  // lui
		case 0x4:  // beq
		case 0xa:  // slti
		case 0xb:  // sltiu
		case 0x2:  // j

		default:
			return 1;
	}

	return 0;
}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
	*data1 = Reg[r1];
	*data2 = Reg[r2];
}


/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{
	if(offset & (1 << 15))
		*extended_value = offset | 0xffff0000;
	else
		*extended_value = offset;
}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
	switch(ALUop){

	}
}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{
									//ASK: Might not have to check bounds
	if( ALUresult % 4 == 0 && 0 <= ALUresult && ALUresult < 65536 ){

		if(MemRead  == 1)	*memdata = Mem[ALUresult >> 2];		//ASK: do we need to adjust ALUresult
		if(MemWrite == 1)	Mem[ALUresult >> 2] = data2;

		return 0;
	}


	return 1;
}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
	if(RegWrite == 1){

		if(MemtoReg == 1){

		}
		else if(MemtoReg == 0){

		}

	}
}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{
	PC = PC + 4;


}






