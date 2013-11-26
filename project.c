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
    printf("A = %u\n", A);
    printf("B = %u\n", B);
    printf("Zero = %u\n", *Zero);
    printf("ALU Result before = %u\n", *ALUresult);
	
    
    printf("ALUControl = %d\n", ALUControl);

    int temp1, temp2, tempA, tempB;
    switch(ALUControl){
        case 0: //000 Z = A + B
            *ALUresult = A + B;//may not have *
            break;
            
        case 1: //001 Z = A - B
            *ALUresult = A - B;
            //if (*ALUresult < 0)
            //    *ALUresult = *ALUresult * (-1);
            break;
            
        case 2: //010 if A < B, Z = 1; otherwise, Z = 0
            temp1 = 1;
            while (A > temp1){
                temp2 = temp1;
                temp1 = temp1 * 2;
            }
            
            if((temp1 - A) < temp2)//it is negative
                tempA = (A - temp2)*(-1);
            else
                tempA = A;
            
            temp1 = 1;
            while (B > temp1){
                temp2 = temp1;
                temp1 = temp1 * 2;
            }
            
            if((temp1 - B) < temp2)//it is negative
                tempB = (B - temp2)*(-1);
            else
                tempB = B;
            
            if (tempA < tempB){
                *ALUresult = 1;
                *Zero = 0;
            }
            
            else
                *ALUresult = 0;
            break;
            
        case 3: //011 if A < B, Z = 1; otherwise, Z = 0 (A and B are unsigned integers)
            
            if (A < B)
                *ALUresult = 1;
            else
                *ALUresult = 0;
            break;
            
        case 4: //100 Z = A AND B
            *ALUresult = A & B;
            
        case 5: //101 Z = A OR B
            *ALUresult = A | B;
            
        case 6: //110 Shift left B by 16 bits
            *ALUresult = B * 65536;// 65536 = (4^8)
            
        case 7: //111 Z = A NOR B
            *ALUresult = ~(A | B);//not sure if this is right
            break;
            
            if(*ALUresult == 0)
                *Zero = 1;
            else
                *Zero = 0;
            return;
    }

    printf("\n\nA after = %u\n", A);
    printf("B after = %u\n", B);
    printf("Zero after = %u\n", *Zero);
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
    switch(op)
    {
        case 0x0://add, sub, and, or, slt, sltu
            controls->RegDst = 1;//type R == 1, type I == 0
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 7;
            controls->MemWrite = 0;//may be don't care
            controls->ALUSrc = 0;//type R == 0, type I and branching == 1
            controls->RegWrite = 1;
            return 0;
            
            
        case 0x8://addi
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 0;
            controls->MemWrite = 0;//may be don't care
            controls->ALUSrc = 1;//type R == 0, type I and branching == 1
            controls->RegWrite = 1;
            return 0;
            
            
        case 0xA://slti
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 2;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            return 0;
            
            
        case 0xB://sltiu
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 3;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            return 0;
            
            
            
            
        case 0x4://beq
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 1;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 1;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 0;
            return 0;
            
            
        case 0x23://lw
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 1;
            controls->MemtoReg = 1;
            controls->ALUOp = 0;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            return 0;
            
            
        case 0x2B://sw
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 0;
            controls->MemWrite = 1;
            controls->ALUSrc = 1;
            controls->RegWrite = 0;
            return 0;
            
            
        case 0x2://jump
            controls->RegDst = 2;
            controls->Jump = 1;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 0;
            controls->MemWrite = 0;
            controls->ALUSrc = 2;
            controls->RegWrite = 0;
            return 0;
            
            
            /*case 6://
             controls->RegDst = 0;
             controls->Jump = 0;
             controls->Branch = 0;
             controls->MemRead = 0;
             controls->MemtoReg = 0;
             controls->ALUOp = 6;
             controls->MemWrite = 0;
             controls->ALUSrc = 1;
             controls->RegWrite = 0;
             return 0;
             break;*/
            
        default:
            printf("instruction decode");
            return 1;
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
    printf("Zero = %d", *Zero);
    
    char ALUControl = ALUOp;
    if(ALUOp < 0 || ALUOp >7)
        return 1;
    if(ALUOp == 7) //r-type
        switch(funct)
    {
        case 0x20: //add
            ALUControl = 0;
            break;
        case 0x22: //subtract
            ALUControl = 1;
            break;
        case 0x24: //and
            ALUControl = 4;
            break;
        case 0x25: //or
            ALUControl = 5;
            break;
        case 0x27: //nor
            ALUControl = 7;
            break;
        case 0x2A: //slt
            ALUControl = 2;
            break;
        case 0x2B: //sltu
            ALUControl = 3;
            break;
        default:
            printf("ALU_operations type r");
            return 1;
            break;
    }
    if(ALUSrc)
        ALU(data1,extended_value,ALUControl,ALUresult,Zero);
    
    else
        ALU(data1,data2,ALUControl,ALUresult,Zero);
    
    return 0;
    
    printf("\nALU Op after ALU\n\n");

    printf("ALU op after = %d\n", ALUOp);
    printf("ALU result after  = %u\n", *ALUresult);
    printf("ALU src after  = %d\n", ALUSrc);
    printf("funct after  = %u\n", funct);
    printf("data1 after  = %u\n", data1);
    printf("data2 after  = %u\n", data2);
    printf("extend value after  = %u\n", extended_value);
    printf("Zero after = %d", *Zero);
    
    if(*Zero == 1)
        return 1;
    else return 0;
    
    } // end of ALUOp();
    


