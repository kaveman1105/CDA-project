//
//  project.c
//  Project
//
//  Created by Kevin Anderson on 11/14/13.
//  Copyright (c) 2013 Kevin Anderson. All rights reserved.
//
#include "spimcore.h"


/* ALU */

void ALU(unsigned A, unsigned B, char ALUControl, unsigned *ALUresult, char *Zero) {
    printf("\n\n\nALU section\n\n");
	*Zero = 0;
    
    printf("ALUControl = %d\n", ALUControl);

	switch (ALUControl){

	case 0:

		*ALUresult = A + B;
            printf("ALUresult =  A + B\n\t=%u\n", *ALUresult);

		if(*ALUresult == 0)
			*Zero = 1;

		break;

	case 1:

		*ALUresult = A - B;
            printf("ALUresult =  A - B\n\t=%u\n", *ALUresult);

		if( *ALUresult == 0 )
			*Zero = 1;

		break;

	case 2:

		if ( A < B)
			*ALUresult = 1;
		else
			*ALUresult = 0;

		if ( *ALUresult == 0 )

			*Zero = 1;
            printf("ALUresult for A < B\n\t=%u\n", *ALUresult);
		break;

	case 3:

		if ( A > B ){

			*ALUresult = 0;

		}

		else

			*ALUresult = 1;

		if ( *ALUresult == 0 )

			*Zero = 1;
            printf("ALUresult for A > B\n\t=%u\n", *ALUresult);
		break;

	case 4:

		*ALUresult = A & B; // Bitwise AND

		if ( *ALUresult == 0 )

			*Zero = 1;
            printf("ALUresult for A & B\n\t=%u\n", *ALUresult);
		break;

	case 5:

		*ALUresult = A | B; // Bitwise OR

		if ( *ALUresult == 0 )

			*Zero = 1;
            printf("ALUresult for A | B\n\t=%u\n", *ALUresult);
		break;

	case 6:

		*ALUresult = B << 16;

		if(*ALUresult == 0)

			*Zero = 1;
            printf("ALUresult for B << 16\n\t=%u\n", *ALUresult);
		break;

	case 7:

		ALUControl = !A;    //logical not

		if(*ALUresult == 0)

			*Zero = 1;
            printf("ALUresult for !A\n\t=%u\n", *ALUresult);
		break;

	}


}

/* instruction fetch */

int instruction_fetch(unsigned PC, unsigned *Mem, unsigned *instruction) {
    printf("\n\n\ninstruction fetch section\n\n");
	/*From FAQ - PC has to be shifted right

         by 2 bits in order to be used with this updates the program counter (PC) Mem[] */
    printf("PC before >> 2 = %u\n", PC);

	if(PC % 4 == 0){  // halt if not word aligned

		*instruction = Mem[PC >> 2]; // get next instruction
            printf("instruction recieved from mem  = %u\n", *instruction);

		return 0;

	}

	else

		return 1;

}

/* instruction partition */

void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1, unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec) {
    printf("\n\n\ninstruction partition section\n\n");
        unsigned for_r = 0x0000001F;
        unsigned for_f = 0x0000003F;
        unsigned for_o = 0x0000FFFF;
        unsigned for_j = 0x03FFFFFF;
    printf("op before = %u\n", *op);
        *op = instruction >> 26;            // instruction [31-26]
    printf("op after = %u\n\n", *op);
    
    printf("r1 before = %u\n", *r1);
        *r1 = instruction >> 21 & for_r;	// instruction [25-21]
    printf("r1 after = %u\n\n", *r1);
    
    printf("r2 before = %u\n", *r2);
        *r2 = instruction >> 16 & for_r;    // instruction [20-16]
    printf("r2 after = %u\n\n", *r2);
    
    printf("r3 before = %u\n", *r3);
        *r3 = instruction >> 11 & for_r;	// instruction [15-11]
    printf("r3 after = %u\n\n", *r3);
    
    printf("funct before = %u\n", *funct);
        *funct = instruction & for_f;	    // instruction [5-0]
    printf("funct after = %u\n\n", *funct);
    
    printf("offset before = %u\n", *offset);
        *offset = instruction & for_o;	    // instruction [15-0]
    printf("offset after = %u\n\n", *offset);
    
    printf("jsec before = %u\n", *jsec);
        *jsec =	instruction & for_j;        // instruction [25-0]
    printf("jsec after = %u\n", *jsec);

}

