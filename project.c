//
//  project.c
//  Project
//
//  Created by Kevin Anderson on 11/14/13.
//  Copyright (c) 2013 Kevin Anderson. All rights reserved.
//
#include "spimcore.h"

//This is my project branch
/* ALU */

void ALU(unsigned A, unsigned B, char ALUControl, unsigned *ALUresult, char *Zero) {

	Zero = 0;

	switch (ALUControl){

	case '000':

		ALUresult = A + B;

		if(ALUresult == 0)
			*Zero = 1;

		break;

	case '001':

		ALUresult = A - B;

		if( ALUresult == 0 )
			*Zero = 1;

		break;

	case '010':

		if ( A < B)
			ALUresult = 1;
		else
			ALUresult = 0;

		if ( ALUresult == 0 )

			*Zero = 1;

		break;

	case '011':

		if ( A > B ){

			ALUresult = 0;

		}

		else

			ALUresult == 1;

		if ( ALUresult == 0 )

			*Zero = 1;

		break;

	case '100':

		ALUresult = A & B; // Bitwise AND

		if ( ALUresult == 0 )

			*Zero = 1;

		break;

	case '101':

		ALUresult = A | B; // Bitwise OR

		if ( ALUresult == 0 )

			*Zero = 1;

		break;

	case '110':

		ALUresult = B << 16;

		if(ALUresult == 0)

			*Zero = 1;

		break;

	case '111':

		ALUControl = !A;    //logical not

		if(ALUresult == 0)

			*Zero = 1;

		break;

	}


}

/* instruction fetch */

int instruction_fetch(unsigned PC, unsigned *Mem, unsigned *instruction) {

	/*From FAQ - PC has to be shifted right

         by 2 bits in order to be used with this updates the program counter (PC) Mem[] */

	unsigned mindex = PC >> 2;

	if(PC % 4 == 0){  // halt if not word aligned

		*instruction = Mem[mindex]; // get next instruction

		return 0;

	}

	else

		return 1;

}

/* instruction partition */

void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1, unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec) {

        unsigned for_r = 0x0000001F;
        unsigned for_f = 0x0000003F;
        unsigned for_o = 0x0000FFFF;
        unsigned for_j = 0x03FFFFFF;

        *op = instruction >> 26;            // instruction [31-26]

        *r1 = instruction >> 21 & for_r;	// instruction [25-21]

        *r2 = instruction >> 16 & for_r;    // instruction [20-16]

        *r3 = instruction >> 11 & for_r;	// instruction [15-11]

        *funct = instruction & for_f;	    // instruction [5-0]

        *offset = instruction & for_o;	    // instruction [15-0]

        *jsec =	instruction & for_j;        // instruction [25-0]


}

/* instruction decode */

int instruction_decode(unsigned op, struct_controls *controls) {

	/*The value set for the ALUOp of 0-7 represents the binary value of the R-Type Instruction; refer to final project document page 4 */
	switch (op){

	case 0: // R-Type instructions (and, add, slt, sltu)
		controls -> RegDst = 1;
		controls -> Jump = 0;
		controls -> Branch = 0;
		controls -> MemRead = 0;
		controls -> MemtoReg = 0;
		controls -> ALUSrc = 1;
		controls -> MemWrite = 0;
		controls -> ALUOp = 7;
		controls -> RegWrite = 1;
		break;

	case 1: // addi and li
		controls -> RegDst = 0;
		controls -> Jump = 0;
		controls -> Branch = 0;
		controls -> MemRead = 0;
		controls -> MemtoReg = 0;
		controls -> ALUSrc = 1;
		controls -> MemWrite = 0;
		controls -> ALUOp = 0; /*ALU will do addition or “don’t care” */
		controls -> RegWrite = 1;
		break;

	case 2: // slti (Signed)
		controls -> RegDst = 0;
		controls -> Jump = 0;
		controls -> Branch = 0;
		controls -> MemRead = 0;
		controls -> MemtoReg = 0;
		controls -> ALUSrc = 1;
		controls -> MemWrite = 0;
		controls -> ALUOp = 2;  /*ALU will do “set less than” operation*/
		controls -> RegWrite = 1;
		break;

	case 3: // sltiu (Unsigned)
		controls -> RegDst = 0;
		controls -> Jump = 0;
		controls -> Branch = 0;
		controls -> MemRead = 0;
		controls -> MemtoReg = 0;
		controls -> ALUSrc = 1;
		controls -> MemWrite = 0;
		controls -> ALUOp = 3;  /*ALU will do “set less than unsigned” operation*/
        controls -> RegWrite = 1;
		break;

	case 4: // beq (Branch on equal)
		controls -> RegDst = 0;
		controls -> Jump = 0;
		controls -> Branch = 1;
		controls -> MemRead = 0;
		controls -> MemtoReg = 0;
		controls -> ALUSrc = 0;
		controls -> MemWrite = 0;
		controls -> ALUOp = 1;  /*ALU will do subtraction*/
		controls -> RegWrite = 0;
		break;

	case 5: // j (Jump)
		controls -> RegDst = 0;
		controls -> Jump = 1;
		controls -> Branch = 0;
		controls -> MemRead = 0;
		controls -> MemtoReg = 0;
		controls -> ALUSrc = 0;
		controls -> MemWrite = 0;
		controls -> ALUOp = 2;  /*ALU will do addition or “don’t care”*/
		controls -> RegWrite = 0;
		break;

	case 6: // lw (load word)
		controls -> RegDst = 0;
		controls -> Jump = 0;
		controls -> Branch = 0;
		controls -> MemRead = 1;
		controls -> MemtoReg = 1;
		controls -> ALUSrc = 1;
		controls -> MemWrite = 0;
		controls -> ALUOp = 0;  /*ALU will do addition or “don’t care”*/
		controls -> RegWrite = 1;
		break;

	case 7: // lui (load unsigned immediate)
		controls -> RegDst = 0;
		controls -> Jump = 0;
		controls -> Branch = 0;
		controls -> MemRead = 0;
		controls -> MemtoReg = 0;
		controls -> ALUSrc = 1;
		controls -> MemWrite = 0;
		controls -> ALUOp = 6;  /*ALU will shift left extended_value by 16 bits*/
		controls -> RegWrite = 1;
		break;

	case 8: // sw
		controls -> RegDst = 2;
		controls -> Jump = 0;
		controls -> Branch = 0;
		controls -> MemRead = 0;
		controls -> MemtoReg = 2;
		controls -> ALUSrc = 1;
		controls -> MemWrite = 0;
		controls -> ALUOp = 0;  /*ALU will do addition or “don’t care"*/
		controls -> RegWrite = 0;
		break;

    case 9: // sub
        controls -> RegDst = 1;
		controls -> Jump = 0;
		controls -> Branch = 0;
		controls -> MemRead = 0;
		controls -> MemtoReg = 2;
		controls -> ALUSrc = 1;
		controls -> MemWrite = 0;
		controls -> ALUOp = 1;  /*ALU will do subtraction*/
		controls -> RegWrite = 1;
		break;


	default: return 1;
	}
	return 0;
}

