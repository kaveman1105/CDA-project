//
//  project.c
//  Project
//
//  Created by Raikesson W. Charles on 11/18/13.
//  Copyright (c) 2013 Raikesson W. Charles. All rights reserved.
//
#include "spimcore.h"


/* ALU */

void ALU(unsigned A, unsigned B, char ALUControl, unsigned *ALUresult, char *Zero) {
      
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

}

/* instruction fetch */

int instruction_fetch(unsigned PC, unsigned *Mem, unsigned *instruction) {
   
	/*From FAQ - PC has to be shifted right

         by 2 bits in order to be used with this updates the program counter (PC) Mem[] */
   
	if(PC % 4 == 0){  // halt if not word aligned

		*instruction = Mem[PC >> 2]; // get next instruction
           
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
            
            
            case 0xF:// lui
             controls->RegDst = 0;
             controls->Jump = 0;
             controls->Branch = 0;
             controls->MemRead = 0;
             controls->MemtoReg = 0;
             controls->ALUOp = 7;
             controls->MemWrite = 0;
             controls->ALUSrc = 1;
             controls->RegWrite = 1;
             return 0;
             break;
            
        default:
          
            return 1;
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
            return 1;
            break;
    }
    if(ALUSrc)
        ALU(data1,extended_value,ALUControl,ALUresult,Zero);
    
    else
        ALU(data1,data2,ALUControl,ALUresult,Zero);
    
    return 0;
    
    if(*Zero == 1)
        return 1;
    else return 0;
    
    } // end of ALUOp();
    


int rw_memory(unsigned ALUresult, unsigned data2, char MemWrite, char MemRead, unsigned *memdata, unsigned *Mem) {
    if(MemWrite == 1)
    {
        if( ALUresult % 4 != 0 )
        {
           
            return 1;   //Halt, address is not word aligned.
        }
        Mem[ALUresult >> 2] = data2;    //Memory to be written,
    }
    if(MemRead == 1)
    {
        if( ALUresult % 4 != 0 )
        {
            
            return 1;   //Halt, address is not word aligned.
        }
        *memdata = Mem[ALUresult >> 2];
    }
    return 0;
    

/*	if (MemWrite == 1 && MemRead == 0) {
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
    return 0;*/
} // end of rw_memory();

/* Write Register */

void write_register(unsigned r2, unsigned r3, unsigned memdata, unsigned ALUresult, char RegWrite, char RegDst, char MemtoReg, unsigned *Reg) {
      
    unsigned DstReg = r2;   //set destination register to r2 (most likely case)
    if( !(RegWrite == 1) ) //RegWrite = 0
        return;
    if( RegDst == 1 )
        DstReg = r3;
    if( MemtoReg == 1 )
        Reg[DstReg] = memdata;
    else
        Reg[DstReg] = ALUresult;
    
	/*if (MemtoReg == 1 && RegDst == 0 && RegWrite == 1){
		Reg[r2] = memdata;



	}else if (MemtoReg == 0 && RegDst == 0 && RegWrite == 1){
		Reg[r2] = ALUresult;

	}else if (MemtoReg == 0 && RegDst == 0 && RegWrite == 0){ // for the beq instruction
		Reg[r2] = memdata;

	}if (MemtoReg == 0 && RegDst == 1 && RegWrite == 1){
		Reg[r2] = ALUresult;*/

        
        	
} // end of write_register(); 

/* PC update */

void PC_update(unsigned jsec, unsigned extended_value, char Branch, char Jump, char Zero, unsigned *PC) {
	    
    unsigned PC_temp = *PC + 4;
    
    if(Jump == 0)
    {
        if(Branch && Zero == 1)
            *PC =(extended_value * 4) + (*PC + 4);
        
        else
            *PC = *PC + 4;
    }
    else if(Jump == 1)
    {
        PC_temp = PC_temp & 0xF0000000;
        *PC =  (PC_temp | (jsec * 4));
        
    }
    
  /*  if (Jump == 1) {
        jsec = jsec << 2; //shifts jsec left 2
        unsigned temp = *PC >> 2; //gets 4 most significant bits
        temp = temp << 28; //sets up temp to be added to front of jsec
        *PC = temp | jsec; //combines temp and jsec for new PC
    }
    
    else if (Branch == 1 && Zero == 1) {
        *PC = extended_value << 2; //pc gets extended << 2 on branch
    }
    else
    *PC += 4;*/    
    

} // end of PC_Update();  
