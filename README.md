# BakaArch
Silly little emulated computer architecture, just for fun

# Registers:
General purpose registers are represented as two succesive bytes, starting with 0xFF, then the register number

(e.g., 0xFF, 0x0 = r0)

r0 (general purpose) = 0x0

r1 (general purpose) = 0x1

r2 (general purpose) = 0x2

r3 (general purpose) = 0x3

r4 (general purpose) = 0x4

r5 (general purpose) = 0x5

r6 (general purpose) = 0x6

pc (program counter, stores current address to be executed, non addressable)

cmp (comparison result, stores result of comparison operations, non addressable)

cmp result values:

0 = result was equal

1 = result was greater than

2 = result was less than

3 = result was not equal

4 = result was not greater than

5 = result was not less than

# Opcodes:
Values can refer to either a raw value (like 0x1) or a register's value

(NOP)
0x0 - No operation, don't do anything for the cpu cycle!

(ADD, 2 operands)
0x1 - Add the specified value into the specified register

e.x: 0x1(opcode) 0xFF 0x1(register) 0x2(value) (add the contents of r1 with 2, the result being stored in r1)

add r1, 2

(SUB, 2 operands)
0x2 - Subtract the specified register by the specified value 

e.x. 0x2(opcode) 0xFF 0x1(register) 0x2(value) (subtracts the contents of r1 by 2, the result being stored in r1)

sub r1, 2

(LD, 2 operands)
0x3 - Load the specified value into the specified register 

e.x. 0x3(opcode) 0xFF 0x1(register) 0xa(value) (Stores 10 in r1)

ld r1, 10

(JMP, 1 operand)
0x4 - Unconditionally jump to the specified address in memory 

e.x. 0x4(opcode) 0x0000(memory address), (Sets PC to 0x0000, causing that memory address to execute on the next cycle)

jmp 0x0000

(CMP, 2 operands)
0x5 - Compare the specified register with the specified value, set appropriate cmp flag (See: cmp result values)

e.x. 0x5(opcode) 0xFF 0x1(register) 0xC(value) (Compare the contents of r1 with 12, set cmp flag)

cmp r1, 12

(JE, 1 operand)
0x6 - Jump to the specified memory address if the value in cmp is 0 (equal)

e.x. 0x6(opcode) 0x0000(memory address) (jumps to 0x0000 if cmp is 0)

je 0x0000

(JNE, 1 operand)

0x7 - Jump to the specified memory address if the value in cmp is 3 (not equal)

(JGT)

0x8 - Jump to the specified memory address if the value in cmp is 1 (greater than)

(JLT)

0x9 - Jump to the specified memory address if the value in cmp is 2 (less than)

(INC)

0xA - Increment the specified register by 1

(DEINC)

0xB - Deincrement the specified register by 2

(PRINT)

0xC - Print the specified value or register

(HALT)

0xD - Stop running