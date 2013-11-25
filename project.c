#include "spimcore.h"


/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero) {

	*Zero = (A - B == 0);

	switch(ALUControl){

		case 0x0: // Z = A + B
			*ALUresult = A + B;
			break;

		case 0x1: // Z = A – B
			*ALUresult = A - B;
			break;

		case 0x2: // if A < B, Z = 1; otherwise, Z = 0
            //			*ALUresult = ((int)A < (int)B) ? 1 : 0;
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
			break;

		case 0x7: // Z = NOT A
			*ALUresult = !A;

	}

}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction) {

	if( PC % 4 == 0 ) //Check number
		*instruction = Mem[PC >> 2];
	else
		return 1;

	return 0;
}


/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec) {

	*op     = (instruction & 0xfc000000) >> 26; // 11111100 00000000 00000000 00000000
	*r1     = (instruction & 0x03e00000) >> 21; // 00000011 11100000 00000000 00000000
	*r2     = (instruction & 0x001f0000) >> 16; // 00000000 00011111 00000000 00000000
	*r3     = (instruction & 0x0000f800) >> 11; // 00000000 00000000 11111000 00000000
	*funct  =  instruction & 0x0000003f; 		// 00000000 00000000 00000000 00111111
	*offset = (instruction & 0x0000ffff) << 2; 	// 00000000 00000000 11111111 11111111
	*jsec   = (instruction & 0x03ffffff) << 2; 	// 00000011 11111111 11111111 11111111

}



/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls) {
	switch(op){

		case 0x0: // add,sub, and, or, slt, sltu
		 	*controls = (struct_controls) {1,0,0,0,0,7,0,0,1};
			break;

		case 0x8: // addi
			*controls = (struct_controls) {0,0,0,0,0,0,0,1,1};
			break;

		case 0x23: // lw
			*controls = (struct_controls) {0,0,0,1,1,0,0,1,1};
			break;

		case 0x2b: // sw
			*controls = (struct_controls) {0,0,0,0,0,0,1,1,0};
			break;

		case 0xf:  // lui
			*controls = (struct_controls) {0,0,0,0,0,5,0,1,1};
			break;

		case 0x4:  // beq
			*controls = (struct_controls) {0,0,1,0,2,1,0,1,0};
			break;

		case 0xa:  // slti
			*controls = (struct_controls) {0,0,0,0,0,2,0,1,1};
			break;

		case 0xb:  // sltiu
			*controls = (struct_controls) {0,0,0,0,0,3,0,1,1};
			break;

		case 0x2:  // j
			*controls = (struct_controls) {2,1,2,2,2,0,2,2,2};
			break;

		default:
			return 1;
	}

	return 0;
}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2) {
	*data1 = Reg[r1];
	*data2 = Reg[r2];
}


/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value) {
	if(offset & (1 << 15))
		*extended_value = offset | 0xffff0000;
	else
		*extended_value = offset;
}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero) {

	unsigned char ALUControl = ALUOp;

	switch(ALUOp){
		case 0x0:	// Addition or don't care
		case 0x1:	// Subtraction
		case 0x2:	// Set less than signed
		case 0x3:	// Set less than unsigned
		case 0x4:	// and
		case 0x5:	// or
		case 0x6:	// Shift extended_value left by 16 bits
			break;
		case 0x7:	//R-Type Instructions
			switch(funct){
				case 0x20:	// add
					ALUControl = 0x0;
                    break;
				case 0x24:	// and
					ALUControl = 0x4;
                    break;
				case 0x25:	// or
					ALUControl = 0x5;
                    break;
				case 0x2a:	// slt  (set less than signed)
					ALUControl = 0x2;
                    break;
				case 0x2b:	// sltu (set less than unsigned)
					ALUControl = 0x3;
                    break;
				default:	// Any other command will halt the program
					return 1;
			}
            break;
		default:
			return 1;

	}

	ALU( data1, (ALUSrc == 1)? extended_value : data2, ALUControl, ALUresult, Zero);

	return 0;

}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem) {

	if(MemRead  == 1){
		if( ALUresult % 4 == 0 && ALUresult < 65536 )
			*memdata = Mem[ALUresult >> 2];
		else
			return 1;
    }

	if(MemWrite == 1){
		if( ALUresult % 4 == 0 && ALUresult < 65536 )
			Mem[ALUresult >> 2] = data2;
		else
			return 1;
    }

	return 0;

}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg) {


	if(RegWrite == 1){

		if(MemtoReg == 1){
			Reg[r2] = memdata;
		}
		else if(MemtoReg == 0){

			// Reg[(MemtoReg == 0)? r3 : r2] = ALUresult;

			if(RegDst == 1)
				Reg[r3] = ALUresult;
			else
				Reg[r2] = ALUresult;

		}

	}
}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC) {
	*PC = *PC + 4;

    if(Jump == 1)
        *PC = (*PC & 0xf0000000) | jsec;


}