/* instruction decode */

int instruction_decode(unsigned op, struct_controls *controls) {

	/*The value set for the ALUOp of 0-7 represents the binary value of the R-Type Instruction; refer to final project document page 4 */
    printf("\n\n\ninstruction decode sction\n\n");
    printf("op = %u", op);
	switch (op){
        case 1: // Add
            controls -> RegDst = 1;
            controls -> Jump = 0;
            controls -> Branch = 0;
            controls -> MemRead = 0;
            controls -> MemtoReg = 0;
            controls -> ALUSrc = 0;
            controls -> MemWrite = 0;
            controls -> ALUOp = 0;
            controls -> RegWrite = 1;
            break;
            
        case 2: // sub
            controls -> RegDst = 1;
            controls -> Jump = 0;
            controls -> Branch = 0;
            controls -> MemRead = 0;
            controls -> MemtoReg = 0;
            controls -> ALUSrc = 0;
            controls -> MemWrite = 0;
            controls -> ALUOp = 1;
            controls -> RegWrite = 1;
            break;
            
        case 3: // slt
            controls -> RegDst = 1;
            controls -> Jump = 0;
            controls -> Branch = 0;
            controls -> MemRead = 0;
            controls -> MemtoReg = 0;
            controls -> ALUSrc = 0;
            controls -> MemWrite = 0;
            controls -> ALUOp = 2;
            controls -> RegWrite = 1;
            break;
            
        case 4:// sltu                      <-- need to fix this
            controls -> RegDst = 1;
            controls -> Jump = 0;
            controls -> Branch = 0;
            controls -> MemRead = 0;
            controls -> MemtoReg = 0;
            controls -> ALUSrc = 0;
            controls -> MemWrite = 0;
            controls -> ALUOp = 3;
            controls -> RegWrite = 1;
            break;
            
        case 5: // AND
            controls -> RegDst = 1;
            controls -> Jump = 0;
            controls -> Branch = 0;
            controls -> MemRead = 0;
            controls -> MemtoReg = 0;
            controls -> ALUSrc = 0;
            controls -> MemWrite = 0;
            controls -> ALUOp = 4;
            controls -> RegWrite = 1;
            break;
            
        case 6: // OR
            controls -> RegDst = 1;
            controls -> Jump = 0;
            controls -> Branch = 0;
            controls -> MemRead = 0;
            controls -> MemtoReg = 0;
            controls -> ALUSrc = 0;
            controls -> MemWrite = 0;
            controls -> ALUOp = 5;
            controls -> RegWrite = 1;
            break;
            
        case 7: // sll                      <-- need to fix this one too
            controls -> RegDst = 1;
            controls -> Jump = 0;
            controls -> Branch = 0;
            controls -> MemRead = 0;
            controls -> MemtoReg = 0;
            controls -> ALUSrc = 0;
            controls -> MemWrite = 0;
            controls -> ALUOp = 6;
            controls -> RegWrite = 1;
            break;
            
        case 8: // R-type                      <-- need to be fixed
            controls -> RegDst = 1;
            controls -> Jump = 0;
            controls -> Branch = 0;
            controls -> MemRead = 0;
            controls -> MemtoReg = 0;
            controls -> ALUSrc = 0;
            controls -> MemWrite = 0;
            controls -> ALUOp = 0;
            controls -> RegWrite = 1;
            break;
            
        /*case 0: // R-Type instructions (and, add, slt, sltu)
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
            controls -> ALUOp = 0; ALU will do addition or “don’t care”
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
            controls -> ALUOp = 2;  ALU will do “set less than” operation
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
            controls -> ALUOp = 3;  ALU will do “set less than unsigned” operation
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
            controls -> ALUOp = 1;  ALU will do subtraction
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
            controls -> ALUOp = 2;  ALU will do addition or “don’t care”
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
            controls -> ALUOp = 0;  ALU will do addition or “don’t care”
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
            controls -> ALUOp = 6;  ALU will shift left extended_value by 16 bits
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
            controls -> ALUOp = 0;  ALU will do addition or “don’t care"
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
            controls -> ALUOp = 1;  ALU will do subtraction
            controls -> RegWrite = 1;
            break;*/
            
            
        default: return 1;
	}
	return 0;
}

