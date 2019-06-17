# BakaArch
Silly little emulated computer architecture, just for fun

# Registers:
r0 (general purpose) = 0x0
r1 (general purpose) = 0x1
r2 (general purpose) = 0x2
r3 (general purpose) = 0x3
r4 (general purpose) = 0x4
r5 (general purpose) = 0x5
r6 (general purpose) = 0x6
r7 (general purpose) = 0x7

pc (program counter, stores current address to be executed) = 0x9
cmp (comparison result, stores result of comparison operations) = 0xA
(cmp result values:
0 = result was equal
1 = result was greater than
2 = result was less than
3 = result was not equal
4 = result was not greater than
5 = result was not less than
)

# Opcodes:
(NOP)
0x0 - No operation, don't do anything for the cpu cycle!

(ADD)
0x1 - Add 2 specified 8 bit registers or an integer value into a register, e.x. 0x1(opcode) 0x1 0x2(arguments) (adds the contents of 0x1 with 0x2, the result being stored in 0x1)

(SUB)
0x2 - Subtract 2 specified 8 bit registers or an integer value into a register, e.x. 0x1(opcode) 0x1 0x2(arguments) (subtracts the contents of 0x1 with 0x2, the result being stored in 0x1)

(LD)
0x3 - Load the second argument's register contents or an interger value into the specified register, e.x. 0x3(opcode) 0x1(register argument) 1A(value), (Stores the value 1A in 0x1)

(JMP)
0x4 - Unconditionally jump to the specified address in memory e.x. 0x4(opcode) 0x0000(memory address argument), (Sets PC to 0x0000)

(CMP)
0x5 - Compare the specified register's value with the second argument's register contents or interger value, set appropriate cmp flag, e.x. 0x5(opcode) 0x1(register) 0x5(value), (compares values in 0x1 with 0x5, set flag in cmp)

(JE)
0x6 - Jump to the specified memory address if the value in cmp is 0 (equal), e.x. 0x6(opcode) 0x0000(memory address), (jumps to 0x0000 if cmp is 0)

(JNE)
0x7 - Jump to the specified memory address if the value in cmp is 3 (not equal)

(JGT)
0x8 - Jump to the specified memory address if the value in cmp is 1 (greater than)

(JLT)
0x9 - Jump to the specified memory address if the value in cmp is 2 (less than)

(INC)
0xA - Increment the specified register by 1

(DEINC)
0xB - Deincrement the specified register by 2

(OUT)
0xC - Output specified value or register

(HALT)
0xD - Stop running