int rw_memory(unsigned ALUresult, unsigned data2, char MemWrite, char MemRead, unsigned *memdata, unsigned *Mem) {
    printf("\n\n\nrw memory section\n\n");

    
    printf("ALU result = %d\n", ALUresult);
    printf("Data2 = %u\n", data2);
    printf("Mem write = %d\n", MemWrite);
    printf("Mem read = %d\n", MemRead);
    printf("mem data = %u\n", *memdata);
    printf("Mem[data2] = %u\n", Mem[data2]);

    
    //check alignment
    if ((ALUresult >> 2) % 4 != 0) {
        return 1;
    }

	if (MemWrite == 1 && MemRead == 0) {
        Mem[data2] = ALUresult;
        printf("\n\nALU result after = %d\n", ALUresult);
        printf("Data2 after = %u\n", data2);
        printf("Mem write after = %d\n", MemWrite);
        printf("Mem read after = %d\n", MemRead);
        printf("mem data after = %u\n", *memdata);
        printf("Mem[data2] after = %u\n", Mem[data2]);
        return 0;
    }
    if (MemWrite == 0 && MemRead == 1) {
        *memdata = ALUresult;
        printf("\n\nALU result after = %d\n", ALUresult);
        printf("Data2 after = %u\n", data2);
        printf("Mem write after = %d\n", MemWrite);
        printf("Mem read after = %d\n", MemRead);
        printf("mem data after = %u\n", *memdata);
        printf("Mem[data2] after = %u\n", Mem[data2]);
        return 0;
    }
    if (MemWrite == 1 && MemRead == 1) {
        *memdata = ALUresult;
        Mem[data2] = ALUresult;
        printf("\n\nALU result after = %d\n", ALUresult);
        printf("Data2 after = %u\n", data2);
        printf("Mem write after = %d\n", MemWrite);
        printf("Mem read after = %d\n", MemRead);
        printf("mem data after = %u\n", *memdata);
        printf("Mem[data2] after = %u\n", Mem[data2]);
        return 0;
    }
    return 0;
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
    printf("Reg[r2] = %u\n", Reg[r2]);
    printf("Reg[r3] = %u\n", Reg[r3]);
    
    
	if (MemtoReg == 1 && RegDst == 0 && RegWrite == 1){
		Reg[r2] = memdata;



	}else if (MemtoReg == 0 && RegDst == 0 && RegWrite == 1){
		Reg[r2] = ALUresult;

	}else if (MemtoReg == 0 && RegDst == 0 && RegWrite == 0){ // for the beq instruction
		Reg[r2] = memdata;

	}if (MemtoReg == 0 && RegDst == 1 && RegWrite == 1){
		Reg[r2] = ALUresult;

        
        printf("\n\nr2 after = %u\n", r2);
        printf("r3 = after %u\n", r3);
        printf("memdata after = %u\n", memdata);
        printf("ALU result after = %u\n", ALUresult);
        printf("Regwrite after = %d\n", RegWrite);
        printf("RegDst after = %d\n", RegDst);
        printf("MemtoReg after = %d\n", MemtoReg);
        printf("Reg after = %u\n", *Reg);
        printf("Reg[r2] after = %u\n", Reg[r2]);
        printf("Reg[r3] after = %u\n", Reg[r3]);

        
	}
} // end of write_register(); 

/* PC update */

void PC_update(unsigned jsec, unsigned extended_value, char Branch, char Jump, char Zero, unsigned *PC) {
	printf("\n\n\nPc update section\n\n");
    
    printf("jsec = %u\n", jsec);
    printf("extended value = %u\n", extended_value);
    printf("Branch = %d\n", Branch);
    printf("Jump = %d\n", Jump);
    printf("Zero = %u\n", Zero);
    printf("PC = %u\n", *PC);
    
	
    
    if (Jump == 1) {
        jsec = jsec << 2; //shifts jsec left 2
        unsigned temp = *PC >> 2; //gets 4 most significant bits
        temp = temp << 28; //sets up temp to be added to front of jsec
        *PC = temp | jsec; //combines temp and jsec for new PC
    }
    
    else if (Branch == 1 && Zero == 1) {
        *PC = extended_value << 2; //pc gets extended << 2 on branch
    }
    else
    *PC += 4;
    
    
    
    printf("\n\njsec after = %u\n", jsec);
    printf("extended value after = %u\n", extended_value);
    printf("Branch after = %d\n", Branch);
    printf("Jump after = %d\n", Jump);
    printf("Zero after = %u\n", Zero);
    printf("PC after = %u\n", *PC);
    
    

} // end of PC_Update();  