/* Read Register */

void read_register(unsigned r1, unsigned r2, unsigned *Reg, unsigned *data1, unsigned *data2) {
    printf("\n\n\nread register section\n\n");

    printf("r1 = %u\n", r1);
    printf("r2 = %u\n", r2);
    printf("Reg = %u\n", *Reg);
    printf("data1 before read = %u\n", *data1);
    printf("data2 before read = %u\n", *data2);
    
	*data1 = Reg[r1]; // assign data1 to register 1
    printf("data1 = Reg[r1]  = %u\n", *data1);
    
    
	*data2 = Reg[r2]; // assign data2 to r1egister 2
    printf("data2 = Reg[r2] = %u\n", *data2);
}

/* Sign Extend */

void sign_extend(unsigned offset, unsigned *extended_value) {

    printf("\n\n\nsign extend section\n\n");

	unsigned extended;
	unsigned extend1s = 0xFFFF0000;
	unsigned isItNegative = offset >> 15;  /* value of left-most bit of immediate operand is copied to all bits to the left (into the high-order bits)*/
    printf("sign bit = %u\n", isItNegative);
    
	if(isItNegative == 1)

		extended =   extend1s | offset;

	else
		extended = offset;

	*extended_value = extended;

} // end of sign_extend(); 

/* ALU operations */

int ALU_operations(unsigned data1, unsigned data2, unsigned extended_value, unsigned funct, char ALUOp, char ALUSrc, unsigned *ALUresult, char *Zero) {
    
    printf("\n\n\nALU op section\n\n");
    printf("ALU op = %d\n", ALUOp);
    printf("ALU result = %u\n", *ALUresult);
    printf("ALU src = %d\n", ALUSrc);
    printf("funct = %u\n", funct);
    printf("data1 = %u\n", data1);
    printf("data2 = %u\n", data2);
    printf("extend value = %u\n", extended_value);
    
    if (ALUSrc == 1)
        data2 = extended_value;
    
    
          // R-TYPE
            /* For R-TYPE op do ALU() based off funct
             * 0 = add
             * 1 = sub
             * 2 = set less than
             * 3 = set less than unsigned
             * 4 = AND
             * 5 = OR
             * 6 = shift left 16
             * 7 = NOT A
             */
    
    ALU(data1, data2, funct, ALUresult, Zero);
    
    if(*Zero == 1)
        return 1;
    else return 0;
    
    } // end of ALUOp();
    
 /*      //Based on what the ALUSrc is: If it is 1 then we use the immediate value that was sign extended.
    if(ALUSrc == 1)
        data2 = extended_value;


  if(ALUOp == 7)
  {
    //ADD
    if(funct == 4)
    {
        ALUOp = 0;
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
*/

 // end of ALU_operations();


int rw_memory(unsigned ALUresult, unsigned data2, char MemWrite, char MemRead, unsigned *memdata, unsigned *Mem) {
    printf("\n\n\nrw memory section\n\n");
	unsigned LoMem = 0x00000000;
	unsigned HiMem = 0x0000FFFF;
    unsigned check = ALUresult >> 2;
    
    //check alignment
    if (check % 4 != 0) {
        return 1;
    }

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
    printf("\n\n\nwrite register section\n\n");
    printf("r2 = %u\n", r2);
    printf("r3 = %u\n", r3);
    printf("memdata = %u\n", memdata);
    printf("ALU result = %u\n", ALUresult);
    printf("Regwrite = %d\n", RegWrite);
    printf("RegDst = %d\n", RegDst);
    printf("MemtoReg = %d\n", MemtoReg);
    printf("Reg = %u\n", *Reg);
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
	printf("\n\n\nPc update section\n\n");
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