/* Read Register */

void read_register(unsigned r1, unsigned r2, unsigned *Reg, unsigned *data1, unsigned *data2) {



	*data1 = Reg[r1]; // assign data1 to register 1
	*data2 = Reg[r2]; // assign data2 to r1egister 2

}

/* Sign Extend */

void sign_extend(unsigned offset, unsigned *extended_value) {



	unsigned extended;
	unsigned extend1s = 0xFFFF0000;
	unsigned isItNegative = offset >> 15;  /* value of left-most bit of immediate operand is copied to all bits to the left (into the high-order bits)*/

	if(isItNegative == 1)

		extended =   extend1s | offset;

	else
		extended = offset;

	*extended_value = extended;

} // end of sign_extend(); 

/* ALU operations */

int ALU_operations(unsigned data1, unsigned data2, unsigned extended_value, unsigned funct, char ALUOp, char ALUSrc, unsigned *ALUresult, char *Zero) {

       //Based on what the ALUSrc is: If it is 1 then we use the immediate value that was sign extended.
    if(ALUSrc == 1)
        data2 = extended_value;


  if(ALUOp == 111)
  {
    //ADD
    if(funct == 32)
    {
        ALUOp = 000;
        ALU(data1, data2, ALUOp, ALUresult, Zero);
    }

    //SUB
    if(funct == 34)
    {
        ALUOp = 001;
        ALU(data1, data2, ALUOp, ALUresult, Zero);
    }

    //AND
    if(funct == 36)
    {
        ALUOp = 100;
        ALU(data1, data2, ALUOp, ALUresult, Zero);
    }

    //OR
    if(funct == 37)
    {
        ALUOp = 101;
        ALU(data1, data2, ALUOp, ALUresult, Zero);
    }

    //slt
    if(funct == 42)
    {
        ALUOp = 010;
        ALU(data1, data2, ALUOp, ALUresult, Zero);
    }

    //sltu
    if(funct == 43)
    {
        ALUOp = 011;
        ALU(data1, data2, ALUOp, ALUresult, Zero);
    }
  }//end r-type
    else if(ALUOp == 000 || ALUOp == 110 || ALUOp == 001 || ALUOp == 010 || ALUOp == 011)
    {
        ALU(data1, data2, ALUOp, ALUresult, Zero);
    }
    else
        return 1;

    return 0;
} // end of ALU_operations(); 


int rw_memory(unsigned ALUresult, unsigned data2, char MemWrite, char MemRead, unsigned *memdata, unsigned *Mem) {

	unsigned LoMem = 0x00000000;
	unsigned HiMem = 0x0000FFFF;

	if (((ALUresult) < LoMem) && ((ALUresult) > HiMem)){

		return 1;
	}else{
		if (MemRead == 1){
			*memdata = Mem[ALUresult >> 2];

		}
		if (MemWrite == 1){
			Mem[ALUresult >> 2] = data2;

		}
		return 0;
	}
} // end of rw_memory(); 

/* Write Register */

void write_register(unsigned r2, unsigned r3, unsigned memdata, unsigned ALUresult, char RegWrite, char RegDst, char MemtoReg, unsigned *Reg) {

	if (MemtoReg == 1 && RegDst == 0 && RegWrite == 1){
		Reg[r2] = memdata;


 	}else if (MemtoReg == 0 && RegDst == 0 && RegWrite == 1){
		Reg[r2] = ALUresult;

	}else if (MemtoReg == 0 && RegDst == 0 && RegWrite == 0){ // for the beq instruction
		Reg[r2] = memdata;

	}if (MemtoReg == 0 && RegDst == 1 && RegWrite == 1){
		Reg[r3] = ALUresult;

	}
} // end of write_register(); 

/* PC update */

void PC_update(unsigned jsec, unsigned extended_value, char Branch, char Jump, char Zero, unsigned *PC) {
	
	*PC +=4;

	switch (Jump){

	case 0:
		if (Zero == 1 && Branch == 1)
			*PC = (*PC) + (extended_value * 4);
		break;
	case 1:
		if (Zero == 1 && Branch == 1){
			*PC = (*PC & 0xF0000000) | (jsec * 4);
			break;
		}
	} // end of switch() 
} // end of PC_Update();  
